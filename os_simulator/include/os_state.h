//include/os_state.h
#ifndef OS_STATE_H
#define OS_STATE_H

#define MAX_TASKS 10
#define MAX_QUEUES 3  // For multilevel queues

typedef enum {
    SYSTEM = 0,
    INTERACTIVE = 1,
    BATCH = 2
} QueueType;

typedef enum {
    USER_MODE,
    KERNEL_MODE
} SystemMode;



typedef struct {
    int id;
    char name[50];
    char exec_path[256]; 
    int ram_usage;
    int hdd_usage;
    float cpu_usage;
    int is_running;
    int is_queued;
    int ram_required;
    int hdd_required;
    QueueType queue_type;
    int priority;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int count;
} ReadyQueue;

typedef struct {
    SystemMode current_mode;
    char kernel_password[50];
    int total_ram;
    int total_hdd;
    int cpu_cores;
    int used_ram;
    int used_hdd;
    int used_cpu;
    Task tasks[MAX_TASKS];
    int task_count;
    ReadyQueue queues[MAX_QUEUES];  // Multilevel queues
} OSState;

extern OSState system_state;

void schedule_tasks();
void add_to_ready_queue(Task *task,const char *exec_path);
void check_ready_queue();
void update_resource_display();
void show_mode_selection();
void show_resource_monitor();
#endif