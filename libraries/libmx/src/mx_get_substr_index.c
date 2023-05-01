#include "../inc/libmx.h"

bool check_sub(const char *str, const char *sub, int i){
    int j = 0;
    while (sub[j]) {
        if (sub[j++] != str[i++]) return false;
    }
    return true;
}

int mx_get_substr_index(const char *str, const char *sub){
    if (str == NULL || sub == NULL) return -2;
    int i = -1;
    while (str[++i]) {
        if (str[i] == sub[0] && check_sub(str, sub, i)){
            return i;
        }
    }
    return -1;
}
