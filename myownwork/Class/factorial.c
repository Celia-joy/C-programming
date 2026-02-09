#include <stdio.h>
	long int f (int n){
		//base case
		if(n==0 || n==1){
			return 1;
		}
		else {
			//recursive step
			return n*f(n-1);
		}
	}
int main(){
	int number;
	printf("Enter a number: ");
	scanf("%d", &number);
	long int factorial = f(number);
	printf("\n The factorial of %d is %d", number, factorial);
	return 0;
}