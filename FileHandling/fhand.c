// deals with all things file in C

#include <stdio.h>

void read_text_file(){
    // file pointer that helps handle files
    FILE *f;

    //opening file
    // arg1: filename arg2: open mode("r" -> read)
    f = fopen("hello.txt", "r");

    // int? hmm...
    int c;

    printf("char by char:\n\n");
    // reads individual characters of the file 
    // until the end of file is reached
    while((c = fgetc(f)) != EOF){
        printf("%c", c);
    }
    printf("\n");

    // Reset the file pointer to beginning for next read
    rewind(f);

    printf("line by line:\n\n");
    // character array that stores a line of text
    char stream[1024];

    // using fgets() for line read, notice the change in args?
    // and comparison to NULL instead of EOF?
    while(fgets(stream, sizeof(stream), f) != NULL){
        printf("%s", stream);
    }
    printf("\n");

    rewind(f);

    printf("line by line using fscanf:\n\n");
    //using fscanf()
    char line[1024];
    while(fscanf(f, "%s", line) != EOF){
        printf("%s\n", line);
    }
    //notice how we sent the file stream into a variable?
    // we can split different data types using "%_"
    printf("\n");

    fclose(f);
}

void write_text_file(){
    // very similar to read
    FILE *f;

    // "w" for write
    f = fopen("output.txt", "w");

    // character by character
    fputc('C', f);
    fputc('\n', f);

    // line
    fputs("Hello, World!\n", f);

    // fprintf
    int a = 67;
    char *s = "written using fprintf";
    fprintf(f, "a hold the value: %d\nname = %s", a, s);

    fclose(f);
}

void read_binary_file(){
    FILE *f;

    // "rb" for read binary mode
    f = fopen("binary.bin", "rb");
    unsigned char c;

    // fread for reading binary. Notice the '> 0' 
    while(fread(&c, sizeof(char), 1, f) > 0){
        printf("%d ", c);
    }

    fclose(f);
}

void write_binary_file(){
    FILE *f;

    f = fopen("binary.bin", "wb");

    //payload
    unsigned char bytes_to_write[5] = {1, 2, 3, 4, 5};

    fwrite(bytes_to_write, sizeof(char), 5, f);
    //that's it done. 'fwrite' for binary writing

    fclose(f);
}

int main(){
    // read text file
    read_text_file();
    // write into text file
    write_text_file();
    // write into binary file
    write_binary_file();
    // read from binary file
    read_binary_file();

    return 0;
}