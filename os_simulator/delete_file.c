#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>

static void delete_file(GtkEntry *entry, gpointer window) {
    const gchar *filename = gtk_entry_get_text(entry);
    
    if (filename[0] == '\0') {
        return;  // Don't try to delete if empty
    }

    if (unlink(filename) == 0) {
        // Success - show message and clear field
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                 GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_INFO,
                                                 GTK_BUTTONS_OK,
                                                 "Deleted file: %s",
                                                 filename);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_entry_set_text(entry, "");
    } else {
        // Error
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                 GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Failed to delete %s\n%s",
                                                 filename,
                                                 strerror(errno));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Delete File");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *label = gtk_label_new("Filename to delete:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter filename and press Enter");
    g_signal_connect(entry, "activate", G_CALLBACK(delete_file), window);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.deletefile", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}