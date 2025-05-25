/**
 * @file sjf.c
 * @brief Implementation of Shortest Job First (SJF) CPU scheduling algorithm
 */

#include "sjf.h"
#include <limits.h> /* For INT_MAX */

/**
 * @brief Comparison function for sorting processes by arrival time
 * @param a First process
 * @param b Second process
 * @return Negative if a arrives before b, positive if a arrives after b
 */
static int compare_arrival_time(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    return p1->arrival_time - p2->arrival_time;
}

/**
 * @brief Executes the non-preemptive Shortest Job First (SJF) scheduling algorithm
 * 
 * SJF is a non-preemptive scheduling algorithm where the process with the
 * smallest burst time is selected for execution next. Once a process starts
 * executing, it continues until it completes.
 * 
 * @param processes Array of processes
 * @param n Number of processes
 * @return Metrics structure containing the performance metrics
 */
Metrics sjf_non_preemptive_schedule(Process* processes, int n) {
    // Sort processes by arrival time initially
    qsort(processes, n, sizeof(Process), compare_arrival_time);
    
    int current_time = 0;
    int completed = 0;
    bool* is_completed = (bool*)calloc(n, sizeof(bool));
    
    if (!is_completed) {
        perror("Memory allocation failed");
        Metrics empty = {0};
        return empty;
    }
    
    // Continue until all processes are completed
    while (completed < n) {
        int shortest_job_idx = -1;
        int shortest_burst = INT_MAX;
        
        // Find the process with the shortest burst time among the arrived processes
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && 
                processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_job_idx = i;
            }
        }
        
        // If no process is found, advance time to the next arrival
        if (shortest_job_idx == -1) {
            int next_arrival_time = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && processes[i].arrival_time < next_arrival_time) {
                    next_arrival_time = processes[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
            continue;
        }
        
        // Execute the selected process
        Process* p = &processes[shortest_job_idx];
        
        // Set response time when process first gets CPU
        p->response_time = current_time - p->arrival_time;
        p->started = true;
        
        // Execute the process (advance time by burst time)
        current_time += p->burst_time;
        
        // Set completion time
        p->completion_time = current_time;
        p->remaining_time = 0;
        
        // Mark as completed
        is_completed[shortest_job_idx] = true;
        completed++;
    }
    
    free(is_completed);
    
    // Calculate and return metrics
    return calculate_metrics(processes, n);
}

/**
 * @brief Executes the preemptive Shortest Job First (SJF) scheduling algorithm
 * 
 * Also known as Shortest Remaining Time First (SRTF), this is a preemptive version
 * of SJF where the process with the smallest remaining time is selected for execution.
 * If a new process arrives with a smaller burst time than the remaining time of the
 * current process, the current process is preempted.
 * 
 * @param processes Array of processes
 * @param n Number of processes
 * @return Metrics structure containing the performance metrics
 */
Metrics sjf_preemptive_schedule(Process* processes, int n) {
    // Sort processes by arrival time initially
    qsort(processes, n, sizeof(Process), compare_arrival_time);
    
    int current_time = 0;
    int completed = 0;
    bool* is_completed = (bool*)calloc(n, sizeof(bool));
    
    if (!is_completed) {
        perror("Memory allocation failed");
        Metrics empty = {0};
        return empty;
    }
    
    // Continue until all processes are completed
    while (completed < n) {
        int shortest_job_idx = -1;
        int shortest_remaining_time = INT_MAX;
        
        // Find the process with the shortest remaining time among the arrived processes
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && 
                processes[i].remaining_time < shortest_remaining_time) {
                shortest_remaining_time = processes[i].remaining_time;
                shortest_job_idx = i;
            }
        }
        
        // If no process is found, advance time to the next arrival
        if (shortest_job_idx == -1) {
            int next_arrival_time = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && processes[i].arrival_time < next_arrival_time) {
                    next_arrival_time = processes[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
            continue;
        }
        
        // Execute the selected process for 1 time unit
        Process* p = &processes[shortest_job_idx];
        
        // Set response time when process first gets CPU
        if (!p->started) {
            p->response_time = current_time - p->arrival_time;
            p->started = true;
        }
        
        // Execute for 1 time unit
        p->remaining_time--;
        current_time++;
        
        // Check if the process is completed
        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            is_completed[shortest_job_idx] = true;
            completed++;
        }
        
        // Check if a new process has arrived that should preempt the current one
        // This is handled automatically in the next iteration of the loop
    }
    
    free(is_completed);
    
    // Calculate and return metrics
    return calculate_metrics(processes, n);
}