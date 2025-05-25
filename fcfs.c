/**
 * @file fcfs.c
 * @brief Implementation of First-Come-First-Serve (FCFS) CPU scheduling algorithm
 */

#include "fcfs.h"

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
 * @brief Executes the First-Come-First-Serve (FCFS) scheduling algorithm
 * 
 * FCFS is a non-preemptive scheduling algorithm where processes are executed
 * in the order they arrive in the ready queue.
 * 
 * @param processes Array of processes
 * @param n Number of processes
 * @return Metrics structure containing the performance metrics
 */
Metrics fcfs_schedule(Process* processes, int n) {
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compare_arrival_time);
    
    int current_time = 0;
    
    // Execute processes in order of arrival
    for (int i = 0; i < n; i++) {
        // If the process hasn't arrived yet, advance time
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        // Set response time when process first gets CPU
        processes[i].response_time = current_time - processes[i].arrival_time;
        processes[i].started = true;
        
        // Execute the process (advance time by burst time)
        current_time += processes[i].burst_time;
        
        // Set completion time
        processes[i].completion_time = current_time;
        processes[i].remaining_time = 0;
    }
    
    // Calculate and return metrics
    return calculate_metrics(processes, n);
}