char *mx_strchr(const char *s, int c){
    while (*s) {
        if (c == *s) {
            return (char*)s;
        }
        s++;
    }
    return 0;
}

