#include <stdio.h>
int main () {
int n = 10;
int array [10][10];
for (int rows = 0; rows < n; rows++){
	for (int cols = 0; cols <= rows; cols++){
		if(cols == 0 || cols == rows){
			array[rows][cols] = 1;
		}
		else{
			array[rows][cols] = array[rows-1][cols-1] + array[rows-1][cols];
		}
	}
}
for (int rows =0; rows < n; rows++){
	for(int cols = 0; cols <= rows; cols++){
		printf("%d", array[rows][cols]);
	}
	printf("\n");
}

	return 0;
}