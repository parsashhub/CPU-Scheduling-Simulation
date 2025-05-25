/**
 * @file rr.h
 * @brief Round Robin (RR) CPU scheduling algorithm
 */

#ifndef RR_H
#define RR_H

#include "common.h"

/**
 * @brief Executes the Round Robin (RR) scheduling algorithm
 * 
 * Round Robin is a preemptive scheduling algorithm where each process is assigned a
 * fixed time slice (quantum) in a cyclic way. If a process's remaining burst time
 * exceeds the time quantum, the process is preempted and added to the end of the
 * ready queue.
 * 
 * @param processes Array of processes
 * @param n Number of processes
 * @param time_quantum Time slice allocated to each process
 * @return Metrics structure containing the performance metrics
 */
Metrics rr_schedule(Process* processes, int n, int time_quantum);

#endif /* RR_H */