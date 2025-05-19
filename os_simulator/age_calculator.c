#include <gtk/gtk.h>
#include <time.h>
#include <string.h>

// Function to calculate age
void calculate_age(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    GtkWidget *result_label = entries[3];
    
    // Get day, month, year from entries
    const gchar *day_text = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *month_text = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *year_text = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    
    // Convert to integers
    int day = atoi(day_text);
    int month = atoi(month_text);
    int year = atoi(year_text);
    
    // Get current date
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_day = current_time->tm_mday;
    int current_month = current_time->tm_mon + 1; // tm_mon is 0-11
    int current_year = current_time->tm_year + 1900; // tm_year is years since 1900
    
    // Calculate age
    int age = current_year - year;
    
    // Adjust if birthday hasn't occurred yet this year
    if (current_month < month || (current_month == month && current_day < day)) {
        age--;
    }
    
    // Prepare result text
    char result[100];
    if (day <= 0 || month <= 0 || year <= 0 || month > 12 || day > 31 ||
        (month == 2 && day > 29) || 
        ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)) {
        snprintf(result, sizeof(result), "Invalid date entered!");
    } else {
        snprintf(result, sizeof(result), "You are %d years old.", age);
    }
    
    gtk_label_set_text(GTK_LABEL(result_label), result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Age Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create title label
    GtkWidget *title_label = gtk_label_new("Enter your date of birth:");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 5);
    
    // Create entry fields
    GtkWidget *entries[4]; // day, month, year, result_label
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 5);
    
    // Day entry
    GtkWidget *day_label = gtk_label_new("Day:");
    gtk_grid_attach(GTK_GRID(grid), day_label, 0, 0, 1, 1);
    
    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "DD");
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);
    
    // Month entry
    GtkWidget *month_label = gtk_label_new("Month:");
    gtk_grid_attach(GTK_GRID(grid), month_label, 0, 1, 1, 1);
    
    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "MM");
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);
    
    // Year entry
    GtkWidget *year_label = gtk_label_new("Year:");
    gtk_grid_attach(GTK_GRID(grid), year_label, 0, 2, 1, 1);
    
    entries[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "YYYY");
    gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);
    
    // Calculate button
    GtkWidget *calculate_button = gtk_button_new_with_label("Calculate Age");
    g_signal_connect(calculate_button, "clicked", G_CALLBACK(calculate_age), entries);
    gtk_box_pack_start(GTK_BOX(vbox), calculate_button, FALSE, FALSE, 5);
    
    // Result label
    entries[3] = gtk_label_new("");
    gtk_label_set_selectable(GTK_LABEL(entries[3]), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), entries[3], FALSE, FALSE, 5);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}