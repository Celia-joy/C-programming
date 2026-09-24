/*
 * cli_main.c
 * Interactive console frontend for the Library Management System.
 * Compile:
 *   gcc -Wall -Wextra -o library cli_main.c db_ops.c $(mysql_config --cflags --libs)
 * Run:
 *   ./library
 */

#include "db_ops.h"
#include <ctype.h>

static int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin)) return 0;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[--len] = '\0';
    return 1;
}

static void read_required(const char *prompt, char *buf, size_t size) {
    do {
        read_line(prompt, buf, size);
    } while (buf[0] == '\0');
}

static int read_int(const char *prompt) {
    char tmp[32];
    read_line(prompt, tmp, sizeof(tmp));
    return atoi(tmp);
}

static double read_double(const char *prompt) {
    char tmp[32];
    read_line(prompt, tmp, sizeof(tmp));
    return atof(tmp);
}

/* Clear the terminal in a portable way. */
static void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Pause until the user presses Enter. */
static void pause_prompt(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void hr(void) {
    printf("──────────────────────────────────────────────────\n");
}


static void menu_books_list(MYSQL *conn) {
    Book *books = NULL;
    int count = 0;
    int rc = db_book_read_all(conn, &books, &count);
    if (rc != DB_OK) { printf("Error reading books.\n"); return; }
    if (count == 0)  { printf("No books found.\n"); return; }

    printf("\n%-6s %-35s %-6s %-6s %-14s %-12s %-6s %-5s %-10s\n",
           "ID", "Title", "AuthID", "PubID", "ISBN", "Genre",
           "Year", "Copies", "Shelf");
    hr();
    for (int i = 0; i < count; i++) {
        printf("%-6d %-35.35s %-6d %-6d %-14.14s %-12.12s %-6d %-5d %-10s\n",
               books[i].book_id,
               books[i].title,
               books[i].author_id,
               books[i].publisher_id,
               books[i].isbn,
               books[i].genre,
               books[i].year_published,
               books[i].copies_available,
               books[i].shelf_location);
    }
    db_book_free_array(books);
}

static void menu_books_view_one(MYSQL *conn) {
    int id = read_int("  Book ID: ");
    Book b = {0};
    int rc = db_book_read_by_id(conn, id, &b);
    if (rc == DB_ERR_NO_ROWS) { printf("Book not found.\n"); return; }
    if (rc != DB_OK)          { printf("Query error.\n");    return; }

    printf("\n");
    hr();
    printf("  Book ID       : %d\n",   b.book_id);
    printf("  Title         : %s\n",   b.title);
    printf("  Author ID     : %d\n",   b.author_id);
    printf("  Publisher ID  : %d\n",   b.publisher_id);
    printf("  ISBN          : %s\n",   b.isbn);
    printf("  Genre         : %s\n",   b.genre);
    printf("  Year Published: %d\n",   b.year_published);
    printf("  Copies Avail. : %d\n",   b.copies_available);
    printf("  Shelf Location: %s\n",   b.shelf_location);
    hr();
}

static void menu_books_add(MYSQL *conn) {
    Book b = {0};
    printf("\n  — Add New Book —\n");
    read_required("  Title          : ", b.title,          sizeof(b.title));
    b.author_id    = read_int("  Author ID      : ");
    b.publisher_id = read_int("  Publisher ID   : ");
    read_line("  ISBN           : ", b.isbn,           sizeof(b.isbn));
    read_line("  Genre          : ", b.genre,          sizeof(b.genre));
    b.year_published   = read_int("  Year Published : ");
    b.copies_available = read_int("  Copies Avail.  : ");
    read_line("  Shelf Location : ", b.shelf_location, sizeof(b.shelf_location));

    int rc = db_book_create(conn, &b);
    if (rc == DB_OK)             printf("  ✔  Book added successfully.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Author or Publisher ID does not exist.\n");
    else                         printf("  ✖  Failed to add book.\n");
}

static void menu_books_edit(MYSQL *conn) {
    int id = read_int("  Book ID to edit: ");
    Book b = {0};
    if (db_book_read_by_id(conn, id, &b) != DB_OK) { printf("Book not found.\n"); return; }

    printf("  (Leave blank to keep current value)\n");
    char tmp[256];

    read_line("  Title          : ", tmp, sizeof(tmp));
    if (tmp[0]) strncpy(b.title, tmp, sizeof(b.title) - 1);

    read_line("  Author ID      : ", tmp, sizeof(tmp));
    if (tmp[0]) b.author_id = atoi(tmp);

    read_line("  Publisher ID   : ", tmp, sizeof(tmp));
    if (tmp[0]) b.publisher_id = atoi(tmp);

    read_line("  ISBN           : ", tmp, sizeof(tmp));
    if (tmp[0]) strncpy(b.isbn, tmp, sizeof(b.isbn) - 1);

    read_line("  Genre          : ", tmp, sizeof(tmp));
    if (tmp[0]) strncpy(b.genre, tmp, sizeof(b.genre) - 1);

    read_line("  Year Published : ", tmp, sizeof(tmp));
    if (tmp[0]) b.year_published = atoi(tmp);

    read_line("  Copies Avail.  : ", tmp, sizeof(tmp));
    if (tmp[0]) b.copies_available = atoi(tmp);

    read_line("  Shelf Location : ", tmp, sizeof(tmp));
    if (tmp[0]) strncpy(b.shelf_location, tmp, sizeof(b.shelf_location) - 1);

    int rc = db_book_update(conn, &b);
    if (rc == DB_OK)             printf("  ✔  Book updated.\n");
    else if (rc == DB_ERR_NO_ROWS)    printf("  ✖  Book not found.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Author or Publisher ID does not exist.\n");
    else                         printf("  ✖  Update failed.\n");
}

static void menu_books_delete(MYSQL *conn) {
    int id = read_int("  Book ID to delete: ");
    printf("  Are you sure? (y/n): ");
    char c[4]; read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }

    int rc = db_book_delete(conn, id);
    if (rc == DB_OK)             printf("  ✔  Book deleted.\n");
    else if (rc == DB_ERR_NO_ROWS)    printf("  ✖  Book not found.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Book has active borrowings — return them first.\n");
    else                         printf("  ✖  Delete failed.\n");
}

