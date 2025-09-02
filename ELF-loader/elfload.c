#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <inttypes.h>

const char *ptype_to_str(uint32_t type);

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
    //Now that we've validated the ELF file
    // we need to read the entire ELF file header into memory and print some of its key fields.
    Elf64_Ehdr elf_headr;
    lseek(fd, 0, SEEK_SET); // go back the 16 bytes we read, we're reading the entire header into elf_header now
    bytes_read = read(fd, &elf_headr, sizeof elf_headr);
    if(bytes_read != sizeof elf_headr){
        perror("read");
        close(fd);
        return 1;
    }
    printf("\n");
    printf("Program entry point: 0x%" PRIx64 "\n", elf_headr.e_entry);
    printf("Program header offset: %" PRIu64 "\n", elf_headr.e_phoff);
    printf("Section header offset: %" PRIu64 "\n", elf_headr.e_shoff);
    printf("\n");

    uint16_t n_programs = elf_headr.e_phnum;
    uint16_t pg_size = elf_headr.e_phentsize;
    // now we move onto program headers
    // program header table tells the loader what parts of the file 
    // need to be mapped into memory and with what permissions.
    Elf64_Phdr elf_phdr;
    lseek(fd, elf_headr.e_phoff, SEEK_SET);
    for(uint16_t i=0;i<n_programs;++i){
        // read each program header
        ssize_t bytes_read = read(fd, &elf_phdr, sizeof elf_phdr);
        if(bytes_read != sizeof elf_phdr){
            perror("read");
            close(fd);
            return 1;
        }
        printf("[%"PRIu16"] %s\tOffset: 0x%"PRIu64"\tVirtAddr: %"PRIu64"\tFileSize: %"PRIu64"\tMemSize: %"PRIu64"\tFlags: %"PRIu32"\n", 
                i, ptype_to_str(elf_phdr.p_type), elf_phdr.p_offset, elf_phdr.p_vaddr, elf_phdr.p_filesz, elf_phdr.p_memsz, elf_phdr.p_flags);
    }

    close(fd);
    return 0;


}

const char *ptype_to_str(uint32_t type) {
    switch(type) {
        case PT_NULL:    return "NULL";
        case PT_LOAD:    return "LOAD";
        case PT_DYNAMIC: return "DYNAMIC";
        case PT_INTERP:  return "INTERP";
        case PT_NOTE:    return "NOTE";
        case PT_SHLIB:   return "SHLIB";
        case PT_PHDR:    return "PHDR";
        case PT_TLS:     return "TLS";
        default:         return "UNKNOWN";
    }
}