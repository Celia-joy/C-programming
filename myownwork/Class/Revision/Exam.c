#include <stdio.h>
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

int coutDigits(int n){
	if( n >= 0 && n < 10){
		return 1;
	}
	return 1+coutDigits(n/10);
}
int main (){
	int nbr;
	printf("Enter any number: ");
	scanf("%d", &nbr);
	printf("The number of digits of %d is %d: ", nbr, coutDigits(nbr));
	return 0;
}