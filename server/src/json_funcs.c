#include "../inc/uchat.h"

t_message* get_msg_info(char* buffer){
    t_message *msg = malloc(sizeof(t_message));
    char *temp;
    json_object *j_obj = json_tokener_parse(buffer);
    json_object_object_foreach((json_object *)j_obj, key, val){
        if(mx_strcmp("text", key) == 0){
            temp = (char *)json_object_get_string(val);
            msg -> text = mx_replace_substr(temp, "\n", "\0");
        }
        else if(mx_strcmp("date", key) == 0)
            msg -> date = (char *)json_object_get_string(val);
        else if(mx_strcmp("reciepient", key) == 0)
            msg -> reciepient = (int)json_object_get_int(val);
    }
    return msg;
}

t_registr *get_registr_info(char* buffer){
    t_registr *reg = malloc(sizeof(t_registr));
    reg-> login = NULL;
    reg-> pass = NULL;
    char *temp;
    json_object *j_obj = json_tokener_parse(buffer);
    json_object_object_foreach((json_object *)j_obj, key, val){
        if(mx_strcmp("newlogin", key) == 0){
            temp = (char *)json_object_get_string(val);
            reg -> login = mx_replace_substr(temp, "\n", "\0");
        }
        else if(mx_strcmp("newpass", key) == 0){
            temp = (char *)json_object_get_string(val);
            reg -> pass = mx_replace_substr(temp, "\n", "\0");
        }
    }
    return reg;
}

t_registr* get_login_info(char *buffer){
    t_registr *reg = malloc(sizeof(t_registr));
    char *temp;
    json_object *j_obj = json_tokener_parse(buffer);
    json_object_object_foreach((json_object *)j_obj, key, val){
        if(mx_strcmp("login", key) == 0){
            temp = (char *)json_object_get_string(val);
            reg -> login = mx_replace_substr(temp, "\n", "\0");
        }
        else if(mx_strcmp("pass", key) == 0){
            temp = (char *)json_object_get_string(val);
            reg -> pass = mx_replace_substr(temp, "\n", "\0");
        }
    }
    return reg;
}

char* get_action(char *msg){
    json_object *j_obj = json_tokener_parse(msg);
    json_object *j_request = json_object_new_object();
    json_object_object_get_ex(j_obj, "type", &j_request);
    return (char*)json_object_get_string(j_request);
}

