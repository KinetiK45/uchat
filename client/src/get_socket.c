#include "../inc/client.h"


t_sock* get_socket(char* argv[]){
    SSL_library_init();
    t_sock *sock = socket_creation(argv);
    init_clnt_ctx(sock);
    init_clnt_ssl(sock);
    create_and_launch_recv_thread(sock);
    return sock;
}
