#ifndef RESOURCES_H
#define RESOURCES_H

#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int total_cpu;
    int available_cpu;
    int total_memory;
    int available_memory;
    int total_storage;
    int available_storage;
    pthread_mutex_t resource_lock;
} ResourcePool;

typedef struct {
    int allocated_cpu;
    int allocated_memory;
    int allocated_storage;
} ResourceAllocation;

ResourcePool* resources_create(int cpu, int memory, int storage);
int resources_allocate(ResourcePool* pool, int cpu, int memory, int storage);
void resources_deallocate(ResourcePool* pool, int cpu, int memory, int storage);
void resources_print_status(ResourcePool* pool);
void resources_free(ResourcePool* pool);
int resources_available(ResourcePool* pool, int cpu, int memory, int storage);
float resources_utilization_percentage(ResourcePool* pool);

#endif
