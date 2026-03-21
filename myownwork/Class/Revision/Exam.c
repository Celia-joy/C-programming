#include <stdio.h>
//1.
/*int main () {
	int A[2][2] = {{5, 6}, {7, 8}};
	int B[2][3] = {{1, 2}, {3, 4}};
	int C[2][2];
	 for (int i =0; i<2; i++){
	 	for(int j =0; j<2; j++){
	 		C[i][j] = A[i][j] - B[i][j];
		 }
	 }
	 for (int i =0; i<2; i++){
	 	for(int j =0; j<2; j++){
	 		printf("Matrix A[%d][%d]: %d\n ", i, j, A[i][j]);
	 		printf("Matrix B[%d][%d]: %d\n ", i, j, B[i][j]);
	 		printf("Subtraction matrix C[%d][%d]: %d\n", i, j, C[i][j]);
		 }
	 }	 
	 return 0;
}*/

//2. 
/*int coutDigits(int n){
	if( n >= 0 && n < 10){
		return 1;
	}
	return 1+coutDigits(n/10);
}
int main (){
	int nbr;
	printf("Enter any positive number: ");
	scanf("%d", &nbr);
	printf("The number of digits of %d is %d ", nbr, coutDigits(nbr));
	return 0;
}*/

//3.

/*int factorial(int n){
	if (n == 1 || n == 0){
		return 1;
	}
	return n*factorial(n-1);
}
int main (){
	int nbr;
	printf("Enter any positive number: ");
	scanf("%d", &nbr);
	printf("The factorial of %d is %d", nbr, factorial(nbr));
	return 0;
}*/

//4.

/*int fiboSequence(int n){
	if (n == 0){
		return 0;
	}
	else if (n == 1){
		return 1;
	}
	else {
		return fiboSequence(n-1) + fiboSequence(n-2);		
	}
}
int main (){
	int nbr;
	printf("Enter any positive number: ");
	scanf("%d", &nbr);
	for (int i =0; fiboSequence(i)<nbr; i++ ){
		printf("The fibonacci sequence less than %d is %d\n", nbr, fiboSequence(i));		
	}
	return 0;
}*/

//5.

/*int main () {
	int arr[2][3][4] = {{{1,2,3,4}, {5,6,7,8}, {9,10,11,12}}, {{13,14,15,16}, {17,18,19,20},{21,22,23,24}}};
	for (int i= 0; i<2; i++){
		for(int j=0; j<3; j++){
			for(int k=0; k<4; k++){
				printf("arr[%d][%d][%d] = %d\n ",i,j,k, arr[i][j][k]);
			}
		}
	}
	return 0;
}*/

//6. 
/*int main (){
	int n;
	printf("The size of the array: ");
	scanf("%d", &n);
	 int A[n];
	 for (int i =0; i<n; i++){
	 	printf("Enter the element %d:  ", i+1);
	 	scanf("%d", &A[i]);
	 }
	 int sum = 0;
	 for (int i=0; i<n;i++){
	 	sum += A[i];
	 }
	 printf("The sum of elements in the array is : %d", sum);
	 
	return 0;
}*/

//7.

/*int main (){
	int A[3] = {2,5,7};
		printf("The values stored in the array are:\n");
	for(int i=0;i<3; i++){
	    printf("%d\n",A[i]);
	}	
	printf("The values stored into the array in reverse are:\n");
	for (int j=2; j>=0; j--){
		printf("%d\n", A[j]);
	}
	return 0;
}*/

//8.

/*int main (){
	int a=10, b=20;
	printf("a=%d, b=%d\n", a*2, b+a);
	a=a+b;
	b=a-b;
	a=a-b;
	printf("a=%d, b=%d\n", a, b);
	return 0;
}*/

//9. 

/*int main (){
	int a, b, c, x;
	printf("Enter the first number: ");
	scanf("%d", &a); 
	printf("Enter the second number: ");
	scanf("%d", &b); 
	printf("Enter the third number: ");
	scanf("%d", &c); 
	x = (a>b) ? a: (b-c);
	printf("%d" ,x);
	return 0;
}*/

//10. 

int main (){
	float  C,F;
	printf("Enter temperature in degree Celcius :");
	scanf("%f", &C);
	F = (C*9/5)+32;
	printf("%.2f in Fahrenheit is %.2f\n",C, F);
	return 0;
}

