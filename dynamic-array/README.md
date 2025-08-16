# Dynamic Array: Vectors in C

Implementing a dynamically growing array like vectors in C++ in C. This is a basic implementation that wroks only for integer arrays and can be extended for all data types. 

- vector.h - header file
- vector.c - implementation file
- usage.c - uses the vector to create a dynamic array

`IntegerVector V`<br>
    - declares a vector(dynamic array) V<br>
`initialize_vector(&V)`:<br> 
    - Initializes the vector V with default values, takes one parameter: the reference to the declared vector<br>
`push_back(&V, value)`:<br> 
    - adds an element to the end of the array. Dynamically grows to accomodate any number of inputs.<br>
    - takes two paramets: &V(refernce of vector to add to), value(the integeer value to be added)<br>
`delete_vector(&V)`:<br> 
    - frees the memory used by the vector and resets all fields<br>
<hr>

`initialize_vector` allocates memory to the data in vector using 'malloc'. `push_back` checks if memory is available before adding new data and if memory is not available, it increases the size of the array by doubling the previous size and uses 'realloc' to allocate new memory. 'realloc' handles a little bit of fragmentation issues by trying to grow inplace if possible. 

<strong>Good to know:</strong> `realloc` can only be used if the memory that is being reallocated was initially allocated using `malloc`, `calloc` or `realloc`, if not it might cause undefined behaviour or errors.

### To run code

```
// 'cd' into the folder

make
./program
```

