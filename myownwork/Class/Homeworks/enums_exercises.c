
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//5.

/*enum ErrorCode {
	OK=0,
	FileNotFound=1,
	AccessDenied=2	
};
void printCode(enum ErrorCode code){
	if (code == OK ){
		printf("Successful");
	}
	else if (code == FileNotFound){
		printf("404");
	}
	else if(code == AccessDenied){
		printf("Not authorized");
	}
}
int main (){
	printCode(OK);
	printf("\n");
	printCode(FileNotFound);
	printf("\n");
	printCode(AccessDenied);
	return 0;
}*/

//6.
/*enum Days{
	Monday=1,
	Tue,
	Wed,
	Thur,
	Fri,
	Sat,
	Sun
};
void printDays(enum Days day){
	if(day >=1 && day <=5){
		printf("1");
	}
	else{
		printf("0");
	}
}
int main(){
	enum Days day = Monday;
	printDays(day);
	return 0;
}*/

//7.

/*enum FileAccess {
    Read = 1,
    Write = 2,
    Execute = 4
};

int main() {
    int permission = Read | Write;  
    if (permission & Write) {
        printf("'Write' permission included\n");
    } else {
        printf("'Write' permission NOT included\n");
    }

    return 0;
}*/

//8.

enum Color {
	Red,
	Green,
	Blue
};
int main (){
	char input[20];
	char *colorNames[] = {"Red", "Green","Blue"};
	printf("Enter a color: ");
	scanf("%s", input);
	
	enum Color color;
	int found =0;
	
	for (int i=0; i<3; i++){
		if(strcmp(input, colorNames[i])==0){
			color=i;
			found=1;
			break;
		}
	}
	if(found){
		printf("Enum value = %d\n", color);
	}
	else {
		printf("Invalid color\n");
	}
	return 0;
}

