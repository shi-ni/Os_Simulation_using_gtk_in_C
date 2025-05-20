# 🖥️ OS Simulation using GTK in C

**Author:** Hasan Zubair
**Date:** May 2025

---

## 📌 Project Overview

This Operating System Simulator replicates a basic desktop environment using **C** and **GTK** on **Linux**. It allows users to launch multiple applications (tasks) in separate terminal windows, simulating:

- Process management  
- Memory allocation  
- Inter-process communication (IPC)

Each application runs as a separate process using `fork()` and `exec()`. Tasks communicate with the system for resource requests (RAM, HDD) via IPC. A GTK-based desktop interface acts as the main control panel.

---

## 📁 Project Structure

| File/Folder           | Description                                      |
|------------------------|--------------------------------------------------|
| `main.c`               | Entry point of the OS simulator                  |
| `desktop.c` / `desktop.h` | Handles the desktop UI (GTK-based)          |
| `os_simulator`         | Final compiled binary (desktop launcher)        |
| `task_manager.c`       | Manages processes and task scheduling           |
| `resource_monitor.c`   | Monitors memory and CPU usage                   |
| `include/`             | Header files                                    |
| `makefile`             | Builds all modules                              |
| `notes.txt`            | Project notes or reminders                      |
| `*.c` files            | Individual application/task source code         |

---

## 🧩 Applications / Tasks Available

Each of the following runs as a **separate child process**:

- `calculator.c` – Basic calculator  
- `notepad.c` – Text editor  
- `clock.c` – Real-time digital clock  
- `calendar.c` – Displays a calendar  
- `age_calculator.c` – Calculates age from birthdate  
- `factorial.c` – Computes factorial  
- `fibonacci.c` – Displays Fibonacci series  
- `num_guess.c` – Number guessing game  
- `tic_tac.c` – Tic-Tac-Toe game  
- `file_creator.c` – Creates files  
- `copy_file.c`, `move_file.c`, `delete_file.c` – File operations  
- `encrypt.c`, `decrypt.c` – File encryption/decryption  
- `beep.c` – Beep sound (if supported)

---

## ⚙️ Compilation Instructions

To compile all components:

```bash
make clean && make
