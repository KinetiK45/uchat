#include "../inc/libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n){
    unsigned char *result = (unsigned char*)dst;
    unsigned char *s = (unsigned char*)src;
    for (int i = 0; i < (int)n; i++) {
        result[i] = s[i];
    }
    dst = result;
    return dst;
}
