#include <stdio.h>
//Call by value
/*void fn(int x){
	x= 10;
	printf("%d\n", x);
}
int main (){
	int a = 5;
	printf("%d\n", a);
	 fn(a);
	return 0;
}*/

//Call by reference
void fn (int *x){
	*x= 10;
	printf("%d", *x);
}
int main (){
	int a = 5;
	int *ptr = &a;
	fn(&a);
	return 0;
}