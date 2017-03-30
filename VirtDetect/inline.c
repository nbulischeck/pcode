#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	char *str = "Hello, world!\n";
	char *msg = malloc(100);
	int len = strlen(str), fd;

	printf("Original string is: ");
	fflush(stdout);

	/* Prints original string to console */
	asm("int $0x80;"::"a"(4), "b"(1), "c"(str), "d"(len));

	/* Creates "File.txt" */
	asm("int $0x80;"::"a"(8), "b"("File.txt"), "c"(0777));

	/* Grab the file descriptor */
	asm("movl %0, %%eax;": "=a"(fd));

	/* Write string to file */
	asm("int $0x80;"::"a"(4), "b"(fd), "c"(str), "d"(len));

	/* Close file */
	asm("int $0x80;"::"a"(6), "b"(fd));

	/* Reopen file */
	asm("int $0x80;"::"a"(5), "b"("File.txt"), "c"(0), "d"(0777));

	/* Grab the file descriptor again */
	asm("movl %0, %%eax;": "=a"(fd));

	/* Read the file */
	asm("int $0x80;"::"a"(3), "b"(fd), "c"(msg), "d"(len));

	/* Close file */
	asm("int $0x80;"::"a"(6), "b"(fd));

	printf("String read from file: ");
	fflush(stdout);

	/* Print the message */
	asm("int $0x80;"::"a"(4), "b"(1), "c"(msg), "d"(len));

	free(msg);

	/* Exit */
	asm( 	"movl $1,%eax;"
			"xorl %ebx,%ebx;"
			"int  $0x80"
	);

}
