#include "../inc/libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace){
    if (!str || !sub || !replace) return NULL;
    int count = mx_count_substr(str, sub);
    char *source = mx_strdup(str);
    int resultSize = mx_strlen(str) - mx_strlen(sub) * count + mx_strlen(replace) + count;
    int shift = 0;
    int shift_source = 0;
    char *result = mx_strnew(resultSize);
    for (int i = 0; i < count; i++) {
        int index = mx_get_substr_index(source, sub);
        mx_strncpy(result, source, index);
        result = mx_strcat(result, replace);
        shift_source += index + mx_strlen(sub);
        source += index + mx_strlen(sub);
        shift += index + mx_strlen(replace);
        result += index + mx_strlen(replace);
    }
    result -= shift;
    result = mx_strcat(result, source);
    source -= shift_source;
    mx_strdel(&source);
    return result;
}

