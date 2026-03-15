#ifndef VM_H
#define VM_H

#include <time.h>
#include <semaphore.h>

typedef enum {
    VM_READY,
    VM_RUNNING,
    VM_BLOCKED,
    VM_COMPLETED
} VM_State;

typedef struct {
    int vm_id;
    int cpu_required;
    int memory_required;
    int storage_required;
    int priority;
    int burst_time;
    int remaining_time;
    time_t arrival_time;
    time_t start_time;
    time_t completion_time;
    VM_State state;
    int waiting_for_resources;
} VirtualMachine;

typedef struct {
    VirtualMachine **vms;
    int count;
    int capacity;
} VMQueue;

VirtualMachine* vm_create(int vm_id, int cpu, int memory, int storage, int priority, int burst_time);
void vm_free(VirtualMachine* vm);
VMQueue* vm_queue_create(int capacity);
void vm_queue_add(VMQueue* queue, VirtualMachine* vm);
VirtualMachine* vm_queue_remove(VMQueue* queue, int index);
void vm_queue_free(VMQueue* queue);
void vm_print_info(VirtualMachine* vm);

#endif
