#include "../inc/uchat.h"

void create_server_context(t_sock *sock) {
    const SSL_METHOD *method = NULL;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLS_server_method();
    sock->ctx = SSL_CTX_new(method);
}
