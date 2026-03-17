# include <stdio.h>
//1.How do you define a struct to store
// a student's name, age, and location

/*struct student {
	char name[10];
	int age;
	char location[10];
};*/
	

//2. How do you access structure 
//members using the dot (.) operator?

/*int main (){
	struct student s;
	printf("Enter your name: ");
	scanf("%s", s.name);
	printf("Enter your age: ");
	scanf("%d", &s.age);
	printf("Enter your location: ");
	scanf("%s", s.location);
	printf("Age: %d\n", s.age);
	printf(" Name: %s\n", s.name);
	printf("Location: %s", s.location);
	return 0;
}*/

//3. What are the different ways to initialize 
//a structure variable at the time of declaration?

/*struct student s {"Celia", 15, "Huye"};
struct student s {.name = "Alice", .age = 15, .location = "Huye"};*/

// 4. How do you create an array to store details of 10 students?

/*struct student S[10] {
	{"IHIRWE", 15, "Huye"},
	{"Celia", 15, "Huye"},
	{"Joy", 15, "Huye"},
	{"NZIZA", 16, "Huye"},
	{"Ange", 16, "Huye"},
	{"SIMBI", 15, "Huye"},
	{"Lys", 15, "Huye"},
	{"Anais", 15, "Huye"},
	{"Christa", 15, "Huye"},
	{"Seraphin", 15, "Huye"},
	
}*/

//5. How do you find the total memory 
//size occupied by a structure using sizeof?

/*struct Student s1;
printf("Size of struct: %zu bytes\n", sizeof(s1));*/

//6.How do you access structure members 
//using the arrow (->) operator?
/*int main (){
	struct student {
		char name[10];
		int age;
		char location[10];		
	};
	struct student S = {"Celia", 15, "Huye"};
    struct student *ptr = &S;
    printf("%s\n", ptr->name);
    printf("%d\n", ptr->age);
    printf("%s", ptr->location);   
    return 0;	
}*/

//7. What is the difference between passing 
//a structure by value and passing it by address?
/*int main () {
	struct student {
		char name[10];
		int age;
		char location[10];		
	};
	struct student S = {"Celia", 15, "Huye"};
	struct student *ptr = &S;
	//By value
	printf("%s\n", S.name);
	printf("%d\n", S.age);
	printf("%s\n", S.location);
	//By address
	printf("%s\n", ptr->name);
	printf("%d\n", ptr->age);
	printf("%s", ptr->location);
	return 0;	
}*/
//8.  How do you define a structure within another structure?
/*#include <string.h>
int main () {
	struct Address {
    char street[100];
    char city[50];
    char country[50];
};
struct student {
		char name[10];
		int age;
		char location[10];
		struct Address address;		
	};
	struct student S = {"Celia", 15, "Huye"};
	strcpy(S.address.city , "Huye City");
	strcpy(S.address.street , "KG.st.103");
	strcpy(S.address.country , "Rwanda"); 
	printf("%s\n", S.name);
	printf("%d\n", S.age);
	printf("%s\n", S.location);
	printf("%s\n", S.address.street);
	printf("%s\n", S.address.city);
	printf("%s\n", S.address.country);
	return 0;
}*/

//10. Can you assign one structure 
//variable directly to another?
// What does this operation do?
/*int main () {
struct student {
		char name[10];
		int age;
		char location[10];	
	};
	struct student S = {"Celia", 15, "Huye"};
	struct student S1;
	S1 = S;
	printf("%s\n", S1.name);
	printf("%d\n", S1.age);
	printf("%s", S1.location);
	return 0;
	//=> Yes you can
}*/

//12. Why does sizeof(struct {char c; int i;})
// often return 8 instead of 5?
//This is due to struct padding 

//14. How can typedef simplify 
//structure declaration and pointer usage?

//You don't have to write struct again 
//Ex:
 int main () {
	typedef struct {
		char name[10];
		int age;
		char location[10];		
	} student;
    student S = {"Celia", 15, "Huye"};
	printf("%s\n", S.name);
	printf("%d\n", S.age);
	printf("%s\n", S.location);
	return 0;
}







