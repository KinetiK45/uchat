#include "../inc/client.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        mx_printerr("Usage: ./uchat ‹server IP> <server port>\n");
        exit(0);
    }
    t_sock* sock = get_socket(argv);
    init_client(argc,argv,sock);
    return 0;
    
}
