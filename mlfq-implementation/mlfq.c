#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUE_LEVELS 3

// Time quantums for each queue level
const int time_quantum[MAX_QUEUE_LEVELS] = {2, 4, -1}; // -1 for FCFS (no preemption)

// Process States
typedef enum {
    READY,
    RUNNING,
    FINISHED
} ProcessState;

// Process Control Block (PCB)
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int current_queue; // 0 = high priority
    ProcessState state;
} Process;

// Queue structure for each level
typedef struct {
    Process* queue[MAX_PROCESSES];
    int front, rear;
} ProcessQueue;

// Global list of processes
Process process_list[MAX_PROCESSES];
int total_processes = 0;

// MLFQ Queues
ProcessQueue mlfq[MAX_QUEUE_LEVELS];

// ------------------ Function Prototypes ------------------

void init_queues();
void enqueue(ProcessQueue* q, Process* p);
Process* dequeue(ProcessQueue* q);
int is_empty(ProcessQueue* q);

void add_process(int pid, int arrival_time, int burst_time);
void simulate_mlfq();

void run_process(Process* p, int queue_level, int current_time);
void check_arrivals(int current_time);

// ----------------------------------------------------------

