#include "../inc/client.h"

gboolean set_scrollbar_position_to_last_message(GtkScrolledWindow* scrolled_window) {
    GtkAdjustment *adjustment;
    double max_value = 0.0;
    adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll_massage));
    max_value = gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment);
    if (max_value < 1) return TRUE;
    gtk_adjustment_set_value(adjustment, max_value);
    return FALSE;
}

void set_style_file_inspector(GtkWidget * entry){
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_style_context_add_class(context, "file_inspector");
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(sent_message));
    GtkTextIter cursor_iter;
    int step = 40;
    gtk_text_buffer_get_iter_at_mark(buffer, &cursor_iter, gtk_text_buffer_get_insert(buffer));
    gint cursor_position = gtk_text_iter_get_offset(&cursor_iter);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    if ((cursor_position - mx_count_substr(text, "\n")) % 30 == 0 && cursor_position != 0){
        gtk_text_buffer_insert(buffer, &cursor_iter, "\n", 1);

    }
    return FALSE;
}

gboolean on_sent_message_focus_in(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    
    GtkTextIter start, end;
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(sent_message));
    gtk_text_buffer_get_bounds (buffer, &start, &end);
    char *text = mx_strtrim(gtk_text_buffer_get_text (buffer, &start, &end, FALSE));
    if (text != NULL && mx_strcmp(text, "Write text here...") == 0){
        gtk_text_buffer_set_text(buffer, "", -1);
        gtk_widget_set_name(sent_message, "sent_message");
    }
    return FALSE;

}

gboolean on_sent_message_focus_out(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    GtkTextIter start, end;
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(sent_message));
    gtk_text_buffer_get_bounds (buffer, &start, &end);
    char *text = mx_strtrim(gtk_text_buffer_get_text (buffer, &start, &end, FALSE));
    if (!text){
        const gchar *text = "Write text here...";
        gtk_text_buffer_set_text(buffer, text, -1);
        gtk_widget_set_name(sent_message, "sent_message_inactive");
    }
    
    return FALSE;
}

void set_entry_style(GtkWidget * entry){
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_style_context_add_class(context, "text_entry");
}

gboolean on_textview_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        send_clnts_msg_to_server(NULL, rabotyaga);
        return TRUE;
    }
    
    return FALSE;
}


void destroy_widget_with_id(GtkWidget *widget, gpointer data) {
    if(mx_atoi(gtk_widget_get_name(widget)) == rabotyaga->user_id){
        gtk_widget_destroy(widget);
    }
}

void destroy_widget(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(widget);
}

gboolean request_delete_acc(GtkButton* button, gpointer data){
       json_object* j_obj = json_object_new_object();
       json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
       json_object_object_add(j_obj, "type", json_object_new_string(DEL_USER));
       send_message(((t_sock *)data), (char*)json_object_get_string(j_obj));
    gtk_widget_destroy(interface_window);
       return FALSE;
}

gboolean request_editing(GtkButton* button, gpointer data){
    char* username = NULL;
    username = (char*)gtk_entry_get_text(GTK_ENTRY(entry_status));
    char* password = NULL;
    password = (char*)gtk_entry_get_text(GTK_ENTRY(entry_name));
    if(mx_strtrim(username) != NULL || mx_strtrim(password) != NULL){
        send_message(((t_sock *)data), edit_profile(username, password));
    }
    return FALSE;
}


gboolean request_current_user(GtkButton* button, gpointer data){
    json_object* j_obj = json_object_new_object();
    json_object_object_add(j_obj, "user_id", json_object_new_int(current_user));
    json_object_object_add(j_obj, "type", json_object_new_string(CUR_USER));
    send_message(((t_wrapper *)data)->sock, (char*)json_object_get_string(j_obj));
    return FALSE;
}

void search_request(GtkButton *button, gpointer data){
    char* l_data =  (char *)gtk_entry_get_text(GTK_ENTRY(search_user));
    gtk_container_foreach(GTK_CONTAINER(box_show), destroy_widget, NULL);
    if(mx_strlen(l_data) > 2){
        json_object* j_obj = json_object_new_object();
        json_object_object_add(j_obj, "pattern", json_object_new_string(l_data));
        json_object_object_add(j_obj, "type", json_object_new_string(SEARCH_USER));
        send_message( ((t_wrapper *)data)->sock, (char *)json_object_to_json_string(j_obj));
    }
}

void send_clnts_msg_to_server(GtkButton *button, gpointer data) {
    GtkTextIter start, end;
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(sent_message));
    gtk_text_buffer_get_bounds (buffer, &start, &end);
    char *text = mx_strtrim(gtk_text_buffer_get_text (buffer, &start, &end, FALSE));
    if(mx_strtrim(text) != NULL){
        send_message(rabotyaga->sock, create_msg(text, rabotyaga->user_id));
        gtk_text_buffer_delete(buffer, &start, &end);
        
    }
}

gboolean delete_user_box(gpointer data){
    gtk_container_foreach(GTK_CONTAINER(box_user), destroy_widget_with_id, NULL);
    gtk_container_foreach(GTK_CONTAINER(box_show_message), destroy_widget, NULL);
    gtk_container_foreach(GTK_CONTAINER(box_get_message), destroy_widget, NULL);
    gtk_button_set_label(GTK_BUTTON(button_dialog_name),"Group or username");
    return FALSE;
}

