#include <stdio.h>

struct student {
	int id, age;
	char lname[20], fname[20], gender[10];
} stud1, stud2;

struct student *ptrStudent = NULL;
void readData () {
	ptrStudent = &stud1;
	printf("Enter data for student 1\n__________________\n");
	printf("Id:");
	scanf("%d", &ptrStudent->id);
	printf("Age:");
	scanf("%d", &ptrStudent->age);
	printf("Lastname:");
	scanf("%s", ptrStudent->lname);
	printf("Firstname:");
	scanf("%s", ptrStudent->fname);
	printf("Gender:");
	scanf("%s", ptrStudent->gender);
	printf("\n");
	
	ptrStudent = &stud2;
	printf("Enter data for student 2\n__________________\n");
	printf("Id:");
	scanf("%d", &ptrStudent->id);
	printf("Age:");
	scanf("%d", &ptrStudent->age);
	printf("Lastname:");
	scanf("%s", ptrStudent->lname);
	printf("Firstname:");
	scanf("%s", ptrStudent->fname);
	printf("Gender:");
	scanf("%s", ptrStudent->gender);
	printf("\n");
}

void outputData () {
	ptrStudent = &stud1;
	printf("Gender:%s\n", ptrStudent->gender);
	printf("Firstname:%s\n", ptrStudent->fname);
	printf("Lastname:%s\n", ptrStudent->lname);
	printf("Age:%d\n", ptrStudent->age);
	printf("Id:%d\n", ptrStudent->id);
	printf("\n");
	
	ptrStudent = &stud2;
	printf("Gender:%s\n", ptrStudent->gender);
	printf("Firstname:%s\n", ptrStudent->fname);
	printf("Lastname:%s\n", ptrStudent->lname);
	printf("Age:%d\n", ptrStudent->age);
	printf("Id:%d\n", ptrStudent->id);
	printf("\n");
}
int main (){
	readData();
	outputData();
	return 0;
}