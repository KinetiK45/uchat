#include "../inc/client.h"

char *get_text_no_files(char* msg_text) {
    if (!msg_text) return NULL;
    if (!mx_strstr(msg_text, "<file>")){
        return mx_strdup(msg_text);
    }
    char *result = mx_strnew(0);
    int index = 0;
    while (1) {
        char *start = mx_strstr(&msg_text[index], "<file>");
        if (!start) {
            char *part_to_add = mx_strdup(&msg_text[index]);
            char *temp = mx_strjoin(result, part_to_add);
            mx_strdel(&result);
            mx_strdel(&part_to_add);
            result = temp;
            break;
        }
        int start_index = index;
        index = start - msg_text;
        char *part_to_add = mx_strndup(&msg_text[start_index], index - start_index);
        char *temp = mx_strjoin(result, part_to_add);
        mx_strdel(&result);
        mx_strdel(&part_to_add);
        result = temp;
        char *end = mx_strstr(start, "</file>");
        if (end) {
            index = end - msg_text + mx_strlen("</file>");
        }
        else {
            break;
        }
    }
        
    return result;
}
