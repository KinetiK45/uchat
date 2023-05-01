#include "../inc/uchat.h"

void handle_connection(t_sock *sock) {
    int new_clnt_sock;
    if(__DARWIN_FD_ISSET(sock->main_socket_fd, &sock->readfds)) {
        accept_connection(sock,&new_clnt_sock);
        add_new_socket_to_connections_arr(sock, &new_clnt_sock);
    }
    handle_receiving_data(sock);
}
