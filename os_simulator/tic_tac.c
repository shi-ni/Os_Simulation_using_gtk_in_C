#include <gtk/gtk.h>
#include <stdbool.h>

// Game state
char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
bool is_x_turn = true;

// Function to check for a winner
char check_winner() {
    // Check rows
    for (int i = 0; i < 9; i += 3) {
        if (board[i] != ' ' && board[i] == board[i+1] && board[i] == board[i+2]) {
            return board[i];
        }
    }
    
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] != ' ' && board[i] == board[i+3] && board[i] == board[i+6]) {
            return board[i];
        }
    }
    
    // Check diagonals
    if (board[0] != ' ' && board[0] == board[4] && board[0] == board[8]) {
        return board[0];
    }
    if (board[2] != ' ' && board[2] == board[4] && board[2] == board[6]) {
        return board[2];
    }
    
    // Check for tie
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            return ' '; // Game not finished
        }
    }
    
    return 'T'; // Tie
}

// Function to reset the game
void reset_game(GtkWidget *widget, gpointer data) {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
    is_x_turn = true;
    
    GtkWidget **buttons = (GtkWidget **)data;
    for (int i = 0; i < 9; i++) {
        gtk_button_set_label(GTK_BUTTON(buttons[i]), " ");
    }
}

// Callback function for button clicks
void button_clicked(GtkWidget *widget, gpointer data) {
    int position = GPOINTER_TO_INT(data);
    
    // If the position is already taken or game is over, do nothing
    if (board[position] != ' ' || check_winner() != ' ') {
        return;
    }
    
    // Mark the position
    if (is_x_turn) {
        board[position] = 'X';
        gtk_button_set_label(GTK_BUTTON(widget), "X");
    } else {
        board[position] = 'O';
        gtk_button_set_label(GTK_BUTTON(widget), "O");
    }
    
    is_x_turn = !is_x_turn;
    
    // Check for winner
    char winner = check_winner();
    if (winner != ' ') {
        GtkWidget *dialog;
        if (winner == 'T') {
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, 
                                          GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                                          "It's a tie!");
        } else {
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, 
                                          GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                                          "%c wins!", winner);
        }
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create grid for buttons
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    // Create buttons
    GtkWidget *buttons[9];
    for (int i = 0; i < 9; i++) {
        buttons[i] = gtk_button_new_with_label(" ");
        gtk_widget_set_size_request(buttons[i], 80, 80);
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i));
        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 3, i / 3, 1, 1);
    }
    
    // Create reset button
    GtkWidget *reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_game), buttons);
    gtk_grid_attach(GTK_GRID(grid), reset_button, 0, 3, 3, 1);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the GTK main loop
    gtk_main();
    
    return 0;
}