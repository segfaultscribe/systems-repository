#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 256
#define MAX_TOKENS 50

int main(){
    char input[MAX_BUFFER_SIZE];
    char *tokens[MAX_TOKENS];
    char exit_sequence[] = "exit";
    int tokenCount = 0;
    
    while(1){
        printf("%s", "carapace> ");

        if(fgets(input, sizeof(input), stdin) != NULL){
            // strcspn will return the first index of the second argument in the first argument
            input[strcspn(input, "\n")] = '\0';
            if(strcmp(input, exit_sequence) == 0){
                printf("Bye!\n");
                break;
            }
            char *word = strtok(input, " ");
            while(word != NULL && tokenCount < MAX_TOKENS){
                tokens[tokenCount] = word;
                tokenCount++;
                word = strtok(NULL, " ");
            }

            pid_t pid = fork();
            if(pid == -1){
                perror("Fork failed:");
                return 1;
            } else if(pid == 0){
                // child
                if (execvp(tokens[0], tokens) == -1) {
                    // execvp returns only if there is an error
                    perror("execvp failed");
                    return 1;
                }
            } else {
                wait(NULL);
            }
        } else {
            printf(" Error reading input!");
        }
    }
    return 0;
}