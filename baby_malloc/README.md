# Custom bump allocator in C - baby_allocator

A simple bump allocator function to understand how memory allocators work under the hood. It’s not meant to replace malloc/free, but to show how they could be implemented at a basic level.<br>

Things you could look forward to in this implementation

- Static memory pool simulating a heap (memory_pool)
- `Metadata headers` per block (size, free, next)
- Memory size `alignment` to 8 bytes
- Block splitting to allocate only what is needed
- `Free` blocks by marking them available (made possible using the metadata)
- Combine adjacent free blocks to prevent fragmentation
- Safe wrapper (bby_malloc) for explicit error handling
- Debug dump function to visualize allocator state


### Allocator flow
- The pool starts as one big free block.
- On allocation, search for a block, split it, return pointer after header.
- On free, mark block free, merge with next if possible.
- A linked list of blocks tracks memory usage.

### Usage
`safe`
```C
void* p1; //bby_malloc(void** out, size_t size) expects void**
if (bby_malloc(&p1, 100)) { 
    printf("Allocated 100 bytes at %p\n", p1);
}
dump_allocator_state();

bby_free(p1);
dump_allocator_state();
```
`by-passing bby_malloc()`
```C
int* p1 = internal_allocation(10 * sizeof(int));
if (p1 != NULL) {
    printf("Allocated %zu bytes at %p\n", 10 * sizeof(int), p1);
}
dump_allocator_state();

my_free(p1);
dump_allocator_state();
```
