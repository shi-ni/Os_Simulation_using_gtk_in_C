#include <gtk/gtk.h>
#include <stdbool.h>

// Game state
int min_range = 1;
int max_range = 30;
int current_guess = 15; // Start with middle of full range
bool is_higher_phase = true;

// Widgets
GtkWidget *question_label;
GtkWidget *higher_button;
GtkWidget *lower_button;
GtkWidget *correct_button;
GtkWidget *restart_button;

void update_question() {
    char text[100];
    if (is_higher_phase) {
        snprintf(text, sizeof(text), "Is your number between 15 and 30?");
    } else {
        snprintf(text, sizeof(text), "Is your number %d?", current_guess);
    }
    gtk_label_set_text(GTK_LABEL(question_label), text);
}

void make_guess() {
    current_guess = (min_range + max_range) / 2;
    update_question();
}

void reset_game() {
    min_range = 1;
    max_range = 30;
    current_guess = 15;
    is_higher_phase = true;
    
    gtk_widget_set_sensitive(higher_button, TRUE);
    gtk_widget_set_sensitive(lower_button, TRUE);
    gtk_widget_set_sensitive(correct_button, TRUE);
    
    update_question();
}

void on_higher(GtkWidget *widget, gpointer data) {
    if (is_higher_phase) {
        min_range = 16; // Narrow to upper half
        is_higher_phase = false;
    } else {
        min_range = current_guess + 1;
    }
    
    if (min_range > max_range) {
        gtk_label_set_text(GTK_LABEL(question_label), "That's not possible! Let's try again.");
        reset_game();
        return;
    }
    
    make_guess();
}

void on_lower(GtkWidget *widget, gpointer data) {
    if (is_higher_phase) {
        max_range = 15; // Narrow to lower half
        is_higher_phase = false;
    } else {
        max_range = current_guess - 1;
    }
    
    if (min_range > max_range) {
        gtk_label_set_text(GTK_LABEL(question_label), "That's not possible! Let's try again.");
        reset_game();
        return;
    }
    
    make_guess();
}

void on_correct(GtkWidget *widget, gpointer data) {
    char text[100];
    snprintf(text, sizeof(text), "I guessed it! Your number is %d.", current_guess);
    gtk_label_set_text(GTK_LABEL(question_label), text);
    
    gtk_widget_set_sensitive(higher_button, FALSE);
    gtk_widget_set_sensitive(lower_button, FALSE);
    gtk_widget_set_sensitive(correct_button, FALSE);
}

void on_restart(GtkWidget *widget, gpointer data) {
    reset_game();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Number Guessing Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create question label
    question_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), question_label, TRUE, TRUE, 5);
    
    // Create buttons container
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, TRUE, TRUE, 5);
    
    // Create action buttons
    higher_button = gtk_button_new_with_label("Higher");
    g_signal_connect(higher_button, "clicked", G_CALLBACK(on_higher), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), higher_button);
    
    lower_button = gtk_button_new_with_label("Lower");
    g_signal_connect(lower_button, "clicked", G_CALLBACK(on_lower), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), lower_button);
    
    correct_button = gtk_button_new_with_label("Correct");
    g_signal_connect(correct_button, "clicked", G_CALLBACK(on_correct), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), correct_button);
    
    // Create restart button
    restart_button = gtk_button_new_with_label("New Game");
    g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), restart_button, TRUE, TRUE, 5);
    
    // Initialize game
    reset_game();
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}