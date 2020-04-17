#include "stdio.h"

int main(int argc, char **args) {
	int w = 5;
	int h = 5;

	for(int y=0; y<h; y++) {
		for(int x=0; x<w; x++) {
			printf("(%d, %d)\n", x, y);
			int R = x+(w-1)*y;
			int L = R-1;
			int D = (w-1)*h+x+w*y;
			int U = D-w;

			if(x != w-1) 	printf("\tR = %d\n", R);
			if(x != 0) 		printf("\tL = %d\n", L);
			if(y != h-1) 	printf("\tD = %d\n", D);
			if(y != 0)	 	printf("\tU = %d\n", U);
		}
	}



	return 0;
}