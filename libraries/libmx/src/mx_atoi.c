//
//  mx_atoi.c
//  uchat_server1
//
//  Created by Ivan Homenuk on 4/19/23.
//  Copyright © 2023 Ivan Homenuk. All rights reserved.
//


#include "../inc/libmx.h"

int mx_atoi(const char *str) {
    long int res = 0;
    if(str == NULL) return -5;
    if(*str == '-') return -5;
    while (*str) {
        if(mx_isdigit(*str)){
            res = res * 10;
            res = res + (*str - 48);
        }
        else return -5;
        str++;
    }
    return res;
}
