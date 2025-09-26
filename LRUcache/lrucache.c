#include <stdio.h>

#define CACHE_SIZE 1000  // or use dynamic allocation


/* Things to define
1. get(key)
    If key is in hash table:
    Move node to front of DLL
    Return value
    Else: return -1

2. put(key)
    If key exists:
        Update value
        Move node to front
    Else:
        If cache is full:
        Remove node at tail (least recently used)
        Remove it from hash table
        Create new node, add to front
        Insert into hash table
*/

// A doubly linked list
typedef struct DllNode {
    int key;
    int value;
    struct DllNode* prev;
    struct DllNode* next;
} DllNode;

DllNode* LRUCache[CACHE_SIZE]; 


int main(){
    while(1){
        
    }
    return 0;
}