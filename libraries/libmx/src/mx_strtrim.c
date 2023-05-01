#include "../inc/libmx.h"

char *mx_strtrim(const char *str){
    if(!str) return NULL;
    char *src = mx_strnew(mx_strlen(str));
    mx_strcpy(src, str);
    int start_counter = 0;
    while (*src) {
        if (mx_isspace(*src)){
            src++;
            start_counter++;
        }
        else break;
    }
    int count = 0;
    for (int i = mx_strlen(src)-1; i >= 0; i--) {
        if (mx_isspace(src[i])) count++;
        else break;
    }
    char *result;
    if (mx_strlen(src) - count == 0) result = NULL;
    else result = mx_strndup(src, mx_strlen(src) - count);
    src -= start_counter;
    mx_strdel(&src);
    return result;
}
