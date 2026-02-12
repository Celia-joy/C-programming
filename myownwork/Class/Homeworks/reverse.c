#include <stdio.h>
void reverse (char str[], int id){
	if(str[id] == '\0'){
		return;
	}
	reverse(str, id + 1);
	printf("%c", str[id]);
}
int main (){
	char str[100];
	printf("Enter a string: ");
	scanf("%s", str);
	printf("Reversed string: ");
	reverse(str, 0);
	return 0;
}