#include "../inc/libmx.h"

int mx_strlen(const char *s){
    int size = 0;
    while (*s) {
        size++;
        s++;
    }
    return size;
}
