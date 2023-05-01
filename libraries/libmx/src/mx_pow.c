#include "../inc/libmx.h"

double mx_pow(double n, unsigned int pow){
    if (pow == 0) return 1;
    double result = 1;
    for (int i = 0; i < (int)pow; i++) {
        result *= n;
    }
    return result;
}
