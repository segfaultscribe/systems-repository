#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_signal(int sig){
    printf("Received signal: %d (%s)\n", sig, strsignal(sig));
    fflush(stdout);
}

int main(){

    for(int x=1;x<NSIG;++x){
        if(x == SIGKILL || x == SIGSTOP) continue;

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = handle_signal;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;

        sigaction(x, &sa, NULL);

    }
    
    while (1) {
        pause();
    }

    return 0;
}