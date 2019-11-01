#include <stdio.h>
#include <unistd.h>

void shell(void){
    execve("/bin/bash", NULL, NULL);
}
void unsafe(void){
    char buffer[16];
    fgets(buffer, 32, stdin);
}
void safe(void){
    char buffer[16];
    fgets(buffer, 16, stdin);
}
int main(void){
    safe();
    unsafe();
}
