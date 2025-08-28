#include <stdio.h>
#include <string.h>
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

int main(){
    FILE *f = fopen("temp.json", "r");

    parser_state state = STATE_START;

    char line_buffer[512];
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
                        if (ch == '"'){
                            val_index = 0;
                            state = STATE_VALUE_STRING;
                        } else if (isdigit(ch) || ch == '-'){
                            val_index = 0;
                            val_buffer[val_index] = ch;
                            state = STATE_VALUE_NUMBER;
                        } else if (ch == 't'){
                            val_index = 0;
                            val_buffer[val_index] = ch;
                            state = STATE_VALUE_TRUE;
                        } else if (ch == 'f'){
                            val_index = 0;
                            val_buffer[val_index] = ch;
                            state = STATE_VALUE_FALSE;
                        } else if (ch == 'n'){
                            val_index = 0;
                            val_buffer[val_index] = ch;
                            state = STATE_VALUE_NULL;
                        }
                    break;

                case STATE_VALUE_STRING:
                        if (ch == '"'){
                            val_buffer[val_index] = '\0';
                            printf("Key: %s, Value: %s\n", key_buffer, val_buffer);
                            // TODO: modify this to some other function based on usecase
                            state = STATE_VALUE_END;
                        } else {
                            val_buffer[val_index++] = ch;
                        }
                    break;
                
                case STATE_VALUE_NUMBER:
                        if(isdigit(ch) || ch == '.'){
                            val_buffer[val_index] = ch;
                        } else {
                            val_buffer[val_index] = '\0';
                            printf("Key: %s, Value: %s\n", key_buffer, val_buffer);
                            state = STATE_VALUE_END;
                            i--;  // go back one character since this character could be ',' or '}'
                        }
                    break;
                
                case STATE_VALUE_TRUE:
                        if( (literal_index == 0 && ch == 't') ||
                            (literal_index == 0 && ch == 'r') ||
                            (literal_index == 0 && ch == 'u') ||
                            (literal_index == 0 && ch == 'e')){
                                literal_index++;
                                if (literal_index == 4) {
                                    printf("Key: %s, Value: true\n", key_buffer);
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
                                    printf("Key: %s, Value: false\n", key_buffer);
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
                                    printf("Key: %s, Value: null\n", key_buffer);
                                    state = STATE_VALUE_END;
                                }
                            } else {
                                fprintf(stderr, "Error: Invalid character '%c' in 'null'\n", ch);
                                return 1;
                            }
                    break;
                
                case STATE_VALUE_END:
                        if (ch == ","){
                            state = STATE_KEY_BEGIN;
                        } else if (ch == "}") {
                            state = STATE_DONE;
                        }
                    break;
            }
        }
    }
    return 0;;
}