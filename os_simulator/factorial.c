#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// Function to calculate factorial
void calculate_factorial(GtkWidget *widget, gpointer data) {
    GtkWidget **widgets = (GtkWidget **)data;
    GtkEntry *entry = GTK_ENTRY(widgets[0]);
    GtkLabel *result_label = GTK_LABEL(widgets[1]);
    
    const gchar *text = gtk_entry_get_text(entry);
    int n = atoi(text);
    
    // Validate input
    if (n < 0) {
        gtk_label_set_text(result_label, "Error: Negative number!");
        return;
    }
    if (n > 20) { // 21! overflows 64-bit integer
        gtk_label_set_text(result_label, "Error: Number too large!");
        return;
    }
    
    // Calculate factorial
    unsigned long long factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial *= i;
    }
    
    // Display result
    char result[50];
    snprintf(result, sizeof(result), "%d! = %llu", n, factorial);
    gtk_label_set_text(result_label, result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Factorial Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create widgets
    GtkWidget *label = gtk_label_new("Enter a number (0-20):");
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Calculate");
    GtkWidget *result_label = gtk_label_new("");
    
    // Set up widgets
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "e.g., 5");
    gtk_label_set_selectable(GTK_LABEL(result_label), TRUE);
    
    // Pack widgets into container
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 5);
    
    // Connect button click signal
    GtkWidget *widgets[2] = {entry, result_label};
    g_signal_connect(button, "clicked", G_CALLBACK(calculate_factorial), widgets);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}