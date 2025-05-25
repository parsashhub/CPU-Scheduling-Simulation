/**
 * @file rr.c
 * @brief Implementation of Round Robin (RR) CPU scheduling algorithm
 */

#include "rr.h"

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
 * @brief Simple queue implementation for Round Robin scheduling
 */
typedef struct {
    int* data;      /**< Array to store process indices */
    int capacity;   /**< Maximum capacity of the queue */
    int size;       /**< Current size of the queue */
    int front;      /**< Front index of the queue */
    int rear;       /**< Rear index of the queue */
} Queue;

/**
 * @brief Initializes a new queue
 * @param capacity Maximum capacity of the queue
 * @return Initialized queue
 */
static Queue* create_queue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        perror("Memory allocation failed");
        return NULL;
    }
    
    queue->data = (int*)malloc(capacity * sizeof(int));
    if (!queue->data) {
        perror("Memory allocation failed");
        free(queue);
        return NULL;
    }
    
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    
    return queue;
}

/**
 * @brief Checks if the queue is empty
 * @param queue Queue to check
 * @return true if empty, false otherwise
 */
static bool is_empty(Queue* queue) {
    return queue->size == 0;
}

/**
 * @brief Checks if the queue is full
 * @param queue Queue to check
 * @return true if full, false otherwise
 */
static bool is_full(Queue* queue) {
    return queue->size == queue->capacity;
}

/**
 * @brief Adds an element to the rear of the queue
 * @param queue Queue to add to
 * @param value Value to add
 * @return true if successful, false if queue is full
 */
static bool enqueue(Queue* queue, int value) {
    if (is_full(queue)) {
        return false;
    }
    
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->data[queue->rear] = value;
    queue->size++;
    
    return true;
}

/**
 * @brief Removes and returns the element at the front of the queue
 * @param queue Queue to remove from
 * @param value Pointer to store the removed value
 * @return true if successful, false if queue is empty
 */
static bool dequeue(Queue* queue, int* value) {
    if (is_empty(queue)) {
        return false;
    }
    
    *value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    return true;
}

/**
 * @brief Frees the memory allocated for the queue
 * @param queue Queue to free
 */
static void free_queue(Queue* queue) {
    if (queue) {
        free(queue->data);
        free(queue);
    }
}

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
Metrics rr_schedule(Process* processes, int n, int time_quantum) {
    // Sort processes by arrival time initially
    qsort(processes, n, sizeof(Process), compare_arrival_time);
    
    // Create a queue for ready processes
    Queue* ready_queue = create_queue(n * 100);  // Large enough to handle all possible enqueues
    if (!ready_queue) {
        Metrics empty = {0};
        return empty;
    }
    
    int current_time = 0;
    int completed = 0;
    int* arrival_index = (int*)calloc(n, sizeof(int));
    
    if (!arrival_index) {
        perror("Memory allocation failed");
        free_queue(ready_queue);
        Metrics empty = {0};
        return empty;
    }
    
    // Initialize arrival index array
    for (int i = 0; i < n; i++) {
        arrival_index[i] = i;
    }
    
    int next_arrival_idx = 0;
    
    // Continue until all processes are completed
    while (completed < n) {
        // Check for newly arrived processes and add them to the ready queue
        while (next_arrival_idx < n && processes[next_arrival_idx].arrival_time <= current_time) {
            enqueue(ready_queue, next_arrival_idx);
            next_arrival_idx++;
        }
        
        // If ready queue is empty, advance time to the next arrival
        if (is_empty(ready_queue)) {
            if (next_arrival_idx < n) {
                current_time = processes[next_arrival_idx].arrival_time;
                continue;
            } else {
                // No more processes to arrive, but this shouldn't happen if we're not done
                break;
            }
        }
        
        // Get the next process from the ready queue
        int process_idx;
        dequeue(ready_queue, &process_idx);
        Process* p = &processes[process_idx];
        
        // Set response time when process first gets CPU
        if (!p->started) {
            p->response_time = current_time - p->arrival_time;
            p->started = true;
        }
        
        // Determine how long this process will run
        int execution_time = (p->remaining_time < time_quantum) ? p->remaining_time : time_quantum;
        
        // Execute the process for the determined time
        p->remaining_time -= execution_time;
        current_time += execution_time;
        
        // Check for newly arrived processes during this execution and add them to the ready queue
        while (next_arrival_idx < n && processes[next_arrival_idx].arrival_time <= current_time) {
            enqueue(ready_queue, next_arrival_idx);
            next_arrival_idx++;
        }
        
        // Check if the process is completed
        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            completed++;
        } else {
            // Process still has remaining time, add it back to the ready queue
            enqueue(ready_queue, process_idx);
        }
    }
    
    free(arrival_index);
    free_queue(ready_queue);
    
    // Calculate and return metrics
    return calculate_metrics(processes, n);
}