#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 256
#define MAX_TOKENS 50

void handle_sigint(int sig) {
    printf("\ncarapace> ");
    fflush(stdout);
}

int main(){
    signal(SIGINT, handle_sigint);
    char input[MAX_BUFFER_SIZE];
    char *tokens[MAX_TOKENS];
    char exit_sequence[] = "exit";
    // int tokenCount = 0;
    
    while(1){
        int is_background_process = 0;
        
        printf("%s", "carapace> ");

        if(fgets(input, sizeof(input), stdin) != NULL){
            // strcspn will return the first index of the second argument in the first argument
            input[strcspn(input, "\n")] = '\0';
            if(strcmp(input, exit_sequence) == 0){
                printf("Bye!\n");
                break;
            }
            int tokenCount = 0;
            char *word = strtok(input, " ");
            while(word != NULL && tokenCount < MAX_TOKENS){
                tokens[tokenCount] = word;
                tokenCount++;
                word = strtok(NULL, " ");
            }
            tokens[tokenCount] = NULL;
            if(strcmp(tokens[tokenCount-1], "&") == 0){
                is_background_process = 1;
                tokens[tokenCount - 1] = NULL;
                tokenCount--;
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
                if(is_background_process == 0){
                    wait(NULL);
                } else{
                    printf("[Background PID] %d\n", pid);
                }
            }
        } else {
            printf(" Error reading input!");
        }
    }
    return 0;
}