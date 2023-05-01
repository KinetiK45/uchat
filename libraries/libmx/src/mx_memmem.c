#include "../inc/libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len){
    if (big_len < little_len) return NULL;
    unsigned char *b = (unsigned char *) big;
    unsigned char *l = (unsigned char *) little;
    for (int i = 0; i < (int)big_len; i++) {
        if (b[i] == l[0]){
            for (int j = 0; j < (int)little_len; j++) {
                if (b[i + j] != l[j]) break;
                if (j == (int)little_len - 1) return &b[i];
            }
        }
    }
    return NULL;
}
