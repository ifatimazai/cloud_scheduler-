@echo off
REM Cloud Scheduler Run Script for Windows

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
echo ║  Running with Priority Scheduling                       ║
echo ╚════════════════════════════════════════════════════════╝
echo.

REM Run with Priority Scheduling
"%EXE%" priority

echo.
echo Simulation log saved to: simulation_log.txt
echo.
