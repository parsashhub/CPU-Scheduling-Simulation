/**
 * @file common.h
 * @brief Common structures and functions for CPU scheduling algorithms
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @struct Process
 * @brief Structure to represent a process with its attributes
 */
typedef struct {
    char id[10];         /**< Process identifier */
    int arrival_time;    /**< Time at which process arrives */
    int burst_time;      /**< CPU time required by the process */
    int priority;        /**< Priority of the process (lower value means higher priority) */
    
    /* Fields used for calculating metrics */
    int remaining_time;  /**< Remaining burst time */
    int completion_time; /**< Time at which process completes execution */
    int turnaround_time; /**< Time difference between completion time and arrival time */
    int waiting_time;    /**< Time difference between turnaround time and burst time */
    int response_time;   /**< Time at which the process first gets the CPU */
    bool started;        /**< Flag to check if process has started execution */
} Process;

/**
 * @struct Metrics
 * @brief Structure to store performance metrics of scheduling algorithms
 */
typedef struct {
    float avg_turnaround_time; /**< Average turnaround time */
    float avg_waiting_time;    /**< Average waiting time */
    float avg_response_time;   /**< Average response time */
} Metrics;

/**
 * @brief Reads process data from a CSV file
 * @param filename Name of the CSV file
 * @param processes Array to store the processes
 * @return Number of processes read
 */
int read_processes(const char* filename, Process** processes);

/**
 * @brief Prints the details of all processes
 * @param processes Array of processes
 * @param n Number of processes
 */
void print_processes(Process* processes, int n);

/**
 * @brief Prints the metrics of a scheduling algorithm
 * @param metrics Metrics structure containing the performance metrics
 * @param algorithm_name Name of the scheduling algorithm
 */
void print_metrics(Metrics metrics, const char* algorithm_name);

/**
 * @brief Calculates performance metrics for the given processes
 * @param processes Array of processes
 * @param n Number of processes
 * @return Metrics structure containing the calculated metrics
 */
Metrics calculate_metrics(Process* processes, int n);

/**
 * @brief Creates a deep copy of the processes array
 * @param src Source array of processes
 * @param n Number of processes
 * @return Newly allocated array with copied processes
 */
Process* copy_processes(Process* src, int n);

#endif /* COMMON_H */