#include <stdio.h>
//base case
int natural (int number){
	if(number <0 || number> 49){
	return 1;
}
else {
	for(number =0; number <= 49; number++){
		return natural(number);
	}
}
}

int main (){
	int number;
	printf("The first 50 natural number are: \n");
	printf("%d" ,natural(number) );
	return 0;
}