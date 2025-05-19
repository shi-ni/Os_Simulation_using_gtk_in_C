#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static void move_file(GtkButton *button, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    GtkWidget *source_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(window), "source_entry"));
    GtkWidget *dest_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(window), "dest_entry"));
    
    const gchar *source = gtk_entry_get_text(GTK_ENTRY(source_entry));
    const gchar *destination = gtk_entry_get_text(GTK_ENTRY(dest_entry));

    if (source[0] == '\0' || destination[0] == '\0') {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_OK,
                                                "Both fields must be filled");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (rename(source, destination) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Moved '%s' to '%s'",
                                                source, destination);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        // Clear entries
        gtk_entry_set_text(GTK_ENTRY(source_entry), "");
        gtk_entry_set_text(GTK_ENTRY(dest_entry), "");
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_OK,
                                                "Error moving file: %s",
                                                strerror(errno));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "File Mover");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Source file entry
    GtkWidget *source_label = gtk_label_new("Source File:");
    gtk_grid_attach(GTK_GRID(grid), source_label, 0, 0, 1, 1);

    GtkWidget *source_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(source_entry), "/path/to/source/file");
    gtk_grid_attach(GTK_GRID(grid), source_entry, 1, 0, 1, 1);

    // Destination entry
    GtkWidget *dest_label = gtk_label_new("Destination:");
    gtk_grid_attach(GTK_GRID(grid), dest_label, 0, 1, 1, 1);

    GtkWidget *dest_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(dest_entry), "/path/to/destination");
    gtk_grid_attach(GTK_GRID(grid), dest_entry, 1, 1, 1, 1);

    // Move button
    GtkWidget *move_button = gtk_button_new_with_label("Move File");
    g_signal_connect(move_button, "clicked", G_CALLBACK(move_file), window);
    gtk_grid_attach(GTK_GRID(grid), move_button, 0, 2, 2, 1);

    // Store entries in window data for access in callback
    g_object_set_data(G_OBJECT(window), "source_entry", source_entry);
    g_object_set_data(G_OBJECT(window), "dest_entry", dest_entry);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.filemover", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}