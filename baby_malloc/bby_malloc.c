#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 1024*1024

static char memory_pool[POOL_SIZE];
static size_t offset = 0;


static block_meta* free_list = (block_meta*)memory_pool;

typedef struct {
    size_t size;      
    int free;         
    struct block_meta* next; 
} block_meta;

void internal_allocation(size_t size){
    block_meta *curr = free_list;
    size = (size + 7) & ~7;

    while(curr){
        if(curr->free && curr->size > size){
            curr->free = 0;

            if(curr->size > size + sizeof(block_meta)){
                block_meta *next = (block_meta*)((char*)curr + sizeof(block_meta) + size);

                next->size = curr->size - size - sizeof(block_meta);
                next->free = 1;
                next->next = curr->next;

                curr->size = size;
                curr->next = next;
            }

            return (char*)curr + sizeof(block_meta);
        }
        curr = curr->next;
    }
    return NULL;
}

void *bby_malloc(void *pointer, size_t size){
    int check = offset + size < POOL_SIZE ? 1 : 0;

    if(!check) return NULL;

    void *ptr = &memory_pool[offset];
    offset += size;
    return ptr;
}

int main(){

}