// desktop.c
#include <gtk/gtk.h>
#include <stdio.h>
#include "include/os_state.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void launch_desktop();
void prompt_kernel_password(GtkWidget *widget, gpointer window);
void terminate_all_tasks(GtkWidget *widget, gpointer data);
void update_resource_display();
void launch_kernel_ui();

GtkLabel *clock_label;
GtkLabel *calendar_label;
extern OSState system_state;

gboolean update_clock(gpointer data)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char time_str[64];
    strftime(time_str, sizeof(time_str), "Time: %H:%M:%S", t);
    gtk_label_set_text(clock_label, time_str);

    return TRUE;
}

void update_calendar()
{
    const char *calendar_text =
        "    May 2025\n"
        "Su Mo Tu We Th Fr Sa\n"
        "             1  2  3\n"
        " 4  5  6  7  8  9 10\n"
        "11 12 13 14 15 16 17\n"
        "18 19 20 21 22 23 24\n"
        "25 26 27 28 29 30 31\n";
    gtk_label_set_text(calendar_label, calendar_text);
}


void add_to_ready_queue(Task *task, const char *exec_path)
{
    QueueType queue_type;

    // Determine queue type based on task characteristics
    if (strcmp(task->name, "Task Manager") == 0)
    {
        queue_type = SYSTEM;
        task->priority = 0; // Highest priority for system tasks
    }
    else if (strcmp(task->name, "Calculator") == 0 ||
             strcmp(task->name, "Notepad") == 0)
    {
        queue_type = INTERACTIVE;
        task->priority = 1;
    }
    else
    {
        queue_type = BATCH;
        task->priority = 2;
    }

    // Store the executable path in the task
    snprintf(task->exec_path, sizeof(task->exec_path), "%s", exec_path);

    // Add to appropriate queue
    if (system_state.queues[queue_type].count < MAX_TASKS)
    {
        system_state.queues[queue_type].tasks[system_state.queues[queue_type].count++] = *task;
        task->is_queued = 1;
        printf("Added %s to %s queue\n", task->name,
               queue_type == SYSTEM ? "System" : queue_type == INTERACTIVE ? "Interactive"
                                                                           : "Batch");
    }
}
void check_ready_queue()
{
    // Check queues in priority order (System -> Interactive -> Batch)
    for (int q = 0; q < MAX_QUEUES; q++)
    {
        ReadyQueue *queue = &system_state.queues[q];

        for (int i = 0; i < queue->count; i++)
        {
            Task *task = &queue->tasks[i];

            // Check if resources are available
            if ((system_state.total_ram - system_state.used_ram) >= task->ram_required &&
                (system_state.total_hdd - system_state.used_hdd) >= task->hdd_required &&
                (system_state.cpu_cores - system_state.used_cpu) >= 1)
            {

                const char *exec_path = NULL;
                if (strcmp(task->name, "Calculator") == 0)
                {
                    exec_path = "./calculator";
                }
                else if (strcmp(task->name, "Notepad") == 0)
                {
                    exec_path = "./notepad";
                }
                else if (strcmp(task->name, "Calendar") == 0)
                {
                    exec_path = "./calendar";
                }
                else if (strcmp(task->name, "Clock") == 0)
                {
                    exec_path = "./clock";
                }
                else if (strcmp(task->name, "Resource Monitor") == 0)
                {
                    exec_path = "./resource_monitor";
                }
                else if (strcmp(task->name, "File Creator") == 0)
                {
                    exec_path = "./file_creator";
                }
                else if (strcmp(task->name, "File Deletion") == 0)
                {
                    exec_path = "./delete_file";
                }
                else if (strcmp(task->name, "File Mover") == 0)
                {
                    exec_path = "./move_file";
                }
                else if (strcmp(task->name, "Number Guessing Game") == 0)
                {
                    exec_path = "./num_guess";
                }
                else if (strcmp(task->name, "Age Calculator") == 0)
                {
                    exec_path = "./age_calculator";
                }
                else if (strcmp(task->name, "Copy Files") == 0)
                {
                    exec_path = "./copy_file";
                }
                else if (strcmp(task->name, "Tic Tac") == 0)
                {
                    exec_path = "./tic_tac";
                }
                else if (strcmp(task->name, "Encryption") == 0)
                {
                    exec_path = "./encrypt";
                }
                else if (strcmp(task->name, "Decryption") == 0)
                {
                    exec_path = "./decrypt";
                }
                else if (strcmp(task->name, "Factorial Calculator") == 0)
                {
                    exec_path = "./factorial";
                }
                else if (strcmp(task->name, "Fibonacci Game") == 0)
                {
                    exec_path = "./fibonacci";
                }
                else
                {
                    printf("Unknown task type: %s\n", task->name);
                    continue;
                }

                // Launch the task
                int pid = fork();
                if (pid == 0)
                {
                    execl(exec_path, exec_path, NULL);
                    perror("Failed to launch queued task");
                    exit(1);
                }
                else if (pid > 0)
                {
                    // Update task state
                    task->id = pid;
                    task->is_running = 1;
                    task->is_queued = 0;
                    system_state.used_ram += task->ram_required;
                    system_state.used_hdd += task->hdd_required;
                    system_state.used_cpu += 1;

                    // Add to global task list
                    system_state.tasks[system_state.task_count++] = *task;

                    // Remove from queue
                    for (int j = i; j < queue->count - 1; j++)
                    {
                        queue->tasks[j] = queue->tasks[j + 1];
                    }
                    queue->count--;

                    printf("Launched queued task: %s\n", task->name);
                }
            }
        }
    }
}

