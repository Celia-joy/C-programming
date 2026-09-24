#include <stdio.h>
#include <stdlib.h>
/*enum Days{
	Mon=1,
	Tue,
	Wed,
	Thur,
	Fri,
	Sat,
	Sun	
};
int main (){
	char *names[] = {"Mon","Tue","Wed","Thur","Fri","Sat","Sun"};
	for(int i=Mon; i<=Sun; i++){
		printf("Day %d is %s\n", i, names[i-1]);	
	}
	return 0;
}*/

/*enum Gender{
	Male,
	Female,
	Others
};
int main (){
	enum Gender choice;
	printf("0: Male, 1:Female, 2:Others\n");
	printf("Enter any number: ");
	scanf("%d", &choice);
	switch(choice){
		case 0:
			printf("Male");
			break;
		case 1:
			printf("Female");
			break;
		case 2:
			printf("Others");
			break;
		default:
			printf("Invalid choice");
			break;
	}
	return 0;
}*/

/*enum TrafficLight{
	Red,
	Yellow,
	Green
};
int main(){
	char input[20];
	enum TrafficLight light;
	printf("Enter the traffic light: ");
	scanf("%s", input);
	if(strcmp(input,"Red")==0){
		light = Red;
	}
	else if (strcmp(input,"Yellow")==0){
		light = Yellow;
	}
	else if (strcmp(input, "Green")==0){
		light = Green;
	}
	else {
		printf("Invalid input");
	}
	if(light == Red){
		printf("Stop");
	}
	else if (light == Yellow){
		printf("Ready");
	}
	else {
		printf("Go");
	}
	return 0;
}*/
/*enum Priority{
	Low=10,
	Medium=20,
	High=30,
	Next
};
int main (){
	printf("Low is %d\n", Low);
	printf("Medium is %d\n", Medium);
	printf("High is %d\n", High);
	printf("Next is %d\n", Next);
	return 0;
}*/
/*enum ErrorCode{
	OK=0,
	FileNotFound=1,
	AccessDenied=2
};
int main(){
	int input;
	enum ErrorCode code;
	printf("0:OK, 1:FileNotFound, 2:AccessDenied\n");
	printf("Enter the code: ");
	scanf("%d", &code);
	if(code == OK){
		printf("Successful");
	}
	else if(code == FileNotFound){
		printf("404");
	}
	else if (code == AccessDenied){
		printf("Unauthorized");
	}
	else{
		printf("Invalid error code");
	}
	return 0;
}*/
/*enum Days{
	Mon=1,
	Tue,
	Wed,
	Thur,
	Fri,
	Sat,
	Sun	
};
void printDays(enum Days day){
	if(day >=1 && day<=5){
		printf("1");
	}
	else{
		printf("0");
	}
}
int main (){
	enum Days day = Sun;
	printDays(day);
	return 0;
}*/

/*enum FileAccess{
	Read=1,
	Write=2,
	Execute=4
};
int main (){
	int permission = Read | Write;
	if(permission & Write){
		printf("'Write' permission included");
	}
	else{
		printf("'Write' permission not included");
	}
	return 0;
}*/

/*union Member{
	int intValue;
	float floatValue;
};
int main(){
	union Member n;
	
	n.intValue=42;
	printf("%d\n", n.intValue);
	
	n.floatValue=3.14;
	printf("%f\n", n.floatValue);
	
	return 0;
}*/

/*struct Student{
	int num_grades;
	float grade[];	
};
int main(){
	int n;
	printf("How many subjects: ");
	scanf("%d", &n);
	struct Student *s = malloc(n*sizeof(float));
	//struct Student *s = calloc(n, sizeof(float));
	s->grade[0]=90;
	s->grade[1]=95;
	s->grade[2]=90;
	
	s->num_grades=n;
	for(int i=0;i<s->num_grades;i++){
		printf("Grade %d: %.1f\n", i+1,s->grade[i]);
	}
	free(s);
	return 0;	
}*/
/*union U {
    int intValue;
    float floatValue;
    double doubleValue;
};
int main(){
	union U num;
    num.intValue = 10;       
    num.floatValue = 3.14;  
    num.doubleValue = 2.71828;
    printf("%d\n", num.intValue); 
	return 0;
}*/

struct Point{
	int x,y;
};
struct Rectangle{
	struct Point upper_left, lower_right;
};
int Area(struct Rectangle R){
	int width = R.lower_right.x - R.upper_left.x;
	int height = R.upper_left.y - R.lower_right.y;
	return width*height;
}
struct Point Center(struct Rectangle R){
	struct Point center;
	center.x = (R.upper_left.x + R.lower_right.x)/2;
	center.y = (R.lower_right.y + R.upper_left.y)/2;
	return center;
}
int main (){
	struct Rectangle R = {{0, 10}, {6, 0}};
	int area = Area(R);
	printf("The area of rectangle is %d\n", area);
	struct Point center = Center(R);
	printf("The center of the rectangle is (%d, %d)\n", center.x, center.y);
	return 0;
}
