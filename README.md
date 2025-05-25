# CPU Scheduling Algorithms Simulation

This project implements and simulates various CPU scheduling algorithms to compare their performance based on different metrics. The implemented algorithms include:

- **First-Come-First-Serve (FCFS)**: A non-preemptive algorithm where processes are executed in the order they arrive.
- **Shortest Job First (SJF)**: A non-preemptive algorithm where the process with the smallest burst time is selected for execution.
- **Shortest Remaining Time First (SRTF)**: A preemptive version of SJF where the process with the smallest remaining time is selected for execution.
- **Round Robin (RR)**: A preemptive algorithm where each process is assigned a fixed time slice in a cyclic way.

## Project Structure

```
.
├── Makefile           # Build configuration
├── README.md          # Project documentation
├── common.c           # Common utility functions implementation
├── common.h           # Common structures and function declarations
├── data/              # Directory containing process data
│   └── processes.csv  # Sample process data in CSV format
├── fcfs.c             # FCFS algorithm implementation
├── fcfs.h             # FCFS algorithm declarations
├── main.c             # Main program entry point
├── rr.c               # Round Robin algorithm implementation
├── rr.h               # Round Robin algorithm declarations
├── sjf.c              # SJF/SRTF algorithm implementations
└── sjf.h              # SJF/SRTF algorithm declarations
```

## Process Data Format

The process data is stored in CSV format with the following columns:

- `process_id`: Unique identifier for the process
- `arrival_time`: Time at which the process arrives in the ready queue
- `burst_time`: CPU time required by the process
- `priority`: Priority of the process (lower value means higher priority)

Example:
```
process_id,arrival_time,burst_time,priority
P1,0,6,3
P2,1,8,1
...
```

## Performance Metrics

The simulation calculates the following performance metrics for each scheduling algorithm:

- **Turnaround Time**: Time difference between completion time and arrival time
- **Waiting Time**: Time difference between turnaround time and burst time
- **Response Time**: Time at which the process first gets the CPU after arrival

The average of these metrics is used to compare the performance of different scheduling algorithms.

## Building and Running

### Prerequisites

- GCC compiler
- Make utility

### Building the Project

```bash
make
```

### Running the Simulation

To run all scheduling algorithms:
```bash
./cpu_scheduler
```

To run a specific algorithm:
```bash
./cpu_scheduler -a [algorithm]
```
Where `[algorithm]` can be one of: `fcfs`, `sjf`, `srtf`, `rr`, or `all`.

To specify a custom process data file:
```bash
./cpu_scheduler -f [file_path]
```

To set a custom time quantum for Round Robin:
```bash
./cpu_scheduler -a rr -q [quantum]
```

For help:
```bash
./cpu_scheduler -h
```

### Makefile Targets

- `make`: Build the project
- `make clean`: Remove object files and executable
- `make run`: Run all algorithms with default settings
- `make run_fcfs`: Run only FCFS algorithm
- `make run_sjf`: Run only SJF algorithm
- `make run_srtf`: Run only SRTF algorithm
- `make run_rr`: Run only RR algorithm with default quantum
- `make run_rr_q4`: Run only RR algorithm with quantum = 4

## Implementation Details

### Common Module

The `common.c/h` files provide utility functions and data structures used by all scheduling algorithms, including:

- Process structure to store process attributes and metrics
- Functions to read process data from CSV files
- Functions to calculate and print performance metrics

### FCFS Implementation

The FCFS algorithm is implemented in `fcfs.c/h`. It sorts processes by arrival time and executes them in that order without preemption.

### SJF/SRTF Implementation

The SJF and SRTF algorithms are implemented in `sjf.c/h`. The non-preemptive SJF selects the process with the smallest burst time among the arrived processes. The preemptive SRTF continuously selects the process with the smallest remaining time and can preempt the currently running process if a new process with a smaller remaining time arrives.

### Round Robin Implementation

The Round Robin algorithm is implemented in `rr.c/h`. It uses a queue to manage processes and assigns a fixed time quantum to each process in a cyclic manner. If a process's remaining time exceeds the quantum, it is preempted and added to the end of the queue.

## Example Output

The program outputs a table of process details and metrics for each scheduling algorithm, followed by the average metrics:

```
Read 10 processes from data/processes.csv

Running First-Come-First-Serve (FCFS) algorithm...

Process    Arrival      Burst      Priority    Completion     Turnaround     Waiting      
----------------------------------------------------------------------------------
P1         0            6          3           6              6              0              
P2         1            8          1           14             13             5              
...
----------------------------------------------------------------------------------

FCFS Scheduling Algorithm Metrics:
Average Turnaround Time: 28.30
Average Waiting Time: 21.40
Average Response Time: 21.40
----------------------------------------------------------------------------------
```
