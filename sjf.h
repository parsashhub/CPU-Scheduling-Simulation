/**
 * @file sjf.h
 * @brief Shortest Job First (SJF) CPU scheduling algorithm
 */

#ifndef SJF_H
#define SJF_H

#include "common.h"

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
Metrics sjf_non_preemptive_schedule(Process* processes, int n);

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
Metrics sjf_preemptive_schedule(Process* processes, int n);

#endif /* SJF_H */