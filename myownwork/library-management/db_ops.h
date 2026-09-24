#ifndef DB_OPS_H
#define DB_OPS_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DB_HOST     "127.0.0.1"
#define DB_USER     "root"
#define DB_PASS     ""
#define DB_NAME     "library_db"
#define DB_PORT     3306
#define DB_SOCK     NULL
#define DB_FLAGS    0

/* ─────────────────────────────────────────────
   Return Codes
───────────────────────────────────────────── */
#define DB_OK           0
#define DB_ERR_CONNECT  1
#define DB_ERR_QUERY    2
#define DB_ERR_NO_ROWS  3
#define DB_ERR_CONSTRAINT 4
#define DB_ERR_PARAM    5

/* ─────────────────────────────────────────────
   Entity Structs
───────────────────────────────────────────── */

typedef struct {
    int    book_id;
    char   title[256];
    int    author_id;
    int    publisher_id;
    char   isbn[32];
    char   genre[64];
    int    year_published;
    int    copies_available;
    char   shelf_location[32];
} Book;

typedef struct {
    int  author_id;
    char name[128];
    char bio[1024];
} Author;

typedef struct {
    int  publisher_id;
    char name[128];
    char address[256];
    char contact_info[128];
} Publisher;

typedef struct {
    int  member_id;
    char name[128];
    char address[256];
    char phone[32];
    char email[128];
    char date_joined[16];       /* YYYY-MM-DD */
    char membership_status[32];
} Member;

typedef struct {
    int  staff_id;
    char name[128];
    char role[64];
    char email[128];
    char phone[32];
} Staff;

typedef struct {
    int  borrowing_id;
    int  book_id;
    int  member_id;
    char borrow_date[16];
    char due_date[16];
    char return_date[16];       /* empty string when not yet returned */
    int  staff_id;
} Borrowing;

typedef struct {
    int    fine_id;
    int    borrowing_id;
    double amount;
    int    paid;                /* 0 = false, 1 = true */
    char   date_paid[16];       /* empty string when unpaid */
} Fine;

/* ─────────────────────────────────────────────
   Connection Lifecycle
───────────────────────────────────────────── */
MYSQL *db_connect(void);
void   db_disconnect(MYSQL *conn);

/* ─────────────────────────────────────────────
   Books CRUD
───────────────────────────────────────────── */
int db_book_create(MYSQL *conn, const Book *book);
int db_book_read_by_id(MYSQL *conn, int book_id, Book *out);
int db_book_read_all(MYSQL *conn, Book **out_array, int *out_count);
int db_book_update(MYSQL *conn, const Book *book);
int db_book_delete(MYSQL *conn, int book_id);
void db_book_free_array(Book *array);

/* ─────────────────────────────────────────────
   Borrowings CRUD  (atomic borrow / return)
───────────────────────────────────────────── */
int db_borrowing_create(MYSQL *conn, const Borrowing *b);   /* atomic: checks + decrements copies */
int db_borrowing_read_by_id(MYSQL *conn, int borrowing_id, Borrowing *out);
int db_borrowing_read_all(MYSQL *conn, Borrowing **out_array, int *out_count);
int db_borrowing_return_book(MYSQL *conn, int borrowing_id, const char *return_date);  /* atomic: sets return_date + increments copies + auto-fine */
int db_borrowing_delete(MYSQL *conn, int borrowing_id);
void db_borrowing_free_array(Borrowing *array);

/* ─────────────────────────────────────────────
   Authors CRUD
───────────────────────────────────────────── */
int db_author_create(MYSQL *conn, const Author *a);
int db_author_read_by_id(MYSQL *conn, int author_id, Author *out);
int db_author_read_all(MYSQL *conn, Author **out_array, int *out_count);
int db_author_update(MYSQL *conn, const Author *a);
int db_author_delete(MYSQL *conn, int author_id);
void db_author_free_array(Author *array);

/* ─────────────────────────────────────────────
   Publishers CRUD
───────────────────────────────────────────── */
int db_publisher_create(MYSQL *conn, const Publisher *p);
int db_publisher_read_by_id(MYSQL *conn, int publisher_id, Publisher *out);
int db_publisher_read_all(MYSQL *conn, Publisher **out_array, int *out_count);
int db_publisher_update(MYSQL *conn, const Publisher *p);
int db_publisher_delete(MYSQL *conn, int publisher_id);
void db_publisher_free_array(Publisher *array);

/* ─────────────────────────────────────────────
   Members CRUD
───────────────────────────────────────────── */
int db_member_create(MYSQL *conn, const Member *m);
int db_member_read_by_id(MYSQL *conn, int member_id, Member *out);
int db_member_read_all(MYSQL *conn, Member **out_array, int *out_count);
int db_member_update(MYSQL *conn, const Member *m);
int db_member_delete(MYSQL *conn, int member_id);
void db_member_free_array(Member *array);

/* ─────────────────────────────────────────────
   Staff CRUD
───────────────────────────────────────────── */
int db_staff_create(MYSQL *conn, const Staff *s);
int db_staff_read_by_id(MYSQL *conn, int staff_id, Staff *out);
int db_staff_read_all(MYSQL *conn, Staff **out_array, int *out_count);
int db_staff_update(MYSQL *conn, const Staff *s);
int db_staff_delete(MYSQL *conn, int staff_id);
void db_staff_free_array(Staff *array);

/* ─────────────────────────────────────────────
   Fines CRUD
───────────────────────────────────────────── */
int db_fine_create(MYSQL *conn, const Fine *f);
int db_fine_read_by_id(MYSQL *conn, int fine_id, Fine *out);
int db_fine_read_all(MYSQL *conn, Fine **out_array, int *out_count);
int db_fine_pay(MYSQL *conn, int fine_id, const char *date_paid);
int db_fine_delete(MYSQL *conn, int fine_id);
void db_fine_free_array(Fine *array);

void db_print_error(const char *context, MYSQL *conn);
int  db_foreign_key_exists(MYSQL *conn, const char *table, const char *pk_col, int id);

#endif
