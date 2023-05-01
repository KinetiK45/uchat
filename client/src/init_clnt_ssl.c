#include "../inc/client.h"

void init_clnt_ssl(t_sock *sock) {
    SSL *ssl = NULL;
    ssl = SSL_new(sock->ctx);
    SSL_set_fd(ssl, sock->socket);
    sock->ssl = ssl;
    int n;
    if((n = SSL_connect(sock->ssl)) <= 0){
        exit(1);
    }
}
