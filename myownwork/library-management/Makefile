# ─────────────────────────────────────────────────────────
# Makefile — Library Management System (CLI)
# Usage:
#   make          → build the CLI app
#   make clean    → remove compiled files
#   make run      → build and run
#   make db       → load the MySQL schema (prompts for password)
# ─────────────────────────────────────────────────────────

CC      = gcc
TARGET  = library
SRCS    = cli_main.c db_ops.c
HEADERS = db_ops.h

CFLAGS  = -Wall -Wextra -g $(shell mysql_config --cflags)
LIBS    = $(shell mysql_config --libs)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)
	@echo "✔  Built: ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

db:
	mysql -u root -p < library_db.sql
	@echo "✔  Schema loaded into MySQL."

clean:
	rm -f $(TARGET)
	@echo "✔  Cleaned."

.PHONY: run db clean
