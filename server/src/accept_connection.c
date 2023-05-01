#include "../inc/uchat.h"

void accept_connection(t_sock *serv_sock, int *new_clnt_sock) {
    if ((*new_clnt_sock = accept(serv_sock->main_socket_fd,
      (struct sockaddr *) &serv_sock->address, (socklen_t *)&serv_sock->addrlen)) < 0)
        die_with_error("accept failed\n");
}
