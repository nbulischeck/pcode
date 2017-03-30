#include <stdio.h>

int main(){
#if defined (__x86_64__) || defined(_M_X64)
	printf("64 BIT!\n");
#elif defined (__i386) || defined(_M_IX86)
	printf("32 BIT!\n");
#endif
	return 0;
}
