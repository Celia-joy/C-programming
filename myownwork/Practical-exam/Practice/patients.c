//Hospital management system
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "patients.dat"
#define MAX 100

// Structure (very important for exam)
typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
    char disease[100];
} Patient;

// Function prototypes
void addPatient();
void displayPatients();
void searchPatient();
void updatePatient();
void deletePatient();
int getNextId();

int main() {
    int choice;
    while(1) {
        printf("\n=== Hospital Management System ===\n");
        printf("1. Add Patient\n");
        printf("2. Display All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

int getNextId() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) return 1;
    
    Patient p;
    int maxId = 0;
    while(fread(&p, sizeof(Patient), 1, f)) {
        if(p.id > maxId) maxId = p.id;
    }
    fclose(f);
    return maxId + 1;
}

void addPatient() {
    FILE *f = fopen(FILENAME, "ab");
    if(!f) {
        printf("Error opening file!\n");
        return;
    }
    
    Patient p;
    p.id = getNextId();
    
    printf("Enter Name: ");
    getchar(); // consume newline
    fgets(p.name, 50, stdin);
    p.name[strcspn(p.name, "\n")] = 0; // remove newline
    
    printf("Enter Age: ");
    scanf("%d", &p.age);
    
    printf("Enter Gender (M/F): ");
    scanf("%s", p.gender);
    
    printf("Enter Disease: ");
    getchar();
    fgets(p.disease, 100, stdin);
    p.disease[strcspn(p.disease, "\n")] = 0;
    
    fwrite(&p, sizeof(Patient), 1, f);
    fclose(f);
    printf("Patient added successfully! ID: %d\n", p.id);
}

void displayPatients() {
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No patients found!\n");
        return;
    }
    
    Patient p;
    printf("\nID\tName\t\tAge\tGender\tDisease\n");
    printf("--------------------------------------------------\n");
    while(fread(&p, sizeof(Patient), 1, f)) {
        printf("%d\t%-15s\t%d\t%s\t%s\n", p.id, p.name, p.age, p.gender, p.disease);
    }
    fclose(f);
}

void searchPatient() {
    int id;
    printf("Enter Patient ID to search: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb");
    if(!f) {
        printf("No records!\n");
        return;
    }
    
    Patient p;
    int found = 0;
    while(fread(&p, sizeof(Patient), 1, f)) {
        if(p.id == id) {
            printf("Found: ID=%d, Name=%s, Age=%d, Gender=%s, Disease=%s\n", 
                   p.id, p.name, p.age, p.gender, p.disease);
            found = 1;
            break;
        }
    }
    fclose(f);
    if(!found) printf("Patient not found!\n");
}

void updatePatient() {
    int id;
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb+");  // rb+ for read + write
    if(!f) {
        printf("No records!\n");
        return;
    }
    
    Patient p;
    long pos;
    int found = 0;
    while(fread(&p, sizeof(Patient), 1, f)) {
        if(p.id == id) {
            pos = ftell(f) - sizeof(Patient);  // go back to this record
            found = 1;
            
            printf("Enter new Name: ");
            getchar();
            fgets(p.name, 50, stdin);
            p.name[strcspn(p.name, "\n")] = 0;
            
            printf("Enter new Age: ");
            scanf("%d", &p.age);
            
            printf("Enter new Gender: ");
            scanf("%s", p.gender);
            
            printf("Enter new Disease: ");
            getchar();
            fgets(p.disease, 100, stdin);
            p.disease[strcspn(p.disease, "\n")] = 0;
            
            fseek(f, pos, SEEK_SET);  // move back
            fwrite(&p, sizeof(Patient), 1, f);
            printf("Updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if(!found) printf("Patient not found!\n");
}

void deletePatient() {
    int id;
    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);
    
    FILE *f = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if(!f || !temp) {
        printf("Error!\n");
        return;
    }
    
    Patient p;
    int found = 0;
    while(fread(&p, sizeof(Patient), 1, f)) {
        if(p.id != id) {
            fwrite(&p, sizeof(Patient), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(temp);
    
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    
    if(found)
        printf("Patient deleted successfully!\n");
    else
        printf("Patient not found!\n");
}