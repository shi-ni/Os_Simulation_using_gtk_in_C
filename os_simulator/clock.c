//clock.c
#include <gtk/gtk.h>
#include <time.h>

GtkLabel *clock_label;

gboolean update_clock(gpointer data) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char time_str[64];
    strftime(time_str, sizeof(time_str), "Time: %H:%M:%S", t);
    gtk_label_set_text(clock_label, time_str);

    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Clock");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    clock_label = GTK_LABEL(gtk_label_new("Time: --:--:--"));
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(clock_label));
    g_timeout_add_seconds(1, update_clock, NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
