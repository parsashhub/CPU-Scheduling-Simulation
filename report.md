# CPU Scheduling Algorithms Simulation Project Report

## 1. Introduction

This report presents a comprehensive analysis of a CPU scheduling algorithms simulation project implemented in C. The project simulates several fundamental CPU scheduling algorithms used in operating systems to manage process execution. The simulation allows for the evaluation and comparison of different scheduling strategies based on key performance metrics.

### 1.1 Project Objectives

The main objectives of this project are:

1. Implement and simulate common CPU scheduling algorithms:
   - First-Come-First-Serve (FCFS)
   - Shortest Job First (SJF) - non-preemptive
   - Shortest Remaining Time First (SRTF) - preemptive version of SJF
   - Round Robin (RR) with configurable time quantum

2. Calculate and compare performance metrics for each algorithm:
   - Turnaround Time
   - Waiting Time
   - Response Time

3. Create a modular, well-documented codebase that can be easily extended with additional scheduling algorithms

## 2. Theoretical Background

### 2.1 CPU Scheduling Concepts

CPU scheduling is a fundamental operating system function that determines which process in the ready queue should be allocated the CPU. Scheduling decisions occur when a process:

- Switches from running to waiting state
- Switches from running to ready state
- Switches from waiting to ready state
- Terminates

Scheduling algorithms can be categorized as:

- **Non-preemptive**: Once a process starts executing, it continues until it completes or enters a waiting state
- **Preemptive**: The currently running process can be interrupted and moved to the ready queue if a higher priority process arrives

### 2.2 Performance Metrics

The following metrics are used to evaluate scheduling algorithms:

- **Turnaround Time**: Time from process submission to completion
  - Formula: Completion Time - Arrival Time

- **Waiting Time**: Total time a process spends waiting in the ready queue
  - Formula: Turnaround Time - Burst Time

- **Response Time**: Time from process submission until it first gets CPU time
  - Formula: First CPU Time - Arrival Time

## 3. Implemented Scheduling Algorithms

### 3.1 First-Come-First-Serve (FCFS)

**Description**: FCFS is the simplest scheduling algorithm that executes processes in the order they arrive in the ready queue.

**Characteristics**:
- Non-preemptive
- Easy to implement
- Can lead to the "convoy effect" where short processes wait behind long ones

**Implementation Details**:

The FCFS algorithm is implemented in `fcfs.c`. The key steps are:

1. Sort processes by arrival time using `qsort` with a custom comparison function
2. Process each job in order of arrival
3. For each process:
   - If current time is less than arrival time, advance time to arrival time
   - Set response time when process first gets CPU
   - Execute the process (advance time by burst time)
   - Set completion time
4. Calculate and return performance metrics

### 3.2 Shortest Job First (SJF) - Non-preemptive

**Description**: SJF selects the process with the smallest burst time from the ready queue.

**Characteristics**:
- Non-preemptive
- Optimal for minimizing average waiting time
- Requires knowledge of burst times in advance
- Potential for starvation of longer processes

**Implementation Details**:

The non-preemptive SJF algorithm is implemented in `sjf.c`. The key steps are:

1. Sort processes by arrival time initially
2. At each scheduling point:
   - Find the process with the shortest burst time among arrived processes
   - If no process is available, advance time to the next arrival
   - Execute the selected process to completion
   - Update response and completion times
3. Calculate and return performance metrics

### 3.3 Shortest Remaining Time First (SRTF) - Preemptive SJF

**Description**: SRTF is the preemptive version of SJF where the process with the smallest remaining time is selected for execution.

**Characteristics**:
- Preemptive
- Optimal for minimizing average waiting time
- Higher overhead due to frequent context switches
- Potential for starvation of longer processes

**Implementation Details**:

The preemptive SRTF algorithm is implemented in `sjf.c`. The key steps are:

1. Sort processes by arrival time initially
2. At each time unit:
   - Find the process with the shortest remaining time among arrived processes
   - If no process is available, advance time to the next arrival
   - Execute the selected process for 1 time unit
   - Update remaining time
   - If process completes, update completion time
   - Check for new arrivals that might preempt the current process
3. Calculate and return performance metrics

### 3.4 Round Robin (RR)

**Description**: Round Robin assigns a fixed time quantum to each process in a cyclic manner.

**Characteristics**:
- Preemptive
- Fair allocation of CPU time
- Good for time-sharing systems
- Performance depends on the time quantum size

**Implementation Details**:

The Round Robin algorithm is implemented in `rr.c`. The key steps are:

1. Sort processes by arrival time initially
2. Implement a queue for ready processes
3. At each scheduling point:
   - Add newly arrived processes to the ready queue
   - If ready queue is empty, advance time to next arrival
   - Get the next process from the ready queue
   - Set response time if process is running for the first time
   - Execute the process for the time quantum or until completion
   - If process is not complete, add it back to the ready queue
4. Calculate and return performance metrics

