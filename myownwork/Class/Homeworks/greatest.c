#include <stdio.h>
int findLargest (int arr[], int size){
	if (size == 1 ){
		return arr[0];
	}
	int max = arr[0];
	if(arr[size-1] > max){
		return arr[size-1];
	}
	return max;
}
int main(){
	int A[5] = {2, 3, 5, 7, 9};
	printf("The largest number in the array is: %d ", findLargest(A, 5));
	return 0;
}