#include <stdio.h>
#include <stdint.h>

int main(void){
    uint8_t foo; // 0-255

    foo = 255;     // 2^7-1 or 0111 1111

    printf("Initial Value: %d\n", foo);

    foo = foo + 1; // 2^7   or 1000 0000

    printf("Add one: %d\n", foo);

    return 0;
}
