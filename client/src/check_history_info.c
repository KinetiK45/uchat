#include "../inc/client.h"

bool check_history_info(char* buff){
        json_object* j_temp = json_tokener_parse(buff);
        json_object* j_type = json_object_new_object();
        json_object_object_get_ex(j_temp, "type", &j_type);
        char* type = json_object_get_string(j_type);
        json_object_object_get_ex(j_temp, "current_chat", &j_type);
        int cur_chat = json_object_get_int(j_type);
        if(mx_strcmp(MSG_HISTORY, type) == 0 && cur_chat == rabotyaga->user_id){
            return true;
        }
        else
            return false;
   
}

t_chat_msg* get_msg_history(char* buff){
    json_object* j_temp = json_tokener_parse(buff);
    json_object* j_status = json_object_new_object();
    json_object* j_array = json_object_new_array();
    json_object_object_get_ex(j_temp, "history_status", &j_status);
    if(mx_strcmp("full", json_object_get_string(j_status)) == 0){
        json_object_object_get_ex(j_temp, "array_of_msgs", &j_array);
        int array_len = json_object_array_length(j_array);
        t_chat_msg* chat_msg = malloc(sizeof(t_chat_msg)*(array_len+1));
        for(int i = 0; i < array_len; i++){
            json_object *j_obj = json_object_array_get_idx(j_array, i);
            json_object* j_a = json_object_new_object();
            json_object_object_get_ex(j_obj, "time", &j_a);
            chat_msg[i].time = (char *)json_object_get_string(j_a);
            json_object_object_get_ex(j_obj, "text", &j_a);
            chat_msg[i].msg_text = get_text_no_files((char *)json_object_get_string(j_a));
            json_object_object_get_ex(j_obj, "msg_id", &j_a);
            chat_msg[i].msg_id = json_object_get_int(j_a);
            json_object_object_get_ex(j_obj, "sender_id", &j_a);
            chat_msg[i].sender_id = json_object_get_int(j_a);
            json_object_object_get_ex(j_obj, "filename", &j_a);
            chat_msg[i].filename =(char*) json_object_get_string(j_a);
            if(mx_strcmp(chat_msg[i].filename, "no_file") == 0){
                chat_msg[i].filename = NULL;
            }
        }
        chat_msg[array_len].msg_text = NULL;
        chat_msg[array_len].time = NULL;
        chat_msg[array_len].msg_id = -1;
        chat_msg[array_len].sender_id = -1;
        chat_msg[array_len].filename = NULL;
        return chat_msg;
    }
    else
        return NULL;
}
