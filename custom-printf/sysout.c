#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

void print_char(char c){
    write(1, &c, 1);
}

void print_str(char *s){
    while(*s != '\0'){
        write(1, s++, 1);
    }
}

void print_int(int n){
    char buf[12]; // 32 bit integers
    int i = 0;
    int negative = n < 0 ? 1: 0;

    if(negative){
        n = -n;
    }

    if(n == 0){
        print_char('0');
        return;
    }

    while(n){
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    
    if(negative){
        buf[i++] = '-';
    }

    while(i--){
        write(1, &buf[i], 1);
    }
}

void sysout(char *s, ...){
    va_list args;
    va_start(args, s); 
    // %d, %c, %s for now
    // parse the incoming string to look for exact character
    while(*s != '\0'){
        if(*s == '%'){
            s++;
            char c = *s;
            switch (c)
            {
            case 'c':
                    char v = (char)va_arg(args, int);
                    print_char(v);
                break;
            case 's':
                    char *str = va_arg(args, char*);
                    print_str(str);
                break;
            case 'd':
                    int n = va_arg(args, int);
                    print_int(n);
                break;
            default:
                    char space = ' ';
                    write(1, &space, 1);
                break;
            }
            s++;
        } else {
            write(1, s, 1);
            s++;
        }
    }
    va_end(args);
}

int main(){
    char *s = "Hello World!";
    sysout("The first program is always: %s\n", s);
    
    char c = 'A';
    sysout("The letter %c is the first letter of the alphabet.\n", c);
    
    int n = 100;
    sysout("%d is an integer!\n", n);
    
    sysout("A string %s, A character %c and an integer %d walks into a bar.", s, c, n);
    
    return 0;
}