gboolean interface(gpointer data) {

    GtkWidget* out_search = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(out_search),"Password");
    interface_window = gtk_application_window_new(app);
    gtk_widget_set_size_request(interface_window, 1120, -1);
    gtk_window_set_title(GTK_WINDOW(interface_window), "uChaiber");
    gtk_window_set_default_size(GTK_WINDOW(interface_window), 1200, 800);
    GtkWidget* interface_planed = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_position(GTK_PANED(interface_planed), 400);
    interface_box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    interface_box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_paned_add1(GTK_PANED(interface_planed), interface_box1);
    GtkWidget* interface_fixed = gtk_fixed_new();
    gtk_widget_set_size_request(interface_fixed, 400, 60);
    gtk_box_pack_start(GTK_BOX(interface_box1), interface_fixed, FALSE, FALSE, 0);
    
    interface_search = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(interface_search),"Search...");
    set_entry_style(interface_search);
    set_entry_style(registration_login);
    gtk_widget_set_size_request(interface_search, 330, 60);
    
    gtk_fixed_put(GTK_FIXED(interface_fixed),interface_search,70,0);
    GtkWidget* button_profile = gtk_button_new();
    gtk_widget_set_name(button_profile, "button_profile");
    gtk_widget_set_size_request(button_profile, 60, 60);
    gtk_button_set_label(GTK_BUTTON(button_profile), "Profile");
    gtk_fixed_put(GTK_FIXED(interface_fixed),button_profile,0,0);
    g_signal_connect(button_profile,"clicked",G_CALLBACK(request_current_user),data);

    box_user = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    scroll_user = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scroll_user), 10);
    gtk_container_add(GTK_CONTAINER(scroll_user), box_user);
    gtk_box_pack_start(GTK_BOX(interface_box1), scroll_user, TRUE, TRUE, 0);
    GtkWidget* low_button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    GtkWidget* button_add_user = gtk_button_new_with_label("Add user");
    gtk_widget_set_name(button_add_user, "button_add_user");
    gtk_widget_set_size_request(button_add_user, -1, 80);

    gtk_box_pack_start(GTK_BOX(low_button_box), button_add_user, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(interface_box1), low_button_box, FALSE, FALSE, 0);
    g_signal_connect(button_add_user,"clicked",G_CALLBACK(add_new_dialog),data);
    
    gtk_paned_add2(GTK_PANED(interface_planed), interface_box2);
    button_dialog_name = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_dialog_name), "Group or username");
    gtk_widget_set_name(button_dialog_name, "button_dialog_name");

    gtk_widget_set_size_request(button_dialog_name, -1, 60);
    gtk_box_pack_start(GTK_BOX(interface_box2), button_dialog_name, FALSE, FALSE, 0);
    g_signal_connect(button_dialog_name,"clicked",G_CALLBACK(request_info_about_user),NULL);

    scroll_massage = gtk_scrolled_window_new(NULL, NULL);
    box_show_message = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(interface_box2), scroll_massage, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_massage), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    box_mesage_horizontal_part = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    box_get_message = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box_mesage_horizontal_part), box_get_message, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(box_mesage_horizontal_part), box_show_message, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(scroll_massage), 10);
    gtk_container_add(GTK_CONTAINER(scroll_massage), box_mesage_horizontal_part);
    
    GtkWidget* box_sent_message = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,3);
    sent_message = gtk_text_view_new();
    g_signal_connect(sent_message, "key_press_event", G_CALLBACK(on_key_release), NULL);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(sent_message));
    const gchar *text = "Write text here...";
    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_widget_set_name(sent_message, "sent_message_inactive");
    gtk_widget_set_name(box_sent_message, "box_sent_message");

    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(sent_message), 25);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(sent_message), 5);
    
    GtkWidget* scroll_sent_mess = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_sent_mess), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_container_set_border_width(GTK_CONTAINER(scroll_sent_mess), 10);
    gtk_container_add(GTK_CONTAINER(scroll_sent_mess), sent_message);
    gtk_widget_set_size_request(scroll_sent_mess,-1,73);
    
    gtk_widget_set_size_request(sent_message, -1, 60);
    gtk_entry_set_placeholder_text(GTK_ENTRY(sent_message),"Write message xxyita polnaya");
    gtk_box_pack_start(GTK_BOX(box_sent_message), scroll_sent_mess, TRUE, TRUE, 0);
    button_send = gtk_button_new_with_label("Send");
    gtk_widget_set_margin_top(button_send, 8);
    gtk_widget_set_margin_bottom(button_send, 7);
    GtkWidget* button_add = gtk_button_new_with_label("Add");
    gtk_widget_set_margin_top(button_add, 8);
    gtk_widget_set_margin_bottom(button_add, 7);
    gtk_widget_set_margin_end(button_add, 3);
    g_signal_connect(button_add,"clicked",G_CALLBACK(add),NULL);

    gtk_widget_set_size_request(button_add, 60, 60);
  g_signal_connect(button_send,"clicked",G_CALLBACK(send_clnts_msg_to_server),rabotyaga);
    
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(sent_message), GTK_WRAP_WORD);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(sent_message), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(sent_message), TRUE);
    
    g_signal_connect(G_OBJECT(sent_message), "focus-in-event", G_CALLBACK(on_sent_message_focus_in), NULL);
        g_signal_connect(G_OBJECT(sent_message), "focus-out-event", G_CALLBACK(on_sent_message_focus_out), NULL);
    g_signal_connect(sent_message, "key_press_event", G_CALLBACK(on_textview_key_press), NULL);


    gtk_box_pack_end(GTK_BOX(box_sent_message), button_send, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box_sent_message), button_add, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(interface_box2), box_sent_message, FALSE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(interface_window), interface_planed);
    gtk_widget_show_all(interface_window);
    
    return 0;
}

