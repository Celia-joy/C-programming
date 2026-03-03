#include <stdio.h>
int main(){
	int number [6] = {1, 2, 3, 4, 5, 6};
	int n_nbr[6];
	n_nbr [0] = number[0]*number[1];
	n_nbr [1] = number[0]*number[2];
	n_nbr [2] = number[1]*number[3];
	n_nbr [3] = number[2]*number[4];
	n_nbr [4] = number[3]*number[5];
	n_nbr [5] = number[4]*number[5];
	for(int i = 1; i <6 ; i++){
		printf("%d, ", n_nbr[i]);
	}
	return 0;
}