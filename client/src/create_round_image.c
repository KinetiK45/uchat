#include "../inc/client.h"

GtkWidget* create_round_image(const gchar* filename, gint size){
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, size, size, FALSE, NULL);
    if (!pixbuf) {
        return NULL;
    }
    GtkWidget* image_widget = gtk_drawing_area_new();
    gtk_widget_set_size_request(image_widget, size, size);
    gtk_widget_set_app_paintable(image_widget, TRUE);

    g_signal_connect(image_widget, "draw", G_CALLBACK(on_draw_event), pixbuf);

    return image_widget;
}

gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data)
{
    GdkPixbuf* pixbuf = (GdkPixbuf*)user_data;
    gint size = MIN(gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size, size);
    cairo_t* cr2 = cairo_create(surface);

    cairo_arc(cr2, size/2.0, size/2.0, size/2.0, 0, 2 * G_PI);
    cairo_clip(cr2);

    GdkPixbuf* scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, size, size, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr2, scaled_pixbuf, 0, 0);
    cairo_paint(cr2);

    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    g_object_unref(scaled_pixbuf);

    cairo_destroy(cr2);
    cairo_surface_destroy(surface);

    return FALSE;
}
