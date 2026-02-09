/*Names:
1. IHIRWE Celia Joy
2. IHOZO Raissa Flora
3. UMURERWA Amelie Gift 
4. YEHOVAYIRE Moise
*/

#include <stdio.h>

int add(int num1, int num2) {
    return num1 + num2;
}

int subtract(int num1, int num2) {
    return num1 - num2;
}

int multiply(int num1, int num2) {
    return num1 * num2;
}

float divide(int num1, int num2) {
    return (float)num1 / num2;
}

int main() {
    int num1, num2, operation=0;

    printf("Enter the first number: ");
    scanf("%d", &num1);

    printf("Enter the second number: ");
    scanf("%d", &num2);

    while (operation < 1 || operation > 5) {
        printf("\nOperation 1: +\n");
        printf("Operation 2: -\n");
        printf("Operation 3: *\n");
        printf("Operation 4: /\n");
        printf("5. Exit\n");
        printf("Enter the number of operation you want to do: ");
        scanf("%d", &operation);

        if (operation < 1 || operation > 5) {
            printf("Invalid input (Please enter a number between 1 to 5)\n");
        }
    }

    switch (operation) {
        case 1:
            printf("%d + %d = %d\n", num1, num2, add(num1, num2));
            break;
        case 2:
            printf("%d - %d = %d\n", num1, num2, subtract(num1, num2));
            break;
        case 3:
            printf("%d * %d = %d\n", num1, num2, multiply(num1, num2));
            break;
        case 4:
            if (num2 == 0) {
                printf("Impossible - division by zero\n");
            } else {
                printf("%d / %d = %.2f\n", num1, num2, divide(num1, num2));
            }
            break;
        default:
            printf("Exiting...\n");
            break;
    }

    return 0;
}