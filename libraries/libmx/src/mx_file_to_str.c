#include "../inc/libmx.h"

char *mx_file_to_str(const char *filename){
    int file = open(filename, O_RDONLY);
    if (file == -1) return NULL;
    char c;
    int count = 0;
    while (read(file, &c, 1) > 0) {
        count++;
    }
    if (close(file) != 0) return NULL;
    if (count == 0) return "";
    close(file);
    char *filedata = mx_strnew(count);
    file = open(filename, O_RDONLY);
    if (read(file, filedata, count) < 0){
        mx_strdel(&filedata);
        return NULL;
    }
    close(file);
    return filedata;
}
