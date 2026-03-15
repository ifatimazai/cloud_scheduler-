#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../include/vm.h"
#include "../include/resources.h"
#include "../include/scheduler.h"
#include "../include/deadlock.h"
#include "../include/logger.h"

#define MAX_VMS 50
#define TOTAL_CPU 16
#define TOTAL_MEMORY 32
#define TOTAL_STORAGE 100
#define SIMULATION_TIME 30  // seconds

typedef struct {
    Scheduler* scheduler;
    DeadlockDetector* detector;
    int total_vms_created;
    int simulation_running;
    pthread_mutex_t sim_lock;
} SimulationContext;

SimulationContext sim_context;

// Thread function to simulate VM execution
void* vm_execution_thread(void* arg) {
    VirtualMachine* vm = (VirtualMachine*)arg;
    
    char log_msg[256];
    sprintf(log_msg, "VM %d started execution (CPU: %d, Memory: %d, Storage: %d)",
            vm->vm_id, vm->cpu_required, vm->memory_required, vm->storage_required);
    logger_log(LOG_INFO, log_msg);
    
    // Simulate execution
    sleep(vm->burst_time / 1000);  // Convert milliseconds to seconds
    
    vm->state = VM_COMPLETED;
    vm->completion_time = time(NULL);
    
    sprintf(log_msg, "VM %d completed execution", vm->vm_id);
    logger_log(LOG_SUCCESS, log_msg);
    
    return NULL;
}

// Thread function to handle VM arrivals
void* vm_arrival_thread(void* arg) {
    srand(time(NULL));
    
    while (sim_context.simulation_running) {
        sleep(1 + rand() % 3);  // VMs arrive randomly every 1-3 seconds
        
        pthread_mutex_lock(&sim_context.sim_lock);
        
        if (sim_context.total_vms_created < MAX_VMS && sim_context.simulation_running) {
            int vm_id = sim_context.total_vms_created++;
            int cpu = 1 + rand() % 4;
            int memory = 2 + rand() % 6;
            int storage = 5 + rand() % 15;
            int priority = rand() % 5;
            int burst_time = 1000 + rand() % 4000;  // 1-5 seconds in milliseconds
            
            VirtualMachine* vm = vm_create(vm_id, cpu, memory, storage, priority, burst_time);
            scheduler_add_vm(sim_context.scheduler, vm);
            
            char log_msg[256];
            sprintf(log_msg, "VM %d arrived (Priority: %d, Burst: %dms)", 
                    vm_id, priority, burst_time);
            logger_log(LOG_INFO, log_msg);
            
            // Update deadlock detector
            deadlock_detector_add_request(sim_context.detector, vm_id, 0, 0, 0, 
                                         cpu, memory, storage);
        }
        
        pthread_mutex_unlock(&sim_context.sim_lock);
    }
    
    return NULL;
}

// Thread function for scheduler
void* scheduler_thread(void* arg) {
    while (sim_context.simulation_running) {
        pthread_mutex_lock(&sim_context.sim_lock);
        
        if (sim_context.scheduler->ready_queue->count > 0) {
            VirtualMachine* vm = scheduler_select_next_vm(sim_context.scheduler);
            
            if (vm) {
                // Check for deadlock
                if (deadlock_detector_check_cycle(sim_context.detector)) {
                    char log_msg[256];
                    sprintf(log_msg, "WARNING: Potential deadlock detected with VM %d", vm->vm_id);
                    logger_log(LOG_WARNING, log_msg);
                    
                    // Try to allocate using Banker's algorithm
                    if (!deadlock_prevent_banker_algorithm(sim_context.detector, 
                                                          sim_context.scheduler->resources, vm)) {
                        sprintf(log_msg, "Deadlock prevention: VM %d allocation denied", vm->vm_id);
                        logger_log(LOG_WARNING, log_msg);
                        scheduler_add_vm(sim_context.scheduler, vm);
                        pthread_mutex_unlock(&sim_context.sim_lock);
                        usleep(100000);
                        continue;
                    }
                }
                
                // Try to allocate resources
                if (scheduler_allocate_resources(sim_context.scheduler, vm)) {
                    char log_msg[256];
                    sprintf(log_msg, "VM %d scheduled for execution", vm->vm_id);
                    logger_log(LOG_SUCCESS, log_msg);
                    
                    // Create execution thread
                    pthread_t exec_thread;
                    pthread_create(&exec_thread, NULL, vm_execution_thread, vm);
                    pthread_detach(exec_thread);
                    
                    // Add to completed queue after execution
                    vm_queue_add(sim_context.scheduler->completed_queue, vm);
                    scheduler_deallocate_resources(sim_context.scheduler, vm);
                    
                    sim_context.scheduler->total_context_switches++;
                    
                    // Update deadlock detector
                    deadlock_detector_remove_request(sim_context.detector, vm->vm_id);
                } else {
                    char log_msg[256];
                    sprintf(log_msg, "VM %d waiting for resources", vm->vm_id);
                    logger_log(LOG_WARNING, log_msg);
                    
                    // Put back in queue
                    scheduler_add_vm(sim_context.scheduler, vm);
                }
            }
        }
        
        pthread_mutex_unlock(&sim_context.sim_lock);
        usleep(500000);  // 500ms
    }
    
    return NULL;
}

