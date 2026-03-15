#include "../include/scheduler.h"
#include "../include/logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

Scheduler* scheduler_create(int cpu, int memory, int storage, SchedulingPolicy policy) {
    Scheduler* scheduler = (Scheduler*)malloc(sizeof(Scheduler));
    if (!scheduler) return NULL;
    
    scheduler->ready_queue = vm_queue_create(100);
    scheduler->completed_queue = vm_queue_create(100);
    scheduler->resources = resources_create(cpu, memory, storage);
    scheduler->policy = policy;
    scheduler->total_context_switches = 0;
    scheduler->avg_waiting_time = 0.0;
    scheduler->avg_turnaround_time = 0.0;
    
    return scheduler;
}

VirtualMachine* scheduler_select_next_vm(Scheduler* scheduler) {
    if (scheduler->ready_queue->count == 0) return NULL;
    
    int selected_index = 0;
    
    if (scheduler->policy == PRIORITY_SCHEDULING) {
        // Select VM with highest priority (lower number = higher priority)
        for (int i = 1; i < scheduler->ready_queue->count; i++) {
            if (scheduler->ready_queue->vms[i]->priority < 
                scheduler->ready_queue->vms[selected_index]->priority) {
                selected_index = i;
            }
        }
    } else if (scheduler->policy == SJF_SCHEDULING) {
        // Select VM with shortest burst time
        for (int i = 1; i < scheduler->ready_queue->count; i++) {
            if (scheduler->ready_queue->vms[i]->burst_time < 
                scheduler->ready_queue->vms[selected_index]->burst_time) {
                selected_index = i;
            }
        }
    }
    
    return vm_queue_remove(scheduler->ready_queue, selected_index);
}

int scheduler_allocate_resources(Scheduler* scheduler, VirtualMachine* vm) {
    if (resources_allocate(scheduler->resources, vm->cpu_required, 
                          vm->memory_required, vm->storage_required)) {
        vm->state = VM_RUNNING;
        vm->start_time = time(NULL);
        return 1;
    }
    
    vm->state = VM_BLOCKED;
    vm->waiting_for_resources = 1;
    return 0;
}

void scheduler_deallocate_resources(Scheduler* scheduler, VirtualMachine* vm) {
    resources_deallocate(scheduler->resources, vm->cpu_required, 
                        vm->memory_required, vm->storage_required);
}

void scheduler_add_vm(Scheduler* scheduler, VirtualMachine* vm) {
    vm_queue_add(scheduler->ready_queue, vm);
}

void scheduler_print_statistics(Scheduler* scheduler) {
    printf("\n========== SCHEDULER STATISTICS ==========\n");
    printf("Scheduling Policy: %s\n", 
           scheduler->policy == PRIORITY_SCHEDULING ? "PRIORITY" : "SJF");
    printf("Total VMs Completed: %d\n", scheduler->completed_queue->count);
    printf("Total Context Switches: %d\n", scheduler->total_context_switches);
    
    if (scheduler->completed_queue->count > 0) {
        printf("Average Waiting Time: %.2f seconds\n", scheduler->avg_waiting_time);
        printf("Average Turnaround Time: %.2f seconds\n", scheduler->avg_turnaround_time);
    }
    
    printf("==========================================\n\n");
}

void scheduler_free(Scheduler* scheduler) {
    if (scheduler) {
        vm_queue_free(scheduler->ready_queue);
        vm_queue_free(scheduler->completed_queue);
        resources_free(scheduler->resources);
        free(scheduler);
    }
}

void scheduler_calculate_metrics(Scheduler* scheduler) {
    if (scheduler->completed_queue->count == 0) return;
    
    double total_waiting_time = 0.0;
    double total_turnaround_time = 0.0;
    
    for (int i = 0; i < scheduler->completed_queue->count; i++) {
        VirtualMachine* vm = scheduler->completed_queue->vms[i];
        double waiting_time = (double)(vm->start_time - vm->arrival_time);
        double turnaround_time = (double)(vm->completion_time - vm->arrival_time);
        
        total_waiting_time += waiting_time;
        total_turnaround_time += turnaround_time;
    }
    
    scheduler->avg_waiting_time = total_waiting_time / scheduler->completed_queue->count;
    scheduler->avg_turnaround_time = total_turnaround_time / scheduler->completed_queue->count;
}
