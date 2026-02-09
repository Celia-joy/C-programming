#include <stdio.h>

/* 1. Hello World function */
void displayMessage() {
    printf("Hello World\n");
}

/* Extra function (fn2) */
void fn2() {
    printf("This is fn2 function\n");
}

/* 2. Display name function */
void displayName(char name[]) {
    printf("Name: %s\n", name);
}

/* 3. Square of a number */
int SquareOfNumber(int number) {
    return number * number;
}

/* 4. Area of rectangle */
int rectangle(int length, int width) {
    return length * width;
}

int main() {
    // 1
    displayMessage();
    fn2();

    // 2
    char name[] = "Gaju";
    displayName(name);

    // 3
    int number = 12;
    int square = SquareOfNumber(number);
    printf("The square of %d is %d\n", number, square);

    // 4
    int length = 12;
    int width = 10;
    int area = rectangle(length, width);
    printf("The area of %d and %d is %d\n", length, width, area);

    return 0;
}
