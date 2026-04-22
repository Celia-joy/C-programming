#include <stdio.h>
int power(int n1, int n2){
	int result = 1;
	for(int i=1; i<=n2; i++){
		result*=n1;
	}
	return result;
}
int main (){
	int n1,n2;
	printf("Enter the base:");
	scanf("%d", &n1);
	printf("Enter the exponent:");
	scanf("%d", &n2);
	printf("%d power %d is %d", n1,n2,power(n1,n2));
	return 0;
}