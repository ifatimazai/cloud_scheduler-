# Cloud Scheduler - Complete Usage Guide

## 📖 Table of Contents

1. [Getting Started](#getting-started)
2. [Building the Project](#building-the-project)
3. [Running the Simulator](#running-the-simulator)
4. [Understanding Output](#understanding-output)
5. [Configuration](#configuration)
6. [Performance Analysis](#performance-analysis)
7. [Advanced Features](#advanced-features)
8. [Common Issues](#common-issues)

---

## Getting Started

The Shared Cloud Resource Scheduler is a multi-threaded simulation demonstrating:
- Virtual Machine (VM) scheduling in cloud environments
- CPU resource allocation with synchronization
- Deadlock detection and prevention
- Performance metrics calculation

### What You'll Learn

1. **Multithreading**: How multiple VMs run concurrently
2. **Synchronization**: Mutex-based resource protection
3. **Scheduling Algorithms**: Priority vs. Shortest Job First
4. **Deadlock Prevention**: Banker's Algorithm implementation
5. **Performance Analysis**: Waiting times and utilization

---

## Building the Project

### Windows Users

**Step 1: Download MinGW-w64**
- Visit: https://www.mingw-w64.org/
- Download installer
- During setup, select:
  - Architecture: x86_64
  - Threads: posix
  - Exception handling: dwarf2

**Step 2: Build**
```cmd
cd "C:\Users\User\Documents\os project\cloud_scheduler"
build.bat
```

**Step 3: Verify**
```cmd
dir bin
REM Should show: cloud_scheduler.exe
```

### Linux/macOS Users

**Step 1: Install Tools**
```bash
# Ubuntu/Debian
sudo apt-get install build-essential gcc make

# macOS
xcode-select --install
```

**Step 2: Build**
```bash
cd cloud_scheduler
make
```

**Step 3: Verify**
```bash
ls -la bin/
REM Should show: cloud_scheduler
```

### Using WSL

```bash
# In WSL terminal
wsl
cd /mnt/c/Users/User/Documents/"os project"/cloud_scheduler
make
make run
```

---

## Running the Simulator

### Basic Execution

**Priority Scheduling (Higher Priority = Smaller Number)**
```cmd
# Windows
run.bat

# Linux/macOS
./bin/cloud_scheduler priority
# or
make run
```

**Shortest Job First (SJF) Scheduling**
```cmd
# Windows
run_sjf.bat

# Linux/macOS
./bin/cloud_scheduler sjf
# or
make run-sjf
```

### Execution Flow

1. **Initialization** (0-2 seconds)
   - Resource pool created
   - Scheduler initialized
   - All threads started

2. **Active Simulation** (2-28 seconds)
   - VMs randomly arrive
   - Scheduler allocates resources
   - VMs execute concurrently
   - Deadlock detection running
   - Status updates every 5 seconds

3. **Cleanup** (28-30 seconds)
   - Stop accepting new VMs
   - Wait for execution completion
   - Calculate metrics
   - Generate report

4. **Final Report** (30+ seconds)
   - Performance statistics displayed
   - Log file saved

---

## Understanding Output

### Console Output Example

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

[00:00:01] INFO: VM 0 arrived (Priority: 2, Burst: 3421ms)
[00:00:02] SUCCESS: VM 0 scheduled for execution
[00:00:05] INFO: VM 1 arrived (Priority: 1, Burst: 2100ms)
...
```

### Log Levels Explained

| Level | Meaning | Example |
|-------|---------|---------|
| **INFO** | Informational event | VM arrived, resource status |
| **SUCCESS** | Successful operation | VM allocated, execution started |
| **WARNING** | Potential issue | Deadlock detected, resource waiting |
| **ERROR** | Error condition | Allocation failed |

### Real-Time Status (Every 5 Seconds)

```
========== RESOURCE STATUS ==========
CPU:     10 / 16 cores available
Memory:  22 / 32 MB available
Storage: 75 / 100 GB available
Overall Utilization: 30.21%
=====================================

Ready Queue: 5 VMs
Completed: 3 VMs
Total Created: 8 VMs

⚠️  WARNING: Deadlock condition detected!
```

### Final Statistics

```
========== SCHEDULER STATISTICS ==========
Scheduling Policy: PRIORITY
Total VMs Completed: 15
Total Context Switches: 42
Average Waiting Time: 8.32 seconds
Average Turnaround Time: 15.45 seconds
==========================================
```

### Interpreting Metrics

**Average Waiting Time**
- Lower is better
- Time spent in ready queue before execution
- Affected by scheduling policy and VM arrival rate

**Average Turnaround Time**
- Total time from arrival to completion
- Includes waiting + execution time
- Measure of system responsiveness

**Context Switches**
- Number of times scheduler switched between VMs
- Higher number = more scheduling overhead
- Can indicate CPU-bound workload

**Resource Utilization**
- Percentage of total resources in use
- Higher utilization = more efficient system
- Close to 100% may indicate bottlenecks

---

## Configuration

### Modifying System Parameters

Edit `src/main.c` to change:

```c
// System Resources
#define TOTAL_CPU 16              // Number of CPU cores
#define TOTAL_MEMORY 32           // Memory in MB
#define TOTAL_STORAGE 100         // Storage in GB

// Simulation Parameters
#define SIMULATION_TIME 30        // Duration in seconds
#define MAX_VMS 50               // Maximum VMs to simulate
```

### VM Creation Parameters (in vm_arrival_thread)

Adjust in `src/main.c`:

```c
// CPU cores per VM (1-4)
int cpu = 1 + rand() % 4;

// Memory per VM (2-7 MB)
int memory = 2 + rand() % 6;

// Storage per VM (5-19 GB)
int storage = 5 + rand() % 15;

// Priority (0-4, 0 is highest)
int priority = rand() % 5;

// Burst time (1000-5000 ms)
int burst_time = 1000 + rand() % 4000;
```

### Example Configurations

**High Contention (Many VMs, Limited Resources)**
```c
#define TOTAL_CPU 4               // Limited CPU
#define TOTAL_MEMORY 8            // Limited memory
#define MAX_VMS 50               // Many VMs
```

**Well-Provisioned (Abundant Resources)**
```c
#define TOTAL_CPU 32              // Plenty of CPU
#define TOTAL_MEMORY 64           // Plenty of memory
#define TOTAL_STORAGE 200         // Plenty of storage
#define MAX_VMS 20               // Fewer competing VMs
```

**Short Simulation (Testing)**
```c
#define SIMULATION_TIME 10        // Only 10 seconds
#define MAX_VMS 10               // Only 10 VMs
```

---

## Performance Analysis

### Comparing Scheduling Policies

**Run Both and Compare:**
```bash
# Run Priority Scheduling
make run
cp simulation_log.txt priority_log.txt

# Run SJF Scheduling
make run-sjf
cp simulation_log.txt sjf_log.txt

# Compare metrics
cat priority_log.txt
cat sjf_log.txt
```

### Analyzing Log Files

**Look for patterns:**

Priority Scheduling Log:
- Higher-priority VMs execute sooner
- May cause starvation of low-priority VMs
- Lower waiting time for high-priority jobs

SJF Log:
- Shorter jobs execute first
- Better average waiting time
- Minimizes context switches

### Deadlock Analysis

Check for deadlock events:
```bash
# Linux/macOS
grep -i "deadlock\|WARNING" simulation_log.txt

# Windows
findstr /I "deadlock WARNING" simulation_log.txt
```

### Resource Utilization Trends

Monitor how resources change:
```
Time    CPU    Memory    Storage    Utilization
00:00   16/16  32/32     100/100    0%
00:05   12/16  26/32     85/100     15.2%
00:10   8/16   20/32     70/100     28.1%
00:15   4/16   10/32     50/100     45.3%
00:20   2/16   8/32      40/100     52.1%
00:25   16/16  32/32     100/100    0%
```

---

## Advanced Features

### Deadlock Detection in Action

The simulator implements:

1. **Cycle Detection**
   - Checks if VM waiting patterns form cycles
   - Logs when potential deadlock detected

2. **Banker's Algorithm**
   - Grants resources only if safe state guaranteed
   - Prevents unsafe allocations
   - Logged when allocation denied

### Thread Architecture

**Four Main Threads:**

1. **VM Arrival Thread**
   - Generates new VM arrivals
   - Every 1-3 seconds
   - Random resource requirements

2. **Scheduler Thread**
   - Selects next VM to run
   - Allocates resources
   - Monitors deadlocks
   - Every 500ms

3. **Execution Threads**
   - One per running VM
   - Simulates work with sleep
   - Detach after completion

4. **Monitoring Thread**
   - Displays status every 5 seconds
   - Shows resources, queues, warnings

### Synchronization Mechanisms

**Mutexes Used:**
- `resource_lock`: Protects resource pool access
- `detector_lock`: Protects deadlock detector state
- `sim_lock`: Protects simulation context

---

## Common Issues

### Issue: Build Fails

**Windows - "gcc not found"**
```cmd
REM Verify installation
gcc --version

REM If not found, add to PATH:
set PATH=%PATH%;C:\Program Files\mingw-w64\x86_64-13.1.0-posix-seh-rt_v11-rev1\mingw64\bin
```

**Linux - Missing dependencies**
```bash
sudo apt-get install build-essential
sudo apt-get install libpthread-stubs0-dev
```

### Issue: Compilation Errors

**Common fix: Missing #include**
- Ensure all header files are in `include/` directory
- Verify include paths in Makefile/batch script

**common fix: Pthread errors**
- Windows: Ensure MinGW-w64 was installed with "posix" threads
- Linux: Install libpthread development package

### Issue: Program Crashes

**Reduce system load:**
```c
#define TOTAL_CPU 8      // Instead of 16
#define MAX_VMS 20      // Instead of 50
#define SIMULATION_TIME 15  // Instead of 30
```

**Check available memory:**
```cmd
REM Windows
systeminfo | find "Available Physical Memory"

REM Linux
free -h
```

### Issue: No Deadlock Detected

This is **normal** - it means:
- System is well-provisioned
- Resources not heavily contended
- VMs completing quickly enough

**To trigger deadlock:**
```c
#define TOTAL_CPU 4
#define TOTAL_MEMORY 8
#define MAX_VMS 50  // Increase this
```

### Issue: Very Low Utilization

Possible causes:
- Resources too abundant
- VMs too small
- Too few VMs arriving

**Solutions:**
```c
#define TOTAL_CPU 8         // Reduce resources
#define TOTAL_MEMORY 16
#define TOTAL_STORAGE 50
```

---

## Performance Tuning

### Optimize for Low Waiting Time
```c
// Run for longer
#define SIMULATION_TIME 60

// Use SJF scheduling
// Increase system resources
#define TOTAL_CPU 32
#define TOTAL_MEMORY 128
```

### Optimize for High Throughput
```c
// Smaller VM sizes
int cpu = 1 + rand() % 2;      // 1-2 cores
int memory = 1 + rand() % 3;   // 1-3 MB
int burst_time = 500 + rand() % 1500;  // 0.5-2s
```

### Stress Testing
```c
#define TOTAL_CPU 8          // Limited resources
#define TOTAL_MEMORY 16
#define TOTAL_STORAGE 50
#define MAX_VMS 100         // Many VMs
```

---

## Files Generated

After each run:

| File | Contents |
|------|----------|
| `simulation_log.txt` | Complete event log with timestamps |
| `bin/cloud_scheduler.exe` | Compiled executable |
| Console output | Real-time status updates |

---

## Next Steps

1. **Experiment** with different scheduling policies
2. **Modify** system parameters to see impact
3. **Analyze** log files for patterns
4. **Study** source code to understand implementation
5. **Extend** project with new features

---

## Support Resources

- **README.md**: Feature overview and architecture
- **INSTALL.md**: Installation instructions
- **Source Code**: Well-commented implementation
- **Log Files**: Detailed simulation trace

---

**Happy Simulating! 🚀**
