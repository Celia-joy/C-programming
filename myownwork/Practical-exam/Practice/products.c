#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "products.dat"

typedef struct {
    int id;
    char name[50];
    char category[50];
    float price;
    int quantity;
    char description[100];
} Product;

// Function prototypes
void addProduct();
void displayProducts();
void searchProduct();
void updateProduct();
void deleteProduct();
int getNextId();

void clearInputBuffer() {
    while(getchar() != '\n');
}

int main() {
    int choice;
    while(1) {
        printf("\n=== Product Management System ===\n");
        printf("1. Add Product\n");
        printf("2. Display All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: addProduct(); break;
            case 2: displayProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateProduct(); break;
            case 5: deleteProduct(); break;
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
    
    Product p;
    int maxId = 0;
    while(fread(&p, sizeof(Product), 1, f)) {
        if(p.id > maxId) maxId = p.id;
    }
    fclose(f);
    return maxId + 1;
}

void addProduct() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) {
        printf("Error opening file!\n");
        return;
    }
    
    Product p;
    p.id = getNextId();
    
    printf("Enter Product Name: ");
    fgets(p.name, 50, stdin);
    p.name[strcspn(p.name, "\n")] = 0;
    
    printf("Enter Category: ");
    fgets(p.category, 50, stdin);
    p.category[strcspn(p.category, "\n")] = 0;
    
    printf("Enter Price: ");
    scanf("%f", &p.price);
    clearInputBuffer();
    
    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);
    clearInputBuffer();
    
    printf("Enter Description: ");
    fgets(p.description, 100, stdin);
    p.description[strcspn(p.description, "\n")] = 0;
    
    fwrite(&p, sizeof(Product), 1, f);
    fclose(f);
    
    printf("Product added successfully! ID: %d\n", p.id);
}

void displayProducts() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No products found!\n");
        return;
    }
    
    Product p;
    printf("\nID\tName\t\tCategory\t\tPrice\tQuantity\tDescription\n");
    printf("--------------------------------------------------------------------------------\n");
    
    while(fread(&p, sizeof(Product), 1, f)) {
        printf("%d\t%-15s\t%-20s\t%.2f\t%d\t\t%-30s\n", 
               p.id, p.name, p.category, p.price, p.quantity, p.description);
    }
    fclose(f);
}

void searchProduct() {
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Product p;
    int found = 0;
    while(fread(&p, sizeof(Product), 1, f)) {
        if(p.id == id) {
            printf("\nProduct Found:\n");
            printf("ID: %d\nName: %s\nCategory: %s\nPrice: %.2f\nQuantity: %d\nDescription: %s\n",
                   p.id, p.name, p.category, p.price, p.quantity, p.description);
            found = 1;
            break;
        }
    }
    fclose(f);
    if(!found) printf("Product with ID %d not found!\n", id);
}

void updateProduct() {
    int id;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb+");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Product p;
    long pos;
    int found = 0;
    
    while(fread(&p, sizeof(Product), 1, f)) {
        if(p.id == id) {
            pos = ftell(f) - sizeof(Product);
            found = 1;
            
            printf("Enter new Name: ");
            fgets(p.name, 50, stdin);
            p.name[strcspn(p.name, "\n")] = 0;
            
            printf("Enter new Category: ");
            fgets(p.category, 50, stdin);
            p.category[strcspn(p.category, "\n")] = 0;
            
            printf("Enter new Price: ");
            scanf("%f", &p.price);
            clearInputBuffer();
            
            printf("Enter new Quantity: ");
            scanf("%d", &p.quantity);
            clearInputBuffer();
            
            printf("Enter new Description: ");
            fgets(p.description, 100, stdin);
            p.description[strcspn(p.description, "\n")] = 0;
            
            fseek(f, pos, SEEK_SET);
            fwrite(&p, sizeof(Product), 1, f);
            printf("Product updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if(!found) printf("Product not found!\n");
}

void deleteProduct() {
    int id;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    if(!f || !temp) {
        printf("Error opening file!\n");
        return;
    }
    
    Product p;
    int found = 0;
    while(fread(&p, sizeof(Product), 1, f)) {
        if(p.id != id) {
            fwrite(&p, sizeof(Product), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    if(found)
        printf("Product deleted successfully!\n");
    else
        printf("Product not found!\n");
}