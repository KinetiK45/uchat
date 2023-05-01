#include "../inc/client.h"


int init_client(int argc, char **argv, t_sock* sock) {
    
    int status;
    current_user = -1;
      app = gtk_application_new ("org.gtk.uChaiber", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (log_in_table), sock);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);
  return status;
}
