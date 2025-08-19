#include <stdio.h>
#include <ctype.h>

//ANSI color code escapes
#define COLOR_RESET   "\x1b[0m"
#define COLOR_OFFSET  "\x1b[36m"
#define COLOR_HEX     "\x1b[33m"
#define COLOR_ASCII   "\x1b[32m"

int use_color = 0;

void hexditch(FILE *f){
    char buffer[16];
    size_t offset = 0;
    size_t bytes;

    while((bytes = fread(buffer, 1, 16, f)) > 0){
        // Hex
        if(use_color) printf(COLOR_OFFSET);
        printf("%08zx\t", offset);
        if(use_color) printf(COLOR_RESET);

        if(use_color) printf(COLOR_HEX);
        for(size_t i=0;i<bytes;++i) printf("%02x ", buffer[i]);
        if(use_color) printf(COLOR_RESET);

        printf("\t");

        //ASCII
        printf("|");
        if(use_color) printf(COLOR_ASCII);
        for(size_t j=0;j<bytes;++j) printf("%c", isprint(buffer[j]) ? buffer[j] : '.');
        if(use_color) printf(COLOR_RESET);
        printf("|\n");

        offset += bytes;
    }

    fclose(f);
}

void print_help(const char *progname) {
    printf("Usage: %s [--color] <filename>\n", progname);
    printf("Options:\n");
    printf("  --color       Enable colored output\n");
    printf("  --no-color    Disable colored output (default)\n");
    printf("  -h, --help    Show this help message\n");
}

int main(int argc, char *argv[]){
    const char *filename = NULL;
    // if (argc != 2) {
    //     fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    //     return 1;
    // }

    for(int i=1;i<argc;++i){
        if(argv[i] == "--color"){
            use_color = 1;
        } else if(argv[i] == "-h" || argv[i] == "--help") {
            print_help(argv[0]);
            return 0;
        } else if(argv[i][0] != '-' && filename == NULL){
            filename = argv[i];
        } else {
            fprintf(stderr, "unknown argument %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if(!filename){
        frpintf(stderr, "No input file given!");
        print_help(argv[0]);
        return 1;
    }

    FILE *f = fopen(filename, "rb");
    
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    hexditch(f);
    return 0;
}