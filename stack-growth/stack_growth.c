#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool growthSecond(int* first){
    int second = 99;

    return (intptr_t)&second > (intptr_t)first;
}

bool growth(){
    int base = 0;

    growthSecond(&base);
}

int main(){
    printf("%s", growth() ? "Stack grows Up" : "Stack grows down");
    return 0;
}