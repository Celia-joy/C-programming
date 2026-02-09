#include <stdio.h>
#include <string.h>
int main() {
    char C[] = "abcd";
    printf("%d", sizeof(C));

    char C1[50] = "abcd";
    printf("\n%d", sizeof(C1));

    char C2[] = {'a', 'b', 'c', 'd', '\0'};
    printf("\n%d", sizeof(C2));

    char Cn[5] = {'a', 'b', 'c', 'd', '\0'};
    printf("\n%d", sizeof(Cn));
    printf("\n");
    
    char name[30];
    printf("\nEnter name:");
    gets(name);
    puts(name);
    printf("\n");
    
    char D[] = "abcd";
    printf("%d\n", strlen(D));
    printf("\n");
    
    char E[10] = "World";
    char Z[] = "Hello"; 
	char message = strcpy(Z, E);                         
    puts(message);
	
	
	char R[20];
	strcat(Z,E);
	puts(R); 
	
	int element[3];
	
	printf("Give the first element:");
	scanf("%d", &element[0] );
	
	printf("Give the second element:");
	scanf("%d", &element[1]);
	
	printf("Give the third element:");
	scanf("%d", &element[2]);
	
	printf("The values store into the array are: %d %d %d\n", element[0], element[1], element[2]);
	printf("The values store into the array in reverse are: %d %d %d", element[2], element[1], element[0]);
    return 0;
}