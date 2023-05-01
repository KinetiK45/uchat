#include "../inc/libmx.h"

bool mx_isdigit(int c){
    if (c > 47 && c < 58) {
        return true;
    }
    return false;
}
