#include <gtk/gtk.h>

void on_clicked(GtkWidget *widget, gpointer entry) {
    GtkEntry *text_entry = GTK_ENTRY(entry);
    const gchar *text = gtk_entry_get_text(text_entry);
    
    FILE *file = fopen("notes.txt", "w");
    if (!file) {
        g_print("Error opening file!\n");
        return;
    }
    
    fprintf(file, "%s", text);
    fclose(file);
    g_print("Text saved to file.\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *label, *entry, *box, *btn;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_title(GTK_WINDOW(window), "Notepad");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create and pack label
    label = gtk_label_new("Type your text here:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Create and pack entry (text input)
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // Create and pack button
    btn = gtk_button_new_with_label("Save to File");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_clicked), entry);  // Pass entry directly
    gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);

    // Show everything
    gtk_widget_show_all(window);

    // Enter GTK main loop
    gtk_main();

    return 0;
}