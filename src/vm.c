#include "../include/vm.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

VirtualMachine* vm_create(int vm_id, int cpu, int memory, int storage, int priority, int burst_time) {
    VirtualMachine* vm = (VirtualMachine*)malloc(sizeof(VirtualMachine));
    if (!vm) return NULL;
    
    vm->vm_id = vm_id;
    vm->cpu_required = cpu;
    vm->memory_required = memory;
    vm->storage_required = storage;
    vm->priority = priority;
    vm->burst_time = burst_time;
    vm->remaining_time = burst_time;
    vm->arrival_time = time(NULL);
    vm->start_time = 0;
    vm->completion_time = 0;
    vm->state = VM_READY;
    vm->waiting_for_resources = 0;
    
    return vm;
}

void vm_free(VirtualMachine* vm) {
    if (vm) {
        free(vm);
    }
}

VMQueue* vm_queue_create(int capacity) {
    VMQueue* queue = (VMQueue*)malloc(sizeof(VMQueue));
    if (!queue) return NULL;
    
    queue->vms = (VirtualMachine**)malloc(sizeof(VirtualMachine*) * capacity);
    if (!queue->vms) {
        free(queue);
        return NULL;
    }
    
    queue->count = 0;
    queue->capacity = capacity;
    return queue;
}

void vm_queue_add(VMQueue* queue, VirtualMachine* vm) {
    if (queue->count < queue->capacity) {
        queue->vms[queue->count++] = vm;
    }
}

VirtualMachine* vm_queue_remove(VMQueue* queue, int index) {
    if (index < 0 || index >= queue->count) return NULL;
    
    VirtualMachine* vm = queue->vms[index];
    
    for (int i = index; i < queue->count - 1; i++) {
        queue->vms[i] = queue->vms[i + 1];
    }
    
    queue->count--;
    return vm;
}

void vm_queue_free(VMQueue* queue) {
    if (queue) {
        for (int i = 0; i < queue->count; i++) {
            vm_free(queue->vms[i]);
        }
        free(queue->vms);
        free(queue);
    }
}

void vm_print_info(VirtualMachine* vm) {
    if (!vm) return;
    
    printf("[VM %d] State: ", vm->vm_id);
    switch (vm->state) {
        case VM_READY: printf("READY"); break;
        case VM_RUNNING: printf("RUNNING"); break;
        case VM_BLOCKED: printf("BLOCKED"); break;
        case VM_COMPLETED: printf("COMPLETED"); break;
    }
    printf(" | CPU: %d | Memory: %d | Storage: %d | Priority: %d | Burst: %d\n",
           vm->cpu_required, vm->memory_required, vm->storage_required, 
           vm->priority, vm->burst_time);
}
