# Cloud Scheduler - Quick Reference

## 🚀 Quickest Start (30 seconds)

### Windows
```cmd
cd "C:\Users\User\Documents\os project\cloud_scheduler"
build.bat && run.bat
```

### Linux/macOS
```bash
cd cloud_scheduler
make && make run
```

---

## 📂 Project Structure
```
cloud_scheduler/
├── include/        → Header files (.h)
├── src/            → Implementation (.c)
├── bin/            → Compiled executable
├── Makefile        → Linux/macOS build
├── build.bat       → Windows build
├── run.bat         → Windows run (Priority)
├── run_sjf.bat     → Windows run (SJF)
├── README.md       → Features & architecture
├── INSTALL.md      → Installation steps
├── USAGE.md        → Configuration guide
└── PROJECT_SUMMARY.md → Complete details
```

---

## 🎯 Core Concepts

| Concept | File | What It Does |
|---------|------|--------------|
| VM Management | `vm.c` | Create and track virtual machines |
| Resources | `resources.c` | Allocate/deallocate CPU, memory, storage |
| Scheduler | `scheduler.c` | Select VMs using Priority or SJF |
| Deadlock | `deadlock.c` | Detect and prevent circular waits |
| Logging | `logger.c` | Track events with timestamps |
| Main | `main.c` | Orchestrate everything |

---

## 🔧 Configuration (Edit src/main.c)

```c
#define TOTAL_CPU 16           // CPU cores
#define TOTAL_MEMORY 32        // Memory (MB)
#define TOTAL_STORAGE 100      // Storage (GB)
#define SIMULATION_TIME 30     // Duration (seconds)
#define MAX_VMS 50            // Maximum VMs
```

---

## 📊 Output Files

After running:
- `simulation_log.txt` → Detailed event log
- `bin/cloud_scheduler.exe` → Compiled program (Windows)
- Console output → Real-time status

---

## 🎓 Key Learnings

- **Multithreading**: 4 concurrent threads
- **Synchronization**: Mutexes protect shared resources
- **Scheduling**: Priority vs. SJF algorithms
- **Deadlock**: Banker's Algorithm prevention
- **Performance**: Waiting time & utilization metrics

---

## 🧪 Run Modes

```bash
# Priority Scheduling (default)
./bin/cloud_scheduler priority
make run

# Shortest Job First
./bin/cloud_scheduler sjf
make run-sjf
```

---

## 📈 Metrics Generated

1. **VM Scheduling**
   - Total VMs completed
   - Context switches

2. **Performance**
   - Average waiting time
   - Average turnaround time

3. **Resources**
   - CPU/Memory/Storage utilization
   - Available vs. total resources

4. **Deadlock**
   - Potential deadlocks detected
   - Allocations prevented

---

## 🔐 Thread Safety

All shared resources protected:
- Resource pool → `pthread_mutex_t resource_lock`
- Deadlock detector → `pthread_mutex_t detector_lock`
- Simulation state → `pthread_mutex_t sim_lock`

---

## ⚡ Performance Tips

**High Performance**
```c
#define TOTAL_CPU 32
#define TOTAL_MEMORY 64
#define MAX_VMS 20
```

**High Contention** (to see deadlocks)
```c
#define TOTAL_CPU 4
#define TOTAL_MEMORY 8
#define MAX_VMS 50
```

**Quick Testing**
```c
#define SIMULATION_TIME 10
#define MAX_VMS 10
```

---

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| "gcc not found" | Install MinGW-w64 on Windows |
| "pthread.h missing" | Use posix threads in MinGW |
| Build fails | Run `build.bat` (Windows) or `make` (Linux) |
| Program crashes | Reduce MAX_VMS and TOTAL resources |
| No deadlock | Reduce TOTAL_CPU/MEMORY or increase MAX_VMS |

---

## 📚 Documentation Files

1. **README.md** - Complete feature overview
2. **INSTALL.md** - Step-by-step installation
3. **USAGE.md** - Configuration and analysis guide
4. **PROJECT_SUMMARY.md** - Full project details

Read them in order for complete understanding!

---

## 🎬 Example Session

```
1. Open terminal/cmd
2. Navigate to project: cd cloud_scheduler
3. Build: build.bat (Windows) or make (Linux)
4. Run: run.bat (Windows) or make run (Linux)
5. Watch real-time output with colors
6. Check simulation_log.txt for details
7. Modify parameters and run again
8. Compare different scheduling policies
```

---

## 🌟 What You Can Do

✅ Run complete cloud simulation
✅ Compare scheduling algorithms
✅ Analyze performance metrics
✅ Observe deadlock detection
✅ Monitor resource allocation
✅ Study OS concepts in action
✅ Modify and experiment
✅ Learn multi-threaded C programming

---

## 🎯 Next Steps

1. **Build** the project
2. **Run** with default settings
3. **Read** the generated log file
4. **Modify** configuration parameters
5. **Run** again with SJF
6. **Compare** the metrics
7. **Study** the source code
8. **Extend** with new features

---

## 📋 Checklist

Before running:
- [ ] Compiler installed (gcc)
- [ ] Project downloaded
- [ ] Can navigate to project directory
- [ ] Ready to compile

After building:
- [ ] Executable created
- [ ] No compilation errors
- [ ] Ready to run

After running:
- [ ] Console shows output
- [ ] Log file created
- [ ] Metrics displayed
- [ ] Ready to analyze

---

**Everything is ready! Start building and learning! 🚀**

**Location**: `C:\Users\User\Documents\os project\cloud_scheduler`
