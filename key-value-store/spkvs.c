// SPKVS: Simple Persistent Key-Value Store
#include <stdio.h>
#include <string.h>
// pase 1:  Open a server cli
//          Parse input into a file and add input validation
//          pase 1 input: PUT, GET 

void cli_prompt(){
    printf("Store $> ");
    fflush(stdout);
}

int main(){

    while(1){
        cli_prompt();
    }
    return 0;
}