void create_process(const char *task_name, const char *exec_path,
                    int ram_required, int hdd_required, float cpu_usage)
{
    // Check resources
    if ((system_state.total_ram - system_state.used_ram) < ram_required ||
        (system_state.total_hdd - system_state.used_hdd) < hdd_required ||
        (system_state.cpu_cores - system_state.used_cpu) < 1) // Check CPU availability
    {
        printf("Insufficient resources for %s. Adding to ready queue.\n", task_name);

        Task new_task = {
            .id = -1,
            .ram_required = ram_required,
            .hdd_required = hdd_required,
            .cpu_usage = cpu_usage,
            .is_running = 0,
            .is_queued = 1};
        snprintf(new_task.name, sizeof(new_task.name), "%s", task_name);

        add_to_ready_queue(&new_task, exec_path);
        return;
    }

    // If resources available, launch immediately
    system_state.used_ram += ram_required;
    system_state.used_hdd += hdd_required;
    system_state.used_cpu += 1; // Deduct one CPU core

    int pid = fork();
    if (pid == 0)
    {
        execl(exec_path, exec_path, NULL);
        perror("Failed to launch task");
        exit(1);
    }
    else if (pid > 0)
    {
        Task *t = &system_state.tasks[system_state.task_count++];
        t->id = pid;
        snprintf(t->name, sizeof(t->name), "%s", task_name);
        t->cpu_usage = cpu_usage;
        t->ram_usage = ram_required;
        t->hdd_usage = hdd_required;
        t->is_running = 1;
        t->is_queued = 0;

        if (strcmp(task_name, "Task Manager") == 0)
        {
            t->queue_type = SYSTEM;
            t->priority = 0;
        }
        else if (strcmp(task_name, "Calculator") == 0 ||
                 strcmp(task_name, "Notepad") == 0)
        {
            t->queue_type = INTERACTIVE;
            t->priority = 1;
        }
        else
        {
            t->queue_type = BATCH;
            t->priority = 2;
        }
    }
    update_resource_display();
}

void open_calculator(GtkWidget *widget, gpointer data)
{
    create_process("Calculator", "./calculator", 1000, 500, 1.0);
}

void open_note_pad(GtkWidget *widget, gpointer data)
{
    create_process("Notepad", "./notepad", 1000, 500, 1.0);
}

