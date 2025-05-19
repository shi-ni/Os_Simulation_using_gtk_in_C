#include <gtk/gtk.h>
#include <unistd.h>

// Global variable for the window
static GtkWidget *window = NULL;

// Function to play beep and close
static gboolean play_beep_and_close(gpointer data) {
    // Play system beep sound
    gdk_beep();
    
    // Close the window after 5 seconds
    g_timeout_add_seconds(5, (GSourceFunc)gtk_window_close, window);
    
    return FALSE; // Only run once
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Beep Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    
    // Add a label
    GtkWidget *label = gtk_label_new("Playing beep for 5 seconds...");
    gtk_container_add(GTK_CONTAINER(window), label);
    
    // Connect the close signal
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Show the window
    gtk_widget_show_all(window);
    
    // Start beep and timer when window is shown
    g_timeout_add(100, play_beep_and_close, NULL);
    
    // Run main loop
    gtk_main();
    
    return 0;
}