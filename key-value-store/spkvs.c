// SPKVS: Simple Persistent Key-Value Store
#include <stdio.h>
#include <string.h>
/* phase 1:  Open a server cli
            Parse input into a file and add input validation
            pase 1 input: PUT, GET 
            PUT <key> <value>
            GET <key>
            key: max 128 chars
            value max 512 chars */

void cli_prompt(){
    printf("Store $> ");
    fflush(stdout);
}

void error_out(const char *err){
    fprintf(stderr, "%s\n", err);
}

// void handle_put(char *key, char *value){
//     FILE *f = fopen("store.txt", "w");
// }

int main(){

    while(1){
        cli_prompt();

        char input[700];
        if(fgets(input, sizeof(input), stdin) != NULL){
            if (strcmp(input, "exit\n") == 0) {
                printf("Bye!\n");
                break;
            }
            // We're in the main part
            char *word = strtok(input, " ");
            if(strcmp(word, "PUT") == 0){
                char *key = strtok(NULL, " ");
                char *value = strtok(NULL, " "); 
                char *overflow = strtok(NULL, " ");

                if(overflow != NULL){
                    error_out("INVALID PUT FORMAT!");
                    fflush(stdout);
                    continue;
                }

                // handle_put(key, value);
            }
        }

    }
    return 0;
}