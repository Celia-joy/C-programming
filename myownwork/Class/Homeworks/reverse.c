#include <stdio.h>
/*void reverse (char str[], int index){
if(str[index] == '\0'){
	return;
}
reverse (str, index + 1 );
printf("%c", str[index]);
}
int main (){
	char str[100];
	printf("Enter a string:");
	scanf("%s", &str);
	printf("Reversed string: ");
	reverse (str, 0);
	return 0;
}*/
void reverse (char str[], int id){
	if (str[id] == '\0'){
		return;
	}
	reverse (str , id + 1);
	printf("%c", str[id]);
}
int main (){
	int str[50];
	printf("Enter a string: ");
	scanf("%s", str);
	printf("The reversed string is : ");
	reverse(str, 0);
	return 0;
}