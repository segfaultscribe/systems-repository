#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 256
#define MAX_TOKENS 50

void handle_sigint(int sig) {
    printf("\ncarapace> ");
    fflush(stdout);
}

void print_prompt() {
    printf("carapace> ");
    fflush(stdout);
}

int parse_input(char *input, char **tokens, int *is_background_process) {
    int tokenCount = 0;
    // Remove trailing newline replace it with '\0'
    input[strcspn(input, "\n")] = '\0';
    char *word = strtok(input, " ");
    while (word != NULL && tokenCount < MAX_TOKENS - 1) {
        tokens[tokenCount] = word;
        tokenCount++;
        word = strtok(NULL, " ");
    }
    tokens[tokenCount] = NULL;
    // Check for background process indicated by '&'
    if (tokenCount > 0 && strcmp(tokens[tokenCount - 1], "&") == 0) {
        *is_background_process = 1;
        tokens[tokenCount - 1] = NULL;
        tokenCount--;
    } else {
        *is_background_process = 0;
    }
    return tokenCount;
}

void execute_command(char **tokens, int is_background_process) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed:");
    } else if (pid == 0) {
        // Child process
        char *input_file = NULL;
        handle_input_redirection(tokens, &input_file);

        if (input_file != NULL) {
            freopen(input_file, "r", stdin);
        }

        if (execvp(tokens[0], tokens) == -1) {
            perror("execvp failed");
        }
        _exit(1);
    } else {
        // Parent process
        if (is_background_process == 0) {
            wait(NULL);
        } else {
            printf("[Background PID] %d\n", pid);
        }
    }
}

int handle_input_redirection(char **tokens, char **input_file){
    int t = 0;
    while(tokens[t]!=NULL){
        if(strcmp(tokens[t], "<") == 0){
            if (tokens[t + 1] != NULL) {
                *input_file = tokens[i + 1];

                // remove the '<' and the filename after it
                for (int t = i; tokens[j + 2] != NULL; j++) {
                    tokens[j] = tokens[j + 2];
                }
                tokens[t] = NULL;

                return 1;
            } else {
                fprintf(stderr, "Error: No input file specified after '<'\n");
                return -1;
            }
        }
        ++t;
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    char input[MAX_BUFFER_SIZE];
    char *tokens[MAX_TOKENS];
    char exit_sequence[] = "exit";

    while (1) {
        int is_background_process = 0;

        print_prompt();

        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (strcmp(input, "exit\n") == 0) {
                printf("Bye!\n");
                break;
            }
            int tokenCount = parse_input(input, tokens, &is_background_process);
            if (tokenCount == 0) {
                continue;  // No command entered, skip
            }
            execute_command(tokens, is_background_process);
        } else {
            printf("Error reading input!\n");
        }
    }

    return 0;
}
