# include <stdio.h>

/*void greetingMessage(char name[]);
int main (){

	char name[50];
	printf("Enter your name:");
	scanf("%49s", name);
	
	greetingMessage(name);
	return 0;
}

void greetingMessage(char name[]){
	printf("Hello %s!\n", name);
}*/


void sum( int x, int y);
int main (){
	int x, y;
	printf("Enter the first number:  ");
	scanf("%d", &x);
	
	printf("Enter the second number: ");
	scanf("%d", &y);
	
	sum(x,y);
	return 0;
}

void sum(int x, int y){
	int result = x+y;
	printf("The sum of %d and %d is %d", x, y, result);
}