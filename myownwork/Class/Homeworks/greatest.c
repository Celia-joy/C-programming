#include <stdio.h>
int findLargest(int arr[], int size){
	if (size == 1){
		return arr[0];
	}
	int max = arr [0];
	if(arr[size-1] > max){
		return arr[size-1];
	}
	return max;	
}
int main (){
	int A[5]= {2,3,0,5,7};
	printf("The largest number is : %d", findLargest(A, 5));
	return 0;
}