#include <stdio.h>
//base case
int natural (int number){
	if (number < 0){
		return 0;
	}
	else if(number > 49){
		return 0;
	}
    else {
    	printf("%d, ", number);
		return natural(number+1);
	}
}

int main (){
	printf("The first 50 natural number are: \n");
	natural(0) ;
	return 0;
}