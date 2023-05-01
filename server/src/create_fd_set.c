#include "../inc/uchat.h"

int create_fd_set(t_sock *sock) {
    int max_sock_fd = sock->main_socket_fd;
    __DARWIN_FD_ZERO(&sock->readfds);
    __DARWIN_FD_SET(sock->main_socket_fd,&sock->readfds);
    __DARWIN_FD_ZERO(&sock->exceptfds);
    __DARWIN_FD_SET(sock->main_socket_fd, &sock->exceptfds);
    int peer_sock_fd;
    for(int i = 0; i < MAX_CLIENTS_PER_ONCE; i++) {
        if((peer_sock_fd = sock->connections[i].clnt_socket) > 0) {
            __DARWIN_FD_SET(peer_sock_fd, &sock->readfds);
            __DARWIN_FD_SET(peer_sock_fd, &sock->exceptfds);
        }
        if (peer_sock_fd > max_sock_fd)
            max_sock_fd = peer_sock_fd;
            
    }
    return max_sock_fd;
}
