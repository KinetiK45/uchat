#include "../inc/libmx.h"

void mx_printint(int n){
    long number = n;
    if (number < 0) {
        mx_printchar('-');
        number *= -1;
    }
    if (number == 0) mx_printchar('0');
    int size = 0;
    long num = number;
    while (num != 0) {
        num /= 10;
        size++;
    }
    int array[size];
    for (int i = 0; i < size; i++) {
        array[i] = number % 10;
        number /= 10;
    }
    for (int i = size-1; i >= 0; i--) {
        mx_printchar(array[i] + '0');
    }
}
