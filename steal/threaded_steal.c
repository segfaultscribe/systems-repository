#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>     
#include <limits.h>     
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include<pthread.h>

#define BUF_SIZE 4096
#define PROGRESS_WIDTH 30
#define BUF_COUNT 8
#define MAX_JOBS 1024
#define NUM_THREADS 8

int copy_files();
void print_progress(off_t stolen, off_t total);
void *worker(void *arg);


typedef struct copy_job {
    char from[PATH_MAX];
    char to[PATH_MAX];
} copy_job;

copy_job job_queue[MAX_JOBS];
int job_count = 0;
int job_index = 0;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(stderr, "Usage: %s <source1> <source2> ... <destination_dir>\n", argv[0]);
        return 1;
    }

    char *destination_folder = argv[argc-1];

    struct stat st;
    if(stat(destination_folder, &st) < 0 || S_ISDIR(st.st_mode) == 0){
        fprintf(stderr, "Couldn't fetch folder details(or not a folder): %s\n", argv[argc-1]);
        return 1;
    }

    for (int i=1; i < argc - 1; ++i){
        //get the souce folder path 
        //get the destination file path
        //call the worker

        char *src_path = argv[i];
        char *filename = basename(src_path);
        snprintf(job_queue[job_count].from, PATH_MAX, "%s", src_path);
        snprintf(job_queue[job_count].to, PATH_MAX, "%s/%s", destination_folder, filename);
        
        ++job_count;
    }

    if(job_count == 0){
        fprintf(stderr, "No files to copy!\n");
        return 1;
    }

    //threading
    pthread_t threads[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;++i){
        if(pthread_create(&threads[i], NULL, worker, NULL) != 0){
            perror("Failed to spawn thread");
            return 1;
        }
    }
    
    for(int j=0;j<NUM_THREADS;++j){
        pthread_join(threads[j], NULL);
    }
    
    printf("All files copied successfully.\n");
    return 0;
}


int copy_files(const char *src, const char *dest){
    int src_fd = open(src, O_RDONLY);
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    struct stat st;
    if(fstat(src_fd, &st) < 0){
        perror("Error fetching file details!");
        return 1;
    }
    off_t total_size = st.st_size;

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
        
    close(src_fd);
    close(dest_fd);
    return 0;
}

void *worker(void *arg){
    while(1){
        pthread_mutex_lock(&queue_mutex);

        if(job_index >= job_count){
            pthread_mutex_unlock(&queue_mutex);
            break;
        }

        copy_job next_job = job_queue[job_index];
        ++job_index;

        pthread_mutex_unlock(&queue_mutex);

        printf("Thread %ld copying %s -> %s\n", pthread_self(), next_job.from, next_job.to);

        if (copy_files(next_job.from, next_job.to) != 0) {
            fprintf(stderr, "Failed to copy %s -> %s\n", next_job.from, next_job.to);
        }
    }
    return NULL;
}

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