/*
 * db_ops.c
 * Unified MySQL connection engine + full CRUD for all 7 entities.
 * Compile:  gcc -o library db_ops.c cli_main.c $(mysql_config --cflags --libs)
 */

#include "db_ops.h"


MYSQL *db_connect(void) {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "[db_connect] mysql_init() failed: out of memory.\n");
        return NULL;
    }

    my_bool reconnect = 1;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    if (!mysql_real_connect(conn,
                            DB_HOST, DB_USER, DB_PASS,
                            DB_NAME, DB_PORT,
                            DB_SOCK, DB_FLAGS)) {
        fprintf(stderr, "[db_connect] Connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    /* Enforce FK constraints for this session */
    if (mysql_query(conn, "SET FOREIGN_KEY_CHECKS = 1;") != 0) {
        fprintf(stderr, "[db_connect] Warning: could not set FK checks: %s\n",
                mysql_error(conn));
    }

    return conn;
}

void db_disconnect(MYSQL *conn) {
    if (conn) {
        mysql_close(conn);
    }
}


void db_print_error(const char *context, MYSQL *conn) {
    fprintf(stderr, "[%s] MySQL error %u: %s\n",
            context, mysql_errno(conn), mysql_error(conn));
}

/*
 * Sanitise a user string for safe interpolation.
 * `out` must be at least 2*len+1 bytes.
 */
static void escape(MYSQL *conn, char *out, const char *in, size_t len) {
    mysql_real_escape_string(conn, out, in, (unsigned long)strnlen(in, len));
}

int db_foreign_key_exists(MYSQL *conn, const char *table,
                          const char *pk_col, int id) {
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT 1 FROM `%s` WHERE `%s` = %d LIMIT 1;",
             table, pk_col, id);
    if (mysql_query(conn, query) != 0) return 0;
    MYSQL_RES *res = mysql_store_result(conn);
    int exists = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    return exists;
}


int db_book_create(MYSQL *conn, const Book *book) {
    if (!conn || !book) return DB_ERR_PARAM;

    /* Validate foreign keys before touching the DB */
    if (!db_foreign_key_exists(conn, "Authors", "author_id", book->author_id)) {
        fprintf(stderr, "[db_book_create] author_id %d does not exist.\n",
                book->author_id);
        return DB_ERR_CONSTRAINT;
    }
    if (!db_foreign_key_exists(conn, "Publishers", "publisher_id",
                               book->publisher_id)) {
        fprintf(stderr, "[db_book_create] publisher_id %d does not exist.\n",
                book->publisher_id);
        return DB_ERR_CONSTRAINT;
    }

    char e_title[512], e_isbn[66], e_genre[130], e_shelf[66];
    escape(conn, e_title, book->title,          sizeof(book->title) - 1);
    escape(conn, e_isbn,  book->isbn,           sizeof(book->isbn)  - 1);
    escape(conn, e_genre, book->genre,          sizeof(book->genre) - 1);
    escape(conn, e_shelf, book->shelf_location, sizeof(book->shelf_location) - 1);

    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO Books "
             "(title, author_id, publisher_id, isbn, genre, "
             " year_published, copies_available, shelf_location) "
             "VALUES ('%s', %d, %d, '%s', '%s', %d, %d, '%s');",
             e_title, book->author_id, book->publisher_id,
             e_isbn, e_genre,
             book->year_published, book->copies_available, e_shelf);

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_book_create", conn);
        return DB_ERR_QUERY;
    }
    return DB_OK;
}

int db_book_read_by_id(MYSQL *conn, int book_id, Book *out) {
    if (!conn || !out) return DB_ERR_PARAM;

    char query[256];
    snprintf(query, sizeof(query),
             "SELECT book_id, title, author_id, publisher_id, isbn, genre, "
             "year_published, copies_available, shelf_location "
             "FROM Books WHERE book_id = %d LIMIT 1;", book_id);

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_book_read_by_id", conn);
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) { db_print_error("db_book_read_by_id:store", conn); return DB_ERR_QUERY; }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }

    out->book_id          = atoi(row[0]);
    strncpy(out->title,          row[1] ? row[1] : "", sizeof(out->title) - 1);
    out->author_id        = atoi(row[2]);
    out->publisher_id     = atoi(row[3]);
    strncpy(out->isbn,           row[4] ? row[4] : "", sizeof(out->isbn) - 1);
    strncpy(out->genre,          row[5] ? row[5] : "", sizeof(out->genre) - 1);
    out->year_published   = atoi(row[6]);
    out->copies_available = atoi(row[7]);
    strncpy(out->shelf_location, row[8] ? row[8] : "",
            sizeof(out->shelf_location) - 1);

    mysql_free_result(res);
    return DB_OK;
}

