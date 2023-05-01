#include "../inc/client.h"

void check_server_response(char *buff, t_sock* sock){
    if(current_user == -1){
        if(check_login_info(buff)){
               gdk_threads_add_idle((GSourceFunc)gtk_widget_hide, window);
               gdk_threads_add_idle(interface, wrapper(sock));
           }
           else if(check_login_info(buff) == FALSE){
               gtk_entry_set_placeholder_text(GTK_ENTRY(p_entry),"Incorect password or login");
               gtk_entry_set_placeholder_text(GTK_ENTRY(l_entry),"Incorect password or login");
           }
            if(check_reg_info(buff)){
               gdk_threads_add_idle((GSourceFunc)gtk_widget_hide, registration_window);
               gdk_threads_add_idle(interface, wrapper(sock));
           }
           else if(check_reg_info(buff) == FALSE){
               gtk_entry_set_placeholder_text(GTK_ENTRY(registration_login),"Registration error");
               gtk_entry_set_placeholder_text(GTK_ENTRY(registration_password),"Registration error");
               gtk_entry_set_placeholder_text(GTK_ENTRY(registration_confirm),"Registration error");
           }
    }
    else if(check_users_info(buff)){
        t_user_info* user_info = get_users_info(buff);
        int i = 0;
        while(user_info[i].photo_path){
            user_info[i].sock = sock;
            if(user_info[i].user_id != current_user){
                gdk_threads_add_idle((GSourceFunc)show_user_what_found, &user_info[i]);
            }
            i++;
        }
    }
    else if(check_history_info(buff)){
        t_chat_msg* chat_msg = get_msg_history(buff);
        gdk_threads_add_idle((GSourceFunc)sent, chat_msg);
    }
    else if(mx_strcmp(check_user_info(buff), "cur_user") == 0){
         t_user_info* user_info = get_user_info(buff);
         user_info->sock = sock;
         gdk_threads_add_idle((GSourceFunc)change_profile, user_info);
    }
    else if(mx_strcmp(check_user_info(buff), "list_of_chats") == 0){
          t_user_info* user_info = get_user_info(buff);
          user_info->sock = sock;
          gdk_threads_add_idle((GSourceFunc)add_new_to_box_message, user_info);
    }
    else if(mx_strcmp(check_user_info(buff), "accepted") == 0){
          t_user_info* user_info = get_user_info(buff);
          user_info->sock = sock;
          gdk_threads_add_idle((GSourceFunc)add_new_to_box_message, user_info);
     }
    else if(mx_strcmp(check_user_info(buff), "info_about_user") == 0){
          get_info_about_user(buff);
          gdk_threads_add_idle((GSourceFunc)info_about_user, NULL);
    }
    else if(mx_strcmp(check_user_info(buff), "chat_deleted") == 0){
        get_user_to_delete(buff);
        gdk_threads_add_idle((GSourceFunc)delete_user_box, NULL);
     }
}

void *recv_messages_from_server(void *thread_args) {

    t_sock *clnt_sock = ((struct s_thread_args*)thread_args)->client_sock;
    size_t rcv_bytes;
    for(;;) {
        if((rcv_bytes = SSL_read(clnt_sock->ssl, buff, RECV_BUFF_SIZE)) <= 0){
            gdk_threads_add_idle((GSourceFunc)window_reconnect, NULL);
            while(true){}
        }
        buff[rcv_bytes] = '\0';
        check_server_response(buff, clnt_sock);
        bzero(buff,RECV_BUFF_SIZE);
    }
}
