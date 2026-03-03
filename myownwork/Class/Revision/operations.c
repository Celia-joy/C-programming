#include <stdio.h>
int main (){
	int x, y, z;
	int a = 48;
	int b = 49;
	int c = (a>b)?b:a;
	printf("Enter the first number: ");
	scanf("%d", &x);
	printf("Enter the second number: ");
	scanf("%d", &y);
	printf("Enter the third number: ");
	scanf("%d", &z);
	if(x>y && x>z){
		printf("The greatest number is: %d\n", x);
	}
	else if(y>x && y>z){
		printf("The greatest number is: %d\n", y);
	}
	else{
		printf("The greatest number is: %d\n", z);
	}
	printf("The left shift of a is %d\n", a<< 2);
	printf("The right shift of a is %d\n", a>> 2);
	printf("%d", c);	
	return 0;
}