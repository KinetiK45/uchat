#include "../inc/client.h"


void init_clnt_ctx(t_sock *sock) {
    const SSL_METHOD *method = NULL;
    SSL_CTX *ctx = NULL;
    
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    method = TLS_client_method();
    ctx = SSL_CTX_new(method);
    
    if (!ctx) {
        exit(1);
    }
    sock->ctx = ctx;
}