static void menu_books(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║       BOOKS MANAGEMENT       ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all books\n");
        printf("  2. View book by ID\n");
        printf("  3. Add new book\n");
        printf("  4. Edit book\n");
        printf("  5. Delete book\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_books_list(conn);     break;
            case 2: menu_books_view_one(conn); break;
            case 3: menu_books_add(conn);      break;
            case 4: menu_books_edit(conn);     break;
            case 5: menu_books_delete(conn);   break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}

/* ═══════════════════════════════════════════════════════
   §2  AUTHORS MENU
═══════════════════════════════════════════════════════ */

static void menu_authors_list(MYSQL *conn) {
    Author *arr = NULL; int count = 0;
    if (db_author_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No authors found.\n"); return; }
    printf("\n%-6s %-30s %s\n", "ID", "Name", "Bio (truncated)");
    hr();
    for (int i = 0; i < count; i++) {
        printf("%-6d %-30.30s %.50s\n", arr[i].author_id, arr[i].name, arr[i].bio);
    }
    db_author_free_array(arr);
}

static void menu_authors_add(MYSQL *conn) {
    Author a = {0};
    printf("\n  — Add New Author —\n");
    read_required("  Name: ", a.name, sizeof(a.name));
    read_line("  Bio : ", a.bio,  sizeof(a.bio));
    int rc = db_author_create(conn, &a);
    printf(rc == DB_OK ? "  ✔  Author added.\n" : "  ✖  Failed.\n");
}

static void menu_authors_edit(MYSQL *conn) {
    int id = read_int("  Author ID to edit: ");
    Author a = {0};
    if (db_author_read_by_id(conn, id, &a) != DB_OK) { printf("Not found.\n"); return; }
    printf("  (Leave blank to keep)\n");
    char tmp[1024];
    read_line("  Name: ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(a.name, tmp, sizeof(a.name)-1);
    read_line("  Bio : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(a.bio,  tmp, sizeof(a.bio) -1);
    int rc = db_author_update(conn, &a);
    printf(rc == DB_OK ? "  ✔  Author updated.\n" : "  ✖  Update failed.\n");
}

static void menu_authors_delete(MYSQL *conn) {
    int id = read_int("  Author ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_author_delete(conn, id);
    if (rc == DB_OK)             printf("  ✔  Deleted.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Author has linked books.\n");
    else                         printf("  ✖  Failed.\n");
}

static void menu_authors(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║      AUTHORS MANAGEMENT      ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all authors\n");
        printf("  2. Add new author\n");
        printf("  3. Edit author\n");
        printf("  4. Delete author\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_authors_list(conn);   break;
            case 2: menu_authors_add(conn);    break;
            case 3: menu_authors_edit(conn);   break;
            case 4: menu_authors_delete(conn); break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}

/* ═══════════════════════════════════════════════════════
   §3  PUBLISHERS MENU
═══════════════════════════════════════════════════════ */

static void menu_publishers_list(MYSQL *conn) {
    Publisher *arr = NULL; int count = 0;
    if (db_publisher_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No publishers found.\n"); return; }
    printf("\n%-6s %-28s %-30s %s\n", "ID", "Name", "Address", "Contact");
    hr();
    for (int i = 0; i < count; i++)
        printf("%-6d %-28.28s %-30.30s %.30s\n",
               arr[i].publisher_id, arr[i].name, arr[i].address, arr[i].contact_info);
    db_publisher_free_array(arr);
}

static void menu_publishers_add(MYSQL *conn) {
    Publisher p = {0};
    printf("\n  — Add New Publisher —\n");
    read_required("  Name        : ", p.name,         sizeof(p.name));
    read_line("  Address     : ", p.address,      sizeof(p.address));
    read_line("  Contact Info: ", p.contact_info, sizeof(p.contact_info));
    int rc = db_publisher_create(conn, &p);
    printf(rc == DB_OK ? "  ✔  Publisher added.\n" : "  ✖  Failed.\n");
}

static void menu_publishers_edit(MYSQL *conn) {
    int id = read_int("  Publisher ID: ");
    Publisher p = {0};
    if (db_publisher_read_by_id(conn, id, &p) != DB_OK) { printf("Not found.\n"); return; }
    char tmp[256];
    printf("  (Leave blank to keep)\n");
    read_line("  Name        : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(p.name,         tmp, sizeof(p.name)        -1);
    read_line("  Address     : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(p.address,      tmp, sizeof(p.address)     -1);
    read_line("  Contact Info: ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(p.contact_info, tmp, sizeof(p.contact_info)-1);
    int rc = db_publisher_update(conn, &p);
    printf(rc == DB_OK ? "  ✔  Updated.\n" : "  ✖  Failed.\n");
}

static void menu_publishers_delete(MYSQL *conn) {
    int id = read_int("  Publisher ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_publisher_delete(conn, id);
    if (rc == DB_OK)             printf("  ✔  Deleted.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Publisher has linked books.\n");
    else                         printf("  ✖  Failed.\n");
}

static void menu_publishers(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║    PUBLISHERS MANAGEMENT     ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all publishers\n");
        printf("  2. Add new publisher\n");
        printf("  3. Edit publisher\n");
        printf("  4. Delete publisher\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_publishers_list(conn);   break;
            case 2: menu_publishers_add(conn);    break;
            case 3: menu_publishers_edit(conn);   break;
            case 4: menu_publishers_delete(conn); break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}

/* ═══════════════════════════════════════════════════════
   §4  MEMBERS MENU
═══════════════════════════════════════════════════════ */

static void menu_members_list(MYSQL *conn) {
    Member *arr = NULL; int count = 0;
    if (db_member_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No members found.\n"); return; }
    printf("\n%-6s %-25s %-14s %-25s %-12s %s\n",
           "ID", "Name", "Phone", "Email", "Joined", "Status");
    hr();
    for (int i = 0; i < count; i++)
        printf("%-6d %-25.25s %-14.14s %-25.25s %-12s %s\n",
               arr[i].member_id, arr[i].name, arr[i].phone,
               arr[i].email, arr[i].date_joined, arr[i].membership_status);
    db_member_free_array(arr);
}

static void menu_members_add(MYSQL *conn) {
    Member m = {0};
    printf("\n  — Add New Member —\n");
    read_required("  Name              : ", m.name,              sizeof(m.name));
    read_line("  Address           : ", m.address,           sizeof(m.address));
    read_line("  Phone             : ", m.phone,             sizeof(m.phone));
    read_line("  Email             : ", m.email,             sizeof(m.email));
    read_required("  Date Joined(YYYY-MM-DD): ", m.date_joined, sizeof(m.date_joined));
    read_line("  Status (active/suspended): ", m.membership_status, sizeof(m.membership_status));
    if (m.membership_status[0] == '\0') strncpy(m.membership_status, "active", 7);
    int rc = db_member_create(conn, &m);
    printf(rc == DB_OK ? "  ✔  Member added.\n" : "  ✖  Failed.\n");
}

static void menu_members_edit(MYSQL *conn) {
    int id = read_int("  Member ID: ");
    Member m = {0};
    if (db_member_read_by_id(conn, id, &m) != DB_OK) { printf("Not found.\n"); return; }
    char tmp[256];
    printf("  (Leave blank to keep)\n");
    read_line("  Name    : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(m.name,              tmp, sizeof(m.name)             -1);
    read_line("  Address : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(m.address,           tmp, sizeof(m.address)          -1);
    read_line("  Phone   : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(m.phone,             tmp, sizeof(m.phone)            -1);
    read_line("  Email   : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(m.email,             tmp, sizeof(m.email)            -1);
    read_line("  Status  : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(m.membership_status, tmp, sizeof(m.membership_status)-1);
    int rc = db_member_update(conn, &m);
    printf(rc == DB_OK ? "  ✔  Updated.\n" : "  ✖  Failed.\n");
}

static void menu_members_delete(MYSQL *conn) {
    int id = read_int("  Member ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_member_delete(conn, id);
    if (rc == DB_OK)             printf("  ✔  Deleted.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Member has unreturned books.\n");
    else                         printf("  ✖  Failed.\n");
}

static void menu_members(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║      MEMBERS MANAGEMENT      ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all members\n");
        printf("  2. Add new member\n");
        printf("  3. Edit member\n");
        printf("  4. Delete member\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_members_list(conn);   break;
            case 2: menu_members_add(conn);    break;
            case 3: menu_members_edit(conn);   break;
            case 4: menu_members_delete(conn); break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}

/* ═══════════════════════════════════════════════════════
   §5  STAFF MENU
═══════════════════════════════════════════════════════ */

static void menu_staff_list(MYSQL *conn) {
    Staff *arr = NULL; int count = 0;
    if (db_staff_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No staff found.\n"); return; }
    printf("\n%-6s %-25s %-20s %-28s %s\n", "ID", "Name", "Role", "Email", "Phone");
    hr();
    for (int i = 0; i < count; i++)
        printf("%-6d %-25.25s %-20.20s %-28.28s %s\n",
               arr[i].staff_id, arr[i].name, arr[i].role, arr[i].email, arr[i].phone);
    db_staff_free_array(arr);
}

static void menu_staff_add(MYSQL *conn) {
    Staff s = {0};
    printf("\n  — Add New Staff Member —\n");
    read_required("  Name : ", s.name,  sizeof(s.name));
    read_line("  Role : ", s.role,  sizeof(s.role));
    read_line("  Email: ", s.email, sizeof(s.email));
    read_line("  Phone: ", s.phone, sizeof(s.phone));
    int rc = db_staff_create(conn, &s);
    printf(rc == DB_OK ? "  ✔  Staff added.\n" : "  ✖  Failed.\n");
}

static void menu_staff_edit(MYSQL *conn) {
    int id = read_int("  Staff ID: ");
    Staff s = {0};
    if (db_staff_read_by_id(conn, id, &s) != DB_OK) { printf("Not found.\n"); return; }
    char tmp[256];
    printf("  (Leave blank to keep)\n");
    read_line("  Name : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(s.name,  tmp, sizeof(s.name) -1);
    read_line("  Role : ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(s.role,  tmp, sizeof(s.role) -1);
    read_line("  Email: ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(s.email, tmp, sizeof(s.email)-1);
    read_line("  Phone: ", tmp, sizeof(tmp)); if (tmp[0]) strncpy(s.phone, tmp, sizeof(s.phone)-1);
    int rc = db_staff_update(conn, &s);
    printf(rc == DB_OK ? "  ✔  Updated.\n" : "  ✖  Failed.\n");
}

static void menu_staff_delete(MYSQL *conn) {
    int id = read_int("  Staff ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_staff_delete(conn, id);
    if (rc == DB_OK)             printf("  ✔  Deleted.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Staff has processed borrowings — reassign first.\n");
    else                         printf("  ✖  Failed.\n");
}

static void menu_staff(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║       STAFF MANAGEMENT       ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all staff\n");
        printf("  2. Add new staff\n");
        printf("  3. Edit staff\n");
        printf("  4. Delete staff\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_staff_list(conn);   break;
            case 2: menu_staff_add(conn);    break;
            case 3: menu_staff_edit(conn);   break;
            case 4: menu_staff_delete(conn); break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}



static void menu_borrowings_list(MYSQL *conn) {
    Borrowing *arr = NULL; int count = 0;
    if (db_borrowing_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No borrowings found.\n"); return; }
    printf("\n%-6s %-6s %-6s %-12s %-12s %-12s %-6s\n",
           "BorID", "BookID", "MemID", "Borrowed", "Due", "Returned", "Staff");
    hr();
    for (int i = 0; i < count; i++) {
        printf("%-6d %-6d %-6d %-12s %-12s %-12s %-6d\n",
               arr[i].borrowing_id,
               arr[i].book_id,
               arr[i].member_id,
               arr[i].borrow_date,
               arr[i].due_date,
               arr[i].return_date[0] ? arr[i].return_date : "(out)",
               arr[i].staff_id);
    }
    db_borrowing_free_array(arr);
}

static void menu_borrowings_borrow(MYSQL *conn) {
    Borrowing b = {0};
    printf("\n  — Issue a Book —\n");
    b.book_id   = read_int("  Book ID         : ");
    b.member_id = read_int("  Member ID       : ");
    b.staff_id  = read_int("  Staff ID        : ");
    read_required("  Borrow Date (YYYY-MM-DD): ", b.borrow_date, sizeof(b.borrow_date));
    read_required("  Due Date    (YYYY-MM-DD): ", b.due_date,    sizeof(b.due_date));

    int rc = db_borrowing_create(conn, &b);
    if (rc == DB_OK)             printf("  ✔  Book issued successfully.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Check: no copies available, or invalid Book/Member/Staff ID.\n");
    else                         printf("  ✖  Failed to issue book.\n");
}

static void menu_borrowings_return(MYSQL *conn) {
    int id = read_int("  Borrowing ID: ");
    char ret_date[16] = {0};
    read_required("  Return Date (YYYY-MM-DD): ", ret_date, sizeof(ret_date));

    int rc = db_borrowing_return_book(conn, id, ret_date);
    if (rc == DB_OK)             printf("  ✔  Book returned. Any applicable fine has been auto-applied.\n");
    else if (rc == DB_ERR_NO_ROWS)    printf("  ✖  Borrowing ID not found.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  This borrowing has already been returned.\n");
    else                         printf("  ✖  Return failed.\n");
}

static void menu_borrowings_view(MYSQL *conn) {
    int id = read_int("  Borrowing ID: ");
    Borrowing b = {0};
    int rc = db_borrowing_read_by_id(conn, id, &b);
    if (rc == DB_ERR_NO_ROWS) { printf("Not found.\n"); return; }
    if (rc != DB_OK)          { printf("Error.\n");     return; }
    printf("\n");
    hr();
    printf("  Borrowing ID : %d\n",  b.borrowing_id);
    printf("  Book ID      : %d\n",  b.book_id);
    printf("  Member ID    : %d\n",  b.member_id);
    printf("  Borrow Date  : %s\n",  b.borrow_date);
    printf("  Due Date     : %s\n",  b.due_date);
    printf("  Return Date  : %s\n",  b.return_date[0] ? b.return_date : "(not yet returned)");
    printf("  Staff ID     : %d\n",  b.staff_id);
    hr();
}

static void menu_borrowings_delete(MYSQL *conn) {
    int id = read_int("  Borrowing ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_borrowing_delete(conn, id);
    printf(rc == DB_OK ? "  ✔  Deleted (linked fines also removed).\n" : "  ✖  Failed.\n");
}

static void menu_borrowings(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║    BORROWINGS MANAGEMENT     ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all borrowings\n");
        printf("  2. View borrowing by ID\n");
        printf("  3. Issue a book  (Borrow)\n");
        printf("  4. Return a book\n");
        printf("  5. Delete borrowing record\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_borrowings_list(conn);    break;
            case 2: menu_borrowings_view(conn);    break;
            case 3: menu_borrowings_borrow(conn);  break;
            case 4: menu_borrowings_return(conn);  break;
            case 5: menu_borrowings_delete(conn);  break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}



static void menu_fines_list(MYSQL *conn) {
    Fine *arr = NULL; int count = 0;
    if (db_fine_read_all(conn, &arr, &count) != DB_OK) { printf("Error.\n"); return; }
    if (count == 0) { printf("No fines found.\n"); return; }
    printf("\n%-8s %-10s %-10s %-6s %s\n",
           "FineID", "BorrowID", "Amount", "Paid", "Date Paid");
    hr();
    for (int i = 0; i < count; i++)
        printf("%-8d %-10d $%-9.2f %-6s %s\n",
               arr[i].fine_id,
               arr[i].borrowing_id,
               arr[i].amount,
               arr[i].paid ? "Yes" : "No",
               arr[i].date_paid[0] ? arr[i].date_paid : "-");
    db_fine_free_array(arr);
}

static void menu_fines_view(MYSQL *conn) {
    int id = read_int("  Fine ID: ");
    Fine f = {0};
    int rc = db_fine_read_by_id(conn, id, &f);
    if (rc == DB_ERR_NO_ROWS) { printf("Not found.\n"); return; }
    if (rc != DB_OK)          { printf("Error.\n"); return; }
    printf("\n");
    hr();
    printf("  Fine ID      : %d\n",    f.fine_id);
    printf("  Borrowing ID : %d\n",    f.borrowing_id);
    printf("  Amount       : $%.2f\n", f.amount);
    printf("  Paid         : %s\n",    f.paid ? "Yes" : "No");
    printf("  Date Paid    : %s\n",    f.date_paid[0] ? f.date_paid : "(unpaid)");
    hr();
}

static void menu_fines_add(MYSQL *conn) {
    Fine f = {0};
    printf("\n  — Add Manual Fine —\n");
    f.borrowing_id = read_int("  Borrowing ID: ");
    f.amount       = read_double("  Amount ($)  : ");
    f.paid         = 0;
    int rc = db_fine_create(conn, &f);
    if (rc == DB_OK)             printf("  ✔  Fine added.\n");
    else if (rc == DB_ERR_CONSTRAINT) printf("  ✖  Borrowing ID not found.\n");
    else                         printf("  ✖  Failed.\n");
}

static void menu_fines_pay(MYSQL *conn) {
    int id = read_int("  Fine ID to mark as paid: ");
    char date[16] = {0};
    read_required("  Payment Date (YYYY-MM-DD): ", date, sizeof(date));
    int rc = db_fine_pay(conn, id, date);
    if (rc == DB_OK)          printf("  ✔  Fine marked as paid.\n");
    else if (rc == DB_ERR_NO_ROWS) printf("  ✖  Fine not found.\n");
    else                      printf("  ✖  Failed.\n");
}

static void menu_fines_delete(MYSQL *conn) {
    int id = read_int("  Fine ID to delete: ");
    char c[4]; printf("  Sure? (y/n): "); read_line("", c, sizeof(c));
    if (tolower(c[0]) != 'y') { printf("  Cancelled.\n"); return; }
    int rc = db_fine_delete(conn, id);
    printf(rc == DB_OK ? "  ✔  Deleted.\n" : "  ✖  Failed.\n");
}

static void menu_fines(MYSQL *conn) {
    int choice;
    do {
        clear_screen();
        printf("\n  ╔══════════════════════════════╗\n");
        printf("  ║       FINES MANAGEMENT       ║\n");
        printf("  ╚══════════════════════════════╝\n");
        printf("  1. List all fines\n");
        printf("  2. View fine by ID\n");
        printf("  3. Add manual fine\n");
        printf("  4. Mark fine as paid\n");
        printf("  5. Delete fine\n");
        printf("  0. Back\n");
        hr();
        choice = read_int("  Choice: ");
        printf("\n");
        switch (choice) {
            case 1: menu_fines_list(conn);   break;
            case 2: menu_fines_view(conn);   break;
            case 3: menu_fines_add(conn);    break;
            case 4: menu_fines_pay(conn);    break;
            case 5: menu_fines_delete(conn); break;
            case 0: return;
            default: printf("  Invalid option.\n");
        }
        if (choice != 0) pause_prompt();
    } while (1);
}

/* ═══════════════════════════════════════════════════════
   §8  MAIN MENU + ENTRY POINT
═══════════════════════════════════════════════════════ */

static void print_main_menu(void) {
    clear_screen();
    printf("\n");
    printf("  ╔══════════════════════════════════════════════╗\n");
    printf("  ║       LIBRARY MANAGEMENT SYSTEM  v1.0       ║\n");
    printf("  ╚══════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  1.  Books\n");
    printf("  2.  Authors\n");
    printf("  3.  Publishers\n");
    printf("  4.  Members\n");
    printf("  5.  Staff\n");
    printf("  6.  Borrowings  (Issue / Return)\n");
    printf("  7.  Fines\n");
    printf("\n");
    printf("  0.  Exit\n");
    printf("\n");
    hr();
}

int main(void) {
    printf("  Connecting to database...\n");
    MYSQL *conn = db_connect();
    if (!conn) {
        fprintf(stderr,
            "\n  ✖  Could not connect to MySQL.\n"
            "     Check DB_HOST / DB_USER / DB_PASS in db_ops.h\n"
            "     and make sure MySQL is running.\n\n");
        return 1;
    }
    printf("  ✔  Connected to '%s' on %s\n\n", DB_NAME, DB_HOST);
    pause_prompt();

    int choice;
    do {
        print_main_menu();
        choice = read_int("  Choice: ");
        switch (choice) {
            case 1: menu_books(conn);      break;
            case 2: menu_authors(conn);    break;
            case 3: menu_publishers(conn); break;
            case 4: menu_members(conn);    break;
            case 5: menu_staff(conn);      break;
            case 6: menu_borrowings(conn); break;
            case 7: menu_fines(conn);      break;
            case 0: break;
            default: printf("  Invalid option.\n"); pause_prompt(); break;
        }
    } while (choice != 0);

    db_disconnect(conn);
    printf("\n  Goodbye.\n\n");
    return 0;
}
