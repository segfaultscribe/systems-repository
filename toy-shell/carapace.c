#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

int main(){
    while(1){
        printf("%s", "carapace> ");

        char input[MAX_BUFFER_SIZE];
        char exit_sequence[] = "exit";

        fgets(input, sizeof(input), stdin);
        //strcspn will return the first index of the second argument in the first argument
        input[strcspn(input, "\n")] = '\0';
        if(strcmp(input, exit_sequence) == 0){
            printf("Bye!\n");
            break;
        }
        //just echo for now
        printf("shell echoes: %s\n", input);
    }
    return 0;
}