void open_resource_monitor(GtkWidget *widget, gpointer data)
{
    show_resource_monitor();
}

void open_file_creator(GtkWidget *widget, gpointer data)
{
    create_process("File Creator", "./file_creator", 500, 1000, 1.0);
}

void open_delete_file(GtkWidget *widget, gpointer data)
{
    create_process("File Deletion", "./delete_file", 500, 2000, 1.0);
}

void open_move_file(GtkWidget *widget, gpointer data)
{
    create_process("File Mover", "./move_file", 500, 2000, 1.0);
}

void open_number_guess(GtkWidget *widget, gpointer data)
{
    create_process("Number Guessing Game", "./num_guess", 500, 1000, 1.0);
}

void open_age_calculator(GtkWidget *widget, gpointer data)
{
    create_process("Age Calculator", "./age_calculator", 500, 1000, 1.0);
}
void open_copy_file(GtkWidget *widget, gpointer data)
{
    create_process("Copy Files", "./copy_file", 500, 1000, 1.0);
}

void open_tic_tac(GtkWidget *widget, gpointer data)
{
    create_process("Tic Tac", "./tic_tac", 500, 2000, 1.0);
}

void open_encrypt_(GtkWidget *widget, gpointer data)
{
    create_process("Encryption", "./encrypt", 500, 2000, 1.0);
}

void open_decrypt(GtkWidget *widget, gpointer data)
{
    create_process("Decryption", "./decrypt", 500, 2000, 1.0);
}

void open_factorial(GtkWidget *widget, gpointer data)
{
    create_process("Factorial Calculator", "./factorial", 500, 1000, 1.0);
}

void open_fibonacci(GtkWidget *widget, gpointer data)
{
    create_process("Fibonacci Game", "./fibonacci", 500, 1000, 1.0);
}

void show_mode_selection()
{
    GtkWidget *window, *box, *user_btn, *kernel_btn, *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Select Mode");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    label = gtk_label_new("Select system mode:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

    user_btn = gtk_button_new_with_label("User Mode");
    g_signal_connect(user_btn, "clicked", G_CALLBACK(launch_desktop), NULL);
    gtk_box_pack_start(GTK_BOX(box), user_btn, FALSE, FALSE, 5);

    kernel_btn = gtk_button_new_with_label("Kernel Mode");
    g_signal_connect(kernel_btn, "clicked", G_CALLBACK(prompt_kernel_password), window);
    gtk_box_pack_start(GTK_BOX(box), kernel_btn, FALSE, FALSE, 5);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
}

void terminate_all_tasks(GtkWidget *widget, gpointer data)
{
    if (system_state.current_mode != KERNEL_MODE)
    {
        printf("Permission denied: Requires kernel mode\n");
        return;
    }

    for (int i = 0; i < system_state.task_count; i++)
    {
        Task *t = &system_state.tasks[i];
        if (t->is_running)
        {
            kill(t->id, SIGTERM);
            system_state.used_ram -= t->ram_usage;
            system_state.used_hdd -= t->hdd_usage;
            system_state.used_cpu -= 1;
        }
    }
    system_state.task_count = 0;
    update_resource_display();
}

