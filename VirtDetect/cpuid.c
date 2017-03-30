#include <stdio.h>

int main(){

	unsigned long x[4] = {}, y[4], i, j;
	char c;

	asm volatile(	".byte 15;.byte 162"
			:"=a"(x[0]), "=b"(x[1]), "=c"(x[3]), "=d"(x[2])
			:"0"(0) );

	if (!x[0]) return 1;

	for (i = 1;i < 4;++i){
		for (j = 0;j < 4;++j) {
			printf("\nC is %lu : ", x[i]);
			c = x[i] >> (8 * j);
			if (c < 32) c = 32;
			if (c > 126) c = 126;
			putchar(c);
		}
	}
	puts("\n");

	return 0;
}
