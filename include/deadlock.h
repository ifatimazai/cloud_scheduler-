#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "vm.h"
#include "resources.h"

typedef struct {
    int vm_id;
    int cpu_held;
    int memory_held;
    int storage_held;
    int cpu_waiting;
    int memory_waiting;
    int storage_waiting;
} ResourceRequest;

typedef struct {
    ResourceRequest *requests;
    int count;
    pthread_mutex_t lock;
} DeadlockDetector;

DeadlockDetector* deadlock_detector_create(int max_vms);
void deadlock_detector_add_request(DeadlockDetector* detector, int vm_id, 
                                   int cpu_held, int memory_held, int storage_held,
                                   int cpu_waiting, int memory_waiting, int storage_waiting);
void deadlock_detector_remove_request(DeadlockDetector* detector, int vm_id);
int deadlock_detector_check_cycle(DeadlockDetector* detector);
int deadlock_prevent_banker_algorithm(DeadlockDetector* detector, ResourcePool* pool, VirtualMachine* vm);
void deadlock_detector_print_status(DeadlockDetector* detector);
void deadlock_detector_free(DeadlockDetector* detector);

#endif
