#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to generate Fibonacci series
void generate_fibonacci(GtkWidget *widget, gpointer data) {
    GtkWidget **widgets = (GtkWidget **)data;
    GtkEntry *entry = GTK_ENTRY(widgets[0]);
    GtkLabel *result_label = GTK_LABEL(widgets[1]);
    
    const gchar *text = gtk_entry_get_text(entry);
    int limit = atoi(text);
    
    // Validate input
    if (limit < 0) {
        gtk_label_set_text(result_label, "Error: Please enter positive number");
        return;
    }
    if (limit > 1000) { // Prevent very large outputs
        gtk_label_set_text(result_label, "Error: Number too large (max 1000)");
        return;
    }
    
    // Generate Fibonacci series
    char result[5000] = "Fibonacci series: ";
    if (limit == 0) {
        strcat(result, "0");
    } else {
        int a = 0, b = 1, c;
        strcat(result, "0, 1");
        
        while (1) {
            c = a + b;
            if (c > limit) break;
            
            char num_str[20];
            snprintf(num_str, sizeof(num_str), ", %d", c);
            strcat(result, num_str);
            
            a = b;
            b = c;
        }
    }
    
    // Display result
    gtk_label_set_text(result_label, result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Fibonacci Series Generator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create widgets
    GtkWidget *label = gtk_label_new("Enter upper limit for Fibonacci series:");
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Generate");
    GtkWidget *result_label = gtk_label_new("");
    
    // Set up widgets
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "e.g., 100");
    gtk_label_set_selectable(GTK_LABEL(result_label), TRUE);
    gtk_label_set_line_wrap(GTK_LABEL(result_label), TRUE);
    
    // Pack widgets into container
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 5);
    
    // Connect button click signal
    GtkWidget *widgets[2] = {entry, result_label};
    g_signal_connect(button, "clicked", G_CALLBACK(generate_fibonacci), widgets);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}