
#include <stdio.h>
int gcd(int a, int b){
	if(b == 0){
		return a;
	}
	return gcd(b, a%b);
}
int main(){
	int a, b;
	

	printf("Enter the first positive number:  ");
	scanf("%d", &a);
	printf("Enter the second positive number:  ");
	scanf("%d", &b);
	int result = gcd(a,b);
	
		if (a == 0 || b== 0){
		printf("The GCD of 0 and 0 is undefined");
		return 1;
	}
	else if (a < 0 || b < 0){
		printf("Please enter a positive number");
		return 1;
	}
	else {
		printf("The GCD of %d and %d is %d ", a, b, result);
	}
	return 0;
}