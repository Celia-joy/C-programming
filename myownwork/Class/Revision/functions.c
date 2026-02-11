#include <stdio.h>
/*void displayMessage (char name []){
	printf("Hello %s", name);
}
int main(){
	char name[50];
	printf("Enter your name: ");
	scanf("%s", name);
	displayMessage(name);
	return 0;
}*/

/*int squareNumber (num){
	return num*num;
}
int main (){
	int num;
	printf("Enter any number: ");
	scanf("%d", &num);
	printf("The square of %d is %d", num, squareNumber(num));
	return 0;
}*/
int area (int length, int width){
	return length*width;
}
int main (){
	int length;
	int width;
	printf("Enter length of rectangle: ");
	scanf("%d", &length);
	printf("Enter width of rectangle: ");
	scanf("%d", &width);
	printf("The area of rectangle is: %d ", area(length, width));
	return 0;	
}