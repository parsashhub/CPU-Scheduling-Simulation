/**
 * @file fcfs.h
 * @brief First-Come-First-Serve (FCFS) CPU scheduling algorithm
 */

#ifndef FCFS_H
#define FCFS_H

#include "common.h"

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
Metrics fcfs_schedule(Process* processes, int n);

#endif /* FCFS_H */