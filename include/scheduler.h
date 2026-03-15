#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "vm.h"
#include "resources.h"

typedef enum {
    PRIORITY_SCHEDULING,
    SJF_SCHEDULING
} SchedulingPolicy;

typedef struct {
    VMQueue *ready_queue;
    VMQueue *completed_queue;
    ResourcePool *resources;
    SchedulingPolicy policy;
    int total_context_switches;
    double avg_waiting_time;
    double avg_turnaround_time;
} Scheduler;

Scheduler* scheduler_create(int cpu, int memory, int storage, SchedulingPolicy policy);
VirtualMachine* scheduler_select_next_vm(Scheduler* scheduler);
int scheduler_allocate_resources(Scheduler* scheduler, VirtualMachine* vm);
void scheduler_deallocate_resources(Scheduler* scheduler, VirtualMachine* vm);
void scheduler_add_vm(Scheduler* scheduler, VirtualMachine* vm);
void scheduler_print_statistics(Scheduler* scheduler);
void scheduler_free(Scheduler* scheduler);
void scheduler_calculate_metrics(Scheduler* scheduler);

#endif
