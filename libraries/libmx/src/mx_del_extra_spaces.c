#include "../inc/libmx.h"

char *mx_del_extra_spaces(const char *str){
    if (!str) return NULL;
    char* trim = mx_strtrim(str);
    if (trim == NULL) return NULL;
    int countOfSpaces = 0;
    for (int i = 0; i < mx_strlen(trim) - 1; i++) {
        if (mx_isspace(trim[i]) && mx_isspace(trim[i + 1]))
            countOfSpaces++;
    }
    char *result = mx_strnew(mx_strlen(trim) - countOfSpaces);
    for (int i = 0, j = 0; i < mx_strlen(trim); i++) {
        if (mx_isspace(trim[i]) && mx_isspace(trim[i + 1])) continue;
        result[j] = trim[i];
        j++;
    }
    mx_strdel(&trim);
    return result;
}
