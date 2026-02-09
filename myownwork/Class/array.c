#include <stdio.h>
int main (){
	int a[5]= {6, 7, 8, 3 ,5};
	int i;
	for(i=0;i<5;i++){
		printf("%p\n", &a[i]);
	}
	printf("\n");
	char c[5]= {
		'g', 't', 'v', 'c', 'h'
	};
	
	for(i=0;i<5;i++){
		printf("%p\n", &c[i]);
	}
	return 0;
}