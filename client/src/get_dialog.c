#include "../inc/client.h"

void get_dialog(GtkButton *button, gpointer data){
    rabotyaga->user_id = mx_atoi(gtk_widget_get_name(GTK_WIDGET(button)));
    gtk_button_set_label(GTK_BUTTON(button_dialog_name),gtk_button_get_label(GTK_BUTTON(button)));
}

