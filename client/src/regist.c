#include "../inc/client.h"

static void entry_visibility(GtkButton *b) {
    if (gtk_entry_get_visibility(GTK_ENTRY(registration_password)) && gtk_entry_get_visibility(GTK_ENTRY(registration_confirm))){
        gtk_entry_set_visibility(GTK_ENTRY(registration_password), FALSE);
        gtk_entry_set_visibility(GTK_ENTRY(registration_confirm), FALSE);

    }

    else {
        gtk_entry_set_visibility(GTK_ENTRY(registration_password), TRUE);
        gtk_entry_set_visibility(GTK_ENTRY(registration_confirm), TRUE);

    }
}

bool is_pass_secure(char* pass){
    int len = mx_strlen(pass);
    if(len < 4)
        return false;
    int num = 0;
    int low = 0;
    int upper = 0;
    for(int i = 0; i < len; i++){
        if(mx_isdigit(pass[i])){
            num++;
        }
        else if(mx_islower(pass[i])){
            low++;
        }
        else if (mx_isupper(pass[i])){
            upper++;
        }
    }
    if(low != 0 && upper != 0 && num != 0)
        return true;
    else
        return false;
}

void send_reg(GtkWidget *w,t_sock* sock){
        
     char* l_reg = (char *)gtk_entry_get_text(GTK_ENTRY(registration_login));
    char* p_reg = (char *)gtk_entry_get_text(GTK_ENTRY(registration_password));
    char* p_confirm = (char *)gtk_entry_get_text(GTK_ENTRY(registration_confirm));
   // if(mx_strlen(l_reg) >= 4 && mx_strcmp(p_reg, p_confirm) == 0 && is_pass_secure(p_reg) == true){
       // is_pass_secure(p_reg);
        send_message(sock, create_reg(l_reg, p_reg));
   // }
   /* else if(mx_strlen(l_reg) < 4){
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_login),"Login is too short");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_password),"");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_confirm),"");
    }
    else if(is_pass_secure(p_reg) == false){
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_login),"");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_password),"The password is not secure");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_confirm),"The password is not secure");
    }
    else if(mx_strcmp(p_reg, p_confirm) == 0 ){
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_login),"");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_password),"");
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_confirm),"The password is incorrect");
    }*/
   

}
void regist(GtkWidget* w, t_sock* sock){
    
   registration_window = gtk_application_window_new(app);
   gtk_window_set_title(GTK_WINDOW(registration_window), "Sign up on uChaiber");
   gtk_window_set_default_size(GTK_WINDOW(registration_window), 500, 410);
   
   registrate_fixed = gtk_fixed_new();
   registration_login = gtk_entry_new();
    
   gtk_entry_set_placeholder_text(GTK_ENTRY(registration_login),"Login:");
    GtkStyleContext *context = gtk_widget_get_style_context(registration_login);
    gtk_style_context_add_class(context, "text_entry");
    registration_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(registration_password),"Password");
    context = gtk_widget_get_style_context(registration_password);
    gtk_style_context_add_class(context, "text_entry");
    gtk_entry_set_visibility(GTK_ENTRY(registration_password), FALSE);
    gtk_fixed_put(GTK_FIXED(registrate_fixed),registration_login,90,45);
    gtk_widget_set_size_request(registration_login, 320, 70);
    gtk_fixed_put(GTK_FIXED(registrate_fixed),registration_password,90,125);
    gtk_widget_set_size_request(registration_password, 320, 70);
    registration_confirm = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(registration_confirm),"Confirm password:");
    context = gtk_widget_get_style_context(registration_confirm);
    gtk_style_context_add_class(context, "text_entry");
        gtk_entry_set_visibility(GTK_ENTRY(registration_confirm), FALSE);
        
        gtk_fixed_put(GTK_FIXED(registrate_fixed),registration_confirm,90,210);
        gtk_widget_set_size_request(registration_confirm, 320, 70);
    
    reg = gtk_button_new_with_label("Registration");

    g_signal_connect(reg,"clicked",G_CALLBACK(send_reg),sock);
    
    gtk_fixed_put(GTK_FIXED(registrate_fixed),reg,130,290);
    gtk_widget_set_size_request(reg, 80, 80);
    GtkImage* login = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                        "user.png"));
    GtkImage* password = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                           "password.png"));
    GtkImage* password_confirm = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                                   "password.png"));
    gtk_fixed_put(GTK_FIXED(registrate_fixed),GTK_WIDGET(login),33,65);
    gtk_fixed_put(GTK_FIXED(registrate_fixed),GTK_WIDGET(password),33,148);
    gtk_fixed_put(GTK_FIXED(registrate_fixed),GTK_WIDGET(password_confirm),33,226);
    
    GtkImage* eye = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                      "eye.png"));
    show_password_regisrtation = gtk_button_new();
     gtk_button_set_image(GTK_BUTTON(show_password_regisrtation),GTK_WIDGET(eye));
    gtk_fixed_put(GTK_FIXED(registrate_fixed),show_password_regisrtation,415,130);
    gtk_widget_set_size_request(show_password_regisrtation, 60, 60);
   gtk_container_add(GTK_CONTAINER(registration_window),registrate_fixed);
    g_signal_connect(show_password_regisrtation,"clicked",G_CALLBACK(entry_visibility),NULL);
   gtk_widget_show_all(registration_window);
}
