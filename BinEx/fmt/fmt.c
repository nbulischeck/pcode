#include <stdio.h>

int main(void){
    char buffer[32];
    fgets(buffer, 32, stdin);
    printf(buffer);
}
