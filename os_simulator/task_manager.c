// #include <gtk/gtk.h>
// #include "include/os_state.h"
// #include <pthread.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <string.h>

// GtkListStore *store;

// // Thread-safe function to clear the store
// gboolean clear_store_idle(gpointer data) {
//     gtk_list_store_clear(store);
//     return G_SOURCE_REMOVE;
// }

// // Thread-safe function to insert a task into the GTK list store
// gboolean insert_task_idle(gpointer user_data) {
//     Task *task = (Task *)user_data;

//     gtk_list_store_insert_with_values(store, NULL, -1,
//         0, task->name,
//         1, task->id,
//         2, task->ram_usage,
//         3, task->hdd_usage,
//         4, task->is_running ? "Running" : "Stopped",
//         -1);

//     free(task);
//     return G_SOURCE_REMOVE;
// }

// void *listen_for_tasks(void *arg) {
//     char buffer[512];

//     while (1) {
//         FILE *pipe = fopen(PIPE_PATH, "r");
//         if (!pipe) {
//             sleep(1);
//             continue;
//         }

//         g_idle_add(clear_store_idle, NULL); // Clear GTK list store safely

//         while (fgets(buffer, sizeof(buffer), pipe)) {
//             Task *task = malloc(sizeof(Task));
//             if (!task)
//              continue;

//             // Fill only required fields from the pipe data
//             sscanf(buffer, "%[^,],%d,%d,%d,%d",
//                    task->name, &task->id, &task->ram_usage,
//                    &task->hdd_usage, &task->is_running);

//             g_idle_add(insert_task_idle, task); // Insert row into GTK safely
//         }

//         fclose(pipe);
//         sleep(1);
//     }

//     return NULL;
// }

// void show_task_manager(GtkWidget *widget, gpointer data) {
//     GtkWidget *window, *scrolled_window, *treeview;
//     GtkCellRenderer *renderer;
//     GtkTreeViewColumn *column;

//     pthread_t thread_id;

//     store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);

//     pthread_create(&thread_id, NULL, listen_for_tasks, NULL);

//     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_title(GTK_WINDOW(window), "Task Manager");
//     gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

//     scrolled_window = gtk_scrolled_window_new(NULL, NULL);
//     gtk_container_add(GTK_CONTAINER(window), scrolled_window);

//     treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
//     gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);

//     // Task Name
//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Task", renderer, "text", 0, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     // PID
//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("PID", renderer, "text", 1, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     // RAM Usage
//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("RAM (MB)", renderer, "text", 2, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     // HDD Usage
//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("HDD (MB)", renderer, "text", 3, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     // Status
//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Status", renderer, "text", 4, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     gtk_widget_show_all(window);
// }

// int main(int argc, char *argv[]) {
//     gtk_init(&argc, &argv);
//     show_task_manager(NULL, NULL);
//     gtk_main();
//     return 0;
// }
