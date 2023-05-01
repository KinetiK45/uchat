#include "../inc/client.h"

char* gettime(void){
    time_t now;
    time(&now);
    struct tm *date_time = localtime(&now);
    json_object *j_time = json_object_new_object();
    json_object_object_add(j_time, "hours", json_object_new_int(date_time ->tm_hour));
    json_object_object_add(j_time, "minutes", json_object_new_int(date_time ->tm_min));
    json_object_object_add(j_time, "seconds", json_object_new_int(date_time ->tm_sec));
    json_object_object_add(j_time, "day", json_object_new_int(date_time ->tm_mday));
    json_object_object_add(j_time, "month", json_object_new_int(date_time ->tm_mon+1));
    json_object_object_add(j_time, "year", json_object_new_int(date_time ->tm_year+1900));
    return (char *)json_object_to_json_string(j_time);
}

char* create_msg(char* sending_msg, int recepient){
    json_object *j_msg = json_object_new_object();
    json_object_object_add(j_msg, "text", json_object_new_string(sending_msg));
    //json_object_object_add(j_msg, "date", json_tokener_parse(gettime()));
    json_object_object_add(j_msg, "sender", json_object_new_int(current_user));
    json_object_object_add(j_msg, "reciepient", json_object_new_int(recepient));
    json_object_object_add(j_msg, "type", json_object_new_string("msg"));
    return (char *)json_object_to_json_string(j_msg);
}

char* request_search_user(char* username){
    json_object *j_msg = json_object_new_object();
    json_object_object_add(j_msg, "username", json_object_new_string(username));
    json_object_object_add(j_msg, "type", json_object_new_string(SEARCH_USER));
    return (char *)json_object_to_json_string(j_msg);
}

char* create_reg(char* login, char* pass){
    if(mx_strtrim(login) != NULL && mx_strtrim(pass) != NULL){
        json_object *j_msg = json_object_new_object();
           json_object_object_add(j_msg, "newlogin", json_object_new_string(login));
           json_object_object_add(j_msg, "newpass", json_object_new_string(pass));
           json_object_object_add(j_msg, "type", json_object_new_string(REG));
           return (char *)json_object_to_json_string(j_msg);
    }
    else
        return NULL;
}

gboolean request_info_about_user(GtkButton* button, gpointer data){
    if(mx_strcmp(gtk_button_get_label(button), "Group or username") != 0){
        json_object* j_obj = json_object_new_object();
           json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
           json_object_object_add(j_obj, "reciepient", json_object_new_int(rabotyaga->user_id));
           json_object_object_add(j_obj, "type", json_object_new_string("info_about_user"));
           send_message(rabotyaga->sock, json_object_get_string(j_obj));
    }
    return FALSE;
}

void get_info_about_user(char* buff){
     json_object* j_temp = json_tokener_parse(buff);
    json_object* j_obj = json_object_new_object();
    json_object_object_get_ex(j_temp, "profile_img", &j_obj);
    rabotyaga->profile_img = json_object_get_string(j_obj);
    json_object_object_get_ex(j_temp, "username", &j_obj);
    rabotyaga->username = json_object_get_string(j_obj);
    json_object_object_get_ex(j_temp, "num_of_msgs", &j_obj);
    rabotyaga->num_msgs = json_object_get_int(j_obj);
    json_object_object_get_ex(j_temp, "num_of_files", &j_obj);
    rabotyaga->count_files = json_object_get_int(j_obj);
    json_object_object_get_ex(j_temp, "reciepient", &j_obj);
    rabotyaga->user_id = json_object_get_int(j_obj);
}

char* create_login(char* login, char* pass){
    json_object *j_msg = json_object_new_object();
    json_object_object_add(j_msg, "login", json_object_new_string(login));
    json_object_object_add(j_msg, "pass", json_object_new_string(pass));
    json_object_object_add(j_msg, "type", json_object_new_string(LOG));
    return (char *)json_object_to_json_string(j_msg);
}

char* edit_profile(char* username, char* password){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "username", json_object_new_string(username));
    json_object_object_add(j_obj, "new_password", json_object_new_string(password));
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "type", json_object_new_string(EDIT_PROFILE));
    return (char *)json_object_to_json_string(j_obj);
}


bool check_users_info(char* buff){
    json_object* j_temp = json_tokener_parse(buff);
    if(json_object_get_type(j_temp) != json_type_array){
        return false;
    }
    size_t array_len = json_object_array_length(j_temp);
    json_object *j_obj = json_object_array_get_idx(j_temp, array_len-1);
    json_object* j_type = json_object_new_object();
    json_object_object_get_ex(j_obj, "type", &j_type);
    if(mx_strcmp(USER_FOUND, json_object_get_string(j_type)) == 0){
        return true;
    }
    else
        return false;
}

char* check_user_info(char* buff){
    json_object* j_temp = json_tokener_parse(buff);
    if(json_object_get_type(j_temp) == json_type_array){
        return false;
    }
    json_object* j_type = json_object_new_object();
    json_object_object_get_ex(j_temp, "type", &j_type);
    return (char*)json_object_get_string(j_type);
}

bool check_login_info(char* buff){
    json_object *j_obj = json_tokener_parse(buff);
    if(json_object_get_type(j_obj) == json_type_array){
        return false;
    }
    json_object *j_info = json_object_new_object();
    json_object_object_get_ex(j_obj, "type", &j_info);
    if(mx_strcmp(json_object_get_string(j_info), LOGIN_SUCCESS) == 0){
        json_object_object_get_ex(j_obj, "user_id", &j_info);
        current_user = json_object_get_int(j_info);
        return true;
    }
    else
        return false;
}

