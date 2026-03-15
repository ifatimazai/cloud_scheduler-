#include "../include/resources.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ResourcePool* resources_create(int cpu, int memory, int storage) {
    ResourcePool* pool = (ResourcePool*)malloc(sizeof(ResourcePool));
    if (!pool) return NULL;
    
    pool->total_cpu = cpu;
    pool->available_cpu = cpu;
    pool->total_memory = memory;
    pool->available_memory = memory;
    pool->total_storage = storage;
    pool->available_storage = storage;
    
    pthread_mutex_init(&pool->resource_lock, NULL);
    
    return pool;
}

int resources_allocate(ResourcePool* pool, int cpu, int memory, int storage) {
    pthread_mutex_lock(&pool->resource_lock);
    
    if (pool->available_cpu >= cpu && 
        pool->available_memory >= memory && 
        pool->available_storage >= storage) {
        
        pool->available_cpu -= cpu;
        pool->available_memory -= memory;
        pool->available_storage -= storage;
        
        pthread_mutex_unlock(&pool->resource_lock);
        return 1;
    }
    
    pthread_mutex_unlock(&pool->resource_lock);
    return 0;
}

void resources_deallocate(ResourcePool* pool, int cpu, int memory, int storage) {
    pthread_mutex_lock(&pool->resource_lock);
    
    pool->available_cpu += cpu;
    pool->available_memory += memory;
    pool->available_storage += storage;
    
    // Ensure we don't exceed total resources (safety check)
    if (pool->available_cpu > pool->total_cpu) pool->available_cpu = pool->total_cpu;
    if (pool->available_memory > pool->total_memory) pool->available_memory = pool->total_memory;
    if (pool->available_storage > pool->total_storage) pool->available_storage = pool->total_storage;
    
    pthread_mutex_unlock(&pool->resource_lock);
}

void resources_print_status(ResourcePool* pool) {
    pthread_mutex_lock(&pool->resource_lock);
    
    printf("\n========== RESOURCE STATUS ==========\n");
    printf("CPU:     %d / %d cores available\n", pool->available_cpu, pool->total_cpu);
    printf("Memory:  %d / %d MB available\n", pool->available_memory, pool->total_memory);
    printf("Storage: %d / %d GB available\n", pool->available_storage, pool->total_storage);
    printf("Overall Utilization: %.2f%%\n", resources_utilization_percentage(pool));
    printf("=====================================\n\n");
    
    pthread_mutex_unlock(&pool->resource_lock);
}

void resources_free(ResourcePool* pool) {
    if (pool) {
        pthread_mutex_destroy(&pool->resource_lock);
        free(pool);
    }
}

int resources_available(ResourcePool* pool, int cpu, int memory, int storage) {
    pthread_mutex_lock(&pool->resource_lock);
    
    int available = (pool->available_cpu >= cpu && 
                    pool->available_memory >= memory && 
                    pool->available_storage >= storage);
    
    pthread_mutex_unlock(&pool->resource_lock);
    return available;
}

float resources_utilization_percentage(ResourcePool* pool) {
    int total_resources = pool->total_cpu + pool->total_memory + pool->total_storage;
    int available_resources = pool->available_cpu + pool->available_memory + pool->available_storage;
    
    if (total_resources == 0) return 0.0;
    
    return ((float)(total_resources - available_resources) / total_resources) * 100.0;
}
