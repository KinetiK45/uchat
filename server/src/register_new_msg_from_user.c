#include "../inc/uchat.h"

void register_new_msg_from_user(int sender_id, int send_to, char *msg){
    int chat_id = get_chatid_between_users(sender_id, send_to);
    time_t now = time(0);
    char *temp = ctime(&now);
    char *time = mx_strndup(temp, mx_strlen(temp) - 1);
    register_new_msg_in_db(chat_id, sender_id, time, msg);
    mx_strdel(&time);
}
