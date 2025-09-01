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

    if (elf_header.e_ident[EI_MAG0] != ELFMAG0 ||
        elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header.e_ident[EI_MAG2] != ELFMAG2 ||
        elf_header.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Not a valid ELF file\n");
        close(fd);
        return 1;
    }

    printf("Valid ELF file detected!\n");
    printf("Entry point: 0x%lx\n", elf_header.e_entry);
    printf("Program header offset: %lu\n", elf_header.e_phoff);
    printf("Section header offset: %lu\n", elf_header.e_shoff);

    close(fd);
    return 0;


}