void check_action(t_sock *sock, char *msg, t_connected_peer *peer){
    char* action = get_action(msg);
    if(mx_strcmp(action, REG) == 0){
        t_registr  *reg = get_registr_info(msg);
        if(register_new_user(reg -> login, reg -> pass)){
            int user_id = get_id_by_login(reg->login);
            write_socket_in_db(peer->clnt_socket, user_id);
            json_object* j_temp = json_object_new_object();
            json_object_object_add(j_temp, "type", json_object_new_string(REGIST_SUCCESS));
            json_object_object_add(j_temp, "user_id", json_object_new_int(user_id));
            send_message_by_socket(peer, (char*) json_object_get_string(j_temp));
        }
        else{
            json_object* j_temp = json_object_new_object();
            json_object_object_add(j_temp, "type", json_object_new_string(REG_ERROR));
            send_message_by_socket(peer,(char*)json_object_get_string(j_temp));        }
    }
    else if(mx_strcmp(action, LOG) == 0){
        int login_result = check_logpass_in_db(get_login_info(msg));
        if( login_result > 0){
            write_socket_in_db(peer->clnt_socket, login_result);
            send_message_by_socket(peer, (char*) login_success(get_login_info(msg)->login));
            int *chats_id = get_chats_for_user(login_result);
            if(chats_id != NULL){
                t_userinfo user_info;
                for(int i = 0; chats_id[i] != -1; i++){
                    user_info = get_userinfo_by_chatid_for_userid(chats_id[i], login_result);
                    json_object* res = userinfo_to_json(user_info);
                    json_object_object_add(res, "type", json_object_new_string("list_of_chats"));
                    send_message_by_socket(peer, (char*)json_object_get_string(res));
                    }
            }
        }
        else{
            json_object* j_temp = json_object_new_object();
            json_object_object_add(j_temp, "type", json_object_new_string(LOGIN_ERROR));
            send_message_by_socket(peer,(char*)json_object_get_string(j_temp));
        }
    }
    else if(mx_strcmp(action, SEARCH_USER) == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_pattern = json_object_new_object();
        json_object_object_get_ex(j_temp, "pattern", &j_pattern);
        if( find_user((char*)json_object_get_string(j_pattern)) != NULL){
            t_userinfo* userinfo = find_user((char*)json_object_get_string(j_pattern));
            json_object* j_array = json_object_new_array();
            int i = 0;
            while(userinfo[i].username != NULL){
                json_object* j_info = json_object_new_object();
                json_object_object_add(j_info,"user_id", json_object_new_int(userinfo[i].user_id));
                json_object_object_add(j_info, "username", json_object_new_string(userinfo[i].username));
                json_object_object_add(j_info,  "photo", json_object_new_string(userinfo[i].profile_img));
                json_object_array_add(j_array, j_info);
                i++;
            }
            json_object* jobj = json_object_new_object();
            json_object_object_add(jobj, "type", json_object_new_string(USER_FOUND));
            json_object_array_add(j_array, jobj);
            send_message_by_socket(peer, (char *)json_object_get_string(j_array));
        }
    }
    else if(mx_strcmp(action, MSG_HISTORY) == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_id = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_id);
        int id1 = json_object_get_int(j_id);
        json_object_object_get_ex(j_temp, "reciepient_id", &j_id);
        int id2 = json_object_get_int(j_id);
        create_history(peer, id1, id2);
    }
    else if(mx_strcmp(action, "msg") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int reciepient = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "text", &j_obj);
        char* text = (char *)json_object_get_string(j_obj);
        json_object_object_get_ex(j_temp, "sender", &j_obj);
        int sender = json_object_get_int(j_obj);
        register_new_msg_from_user(sender, reciepient, text);
        create_history(peer, sender, reciepient);
        int reciep_socket =  get_user_socket_by_id(reciepient);
        if(reciep_socket != -1){
            create_history(get_sock_for_send(sock, reciep_socket), reciepient, sender);
        }
    }
    else if(mx_strcmp(action, EDIT_PROFILE) == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "username", &j_obj);
        char* username = (char *)json_object_get_string(j_obj);
        if(mx_strcmp(username, "") != 0)
            change_username(user_id, username);
        json_object_object_get_ex(j_temp, "new_password", &j_obj);
        char* new_password = (char *)json_object_get_string(j_obj);
        if(mx_strcmp(new_password, "") != 0)
            change_pass_by_id(user_id, new_password);
    }
    else if(mx_strcmp(action, CUR_USER) == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        t_userinfo user_info = get_userinfo_by_id(user_id);
        json_object* res = userinfo_to_json(user_info);
        json_object_object_add(res, "type", json_object_new_string("cur_user"));
        send_message_by_socket(peer, (char*)json_object_get_string(res));
    }
    else if(mx_strcmp(action, DEL_USER) == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        delete_user_by_id(user_id);
    }
    else if(mx_strcmp(action, "edit_msg") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "msg_id", &j_obj);
        int msg_id = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "msg", &j_obj);
        char *new_msg = (char *)json_object_get_string(j_obj);
        change_msg_by_msgid(msg_id, new_msg);
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int id_1 = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int id_2 = json_object_get_int(j_obj);
        create_history(peer, id_1, id_2);
        int reciep_socket =  get_user_socket_by_id(id_2);
        if(reciep_socket != -1){
            create_history(get_sock_for_send(sock, reciep_socket), id_2, id_1);
        }
    }
    else if(mx_strcmp(action, "delete_msg") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "msg_id", &j_obj);
        int msg_id = json_object_get_int(j_obj);
        delete_msg(msg_id);
        json_object_object_get_ex(j_temp, "sender", &j_obj);
        int sender = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int reciepient = json_object_get_int(j_obj);
        create_history(peer, sender, reciepient);
        int reciep_socket =  get_user_socket_by_id(reciepient);
        if(reciep_socket != -1){
            create_history(get_sock_for_send(sock, reciep_socket), reciepient, sender);
        }
    }
    else if(mx_strcmp(action, "add_request") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int reciepient = json_object_get_int(j_obj);
        if(check_chat_existing(user_id, reciepient) == -1){
            initialize_new_chat(user_id, reciepient);
            send_message_by_socket(peer, user_to_add(reciepient));
            int reciep_sock = get_user_socket_by_id(reciepient);
            if(reciep_sock != -1){
                send_message_by_socket(get_sock_for_send(sock, reciep_sock), user_to_add(user_id));
            }
        }
    }
    else if(mx_strcmp(action, "info_about_user") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int reciepient = json_object_get_int(j_obj);
        t_userinfo user_info = get_userinfo_by_id(reciepient);
        if(user_info.username != NULL){
            int chat_id = get_chatid_between_users(user_id, user_info.user_id);
            int num = get_msg_size(chat_id) - 2;
            t_chat_msg* chat_msg = get_msg_history(-1, -1, chat_id);
            int i = 0;
            int count_files = 0;
            if(chat_msg != NULL){
                while(chat_msg[i].time != NULL){
                    if(get_files_from_msg(chat_msg[i].msg_id) != NULL){
                        count_files++;
                    }
                    i++;
                }
            }
            send_message_by_socket(peer, info_about_user(user_info, num, count_files));
        }
    }
    else if(mx_strcmp(action, "delete_chat") == 0){
        json_object* j_temp = json_tokener_parse(msg);
        json_object* j_obj = json_object_new_object();
        json_object_object_get_ex(j_temp, "user_id", &j_obj);
        int user_id = json_object_get_int(j_obj);
        json_object_object_get_ex(j_temp, "reciepient", &j_obj);
        int reciepient = json_object_get_int(j_obj);
        if(check_chat_existing(user_id, reciepient) != -1){
            int chat_id = get_chatid_between_users(user_id, reciepient);
            delete_chat(chat_id);
            send_message_by_socket(peer, chat_deleted(reciepient));
            int reciep_socket =  get_user_socket_by_id(reciepient);
            if(reciep_socket != -1){
                send_message_by_socket(get_sock_for_send(sock, reciep_socket), chat_deleted(user_id));
            }
        }
    }
    else if(mx_strcmp(action, "clear_history") == 0){
         json_object* j_temp = json_tokener_parse(msg);
         json_object* j_obj = json_object_new_object();
         json_object_object_get_ex(j_temp, "user_id", &j_obj);
         int user_id = json_object_get_int(j_obj);
         json_object_object_get_ex(j_temp, "reciepient", &j_obj);
         int reciepient = json_object_get_int(j_obj);
        t_userinfo user_info = get_userinfo_by_id(reciepient);
         if(check_chat_existing(user_id, reciepient) != -1){
            int chat_id = get_chatid_between_users(user_id, reciepient);
            delete_chat_history_by_id(chat_id);
            int num = get_msg_size(chat_id) - 2;
            t_chat_msg* chat_msg = get_msg_history(-1, -1, chat_id);
            int i = 0;
            int count_files = 0;
                if(chat_msg != NULL){
                    while(chat_msg[i].time != NULL){
                        if(get_files_from_msg(chat_msg[i].msg_id) != NULL){
                            count_files++;
                        }
                    i++;
                    }
                }
            send_message_by_socket(peer, info_about_user(user_info, num, count_files));
            create_history(peer, user_id, reciepient);
            int reciep_socket =  get_user_socket_by_id(reciepient);
            if(reciep_socket != -1){
                create_history(get_sock_for_send(sock, reciep_socket), reciepient, user_id);
            }
         }
     }
}

