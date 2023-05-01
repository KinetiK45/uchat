#include "../inc/client.h"


void request_msg_history(GtkButton* button, gpointer data){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "reciepient_id", json_object_new_int(mx_atoi(gtk_widget_get_name(GTK_WIDGET(button)))));
    json_object_object_add(j_obj, "type", json_object_new_string(MSG_HISTORY));
    send_message(((t_rabotyaga_reciepient*)data)->sock, (char *)json_object_get_string(j_obj));
}
