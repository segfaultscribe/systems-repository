#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h> 

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntegerVector;

// Initializes vector with default capacity = 4
void initialize_vector(IntegerVector *vec);
// Pushes a value to the end of the vector, resizes vector if not enough memory
void push_back(IntegerVector *vec, int val);
// Frees vector memory and resets fields
void delete_vector(IntegerVector *vec);

#endif 
