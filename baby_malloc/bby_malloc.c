#include <stdio.h>

#define POOL_SIZE 1024*1024

static char memory_pool[POOL_SIZE];
static size_t offset = 0;

typedef struct {
    size_t size;      
    int free;         
    struct block_meta* next; 
} block_meta;

void *bby_malloc(void *pointer, size_t size){
    int check = offset + size < POOL_SIZE ? 1 : 0;

    if(!check) return NULL;

    void *ptr = &memory_pool[offset];
    offset += size;
    return ptr;
}

int main(){
    
}