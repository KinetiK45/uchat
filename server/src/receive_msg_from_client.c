#include "../inc/uchat.h"

void disconnect(t_sock *sock, t_connected_peer *peer) {
    close(SSL_get_fd(peer->ssl));
    peer->clnt_socket = -1;
    peer->user_id = -1;
    bzero(peer->recv_buff, BUFF_SIZE);
    bzero(peer->send_buff,BUFF_SIZE);
}

void mx_clear_buff(char buff[BUFF_SIZE]){
    for (int i = 0; i < BUFF_SIZE; i++) {
        buff[i] = '\0';
    }
}

int SSL_read_all(SSL *ssl, void* buf, int num)
{
    char* ptr = (char*)buf;
    int read_bytes = 0;
    while (read_bytes < num)
    {
        int r = SSL_read(ssl, ptr + read_bytes, num - read_bytes);
        if (r <= 0)
            return r;
        read_bytes += r;
    }
    return read_bytes;
}


unsigned long check_on_big_file(t_sock* sock, char *msg, t_connected_peer *peer) {
    char *action = get_action(msg);
    unsigned long filesize = 0;
    if (mx_strcmp(action,BIG_FILE) == 0) {
        peer->reading_mode = 1;
        json_object* jobj = json_tokener_parse(msg);
        json_object* jtemp = json_object_new_object();
        json_object_object_get_ex(jobj, "user_id", &jtemp);
        int user_id = json_object_get_int(jtemp);
        json_object_object_get_ex(jobj, "new_photo", &jtemp);
        peer->file_path = mx_strjoin(RESOURCES_LOCATION, (char *)json_object_get_string(jtemp));
        change_profile_img_to_accid(user_id, (char*)json_object_get_string(jtemp));
        json_object_object_get_ex(jobj, "filesize", &jtemp);
        return filesize = json_object_get_int64(jtemp);
    }
    else if (mx_strcmp(action,"big_file_with_text") == 0) {
        peer->reading_mode = 1;
        json_object* jobj = json_tokener_parse(msg);
        json_object* jtemp = json_object_new_object();
        json_object_object_get_ex(jobj, "user_id", &jtemp);
        peer->user_id = json_object_get_int(jtemp);
        json_object_object_get_ex(jobj, "reciepient", &jtemp);
        peer->recipient_id = json_object_get_int(jtemp);
        json_object_object_get_ex(jobj, "msg_text", &jtemp);
        char* msg_text = (char*)json_object_get_string(jtemp);
        json_object_object_get_ex(jobj, "file_name", &jtemp);
        char* filename = (char*)json_object_get_string(jtemp);
        peer->file_path = mx_strjoin(RESOURCES_LOCATION, filename);
        json_object_object_get_ex(jobj, "filesize", &jtemp);
        if (mx_strcmp(msg_text,"") == 0)
            register_new_msg_from_user( peer->user_id, peer->recipient_id, " ");
        else
            register_new_msg_from_user( peer->user_id, peer->recipient_id, msg_text);
        t_chat_msg *msg = get_msg_history( peer->user_id, peer->recipient_id, -1);
        int i = 0;
        while(msg[i + 1].time != NULL)
            i++;
        
        add_file_to_msg_id(msg[i].msg_id, filename);
        create_history(peer, peer->user_id, peer->recipient_id);
        int reciep_socket =  get_user_socket_by_id(peer->recipient_id);
               if(reciep_socket != -1){
                   create_history(get_sock_for_send(sock, reciep_socket), peer->recipient_id, peer->user_id);
               }
        return filesize = json_object_get_int64(jtemp);
    }
    peer->reading_mode = 0;
    return filesize;
}

void recieve_msg_from_client(t_sock *sock, t_connected_peer *peer) {
    char *msg = NULL;
    FILE *rcv_file;
    unsigned long total_bytes_read = 0;
    if(SSL_accept(peer->ssl) <= 0)
        disconnect(sock, peer);
    int num_of_bytes;
    if (peer->reading_mode == SHORT_READ_MODE) {
        if ((num_of_bytes = SSL_read(peer->ssl, (char *)&peer->recv_buff,BUFF_SIZE)) <= 0){
            delete_socket_from_db(peer->clnt_socket);
            disconnect(sock, peer);
        }
        peer->recv_buff[num_of_bytes] = '\0';
        msg = mx_replace_substr(peer->recv_buff, "\n", "\0");
    }
    else {
        rcv_file = fopen(peer->file_path, "wb");
        while (total_bytes_read < peer->filesize) {
            mx_memset(&peer->recv_buff, 0, sizeof(peer->recv_buff));
            if ((num_of_bytes = SSL_read(peer->ssl, peer->recv_buff, BUFF_SIZE)) < 0){
                delete_socket_from_db(peer->clnt_socket);
                disconnect(sock, peer);
                die_with_error("SSL READ FAILED");
            }
            total_bytes_read += num_of_bytes;
            fwrite(peer->recv_buff, 1, num_of_bytes, rcv_file);
        }
        fclose(rcv_file);
        peer->reading_mode = 0;
    }
    if (num_of_bytes > 0 && msg != NULL) {
        peer->filesize = check_on_big_file(sock, msg, peer);
        check_action(sock, msg, peer);
    }
    
    msg = NULL;
}

