//main.c
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "include/os_state.h"
#include "desktop.h"
#include <sys/wait.h>
#include <fcntl.h>
OSState system_state;

void terminate_task_by_pid(pid_t pid) {
    for (int i = 0; i < system_state.task_count; i++) {
        Task *t = &system_state.tasks[i];
        printf("Checking task %s with pid %d\n", t->name, t->id);
        if (t->id == pid) {
            if (t->is_running) {
                printf("Process (%s) with PID %d terminated. Reclaiming resources.\n\n\n", t->name, pid);
                system_state.used_ram -= t->ram_usage;
                system_state.used_hdd -= t->hdd_usage;
                system_state.used_cpu -= 1;  // Return one CPU core
                t->is_running = 0;
                
                // Remove from global task list
                for (int j = i; j < system_state.task_count - 1; j++) {
                    system_state.tasks[j] = system_state.tasks[j+1];
                }
                system_state.task_count--;
                
                update_resource_display();
                check_ready_queue();  // Check if queued tasks can now run
            } else {
                printf("Process already marked as terminated.\n");
            }
            return;
        }
    }
    printf("Task with PID %d not found or already terminated.\n", pid);
}

void* process_watcher_thread(void* arg) {
    printf("Process watcher thread started\n");
    while (1) {
        int status;
        pid_t pid;

        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("Detected process %d terminated with status %d\n", pid, status);
            terminate_task_by_pid(pid);
            check_ready_queue();  // Check if queued tasks can now run
        }
        
        usleep(100000); // Sleep for 100 ms
    }
    
    return NULL;
}

void start_process_watcher() {
    pthread_t watcher;
    pthread_create(&watcher, NULL, process_watcher_thread, NULL);
    pthread_detach(watcher); // Let it run independently
}
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <RAM_MB> <HDD_MB> <CPU_CORES>\n", argv[0]);
        return 1;
    }

    system_state.total_ram = atoi(argv[1]);
    system_state.total_hdd = atoi(argv[2]);
    system_state.cpu_cores = atoi(argv[3]);
    system_state.used_cpu = 0;
    system_state.current_mode = USER_MODE; // Default to user mode
    strcpy(system_state.kernel_password, "123"); // Set kernel password

    printf("Booting OS with RAM: %d MB, HDD: %d MB, Cores: %d\n",
          system_state.total_ram, system_state.total_hdd, system_state.cpu_cores);

    start_process_watcher();
    gtk_init(&argc, &argv);
    show_mode_selection();
    gtk_main();
    
    return 0;
}
