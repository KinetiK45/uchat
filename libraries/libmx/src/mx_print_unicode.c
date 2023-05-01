#include "../inc/libmx.h"

void rest(int i, wchar_t c){
    while (i >= 0) {
        char ch = (c >> i & 63) | 128;
        write(1, &ch, 1);
        i -= 6;
    }
}

void mx_print_unicode(wchar_t c){
        if (c < 128) {
            mx_printchar(c);
        }
        else if (c < 2048) {
            char ch = (c >> 6 & 31) | 192;
            write(1, &ch, 1);
            rest(0, c);
        }
        else if (c < 65536) {
            char ch = (c >> 12 & 15) | 224;
            write(1, &ch, 1);
            rest(6, c);
        }
        else {
            char ch = (c >> 18 & 7) | 240;
            write(1, &ch, 1);
            rest(12, c);
        }
}