gboolean sent(gpointer data){
    GtkWidget* set_text = gtk_label_new("");
    GtkWidget* set_text2 = gtk_label_new("");
    gtk_container_foreach(GTK_CONTAINER(box_show_message), destroy_widget, NULL);
    gtk_container_foreach(GTK_CONTAINER(box_get_message), destroy_widget, NULL);
    t_chat_msg* chat_msgs = (t_chat_msg*)data;
    t_chat_msg* chat_msg;
    int i = 0;
    int arr_size = 0;
    while(chat_msgs && chat_msgs[i].time && chat_msgs[i].msg_text){
        arr_size++;
        i++;
    }
    for(int j = arr_size - 1; j >= 0; j--){
        int mnozh = 13, add = 5, photo_h = 78, interval = 9;
        chat_msg = &chat_msgs[j];
        GtkWidget* box_image = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        GtkWidget* box_image2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        message_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *avatar_image = gtk_image_new_from_file(RESOURCES_LOCATION \
                                                          "user.png");
        GtkWidget *avatar_image2 = gtk_image_new_from_file(RESOURCES_LOCATION \
                                                                 "user.png");
        GtkWidget *timestamp_label;
        GtkWidget *timestamp_label2;
        GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        
        char* temp = chat_msg->msg_text;
        if(temp == NULL)
            set_text = gtk_label_new(" ");
        else
            set_text = gtk_label_new(temp);
        set_text2 = gtk_label_new(temp);
        gtk_widget_set_name(set_text, chat_msg->msg_text);
        gtk_widget_set_name(message_box, mx_itoa(chat_msg->msg_id));
        GtkStyleContext *context = gtk_widget_get_style_context(message_box);
        gtk_style_context_add_class(context, "message-box");
        time_t now = time(0);
        char *temp1 = ctime(&now);
        char *time = mx_strndup(temp1, mx_strlen(temp1) - 1);
        if(mx_strncmp(time, chat_msg->time, 10) == 0){
            char* temp2 = mx_strndup(&chat_msg->time[10], 6);
            char *time_to_show = mx_strjoin("Today", temp2);
            timestamp_label = gtk_label_new(time_to_show);
            timestamp_label2 = gtk_label_new(time_to_show);
            mx_strdel(&time_to_show);
            mx_strdel(&temp2);
        }
        else{
            timestamp_label = gtk_label_new(chat_msg->time);
            timestamp_label2 = gtk_label_new(chat_msg->time);
        }
        gtk_widget_set_name(timestamp_label, chat_msg->msg_text);
        
        gtk_widget_set_margin_top(GTK_WIDGET(timestamp_label), 8);
        gtk_widget_set_margin_top(GTK_WIDGET(avatar_image), 8);
        gtk_widget_set_margin_top(GTK_WIDGET(avatar_image2), 8);
        gtk_widget_set_valign(avatar_image, GTK_ALIGN_END);
        gtk_widget_set_valign(avatar_image2, GTK_ALIGN_END);
        gtk_widget_set_valign(timestamp_label, GTK_ALIGN_END);
        gtk_widget_set_valign(timestamp_label2, GTK_ALIGN_END);
        gtk_widget_set_valign(set_text, GTK_ALIGN_END);
        gtk_widget_set_valign(set_text2, GTK_ALIGN_END);
        gtk_label_set_selectable(GTK_LABEL(set_text), TRUE);
        gtk_label_set_use_markup(GTK_LABEL(set_text), TRUE);
        gtk_widget_set_margin_bottom(GTK_WIDGET(set_text), 10);
        gtk_label_set_line_wrap(GTK_LABEL(set_text), TRUE);
        gtk_label_set_max_width_chars(GTK_LABEL(set_text), 40);
        gtk_label_set_use_markup(GTK_LABEL(set_text2), TRUE);
        gtk_widget_set_margin_bottom(GTK_WIDGET(set_text2), 10);
        gtk_label_set_line_wrap(GTK_LABEL(set_text2), TRUE);
        gtk_label_set_max_width_chars(GTK_LABEL(set_text2), 40);
        g_signal_connect(set_text, "button_press_event", G_CALLBACK(on_label_button_press_event),message_box);
        GtkWidget* box_new = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        if(chat_msg->sender_id != current_user){
            GdkPixbuf* image_chosen = gdk_pixbuf_new_from_file_at_scale(mx_strjoin(RESOURCES_LOCATION,chat_msg->filename) , 150, 150, FALSE, NULL);
            GtkWidget *image_send = gtk_image_new_from_pixbuf(image_chosen);
            GtkWidget *image_send2 = gtk_image_new_from_pixbuf(image_chosen);
            if(chat_msg->filename != NULL){
                gtk_widget_set_margin_top(GTK_WIDGET(timestamp_label), 170);
                gtk_widget_set_margin_top(GTK_WIDGET(timestamp_label2), 170);
                gtk_widget_set_margin_top(GTK_WIDGET(avatar_image), 155);
                gtk_widget_set_margin_top(GTK_WIDGET(avatar_image2), 155);
                gtk_box_pack_start(GTK_BOX(box_image), image_send, FALSE, FALSE, 0);
                gtk_box_pack_start(GTK_BOX(box_image2), image_send2, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_image), set_text, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_image2), set_text, FALSE, FALSE, 0);
                gtk_widget_set_margin_top(GTK_WIDGET(box_image), 10);
                gtk_widget_set_margin_end(GTK_WIDGET(box_image), 10);
                gtk_widget_set_margin_top(GTK_WIDGET(box_image2), 10);
                gtk_widget_set_margin_end(GTK_WIDGET(box_image2), 10);
                gtk_box_pack_end(GTK_BOX(message_box), box_image, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_new), box_image2, FALSE, FALSE, 0);
            }
            else{
                gtk_box_pack_end(GTK_BOX(message_box), set_text, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_new), set_text2, FALSE, FALSE, 0);
                gtk_widget_set_halign(set_text, GTK_ALIGN_END);
                gtk_widget_set_halign(set_text2, GTK_ALIGN_END);
            }
            gtk_widget_set_margin_end(GTK_WIDGET(set_text), 10);
            gtk_widget_set_margin_end(GTK_WIDGET(set_text2), 10);
            gtk_widget_set_halign(timestamp_label, GTK_ALIGN_START);
            gtk_widget_set_halign(timestamp_label2, GTK_ALIGN_START);
            gtk_widget_set_margin_top(GTK_WIDGET(set_text), 8);
            gtk_widget_set_margin_top(GTK_WIDGET(set_text2), 8);
            gtk_box_pack_start(GTK_BOX(message_box), avatar_image, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(message_box), timestamp_label, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box_new), avatar_image2, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box_new), timestamp_label2, FALSE, FALSE, 0);
            gtk_box_pack_end(GTK_BOX(box_get_message), message_box, FALSE, FALSE, 5);
            gtk_box_pack_end(GTK_BOX(box_show_message), box_new, FALSE, FALSE, 5);
            gtk_widget_set_opacity(box_new, 0.0);
            gtk_widget_show_all(box_get_message);
            }
            else{
                GdkPixbuf* image_chosen = gdk_pixbuf_new_from_file_at_scale(mx_strjoin(RESOURCES_LOCATION,chat_msg->filename) , 150, 150, FALSE, NULL);
                GtkWidget *image_send = gtk_image_new_from_pixbuf(image_chosen);
                GtkWidget *image_send2 = gtk_image_new_from_pixbuf(image_chosen);
                if(chat_msg->filename != NULL){
                    gtk_widget_set_margin_top(GTK_WIDGET(timestamp_label), 170);
                    gtk_widget_set_margin_top(GTK_WIDGET(timestamp_label2), 170);
                    gtk_widget_set_margin_top(GTK_WIDGET(avatar_image), 155);
                    gtk_widget_set_margin_top(GTK_WIDGET(avatar_image2), 155);
                    gtk_box_pack_start(GTK_BOX(box_image), image_send, FALSE, FALSE, 0);
                    gtk_box_pack_end(GTK_BOX(box_image), set_text, FALSE, FALSE, 0);
                    gtk_widget_set_margin_top(GTK_WIDGET(box_image), 10);
                    gtk_widget_set_margin_start(GTK_WIDGET(box_image), 10);
                    gtk_box_pack_start(GTK_BOX(box_image2), image_send2, FALSE, FALSE, 0);
                    gtk_box_pack_end(GTK_BOX(box_image2), set_text2, FALSE, FALSE, 0);
                    gtk_widget_set_margin_top(GTK_WIDGET(box_image2), 10);
                    gtk_widget_set_margin_start(GTK_WIDGET(box_image2), 10);
                    gtk_box_pack_start(GTK_BOX(message_box), box_image, FALSE, FALSE, 0);
                    gtk_box_pack_start(GTK_BOX(box_new), box_image2, FALSE, FALSE, 0);
                }
                else{
                    gtk_widget_set_halign(set_text, GTK_ALIGN_START);
                    gtk_widget_set_halign(set_text2, GTK_ALIGN_START);
                    gtk_box_pack_start(GTK_BOX(message_box), set_text, FALSE, FALSE, 0);
                    gtk_box_pack_start(GTK_BOX(box_new), set_text2, FALSE, FALSE, 0);
                }
                gtk_widget_set_margin_start(GTK_WIDGET(set_text), 10);
                gtk_widget_set_halign(timestamp_label, GTK_ALIGN_END);
                gtk_widget_set_margin_start(GTK_WIDGET(set_text2), 10);
                gtk_widget_set_halign(timestamp_label2, GTK_ALIGN_END);
                gtk_widget_set_margin_top(GTK_WIDGET(set_text), 8);
                gtk_widget_set_margin_top(GTK_WIDGET(set_text2), 8);
                gtk_box_pack_end(GTK_BOX(message_box), avatar_image, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(message_box), timestamp_label, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_new), avatar_image2, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_new), timestamp_label2, FALSE, FALSE, 0);
                gtk_box_pack_end(GTK_BOX(box_get_message), box_new, FALSE, FALSE, 5);
                gtk_box_pack_end(GTK_BOX(box_show_message), message_box, FALSE, FALSE, 5);
                gtk_widget_set_opacity(box_new, 0.0);
                gtk_widget_show_all(box_show_message);
            }
        mx_strdel(&time);
    }
    //free(chat_msgs);
    gdk_threads_add_idle((GSourceFunc)set_scrollbar_position_to_last_message, box_mesage_horizontal_part);
    return FALSE;
}

