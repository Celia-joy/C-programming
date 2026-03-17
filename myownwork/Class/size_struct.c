#include <stdio.h>

int main() {
    struct S {
        int a, b;
        char c;
        double d;
    };

    struct S S1, S2;

    printf("Size of S1: %zu bytes\n", sizeof(S1));
    printf("Size of S2: %zu bytes\n", sizeof(S2));

    return 0;
}