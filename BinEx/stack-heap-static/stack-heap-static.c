#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char string[] = "Hello, world!";
static int also_a_global_variable;

void foo(void){
    int bar = 0;
    char baz[32];

    memset(baz, 0, 32);

    baz[0] = 'c';

    return;
}

int main(void){
    char *memory = malloc(16);
    free(memory);
    return 0;
}
