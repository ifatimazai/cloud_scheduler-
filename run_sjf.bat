@echo off
REM Cloud Scheduler Run Script for Windows - SJF Mode

setlocal

set BIN_DIR=bin
set EXE=%BIN_DIR%\cloud_scheduler.exe

if not exist "%EXE%" (
    echo ERROR: Executable not found. Please run build.bat first.
    exit /b 1
)

echo.
echo ╔════════════════════════════════════════════════════════╗
echo ║  SHARED CLOUD RESOURCE SCHEDULER SIMULATOR             ║
echo ║  Running with SJF Scheduling                            ║
echo ╚════════════════════════════════════════════════════════╝
echo.

REM Run with SJF Scheduling
"%EXE%" sjf

echo.
echo Simulation log saved to: simulation_log.txt
echo.