char* info_about_user(t_userinfo user_info, int num, int count_files){
    json_object* j_res = json_object_new_object();
    json_object_object_add(j_res, "profile_img", json_object_new_string(user_info.profile_img));
    json_object_object_add(j_res, "num_of_msgs", json_object_new_int(num));
    json_object_object_add(j_res, "num_of_files", json_object_new_int(count_files));
    json_object_object_add(j_res, "reciepient", json_object_new_int(user_info.user_id));
    json_object_object_add(j_res, "username", json_object_new_string(user_info.username));
    json_object_object_add(j_res, "type", json_object_new_string("info_about_user"));
    return (char *)json_object_get_string(j_res);
}

char* chat_deleted(int reciepient){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "reciepient", json_object_new_int(reciepient));
    json_object_object_add(j_obj, "type", json_object_new_string("chat_deleted"));
    return (char *)json_object_get_string(j_obj);
}

char* user_to_add(int reciepient){
    t_userinfo user_info = get_userinfo_by_id(reciepient);
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(reciepient));
    json_object_object_add(j_obj, "profile_img", json_object_new_string(user_info.profile_img));
    json_object_object_add(j_obj, "username", json_object_new_string(user_info.username));
    json_object_object_add(j_obj, "type", json_object_new_string("accepted"));
    return (char *)json_object_get_string(j_obj);
}