gboolean on_label_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == GDK_BUTTON_SECONDARY) {
        GtkWidget *menu = gtk_menu_new();
        GtkStyleContext *context = gtk_widget_get_style_context(menu);
        gtk_style_context_add_class(context, "menu");
        t_msg_to_edit *msg_to_edit = malloc(sizeof(t_msg_to_edit));
        msg_to_edit->box = GTK_WIDGET(data);
        msg_to_edit->label = widget;

        GtkWidget *edit_item = gtk_menu_item_new_with_label("Edit");
        g_signal_connect(edit_item, "activate", G_CALLBACK(on_edit_activate), msg_to_edit->box);
        g_signal_connect(edit_item, "activate", G_CALLBACK(gtk_widget_destroy), menu);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), edit_item);
        GtkWidget *copy_item = gtk_menu_item_new_with_label("Copy");
        g_signal_connect(copy_item, "activate", G_CALLBACK(on_copy_activate), msg_to_edit);
        g_signal_connect(copy_item, "activate", G_CALLBACK(gtk_widget_destroy), menu);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), copy_item);
        GtkWidget *delete_item = gtk_menu_item_new_with_label("Delete");
        g_signal_connect(delete_item, "activate", G_CALLBACK(on_delete_activate), msg_to_edit->box);
        g_signal_connect(delete_item, "activate", G_CALLBACK(gtk_widget_destroy), menu);

        gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete_item);
        gtk_container_add(GTK_CONTAINER(box_mesage_horizontal_part), menu);
        gtk_widget_show_all(menu);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*) event);
        return TRUE;
    }
    return FALSE;
}

