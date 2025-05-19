#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calendar");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    const char *calendar_text =
        "    May 2025\n"
        "Su Mo Tu We Th Fr Sa\n"
        "             1  2  3\n"
        " 4  5  6  7  8  9 10\n"
        "11 12 13 14 15 16 17\n"
        "18 19 20 21 22 23 24\n"
        "25 26 27 28 29 30 31\n";

    GtkWidget *label = gtk_label_new(calendar_text);
    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
