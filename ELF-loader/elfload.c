#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <inttypes.h>

const char *ptype_to_str(uint32_t type);
void print_flags(uint32_t flags);

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

    // Sanity checks
    switch(elf_headr.e_type){
        case ET_EXEC: printf("Type: Executable file\n"); break;
        case ET_DYN:  printf("Type: Shared object (so/dylib)\n"); break;
        case ET_REL:  printf("Type: Relocatable object file\n"); break;
        default:      printf("Type: Unknown (%u)\n", elf_headr.e_type); break;
    }

    // check system architecture
    if(elf_headr.e_machine != EM_X86_64){
        fprintf(stderr, "Unsupported architecture: %u\n", elf_headr.e_machine);
        close(fd);
        return 1;
    } else {
        printf("Machine: x86_64\n");
    }
    // check version
    if (elf_headr.e_version != EV_CURRENT) {
        fprintf(stderr, "Invalid ELF version\n");
        close(fd);
        return 1;
    }
    // check program headers
    if(elf_headr.e_phnum == 0){
        fprintf(stderr, "No program headers found!");
        close(fd);
        return 1;
    }
    // check program header size
    if (elf_headr.e_phentsize != sizeof(Elf64_Phdr)) {
        fprintf(stderr, "Unexpected program header entry size\n");
        close(fd);
        return 1;
    }
    printf("\n");
    printf("Program entry point: 0x%" PRIx64 "\n", elf_headr.e_entry);
    printf("Program header offset: %" PRIu64 "\n", elf_headr.e_phoff);
    printf("Section header offset: %" PRIu64 "\n", elf_headr.e_shoff);
    printf("\n");

    uint16_t n_programs = elf_headr.e_phnum;
    // uint16_t pg_size = elf_headr.e_phentsize;
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
        //printout each header part and then the flags 
        printf("[%"PRIu16"] %s\tOffset: 0x%"PRIu64"\tVirtAddr: %"PRIu64"\tFileSize: %"PRIu64"\tMemSize: %"PRIu64"\t", 
                i, ptype_to_str(elf_phdr.p_type), elf_phdr.p_offset, elf_phdr.p_vaddr, elf_phdr.p_filesz, elf_phdr.p_memsz);
        print_flags(elf_phdr.p_flags);
        printf("\n");
    }

        // prepare to load
        // reading all headers into an array
        Elf64_Phdr *phdrs = malloc(sizeof(Elf64_Phdr) * elf_headr.e_phnum);
        if(!phdrs){
            perror("malloc");
            close(fd);
            return 1;
        }
        // position the file descriptor at the start
        lseek(fd, elf_headr.e_phoff, SEEK_SET);
        bytes_read = read(fd, phdrs, sizeof(Elf64_Phdr) * elf_headr.e_phnum);
        if (bytes_read != sizeof(Elf64_Phdr) * elf_headr.e_phnum) {
            perror("read");
            free(phdrs);
            close(fd);
            return 1;
        }

        // check PT_LOAD segments
        // WHY?
        // The loader (only) cares about PT_LOAD, because those are the segments that make the program runnable. 
        // If an ELF has no PT_LOAD, there’s literally nothing to put in memory, not an executable in the normal sense.

        int has_load = 0;
        for(int i=0;i<elf_headr.e_phnum;++i){
            if(phdrs[i].p_type == PT_LOAD){
                ++has_load;
            }
        }
        if (!has_load) {
            fprintf(stderr, "No loadable segments in ELF!\n");
            free(phdrs);
            close(fd);
            return 1;
        }
        // simulate loading by printing for only LOAD segments
        for(int i=0;i<elf_headr.e_phnum;++i){
            Elf64_Phdr load = phdrs[i];

            if(load.p_type == PT_LOAD){
                printf("LOAD SEGMENT:\n");
                printf("  VirtAddr: 0x%016" PRIx64 "\n", load.p_vaddr);
                printf("  FileOffset: 0x%016" PRIx64 "\n", load.p_offset);
                printf("  FileSize: %" PRIu64 "\n", load.p_filesz);
                printf("  MemSize : %" PRIu64 "\n", load.p_memsz);

                printf("  Flags: %c%c%c\n",
                    (load.p_flags & PF_R) ? 'R' : '-',
                    (load.p_flags & PF_W) ? 'W' : '-',
                    (load.p_flags & PF_X) ? 'X' : '-');

                if (load.p_memsz > load.p_filesz) {
                    printf("  (extra %" PRIu64 " bytes should be zero-initialized)\n",
                        load.p_memsz - load.p_filesz);
                }
                printf("\n");
            }
        }
        free(phdrs);
        close(fd);
        return 0;
    }



const char *ptype_to_str(uint32_t type) {
    switch(type) {
        case PT_NULL:           return "NULL";
        case PT_LOAD:           return "LOAD";
        case PT_DYNAMIC:        return "DYNAMIC";
        case PT_INTERP:         return "INTERP";
        case PT_NOTE:           return "NOTE";
        case PT_SHLIB:          return "SHLIB";
        case PT_PHDR:           return "PHDR";
        case PT_TLS:            return "TLS";
        case PT_GNU_EH_FRAME:   return "GNU_EH_FRAME";
        case PT_GNU_STACK:      return "GNU_STACK";
        case PT_GNU_RELRO:      return "GNU_RELRO";
        case PT_GNU_PROPERTY:   return "GNU_PROPERTY";
        default:                return "UNKNOWN";
    }
}

void print_flags(uint32_t flags) {
    printf("Flags: %c%c%c",
        (flags & PF_R) ? 'R' : '-',
        (flags & PF_W) ? 'W' : '-',
        (flags & PF_X) ? 'E' : '-');
}