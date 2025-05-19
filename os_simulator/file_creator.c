#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Function to handle file creation
static void create_file(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    gint response;

    // Create a dialog window
    dialog = gtk_dialog_new_with_buttons("Create New File",
                                       GTK_WINDOW(data),
                                       GTK_DIALOG_MODAL,
                                       "_OK",
                                       GTK_RESPONSE_ACCEPT,
                                       "_Cancel",
                                       GTK_RESPONSE_REJECT,
                                       NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Add a label and entry for filename input
    GtkWidget *label = gtk_label_new("Enter filename:");
    entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        const gchar *filename = gtk_entry_get_text(GTK_ENTRY(entry));

        // Create the file
        int fd = open(filename, O_CREAT | O_WRONLY, 0644);
        if (fd == -1)
        {
            GtkWidget *error_dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                            GTK_DIALOG_MODAL,
                                                            GTK_MESSAGE_ERROR,
                                                            GTK_BUTTONS_OK,
                                                            "Failed to create file: %s",
                                                            filename);
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
        else
        {
            close(fd);

            // Show success message
            GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                              GTK_DIALOG_MODAL,
                                                              GTK_MESSAGE_INFO,
                                                              GTK_BUTTONS_OK,
                                                              "File created successfully: %s",
                                                              filename);
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        }
    }

    gtk_widget_destroy(dialog);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;

    // Create the main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "File Creator");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    // Create a box to hold the button
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create the button
    button = gtk_button_new_with_label("Create New File");
    g_signal_connect(button, "clicked", G_CALLBACK(create_file), window);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // Create the application with the updated flag
    app = gtk_application_new("org.example.filecreator", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}