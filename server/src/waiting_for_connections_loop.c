#include "../inc/uchat.h"

void waiting_for_connections_loop(t_sock *sock) {
    while(1) {
        int ndfs = create_fd_set(sock) + 1;
        if (select(ndfs, &sock->readfds, NULL,&sock->exceptfds,NULL) > 0) {
            handle_connection(sock);
        }
        
    }
}
