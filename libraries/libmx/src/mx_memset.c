#include "../inc/libmx.h"

void *mx_memset(void *b, int c, size_t len){
    unsigned char *result = b;
    for (int i = 0; i < (int)len; i++) {
        result[i] = c;
    }
    return result;
}
