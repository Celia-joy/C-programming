#include <stdio.h>
union MyUnion {
	int i;
	char c;
	float f;
} u1, u2, u3;
int main (){
	printf("U1 data input:\n__________________\n");
	printf("I:");
	scanf("%d", &u1.i);
	printf("C:");
	scanf("%d", &u1.c);
	printf("F:");
	scanf("%d", &u1.f);
	printf("\n");
	
	printf("U2 data input:\n__________________\n");
	printf("I:");
	scanf("%d", &u2.i);
	printf("C:");
	scanf("%d", &u2.c);
	printf("F:");
	scanf("%d", &u2.f);
	
	printf("U3 data input:\n__________________\n");
	printf("I:");
	scanf("%d", &u3.i);
	printf("C:");
	scanf("%d", &u3.c);
	printf("F:");
	scanf("%d", &u3.f);
	
	printf("%d", )
	return 0;
	
}