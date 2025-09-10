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
        char *input_file = NULL, *output_file = NULL;
        handle_input_redirection(tokens, &input_file);
        handle_output_redirection(tokens, &output_file);
        // handle input file 
        if (input_file != NULL) {
            if (freopen(input_file, "r", stdin) == NULL) {
                perror("Failed to open input file");
                _exit(1);
            }
        }
        // handle output file
        if (output_file != NULL) {
            if (freopen(output_file, "w", stdout) == NULL) {
                perror("Failed to open output file");
                _exit(1);
            }
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
    while(tokens[t] != NULL){
        if(strcmp(tokens[t], "<") == 0){
            if (tokens[t + 1] != NULL) {
                *input_file = tokens[t + 1];

                // Remove '<' and the filename
                for (int j = t; tokens[j + 2] != NULL; j++) {
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
    return 0;
}

int handle_output_redirection(char **tokens, char **output_file){
    int t = 0;
    while(tokens[t] != NULL){
        if(strcmp(tokens[t], ">") == 0){
            if (tokens[t + 1] != NULL) {
                *output_file = tokens[t + 1];

                // Remove '>' and the filename 
                for (int j = t; tokens[j + 2] != NULL; j++) {
                    tokens[j] = tokens[j + 2];
                }
                tokens[t] = NULL;

                return 1;
            } else {
                fprintf(stderr, "Error: No output file specified after '>'\n");
                return -1;
            }
        }
        ++t;
    }
    return 0;
}

void pipe_out(char **tokens){
    int pipefd[2]; // 0 right end, 1 left end
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return;
    }
    int pipe_pos = 0;
    while (tokens[pipe_pos] != NULL && strcmp(tokens[pipe_pos], "|") != 0) {
        pipe_pos++;
    }

    if(tokens[pipe_pos]==NULL){
        fprintf(stderr, "Error: No pipe symbol found\n");
        return;
    }
    tokens[pipe_pos] = NULL;  

    char **cmd1 = tokens;            
    char **cmd2 = &tokens[pipe_pos + 1];  

    // we need to create two child process for each side of the pipe | 

    //process 1
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First child process
        close(pipefd[0]);                 
        dup2(pipefd[1], STDOUT_FILENO);      
        close(pipefd[1]);                     

        if (execvp(cmd1[0], cmd1) == -1) {
            perror("execvp cmd1 failed");
            _exit(1);
        }
    }
    //process 2
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second child process
        close(pipefd[1]);                     
        dup2(pipefd[0], STDIN_FILENO);       
        close(pipefd[0]);                     

        if (execvp(cmd2[0], cmd2) == -1) {
            perror("execvp cmd2 failed");
            _exit(1);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
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
            int has_pipe = 0;
            for (int i = 0; i < tokenCount; i++) {
                if (strcmp(tokens[i], "|") == 0) {
                    has_pipe = 1;
                    break;
                }
            }

            if (has_pipe) {
                pipe_out(tokens);
            } else {
                execute_command(tokens, is_background_process);
            }
        } else {
            printf("Error reading input!\n");
        }
    }

    return 0;
}
