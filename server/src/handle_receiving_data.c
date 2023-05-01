#include "../inc/uchat.h"

void handle_receiving_data(t_sock *sock) {
    int temp_sock_fd;
    for (int i = 0; i < MAX_CLIENTS_PER_ONCE; i++) {
        temp_sock_fd = sock->connections[i].clnt_socket;
        if (temp_sock_fd > 0 && __DARWIN_FD_ISSET(temp_sock_fd, &sock->readfds)) {
            recieve_msg_from_client(sock, &sock->connections[i]);
        }
    }
}
