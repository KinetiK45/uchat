#include "../inc/libmx.h"

char *mx_strndup(const char *s1, size_t n){
    if (!s1) return NULL;
    int size = (int)n;
    if (mx_strlen(s1) < size) size = mx_strlen(s1);
    char *s = mx_strnew(size);
    mx_strncpy(s, s1, size);
    return s;
}
