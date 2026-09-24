#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "books.dat"

typedef struct {
    int id;
    char title[100];
    char author[50];
    char isbn[20];
    int quantity;
    float price;
} Book;

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void updateBook();
void deleteBook();
int getNextId();

void clearInputBuffer() {
    while(getchar() != '\n');
}

int main() {
    int choice;
    while(1) {
        printf("\n=== Book Management System ===\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: deleteBook(); break;
            case 6: 
                printf("Thank you! Goodbye.\n"); 
                exit(0);
            default: 
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

int getNextId() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) return 1;
    
    Book b;
    int maxId = 0;
    while(fread(&b, sizeof(Book), 1, f)) {
        if(b.id > maxId) maxId = b.id;
    }
    fclose(f);
    return maxId + 1;
}

void addBook() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) {
        printf("Error opening file!\n");
        return;
    }
    
    Book b;
    b.id = getNextId();
    
    printf("Enter Book Title: ");
    fgets(b.title, 100, stdin);
    b.title[strcspn(b.title, "\n")] = 0;
    
    printf("Enter Author: ");
    fgets(b.author, 50, stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    
    printf("Enter ISBN: ");
    fgets(b.isbn, 20, stdin);
    b.isbn[strcspn(b.isbn, "\n")] = 0;
    
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);
    clearInputBuffer();
    
    printf("Enter Price: ");
    scanf("%f", &b.price);
    clearInputBuffer();
    
    fwrite(&b, sizeof(Book), 1, f);
    fclose(f);
    
    printf("Book added successfully! ID: %d\n", b.id);
}

void displayBooks() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No books found!\n");
        return;
    }
    
    Book b;
    printf("\nID\tTitle\t\t\tAuthor\t\tISBN\t\tQuantity\tPrice\n");
    printf("--------------------------------------------------------------------------------\n");
    
    while(fread(&b, sizeof(Book), 1, f)) {
        printf("%d\t%-25s\t%-15s\t%s\t\t%d\t\t%.2f\n", 
               b.id, b.title, b.author, b.isbn, b.quantity, b.price);
    }
    fclose(f);
}

void searchBook() {
    int id;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Book b;
    int found = 0;
    while(fread(&b, sizeof(Book), 1, f)) {
        if(b.id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nISBN: %s\nQuantity: %d\nPrice: %.2f\n",
                   b.id, b.title, b.author, b.isbn, b.quantity, b.price);
            found = 1;
            break;
        }
    }
    fclose(f);
    if(!found) printf("Book with ID %d not found!\n", id);
}

void updateBook() {
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb+");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Book b;
    long pos;
    int found = 0;
    
    while(fread(&b, sizeof(Book), 1, f)) {
        if(b.id == id) {
            pos = ftell(f) - sizeof(Book);
            found = 1;
            
            printf("Enter new Title: ");
            fgets(b.title, 100, stdin);
            b.title[strcspn(b.title, "\n")] = 0;
            
            printf("Enter new Author: ");
            fgets(b.author, 50, stdin);
            b.author[strcspn(b.author, "\n")] = 0;
            
            printf("Enter new ISBN: ");
            fgets(b.isbn, 20, stdin);
            b.isbn[strcspn(b.isbn, "\n")] = 0;
            
            printf("Enter new Quantity: ");
            scanf("%d", &b.quantity);
            clearInputBuffer();
            
            printf("Enter new Price: ");
            scanf("%f", &b.price);
            clearInputBuffer();
            
            fseek(f, pos, SEEK_SET);
            fwrite(&b, sizeof(Book), 1, f);
            printf("Book updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if(!found) printf("Book not found!\n");
}

void deleteBook() {
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    if(!f || !temp) {
        printf("Error opening file!\n");
        return;
    }
    
    Book b;
    int found = 0;
    while(fread(&b, sizeof(Book), 1, f)) {
        if(b.id != id) {
            fwrite(&b, sizeof(Book), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    if(found)
        printf("Book deleted successfully!\n");
    else
        printf("Book not found!\n");
}