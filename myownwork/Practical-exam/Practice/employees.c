#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "employees.dat"

typedef struct {
    int id;
    char name[50];
    char position[50];
    char department[50];
    float salary;
    int experience;     // years of experience
} Employee;

// Function prototypes
void addEmployee();
void displayEmployees();
void searchEmployee();
void updateEmployee();
void deleteEmployee();
int getNextId();

void clearInputBuffer() {
    while(getchar() != '\n');
}

int main() {
    int choice;
    while(1) {
        printf("\n=== Employee Management System ===\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
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
    
    Employee e;
    int maxId = 0;
    while(fread(&e, sizeof(Employee), 1, f)) {
        if(e.id > maxId) maxId = e.id;
    }
    fclose(f);
    return maxId + 1;
}

void addEmployee() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) {
        printf("Error opening file!\n");
        return;
    }
    
    Employee e;
    e.id = getNextId();
    
    printf("Enter Name: ");
    fgets(e.name, 50, stdin);
    e.name[strcspn(e.name, "\n")] = 0;
    
    printf("Enter Position: ");
    fgets(e.position, 50, stdin);
    e.position[strcspn(e.position, "\n")] = 0;
    
    printf("Enter Department: ");
    fgets(e.department, 50, stdin);
    e.department[strcspn(e.department, "\n")] = 0;
    
    printf("Enter Salary: ");
    scanf("%f", &e.salary);
    clearInputBuffer();
    
    printf("Enter Experience (years): ");
    scanf("%d", &e.experience);
    clearInputBuffer();
    
    fwrite(&e, sizeof(Employee), 1, f);
    fclose(f);
    
    printf("Employee added successfully! ID: %d\n", e.id);
}

void displayEmployees() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No employees found!\n");
        return;
    }
    
    Employee e;
    printf("\nID\tName\t\tPosition\t\tDepartment\tSalary\tExperience\n");
    printf("--------------------------------------------------------------------------------\n");
    
    while(fread(&e, sizeof(Employee), 1, f)) {
        printf("%d\t%-15s\t%-20s\t%-15s\t%.2f\t%d years\n", 
               e.id, e.name, e.position, e.department, e.salary, e.experience);
    }
    fclose(f);
}

void searchEmployee() {
    int id;
    printf("Enter Employee ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Employee e;
    int found = 0;
    while(fread(&e, sizeof(Employee), 1, f)) {
        if(e.id == id) {
            printf("\nEmployee Found:\n");
            printf("ID: %d\nName: %s\nPosition: %s\nDepartment: %s\nSalary: %.2f\nExperience: %d years\n",
                   e.id, e.name, e.position, e.department, e.salary, e.experience);
            found = 1;
            break;
        }
    }
    fclose(f);
    if(!found) printf("Employee with ID %d not found!\n", id);
}

void updateEmployee() {
    int id;
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb+");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Employee e;
    long pos;
    int found = 0;
    
    while(fread(&e, sizeof(Employee), 1, f)) {
        if(e.id == id) {
            pos = ftell(f) - sizeof(Employee);
            found = 1;
            
            printf("Enter new Name: ");
            fgets(e.name, 50, stdin);
            e.name[strcspn(e.name, "\n")] = 0;
            
            printf("Enter new Position: ");
            fgets(e.position, 50, stdin);
            e.position[strcspn(e.position, "\n")] = 0;
            
            printf("Enter new Department: ");
            fgets(e.department, 50, stdin);
            e.department[strcspn(e.department, "\n")] = 0;
            
            printf("Enter new Salary: ");
            scanf("%f", &e.salary);
            clearInputBuffer();
            
            printf("Enter new Experience (years): ");
            scanf("%d", &e.experience);
            clearInputBuffer();
            
            fseek(f, pos, SEEK_SET);
            fwrite(&e, sizeof(Employee), 1, f);
            printf("Employee updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if(!found) printf("Employee not found!\n");
}

void deleteEmployee() {
    int id;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    if(!f || !temp) {
        printf("Error opening file!\n");
        return;
    }
    
    Employee e;
    int found = 0;
    while(fread(&e, sizeof(Employee), 1, f)) {
        if(e.id != id) {
            fwrite(&e, sizeof(Employee), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    if(found)
        printf("Employee deleted successfully!\n");
    else
        printf("Employee not found!\n");
}