void on_copy_activate(GtkWidget *widget, gpointer user_data) {
        GtkBox *label = NULL;
        GtkWidget* pizda = GTK_WIDGET(((t_msg_to_edit *)user_data)->box);
        GList *children = gtk_container_get_children(GTK_CONTAINER(pizda));
        GList *iter = children;
        for (; iter != NULL; iter = g_list_next(iter)) {
            GtkWidget *child = GTK_WIDGET(iter->data);
            if (GTK_IS_LABEL(child)) {
                label = GTK_LABEL(child);
                break;
            }
        }
        gchar *text = gtk_widget_get_name(GTK_WIDGET(label));
        GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        gtk_clipboard_set_text(clipboard, text, mx_strlen(text));
        g_list_free(children);
   
}
void on_edit_activate(GtkWidget *widget, gpointer user_data) {
    GtkWidget *label = GTK_WIDGET(user_data);
    gchar *text = gtk_label_get_text(label);
   GtkWidget *dialog = gtk_dialog_new_with_buttons("EDIT", GTK_WINDOW(gtk_widget_get_toplevel(interface_window)), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "Save", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "message-box");
    
    GtkWidget *entry = gtk_entry_new();
    set_entry_style(entry);
    gtk_entry_set_text(GTK_ENTRY(entry), text);
    gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 40);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);
    gtk_widget_show(entry);
    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT){
        int msg_id = mx_atoi(gtk_widget_get_name(GTK_WIDGET(user_data)));
        char *new_text = (char *)gtk_entry_get_text(GTK_ENTRY(entry));
        if(mx_strtrim(new_text) != NULL){
            send_message(rabotyaga->sock, edit_msg(new_text, msg_id));
        }
    }
    gtk_widget_destroy(dialog);
    
}
void on_delete_activate(GtkWidget *widget, gpointer user_data){
    int msg_id = mx_atoi(gtk_widget_get_name(GTK_WIDGET(user_data)));
    send_message(rabotyaga->sock, delete_msg(msg_id, rabotyaga->user_id));
    GtkWidget *label = GTK_WIDGET(user_data);
    gtk_widget_destroy(GTK_WIDGET(label));
}



void add_new_dialog(GtkButton* button,gpointer data) {
    dialog_add = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(dialog_add), 400, 400);
    gtk_window_set_decorated(GTK_WINDOW(dialog_add),FALSE);
    dialog_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    button_box_dialog = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    button_cancel = gtk_button_new_with_label("Cancel");
    gtk_box_pack_end(GTK_BOX(button_box_dialog), button_cancel, FALSE, FALSE, 0);
    box_search = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    search_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_user),"Search ...");
    GtkStyleContext *context = gtk_widget_get_style_context(search_user);
    gtk_style_context_add_class(context, "text_entry");
    char* find_name = (char *)gtk_entry_get_text(GTK_ENTRY(search_user));
    //
    gtk_widget_set_margin_top(search_user, 40);
    gtk_widget_set_margin_start(search_user, 20);
    gtk_widget_set_margin_end(search_user, 20);
    //
    scroll_search = gtk_scrolled_window_new(NULL, NULL);

    gtk_box_pack_start(GTK_BOX(dialog_vbox), box_search, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_search), search_user, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(dialog_vbox), button_box_dialog, FALSE, FALSE, 0);
    //
    box_show = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(scroll_search), box_show);
    gtk_container_set_border_width(GTK_CONTAINER(scroll_search), 10);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), scroll_search, TRUE, TRUE, 0);
    //
    g_signal_connect(search_user,"changed",G_CALLBACK(search_request), data);
g_signal_connect_swapped(button_cancel,"clicked",G_CALLBACK(gtk_widget_destroy),dialog_add);
    gtk_window_set_position(GTK_WINDOW(dialog_add),GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_add),GTK_WINDOW(interface_window));
    gtk_container_add(GTK_CONTAINER(dialog_add), dialog_vbox);
    gtk_widget_show_all(dialog_add);
}

void add_new_group(void) {
    dialog_group = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(dialog_group), 400, 400);
    gtk_window_set_decorated(GTK_WINDOW(dialog_group),FALSE);
    group_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    button_box_group = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    button_cancel_group = gtk_button_new_with_label("Cancel");
    gtk_box_pack_end(GTK_BOX(button_box_group), button_cancel_group, FALSE, FALSE, 0);
    box_search_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    search_group = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_group),"Search group...");
    
    gtk_widget_set_margin_top(search_group, 40);
    gtk_widget_set_margin_start(search_group, 20);
    gtk_widget_set_margin_end(search_group, 20);
    
    scroll_search_group = gtk_scrolled_window_new(NULL, NULL);
    box_show_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_box_pack_start(GTK_BOX(group_box), box_search_group, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_search_group), search_group, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(group_box), button_box_group, FALSE, FALSE, 0);
    
    gtk_container_set_border_width(GTK_CONTAINER(scroll_search_group), 10);
    gtk_container_add(GTK_CONTAINER(scroll_search_group), box_show_group);
    gtk_box_pack_start(GTK_BOX(group_box), scroll_search_group, TRUE, TRUE, 0);
    
    g_signal_connect(search_group,"activate",G_CALLBACK(show_group_what_found),NULL);
    
    gtk_window_set_position(GTK_WINDOW(dialog_group),GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_group),GTK_WINDOW(interface_window));
    g_signal_connect_swapped(button_cancel_group,"clicked",G_CALLBACK(gtk_widget_destroy),dialog_group);
    gtk_container_add(GTK_CONTAINER(dialog_group), group_box);
    gtk_widget_show_all(dialog_group);
}

