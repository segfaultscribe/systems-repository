#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE 4096
#define PROGRESS_WIDTH 30

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

int main(int argc, char *argv[]){

    char *progname = argv[0];

    char *file_to_copy_to_path = argv[2];
    char *file_to_copy_from_path = argv[1];
    

    int ftct_fd = open(file_to_copy_to_path, O_WRONLY | O_CREAT | O_TRUNC , 0644);
    int ftcf_fd = open(file_to_copy_from_path, O_RDONLY);

    printf("\n");
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

    if (ftct_fd < 0){
        perror("Error opening destination file");
        close(ftcf_fd);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t b_read, b_written;
    off_t stolen = 0;
    while((b_read = read(ftcf_fd, buffer, BUF_SIZE)) > 0){
        b_written = write(ftct_fd, buffer, b_read);

        if(b_written != b_read){
            perror("Error copying from source");
            close(ftcf_fd);
            close(ftct_fd);
            return 1;
        }

        stolen += b_written;
        print_progress(stolen, total_size);
    }

    if(b_read < 0) {
        perror("Error reading source file");
    }

    close(ftcf_fd);
    close(ftct_fd);

    printf("\nCopying succesful!\n");
    return 0;

}