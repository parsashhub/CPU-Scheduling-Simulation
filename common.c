/**
 * @file common.c
 * @brief Implementation of common functions for CPU scheduling algorithms
 */

#include "common.h"

/**
 * @brief Reads process data from a CSV file
 * @param filename Name of the CSV file
 * @param processes Pointer to array to store the processes
 * @return Number of processes read
 */
int read_processes(const char* filename, Process** processes) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }
    
    // Count the number of lines (processes) in the file
    int count = 0;
    char buffer[256];
    
    // Skip header line
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }
    
    // Allocate memory for processes
    *processes = (Process*)malloc(count * sizeof(Process));
    if (!*processes) {
        perror("Memory allocation failed");
        fclose(file);
        return -1;
    }
    
    // Reset file pointer to beginning
    rewind(file);
    
    // Skip header line again
    fgets(buffer, sizeof(buffer), file);
    
    // Read process data
    int i = 0;
    while (i < count && fgets(buffer, sizeof(buffer), file)) {
        Process* p = &(*processes)[i];
        
        // Parse CSV line
        char* token = strtok(buffer, ",");
        if (token) {
            strncpy(p->id, token, sizeof(p->id) - 1);
            p->id[sizeof(p->id) - 1] = '\0';
        }
        
        token = strtok(NULL, ",");
        if (token) p->arrival_time = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) p->burst_time = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) p->priority = atoi(token);
        
        // Initialize other fields
        p->remaining_time = p->burst_time;
        p->completion_time = 0;
        p->turnaround_time = 0;
        p->waiting_time = 0;
        p->response_time = -1;  // -1 indicates not started yet
        p->started = false;
        
        i++;
    }
    
    fclose(file);
    return count;
}

/**
 * @brief Prints the details of all processes
 * @param processes Array of processes
 * @param n Number of processes
 */
void print_processes(Process* processes, int n) {
    printf("\n%-10s %-12s %-10s %-10s %-15s %-15s %-15s\n", 
           "Process", "Arrival", "Burst", "Priority", "Completion", "Turnaround", "Waiting");
    printf("----------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        Process p = processes[i];
        printf("%-10s %-12d %-10d %-10d %-15d %-15d %-15d\n", 
               p.id, p.arrival_time, p.burst_time, p.priority, 
               p.completion_time, p.turnaround_time, p.waiting_time);
    }
    printf("----------------------------------------------------------------------------------\n");
}

/**
 * @brief Prints the metrics of a scheduling algorithm
 * @param metrics Metrics structure containing the performance metrics
 * @param algorithm_name Name of the scheduling algorithm
 */
void print_metrics(Metrics metrics, const char* algorithm_name) {
    printf("\n%s Scheduling Algorithm Metrics:\n", algorithm_name);
    printf("Average Turnaround Time: %.2f\n", metrics.avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", metrics.avg_waiting_time);
    printf("Average Response Time: %.2f\n", metrics.avg_response_time);
    printf("----------------------------------------------------------------------------------\n");
}

/**
 * @brief Calculates performance metrics for the given processes
 * @param processes Array of processes
 * @param n Number of processes
 * @return Metrics structure containing the calculated metrics
 */
Metrics calculate_metrics(Process* processes, int n) {
    Metrics metrics = {0};
    float total_turnaround = 0, total_waiting = 0, total_response = 0;
    
    for (int i = 0; i < n; i++) {
        // Calculate turnaround time (completion time - arrival time)
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        
        // Calculate waiting time (turnaround time - burst time)
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        
        // Accumulate for averages
        total_turnaround += processes[i].turnaround_time;
        total_waiting += processes[i].waiting_time;
        total_response += processes[i].response_time;
    }
    
    // Calculate averages
    metrics.avg_turnaround_time = total_turnaround / n;
    metrics.avg_waiting_time = total_waiting / n;
    metrics.avg_response_time = total_response / n;
    
    return metrics;
}

/**
 * @brief Creates a deep copy of the processes array
 * @param src Source array of processes
 * @param n Number of processes
 * @return Newly allocated array with copied processes
 */
Process* copy_processes(Process* src, int n) {
    Process* dest = (Process*)malloc(n * sizeof(Process));
    if (!dest) {
        perror("Memory allocation failed");
        return NULL;
    }
    
    memcpy(dest, src, n * sizeof(Process));
    return dest;
}