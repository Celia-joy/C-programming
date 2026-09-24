#include <stdio.h>
/*int main (){
	int arr[4][4]={};
	int i,j;
	for (i=0;i<=3;i++){
		for(j=0;j<=3;j++){
			if(i>=j){
				if((i == j) || (j == 0)){
					arr[i][j]=1;
				}
				else{
					arr[i][j] = arr[i-1][j] + arr[i-1][j-1];
				}
				printf("%d", arr[i][j]);
			}
		}
		printf("\n");	
	}
	return 0;
}*/
/*int main (){
	int a,b,c,d;
	a=15;
	b=10;
	c=(++a) - b;
	printf("a=%d b=%d c=%d\n", a, b, c);
	d=(b++) + a;
	printf("d = %d\n", d);
	printf("%d\n", (c>d)?1:0);
	printf("%d\n", (c<d)?1:0);
	return 0;
}*/

void swap(int a, int b){
	int temp=a;
	a=b;
	b=temp;
}
int main (){
	int x=5, y=10;
	swap(x,y);
	printf("%d %d\n", x,y);
}