#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

int main(int argc, char *argv[]){
    char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        perror("Error opening binary");
        return 1;
    }

    Elf64_Ehdr elf_header;
    ssize_t bytes_read = read(fd, &elf_header, sizeof(Elf64_Ehdr));
    if (bytes_read != sizeof(Elf64_Ehdr)) {
        perror("read");
        close(fd);
        return 1;
    }


}