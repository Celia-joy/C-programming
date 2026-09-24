#include <stdio.h>
int main (){
	int a=10, b=20;
	printf("a=%d, b=%d\n", a*2, b+a);
	a=a+b;
	b=a-b;
	a=a-b;
	printf("a=%d, b=%d\n", a, b);
	return 0;
}