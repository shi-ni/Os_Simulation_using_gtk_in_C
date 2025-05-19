#include <gtk/gtk.h>
#include <string.h>

// Function to reverse the string
void reverse_string(GtkWidget *widget, gpointer data) {
    GtkWidget **widgets = (GtkWidget **)data;
    GtkEntry *entry = GTK_ENTRY(widgets[0]);
    GtkLabel *result_label = GTK_LABEL(widgets[1]);
    
    const gchar *text = gtk_entry_get_text(entry);
    gint length = strlen(text);
    
    // Create a reversed string
    gchar *reversed = g_malloc(length + 1);
    for (gint i = 0; i < length; i++) {
        reversed[i] = text[length - 1 - i] - 4;
    }
    reversed[length] = '\0';
    
    // Display the result
    gtk_label_set_text(result_label, reversed);
    g_free(reversed);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Password Decryption");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create widgets
    GtkWidget *label = gtk_label_new("Enter a Password to Decrypt:");
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Decrypt");
    GtkWidget *result_label = gtk_label_new("");
    
    // Set up widgets
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Type something...");
    gtk_label_set_selectable(GTK_LABEL(result_label), TRUE);
    
    // Pack widgets into container
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 5);
    
    // Connect button click signal
    GtkWidget *widgets[2] = {entry, result_label};
    g_signal_connect(button, "clicked", G_CALLBACK(reverse_string), widgets);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}