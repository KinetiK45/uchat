#include "../inc/uchat.h"

void die_with_error(char *err_msg) {
    write(1,err_msg, mx_strlen(err_msg));
    exit(1);
}
