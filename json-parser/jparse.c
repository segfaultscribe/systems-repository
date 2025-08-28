#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


typedef enum {
    STATE_START,
    STATE_KEY_BEGIN,
    STATE_KEY,
    STATE_KEY_END,
    STATE_VALUE_BEGIN,
    STATE_VALUE_STRING,
    STATE_VALUE_NUMBER,
    STATE_VALUE_TRUE,
    STATE_VALUE_FALSE,
    STATE_VALUE_NULL,
    STATE_VALUE_END,
    STATE_DONE
} parser_state;

char key_buffer[256];
int key_index = 0;

char val_buffer[256];
int val_index = 0;

int literal_index;

char key_stack[10][256];
int key_depth = 0;        

int in_array = 0;
int array_index = 0;

void get_key(char *parent, char key_stack[][256], int depth, const char *key) {
    parent[0] = '\0';
    for (int i = 0; i < depth; i++) {
        strcat(parent, key_stack[i]);
        strcat(parent, ".");
    }
    strcat(parent, key);
}

int main(){
    FILE *f = fopen("temp.json", "r");

    parser_state state = STATE_START;

    char line_buffer[512];
    // pull this loop out of main maybe?
    // or the switch?
    while(fgets(line_buffer, sizeof line_buffer, f)){
        for(int i=0;line_buffer[i] != '\0';++i){
            char ch = line_buffer[i];

            switch(state){
                case STATE_START:
                        if (ch == '{') {
                            state = STATE_KEY_BEGIN;
                        }
                    break;

                case STATE_KEY_BEGIN:
                        if (ch == '"'){
                            key_index = 0;
                            state = STATE_KEY;
                        }
                    break;
                
                case STATE_KEY:
                        if (ch == '"'){
                            key_buffer[key_index] = '\0';
                            state = STATE_KEY_END;
                        } else {
                            key_buffer[key_index++] = ch;
                        }
                    break;
                
                case STATE_KEY_END:
                        if (ch == ':'){
                            state = STATE_VALUE_BEGIN;
                        }
                    break;
                
                case STATE_VALUE_BEGIN:
                        // we need to handle
                        // 1. string
                        // 2. digits
                        // 3. true
                        // 4. false
                        // 5. null
                        if (ch == '{'){
                            strcpy(key_stack[key_depth++], key_buffer);
                            state = STATE_KEY_BEGIN;
                        } else if (ch == '['){
                            in_array = 1;
                            array_index = 0;

                            strcpy(key_stack[key_depth++], key_buffer);

                            state = STATE_VALUE_BEGIN;
                        } else if (ch == '"'){
                            val_index = 0;
                            state = STATE_VALUE_STRING;
                        } else if (isdigit(ch) || ch == '-' || ch == '.'){
                            val_index = 0;
                            val_buffer[val_index++] = ch;
                            state = STATE_VALUE_NUMBER;
                        } else if (ch == 't'){
                            literal_index = 1;
                            state = STATE_VALUE_TRUE;
                        } else if (ch == 'f'){
                            literal_index = 1;
                            state = STATE_VALUE_FALSE;
                        } else if (ch == 'n'){
                            literal_index = 1;
                            state = STATE_VALUE_NULL;
                        }
                    break;

                case STATE_VALUE_STRING:
                        if (ch == '"'){
                            val_buffer[val_index] = '\0';
                            char full_key[512];
                            if (in_array) {
                                snprintf(full_key, sizeof full_key, "%s[%d]", key_stack[key_depth - 1], array_index++);
                            } else {
                                get_key(full_key, key_stack, key_depth, key_buffer);
                            }
                            printf("Key: %s, Value: %s\n", full_key, val_buffer);
                            // TODO: modify this to some other function based on usecase
                            state = STATE_VALUE_END;
                        } else {
                            val_buffer[val_index++] = ch;
                        }
                    break;
                
                case STATE_VALUE_NUMBER:
                        if(isdigit(ch) || ch == '.'){
                            val_buffer[val_index++] = ch;
                        } else {
                            val_buffer[val_index] = '\0';
                            char full_key[512];

                            if (in_array) {
                                snprintf(full_key, sizeof full_key, "%s[%d]", key_stack[key_depth - 1], array_index++);
                            } else {
                                get_key(full_key, key_stack, key_depth, key_buffer);
}
                            if (strchr(val_buffer, '.') || strchr(val_buffer, 'e') || strchr(val_buffer, 'E')) {
                                float f = strtof(val_buffer, NULL);
                                printf("Key: %s, Value: %.2f\n", full_key, f);
                            } else {
                                int i = atoi(val_buffer);
                                printf("Key: %s, Value: %d\n", full_key, i);
                            }
                            state = STATE_VALUE_END;
                            i--;  // go back one character since this character could be ',' or '}'
                        }
                    break;
                
                case STATE_VALUE_TRUE:
                        if( (literal_index == 0 && ch == 't') ||
                            (literal_index == 1 && ch == 'r') ||
                            (literal_index == 2 && ch == 'u') ||
                            (literal_index == 3 && ch == 'e')){
                                literal_index++;
                                if (literal_index == 4) {
                                    char full_key[512];
                                    if (in_array) {
                                        snprintf(full_key, sizeof full_key, "%s[%d]", key_stack[key_depth - 1], array_index++);
                                    } else {
                                        get_key(full_key, key_stack, key_depth, key_buffer);
                                    }
                                    printf("Key: %s, Value: true\n", full_key);
                                    state = STATE_VALUE_END;
                                }
                            } else {
                                fprintf(stderr, "Error: Invalid character '%c' in 'true'\n", ch);
                                return 1;
                            }
                        break;
                case STATE_VALUE_FALSE:
                            if ((literal_index == 0 && ch == 'f') ||
                                (literal_index == 1 && ch == 'a') ||
                                (literal_index == 2 && ch == 'l') ||
                                (literal_index == 3 && ch == 's') ||
                                (literal_index == 4 && ch == 'e')) {
                                literal_index++;
                                if (literal_index == 5) {
                                    char full_key[512];
                                    if (in_array) {
                                        snprintf(full_key, sizeof full_key, "%s[%d]", key_stack[key_depth - 1], array_index++);
                                    } else {
                                        get_key(full_key, key_stack, key_depth, key_buffer);
                                    }
                                    printf("Key: %s, Value: false\n", full_key);
                                    state = STATE_VALUE_END;
                                }
                            } else {
                                fprintf(stderr, "Error: Invalid character '%c' in 'false'\n", ch);
                                return 1;
                            }
                        break;
                case STATE_VALUE_NULL:
                            if ((literal_index == 0 && ch == 'n') ||
                                (literal_index == 1 && ch == 'u') ||
                                (literal_index == 2 && ch == 'l') ||
                                (literal_index == 3 && ch == 'l')) {
                                literal_index++;
                                if (literal_index == 4) {
                                    char full_key[512];
                                    if (in_array) {
                                        snprintf(full_key, sizeof full_key, "%s[%d]", key_stack[key_depth - 1], array_index++);
                                    } else {
                                        get_key(full_key, key_stack, key_depth, key_buffer);
                                    }
                                    printf("Key: %s, Value: null\n", full_key);
                                    state = STATE_VALUE_END;
                                }
                            } else {
                                fprintf(stderr, "Error: Invalid character '%c' in 'null'\n", ch);
                                return 1;
                            }
                    break;
                
                case STATE_VALUE_END:
                        if (ch == ','){
                            if(in_array){
                                state = STATE_VALUE_BEGIN;
                            } else {
                                state = STATE_KEY_BEGIN;
                            }
                            
                        } else if (ch == '}') {
                            if (key_depth > 0){
                                key_depth--;
                                state = STATE_VALUE_END;
                            } else {
                                state = STATE_DONE;
                            }   
                        } else if (ch == ']') {
                            if (key_depth > 0) key_depth--; 
                            in_array = 0;
                            state = STATE_VALUE_END;
                        }
                    break;
            }
        }
    }
    return 0;;
}