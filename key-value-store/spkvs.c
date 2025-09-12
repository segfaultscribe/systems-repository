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

/*phase 2:  DELETE, LIST commands
            DELETE <key>
            LIST
*/

#define GLOBAL_FILE_NAME "store.txt"

void cli_prompt(){
    printf("Store $> ");
    fflush(stdout);
}

void error_out(const char *err){
    fprintf(stderr, "%s\n", err);
}

void handle_put(char *key, char *value, char *FILE_NAME){
    FILE *f = fopen(FILE_NAME, "a");
    if (f == NULL) {
        perror("Failed to open File");
        return;
    }
    char kv[700];

    if (snprintf(kv, sizeof(kv), "%s=%s", key, value) >= sizeof(kv)) {
        fprintf(stderr, "Key-value pair too long, truncating.\n");
        // continuing
    }

    if (fprintf(f, "%s\n", kv) < 0) {
        perror("Write failed");
        fclose(f); // Try to close even if write fails
        return;
    }
    printf("Succesfully written to file: [%s]\n", kv);
    
    fclose(f);
}

void handle_get(char *key, char *value_out, size_t value_out_size){
    FILE *f = fopen("store.txt", "r");
    if(f == NULL){
        perror("Failed to open file");
        return;
    }

    char value[512];
    while(fgets(value, sizeof value, f) != NULL){
        value[strcspn(value, "\n")] = '\0';
        char *current_key = strtok(value, "=");

        if(strcmp(current_key, key) == 0){
            char *current_value = strtok(NULL, "=");
            strncpy(value_out, current_value, value_out_size - 1);
            value_out[value_out_size - 1] = '\0';
            fclose(f);
            return;
        }
    }
    if(value_out[0] = '\0'){
        fprintf(stderr, "VALUE for key: %s NOT FOUND", key);

    }
    fclose(f);
}

void handle_delete(char *key){
    FILE *from = fopen("store.txt", "r");
    FILE *to = fopen("temp.txt", "w");
    if(from == NULL){
        perror("Failed to open file");
        return;
    }
    if(to == NULL){
        perror("Failed to open file");
        return;
    }

    char line[700];
    int found = 0;
    char value[512];

    while(fgets(line, sizeof line, from) != NULL){
        line[strcspn(line, "\n")] = '\0';
        char hold_line[700];
        strncpy(hold_line, line, sizeof hold_line);
        char *current_key = strtok(line, "=");
        char *current_value = strtok(NULL, "=");

        if (current_key && current_value && strcmp(current_key, key) == 0) {
            snprintf(value, sizeof value, "%s", current_value);
            found = 1;
            continue; 
        }

        fprintf(to, "%s\n", hold_line);
    }
    
    fclose(from);
    fclose(to);

    if(found == 0){
        printf("KEY %s doesn't EXIST!", key);
    }

    int del_status = remove("store.txt");
    if(del_status != 0){
        error_out("Error in delete operation!");
        fclose(from);
        fclose(to);
        return;
    }

    int rename_status = rename("temp.txt", "store.txt");
    if(rename_status != 0){
        error_out("Error in delete operation!");
        fclose(from);
        fclose(to);
        return;
    }

    printf("Successfully DELETED key: %s with VALUE: %s\n", key, value);
}

int main(){

    while(1){
        cli_prompt();

        char input[700];
        // Get stdin input
        if(fgets(input, sizeof(input), stdin) != NULL){
            input[strcspn(input, "\n")] = '\0';
            if (strcmp(input, "exit") == 0) {
                printf("Bye!\n");
                break;
            }
            // Extract the first word to find the command
            char *word = strtok(input, " ");
            if(strcmp(word, "PUT") == 0){               // Handle PUT instruction
                char *key = strtok(NULL, " ");
                char *value = strtok(NULL, " "); 
                char *overflow = strtok(NULL, " ");
                if(overflow != NULL){
                    error_out("INVALID PUT FORMAT!");
                    fflush(stdout);
                    continue;
                }
                handle_put(key, value, GLOBAL_FILE_NAME);
            } else if(strcmp(word, "GET") == 0){        // Handle GET instruction
                char *key = strtok(NULL, " ");
                char *overflow = strtok(NULL, " ");
                if(overflow != NULL){
                    error_out("INVALID GET FORMAT!");
                    fflush(stdout);
                    continue;
                }
                char value[512];
                handle_get(key, value, sizeof value);
                printf("KEY: %s, VALUE: %s\n", key, value);
            } else if(strcmp(word, "DELETE") == 0){     // Handle DELETE instruction
                char *key = strtok(NULL, " ");
                char *overflow = strtok(NULL, " ");
                if(overflow != NULL){
                    error_out("INVALID DELETE FORMAT!");
                    fflush(stdout);
                    continue;
                }
                handle_delete(key);
            }
        }

    }
    return 0;
}