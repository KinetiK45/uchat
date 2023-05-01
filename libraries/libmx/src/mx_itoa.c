#include "../inc/libmx.h"

char *mx_itoa(int number){
    bool isPositive = true;
    int size = 0;
    long num = number;
    if(number == 0)
        return "0";
    if (num < 0){
        isPositive = false;
        num *= -1;
        size++;
    }
    long n = num;
    while (n) {
        n /= 10;
        size++;
    }
    char *result = mx_strnew(size);
    if (!isPositive) *result = '-';
    result += size;
    while (num) {
        *--result = num % 10 + '0';
        num /= 10;
    }
    if (!isPositive) result--;
    return result;
}
