#include <stdio.h>
#include "vector.h"

int main(){

    IntegerVector vector;

    //need to initialize vector first
    initialize_vector(&vector);

    //add elements to the vector
    for(int i=0;i<10;++i){
        push_back(&vector, i);
    }

    //display vector
    for(int j=0;j<10;++j){
        printf("%d ", vector.data[j]); // vector.data[j] also works
    }
    printf("\n");

    delete_vector(&vector);

    return 0;
}
