#include<stdio.h>
#include<stdint.h>

extern void add_values(const uint8_t *src, const uint8_t *src2);

int main(){
    
    uint8_t a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8_t b[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    add_values(a, b);

    printf("SIMD addition result: \n");
    for(int i=0;i<16;i++){
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
