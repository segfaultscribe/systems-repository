#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

//create vecotr with size 4
void initialize_vector(IntegerVector *vec){
    vec->size = 0;
    vec->capacity = 4;
    vec->data = malloc(vec->capacity * sizeof(int));
    
    if (vec->data == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
}

// adds to vector while also dynamically changnng size
void push_back(IntegerVector *vec, int val){
    if(vec->size >= vec->capacity){
        size_t double_capacity = vec->capacity * 2;
        int *temp = realloc(vec->data, (double_capacity * sizeof(int)));
        if(temp == NULL){
            fprintf(stderr, "Failed to allocate memory %zu bytes\n", double_capacity*sizeof(int));
            exit(EXIT_FAILURE);
        }
        vec->data = temp;
        vec->capacity = double_capacity; 
    }
    vec->data[vec->size++] = val;
}

void delete_vector(IntegerVector *vec){
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}


