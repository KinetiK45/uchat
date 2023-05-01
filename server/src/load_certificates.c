#include "../inc/uchat.h"

void load_certificates(SSL_CTX *ctx, char *cert_file, char *key_file) {
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        exit(1);
    }
    
    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        exit(1);
    }
    
    if (!SSL_CTX_check_private_key(ctx)) {
        die_with_error("uchat_server: private key doesn`t match ");
        die_with_error("the public certificate");
        exit(1);
    }
}