void create_history(t_connected_peer* peer, int id_1, int id_2){
    t_chat_msg* chat_msg = get_msg_history(id_1, id_2, -1);
    json_object* j_res = json_object_new_object();
    json_object_object_add(j_res, "type", json_object_new_string(MSG_HISTORY));
    json_object_object_add(j_res, "current_chat", json_object_new_int(id_2));
    json_object* j_array = json_object_new_array();
    int i = 2;
    if(chat_msg != NULL){
        while(chat_msg[i].msg_text){
            json_object* j_temp = json_object_new_object();
            json_object_object_add(j_temp, "time", json_object_new_string(chat_msg[i].time));
            json_object_object_add(j_temp, "text", json_object_new_string(chat_msg[i].msg_text));
            json_object_object_add(j_temp, "msg_id", json_object_new_int(chat_msg[i].msg_id));
            json_object_object_add(j_temp, "sender_id", json_object_new_int(chat_msg[i].sender_id));
            char** file = get_files_from_msg(chat_msg[i].msg_id);
            if(file != NULL){
                json_object_object_add(j_temp, "filename", json_object_new_string(file[0]));
            }
            else
                json_object_object_add(j_temp, "filename", json_object_new_string("no_file"));
            json_object_array_add(j_array, j_temp);
            i++;
        }
        json_object_object_add(j_res, "history_status", json_object_new_string("full"));
        json_object_object_add(j_res, "array_of_msgs", j_array);
        send_message_by_socket(peer,(char *)json_object_get_string(j_res));
    }
    else{
        json_object_object_add(j_res, "history_status", json_object_new_string("empty"));
        send_message_by_socket(peer, (char *)json_object_get_string(j_res));
    }
}

json_object* userinfo_to_json(t_userinfo user_info){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "username", json_object_new_string(user_info.username));
    json_object_object_add(j_obj, "profile_img", json_object_new_string(user_info.profile_img));
    json_object_object_add(j_obj, "user_id", json_object_new_int(user_info.user_id));
    return j_obj;
}

char* login_success(char* login){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "login", json_object_new_string(login));
    json_object_object_add(j_obj, "user_id", json_object_new_int(get_id_by_login(login)));
    json_object_object_add(j_obj, "type", json_object_new_string(LOGIN_SUCCESS));

    return (char *)json_object_get_string(j_obj);
}
