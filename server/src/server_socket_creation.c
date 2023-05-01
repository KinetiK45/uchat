#include "../inc/uchat.h"

void init_connections_array(t_sock* sock) {
    for (int i = 0; i < MAX_CLIENTS_PER_ONCE; i++) {
        sock->connections[i].clnt_socket = -1;
        sock->connections[i].user_id = -1;
    }
}

t_sock *server_socket_creation(int port) {
    t_sock *sock = malloc(sizeof(t_sock));
    
    struct sockaddr_in server_adrr;
    int opt = TRUE;
    if ((sock->main_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die_with_error("socket creation failed\n");
    
    if(setsockopt(sock->main_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )
        die_with_error("setsockopt failed\n");
    sock->address.sin_addr.s_addr = htonl(INADDR_ANY);
    sock->address.sin_family = AF_INET;
    sock->address.sin_port = htons(port);
    
        
    if (bind(sock->main_socket_fd, (struct sockaddr*)(&sock->address),sizeof(server_adrr)) < 0)
        die_with_error("bind of server socket is failed");
    
    if (listen(sock->main_socket_fd, MAX_CLIENTS_PER_ONCE) < 0)
        die_with_error("socket listening is failed");
    
    init_connections_array(sock);
    
    return sock;
}
