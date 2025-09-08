#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(){
    while(1){
        printf("%s", "carapace> ");

        char input[MAX_BUFFER_SIZE];
        char exit_sequence[] = "exit";
        printf("%s", input);
        if(strcmp(input, exit_sequence) == 0){
            printf("breaking up!");
            break;
        }
        // printf("shell echoes: %s\n", input);
    }
    return 0;
}