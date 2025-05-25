# Makefile for CPU Scheduling Algorithms Simulation

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lm

# Source files and object files
SRCS = main.c common.c fcfs.c sjf.c rr.c
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = cpu_scheduler

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

# Run with specific algorithm
run_fcfs: $(TARGET)
	./$(TARGET) -a fcfs

run_sjf: $(TARGET)
	./$(TARGET) -a sjf

run_srtf: $(TARGET)
	./$(TARGET) -a srtf

run_rr: $(TARGET)
	./$(TARGET) -a rr

# Run with custom time quantum
run_rr_q4: $(TARGET)
	./$(TARGET) -a rr -q 4

# Dependencies
main.o: main.c common.h fcfs.h sjf.h rr.h
common.o: common.c common.h
fcfs.o: fcfs.c fcfs.h common.h
sjf.o: sjf.c sjf.h common.h
rr.o: rr.c rr.h common.h

.PHONY: all clean run run_fcfs run_sjf run_srtf run_rr run_rr_q4