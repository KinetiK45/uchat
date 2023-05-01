#include "../inc/uchat.h"

void start_server(int port) {
    prepare_database();
    t_sock *serv_socket = server_socket_creation(port);
    create_server_context(serv_socket);
    char *key = RESOURCES_LOCATION \
    "server.key";
    char *cert = RESOURCES_LOCATION \
    "server.crt";
    load_certificates(serv_socket->ctx,cert,key);
    waiting_for_connections_loop(serv_socket);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        mx_printerr("Usage: ./uchat_server <server port>\n");
        exit(0);
    }
    daemonize(mx_atoi(argv[1]));

    return 0;
}
