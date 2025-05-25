/**
 * @file main.c
 * @brief Main program for CPU scheduling algorithm simulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "common.h"
#include "fcfs.h"
#include "sjf.h"
#include "rr.h"

/**
 * @brief Prints usage information
 * @param program_name Name of the program
 */
void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -f <file>       Process data file (default: data/processes.csv)\n");
    printf("  -a <algorithm>  Scheduling algorithm to use:\n");
    printf("                  fcfs - First-Come-First-Serve\n");
    printf("                  sjf  - Shortest Job First (non-preemptive)\n");
    printf("                  srtf - Shortest Remaining Time First (preemptive SJF)\n");
    printf("                  rr   - Round Robin\n");
    printf("                  all  - Run all algorithms (default)\n");
    printf("  -q <quantum>    Time quantum for Round Robin (default: 2)\n");
    printf("  -h              Display this help message\n");
}

/**
 * @brief Main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit status
 */
int main(int argc, char* argv[]) {
    char* filename = "data/processes.csv";
    char* algorithm = "all";
    int time_quantum = 2;
    
    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "f:a:q:h")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 'a':
                algorithm = optarg;
                break;
            case 'q':
                time_quantum = atoi(optarg);
                if (time_quantum <= 0) {
                    fprintf(stderr, "Error: Time quantum must be positive\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    // Read process data from file
    Process* processes = NULL;
    int n = read_processes(filename, &processes);
    
    if (n <= 0) {
        fprintf(stderr, "Error reading processes from file: %s\n", filename);
        return EXIT_FAILURE;
    }
    
    printf("Read %d processes from %s\n", n, filename);
    
    // Create copies of the processes for each algorithm
    Process* fcfs_processes = NULL;
    Process* sjf_processes = NULL;
    Process* srtf_processes = NULL;
    Process* rr_processes = NULL;
    
    // Run the selected algorithm(s)
    if (strcmp(algorithm, "all") == 0 || strcmp(algorithm, "fcfs") == 0) {
        fcfs_processes = copy_processes(processes, n);
        if (!fcfs_processes) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(processes);
            return EXIT_FAILURE;
        }
        
        printf("\nRunning First-Come-First-Serve (FCFS) algorithm...\n");
        Metrics fcfs_metrics = fcfs_schedule(fcfs_processes, n);
        print_processes(fcfs_processes, n);
        print_metrics(fcfs_metrics, "FCFS");
    }
    
    if (strcmp(algorithm, "all") == 0 || strcmp(algorithm, "sjf") == 0) {
        sjf_processes = copy_processes(processes, n);
        if (!sjf_processes) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(processes);
            free(fcfs_processes);
            return EXIT_FAILURE;
        }
        
        printf("\nRunning Shortest Job First (SJF) non-preemptive algorithm...\n");
        Metrics sjf_metrics = sjf_non_preemptive_schedule(sjf_processes, n);
        print_processes(sjf_processes, n);
        print_metrics(sjf_metrics, "SJF (non-preemptive)");
    }
    
    if (strcmp(algorithm, "all") == 0 || strcmp(algorithm, "srtf") == 0) {
        srtf_processes = copy_processes(processes, n);
        if (!srtf_processes) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(processes);
            free(fcfs_processes);
            free(sjf_processes);
            return EXIT_FAILURE;
        }
        
        printf("\nRunning Shortest Remaining Time First (SRTF) preemptive algorithm...\n");
        Metrics srtf_metrics = sjf_preemptive_schedule(srtf_processes, n);
        print_processes(srtf_processes, n);
        print_metrics(srtf_metrics, "SRTF (preemptive SJF)");
    }
    
    if (strcmp(algorithm, "all") == 0 || strcmp(algorithm, "rr") == 0) {
        rr_processes = copy_processes(processes, n);
        if (!rr_processes) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(processes);
            free(fcfs_processes);
            free(sjf_processes);
            free(srtf_processes);
            return EXIT_FAILURE;
        }
        
        printf("\nRunning Round Robin (RR) algorithm with time quantum = %d...\n", time_quantum);
        Metrics rr_metrics = rr_schedule(rr_processes, n, time_quantum);
        print_processes(rr_processes, n);
        print_metrics(rr_metrics, "Round Robin");
    }
    
    // Free allocated memory
    free(processes);
    if (fcfs_processes) free(fcfs_processes);
    if (sjf_processes) free(sjf_processes);
    if (srtf_processes) free(srtf_processes);
    if (rr_processes) free(rr_processes);
    
    return EXIT_SUCCESS;
}