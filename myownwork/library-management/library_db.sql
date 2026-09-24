-- ============================================================
-- library_db.sql
-- Run once:  mysql -u root -p < library_db.sql
-- ============================================================

CREATE DATABASE IF NOT EXISTS library_db
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE library_db;

-- ── Authors ──────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Authors (
    author_id   INT          NOT NULL AUTO_INCREMENT,
    name        VARCHAR(128) NOT NULL,
    bio         TEXT,
    PRIMARY KEY (author_id)
) ENGINE=InnoDB;

-- ── Publishers ───────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Publishers (
    publisher_id INT          NOT NULL AUTO_INCREMENT,
    name         VARCHAR(128) NOT NULL,
    address      VARCHAR(256),
    contact_info VARCHAR(128),
    PRIMARY KEY (publisher_id)
) ENGINE=InnoDB;

-- ── Books ────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Books (
    book_id          INT          NOT NULL AUTO_INCREMENT,
    title            VARCHAR(256) NOT NULL,
    author_id        INT          NOT NULL,
    publisher_id     INT          NOT NULL,
    isbn             VARCHAR(32),
    genre            VARCHAR(64),
    year_published   INT,
    copies_available INT          NOT NULL DEFAULT 0,
    shelf_location   VARCHAR(32),
    PRIMARY KEY (book_id),
    CONSTRAINT fk_book_author    FOREIGN KEY (author_id)    REFERENCES Authors(author_id),
    CONSTRAINT fk_book_publisher FOREIGN KEY (publisher_id) REFERENCES Publishers(publisher_id)
) ENGINE=InnoDB;

-- ── Members ──────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Members (
    member_id         INT          NOT NULL AUTO_INCREMENT,
    name              VARCHAR(128) NOT NULL,
    address           VARCHAR(256),
    phone             VARCHAR(32),
    email             VARCHAR(128),
    date_joined       DATE,
    membership_status VARCHAR(32)  DEFAULT 'active',
    PRIMARY KEY (member_id)
) ENGINE=InnoDB;

-- ── Staff ────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Staff (
    staff_id INT          NOT NULL AUTO_INCREMENT,
    name     VARCHAR(128) NOT NULL,
    role     VARCHAR(64),
    email    VARCHAR(128),
    phone    VARCHAR(32),
    PRIMARY KEY (staff_id)
) ENGINE=InnoDB;

-- ── Borrowings ───────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Borrowings (
    borrowing_id INT  NOT NULL AUTO_INCREMENT,
    book_id      INT  NOT NULL,
    member_id    INT  NOT NULL,
    borrow_date  DATE NOT NULL,
    due_date     DATE NOT NULL,
    return_date  DATE DEFAULT NULL,
    staff_id     INT  NOT NULL,
    PRIMARY KEY (borrowing_id),
    CONSTRAINT fk_borrow_book   FOREIGN KEY (book_id)   REFERENCES Books(book_id),
    CONSTRAINT fk_borrow_member FOREIGN KEY (member_id) REFERENCES Members(member_id),
    CONSTRAINT fk_borrow_staff  FOREIGN KEY (staff_id)  REFERENCES Staff(staff_id)
) ENGINE=InnoDB;

-- ── Fines ────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS Fines (
    fine_id      INT           NOT NULL AUTO_INCREMENT,
    borrowing_id INT           NOT NULL,
    amount       DECIMAL(10,2) NOT NULL,
    paid         TINYINT(1)    NOT NULL DEFAULT 0,
    date_paid    DATE          DEFAULT NULL,
    PRIMARY KEY (fine_id),
    CONSTRAINT fk_fine_borrowing FOREIGN KEY (borrowing_id) REFERENCES Borrowings(borrowing_id)
) ENGINE=InnoDB;
