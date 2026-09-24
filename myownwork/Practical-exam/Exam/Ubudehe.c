#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "UBUDEHE.txt"

typedef struct
{
long long nationalID;
char names[100];
char gender[10];
char phone[20];
char cell[50];
char village[50];
char startDate[20];
} Record;

void addRecord();
void viewRecords();
void editRecord();
void deleteRecord();

int main()
{
int choice;

do
{
    printf("\n===== UBUDEHE Records Management =====\n");
    printf("1. Add New Record\n");
    printf("2. View All Records\n");
    printf("3. Edit Existing Record\n");
    printf("4. Delete Record\n");
    printf("5. Exit\n");
    printf("Enter Choice: ");

    scanf("%d", &choice);
    getchar();

    switch(choice)
    {
        case 1:
            addRecord();
            break;

        case 2:
            viewRecords();
            break;

        case 3:
            editRecord();
            break;

        case 4:
            deleteRecord();
            break;

        case 5:
            printf("Exiting Program...\n");
            break;

        default:
            printf("Invalid Choice!\n");
    }

} while(choice != 5);

return 0;

}

void addRecord()
{
FILE *fp;
Record r;

fp = fopen(FILE_NAME, "a");

if(fp == NULL)
{
    printf("Error opening file!\n");
    return;
}

printf("Enter National ID: ");
scanf("%lld", &r.nationalID);
getchar();

printf("Enter Names: ");
fgets(r.names, sizeof(r.names), stdin);
r.names[strcspn(r.names, "\n")] = '\0';

printf("Enter Gender (M/F): ");
fgets(r.gender, sizeof(r.gender), stdin);
r.gender[strcspn(r.gender, "\n")] = '\0';

printf("Enter Phone: ");
fgets(r.phone, sizeof(r.phone), stdin);
r.phone[strcspn(r.phone, "\n")] = '\0';

printf("Enter Cell: ");
fgets(r.cell, sizeof(r.cell), stdin);
r.cell[strcspn(r.cell, "\n")] = '\0';

printf("Enter Village: ");
fgets(r.village, sizeof(r.village), stdin);
r.village[strcspn(r.village, "\n")] = '\0';

printf("Enter Starting Date (YYYY-MM-DD): ");
fgets(r.startDate, sizeof(r.startDate), stdin);
r.startDate[strcspn(r.startDate, "\n")] = '\0';

fprintf(fp,
        "%lld|%s|%s|%s|%s|%s|%s\n",
        r.nationalID,
        r.names,
        r.gender,
        r.phone,
        r.cell,
        r.village,
        r.startDate);

fclose(fp);

printf("Record Added Successfully!\n");


}

void viewRecords()
{
FILE *fp;
Record r;


fp = fopen(FILE_NAME, "r");

if(fp == NULL)
{
    printf("No records found!\n");
    return;
}

printf("\n===================================================================================================================\n");
printf("%-18s %-20s %-8s %-15s %-15s %-15s %-15s\n",
       "NationalID",
       "Names",
       "Gender",
       "Phone",
       "Cell",
       "Village",
       "StartDate");
printf("===================================================================================================================\n");

while(fscanf(fp,
             "%lld|%99[^|]|%9[^|]|%19[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
             &r.nationalID,
             r.names,
             r.gender,
             r.phone,
             r.cell,
             r.village,
             r.startDate) == 7)
{
    printf("%-18lld %-20s %-8s %-15s %-15s %-15s %-15s\n",
           r.nationalID,
           r.names,
           r.gender,
           r.phone,
           r.cell,
           r.village,
           r.startDate);
}

fclose(fp);

}

void editRecord()
{
FILE *fp, *temp;
Record r;
long long id;
int found = 0;


printf("Enter National ID to Edit: ");
scanf("%lld", &id);
getchar();

fp = fopen(FILE_NAME, "r");
temp = fopen("temp.txt", "w");

if(fp == NULL)
{
    printf("No records found!\n");
    return;
}

while(fscanf(fp,
             "%lld|%99[^|]|%9[^|]|%19[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
             &r.nationalID,
             r.names,
             r.gender,
             r.phone,
             r.cell,
             r.village,
             r.startDate) == 7)
{
    if(r.nationalID == id)
    {
        found = 1;

        printf("Enter New Names: ");
        fgets(r.names, sizeof(r.names), stdin);
        r.names[strcspn(r.names, "\n")] = '\0';

        printf("Enter New Gender: ");
        fgets(r.gender, sizeof(r.gender), stdin);
        r.gender[strcspn(r.gender, "\n")] = '\0';

        printf("Enter New Phone: ");
        fgets(r.phone, sizeof(r.phone), stdin);
        r.phone[strcspn(r.phone, "\n")] = '\0';

        printf("Enter New Cell: ");
        fgets(r.cell, sizeof(r.cell), stdin);
        r.cell[strcspn(r.cell, "\n")] = '\0';

        printf("Enter New Village: ");
        fgets(r.village, sizeof(r.village), stdin);
        r.village[strcspn(r.village, "\n")] = '\0';

        printf("Enter New Start Date: ");
        fgets(r.startDate, sizeof(r.startDate), stdin);
        r.startDate[strcspn(r.startDate, "\n")] = '\0';
    }

    fprintf(temp,
            "%lld|%s|%s|%s|%s|%s|%s\n",
            r.nationalID,
            r.names,
            r.gender,
            r.phone,
            r.cell,
            r.village,
            r.startDate);
}

fclose(fp);
fclose(temp);

remove(FILE_NAME);
rename("temp.txt", FILE_NAME);

if(found)
    printf("Record Updated Successfully!\n");
else
    printf("Record Not Found!\n");


}

void deleteRecord()
{
FILE *fp, *temp;
Record r;
long long id;
int found = 0;


printf("Enter National ID to Delete: ");
scanf("%lld", &id);
getchar();

fp = fopen(FILE_NAME, "r");
temp = fopen("temp.txt", "w");

if(fp == NULL)
{
    printf("No records found!\n");
    return;
}

while(fscanf(fp,
             "%lld|%99[^|]|%9[^|]|%19[^|]|%49[^|]|%49[^|]|%19[^\n]\n",
             &r.nationalID,
             r.names,
             r.gender,
             r.phone,
             r.cell,
             r.village,
             r.startDate) == 7)
{
    if(r.nationalID != id)
    {
        fprintf(temp,
                "%lld|%s|%s|%s|%s|%s|%s\n",
                r.nationalID,
                r.names,
                r.gender,
                r.phone,
                r.cell,
                r.village,
                r.startDate);
    }
    else
    {
        found = 1;
    }
}

fclose(fp);
fclose(temp);

remove(FILE_NAME);
rename("temp.txt", FILE_NAME);

if(found)
    printf("Record Deleted Successfully!\n");
else
    printf("Record Not Found!\n");


}
