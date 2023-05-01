#include "../inc/client.h"

GFile* css_fp;
GError* error;
GtkCssProvider* provider;
GdkDisplay* display;
GdkScreen* screen;

gboolean on_button_clicked(GtkSwitch *switch_button, gpointer user_data) {
  gboolean state = gtk_switch_get_active(switch_button);
  GtkCssProvider *css_provider = gtk_css_provider_new();
  const char* css_style_file;
  if(state == FALSE) {
    css_style_file = RESOURCES_LOCATION \
    "light.css";
  }
  else {
    css_style_file = RESOURCES_LOCATION \
      "dark.css";
  }
  save_switch_state(state);

  gtk_css_provider_load_from_path(css_provider, css_style_file, NULL);
  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_css_provider_load_from_file(provider, g_file_new_for_path(css_style_file), &error);
  g_object_unref(css_provider);
    return FALSE;
}

void load_css(void) {
  GError* error = NULL;
  const char* css_style_file;
    
  gboolean state = read_switch_state();

  if(state == FALSE) {
    css_style_file = RESOURCES_LOCATION \
      "light.css";
  }
  else {
    css_style_file = RESOURCES_LOCATION \
      "dark.css";
  }

  provider = gtk_css_provider_new();
  display = gdk_display_get_default();
  if (display == NULL) {
    return;
  }
  screen = gdk_display_get_default_screen(display);
  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_css_provider_load_from_file(provider, g_file_new_for_path(css_style_file), &error);
  g_object_unref(provider);
}

void save_switch_state(gboolean state) {
    const char* CONFIG_FILE_PATH = RESOURCES_LOCATION "save";

    int fd = open(CONFIG_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        return;
    }
    write(fd, &state, sizeof(bool));
    close(fd);
}

gboolean read_switch_state(void) {
    const char* CONFIG_FILE_PATH = RESOURCES_LOCATION "save";
    gboolean state = FALSE;
    int fd = open(CONFIG_FILE_PATH, O_RDONLY);
    if (fd < 0) {
        return state;
    }
    bool read_state = FALSE;
    if (read(fd, &read_state, sizeof(bool)) == sizeof(bool)) {
        state = read_state;
    }
    close(fd);

    return state;
}
