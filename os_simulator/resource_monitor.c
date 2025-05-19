#include <gtk/gtk.h>
#include <stdio.h>
#include "include/os_state.h"

extern OSState system_state;

static GtkWidget *ram_label, *hdd_label, *cpu_label;
static GtkWidget *window = NULL;

void update_resource_display() {
    if (window && gtk_widget_get_visible(window)) {
        char ram_text[100], hdd_text[100], cpu_text[100];
        
        int ram_remaining = system_state.total_ram - system_state.used_ram;
        int hdd_remaining = system_state.total_hdd - system_state.used_hdd;
        int cpu_remaining = system_state.cpu_cores - system_state.used_cpu;
        
        snprintf(ram_text, sizeof(ram_text), "RAM: %d/%d MB (%.1f%%)", 
                ram_remaining, system_state.total_ram,
                ((float)ram_remaining/system_state.total_ram)*100);
        snprintf(hdd_text, sizeof(hdd_text), "HDD: %d/%d MB (%.1f%%)", 
                hdd_remaining, system_state.total_hdd,
                ((float)hdd_remaining/system_state.total_hdd)*100);
        snprintf(cpu_text, sizeof(cpu_text), "CPU: %d/%d cores available", 
                cpu_remaining, system_state.cpu_cores);
        
        gtk_label_set_text(GTK_LABEL(ram_label), ram_text);
        gtk_label_set_text(GTK_LABEL(hdd_label), hdd_text);
        gtk_label_set_text(GTK_LABEL(cpu_label), cpu_text);
    }
}

void show_resource_monitor() {
    if (window) {
        gtk_window_present(GTK_WINDOW(window));
        return;
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Resource Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);
    
    // Create resource labels
    ram_label = gtk_label_new("");
    hdd_label = gtk_label_new("");
    cpu_label = gtk_label_new("");
    
    gtk_box_pack_start(GTK_BOX(box), ram_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), hdd_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), cpu_label, FALSE, FALSE, 5);
    
    // Initial update
    update_resource_display();
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroyed), &window);
    
    gtk_widget_show_all(window);
}