void launch_desktop()
{
    GtkWidget *window, *main_box, *box, *info_box, *clock_cal_box;
    GtkWidget *calc_btn, *note_btn, *label;
    char info[256];

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "OS Simulator Desktop");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);

    // Main vertical box
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Info box at top
    info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), info_box, FALSE, FALSE, 5);

    sprintf(info, "RAM: %d MB | HDD: %d MB | Cores: %d",
            system_state.total_ram, system_state.total_hdd, system_state.cpu_cores);
    label = gtk_label_new(info);
    gtk_box_pack_start(GTK_BOX(info_box), label, FALSE, FALSE, 5);

    // Clock and calendar box
    clock_cal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_start(GTK_BOX(main_box), clock_cal_box, FALSE, FALSE, 10);

    // Clock
    clock_label = GTK_LABEL(gtk_label_new("Time: --:--:--"));
    gtk_box_pack_start(GTK_BOX(clock_cal_box), GTK_WIDGET(clock_label), FALSE, FALSE, 5);
    g_timeout_add_seconds(1, update_clock, NULL);

    // Calendar
    calendar_label = GTK_LABEL(gtk_label_new(""));
    gtk_box_pack_start(GTK_BOX(clock_cal_box), GTK_WIDGET(calendar_label), FALSE, FALSE, 5);
    update_calendar();

    // Button box for all application buttons
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), box, TRUE, TRUE, 10);

    calc_btn = gtk_button_new_with_label("Open Calculator");
    g_signal_connect(calc_btn, "clicked", G_CALLBACK(open_calculator), NULL);
    gtk_box_pack_start(GTK_BOX(box), calc_btn, FALSE, FALSE, 5);

    note_btn = gtk_button_new_with_label("Open Notepad");
    g_signal_connect(note_btn, "clicked", G_CALLBACK(open_note_pad), NULL);
    gtk_box_pack_start(GTK_BOX(box), note_btn, FALSE, FALSE, 5);

    GtkWidget *monitor_btn = gtk_button_new_with_label("Resource Monitor");
    g_signal_connect(monitor_btn, "clicked", G_CALLBACK(open_resource_monitor), NULL);
    gtk_box_pack_start(GTK_BOX(box), monitor_btn, FALSE, FALSE, 5);

    GtkWidget *file_creator_btn = gtk_button_new_with_label("Create New File");
    g_signal_connect(file_creator_btn, "clicked", G_CALLBACK(open_file_creator), window);
    gtk_box_pack_start(GTK_BOX(box), file_creator_btn, FALSE, FALSE, 5);

    GtkWidget *delete_btn = gtk_button_new_with_label("Delete File");
    g_signal_connect(delete_btn, "clicked", G_CALLBACK(open_delete_file), window);
    gtk_box_pack_start(GTK_BOX(box), delete_btn, FALSE, FALSE, 5);

    GtkWidget *move_btn = gtk_button_new_with_label("Move File");
    g_signal_connect(move_btn, "clicked", G_CALLBACK(open_move_file), window);
    gtk_box_pack_start(GTK_BOX(box), move_btn, FALSE, FALSE, 5);

    GtkWidget *num_btn = gtk_button_new_with_label("Number Guessing Game");
    g_signal_connect(num_btn, "clicked", G_CALLBACK(open_number_guess), window);
    gtk_box_pack_start(GTK_BOX(box), num_btn, FALSE, FALSE, 5);

    GtkWidget *age_btn = gtk_button_new_with_label("Age Calculator");
    g_signal_connect(age_btn, "clicked", G_CALLBACK(open_age_calculator), window);
    gtk_box_pack_start(GTK_BOX(box), age_btn, FALSE, FALSE, 5);

    GtkWidget *copy_btn = gtk_button_new_with_label("Copy File");
    g_signal_connect(copy_btn, "clicked", G_CALLBACK(open_copy_file), window);
    gtk_box_pack_start(GTK_BOX(box), copy_btn, FALSE, FALSE, 5);

    GtkWidget *tic_btn = gtk_button_new_with_label("Tic Tac Toe Game");
    g_signal_connect(tic_btn, "clicked", G_CALLBACK(open_tic_tac), window);
    gtk_box_pack_start(GTK_BOX(box), tic_btn, FALSE, FALSE, 5);

    GtkWidget *encrypt_btn = gtk_button_new_with_label("Password Encryption");
    g_signal_connect(encrypt_btn, "clicked", G_CALLBACK(open_encrypt_), window);
    gtk_box_pack_start(GTK_BOX(box), encrypt_btn, FALSE, FALSE, 5);

    GtkWidget *decrypt_btn = gtk_button_new_with_label("Password Decryption");
    g_signal_connect(decrypt_btn, "clicked", G_CALLBACK(open_decrypt), window);
    gtk_box_pack_start(GTK_BOX(box), decrypt_btn, FALSE, FALSE, 5);

    GtkWidget *fac_btn = gtk_button_new_with_label("Factorial Calculator");
    g_signal_connect(fac_btn, "clicked", G_CALLBACK(open_factorial), window);
    gtk_box_pack_start(GTK_BOX(box), fac_btn, FALSE, FALSE, 5);

    GtkWidget *fibo_btn = gtk_button_new_with_label("Fibonacci Game");
    g_signal_connect(fibo_btn, "clicked", G_CALLBACK(open_fibonacci), window);
    gtk_box_pack_start(GTK_BOX(box), fibo_btn, FALSE, FALSE, 5);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
}



