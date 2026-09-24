#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.bin"
#define MAX_NAME 50

// Student structure
typedef struct {
    char name[MAX_NAME];
    int id;
    float gpa;
} Student;

void saveStudents(Student *students, int count) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Cannot create file %s\n", FILENAME);
        return;
    }

    // Write count first, then all students
    fwrite(&count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), count, file);

    fclose(file);
    printf("✓ Saved %d students to %s\n", count, FILENAME);
}

int loadStudents(Student **students) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No existing data found. Starting fresh.\n");
        return 0;
    }

    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        printf("Corrupted file. Starting fresh.\n");
        return 0;
    }

    *students = (Student*)malloc(count * sizeof(Student));
    if (*students == NULL) {
        fclose(file);
        printf("Memory allocation failed!\n");
        return 0;
    }

    if (fread(*students, sizeof(Student), count, file) != count) {
        free(*students);
        *students = NULL;
        printf("Error reading student data.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    printf("✓ Loaded %d students from %s\n", count, FILENAME);
    return count;
}

int main() {
    int capacity = 0, count = 0;
    Student *students = NULL;

    // Load existing data
    count = loadStudents(&students);
    capacity = count;

    char choice;
    while (1) {
        printf("\n=== Student Management System ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Save & Exit\n");
        printf("Choice (1-3): ");
        scanf(" %c", &choice);
        getchar(); // Clear buffer

        if (choice == '1') {
            // Expand capacity if needed
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 10;
                students = (Student*)realloc(students, capacity * sizeof(Student));
                if (students == NULL) {
                    printf("Memory allocation failed!\n");
                    break;
                }
            }

            // Add new student
            printf("Enter name: ");
            fgets(students[count].name, MAX_NAME, stdin);
            students[count].name[strcspn(students[count].name, "\n")] = 0;

            printf("Enter ID: ");
            scanf("%d", &students[count].id);
            printf("Enter GPA: ");
            scanf("%f", &students[count].gpa);
            getchar();

            count++;
            printf("✓ Student added!\n");

        } else if (choice == '2') {
            if (count == 0) {
                printf("No students found.\n");
                continue;
            }

            printf("\n%-20s %-8s %-6s\n", "Name", "ID", "GPA");
            printf("----------------------------------------\n");
            for (int i = 0; i < count; i++) {
                printf("%-20s %-8d %.2f\n",
                       students[i].name, students[i].id, students[i].gpa);
            }

        } else if (choice == '3') {
            saveStudents(students, count);
            break;
        }
    }

    // Cleanup
    free(students);
    printf("Program terminated. Data persisted in %s\n", FILENAME);
    return 0;
}