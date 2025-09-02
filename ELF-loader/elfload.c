#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

int main(int argc, char *argv[]){
    // standard argument validation
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
        return 1;
    }
    //get the file name
    char *filename = argv[1];
    // opening file using open(), can also use fopen using a FILE pointer
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        perror("Error opening binary");
        return 1;
    }
    // this is where stuff gets interesting
    // EI_NIDENT is a const value of 16
    // we are doing a read where we require the first 16 bytes in the file
    // to verify the ELF format
    unsigned char e_ident[EI_NIDENT];
    ssize_t bytes_read = read(fd, e_ident, EI_NIDENT);
    if (bytes_read != EI_NIDENT) {
        perror("read");
        close(fd);
        return 1;
    }
    /*
    in-order this will be 
    MAG0: 0x7f
    MAG1: 'E' 
    MAG2: 'L'
    MAG3: 'F'
    this info is pulle from the ELF specification
    */
    if (e_ident[EI_MAG0] != ELFMAG0 ||
        e_ident[EI_MAG1] != ELFMAG1 ||
        e_ident[EI_MAG2] != ELFMAG2 ||
        e_ident[EI_MAG3] != ELFMAG3){
            fprintf(stderr, "Not an ELF File!\n");
            close(fd);
            return 1;
        }
    printf("Valid ELF File acquired!\n");
    // printf("Entry point: 0x%lx\n", elf_header.e_entry);
    // printf("Program header offset: %lu\n", elf_header.e_phoff);
    // printf("Section header offset: %lu\n", elf_header.e_shoff); 
    // extracting class information using EI_CLASS
    if(e_ident[EI_CLASS] == ELFCLASS32){
        printf("CLASS: 32 bit\n");
    } else if (e_ident[EI_CLASS] == ELFCLASS64){
        printf("CLASS: 64 bit\n");
    } else {
        printf("CLASS: Invalid!");
    }
    //extract endianess information using EI_DATA
    if(e_ident[EI_DATA] == ELFDATA2LSB){
        printf("ENDIANNESS: Little Endian\n");
    } else if(e_ident[EI_DATA] == ELFDATA2MSB){
        printf("ENDIANNESS: Big Endian\n");
    } else {
        printf("ENDIANNESS: Invalid Endian!");
    }
    // Elf64_Ehdr elf_header;
    // ssize_t bytes_read = read(fd, &elf_header, sizeof(Elf64_Ehdr));
    // if (bytes_read != sizeof(Elf64_Ehdr)) {
    //     perror("read");
    //     close(fd);
    //     return 1;
    // }
    close(fd);
    return 0;


}