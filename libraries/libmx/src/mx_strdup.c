#include "../inc/libmx.h"

char *mx_strdup(const char *str){
    char *s = mx_strnew(mx_strlen(str));
    mx_strcpy(s, str);
    return s;
}
