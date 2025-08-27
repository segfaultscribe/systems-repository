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

int main(){
    FILE *f;

    
    f = fopen("temp.json", "r");

    char stream[512];
    while(fgets(stream, sizeof stream, f)){

    }
    return 0;;
}