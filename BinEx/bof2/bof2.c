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
    alarm(120);

    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void win(void){
    printf("You win!\n");
    exit(0);
}

int main(int argc, char **argv){
    setup();

    struct {
        char buffer[16];
        void (*fp)(void);
    } s;

    s.fp = NULL;

    printf("win: %p\n", win);

    gets(s.buffer);

    printf("fp: %p\n", s.fp);
    if (s.fp == (void *)win)
        s.fp();

    exit(-1);
}
