#include "../inc/client.h"

ssize_t send_message(t_sock *sock, char *sending_msg){
    ssize_t result_sending = SSL_write(sock->ssl, sending_msg, mx_strlen(sending_msg));
    return result_sending;
}


unsigned long send_file(t_sock *sock, char *path, unsigned long filesize) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        mx_printerr("NULL FILE");
        exit(1);
    }
    
    char buffer[RECV_BUFF_SIZE];
    bzero(buffer,RECV_BUFF_SIZE);
    int bytes_read = 0, bytes_sent = 0;
    unsigned long total_bytes_sent = 0;
    while ((bytes_read = fread(buffer,1,RECV_BUFF_SIZE,file)) > 0 && total_bytes_sent <= filesize) {
        if ((bytes_sent = SSL_write(sock->ssl, buffer, bytes_read)) < 0) {
            mx_printerr("SSL WRITE ERROR ON FILE");
        }
        total_bytes_sent += bytes_sent;
        bzero(buffer, RECV_BUFF_SIZE);
    }
    fclose(file);
    return 0;
}
