#include <stdio.h>
int main (){
	int students;
	int i;
	
	printf("Enter number of students: ");
	scanf("%d", &students);
	
	int marks[students];
	char grade[students];
	
	for ( i = 0; i < students; i++){
		printf("Enter the marks of student %d : ", i+1);
		scanf("%d", &marks[i]);
	if(marks[i] <= 100 && marks[i] >= 90 ){
		grade[i] = 'A';
	}
	else if(marks[i] <= 89 && marks[i] >= 80 ){
		grade[i] = 'B';
	}
	else if(marks[i] <= 79 && marks[i] >= 70 ){
		grade[i] = 'C';
	}
	else if(marks[i] <= 69 && marks[i] >= 60 ){
		grade[i] = 'D';
	}
	else if(marks <= 59 && marks >= 50 ){
		grade[i] = 'E';
	}
	else {
		grade[i] = 'F';
	}
	}
	printf("The results of students: \n  ");
	for (i = 0; i < students; i++){
		printf("The marks of student %d is %d and his grade is %c\n ", i+1, marks[i], grade[i]);
		
	}

	return 0;
}