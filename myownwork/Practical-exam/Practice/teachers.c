#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "teachers.dat"

typedef struct {
    int id;
    char name[50];
    char subject[50];
    char department[50];
    float salary;
    int experience;        // years of experience
} Teacher;

// Function prototypes
void addTeacher();
void displayTeachers();
void searchTeacher();
void updateTeacher();
void deleteTeacher();
int getNextId();

void clearInputBuffer() {
    while(getchar() != '\n');
}

int main() {
    int choice;
    while(1) {
        printf("\n=== Teacher Management System ===\n");
        printf("1. Add Teacher\n");
        printf("2. Display All Teachers\n");
        printf("3. Search Teacher\n");
        printf("4. Update Teacher\n");
        printf("5. Delete Teacher\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: addTeacher(); break;
            case 2: displayTeachers(); break;
            case 3: searchTeacher(); break;
            case 4: updateTeacher(); break;
            case 5: deleteTeacher(); break;
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
    
    Teacher t;
    int maxId = 0;
    while(fread(&t, sizeof(Teacher), 1, f)) {
        if(t.id > maxId) maxId = t.id;
    }
    fclose(f);
    return maxId + 1;
}

void addTeacher() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) {
        printf("Error opening file!\n");
        return;
    }
    
    Teacher t;
    t.id = getNextId();
    
    printf("Enter Teacher Name: ");
    fgets(t.name, 50, stdin);
    t.name[strcspn(t.name, "\n")] = 0;
    
    printf("Enter Subject: ");
    fgets(t.subject, 50, stdin);
    t.subject[strcspn(t.subject, "\n")] = 0;
    
    printf("Enter Department: ");
    fgets(t.department, 50, stdin);
    t.department[strcspn(t.department, "\n")] = 0;
    
    printf("Enter Salary: ");
    scanf("%f", &t.salary);
    clearInputBuffer();
    
    printf("Enter Experience (years): ");
    scanf("%d", &t.experience);
    clearInputBuffer();
    
    fwrite(&t, sizeof(Teacher), 1, f);
    fclose(f);
    
    printf("Teacher added successfully! ID: %d\n", t.id);
}

void displayTeachers() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No teachers found!\n");
        return;
    }
    
    Teacher t;
    printf("\nID\tName\t\tSubject\t\tDepartment\tSalary\tExperience\n");
    printf("--------------------------------------------------------------------------------\n");
    
    while(fread(&t, sizeof(Teacher), 1, f)) {
        printf("%d\t%-15s\t%-15s\t%-15s\t%.2f\t%d years\n", 
               t.id, t.name, t.subject, t.department, t.salary, t.experience);
    }
    fclose(f);
}

void searchTeacher() {
    int id;
    printf("Enter Teacher ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Teacher t;
    int found = 0;
    while(fread(&t, sizeof(Teacher), 1, f)) {
        if(t.id == id) {
            printf("\nTeacher Found:\n");
            printf("ID: %d\nName: %s\nSubject: %s\nDepartment: %s\nSalary: %.2f\nExperience: %d years\n",
                   t.id, t.name, t.subject, t.department, t.salary, t.experience);
            found = 1;
            break;
        }
    }
    fclose(f);
    if(!found) printf("Teacher with ID %d not found!\n", id);
}

void updateTeacher() {
    int id;
    printf("Enter Teacher ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb+");
    if(!f) {
        printf("No records found!\n");
        return;
    }
    
    Teacher t;
    long pos;
    int found = 0;
    
    while(fread(&t, sizeof(Teacher), 1, f)) {
        if(t.id == id) {
            pos = ftell(f) - sizeof(Teacher);
            found = 1;
            
            printf("Enter new Name: ");
            fgets(t.name, 50, stdin);
            t.name[strcspn(t.name, "\n")] = 0;
            
            printf("Enter new Subject: ");
            fgets(t.subject, 50, stdin);
            t.subject[strcspn(t.subject, "\n")] = 0;
            
            printf("Enter new Department: ");
            fgets(t.department, 50, stdin);
            t.department[strcspn(t.department, "\n")] = 0;
            
            printf("Enter new Salary: ");
            scanf("%f", &t.salary);
            clearInputBuffer();
            
            printf("Enter new Experience (years): ");
            scanf("%d", &t.experience);
            clearInputBuffer();
            
            fseek(f, pos, SEEK_SET);
            fwrite(&t, sizeof(Teacher), 1, f);
            printf("Teacher updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if(!found) printf("Teacher not found!\n");
}

void deleteTeacher() {
    int id;
    printf("Enter Teacher ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    if(!f || !temp) {
        printf("Error opening file!\n");
        return;
    }
    
    Teacher t;
    int found = 0;
    while(fread(&t, sizeof(Teacher), 1, f)) {
        if(t.id != id) {
            fwrite(&t, sizeof(Teacher), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    if(found)
        printf("Teacher deleted successfully!\n");
    else
        printf("Teacher not found!\n");
}