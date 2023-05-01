#include "../inc/uchat.h"

t_connected_peer *get_sock_for_send(t_sock *sock, int clnt_sock) {
    for (int i = 0; i < MAX_CLIENTS_PER_ONCE; i++) {
        if(sock->connections[i].clnt_socket == clnt_sock) {
            return &sock->connections[i];
        }
    }
    return NULL;
}

void add_new_socket_to_connections_arr(t_sock *sock, int *new_clnt_sock) {
    for (int i = 0; i < MAX_CLIENTS_PER_ONCE; i++) {
        if(sock->connections[i].clnt_socket < 0) {
            sock->connections[i].clnt_socket = *new_clnt_sock;
            sock->connections[i].address = sock->address;
            sock->connections[i].ssl = create_new_clnt_ssl(SSL_new(sock->ctx),*new_clnt_sock);
            break;
        }
    }
}
