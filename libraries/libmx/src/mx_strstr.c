#include "../inc/libmx.h"

char *mx_strstr(const char *haystack, const char *needle){
    if (!mx_strlen(needle)) return (char*)haystack;
    char *result = (char*)haystack;
    while (mx_strchr(result, needle[0]) != 0) {
        result = mx_strchr(result, needle[0]);
        if (mx_strncmp(result, needle, mx_strlen(needle)) == 0) {
            return result;
        }
        result++;
    }
    return 0;
}
