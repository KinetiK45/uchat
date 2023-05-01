#include "../inc/libmx.h"

unsigned long mx_hex_to_nbr(const char *hex){
    if (*hex == 0) return 0;
    unsigned long result = 0;
    int size = 0;
    while (hex[size]) size++;
    for (int i = 0; i < size; i++) {
        if (mx_isdigit(hex[i])){
            result += hex[i] - 48;
            result *= 16;
        }
        else if (mx_isalpha(hex[i])){
            if (mx_isupper(hex[i])){
                result += hex[i] - 55;
                result *= 16;
            }
            if (mx_islower(hex[i])){
                result += hex[i] - 87;
                result *= 16;
            }
        }
        else return 0;
    }
    return result/16;
}
