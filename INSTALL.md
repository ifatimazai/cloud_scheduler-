# Installation and Setup Guide

## Windows Setup (Recommended)

### 1. Install MinGW/MSYS2

**Option A: MinGW-w64 (Standalone)**
1. Download from: https://www.mingw-w64.org/
2. Choose: MinGW-w64 online installer
3. During installation, select:
   - Version: Latest (e.g., 13.x)
   - Architecture: x86_64
   - Threads: posix (important for pthread)
   - Exception handling: dwarf2
4. Add to PATH:
   - Usually: `C:\Program Files\mingw-w64\x86_64-13.1.0-posix-seh-rt_v11-rev1\mingw64\bin`

**Option B: MSYS2 (Recommended)**
1. Download from: https://www.msys2.org/
2. Install to default location: `C:\msys64`
3. Open MSYS2 terminal
4. Run:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-toolchain
   pacman -S make
   ```

### 2. Verify Installation

Open Command Prompt and verify:
```cmd
gcc --version
gcc (MinGW-w64 13.1.0, built by Brecht Wyseur) 13.1.0
```

### 3. Build the Project

**Using Batch Script (Easiest):**
```cmd
cd "C:\Users\User\Documents\os project\cloud_scheduler"
build.bat
```

**Using Command Prompt:**
```cmd
cd "C:\Users\User\Documents\os project\cloud_scheduler"
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/main.c -o obj/main.c.o
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/vm.c -o obj/vm.c.o
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/resources.c -o obj/resources.c.o
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/scheduler.c -o obj/scheduler.c.o
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/deadlock.c -o obj/deadlock.c.o
gcc -Wall -Wextra -pthread -I./include -std=c99 -O2 ^
  -c src/logger.c -o obj/logger.c.o
gcc obj/main.c.o obj/vm.c.o obj/resources.c.o obj/scheduler.c.o ^
  obj/deadlock.c.o obj/logger.c.o -o bin/cloud_scheduler.exe -pthread -lm
```

### 4. Run the Simulator

**Priority Scheduling:**
```cmd
run.bat
```

**Shortest Job First (SJF):**
```cmd
run_sjf.bat
```

**Manual Execution:**
```cmd
cd bin
cloud_scheduler.exe priority
REM or
cloud_scheduler.exe sjf
```

---

## Linux/macOS Setup (Using Make)

### 1. Install Required Tools

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential gcc make
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc make
```

**macOS:**
```bash
xcode-select --install
```

### 2. Build the Project

```bash
cd cloud_scheduler
make
```

Or with SJF:
```bash
make run-sjf
```

### 3. Run with Different Options

```bash
# Priority Scheduling
./bin/cloud_scheduler priority

# SJF Scheduling
./bin/cloud_scheduler sjf

# Or use Make targets
make run
make run-sjf
```

---

## WSL (Windows Subsystem for Linux) Setup

### 1. Enable WSL2

```powershell
# Open PowerShell as Administrator
wsl --install -d Ubuntu
wsl --set-default-version 2
```

### 2. Install Build Tools in Ubuntu

```bash
sudo apt-get update
sudo apt-get install build-essential gcc make
```

### 3. Navigate and Build

```bash
cd /mnt/c/Users/User/Documents/"os project"/cloud_scheduler
make
make run
```

---

## Troubleshooting

### Error: "gcc not found"

**Windows:** Ensure MinGW/MSYS2 is installed and added to PATH
```cmd
echo %PATH%
```
Should contain GCC installation path

**Linux:** Install via package manager
```bash
sudo apt-get install gcc
```

### Error: "pthread.h not found"

**Windows:** Use POSIX-compliant thread model in MinGW
- Reinstall MinGW-w64 with "posix" threads selected

**Linux:** Install development packages
```bash
sudo apt-get install build-essential
```

### Error: "Permission denied" (Linux/macOS)

Make script executable:
```bash
chmod +x test.sh
./test.sh
```

### Build Hangs or Crashes

- Reduce `#define MAX_VMS` in main.c (default 50)
- Increase virtual memory/swap space
- Check available disk space

---

## Quick Start Commands

### Windows Quick Start
```cmd
@REM 1. Build
build.bat

@REM 2. Run Priority Scheduling
run.bat

@REM 3. Check results
type simulation_log.txt
```

### Linux/macOS Quick Start
```bash
# 1. Build
make

# 2. Run
make run

# 3. View results
cat simulation_log.txt
```

---

## Verification Checklist

- [ ] Compiler installed and in PATH
- [ ] Project directory created
- [ ] All source files present
- [ ] Build succeeds without errors
- [ ] Executable created in bin/
- [ ] Can run simulator without crashes
- [ ] Log file generated
- [ ] Output shows VMs, resources, and metrics

---

## Next Steps After Installation

1. **Read the README.md** for feature overview
2. **Run with different parameters** to see scheduling differences
3. **Analyze the log file** (simulation_log.txt)
4. **Modify system parameters** in main.c to experiment
5. **Study the source code** to understand OS concepts
6. **Run tests** to verify all components

---

## System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| RAM | 512 MB | 2 GB |
| Disk Space | 50 MB | 200 MB |
| Compiler | GCC 7.0+ | GCC 12.0+ |
| OS | Windows 10+/Linux/macOS | Windows 11/Ubuntu 22.04+ |
| Threads | POSIX-compliant | libpthread |

---

**For issues or questions, refer to the README.md or check the generated log files.**
