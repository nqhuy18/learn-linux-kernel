#include <stdio.h>
#include <signal.h>
#include <string.h>
void main () {
    sigset_t my_sig_set;
    sigemptyset(&my_sig_set);

    sigaddset(&my_sig_set, SIGINT);
    sigprocmask(SIG_BLOCK, &my_sig_set, NULL);
    for (int sig = 1; sig < 64; sig++) {
        if(sigismember(&my_sig_set, sig)) {
            printf("-Signal %2d: %s\n", sig, strsignal(sig));
        }
    }
    while(1);

}