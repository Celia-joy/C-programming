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

/*int main (){
	float  C,F;
	printf("Enter temperature in degree Celcius: ");
	scanf("%f", &C);
	F = (C*9/5)+32;
	printf("%.2f in Fahrenheit is %.2f\n",C, F);
	return 0;
}*/

//11.
/*int main () {
	int arr[10][10];
	for (int rows=0; rows<10; rows++){
		for (int cols=0; cols<=rows; cols++){
			if (cols == 0 || cols == rows){
				arr[rows][cols] = 1;
			}
			else {
				arr[rows][cols] = arr[rows-1][cols-1] + arr[rows-1][cols];
			}
		}
	}
	for (int rows=0; rows<10; rows++){
		for(int cols=0; cols<=rows; cols++){
			printf("%d" ,arr[rows][cols]);
		}
		printf("\n");
	}	
	return 0;
}*/

//12.

/*int isPrime (int n){
	if (n<2){
		return 0;
	}
	if (n==2){
		return 1;
	}
	if (n%2 == 0){
		return 0;
	}
	for (int i = 3; i<=n/2; i++){
		if (n%i == 0){
			return 0;
		}
	}
	return 1;
}

	int main (){
		int n;
		printf("Enter any positive number: ");
		scanf("%d", &n);
		if(isPrime(n)){
			printf("%d is prime", n);
		}
		else {
			printf("%d is not prime", n);
		}
		return 0;
	}*/
	
//13.

/*int main (){
	int n;
	printf("Enter the size of the array: ");
	scanf("%d", &n);
	int arr[n];
	for (int i=0; i<n; i++){
		printf("Enter element %d: ", i+1);
		scanf("%d", &arr[i]);		
	}
	int min = arr[0];
	for (int i= 0; i<n; i++){
		if (arr[i] < min){
			min = arr[i];
		}		
	}
	printf("The smallest number is %d", min);
	return 0;
}*/
/*int minimum (int arr[], int size){
	int min = arr[0];
	for (int i=0; i<size; i++){
		if (arr[i] < min){
			min = arr[i];
		}
	}
	return min;
}
int main () {
	int n;
	printf("Enter the size of the array: ");
	scanf("%d", &n);
	int arr[n];
	for (int i=0; i<n; i++){
		printf("Enter element %d: ", i+1);
		scanf("%d", &arr[i]);		
	}
	printf("The smallest number is: %d", minimum(arr, n));
	return 0;	
}*/

//14.

/*int gcd(int a, int b){
	if (b==0){
		return a;
	}
	return gcd(b, a%b);
}
int main () {
	int a, b;
	printf("Enter any 1st positive number: ");
	scanf("%d", &a);
	printf("Enter any 2nd positive number: ");
	scanf("%d", &b);
	printf("The gcd of %d and %d is %d", a,b, gcd(a,b));
	return 0;
}*/

//15. 
/*int maximum(int arr[], int size){
	int max = arr[0];
	for (int i=1; i<size; i++){
		if (arr[i] > max){
		    max= arr[i];
		}
	}
	return max;
}
int main () {
	int n;
	printf("Enter the size of the array: ");
	scanf("%d", &n);
	int arr[n];
	for (int i=0; i<n; i++){
		printf("Enter element %d: ", i+1);
		scanf("%d", &arr[i]);		
	}
	printf("The largest number in the array is %d ", maximum(arr, n));
	return 0;
}*/

//16.
/*void reverse (char str, int id){
	if (str[id] == '\0'){
		return;
	}
	reverse(str, id+1);
	printf("%c", str[id])
	
}
int main (){
	char str[100];
	printf("Enter the string: ");
	scanf("%s", str);
	printf("The original string is: ", str);
	printf("The reversed string is: ");
	reverse(str, 0);
	return 0;	
}*/

//17. 

int decimalToBinary(int n){
	if(n == 0){
		return 0;
	}
	decimalToBinary(n/2);
	printf("%d", n%2);
}
 int main (){
 	int n;
 	printf("Enter a number: ");
 	scanf("%d", &n);
 	if (n == 0){
 		printf("0");
	 }
	 else{
	 	decimalToBinary(n);	 	
	 }
 	return 0;
 }


	
		
