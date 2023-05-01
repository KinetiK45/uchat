#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr){
    int count = 0;
    unsigned long num = nbr;
    while (num != 0) {
        num /= 16;
        count++;
    }
    char* result = mx_strnew(count);
    for (int i = count - 1; i >= 0; i--) {
        int temp = nbr % 16;
        if (temp > 9)
            result[i] = temp + 87;
        else
            result[i] = temp + 48;
        nbr /= 16;
    }
    return result;
}