void show_group_what_found(void) {
    find_name = (char *)gtk_entry_get_text(GTK_ENTRY(search_group));
    GtkWidget* fixed_search = gtk_fixed_new();
    gtk_widget_set_margin_start(fixed_search, 15);
    button_search_group = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_search_group),find_name);
    gtk_widget_set_size_request(button_search_group, 350, 60);
    gtk_fixed_put(GTK_FIXED(fixed_search),button_search_group,0,0);
    gtk_box_pack_start(GTK_BOX(box_show_group), fixed_search, FALSE, FALSE, 5);
    g_signal_connect(button_search_group,"clicked",G_CALLBACK(add_new_to_box_message),NULL);
    g_signal_connect_swapped(button_search_group,"clicked",G_CALLBACK(gtk_widget_destroy),dialog_add);
    gtk_widget_show_all(scroll_search_group);
}
gboolean set_new_profile_photo(GtkButton *b, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    
    dialog = gtk_file_chooser_dialog_new ("Open File",
                                              GTK_WINDOW(gtk_widget_get_toplevel(interface_window)),
                                          action,
                                          "_Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "_Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    set_style_file_inspector(dialog);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        t_sock *sock = (t_sock*)data;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        GFile *file = gtk_file_chooser_get_file(chooser);
        char *path = g_file_get_path(file);
        profile_photo = create_round_image(path, 95);
        gtk_fixed_put(GTK_FIXED(profile_fixed),profile_photo,390,7);
        GFileInfo* info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
        gint64 size = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
        request_photo_change(g_file_get_basename(file),sock, (unsigned long)size);
        send_file(sock, path,(unsigned long)size);
        //g_free (path);
        //g_free (file);
    }
    
    gtk_widget_destroy (dialog);
    gtk_widget_show_all(window_profile);
    return FALSE;
}
    
gboolean change_profile( gpointer data) {
    t_user_info* user_info = (t_user_info *)data;
    window_profile = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_profile), "profile");
    gtk_window_set_default_size(GTK_WINDOW(window_profile), 500, 370);
    profile_fixed = gtk_fixed_new();
    button_close = gtk_button_new_with_label("Close");
    button_save = gtk_button_new_with_label("Save details");
    button_delete = gtk_button_new_with_label("Delete account");
    gtk_widget_set_size_request(button_save, 166, 50);
    gtk_widget_set_size_request(button_close, 166, 50);
    gtk_widget_set_size_request(button_delete, 166, 50);
    button_change_photo = gtk_button_new_with_label("Change photo");
    gtk_widget_set_size_request(button_change_photo, 100, 60);
    GtkLabel* label1 = gtk_label_new("Change theme");
    button_change_thema = gtk_switch_new();
    gtk_widget_set_size_request(button_change_thema, 100, 40);
    entry_status = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_status),user_info->username);
    entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name),"PASSWORD");
    set_entry_style(entry_name);
    set_entry_style(entry_status);
    
    gtk_widget_set_size_request(entry_status, 350, 60);
    gtk_widget_set_size_request(entry_name, 280, 60);
    
    profile_photo = create_round_image(mx_strjoin(RESOURCES_LOCATION,user_info->photo_path), 95);
    gtk_fixed_put(GTK_FIXED(profile_fixed),button_save,0,320);
    gtk_fixed_put(GTK_FIXED(profile_fixed),button_close,166,320);
    gtk_fixed_put(GTK_FIXED(profile_fixed),button_delete,333,320);
    gtk_fixed_put(GTK_FIXED(profile_fixed),label1,378,180);
    gtk_fixed_put(GTK_FIXED(profile_fixed),button_change_thema,378,195);
    gtk_fixed_put(GTK_FIXED(profile_fixed),profile_photo,390,7);
    gtk_fixed_put(GTK_FIXED(profile_fixed),button_change_photo,378,108);
    gtk_fixed_put(GTK_FIXED(profile_fixed),entry_name,19,102);
    gtk_fixed_put(GTK_FIXED(profile_fixed),entry_status,19,13);
    gtk_switch_set_active(button_change_thema, read_switch_state());

    g_signal_connect(button_change_thema, "notify::active", G_CALLBACK(on_button_clicked), NULL);
       g_signal_connect(button_change_thema, "state-set", G_CALLBACK(on_button_clicked), NULL);
       g_signal_connect_swapped(button_close,"clicked",G_CALLBACK(gtk_widget_destroy),window_profile);
       g_signal_connect(button_save,"clicked",G_CALLBACK(request_editing), user_info->sock);
       g_signal_connect_swapped(button_save,"clicked",G_CALLBACK(gtk_widget_destroy), window_profile);
       g_signal_connect(button_delete,"clicked",G_CALLBACK(request_delete_acc), user_info->sock);
       g_signal_connect_swapped(button_delete,"clicked",G_CALLBACK(on_window_destroy), window_profile);
       g_signal_connect(button_delete,"clicked",G_CALLBACK(log_in_table), user_info->sock);
       g_signal_connect(button_change_photo,"clicked", G_CALLBACK(set_new_profile_photo),user_info->sock);
    gtk_container_add(GTK_CONTAINER(window_profile), profile_fixed);
    gtk_widget_show_all(window_profile);
    return FALSE;
}

void on_window_destroy(GtkWidget *widget, gpointer data) {
  save_switch_state(gtk_switch_get_active(GTK_SWITCH(button_change_thema)));
    exit(0);
    
}

