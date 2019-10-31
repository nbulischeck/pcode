#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int num){
    printf("Bye!\n");
    exit(-1);
}

void setup(void){
    signal(SIGALRM, handler);
    alarm(5);

    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int main(int argc, char **argv){
    setup();

    struct {
        char buffer[16];
        int foo;
    } s;

    s.foo = 0;

    gets(s.buffer);

    printf("%x\n", s.foo);

    if (s.foo != 0){
        printf("You win!\n");
    }

    return 0;
}
