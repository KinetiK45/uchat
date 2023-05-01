#include "../inc/uchat.h"

ssize_t send_message_by_socket(t_connected_peer *peer, char *sending_msg){
    ssize_t result_sending = SSL_write(peer->ssl, sending_msg, mx_strlen(sending_msg));
    return result_sending;
}
