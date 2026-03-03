#include <stdio.h>

//1.
/*int main (){
	int x= 10;
	int *p= &x;
	printf("%p\n", p);
	printf("%d\n", *p);
	return 0;
}*/

//2.
/*int main (){
	int x= 10;
	int *p= &x;
	*p = 25;
	printf("%p\n", p);
	printf("%d\n", *p);
	return 0;
}*/

//3.
/*int main (){
	int x = 10;
	float y = 25;
	int *p1 = &x;
	float *p2 = &y;
	printf("%p\n", p1);
	printf("%p\n", p2);
	printf("%d\n", *p1);
	printf("%p\n", *p2);
	return 0;
}*/

//4.
/*int main (){
	int *intPtr;
	float *floatPtr;
	char *charPtr;
	printf("%zu\n", sizeof(intPtr));
	printf("%zu\n", sizeof(floatPtr));
	printf("%zu\n",sizeof(charPtr));
	return 0;
}*/

//5.
/*int sum_array(int *arr, int length){
	int sum = 0;
	for(int i = 0; i < length; i++){
		sum +=*(arr + i);
	}
	return sum;
}
int main (){
	int A[5]=  {1,2,3,4,5};
	int length = sizeof(A)/sizeof(A[0]);
	printf("The sum of all elements in the array is: %d", sum_array(A,length));
}*/

//6.
/*int increment_value(int *ptr){
    return (*ptr)++;	
}
int main (){
	int ptr = 25;
	increment_value(&ptr);
	printf("The incremented value is %d", ptr);
	return 0;
}*/

//7.
/*int main (){
	int B[5] = {10, 20, 30, 40, 50};
	int *p = B;
	for(int i = 0; i<5; i++){
		printf("%d\n" , *(p+i));
	}
	return 0;
}*/

//8.
/*int main (){
	int C[5] = {100, 200, 300, 400, 500};
	int *p = C;
	printf("%d\n", C[2]);
	printf("%d\n", *(C+2));
	printf("%d\n", *(p+2));
	return 0;
}*/

//9.
/*int main (){
	int D[5] = {1000, 2000, 3000, 4000, 5000};
	int *p1 = D;
	int *p2 = D+4;
	while(p1<p2){
		int stor = *p1;
		*p1 = *p2;
		*p2 = stor;
		p1++;
		p2--;
	}
	for (int i=0; i<5; i++){
		printf("%d\n", *(D+i));
	}
	return 0;
}*/

//10.
/*int main (){
	char E[5] = "Hello";
	char *p = E;
	for(char i = 0; i<5; i++){
		printf("%c\n", *(p+i));
	}
	return 0;
}*/