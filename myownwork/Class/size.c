#include <stdio.h>

int main() {
    printf("char       : %zu\n", sizeof(char));
    printf("short      : %zu\n", sizeof(short));
    printf("int        : %zu\n", sizeof(int));
    printf("long       : %zu\n", sizeof(long));
    printf("long long  : %zu\n", sizeof(long long));
    printf("float      : %zu\n", sizeof(float));
    printf("double     : %zu\n", sizeof(double));
    printf("void*      : %zu\n", sizeof(void*));   
    return 0;
}