gboolean show_user_what_found(gpointer data){
    t_user_info* user_info = (t_user_info *)data;
    button_search_name = gtk_button_new();
    fixed_search = gtk_fixed_new();
    gtk_button_set_label(GTK_BUTTON(button_search_name),user_info->username);
    gtk_widget_set_name(GTK_WIDGET(button_search_name), mx_itoa(user_info->user_id));
    gtk_widget_set_size_request(button_search_name, 350, 60);
    gtk_fixed_put(GTK_FIXED(fixed_search),button_search_name,0,0);
    gtk_box_pack_start(GTK_BOX(box_show),fixed_search, TRUE, TRUE, 5);
    gtk_window_set_position(GTK_WINDOW(dialog_add),GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_add),GTK_WINDOW(interface_window));
    gtk_widget_set_margin_start(fixed_search, 15);
    g_signal_connect(button_search_name,"clicked",G_CALLBACK(add_request), user_info);
    gtk_widget_show_all(box_show);
    return FALSE;
}

gboolean add_new_to_box_message(gpointer data){
        rabotyaga = (t_rabotyaga_reciepient *)malloc(sizeof(t_rabotyaga_reciepient));
        rabotyaga->sock = ((t_user_info *)data)->sock;
    GtkWidget* button_user_name = gtk_button_new();
    box_new_chat = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
       gtk_widget_set_name(box_new_chat, "box_new_chat");
    gtk_button_set_label(GTK_BUTTON(button_user_name),((t_user_info *)data)->username);
    gtk_widget_set_name(button_user_name, mx_itoa(((t_user_info *)data)->user_id));
    gtk_widget_set_name(box_new_chat, mx_itoa(((t_user_info *)data)->user_id));
GtkWidget* user_photo = create_round_image(mx_strjoin(RESOURCES_LOCATION,((t_user_info *)data)->photo_path), 60);
    gtk_widget_set_size_request(button_user_name, 310, 60);
      gtk_style_context_add_class(gtk_widget_get_style_context(button_user_name), "button_user_name");
    gtk_box_pack_start(GTK_BOX(box_new_chat),user_photo, FALSE, FALSE, 0);
      gtk_box_pack_start(GTK_BOX(box_new_chat),button_user_name, TRUE, TRUE, 0);
    gtk_widget_hide(dialog_add);
      gtk_box_pack_start(GTK_BOX(box_user),box_new_chat, FALSE, FALSE, 2);
    g_signal_connect(button_user_name,"clicked",G_CALLBACK(get_dialog),rabotyaga);
g_signal_connect(button_user_name,"clicked", G_CALLBACK(request_msg_history),rabotyaga);
gtk_widget_show_all(scroll_user);
           return FALSE;
}

gboolean send_photo(GtkButton* b,gpointer data) {
    char *caption = (char*)gtk_entry_get_text(GTK_ENTRY(entry_caption));
    send_message(rabotyaga->sock, create_file_warning(rabotyaga->filename, rabotyaga->user_id, rabotyaga->filesize, caption));
    send_file(rabotyaga->sock, rabotyaga->file_path, rabotyaga->filesize);
    g_free(rabotyaga->filename);
    g_free(rabotyaga->file_path);
    return FALSE;
}

gboolean add(gpointer data) {
    rabotyaga->file_path = NULL;
    rabotyaga->filename = NULL;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new ("Open Photo",
                                              GTK_WINDOW(gtk_widget_get_toplevel(interface_window)),
                                          action,
                                          "_Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "_Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    set_style_file_inspector(dialog);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_mime_type(filter, "image/jpeg");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        GFile *file = gtk_file_chooser_get_file(chooser);
        char *filename = g_file_get_basename(file);
        char *path = g_file_get_path(file);
        rabotyaga->filename = filename;
        rabotyaga->file_path = path;
        GFileInfo* info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
        gint64 size = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
        rabotyaga->filesize = (unsigned long)size;
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Selected Photo");
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);
        
        GtkWidget *box_choosen_image = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
        GtkWidget *box_name_image = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        GdkPixbuf* image_chosen = gdk_pixbuf_new_from_file_at_scale(path, 150, 150, FALSE, NULL);
        GtkWidget *image = gtk_image_new_from_pixbuf(image_chosen);
        GtkWidget *label = gtk_label_new(filename);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_set_valign(label, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(box_name_image), image, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box_name_image), label, TRUE, TRUE, 0);
        GtkWidget* box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        GtkWidget* button_send = gtk_button_new_with_label("Send");
        GtkWidget* button_cancel = gtk_button_new_with_label("Cancel");
        gtk_box_pack_end(GTK_BOX(box_button), button_send, FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(box_button), button_cancel, FALSE, FALSE, 0);
        
        GtkWidget* box_entry = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
        GtkWidget* label_caption = gtk_label_new("Caption");
        gtk_widget_set_margin_bottom(label_caption, 5);
        gtk_widget_set_halign(label_caption, GTK_ALIGN_START);
        gtk_widget_set_valign(label_caption, GTK_ALIGN_START);
        entry_caption = gtk_entry_new();
        gtk_entry_set_max_length(GTK_ENTRY(entry_caption), 25);
        set_entry_style(entry_caption);
        gtk_box_pack_start(GTK_BOX(box_entry), label_caption, FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(box_entry), entry_caption, FALSE, FALSE, 0);
        
        gtk_widget_set_margin_top(box_entry, 15);
        gtk_widget_set_margin_top(box_button, 10);
        
        
        gtk_box_pack_end(GTK_BOX(box_choosen_image), box_button, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box_choosen_image), box_name_image, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box_choosen_image), box_entry, FALSE, FALSE, 0);
        gtk_container_add(GTK_CONTAINER(window), box_choosen_image);
        
        g_signal_connect_swapped(button_cancel,"clicked",G_CALLBACK(gtk_widget_destroy),window);
        g_signal_connect(button_send,"clicked",G_CALLBACK(send_photo),NULL);
        g_signal_connect_swapped(button_send,"clicked",G_CALLBACK(gtk_widget_destroy),window);
        gtk_widget_show_all(window);
        
    }
    gtk_widget_destroy (dialog);
    return FALSE;
}

