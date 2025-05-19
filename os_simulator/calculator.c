#include <gtk/gtk.h>
#include <stdlib.h>

GtkWidget *entry;
double first_number = 0;
char operation = 0;

void on_number_click(GtkWidget *widget, gpointer data) {
    const char *current = gtk_entry_get_text(GTK_ENTRY(entry));
    const char *digit = (const char *)data;

    char new_text[256];
    snprintf(new_text, sizeof(new_text), "%s%s", current, digit);
    gtk_entry_set_text(GTK_ENTRY(entry), new_text);
}

void on_operation_click(GtkWidget *widget, gpointer data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    first_number = atof(text);
    operation = *(char *)data;
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void on_equals_click(GtkWidget *widget, gpointer data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    double second_number = atof(text);
    double result = 0;

    if (operation == '+') {
        result = first_number + second_number;
    } else if (operation == '-') {
        result = first_number - second_number;
    }

    char result_text[64];
    snprintf(result_text, sizeof(result_text), "%.2f", result);
    gtk_entry_set_text(GTK_ENTRY(entry), result_text);
}

void on_clear_click(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    first_number = 0;
    operation = 0;
}

GtkWidget* create_button(const char *label, GCallback callback, gpointer data) {
    GtkWidget *button = gtk_button_new_with_label(label);
    g_signal_connect(button, "clicked", callback, data);
    return button;
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid;
    printf("Calculator running\n");
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Number buttons
    const char *digits[] = {"7", "8", "9", "4", "5", "6", "1", "2", "3", "0"};
    int pos = 0;
    for (int row = 1; row <= 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            GtkWidget *btn = create_button(digits[pos], G_CALLBACK(on_number_click), (gpointer)digits[pos]);
            gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
            pos++;
        }
    }
    GtkWidget *zero_btn = create_button("0", G_CALLBACK(on_number_click), (gpointer)"0");
    gtk_grid_attach(GTK_GRID(grid), zero_btn, 1, 4, 1, 1);

    // Operation buttons
    GtkWidget *add_btn = create_button("+", G_CALLBACK(on_operation_click), (gpointer)"+");
    GtkWidget *sub_btn = create_button("-", G_CALLBACK(on_operation_click), (gpointer)"-");
    GtkWidget *eq_btn = create_button("=", G_CALLBACK(on_equals_click), NULL);
    GtkWidget *clr_btn = create_button("C", G_CALLBACK(on_clear_click), NULL);

    gtk_grid_attach(GTK_GRID(grid), add_btn, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), sub_btn, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), eq_btn, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), clr_btn, 3, 4, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();
        printf("Calculator exiting\n");
    return 0;
}
