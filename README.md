# Shared Cloud Resource Scheduler Simulator

A comprehensive simulation of a cloud data center resource scheduler demonstrating real-world operating system concepts including multithreading, synchronization, scheduling algorithms, and deadlock prevention.

---

# 📋 Overview

This project simulates a cloud computing environment where:

- **Virtual Machines (VMs)** dynamically arrive and request computational resources  
- A **Resource Scheduler** manages allocation of CPU, Memory, and Storage  
- **Scheduling Policies** (Priority-based or Shortest Job First) determine execution order  
- **Synchronization mechanisms** (mutexes, deadlock detection) ensure safe resource sharing  
- **Real-time logging** tracks all system events and resource usage  

---

# 🎯 Key Features

## 1. Dynamic VM Arrivals
- VMs arrive randomly throughout the simulation
- Each VM requests different amounts of CPU, memory, and storage
- Each VM has a unique priority level and execution time

## 2. CPU Scheduling Policies
- **Priority Scheduling**: Executes higher-priority VMs first (lower number = higher priority)
- **Shortest Job First (SJF)**: Executes VMs with shortest burst time first
- Switchable at runtime via command-line arguments

## 3. Resource Management
- Tracks available/allocated resources for CPU cores, memory, and storage
- Prevents over-allocation through synchronized access with mutexes
- Real-time resource utilization monitoring

## 4. Deadlock Detection & Prevention
- **Deadlock Detection**: Uses cycle detection in resource request graph
- **Deadlock Prevention**: Implements Banker's Algorithm to prevent unsafe allocations
- Continuous monitoring and warning system

## 5. Thread-Based Execution
- **VM Arrival Thread**: Simulates new VM arrivals
- **Scheduler Thread**: Selects and allocates resources to VMs
- **Execution Threads**: Each scheduled VM runs in its own thread
- **Monitoring Thread**: Provides real-time system status updates

## 6. Real-Time Logging
- Console output with color-coded status messages
- File-based logging for post-simulation analysis
- Detailed timestamps for all events

---

# 🛠️ Project Structure

```
cloud_scheduler/
├── include/              # Header files
│   ├── vm.h             # Virtual Machine definitions
│   ├── resources.h      # Resource pool management
│   ├── scheduler.h      # Scheduler implementation
│   ├── deadlock.h       # Deadlock detection/prevention
│   └── logger.h         # Logging utilities
├── src/                 # Implementation files
│   ├── main.c           # Main simulation loop
│   ├── vm.c             # VM management
│   ├── resources.c      # Resource allocation
│   ├── scheduler.c      # Scheduling logic
│   ├── deadlock.c       # Deadlock handling
│   └── logger.c         # Logging functions
├── Makefile             # Build configuration
└── README.md            # This file
```

---

# 🚀 Building the Project

## Prerequisites
- GCC compiler
- POSIX-compliant system (Linux/macOS/WSL)
- GNU Make

## Compilation

```bash
# Navigate to project directory
cd cloud_scheduler

# Build the project
make

# Or rebuild from scratch
make rebuild
```

---

# ▶️ Running the Simulation

## With Priority Scheduling (Default)

```bash
make run
```

## With Shortest Job First (SJF) Scheduling

```bash
make run-sjf
```
---

# ▶️ How to Run the Cloud Scheduler Project

Follow these simple steps in VS Code.

## Step 1: Open the Project
Open Visual Studio Code and open the folder named **cloud_scheduler**.

## Step 2: Open the Terminal

Press:

