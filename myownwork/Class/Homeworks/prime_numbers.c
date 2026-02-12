#include <stdio.h>
/*int main (){
	int i;
	int n;
	printf("Enter any number: ");
	scanf("%d", &n);
	if(n<=1){
		printf("Number is not prime");
	}
	for (i=2; i<n;i++){
		if(n%i == 0){
			printf("%d isn't prime", n);
			break;
		}
	}
	if(i == n){
		printf("%d is prime", n);
	}
	return 0;
}*/
int isPrime (int n, int i){
	if(n<= 1){
		return 0;
	}
	if (n == i){
		return 1;
	}
	if (n % i == 0){
		return 0;
	}
	return isPrime(n, i+1);
}
int main (){
	int n;
	int i;
	printf("Enter any number: ");
	scanf("%d", &n);
	if (isPrime(n, 2)){
		printf("%d is prime", n);
	}
	else {
		printf("%d isn't prime", n);
	}
	return 0;
}
