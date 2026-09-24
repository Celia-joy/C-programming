//Student management system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[50];
    int rollNo;
    char department[50];
    float marks;
} Student;

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
int getNextId();

int main() {
    int choice;
    while(1) {
        printf("\n=== Student Management System ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

int getNextId() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) return 1;
    Student s;
    int maxId = 0;
    while(fread(&s, sizeof(Student), 1, f)) {
        if(s.id > maxId) maxId = s.id;
    }
    fclose(f);
    return maxId + 1;
}

void addStudent() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) return;
    
    Student s;
    s.id = getNextId();
    
    printf("Enter Name: ");
    getchar();
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = 0;
    
    printf("Enter Roll No: ");
    scanf("%d", &s.rollNo);
    
    printf("Enter Department: ");
    getchar();
    fgets(s.department, 50, stdin);
    s.department[strcspn(s.department, "\n")] = 0;
    
    printf("Enter Marks: ");
    scanf("%f", &s.marks);
    
    fwrite(&s, sizeof(Student), 1, f);
    fclose(f);
    printf("Student added! ID: %d\n", s.id);
}

void displayStudents() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No students found!\n");
        return;
    }
    
    Student s;
    printf("\nID\tName\t\tRoll No\tDepartment\tMarks\n");
    printf("----------------------------------------------------\n");
    while(fread(&s, sizeof(Student), 1, f)) {
        printf("%d\t%-15s\t%d\t%-15s\t%.2f\n", 
               s.id, s.name, s.rollNo, s.department, s.marks);
    }
    fclose(f);
}

// searchStudent, updateStudent, deleteStudent functions are almost identical
// Just change Patient ? Student and field names accordingly

void searchStudent() {
    int id;
    printf("Enter Student ID: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) return;
    
    Student s;
    while(fread(&s, sizeof(Student), 1, f)) {
        if(s.id == id) {
            printf("Found: ID=%d, Name=%s, Roll=%d, Dept=%s, Marks=%.2f\n",
                   s.id, s.name, s.rollNo, s.department, s.marks);
            fclose(f);
            return;
        }
    }
    fclose(f);
    printf("Student not found!\n");
}

void updateStudent() {
    int id;
    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb+");
    if(!f) return;
    
    Student s;
    long pos;
    while(fread(&s, sizeof(Student), 1, f)) {
        if(s.id == id) {
            pos = ftell(f) - sizeof(Student);
            
            printf("Enter new Name: ");
            getchar();
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = 0;
            
            printf("Enter new Roll No: ");
            scanf("%d", &s.rollNo);
            
            printf("Enter new Department: ");
            getchar();
            fgets(s.department, 50, stdin);
            s.department[strcspn(s.department, "\n")] = 0;
            
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);
            
            fseek(f, pos, SEEK_SET);
            fwrite(&s, sizeof(Student), 1, f);
            printf("Updated successfully!\n");
            fclose(f);
            return;
        }
    }
    fclose(f);
    printf("Student not found!\n");
}

void deleteStudent() {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    
    Student s;
    while(fread(&s, sizeof(Student), 1, f)) {
        if(s.id != id) {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }
    
    fclose(f);
    fclose(temp);
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    printf("Deleted successfully!\n");
}