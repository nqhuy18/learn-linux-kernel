#include <stdio.h>
#include <signal.h>

void myhandler(int sig) {
    printf("Hello world Huy\n");
}
void main () {
    signal(SIGINT, myhandler);
    while(1) {
    }
}