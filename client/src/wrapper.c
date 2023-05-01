#include "../inc/client.h"

void *mx_memalloc(size_t size) {
    size_t i = 0;
    char *str = NULL;

    if ((str = (char *)malloc(size)) == NULL)
        return NULL;
    i = -1;
    while (++i < size)
        str[i] = 0;
    return str;
}

t_wrapper* wrapper(t_sock* sock){
    t_wrapper* wrap = mx_memalloc(sizeof(t_wrapper));
    wrap->sock = sock;
    return wrap;
}
