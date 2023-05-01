#include "../inc/client.h"


t_sock *socket_creation(char* argv[]) {
    t_sock *sock = malloc(sizeof(t_sock));
    if ((sock->socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
        exit(1);
    }
    memset(&sock->address, 0, sizeof(t_saddr));
    sock->address.sin_addr.s_addr = inet_addr(argv[1]);
    sock->address.sin_family = AF_INET;
    sock->address.sin_port = htons(mx_atoi(argv[2]));
    if(connect(sock->socket,(struct sockaddr*)&sock->address,sizeof(t_saddr)) < 0) {
        exit(1);
    }
    return sock;
}
