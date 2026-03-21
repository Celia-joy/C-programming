#include <stdio.h>
int main () {
	int A[3][3] = {{5, 8, 3}, {7, 2, 9}, {1, 6, 4}};
	int B[3][3] = {{1, 2, 1}, {3, 0, 4}, {2, 5, 1}};
	int C[3][3];
	 for (int i =0; i<3; i++){
	 	for(int j =0; j<3; j++){
	 		C[i][j] = A[i][j] - B[i][j];
		 }
	 }
	 for (int i =0; i<3; i++){
	 	for(int j =0; j<3; j++){
	 		printf("Matrix A[%d][%d]: %d\n ", i, j, A[i][j]);
	 		printf("Matrix B[%d][%d]: %d\n ", i, j, B[i][j]);
	 		printf("Subtraction matrix C[%d][%d]: %d\n", i, j, C[i][j]);
		 }
	 }	 
	 return 0;
}