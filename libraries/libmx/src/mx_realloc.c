#include "../inc/libmx.h"

void *mx_realloc(void *ptr, size_t size){
    void *result;
    if (size <= 0){
        free(ptr);
        return NULL;
    }
    else {
        result = malloc(size);
    }
    if (ptr != NULL) result = mx_memcpy(result, ptr, size < malloc_size(ptr) ? size : malloc_size(ptr));
    free(ptr);
    return result;
}
