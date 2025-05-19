# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall $(shell pkg-config --cflags gtk+-3.0) -Iinclude
LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lpthread

# Source files
SRCS = main.c desktop.c resource_monitor.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = os_simulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


