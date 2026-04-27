
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//1.
/*enum Days{
	Mon=1,
	Tue,
	Wed,
	Thur,
	Fri,
	Sat,
	Sun
};
int main(){
	char *names[]={"Mon", "Tue", "Wed", "Thur", "Fri", "Sat","Sun"};
	for (int i=Mon; i<=Sun;i++){
		printf("Day %s is %d\n", names[i-1],i);
	}
	return 0;
}*/

//2.

/*enum Gender{
	Male,
	Female,
	Other
};
int main(){
	enum Gender choice;
	printf("0=Male , 1=Female, 2=Other\n");
	printf("Enter a number of:\n");
	scanf("%d", &choice);
	switch(choice){
		case 0:
			printf("Male");
		case 1:
			printf("Female");
		default:
			printf("Other");
	}
	return 0;	
}*/

//3.
/*enum trafficLight{
	Red,
	Yellow,
	Green 
};
int main (){
	char input[20];	
	enum trafficLight light;
	
	printf("Enter the traffic light: ");
	scanf("%s", input);
	
	if(strcmp(input,"Red")==0){
		light = Red;
	}
	
	else if(strcmp(input,"Yellow")==0){
		light = Yellow;
	}
	else if(strcmp(input,"Green")==0){
		light = Green;		
	}
	else{
		printf("Invalid input\n");
		return 1;
	}
	
	if(light == Red){
		printf("Start");
	}
	else if(light == Yellow){
		printf("Ready");		
	}
	else {
		printf("Go");
	}
	return 0;	
}*/

//4.
/*enum Priority {
	Low =10,
	Medium=20,
	High=30,
	Next
};
int main (){
	printf("Low is %d\n", Low);
	printf("Medium is %d\n", Medium);
	printf("High is %d\n", High);
	printf("Next is %d\n", Next);
	return 0;
}*/

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

/*enum Color {
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
}*/

//9.

/*enum GameState {
	Idle,
	Playing,
	Paused,
	GameOver
};

int main (){
	enum GameState state = Idle;
    for (int i=0;i<5;i++){
	    if(state == Idle){
		   printf("Idle\n");
		   state = Playing;
	    }
	else if(state == Playing){
		printf("Playing\n");
		state = Paused;
	    }
	else if (state == Paused){
		printf("Paused\n");
		state = Playing;
	    }
	else if(state == GameOver){
		printf("Game Over\n");
		break;
	    }
    }
	return 0;
}*/

//10.
/*enum StatusCode{
	Success,
	NotFound,
	Error
};
void printStatusCode(enum StatusCode code){
	if(code == Success){
		printf("Success\n");
	}
	else if (code == NotFound){
		printf("Not Found\n");
	}
	else {
		printf("Error");
	}
}
int main (){
	printStatusCode(Success);
	printf("\n");
	printStatusCode(NotFound);
	printf("\n");
	printStatusCode(Error);
	printf("\n");
	return 0;	
}*/

//11.

/*enum HttpStatus{
	OK = 200,
	BadRequest = 400,
	NotFound = 404,
	InternalServerError = 500
};
void categorizeError(int status){
	if(status >= 200 && status <= 300){
		printf("Success\n");
	}
	else if (status >= 400 && status <=500 ){
		printf("Client Error");
	}
	else if(status >= 500){
		printf("Internal Server Errors");
	}
	else {
		printf("Unknown");
	}
}
int main(){
	categorizeError(404);
	printf("\n");
	categorizeError(OK);
	printf("\n");
	categorizeError(505);
	printf("\n");
	categorizeError(101);
	printf("\n");
	return 0;
}*/


//12.
/*enum LogLevel{
	Debug,
	Info,
	Warning,
	Error
};

char *logNames[] = {"Debug","Info","Warning", "Error"};
char *logLevelToString(enum LogLevel level){
	return (char*) logNames[level];
}
enum LogLevel stringToLoglevel(char *str){
	for (int i=0; i<=3; i++){
		if(strcmp(str,logNames[i])==0){
			return i;
		}
	}
	return -1;
}
int main (){
	printf("%s\n", logLevelToString(Warning));
	enum LogLevel level = stringToLoglevel("Error");
	printf("%d\n", level);
	return 0;
}*/

//13.
/*enum Command {
	CMD_START,
	CMD_STOP,
	CMD_PAUSE,
	CMD_RESTART
};
int main (){
	int input;
	enum Command cmd = input;
	printf("0=CMD_START, 1=CMD_STOP, 2=CMD_PAUSE, 3=CMD_RESTART\n");
	printf("Enter a number:");
	scanf("%d", &input);
	if(input == 0){
		printf("CMD_START");
	}
	else if(input == 1){
		printf("CMD_STOP");
	}
	else if(input == 2){
		printf("CMD_PAUSE");
	}
	else if(input == 3){
		printf("CMD_RESTART");
	}
	else {
		printf("Invalid input");
	}
	return 0;
}*/

//14.
/*enum{
	Rows = 3,
	Cols = 4
};
int main (){
	int matrix[Rows][Cols];
	printf("The matrix size: %d*%d\n", Rows,Cols);
	for(int i=0;i<=Rows;i++){
		for(int j=0; j<Cols;j++){
			matrix[i][j] = i+j;
			printf("%d", matrix[i][j]);
		}
		printf("\n");
	}
	return 0;
}*/
//15.
enum AppConfig{
	MODE_DEVELOPMENT,
	MODE_STANDING,
	MODE_PRODUCTION
};
void logMessage(enum AppConfig mode){
	if(mode == MODE_DEVELOPMENT){
		printf("You are now on developing mode");
	}
	else if(mode == MODE_STANDING){
		printf("Solving errors");
	}
	else if (MODE_PRODUCTION){
		printf("You are now in production mode");
	}	
	else{
		printf("Unknown mode");
	}
}
int main (){
	enum AppConfig mode;
	printf("Enter the mode: ");
	scanf("%d", &mode);
	logMessage(mode);
	return 0;	
}