// Thread function for monitoring
void* monitoring_thread(void* arg) {
    while (sim_context.simulation_running) {
        sleep(5);  // Print status every 5 seconds
        
        pthread_mutex_lock(&sim_context.sim_lock);
        
        printf("\n\n");
        printf("╔════════════════════════════════════════╗\n");
        printf("║    SIMULATION STATUS (5 Second Update)  ║\n");
        printf("╚════════════════════════════════════════╝\n");
        
        resources_print_status(sim_context.scheduler->resources);
        
        printf("Ready Queue: %d VMs\n", sim_context.scheduler->ready_queue->count);
        printf("Completed: %d VMs\n", sim_context.scheduler->completed_queue->count);
        printf("Total Created: %d VMs\n", sim_context.total_vms_created);
        
        if (deadlock_detector_check_cycle(sim_context.detector)) {
            printf("⚠️  WARNING: Deadlock condition detected!\n");
        }
        
        deadlock_detector_print_status(sim_context.detector);
        
        pthread_mutex_unlock(&sim_context.sim_lock);
    }
    
    return NULL;
}

int main(int argc, char* argv[]) {
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  SHARED CLOUD RESOURCE SCHEDULER SIMULATOR            ║\n");
    printf("║  Demonstrating OS Concepts: Scheduling, Synchronization ║\n");
    printf("║  and Deadlock Prevention in Cloud Computing            ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    // Initialize logger
    logger_init("simulation_log.txt");
    logger_log(LOG_INFO, "Simulation started");
    
    // Initialize simulation context
    sim_context.total_vms_created = 0;
    sim_context.simulation_running = 1;
    pthread_mutex_init(&sim_context.sim_lock, NULL);
    
    // Determine scheduling policy
    SchedulingPolicy policy = PRIORITY_SCHEDULING;
    if (argc > 1 && strcmp(argv[1], "sjf") == 0) {
        policy = SJF_SCHEDULING;
        logger_log(LOG_INFO, "Using SJF (Shortest Job First) scheduling");
        printf("\n[Mode] Shortest Job First (SJF) Scheduling\n");
    } else {
        logger_log(LOG_INFO, "Using Priority scheduling");
        printf("\n[Mode] Priority Scheduling\n");
    }
    
    // Create scheduler
    sim_context.scheduler = scheduler_create(TOTAL_CPU, TOTAL_MEMORY, TOTAL_STORAGE, policy);
    if (!sim_context.scheduler) {
        logger_log(LOG_ERROR, "Failed to create scheduler");
        return 1;
    }
    
    // Create deadlock detector
    sim_context.detector = deadlock_detector_create(MAX_VMS);
    if (!sim_context.detector) {
        logger_log(LOG_ERROR, "Failed to create deadlock detector");
        return 1;
    }
    
    printf("\n[System Resources]\n");
    printf("  CPU Cores: %d\n", TOTAL_CPU);
    printf("  Memory: %d MB\n", TOTAL_MEMORY);
    printf("  Storage: %d GB\n\n", TOTAL_STORAGE);
    
    // Create simulation threads
    pthread_t arrival_tid, scheduler_tid, monitor_tid;
    
    pthread_create(&arrival_tid, NULL, vm_arrival_thread, NULL);
    pthread_create(&scheduler_tid, NULL, scheduler_thread, NULL);
    pthread_create(&monitor_tid, NULL, monitoring_thread, NULL);
    
    printf("Simulation running for %d seconds...\n", SIMULATION_TIME);
    printf("Press Ctrl+C to stop\n\n");
    
    // Run simulation for specified time
    sleep(SIMULATION_TIME);
    
    // Stop simulation
    printf("\n\nStopping simulation...\n");
    sim_context.simulation_running = 0;
    
    // Wait for threads
    pthread_join(arrival_tid, NULL);
    pthread_join(scheduler_tid, NULL);
    pthread_join(monitor_tid, NULL);
    
    sleep(2);  // Allow remaining VMs to complete
    
    // Print final statistics
    scheduler_calculate_metrics(sim_context.scheduler);
    
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║              SIMULATION COMPLETE                        ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    scheduler_print_statistics(sim_context.scheduler);
    resources_print_status(sim_context.scheduler->resources);
    
    printf("\nLog file saved to: simulation_log.txt\n");
    
    logger_log(LOG_SUCCESS, "Simulation completed successfully");
    logger_close();
    
    // Cleanup
    scheduler_free(sim_context.scheduler);
    deadlock_detector_free(sim_context.detector);
    pthread_mutex_destroy(&sim_context.sim_lock);
    
    return 0;
}