gboolean add_request(GtkButton* button, gpointer data){
    t_user_info* user_info = (t_user_info *)data;
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(user_info->user_id));
    json_object_object_add(j_obj, "type", json_object_new_string("add_request"));
    send_message(user_info->sock, (char *)json_object_get_string(j_obj));
    return FALSE;
}

gboolean delete_chat(GtkButton* button, gpointer data){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(rabotyaga->user_id));
    json_object_object_add(j_obj, "type", json_object_new_string("delete_chat"));
    send_message(rabotyaga->sock, json_object_get_string(j_obj));
    return FALSE;
}

void get_user_to_delete(char* buff){
    json_object* j_obj = json_tokener_parse(buff);
    json_object* j_info = json_object_new_object();
    json_object_object_get_ex(j_obj, "reciepient", &j_info);
    rabotyaga -> user_id = json_object_get_int(j_info);
}

gboolean clear_history_chat(GtkButton* button, gpointer data){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(rabotyaga->user_id));
    json_object_object_add(j_obj, "type", json_object_new_string("clear_history"));
    send_message(rabotyaga->sock, json_object_get_string(j_obj));
    return FALSE;
}

bool check_reg_info(char* buff){
    json_object *j_obj = json_tokener_parse(buff);
    if(json_object_get_type(j_obj) == json_type_array){
        return false;
    }
    json_object *j_info = json_object_new_object();
    json_object_object_get_ex(j_obj, "type", &j_info);
    if(mx_strcmp(json_object_get_string(j_info), REGIST_SUCCESS) == 0){
        json_object_object_get_ex(j_obj, "user_id", &j_info);
        current_user = json_object_get_int(j_info);
        return true;
    }
    else
        return false;
}

char* create_file_warning(char *file_name,int reciepient_id, unsigned long filesize, char *msg_text) {
    json_object *j_obj = json_object_new_object();
    json_object_object_add(j_obj, "file_name", json_object_new_string(file_name));
    json_object_object_add(j_obj, "msg_text", json_object_new_string(msg_text));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(reciepient_id));
    json_object_object_add(j_obj, "type", json_object_new_string("big_file_with_text"));
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "filesize", json_object_new_int64(filesize));
    return (char *)json_object_get_string(j_obj);
    
}

t_user_info* get_users_info(char* buff){
    json_object* j_user_info = json_tokener_parse(buff);
    int array_len = json_object_array_length(j_user_info);
    json_object *j_temp = json_object_new_object();
    t_user_info* user_info = malloc(sizeof(t_user_info)*(array_len+1));
    for(int i = 0; i < array_len; i++){
        json_object *j_obj = json_object_array_get_idx(j_user_info, i);
        json_object_object_get_ex(j_obj,  "user_id", &j_temp);
        user_info[i].user_id = json_object_get_int(j_temp);
        json_object_object_get_ex(j_obj, "username", &j_temp);
        user_info[i].username = (char*)json_object_get_string(j_temp);
        json_object_object_get_ex(j_obj, "photo", &j_temp);
        user_info[i].photo_path =(char*) json_object_get_string(j_temp);
    }
    user_info[array_len].username = NULL;
    user_info[array_len].photo_path = NULL;
    user_info[array_len].user_id = -1;
    return user_info;
}

char* edit_msg(char* new_msg, int msg_id){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "msg", json_object_new_string(new_msg));
    json_object_object_add(j_obj, "msg_id", json_object_new_int(msg_id));
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(rabotyaga->user_id));
    json_object_object_add(j_obj, "type", json_object_new_string("edit_msg"));
    return (char *)json_object_get_string(j_obj);
}

char* delete_msg(int msg_id, int reciepient){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "msg_id", json_object_new_int(msg_id));
    json_object_object_add(j_obj, "sender", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient", json_object_new_int(reciepient));
    json_object_object_add(j_obj, "type", json_object_new_string("delete_msg"));
    return (char *)json_object_get_string(j_obj);
}

t_user_info* get_user_info(char* buff){
    json_object* j_user_info = json_tokener_parse(buff);
    json_object *j_temp = json_object_new_object();
    t_user_info* user_info = malloc(sizeof(t_user_info));
    json_object_object_get_ex(j_user_info,  "user_id", &j_temp);
    user_info->user_id = json_object_get_int(j_temp);
    json_object_object_get_ex(j_user_info, "username", &j_temp);
    user_info->username = (char*)json_object_get_string(j_temp);
    json_object_object_get_ex(j_user_info, "profile_img", &j_temp);
    user_info->photo_path =(char*) json_object_get_string(j_temp);
    return user_info;
}
void request_photo_change(char *name,t_sock *sock,unsigned long filesize) {
    json_object* jobj = json_object_new_object();
    json_object_object_add(jobj, "user_id", json_object_new_int64(current_user));
    json_object_object_add(jobj,"new_photo", json_object_new_string(name));
    json_object_object_add(jobj,"type", json_object_new_string("big_file"));
    json_object_object_add(jobj, "filesize", json_object_new_int64(filesize));
    
    send_message(sock, (char*)json_object_get_string(jobj));
    
}
int mx_str_cmp(const char *s1, const char *s2){
    while (*s1 || *s2) {
        if(*s1 != *s2){
            return *(const unsigned char*)s1 - *(const unsigned char*)s2;
        }
        s1++;
        s2++;
    }
    return 0;
}
