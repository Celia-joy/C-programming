#include <stdio.h>
//1.
enum Color{
		RED,
		BLUE,
		GREEN
	};
int main(){
	enum Color current = BLUE;
	printf("%d", current);
	return 0;	
}

//2.
/*enum Month{
	JAN=1,
	FEB,
	MAR,
	APR
};
int main (){
	printf("%d\n", JAN);
	printf("%d\n", FEB);
	printf("%d\n", MAR);
	printf("%d\n", APR);
}*/

//3.
/*enum Grade {
	A,
	B,
	C,
	D,
	E,
	F
};
int main (){
	enum Grade grades = B;
	switch(grades){
		case A:
			printf("Excellent\n");
			break;
		case B:
			printf("Good\n");
			break;
		default:
		printf("Try harder");		
	}
	return 0;
}*/
