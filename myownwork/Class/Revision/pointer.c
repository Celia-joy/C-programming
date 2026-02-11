#include <stdio.h>
int main (){
	int myAge = 15;
	int*ptr = &myAge;
	printf("%d\n", myAge);
	printf("%p\n", &myAge);
	printf("%p", ptr);
	printf("%d\n", *ptr);
	return 0;
}