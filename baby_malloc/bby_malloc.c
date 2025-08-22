#include <stdio.h>
#include <stdlib.h>

typedef struct block_meta {
    size_t size;      
    int free;         
    struct block_meta* next; 
} block_meta;


#define POOL_SIZE 1024*1024

static char memory_pool[POOL_SIZE];
static size_t offset = 0;


static block_meta* free_list = (block_meta*)memory_pool;

void init_allocator() {
    free_list->size = POOL_SIZE - sizeof(block_meta);
    free_list->free = 1;
    free_list->next = NULL;
}


void *internal_allocation(size_t size){
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

void my_free(void* ptr) {
    if (!ptr) return;

    block_meta* block = (block_meta*)((char*)ptr - sizeof(block_meta));
    block->free = 1;

    if (block->next && block->next->free) {
        block->size += sizeof(block_meta) + block->next->size;
        block->next = block->next->next;
    }
}

void *bby_malloc(void *pointer, size_t size){
    int check = offset + size < POOL_SIZE ? 1 : 0;

    if(!check) return NULL;

    void *ptr = &memory_pool[offset];
    offset += size;
    return ptr;
}

int main(){
    init_allocator();

    printf("Allocator initialized with %d bytes\n", POOL_SIZE);

    void* p1 = internal_allocation(100);
    printf("p1 = %p\n", p1);

    void* p2 = internal_allocation(200);
    printf("p2 = %p\n", p2);

    my_free(p1);
    printf("Freed p1\n");

    void* p3 = internal_allocation(50);
    printf("p3 = %p\n", p3);

    return 0;
}