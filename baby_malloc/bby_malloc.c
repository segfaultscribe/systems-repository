#include <stdio.h>
#include <stdlib.h>

typedef struct block_meta {
    size_t size;
    int free;
    struct block_meta* next;
} block_meta;

#define POOL_SIZE 1024*1024

int isInitialized = 0;

static char memory_pool[POOL_SIZE];
static block_meta* free_list = (block_meta*)memory_pool;

void init_allocator() {
    free_list->size = POOL_SIZE - sizeof(block_meta);
    free_list->free = 1;
    free_list->next = NULL;
    isInitialized = 1;
}

void* internal_allocation(size_t size) {

    if(!isInitialized) init_allocator();

    block_meta *curr = free_list;
                            // byte alignment - look into this more. 
    size = (size + 7) & ~7; //  memory accesses is faster and more efficient when data is aligned 
                            // 8 bit for x86 

    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;

            if (curr->size > size + sizeof(block_meta)) {
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

void bby_free(void* ptr) {
    if (!ptr) return;

    block_meta* block = (block_meta*)((char*)ptr - sizeof(block_meta));
    block->free = 1;

    // combine with adjacent block if free
    if (block->next && block->next->free) {
        block->size += sizeof(block_meta) + block->next->size;
        block->next = block->next->next;
    }
}

int bby_malloc(void **out, size_t size) {
    void* result = internal_allocation(size);
    if (!result) {
        *out = NULL;
        return 0;
    }
    *out = result;
    return 1;
}

void dump_allocator_state() {
    block_meta* curr = free_list;
    printf("\n----Allocator State---- \n");

    while (curr) {
        printf("[ size=%zu, %s ] -> ",
               curr->size,
               curr->free ? "FREE" : "USED");
        curr = curr->next;
    }

    printf("NULL\n--------------------\n\n");
}

int main() {

    printf("Allocator initialized with %zu bytes\n", (size_t)POOL_SIZE);
    dump_allocator_state();

    void* p1;
    if (bby_malloc(&p1, 100)) {
        printf("Allocated p1 (100 bytes) at %p\n", p1);
    } else {
        printf("Failed to allocate p1\n");
    }
    dump_allocator_state();

    void* p2;
    if (bby_malloc(&p2, 200)) {
        printf("Allocated p2 (200 bytes) at %p\n", p2);
    } else {
        printf("Failed to allocate p2\n");
    }
    dump_allocator_state();

    bby_free(p1);
    printf("Freed p1\n");
    dump_allocator_state();

    void* p3;
    if (bby_malloc(&p3, 50)) {
        printf("Allocated p3 (50 bytes) at %p\n", p3);
    } else {
        printf("Failed to allocate p3\n");
    }
    dump_allocator_state();

    return 0;
}
