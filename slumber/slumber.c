#include <stdio.h>
#include <time.h>
#include <errno.h> 
#include <signal.h>

unsigned int slumber(unsigned int seconds){
    struct timespec req, rem;
    req.tv_sec = seconds;
    req.tv_nsec = 0;

    while(nanosleep(&req, &rem) == -1){
        if(errno == EINTR){
            req = rem;
            printf("[slumber] Interrupted! %ld seconds left...\n", rem.tv_sec);
        } else {
            fprintf(stderr, "Unknown error!");
            break;
        }
    }

    return req.tv_sec;
}

void handle_sigint(int sig) {
    printf("\n[Signal caught] Interrupt received!\n");
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("Going to slumber for 10 seconds. Press Ctrl+C to interrupt me...\n");
    unsigned int remaining = slumber(10);

    printf("It's over!");

    return 0;
}