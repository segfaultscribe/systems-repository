#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>     
#include <limits.h>     
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE 4096
#define PROGRESS_WIDTH 30

int success = 0;

void print_progress(off_t stolen, off_t total){
    float percent = (float)stolen / total;
    int progress = (int)(percent * PROGRESS_WIDTH);

    printf("\rCopying [");
    for(int i=0; i<PROGRESS_WIDTH; ++i){   
        if(i < progress){
            printf("#");
        } else {
            printf("-");
        } 
    }
    printf("] %3.0f%%", percent*100);
    fflush(stdout);
}

int copy_files(int file_number, int src_fd, int dest_fd, off_t total_size){

    char buffer[BUF_SIZE];
    ssize_t b_read, b_written;
    off_t stolen = 0;
    while((b_read = read(src_fd, buffer, BUF_SIZE)) > 0){
        b_written = write(dest_fd, buffer, b_read);

        if(b_written != b_read){
            perror("Error copying from source");
            close(src_fd);
            close(dest_fd);
            return 1;
        }

        stolen += b_written;
        print_progress(stolen, total_size);
    }

    if(b_read < 0) {
        perror("Error reading source file");
    }

    return 0;
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source1> <source2> ... <destination_dir>\n", argv[0]);
        return 1;
    }

    char *destination_folder = argv[argc - 1];

    struct stat st_dest;
    if (stat(destination_folder, &st_dest) != 0 || !S_ISDIR(st_dest.st_mode)) {
        perror("Destination must be a valid directory");
        return 1;
    }

    char *progname = argv[0];

    for(int i=1;i<argc-1;++i){
        //build the source repository
        char *file_to_copy_from_path = argv[i];
        int ftcf_fd = open(file_to_copy_from_path, O_RDONLY);

        struct stat st;
        if(stat(file_to_copy_from_path, &st) < 0){
            perror("Cannot get source file details");
            return 1;
        }

        off_t total_size = st.st_size;
        if (ftcf_fd < 0) {
            perror("Error opening source file");
        return 1;
        }    

        //build the destination repository
        char *filename = basename(file_to_copy_from_path);
        char destination_file[PATH_MAX];

        if (destination_folder[strlen(destination_folder) - 1] != '/') {
            snprintf(destination_file, sizeof(destination_file), "%s/%s", destination_folder, filename);
        } else {
            snprintf(destination_file, sizeof(destination_file), "%s%s", destination_folder, filename);
        }

        int ftct_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int error = copy_files(i, ftcf_fd, ftct_fd, total_size);
        if(error){
            perror("Something went wrong!");
        }
        success += 1;
        close(ftcf_fd);
        close(ftct_fd);
        
        printf(" Copied (%d/%d)\n", i, argc - 2);
    }

    printf("\nCopied %d files to %s\n", success, destination_folder);
    return 0;

}