```
Ctrl + `
```

The terminal will open at the bottom.

## Step 3: Go to the Project Folder

If the terminal is not already inside the folder, type:

```bash
cd cloud_scheduler
```

## Step 4: Run the SJF Scheduler

Type the following command and press **Enter**:

```bash
.\run_sjf.bat
```

## Step 5: Run the Priority Scheduler

Type the following command and press **Enter**:

```bash
.\run.bat
```

If a menu appears, select the **Priority Scheduling** option.

---

## Alternative Method

You can also run the program directly by typing:

```bash
.\cloud_scheduler.exe
```

This will start the Cloud Scheduler simulator directly from the executable file.
## Manual Execution

```bash
./bin/cloud_scheduler priority    # Priority Scheduling
./bin/cloud_scheduler sjf         # SJF Scheduling
```

---

# 📊 Output Explanation

## Console Output

The simulator displays:

- Real-time VM arrivals and completions
- Resource allocation status
- Scheduler statistics
- Deadlock warnings
- Final performance metrics

## Log File

A detailed log file (`simulation_log.txt`) is generated containing:

- All VM lifecycle events
- Resource allocation/deallocation
- Deadlock detection events
- Timestamps for analysis

## Metrics Reported

- **Total VMs Processed**: Number of VMs that completed execution
- **Context Switches**: Number of times scheduler switched between VMs
- **Average Waiting Time**: Mean time VMs spent waiting in queue
- **Average Turnaround Time**: Mean time from arrival to completion
- **Resource Utilization**: Percentage of resources being used

---

# 🔑 Key Components

## Virtual Machine (vm.h/vm.c)

- Represents a VM with resource requirements
- Tracks state: READY → RUNNING → COMPLETED
- Maintains arrival, start, and completion times
- Priority and burst time attributes

## Resource Manager (resources.h/resources.c)

- Manages CPU, Memory, and Storage pools
- Thread-safe allocation/deallocation using mutexes
- Tracks available and total resources
- Calculates utilization percentage

## Scheduler (scheduler.h/scheduler.c)

- Implements Priority and SJF algorithms
- Selects next VM to execute
- Handles resource allocation
- Calculates performance metrics

## Deadlock Detector (deadlock.h/deadlock.c)

- **Cycle Detection**: Identifies circular wait conditions
- **Banker's Algorithm**: Ensures safe resource allocation
- **Request Graph**: Tracks resource requests and allocations
- **Prevention**: Denies unsafe allocations

## Logger (logger.h/logger.c)

- Synchronized console and file logging
- Color-coded log levels (INFO, WARNING, ERROR, SUCCESS)
- Timestamp for each event

---

# 🎓 OS Concepts Demonstrated

## 1. Multithreading
- Multiple threads representing different components
- Thread creation and management
- Thread synchronization and coordination

## 2. Synchronization Primitives
- **Mutexes**: Protect shared resources
- **Resource Locks**: Ensure atomic operations
- **Critical Sections**: Protected code regions

## 3. CPU Scheduling
- Priority-based scheduling
- Shortest Job First (SJF) algorithm
- Context switching overhead

## 4. Resource Allocation
- Request-grant cycle
- Resource fragmentation prevention
- Efficient utilization strategies

## 5. Deadlock Prevention
- Banker's Algorithm implementation
- Circular wait detection
- Safe state verification

## 6. Performance Analysis
- Waiting time calculation
- Turnaround time measurement
- CPU utilization metrics

---

# 📈 System Configuration

Default system resources:

- **CPU Cores**: 16
- **Memory**: 32 MB
- **Storage**: 100 GB
- **Simulation Duration**: 30 seconds
- **Maximum VMs**: 50

Modify in `src/main.c`:

```c
#define TOTAL_CPU 16
#define TOTAL_MEMORY 32
#define TOTAL_STORAGE 100
#define SIMULATION_TIME 30
#define MAX_VMS 50
```

---

# 🧪 Testing

## Test Different Scheduling Policies

```bash
# Test Priority Scheduling
make run
# Compare metrics

# Test SJF Scheduling
make run-sjf
# Compare metrics
```

## Verify Deadlock Handling

- Simulate high contention for resources
- Monitor warnings in real-time output
- Check log file for deadlock events

## Check Resource Utilization

- Monitor real-time resource status
- Verify allocation doesn't exceed total resources
- Analyze utilization over simulation duration

---

# 📝 Example Output

```
╔════════════════════════════════════════════════════════╗
║  SHARED CLOUD RESOURCE SCHEDULER SIMULATOR            ║
║  Demonstrating OS Concepts: Scheduling, Synchronization ║
║  and Deadlock Prevention in Cloud Computing            ║
╚════════════════════════════════════════════════════════╝

[Mode] Priority Scheduling

[System Resources]

  CPU Cores: 16
  Memory: 32 MB
  Storage: 100 GB

Simulation running for 30 seconds...
Press Ctrl+C to stop

[00:00:05] INFO: VM 0 arrived (Priority: 2, Burst: 3421ms)
[00:00:06] SUCCESS: VM 0 scheduled for execution

========== RESOURCE STATUS ==========
CPU:     14 / 16 cores available
Memory:  26 / 32 MB available
Storage: 85 / 100 GB available
Overall Utilization: 12.50%
=====================================
```

---

# 🔧 Troubleshooting

## Compilation Errors

- Ensure pthread library is available
- Use `-pthread` flag in compilation
- Check for missing header files

## Deadlock Issues

- Monitor warnings in console output
- Check deadlock detector status
- Review log file for request patterns

## Resource Exhaustion

- Reduce VM resource requests
- Increase system resource limits
- Reduce simulation duration

---

# 📚 Further Enhancements

Possible improvements:

1. **Advanced Scheduling**: Implement Round Robin, Multi-level feedback queue  
2. **VM Migration**: Support VM migration between nodes  
3. **Load Balancing**: Distribute load across multiple schedulers  
4. **Fault Tolerance**: Handle VM failures and recovery  
5. **Performance Optimization**: Reduce context switching overhead  
6. **Visualization**: GUI-based system visualization  
7. **Statistical Analysis**: Generate detailed performance reports  

---

# 👥 Developer 
- **Fatima Yousaf**: Thread execution, synchronization, CLI integration  

---

# 📄 License

This project is open-source and available for educational purposes.

---

# 🙏 Acknowledgments

This project demonstrates practical implementation of concepts from:

- Operating Systems (Abraham Silberschatz, et al.)
- Advanced Computer Architecture
- Distributed Systems Design

---

**Last Updated**: December 2024  
**Status**: Production Ready
