#include <gtk/gtk.h>
#include <stdio.h>
#include <glib/gstdio.h>

// Function to copy file
void copy_file(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    GtkWidget *status_label = entries[2];
    
    const gchar *source = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *destination = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    
    if (strlen(source) == 0 || strlen(destination) == 0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Please enter both filenames");
        return;
    }
    
    FILE *src, *dst;
    char ch;
    
    src = fopen(source, "rb");
    if (src == NULL) {
        gtk_label_set_text(GTK_LABEL(status_label), "Error: Cannot open source file");
        return;
    }
    
    dst = fopen(destination, "wb");
    if (dst == NULL) {
        fclose(src);
        gtk_label_set_text(GTK_LABEL(status_label), "Error: Cannot create destination file");
        return;
    }
    
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }
    
    fclose(src);
    fclose(dst);
    
    gtk_label_set_text(GTK_LABEL(status_label), "File copied successfully!");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Copier");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create entry fields
    GtkWidget *entries[3]; // source, destination, status_label
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 5);
    
    // Source file entry
    GtkWidget *source_label = gtk_label_new("Source File:");
    gtk_grid_attach(GTK_GRID(grid), source_label, 0, 0, 1, 1);
    
    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter source filename");
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);
    
    // Destination file entry
    GtkWidget *dest_label = gtk_label_new("Destination File:");
    gtk_grid_attach(GTK_GRID(grid), dest_label, 0, 1, 1, 1);
    
    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter destination filename");
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);
    
    // Copy button
    GtkWidget *copy_button = gtk_button_new_with_label("Copy File");
    g_signal_connect(copy_button, "clicked", G_CALLBACK(copy_file), entries);
    gtk_box_pack_start(GTK_BOX(vbox), copy_button, FALSE, FALSE, 5);
    
    // Status label
    entries[2] = gtk_label_new("");
    gtk_label_set_selectable(GTK_LABEL(entries[2]), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), entries[2], FALSE, FALSE, 5);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}