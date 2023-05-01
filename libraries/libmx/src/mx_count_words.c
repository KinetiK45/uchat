#include "../inc/libmx.h"

int mx_count_words(const char *str, char delimiter){
    if (!str) return -1;
    int count = 0, i = 0;
    if (str[0] != delimiter && str[0] != '\0') {
        count++;
    }
    while (str[i] != '\0') {
        if (str[i] == delimiter && str[i+1] != delimiter && str[i+1] != '\0') {
            count++;
        }
        i++;
    }
    return count;
}
