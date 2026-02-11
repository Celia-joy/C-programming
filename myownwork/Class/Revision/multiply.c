#include <stdio.h>
int main (){
	int num[6] = {1,2,3,4,5,6};
	int new[6];
	for(int i =0; i<6; i++){
		printf("%d  ", num[i]);
	}
	printf("\n ");
	new[0] = num[0]*num[1];
	new[1] = num[0]*num[2];
	new[2] = num[1]*num[3];
	new[3] = num[2]*num[4];
	new[4] = num[3]*num[5];
	new[5] = num[4]*num[5];
	for(int i =0; i<6; i++){
		printf("%d  ", new[i]);
	}
	printf("\n ");
	
	int A[3][3]= {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
		};
		int Sum;
		printf("The element to be summed are the lower parts of the triangle\n");
		printf("%d %d %d\n", A[1][0], A[2][0], A[2][1]);
		Sum = A[1][0] + A[2][0] + A[2][1];
		printf("The sum of lower parts of the triangle is: %d", Sum);
	return 0;
}