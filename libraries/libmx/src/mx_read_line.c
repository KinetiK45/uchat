#include "../inc/libmx.h"

int updateptr(char **str, char **save, char delim){
    if (!*save) return 0;
    int size = mx_strlen(*save);
    *str = mx_realloc(*str, size);
    if (mx_memccpy(*str, *save, delim, size) != NULL){
        char *temp = malloc(size);
        temp = mx_memcpy(temp, (char*)mx_memchr(*save, delim, size) + 1, size);
        *str[mx_get_char_index(*str, delim)] = '\0';
        *save = mx_realloc(*save, mx_strlen(temp));
        *save = mx_memcpy(*save, temp, mx_strlen(temp));
        free(temp);
        return -1;
    }
    return size;
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd){
    if (fd < 0) return -2;
    int size = 0;
    static char *save;
    char *str = malloc(buf_size);
    size += updateptr(&str, &save, delim);
    if (size == -1) return mx_strlen(str);
    save = mx_realloc(save, buf_size);
    char *buff = malloc(buf_size);
    while (true) {
        if (read(fd, buff, buf_size) <= 0){
            free(str);
            free(buff);
            return -1;
        }
        size += buf_size;
        str = mx_realloc(str, size);
        if (mx_memccpy(&str[size - buf_size], buff, delim, buf_size) != NULL){
            save = mx_memcpy(save, (char*)mx_memchr(buff, delim, buf_size) + 1, buf_size);
            str[mx_get_char_index(str, delim)] = '\0';
            size = mx_strlen(str);
            break;
        }
    }
    *lineptr = mx_realloc(*lineptr, size);
    if (str[0] == '\0') *lineptr = mx_strnew(size);
    else *lineptr = mx_memcpy(*lineptr, str, size);
    free(str);
    free(buff);
    return size;
}

/*
 
 int main(void){
     char *filename = "/Users/kinetik/Downloads/data.txt";
     int fd = open(filename, O_RDONLY);
     char *filedata = mx_file_to_str(filename);
     printf("filedata = '%s'\n------------------------\n", filedata);
     mx_strdel(&filedata);
     char *str = malloc(1);
     printf("size = %i\n", mx_read_line(&str, 7, '\n', fd));
     printf("'%s'\n---------------------\n", str);
     
      printf("size = %i\n", mx_read_line(&str, 35, ':', fd));
      printf("'%s'\n---------------------\n", str);
     
      printf("size = %i\n", mx_read_line(&str, 1, '.', fd));
      printf("'%s'\n---------------------\n", str);
      printf("size = %i\n", mx_read_line(&str, 10, '\n', fd));
      printf("'%s'\n---------------------\n", str);
     
     
      printf("size = %i\n", mx_read_line(&str, 10, '\n', fd));
      printf("'%s'\n---------------------\n", str);
      
     printf("size = %i\n", mx_read_line(&str, 1, '.', fd));
     printf("'%s'\n---------------------\n", str);
     free(str);
     return 0;
 }
 */
