#include "../inc/client.h"

gboolean exit_from_uchat(gpointer data){
    exit(0);
}

gboolean window_reconnect(gpointer data) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    GtkWidget* label = gtk_label_new("uChaiber kaput!");
    GtkWidget* button_reconnect = gtk_button_new_with_label("Close");
    g_signal_connect(button_reconnect, "clicked", G_CALLBACK(exit_from_uchat), NULL);
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_set_size_request(button_reconnect, -1, 50);
    gtk_widget_set_margin_bottom(button_reconnect, 30);
    gtk_widget_set_margin_start(button_reconnect, 50);
    gtk_widget_set_margin_end(button_reconnect, 50);
    gtk_box_pack_end(GTK_BOX(box), button_reconnect, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
    return FALSE;
}
