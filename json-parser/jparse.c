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
            }
        }
    }
    return 0;;
}