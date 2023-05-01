#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n){
    unsigned char *str = (unsigned char *)s;
    for (int i = (int)n - 1; i >= 0; i--) {
        if (str[i] == c) return &str[i];
    }
    return NULL;
}