gboolean info_about_user(gpointer data){
    gtk_widget_destroy(window_info_about_user);
    window_info_about_user = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(window_info_about_user),GTK_WINDOW(interface_window));
    gtk_window_set_position(GTK_WINDOW(window_info_about_user), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_default_size(GTK_WINDOW(window_info_about_user), 600,800);
    gtk_window_set_decorated(GTK_WINDOW(window_info_about_user),FALSE);
    
    GtkWidget* box_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget* box_name_close = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_widget_set_name(box_name_close, "box_name_close");

GtkWidget* label_user_info = gtk_label_new("Chat info");
    gtk_label_set_xalign(GTK_LABEL(label_user_info), 0.1);
    GtkWidget* button_close = gtk_button_new();
    gtk_box_pack_start(GTK_BOX(box_name_close), label_user_info, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_name_close), button_close, FALSE, FALSE, 0);
    gtk_button_set_label(button_close, "X");
    gtk_widget_set_size_request(button_close,60,-1);
    gtk_widget_set_margin_bottom(button_close, 15);
    gtk_box_pack_start(GTK_BOX(box_info), box_name_close, FALSE, FALSE, 0);
    g_signal_connect_swapped(button_close,"clicked",G_CALLBACK(gtk_widget_destroy),window_info_about_user);
    
    GtkWidget* box_about = gtk_box_new(GTK_ORIENTATION_VERTICAL, 7);
    GtkWidget* scroll_info = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scroll_info), 3);
    gtk_box_pack_end(GTK_BOX(box_info), scroll_info, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(scroll_info), box_about);
    GtkWidget* fixed_info_about_user = gtk_fixed_new();
    GtkLabel* label_name = GTK_LABEL(gtk_label_new(gtk_button_get_label(GTK_BUTTON(button_dialog_name))));
    gtk_widget_set_name(label_name, "user_name_chat_menu");
    gtk_widget_set_size_request(GTK_WIDGET(label_name), 350, 80);
    GtkLabel* label_status = GTK_LABEL(gtk_label_new("user_status"));
    gtk_widget_set_size_request(GTK_WIDGET(label_status), 350, 70);
    gtk_fixed_put(GTK_FIXED(fixed_info_about_user),GTK_WIDGET(label_name),150,0);
    user_photo = GTK_IMAGE(create_round_image(mx_strjoin(RESOURCES_LOCATION, rabotyaga->profile_img), 150));
    gtk_fixed_put(GTK_FIXED(fixed_info_about_user),GTK_WIDGET(user_photo),15,0);
    gtk_label_set_xalign(GTK_LABEL(label_name), 0.25);
    gtk_label_set_xalign(GTK_LABEL(label_status), 0.15);
    gtk_box_pack_start(GTK_BOX(box_about), fixed_info_about_user, FALSE, FALSE, 0);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
       gtk_widget_set_hexpand(box, TRUE);
       gtk_widget_set_margin_top(box, 25);
       gtk_widget_set_margin_bottom(box, 25);
       gtk_widget_set_size_request(box, -1, 70);
    GtkWidget* fixed_message = gtk_fixed_new();
    
    GtkWidget* label_message = gtk_button_new_with_label(mx_strjoin("MESSAGES: ",mx_itoa(rabotyaga->num_msgs)));
    gtk_widget_set_size_request(label_message, 590, 50);

gtk_label_set_xalign(GTK_LABEL(label_message), 0.15);
    gtk_widget_set_margin_top(label_message, 25);

    gtk_fixed_put(GTK_FIXED(fixed_message),label_message,0,0);
    gtk_box_pack_start(GTK_BOX(box_about), fixed_message, FALSE, FALSE, 0);
    //
    GtkWidget* fixed_photo = gtk_fixed_new();
    GtkWidget* label_photo = gtk_button_new_with_label(mx_strjoin("PHOTOS: ",mx_itoa(rabotyaga->count_files)));
    gtk_widget_set_size_request(label_photo, 590, 50);
    gtk_label_set_xalign(GTK_LABEL(label_photo), 0.15);
    gtk_fixed_put(GTK_FIXED(fixed_photo),label_photo,0,0);
    gtk_box_pack_start(GTK_BOX(box_about), fixed_photo, FALSE, TRUE, 0);
    GtkWidget* fixed_audio_file = gtk_fixed_new();
    GtkWidget* button_audio_file = gtk_button_new_with_label("Delete chat");
    gtk_widget_set_size_request(button_audio_file, 590, 50);
    gtk_fixed_put(GTK_FIXED(fixed_audio_file),button_audio_file,0,0);
    gtk_box_pack_start(GTK_BOX(box_about), fixed_audio_file, FALSE, FALSE, 0);
    GtkWidget* fixed_voice_messages = gtk_fixed_new();
    GtkWidget* button_voice_messages = gtk_button_new_with_label("Delete chat history");
    gtk_widget_set_size_request(button_voice_messages, 590, 50);
    gtk_fixed_put(GTK_FIXED(fixed_voice_messages),button_voice_messages,0,0);
    gtk_box_pack_start(GTK_BOX(box_about), fixed_voice_messages, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window_info_about_user), box_info);
    g_signal_connect(button_audio_file, "clicked",G_CALLBACK(delete_chat),data);
    g_signal_connect(button_voice_messages, "clicked",G_CALLBACK(clear_history_chat),data);
    gtk_widget_show_all(window_info_about_user);
    return FALSE;
}

void on_switch_toggled(GtkSwitch *switch_button, gpointer user_data) {
    gboolean state = gtk_switch_get_active(switch_button);
}
