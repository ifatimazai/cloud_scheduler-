#include "../include/deadlock.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DeadlockDetector* deadlock_detector_create(int max_vms) {
    DeadlockDetector* detector = (DeadlockDetector*)malloc(sizeof(DeadlockDetector));
    if (!detector) return NULL;
    
    detector->requests = (ResourceRequest*)malloc(sizeof(ResourceRequest) * max_vms);
    if (!detector->requests) {
        free(detector);
        return NULL;
    }
    
    detector->count = 0;
    pthread_mutex_init(&detector->lock, NULL);
    
    return detector;
}

void deadlock_detector_add_request(DeadlockDetector* detector, int vm_id,
                                   int cpu_held, int memory_held, int storage_held,
                                   int cpu_waiting, int memory_waiting, int storage_waiting) {
    pthread_mutex_lock(&detector->lock);
    
    // Check if VM already exists
    for (int i = 0; i < detector->count; i++) {
        if (detector->requests[i].vm_id == vm_id) {
            detector->requests[i].cpu_held = cpu_held;
            detector->requests[i].memory_held = memory_held;
            detector->requests[i].storage_held = storage_held;
            detector->requests[i].cpu_waiting = cpu_waiting;
            detector->requests[i].memory_waiting = memory_waiting;
            detector->requests[i].storage_waiting = storage_waiting;
            pthread_mutex_unlock(&detector->lock);
            return;
        }
    }
    
    // Add new request
    detector->requests[detector->count].vm_id = vm_id;
    detector->requests[detector->count].cpu_held = cpu_held;
    detector->requests[detector->count].memory_held = memory_held;
    detector->requests[detector->count].storage_held = storage_held;
    detector->requests[detector->count].cpu_waiting = cpu_waiting;
    detector->requests[detector->count].memory_waiting = memory_waiting;
    detector->requests[detector->count].storage_waiting = storage_waiting;
    detector->count++;
    
    pthread_mutex_unlock(&detector->lock);
}

void deadlock_detector_remove_request(DeadlockDetector* detector, int vm_id) {
    pthread_mutex_lock(&detector->lock);
    
    for (int i = 0; i < detector->count; i++) {
        if (detector->requests[i].vm_id == vm_id) {
            for (int j = i; j < detector->count - 1; j++) {
                detector->requests[j] = detector->requests[j + 1];
            }
            detector->count--;
            break;
        }
    }
    
    pthread_mutex_unlock(&detector->lock);
}

// Check for circular wait using DFS
int deadlock_detector_check_cycle(DeadlockDetector* detector) {
    pthread_mutex_lock(&detector->lock);
    
    if (detector->count == 0) {
        pthread_mutex_unlock(&detector->lock);
        return 0;
    }
    
    // Simplified cycle detection: check if any VM is waiting for resources held by another VM
    for (int i = 0; i < detector->count; i++) {
        if ((detector->requests[i].cpu_waiting > 0 || 
             detector->requests[i].memory_waiting > 0 || 
             detector->requests[i].storage_waiting > 0)) {
            
            // Check if any other VM holds those resources
            for (int j = 0; j < detector->count; j++) {
                if (i != j && 
                    ((detector->requests[j].cpu_held > 0 && detector->requests[i].cpu_waiting > 0) ||
                     (detector->requests[j].memory_held > 0 && detector->requests[i].memory_waiting > 0) ||
                     (detector->requests[j].storage_held > 0 && detector->requests[i].storage_waiting > 0))) {
                    
                    pthread_mutex_unlock(&detector->lock);
                    return 1; // Potential deadlock detected
                }
            }
        }
    }
    
    pthread_mutex_unlock(&detector->lock);
    return 0;
}

// Banker's Algorithm for deadlock prevention
int deadlock_prevent_banker_algorithm(DeadlockDetector* detector, ResourcePool* pool, VirtualMachine* vm) {
    pthread_mutex_lock(&detector->lock);
    
    // Check if granting resources to this VM would be safe
    // A state is safe if there exists a sequence of VMs that can complete
    
    int temp_cpu = pool->available_cpu - vm->cpu_required;
    int temp_memory = pool->available_memory - vm->memory_required;
    int temp_storage = pool->available_storage - vm->storage_required;
    
    // Simple heuristic: ensure at least one other VM can complete with remaining resources
    if (detector->count > 1) {
        int can_complete = 0;
        for (int i = 0; i < detector->count; i++) {
            if (detector->requests[i].vm_id != vm->vm_id) {
                if (temp_cpu >= detector->requests[i].cpu_waiting &&
                    temp_memory >= detector->requests[i].memory_waiting &&
                    temp_storage >= detector->requests[i].storage_waiting) {
                    can_complete = 1;
                    break;
                }
            }
        }
        
        pthread_mutex_unlock(&detector->lock);
        return can_complete;
    }
    
    pthread_mutex_unlock(&detector->lock);
    return 1; // Safe to allocate
}

void deadlock_detector_print_status(DeadlockDetector* detector) {
    pthread_mutex_lock(&detector->lock);
    
    printf("\n========== DEADLOCK DETECTOR STATUS ==========\n");
    printf("Total VMs being tracked: %d\n", detector->count);
    
    if (detector->count > 0) {
        printf("\nVM Resource Status:\n");
        for (int i = 0; i < detector->count; i++) {
            printf("  VM %d: CPU(held:%d, waiting:%d), Memory(held:%d, waiting:%d), Storage(held:%d, waiting:%d)\n",
                   detector->requests[i].vm_id,
                   detector->requests[i].cpu_held, detector->requests[i].cpu_waiting,
                   detector->requests[i].memory_held, detector->requests[i].memory_waiting,
                   detector->requests[i].storage_held, detector->requests[i].storage_waiting);
        }
    }
    
    printf("==============================================\n\n");
    
    pthread_mutex_unlock(&detector->lock);
}

void deadlock_detector_free(DeadlockDetector* detector) {
    if (detector) {
        pthread_mutex_destroy(&detector->lock);
        free(detector->requests);
        free(detector);
    }
}