void launch_kernel_ui() {
    GtkWidget *window, *main_box, *button_box, *info_box, *clock_box;
    GtkWidget *label;
    char info[256];
    
    // Global clock label (same as in desktop.c)
    extern GtkLabel *clock_label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "OS Simulator - Kernel Mode");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main vertical box
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Info box at top
    info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), info_box, FALSE, FALSE, 5);

    sprintf(info, "KERNEL MODE | RAM: %d MB | HDD: %d MB | Cores: %d",
            system_state.total_ram, system_state.total_hdd, system_state.cpu_cores);
    label = gtk_label_new(info);
    gtk_box_pack_start(GTK_BOX(info_box), label, FALSE, FALSE, 5);

    // Clock box
    clock_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), clock_box, FALSE, FALSE, 5);
    
    clock_label = GTK_LABEL(gtk_label_new("Time: --:--:--"));
    gtk_box_pack_start(GTK_BOX(clock_box), GTK_WIDGET(clock_label), FALSE, FALSE, 5);
    g_timeout_add_seconds(1, update_clock, NULL);

    // Button box for kernel functions
    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, TRUE, TRUE, 10);

    // Kernel mode specific buttons
    GtkWidget *monitor_btn = gtk_button_new_with_label("Resource Monitor");
    g_signal_connect(monitor_btn, "clicked", G_CALLBACK(open_resource_monitor), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), monitor_btn, FALSE, FALSE, 5);

    GtkWidget *calc_btn = gtk_button_new_with_label("Open Calculator");
    g_signal_connect(calc_btn, "clicked", G_CALLBACK(open_calculator), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), calc_btn, FALSE, FALSE, 5);

    GtkWidget *decrypt_btn = gtk_button_new_with_label("Password Decryption");
    g_signal_connect(decrypt_btn, "clicked", G_CALLBACK(open_decrypt), window);
    gtk_box_pack_start(GTK_BOX(button_box), decrypt_btn, FALSE, FALSE, 5);

    GtkWidget *terminate_all_btn = gtk_button_new_with_label("Terminate All Tasks");
    g_signal_connect(terminate_all_btn, "clicked", G_CALLBACK(terminate_all_tasks), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), terminate_all_btn, FALSE, FALSE, 5);

    gtk_widget_show_all(window);
}

void prompt_kernel_password(GtkWidget *widget, gpointer window)
{
    GtkWidget *dialog, *content_area, *entry, *label;

    dialog = gtk_dialog_new_with_buttons("Enter Kernel Password",
                                         GTK_WINDOW(window),
                                         GTK_DIALOG_MODAL,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_REJECT,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("Enter kernel password:");
    entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

    gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 5);
    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT)
    {
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strcmp(password, system_state.kernel_password) == 0)
        {
            // Hide the mode selection window instead of destroying it
            gtk_widget_hide(GTK_WIDGET(window));
            system_state.current_mode = KERNEL_MODE;
            launch_kernel_ui();
        }
        else
        {
            GtkWidget *error = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Incorrect password!");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
        }
    }
    gtk_widget_destroy(dialog);
}