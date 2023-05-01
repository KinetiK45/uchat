#include "../inc/libmx.h"

void *mx_memchr(const void *s, int c, size_t n){
    unsigned char *str = (unsigned char *)s;
    for (int i = 0; i < (int)n; i++) {
        if (str[i] == c) return &str[i];
    }
    return NULL;
}