int db_book_read_all(MYSQL *conn, Book **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;

    const char *query =
        "SELECT book_id, title, author_id, publisher_id, isbn, genre, "
        "year_published, copies_available, shelf_location FROM Books "
        "ORDER BY book_id;";

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_book_read_all", conn);
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) { db_print_error("db_book_read_all:store", conn); return DB_ERR_QUERY; }

    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;

    if (n == 0) {
        *out_array = NULL;
        mysql_free_result(res);
        return DB_OK;
    }

    *out_array = (Book *)calloc((size_t)n, sizeof(Book));
    if (!*out_array) {
        mysql_free_result(res);
        return DB_ERR_PARAM;
    }

    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].book_id          = atoi(row[0]);
        strncpy((*out_array)[i].title,          row[1] ? row[1] : "",
                sizeof((*out_array)[i].title) - 1);
        (*out_array)[i].author_id        = atoi(row[2]);
        (*out_array)[i].publisher_id     = atoi(row[3]);
        strncpy((*out_array)[i].isbn,           row[4] ? row[4] : "",
                sizeof((*out_array)[i].isbn) - 1);
        strncpy((*out_array)[i].genre,          row[5] ? row[5] : "",
                sizeof((*out_array)[i].genre) - 1);
        (*out_array)[i].year_published   = atoi(row[6]);
        (*out_array)[i].copies_available = atoi(row[7]);
        strncpy((*out_array)[i].shelf_location, row[8] ? row[8] : "",
                sizeof((*out_array)[i].shelf_location) - 1);
        i++;
    }

    mysql_free_result(res);
    return DB_OK;
}

