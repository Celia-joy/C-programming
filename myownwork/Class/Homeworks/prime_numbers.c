#include <stdio.h>

int isDivisible(int num, int divisor) {
    if (divisor * divisor > num) {
        return 0;          
    }
    if (num % divisor == 0) {
        return 1;          
    }
    return isDivisible(num, divisor + 1);
}

int isPrime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    
   
    return !isDivisible(n, 3);
}

int main() {
    int num;
    
    printf("Enter a positive integer: ");
    scanf("%d", &num);
    
    if (num < 0) {
        printf("Negative numbers are not considered prime.\n");
        return 1;
    }
    
    if (isPrime(num)) {
        printf("%d is a PRIME number.\n", num);
    } else {
        printf("%d is NOT a prime number.\n", num);
    }
    
    return 0;
}