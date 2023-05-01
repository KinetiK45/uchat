#include "../inc/libmx.h"

char **mx_strsplit(char const *s, char c){
    if (!s) return NULL;
    int size = mx_count_words(s, c);
    char **arr = (char**)malloc((size + 1)*sizeof(char*));
    const char *str = s;
    for (int i = 0; i < size;) {
        while (*str == c && *str) {
            str++;
        }
        int count = 0;
        while (*str != c && *str) {
            str++;
            count++;
        }
        str -= count;
        char *temp = mx_strnew(count);
        temp = mx_strncpy(temp, str, count);
        arr[i] = temp;
        str += count;
        i++;
    }
    arr[size] = NULL;
    return arr;
}