int db_book_update(MYSQL *conn, const Book *book) {
    if (!conn || !book) return DB_ERR_PARAM;

    if (!db_foreign_key_exists(conn, "Authors", "author_id", book->author_id)) {
        fprintf(stderr, "[db_book_update] author_id %d does not exist.\n",
                book->author_id);
        return DB_ERR_CONSTRAINT;
    }
    if (!db_foreign_key_exists(conn, "Publishers", "publisher_id",
                               book->publisher_id)) {
        fprintf(stderr, "[db_book_update] publisher_id %d does not exist.\n",
                book->publisher_id);
        return DB_ERR_CONSTRAINT;
    }

    char e_title[512], e_isbn[66], e_genre[130], e_shelf[66];
    escape(conn, e_title, book->title,          sizeof(book->title) - 1);
    escape(conn, e_isbn,  book->isbn,           sizeof(book->isbn)  - 1);
    escape(conn, e_genre, book->genre,          sizeof(book->genre) - 1);
    escape(conn, e_shelf, book->shelf_location, sizeof(book->shelf_location) - 1);

    char query[1024];
    snprintf(query, sizeof(query),
             "UPDATE Books SET "
             "title='%s', author_id=%d, publisher_id=%d, isbn='%s', "
             "genre='%s', year_published=%d, copies_available=%d, "
             "shelf_location='%s' "
             "WHERE book_id=%d;",
             e_title, book->author_id, book->publisher_id,
             e_isbn, e_genre,
             book->year_published, book->copies_available,
             e_shelf, book->book_id);

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_book_update", conn);
        return DB_ERR_QUERY;
    }

    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_book_delete(MYSQL *conn, int book_id) {
    if (!conn) return DB_ERR_PARAM;

    /* Prevent deletion if active (unreturned) borrowings exist */
    char check[256];
    snprintf(check, sizeof(check),
             "SELECT 1 FROM Borrowings "
             "WHERE book_id=%d AND return_date IS NULL LIMIT 1;", book_id);
    if (mysql_query(conn, check) != 0) {
        db_print_error("db_book_delete:check", conn);
        return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    int active = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    if (active) {
        fprintf(stderr,
                "[db_book_delete] Cannot delete book %d: active borrowings exist.\n",
                book_id);
        return DB_ERR_CONSTRAINT;
    }

    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Books WHERE book_id=%d;", book_id);
    if (mysql_query(conn, query) != 0) {
        db_print_error("db_book_delete", conn);
        return DB_ERR_QUERY;
    }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_book_free_array(Book *array) {
    free(array);
}

/* ═══════════════════════════════════════════════════════
   §4  BORROWINGS CRUD  (atomic borrow / return)
═══════════════════════════════════════════════════════ */

/*
 * db_borrowing_create
 * ─────────────────────────────────────────────────────
 * Atomically:
 *   1. Verify book / member / staff FK existence.
 *   2. Check copies_available > 0.
 *   3. Decrement copies_available by 1.
 *   4. Insert Borrowings row.
 * All four steps run inside a single transaction; any
 * failure rolls the whole thing back.
 */
int db_borrowing_create(MYSQL *conn, const Borrowing *b) {
    if (!conn || !b) return DB_ERR_PARAM;

    /* Pre-flight FK checks */
    if (!db_foreign_key_exists(conn, "Books",   "book_id",   b->book_id)) {
        fprintf(stderr, "[db_borrowing_create] book_id %d not found.\n", b->book_id);
        return DB_ERR_CONSTRAINT;
    }
    if (!db_foreign_key_exists(conn, "Members", "member_id", b->member_id)) {
        fprintf(stderr, "[db_borrowing_create] member_id %d not found.\n",
                b->member_id);
        return DB_ERR_CONSTRAINT;
    }
    if (!db_foreign_key_exists(conn, "Staff",   "staff_id",  b->staff_id)) {
        fprintf(stderr, "[db_borrowing_create] staff_id %d not found.\n", b->staff_id);
        return DB_ERR_CONSTRAINT;
    }

    /* ── BEGIN transaction ── */
    if (mysql_query(conn, "START TRANSACTION;") != 0) {
        db_print_error("db_borrowing_create:START TRANSACTION", conn);
        return DB_ERR_QUERY;
    }

    /* Step 1: Lock + read copies_available (SELECT ... FOR UPDATE) */
    char check_copies[256];
    snprintf(check_copies, sizeof(check_copies),
             "SELECT copies_available FROM Books "
             "WHERE book_id=%d FOR UPDATE;", b->book_id);

    if (mysql_query(conn, check_copies) != 0) {
        db_print_error("db_borrowing_create:check_copies", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) {
        db_print_error("db_borrowing_create:store_copies", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    int copies = row ? atoi(row[0]) : 0;
    mysql_free_result(res);

    if (copies <= 0) {
        fprintf(stderr,
                "[db_borrowing_create] No copies available for book_id %d.\n",
                b->book_id);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_CONSTRAINT;
    }

    /* Step 2: Decrement copies_available */
    char decrement[128];
    snprintf(decrement, sizeof(decrement),
             "UPDATE Books SET copies_available = copies_available - 1 "
             "WHERE book_id = %d;", b->book_id);

    if (mysql_query(conn, decrement) != 0) {
        db_print_error("db_borrowing_create:decrement", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    /* Step 3: Escape date strings */
    char e_borrow[34], e_due[34];
    escape(conn, e_borrow, b->borrow_date, sizeof(b->borrow_date) - 1);
    escape(conn, e_due,    b->due_date,    sizeof(b->due_date)    - 1);

    /* Step 4: Insert Borrowings row */
    char insert[512];
    snprintf(insert, sizeof(insert),
             "INSERT INTO Borrowings "
             "(book_id, member_id, borrow_date, due_date, return_date, staff_id) "
             "VALUES (%d, %d, '%s', '%s', NULL, %d);",
             b->book_id, b->member_id, e_borrow, e_due, b->staff_id);

    if (mysql_query(conn, insert) != 0) {
        db_print_error("db_borrowing_create:insert", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    /* ── COMMIT ── */
    if (mysql_query(conn, "COMMIT;") != 0) {
        db_print_error("db_borrowing_create:COMMIT", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    return DB_OK;
}

int db_borrowing_read_by_id(MYSQL *conn, int borrowing_id, Borrowing *out) {
    if (!conn || !out) return DB_ERR_PARAM;

    char query[512];
    snprintf(query, sizeof(query),
             "SELECT borrowing_id, book_id, member_id, borrow_date, "
             "due_date, IFNULL(return_date,''), staff_id "
             "FROM Borrowings WHERE borrowing_id=%d LIMIT 1;", borrowing_id);

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_borrowing_read_by_id", conn);
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) { db_print_error("db_borrowing_read_by_id:store", conn); return DB_ERR_QUERY; }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }

    out->borrowing_id = atoi(row[0]);
    out->book_id      = atoi(row[1]);
    out->member_id    = atoi(row[2]);
    strncpy(out->borrow_date,  row[3] ? row[3] : "", sizeof(out->borrow_date)  - 1);
    strncpy(out->due_date,     row[4] ? row[4] : "", sizeof(out->due_date)     - 1);
    strncpy(out->return_date,  row[5] ? row[5] : "", sizeof(out->return_date)  - 1);
    out->staff_id     = atoi(row[6]);

    mysql_free_result(res);
    return DB_OK;
}

int db_borrowing_read_all(MYSQL *conn, Borrowing **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;

    const char *query =
        "SELECT borrowing_id, book_id, member_id, borrow_date, "
        "due_date, IFNULL(return_date,''), staff_id "
        "FROM Borrowings ORDER BY borrowing_id;";

    if (mysql_query(conn, query) != 0) {
        db_print_error("db_borrowing_read_all", conn);
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) { db_print_error("db_borrowing_read_all:store", conn); return DB_ERR_QUERY; }

    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;

    if (n == 0) {
        *out_array = NULL;
        mysql_free_result(res);
        return DB_OK;
    }

    *out_array = (Borrowing *)calloc((size_t)n, sizeof(Borrowing));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }

    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].borrowing_id = atoi(row[0]);
        (*out_array)[i].book_id      = atoi(row[1]);
        (*out_array)[i].member_id    = atoi(row[2]);
        strncpy((*out_array)[i].borrow_date, row[3] ? row[3] : "",
                sizeof((*out_array)[i].borrow_date) - 1);
        strncpy((*out_array)[i].due_date,    row[4] ? row[4] : "",
                sizeof((*out_array)[i].due_date) - 1);
        strncpy((*out_array)[i].return_date, row[5] ? row[5] : "",
                sizeof((*out_array)[i].return_date) - 1);
        (*out_array)[i].staff_id = atoi(row[6]);
        i++;
    }

    mysql_free_result(res);
    return DB_OK;
}

/*
 * db_borrowing_return_book
 * ─────────────────────────────────────────────────────
 * Atomically:
 *   1. Lock the Borrowings row; verify it hasn't already been returned.
 *   2. Set return_date.
 *   3. Increment books.copies_available by 1.
 *   4. If return_date > due_date → auto-insert a Fine.
 *      Fine amount = $1.00 per day overdue (policy adjustable).
 */
int db_borrowing_return_book(MYSQL *conn, int borrowing_id,
                             const char *return_date) {
    if (!conn || !return_date) return DB_ERR_PARAM;

    if (mysql_query(conn, "START TRANSACTION;") != 0) {
        db_print_error("db_borrowing_return_book:START", conn);
        return DB_ERR_QUERY;
    }

    /* Step 1: Lock row and read current state */
    char lock_q[256];
    snprintf(lock_q, sizeof(lock_q),
             "SELECT book_id, due_date, return_date "
             "FROM Borrowings WHERE borrowing_id=%d FOR UPDATE;",
             borrowing_id);

    if (mysql_query(conn, lock_q) != 0) {
        db_print_error("db_borrowing_return_book:lock", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) { mysql_query(conn, "ROLLBACK;"); return DB_ERR_QUERY; }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_NO_ROWS;
    }

    int book_id = atoi(row[0]);
    char due_date[16]     = {0};
    char old_return[16]   = {0};
    strncpy(due_date,   row[1] ? row[1] : "", 15);
    strncpy(old_return, row[2] ? row[2] : "", 15);
    mysql_free_result(res);

    if (old_return[0] != '\0') {
        fprintf(stderr,
                "[db_borrowing_return_book] borrowing_id %d already returned on %s.\n",
                borrowing_id, old_return);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_CONSTRAINT;
    }

    /* Step 2: Set return_date */
    char e_ret[34];
    escape(conn, e_ret, return_date, strlen(return_date));

    char upd_borrow[256];
    snprintf(upd_borrow, sizeof(upd_borrow),
             "UPDATE Borrowings SET return_date='%s' WHERE borrowing_id=%d;",
             e_ret, borrowing_id);

    if (mysql_query(conn, upd_borrow) != 0) {
        db_print_error("db_borrowing_return_book:set_return", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    /* Step 3: Increment copies_available */
    char inc_q[128];
    snprintf(inc_q, sizeof(inc_q),
             "UPDATE Books SET copies_available = copies_available + 1 "
             "WHERE book_id=%d;", book_id);

    if (mysql_query(conn, inc_q) != 0) {
        db_print_error("db_borrowing_return_book:increment", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    /* Step 4: Auto-fine if overdue
     * Use MySQL DATEDIFF to avoid dragging libc date arithmetic in. */
    char fine_check[512];
    snprintf(fine_check, sizeof(fine_check),
             "SELECT DATEDIFF('%s', '%s') AS days_overdue;",
             e_ret, due_date);

    if (mysql_query(conn, fine_check) != 0) {
        db_print_error("db_borrowing_return_book:fine_check", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    res = mysql_store_result(conn);
    if (!res) { mysql_query(conn, "ROLLBACK;"); return DB_ERR_QUERY; }

    row = mysql_fetch_row(res);
    int days_overdue = row ? atoi(row[0]) : 0;
    mysql_free_result(res);

    if (days_overdue > 0) {
        double fine_amount = days_overdue * 1.00;  

        char ins_fine[256];
        snprintf(ins_fine, sizeof(ins_fine),
                 "INSERT INTO Fines (borrowing_id, amount, paid, date_paid) "
                 "VALUES (%d, %.2f, 0, NULL);",
                 borrowing_id, fine_amount);

        if (mysql_query(conn, ins_fine) != 0) {
            db_print_error("db_borrowing_return_book:insert_fine", conn);
            mysql_query(conn, "ROLLBACK;");
            return DB_ERR_QUERY;
        }

        printf("[db_borrowing_return_book] Fine of $%.2f applied "
               "(%d day(s) overdue) on borrowing_id %d.\n",
               fine_amount, days_overdue, borrowing_id);
    }

    if (mysql_query(conn, "COMMIT;") != 0) {
        db_print_error("db_borrowing_return_book:COMMIT", conn);
        mysql_query(conn, "ROLLBACK;");
        return DB_ERR_QUERY;
    }

    return DB_OK;
}

int db_borrowing_delete(MYSQL *conn, int borrowing_id) {
    if (!conn) return DB_ERR_PARAM;


    char del_fines[128];
    snprintf(del_fines, sizeof(del_fines),
             "DELETE FROM Fines WHERE borrowing_id=%d;", borrowing_id);
    if (mysql_query(conn, del_fines) != 0) {
        db_print_error("db_borrowing_delete:del_fines", conn);
        return DB_ERR_QUERY;
    }

    char query[128];
    snprintf(query, sizeof(query),
             "DELETE FROM Borrowings WHERE borrowing_id=%d;", borrowing_id);
    if (mysql_query(conn, query) != 0) {
        db_print_error("db_borrowing_delete", conn);
        return DB_ERR_QUERY;
    }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_borrowing_free_array(Borrowing *array) {
    free(array);
}


int db_author_create(MYSQL *conn, const Author *a) {
    if (!conn || !a) return DB_ERR_PARAM;
    char e_name[258], e_bio[2050];
    escape(conn, e_name, a->name, sizeof(a->name) - 1);
    escape(conn, e_bio,  a->bio,  sizeof(a->bio)  - 1);
    char query[2560];
    snprintf(query, sizeof(query),
             "INSERT INTO Authors (name, bio) VALUES ('%s', '%s');",
             e_name, e_bio);
    if (mysql_query(conn, query) != 0) { db_print_error("db_author_create", conn); return DB_ERR_QUERY; }
    return DB_OK;
}

int db_author_read_by_id(MYSQL *conn, int author_id, Author *out) {
    if (!conn || !out) return DB_ERR_PARAM;
    char query[128];
    snprintf(query, sizeof(query),
             "SELECT author_id, name, bio FROM Authors WHERE author_id=%d LIMIT 1;",
             author_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_author_read_by_id", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }
    out->author_id = atoi(row[0]);
    strncpy(out->name, row[1] ? row[1] : "", sizeof(out->name) - 1);
    strncpy(out->bio,  row[2] ? row[2] : "", sizeof(out->bio)  - 1);
    mysql_free_result(res);
    return DB_OK;
}

int db_author_read_all(MYSQL *conn, Author **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;
    if (mysql_query(conn, "SELECT author_id, name, bio FROM Authors ORDER BY author_id;") != 0) {
        db_print_error("db_author_read_all", conn); return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;
    if (n == 0) { *out_array = NULL; mysql_free_result(res); return DB_OK; }
    *out_array = (Author *)calloc((size_t)n, sizeof(Author));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }
    MYSQL_ROW row; int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].author_id = atoi(row[0]);
        strncpy((*out_array)[i].name, row[1] ? row[1] : "", sizeof((*out_array)[i].name) - 1);
        strncpy((*out_array)[i].bio,  row[2] ? row[2] : "", sizeof((*out_array)[i].bio)  - 1);
        i++;
    }
    mysql_free_result(res);
    return DB_OK;
}

int db_author_update(MYSQL *conn, const Author *a) {
    if (!conn || !a) return DB_ERR_PARAM;
    char e_name[258], e_bio[2050];
    escape(conn, e_name, a->name, sizeof(a->name) - 1);
    escape(conn, e_bio,  a->bio,  sizeof(a->bio)  - 1);
    char query[2560];
    snprintf(query, sizeof(query),
             "UPDATE Authors SET name='%s', bio='%s' WHERE author_id=%d;",
             e_name, e_bio, a->author_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_author_update", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_author_delete(MYSQL *conn, int author_id) {
    if (!conn) return DB_ERR_PARAM;
    char check[256];
    snprintf(check, sizeof(check),
             "SELECT 1 FROM Books WHERE author_id=%d LIMIT 1;", author_id);
    if (mysql_query(conn, check) != 0) { db_print_error("db_author_delete:check", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    int linked = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    if (linked) { fprintf(stderr, "[db_author_delete] Author %d has linked books.\n", author_id); return DB_ERR_CONSTRAINT; }
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Authors WHERE author_id=%d;", author_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_author_delete", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_author_free_array(Author *array) { free(array); }


int db_publisher_create(MYSQL *conn, const Publisher *p) {
    if (!conn || !p) return DB_ERR_PARAM;
    char e_name[258], e_addr[514], e_contact[258];
    escape(conn, e_name,    p->name,         sizeof(p->name)         - 1);
    escape(conn, e_addr,    p->address,      sizeof(p->address)      - 1);
    escape(conn, e_contact, p->contact_info, sizeof(p->contact_info) - 1);
    char query[1152];
    snprintf(query, sizeof(query),
             "INSERT INTO Publishers (name, address, contact_info) "
             "VALUES ('%s', '%s', '%s');", e_name, e_addr, e_contact);
    if (mysql_query(conn, query) != 0) { db_print_error("db_publisher_create", conn); return DB_ERR_QUERY; }
    return DB_OK;
}

int db_publisher_read_by_id(MYSQL *conn, int publisher_id, Publisher *out) {
    if (!conn || !out) return DB_ERR_PARAM;
    char query[196];
    snprintf(query, sizeof(query),
             "SELECT publisher_id, name, address, contact_info "
             "FROM Publishers WHERE publisher_id=%d LIMIT 1;", publisher_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_publisher_read_by_id", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }
    out->publisher_id = atoi(row[0]);
    strncpy(out->name,         row[1] ? row[1] : "", sizeof(out->name)         - 1);
    strncpy(out->address,      row[2] ? row[2] : "", sizeof(out->address)      - 1);
    strncpy(out->contact_info, row[3] ? row[3] : "", sizeof(out->contact_info) - 1);
    mysql_free_result(res);
    return DB_OK;
}

int db_publisher_read_all(MYSQL *conn, Publisher **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;
    if (mysql_query(conn, "SELECT publisher_id, name, address, contact_info FROM Publishers ORDER BY publisher_id;") != 0) {
        db_print_error("db_publisher_read_all", conn); return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;
    if (n == 0) { *out_array = NULL; mysql_free_result(res); return DB_OK; }
    *out_array = (Publisher *)calloc((size_t)n, sizeof(Publisher));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }
    MYSQL_ROW row; int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].publisher_id = atoi(row[0]);
        strncpy((*out_array)[i].name,         row[1] ? row[1] : "", sizeof((*out_array)[i].name)         - 1);
        strncpy((*out_array)[i].address,      row[2] ? row[2] : "", sizeof((*out_array)[i].address)      - 1);
        strncpy((*out_array)[i].contact_info, row[3] ? row[3] : "", sizeof((*out_array)[i].contact_info) - 1);
        i++;
    }
    mysql_free_result(res);
    return DB_OK;
}

int db_publisher_update(MYSQL *conn, const Publisher *p) {
    if (!conn || !p) return DB_ERR_PARAM;
    char e_name[258], e_addr[514], e_contact[258];
    escape(conn, e_name,    p->name,         sizeof(p->name)         - 1);
    escape(conn, e_addr,    p->address,      sizeof(p->address)      - 1);
    escape(conn, e_contact, p->contact_info, sizeof(p->contact_info) - 1);
    char query[1152];
    snprintf(query, sizeof(query),
             "UPDATE Publishers SET name='%s', address='%s', contact_info='%s' "
             "WHERE publisher_id=%d;", e_name, e_addr, e_contact, p->publisher_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_publisher_update", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_publisher_delete(MYSQL *conn, int publisher_id) {
    if (!conn) return DB_ERR_PARAM;
    char check[256];
    snprintf(check, sizeof(check),
             "SELECT 1 FROM Books WHERE publisher_id=%d LIMIT 1;", publisher_id);
    if (mysql_query(conn, check) != 0) { db_print_error("db_publisher_delete:check", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    int linked = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    if (linked) { fprintf(stderr, "[db_publisher_delete] Publisher %d has linked books.\n", publisher_id); return DB_ERR_CONSTRAINT; }
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Publishers WHERE publisher_id=%d;", publisher_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_publisher_delete", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_publisher_free_array(Publisher *array) { free(array); }

int db_member_create(MYSQL *conn, const Member *m) {
    if (!conn || !m) return DB_ERR_PARAM;
    char e_name[258], e_addr[514], e_phone[66], e_email[258], e_date[34], e_status[66];
    escape(conn, e_name,   m->name,              sizeof(m->name)              - 1);
    escape(conn, e_addr,   m->address,           sizeof(m->address)           - 1);
    escape(conn, e_phone,  m->phone,             sizeof(m->phone)             - 1);
    escape(conn, e_email,  m->email,             sizeof(m->email)             - 1);
    escape(conn, e_date,   m->date_joined,       sizeof(m->date_joined)       - 1);
    escape(conn, e_status, m->membership_status, sizeof(m->membership_status) - 1);
    char query[1344];
    snprintf(query, sizeof(query),
             "INSERT INTO Members (name, address, phone, email, date_joined, membership_status) "
             "VALUES ('%s','%s','%s','%s','%s','%s');",
             e_name, e_addr, e_phone, e_email, e_date, e_status);
    if (mysql_query(conn, query) != 0) { db_print_error("db_member_create", conn); return DB_ERR_QUERY; }
    return DB_OK;
}

int db_member_read_by_id(MYSQL *conn, int member_id, Member *out) {
    if (!conn || !out) return DB_ERR_PARAM;
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT member_id, name, address, phone, email, date_joined, membership_status "
             "FROM Members WHERE member_id=%d LIMIT 1;", member_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_member_read_by_id", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }
    out->member_id = atoi(row[0]);
    strncpy(out->name,              row[1] ? row[1] : "", sizeof(out->name)              - 1);
    strncpy(out->address,           row[2] ? row[2] : "", sizeof(out->address)           - 1);
    strncpy(out->phone,             row[3] ? row[3] : "", sizeof(out->phone)             - 1);
    strncpy(out->email,             row[4] ? row[4] : "", sizeof(out->email)             - 1);
    strncpy(out->date_joined,       row[5] ? row[5] : "", sizeof(out->date_joined)       - 1);
    strncpy(out->membership_status, row[6] ? row[6] : "", sizeof(out->membership_status) - 1);
    mysql_free_result(res);
    return DB_OK;
}

int db_member_read_all(MYSQL *conn, Member **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;
    if (mysql_query(conn, "SELECT member_id, name, address, phone, email, date_joined, membership_status FROM Members ORDER BY member_id;") != 0) {
        db_print_error("db_member_read_all", conn); return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;
    if (n == 0) { *out_array = NULL; mysql_free_result(res); return DB_OK; }
    *out_array = (Member *)calloc((size_t)n, sizeof(Member));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }
    MYSQL_ROW row; int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].member_id = atoi(row[0]);
        strncpy((*out_array)[i].name,              row[1] ? row[1] : "", sizeof((*out_array)[i].name)              - 1);
        strncpy((*out_array)[i].address,           row[2] ? row[2] : "", sizeof((*out_array)[i].address)           - 1);
        strncpy((*out_array)[i].phone,             row[3] ? row[3] : "", sizeof((*out_array)[i].phone)             - 1);
        strncpy((*out_array)[i].email,             row[4] ? row[4] : "", sizeof((*out_array)[i].email)             - 1);
        strncpy((*out_array)[i].date_joined,       row[5] ? row[5] : "", sizeof((*out_array)[i].date_joined)       - 1);
        strncpy((*out_array)[i].membership_status, row[6] ? row[6] : "", sizeof((*out_array)[i].membership_status) - 1);
        i++;
    }
    mysql_free_result(res);
    return DB_OK;
}

int db_member_update(MYSQL *conn, const Member *m) {
    if (!conn || !m) return DB_ERR_PARAM;
    char e_name[258], e_addr[514], e_phone[66], e_email[258], e_date[34], e_status[66];
    escape(conn, e_name,   m->name,              sizeof(m->name)              - 1);
    escape(conn, e_addr,   m->address,           sizeof(m->address)           - 1);
    escape(conn, e_phone,  m->phone,             sizeof(m->phone)             - 1);
    escape(conn, e_email,  m->email,             sizeof(m->email)             - 1);
    escape(conn, e_date,   m->date_joined,       sizeof(m->date_joined)       - 1);
    escape(conn, e_status, m->membership_status, sizeof(m->membership_status) - 1);
    char query[1344];
    snprintf(query, sizeof(query),
             "UPDATE Members SET name='%s', address='%s', phone='%s', email='%s', "
             "date_joined='%s', membership_status='%s' WHERE member_id=%d;",
             e_name, e_addr, e_phone, e_email, e_date, e_status, m->member_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_member_update", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_member_delete(MYSQL *conn, int member_id) {
    if (!conn) return DB_ERR_PARAM;
    char check[256];
    snprintf(check, sizeof(check),
             "SELECT 1 FROM Borrowings WHERE member_id=%d AND return_date IS NULL LIMIT 1;",
             member_id);
    if (mysql_query(conn, check) != 0) { db_print_error("db_member_delete:check", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    int active = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    if (active) { fprintf(stderr, "[db_member_delete] Member %d has unreturned books.\n", member_id); return DB_ERR_CONSTRAINT; }
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Members WHERE member_id=%d;", member_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_member_delete", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_member_free_array(Member *array) { free(array); }


int db_staff_create(MYSQL *conn, const Staff *s) {
    if (!conn || !s) return DB_ERR_PARAM;
    char e_name[258], e_role[130], e_email[258], e_phone[66];
    escape(conn, e_name,  s->name,  sizeof(s->name)  - 1);
    escape(conn, e_role,  s->role,  sizeof(s->role)  - 1);
    escape(conn, e_email, s->email, sizeof(s->email) - 1);
    escape(conn, e_phone, s->phone, sizeof(s->phone) - 1);
    char query[832];
    snprintf(query, sizeof(query),
             "INSERT INTO Staff (name, role, email, phone) VALUES ('%s','%s','%s','%s');",
             e_name, e_role, e_email, e_phone);
    if (mysql_query(conn, query) != 0) { db_print_error("db_staff_create", conn); return DB_ERR_QUERY; }
    return DB_OK;
}

int db_staff_read_by_id(MYSQL *conn, int staff_id, Staff *out) {
    if (!conn || !out) return DB_ERR_PARAM;
    char query[196];
    snprintf(query, sizeof(query),
             "SELECT staff_id, name, role, email, phone "
             "FROM Staff WHERE staff_id=%d LIMIT 1;", staff_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_staff_read_by_id", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }
    out->staff_id = atoi(row[0]);
    strncpy(out->name,  row[1] ? row[1] : "", sizeof(out->name)  - 1);
    strncpy(out->role,  row[2] ? row[2] : "", sizeof(out->role)  - 1);
    strncpy(out->email, row[3] ? row[3] : "", sizeof(out->email) - 1);
    strncpy(out->phone, row[4] ? row[4] : "", sizeof(out->phone) - 1);
    mysql_free_result(res);
    return DB_OK;
}

int db_staff_read_all(MYSQL *conn, Staff **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;
    if (mysql_query(conn, "SELECT staff_id, name, role, email, phone FROM Staff ORDER BY staff_id;") != 0) {
        db_print_error("db_staff_read_all", conn); return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;
    if (n == 0) { *out_array = NULL; mysql_free_result(res); return DB_OK; }
    *out_array = (Staff *)calloc((size_t)n, sizeof(Staff));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }
    MYSQL_ROW row; int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].staff_id = atoi(row[0]);
        strncpy((*out_array)[i].name,  row[1] ? row[1] : "", sizeof((*out_array)[i].name)  - 1);
        strncpy((*out_array)[i].role,  row[2] ? row[2] : "", sizeof((*out_array)[i].role)  - 1);
        strncpy((*out_array)[i].email, row[3] ? row[3] : "", sizeof((*out_array)[i].email) - 1);
        strncpy((*out_array)[i].phone, row[4] ? row[4] : "", sizeof((*out_array)[i].phone) - 1);
        i++;
    }
    mysql_free_result(res);
    return DB_OK;
}

int db_staff_update(MYSQL *conn, const Staff *s) {
    if (!conn || !s) return DB_ERR_PARAM;
    char e_name[258], e_role[130], e_email[258], e_phone[66];
    escape(conn, e_name,  s->name,  sizeof(s->name)  - 1);
    escape(conn, e_role,  s->role,  sizeof(s->role)  - 1);
    escape(conn, e_email, s->email, sizeof(s->email) - 1);
    escape(conn, e_phone, s->phone, sizeof(s->phone) - 1);
    char query[832];
    snprintf(query, sizeof(query),
             "UPDATE Staff SET name='%s', role='%s', email='%s', phone='%s' "
             "WHERE staff_id=%d;", e_name, e_role, e_email, e_phone, s->staff_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_staff_update", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_staff_delete(MYSQL *conn, int staff_id) {
    if (!conn) return DB_ERR_PARAM;
    char check[256];
    snprintf(check, sizeof(check),
             "SELECT 1 FROM Borrowings WHERE staff_id=%d LIMIT 1;", staff_id);
    if (mysql_query(conn, check) != 0) { db_print_error("db_staff_delete:check", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    int linked = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    if (linked) { fprintf(stderr, "[db_staff_delete] Staff %d processed borrowings — reassign first.\n", staff_id); return DB_ERR_CONSTRAINT; }
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Staff WHERE staff_id=%d;", staff_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_staff_delete", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_staff_free_array(Staff *array) { free(array); }

int db_fine_create(MYSQL *conn, const Fine *f) {
    if (!conn || !f) return DB_ERR_PARAM;
    if (!db_foreign_key_exists(conn, "Borrowings", "borrowing_id", f->borrowing_id)) {
        fprintf(stderr, "[db_fine_create] borrowing_id %d not found.\n", f->borrowing_id);
        return DB_ERR_CONSTRAINT;
    }
    char query[256];
    snprintf(query, sizeof(query),
             "INSERT INTO Fines (borrowing_id, amount, paid, date_paid) "
             "VALUES (%d, %.2f, %d, %s);",
             f->borrowing_id, f->amount, f->paid ? 1 : 0,
             (f->paid && f->date_paid[0]) ? f->date_paid : "NULL");
    if (mysql_query(conn, query) != 0) { db_print_error("db_fine_create", conn); return DB_ERR_QUERY; }
    return DB_OK;
}

int db_fine_read_by_id(MYSQL *conn, int fine_id, Fine *out) {
    if (!conn || !out) return DB_ERR_PARAM;
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT fine_id, borrowing_id, amount, paid, IFNULL(date_paid,'') "
             "FROM Fines WHERE fine_id=%d LIMIT 1;", fine_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_fine_read_by_id", conn); return DB_ERR_QUERY; }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) { mysql_free_result(res); return DB_ERR_NO_ROWS; }
    out->fine_id      = atoi(row[0]);
    out->borrowing_id = atoi(row[1]);
    out->amount       = atof(row[2]);
    out->paid         = atoi(row[3]);
    strncpy(out->date_paid, row[4] ? row[4] : "", sizeof(out->date_paid) - 1);
    mysql_free_result(res);
    return DB_OK;
}

int db_fine_read_all(MYSQL *conn, Fine **out_array, int *out_count) {
    if (!conn || !out_array || !out_count) return DB_ERR_PARAM;
    if (mysql_query(conn, "SELECT fine_id, borrowing_id, amount, paid, IFNULL(date_paid,'') FROM Fines ORDER BY fine_id;") != 0) {
        db_print_error("db_fine_read_all", conn); return DB_ERR_QUERY;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return DB_ERR_QUERY;
    my_ulonglong n = mysql_num_rows(res);
    *out_count = (int)n;
    if (n == 0) { *out_array = NULL; mysql_free_result(res); return DB_OK; }
    *out_array = (Fine *)calloc((size_t)n, sizeof(Fine));
    if (!*out_array) { mysql_free_result(res); return DB_ERR_PARAM; }
    MYSQL_ROW row; int i = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        (*out_array)[i].fine_id      = atoi(row[0]);
        (*out_array)[i].borrowing_id = atoi(row[1]);
        (*out_array)[i].amount       = atof(row[2]);
        (*out_array)[i].paid         = atoi(row[3]);
        strncpy((*out_array)[i].date_paid, row[4] ? row[4] : "",
                sizeof((*out_array)[i].date_paid) - 1);
        i++;
    }
    mysql_free_result(res);
    return DB_OK;
}

int db_fine_pay(MYSQL *conn, int fine_id, const char *date_paid) {
    if (!conn || !date_paid) return DB_ERR_PARAM;
    char e_date[34];
    escape(conn, e_date, date_paid, strlen(date_paid));
    char query[196];
    snprintf(query, sizeof(query),
             "UPDATE Fines SET paid=1, date_paid='%s' WHERE fine_id=%d;",
             e_date, fine_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_fine_pay", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

int db_fine_delete(MYSQL *conn, int fine_id) {
    if (!conn) return DB_ERR_PARAM;
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM Fines WHERE fine_id=%d;", fine_id);
    if (mysql_query(conn, query) != 0) { db_print_error("db_fine_delete", conn); return DB_ERR_QUERY; }
    if (mysql_affected_rows(conn) == 0) return DB_ERR_NO_ROWS;
    return DB_OK;
}

void db_fine_free_array(Fine *array) { free(array); }
