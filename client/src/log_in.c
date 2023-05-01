#include "../inc/client.h"

static void entry_visibility_login(GtkButton *b) {
    if (gtk_entry_get_visibility(GTK_ENTRY(p_entry))){
        gtk_entry_set_visibility(GTK_ENTRY(p_entry), FALSE);
    }

    else {
        gtk_entry_set_visibility(GTK_ENTRY(p_entry), TRUE);
    }
}

void log_in(GtkWidget *w,t_sock* sock)
 {
 
    char* l_data = (char *)gtk_entry_get_text(GTK_ENTRY(l_entry));
    char* p_data = (char *)gtk_entry_get_text(GTK_ENTRY(p_entry));
    send_message(sock, create_login(l_data, p_data));
    gtk_label_set_text(GTK_LABEL(NULL),l_data);
    gtk_label_set_text(GTK_LABEL(NULL),p_data);
    gtk_entry_set_text(GTK_ENTRY(l_entry),"");
    gtk_entry_set_text(GTK_ENTRY(p_entry),"");
 }

 void log_in_table (GtkApplication* app, t_sock* sock) {
     
      load_css();

     window = gtk_application_window_new (app);
     gtk_window_set_title (GTK_WINDOW(window), "Sign in to uChaiber");
     gtk_window_set_default_size (GTK_WINDOW (window), 500, 370);

    l_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(l_entry),"Login:");
     GtkStyleContext *context = gtk_widget_get_style_context(l_entry);
     gtk_style_context_add_class(context, "text_entry");
   
    p_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(p_entry),"Password");
    gtk_entry_set_visibility(GTK_ENTRY(p_entry), FALSE);
     context = gtk_widget_get_style_context(p_entry);
     gtk_style_context_add_class(context, "text_entry");
    GtkImage* eye = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                      "eye.png"));
    show_password = gtk_button_new();
     gtk_button_set_image(GTK_BUTTON(show_password),GTK_WIDGET(eye));
    signUp = gtk_button_new_with_label("Sign Up");
    registration = gtk_button_new_with_label("Regist");
     
     GtkImage* login = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                         "user.png"));
     GtkImage* password = GTK_IMAGE(gtk_image_new_from_file(RESOURCES_LOCATION \
                                                            "password.png"));

    g_signal_connect(registration,"clicked",G_CALLBACK(regist),sock);
    g_signal_connect(signUp,"clicked",G_CALLBACK(log_in), sock);
    
     l_fixed = gtk_fixed_new();
     gtk_fixed_put(GTK_FIXED(l_fixed),GTK_WIDGET(login),33,65);
     gtk_fixed_put(GTK_FIXED(l_fixed),GTK_WIDGET(password),33,178);
     gtk_widget_set_size_request(l_entry, 320, 70);
     gtk_fixed_put(GTK_FIXED(l_fixed),l_entry,90,46);
     gtk_widget_set_size_request(p_entry, 320, 70);
     gtk_fixed_put(GTK_FIXED(l_fixed),p_entry,90,159);
     gtk_widget_set_size_request(signUp, 80, 80);
     gtk_fixed_put(GTK_FIXED(l_fixed),show_password,415,164);
     gtk_widget_set_size_request(show_password, 60, 60);
     gtk_fixed_put(GTK_FIXED(l_fixed),signUp,300,255);
     gtk_fixed_put(GTK_FIXED(l_fixed),registration,122,255);
     gtk_widget_set_size_request(registration, 80, 80);

    g_signal_connect_swapped (registration, "clicked", G_CALLBACK (gtk_widget_destroy), window);
     g_signal_connect(show_password,"clicked",G_CALLBACK(entry_visibility_login),NULL);
     
     
     gtk_container_add(GTK_CONTAINER(window),l_fixed);
     gtk_widget_show_all (window);
   
 }