The implementation includes a custom queue data structure with operations like `enqueue`, `dequeue`, `is_empty`, and `is_full`.

## 4. Project Structure

The project follows a modular design with separate files for each algorithm and common functionality:

### 4.1 Directory Structure

```
/
├── data/
│   └── processes.csv       # Sample process data
├── common.h                # Common data structures and functions
├── common.c                # Implementation of common functions
├── fcfs.h                  # FCFS algorithm header
├── fcfs.c                  # FCFS algorithm implementation
├── sjf.h                   # SJF algorithm header
├── sjf.c                   # SJF algorithm implementation
├── rr.h                    # Round Robin algorithm header
├── rr.c                    # Round Robin algorithm implementation
├── main.c                  # Main program and CLI interface
├── Makefile                # Build configuration
└── .gitignore              # Git ignore file
```

### 4.2 Key Components

#### 4.2.1 Data Structures

The project defines two main data structures in `common.h`:

1. `Process`: Represents a process with attributes like ID, arrival time, burst time, etc.

```c
typedef struct {
    char id[10];           // Process ID
    int arrival_time;      // Time when process arrives
    int burst_time;        // Total execution time required
    int priority;          // Priority of the process (lower value = higher priority)
    int remaining_time;    // Remaining execution time
    int completion_time;   // Time when process completes
    int turnaround_time;   // Completion time - Arrival time
    int waiting_time;      // Turnaround time - Burst time
    int response_time;     // Time when process first gets CPU - Arrival time
    bool started;          // Whether process has started execution
} Process;
```

2. `Metrics`: Stores performance metrics for a scheduling algorithm

```c
typedef struct {
    float avg_turnaround_time;  // Average turnaround time
    float avg_waiting_time;     // Average waiting time
    float avg_response_time;    // Average response time
} Metrics;
```

#### 4.2.2 Common Functions

The `common.c` file implements several utility functions:

- `read_processes`: Reads process data from a CSV file
- `print_processes`: Displays process details in a tabular format
- `print_metrics`: Displays performance metrics for an algorithm
- `calculate_metrics`: Calculates turnaround, waiting, and response times
- `copy_processes`: Creates a deep copy of a process array

#### 4.2.3 Main Program

The `main.c` file provides a command-line interface with options to:

- Select a specific scheduling algorithm or run all algorithms
- Specify the input data file
- Set the time quantum for Round Robin
- Display help information

#### 4.2.4 Build System

The `Makefile` defines rules for:

- Compiling the project
- Cleaning build artifacts
- Running the program with different algorithms
- Running with custom parameters

## 5. Implementation Analysis

### 5.1 Code Quality

The project demonstrates several good programming practices:

1. **Modularity**: Each algorithm is implemented in separate files
2. **Documentation**: Comprehensive comments using Doxygen-style format
3. **Error Handling**: Memory allocation failures and file I/O errors are handled
4. **Memory Management**: Proper allocation and deallocation of resources
5. **Code Reuse**: Common functions are shared across algorithms

### 5.2 Algorithm Efficiency

The time complexity of the implemented algorithms varies:

- **FCFS**: O(n log n) due to the initial sorting by arrival time
- **SJF (non-preemptive)**: O(n²) as we need to find the shortest job at each step
- **SRTF (preemptive)**: O(n²) with higher constant factors due to more frequent scheduling decisions
- **Round Robin**: O(n * total_burst_time) as each process can be scheduled multiple times

### 5.3 Sample Data Analysis

The project includes a sample data file `processes.csv` with 10 processes:

```
process_id,arrival_time,burst_time,priority
P1,0,6,3
P2,1,8,1
P3,2,7,2
P4,3,3,4
P5,4,4,5
P6,5,5,2
P7,6,2,6
P8,7,9,1
P9,8,4,3
P10,9,10,2
```

This data provides a good mix of processes with different arrival times and burst times for testing the scheduling algorithms.

## 6. Conclusion

This project successfully implements and simulates four common CPU scheduling algorithms: FCFS, SJF (non-preemptive), SRTF (preemptive SJF), and Round Robin. The modular design and well-documented code make it easy to understand and extend.

The simulation allows for the comparison of different scheduling strategies based on key performance metrics like turnaround time, waiting time, and response time. This can help in understanding the trade-offs between different scheduling approaches and selecting the most appropriate algorithm for specific scenarios.

### 6.1 Future Enhancements

Possible improvements to the project include:

1. Implementing additional scheduling algorithms (e.g., Priority Scheduling, Multilevel Queue)
2. Adding visualization of the scheduling process
3. Supporting more complex process models with I/O bursts and dependencies
4. Implementing a graphical user interface for easier interaction
5. Adding statistical analysis and comparison of algorithm performance

## 7. References

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley.
2. Tanenbaum, A. S., & Bos, H. (2014). Modern Operating Systems (4th ed.). Pearson.
3. Stallings, W. (2017). Operating Systems: Internals and Design Principles (9th ed.). Pearson.