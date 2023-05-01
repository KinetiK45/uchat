#include "../inc/uchat.h"

SSL *create_new_clnt_ssl(SSL *ssl,int clnt_sock) {
    SSL_set_fd(ssl, clnt_sock);
    return ssl;
}
