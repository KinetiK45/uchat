#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub){
    if (!str || !sub) return -1;
    int count = 0;
    char *temp = "test";
    while (mx_strstr(str, sub) != 0) {
        if (mx_strncmp(temp, mx_strstr(str, sub), mx_strlen(temp)) != 0) {
            count++;
        }
        temp = mx_strstr(str, sub);
        str++;
    }
    return count;
}
