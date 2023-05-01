#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len){
    unsigned char *result = (unsigned char*)dst;
    unsigned char *s = (unsigned char*)src;
    for (int i = 0; i < (int)len; i++) {
        result[i] = s[i];
    }
    dst = result;
    return dst;
}
