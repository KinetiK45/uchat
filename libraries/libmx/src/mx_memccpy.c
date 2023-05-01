#include "../inc/libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n){
    unsigned char *s = (unsigned char*) src;
    unsigned char *res = (unsigned char*) dst;
    for (int i = 0; i < (int)n; i++) {
        res[i] = s[i];
        if (s[i] == c) return &res[i+1];
    }
    return NULL;
}
