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
    if (!key || !value || !FILE_NAME) {
        error_out("Null argument passed to handle_put");
        return;
    }
    FILE *f = fopen(FILE_NAME, "a+");
    if (f == NULL) {
        perror("Failed to open File");
        return;
    }
    rewind(f);
    char kv[700];

    if (snprintf(kv, sizeof(kv), "%s=%s", key, value) >= sizeof(kv)) {
        fprintf(stderr, "Key-value pair too long, truncating.\n");
        // continuing
    }
    char line[700];
    while(fgets(line, sizeof line, f) != NULL){
        line[strcspn(line, "\n")] = '\0';
        char *current_key = strtok(line, "=");
        if(current_key && strcmp(current_key, key) == 0){
            error_out("EXISITING key error!");
            printf("!! Use UPDATE to change value\n");
            fclose(f);
            return;
        }
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
    if(value_out[0] == '\0'){
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
        return;
    }

    int rename_status = rename("temp.txt", "store.txt");
    if(rename_status != 0){
        error_out("Error in delete operation!");
        return;
    }

    printf("Successfully DELETED [KEY: %s, VALUE: %s]\n", key, value);
}

void handle_list(){
    //we'll list stuff in'ere
    FILE *f = fopen("store.txt", "r");
    char key_value[700];
    while(fgets(key_value, sizeof key_value, f) != NULL){
        char *key = strtok(key_value, "=");
        char *value = strtok(NULL, "=");
        if(key && value){
            printf("KEY: %s, VALUE: %s\n", key, value);
        } 
    }
    fclose(f);
}

void handle_update(char *key, char *value){
    // delete the old key value
    // add the new key value
    // lol
    handle_delete(key);
    handle_put(key, value, GLOBAL_FILE_NAME);
    printf("Successfully Updated KEY: %s, with VALUE: %s", key, value);
} 

void print_help() {
    printf("=========================================\n");
    printf("           SIMPLE KEY VALUE STORE        \n");
    printf("=========================================\n");
    printf("Description:\n");
    printf("  A simple, persistent key-value store written in C.\n\n");

    printf("Usage:\n");
    printf("  Type commands at the prompt as shown below.\n\n");

    printf("Command List:\n");
    printf("  HELP                       : Show this help message\n");
    printf("  PUT <key> <value>          : Assign <value> to <key> and store it\n");
    printf("  UPDATE <key> <value>       : Update <key> with a new <value>\n");
    printf("  GET <key>                  : Fetch the value associated with <key>\n");
    printf("  DELETE <key>               : Delete the key-value pair from storage\n");
    printf("  LIST                       : List all key-value pairs in the store\n");

    printf("\nTips:\n");
    printf("  - Keys and values are stored line-by-line in a file.\n");
    printf("  - Data is persistent between runs.\n");

    printf("=========================================\n");
}

int main(int argc, char *argv[]){
    
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

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
            if (word == NULL) {
                continue; // empty line, prompt again
            }
            if(strcmp(word, "PUT") == 0){               // Handle PUT instruction
                char *key = strtok(NULL, " ");
                char *value = strtok(NULL, " "); 
                char *overflow = strtok(NULL, " ");
                if(!key || !value || overflow != NULL){
                    error_out("INVALID PUT FORMAT!");
                    fflush(stdout);
                    continue;
                }
                handle_put(key, value, GLOBAL_FILE_NAME);
            } else if(strcmp(word, "GET") == 0){        // Handle GET instruction
                char *key = strtok(NULL, " ");
                char *overflow = strtok(NULL, " ");
                if(!key || overflow != NULL){
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
                if(!key || overflow != NULL){
                    error_out("INVALID DELETE FORMAT!");
                    fflush(stdout);
                    continue;
                }
                handle_delete(key);
            } else if(strcmp(word, "LIST") == 0){
                handle_list();
            } else if(strcmp(word, "UPDATE") == 0){
                char *key = strtok(NULL, " ");
                char *value = strtok(NULL, " "); 
                char *overflow = strtok(NULL, " ");
                if (!key || !value || overflow != NULL) {
                    error_out("INVALID UPDATE FORMAT!");
                    fflush(stdout);
                    continue;
                }
                handle_update(key, value);
            } else if(strcmp(word, "help") == 0 || strcmp(word, "HELP") == 0 ){
                print_help();
                continue;
            } else {
                printf("Unknown Command: %s  | use 'help' to know working commands\n", word);
                continue;
            }
        }

    }
    return 0;
}