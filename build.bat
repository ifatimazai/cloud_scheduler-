@echo off
REM Cloud Scheduler Build Script for Windows
REM Requires: GCC and POSIX compliance (MinGW/MSYS2)

setlocal enabledelayedexpansion

echo.
echo ╔════════════════════════════════════════════════════════╗
echo ║  SHARED CLOUD RESOURCE SCHEDULER BUILD SCRIPT           ║
echo ║  Windows Batch Version                                  ║
echo ╚════════════════════════════════════════════════════════╝
echo.

REM Define directories
set SRC_DIR=src
set OBJ_DIR=obj
set BIN_DIR=bin
set INC_DIR=include

REM Compiler settings
set CC=gcc
set CFLAGS=-Wall -Wextra -pthread -I./%INC_DIR% -std=c99 -O2
set LDFLAGS=-pthread -lm

echo Checking for required compiler...
where %CC% >nul 2>&1
if errorlevel 1 (
    echo ERROR: GCC compiler not found. Please install MinGW or MSYS2
    echo Download from: https://www.mingw-w64.org/
    exit /b 1
)
echo ✓ GCC found: %CC%
echo.

REM Create directories
echo Creating build directories...
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
echo ✓ Directories created
echo.

REM Compile source files
echo Compiling source files...

set SOURCES=main.c vm.c resources.c scheduler.c deadlock.c logger.c
set OBJECTS=

for %%F in (%SOURCES%) do (
    echo   Compiling %SRC_DIR%\%%F ...
    %CC% %CFLAGS% -c "%SRC_DIR%\%%F" -o "%OBJ_DIR%\%%F.o"
    if errorlevel 1 (
        echo ERROR: Compilation of %%F failed!
        exit /b 1
    )
    if not defined OBJECTS (
        set OBJECTS="%OBJ_DIR%\%%F.o"
    ) else (
        set OBJECTS=!OBJECTS! "%OBJ_DIR%\%%F.o"
    )
)

echo ✓ All source files compiled
echo.

REM Link executable
echo Linking executable...
%CC% !OBJECTS! -o "%BIN_DIR%\cloud_scheduler.exe" %LDFLAGS%
if errorlevel 1 (
    echo ERROR: Linking failed!
    exit /b 1
)
echo ✓ Linking successful
echo.

echo ╔════════════════════════════════════════════════════════╗
echo ║  BUILD SUCCESSFUL                                       ║
echo ╚════════════════════════════════════════════════════════╝
echo.
echo Executable created: %BIN_DIR%\cloud_scheduler.exe
echo.
echo To run the simulator:
echo   cloud_scheduler.exe priority  (Priority Scheduling)
echo   cloud_scheduler.exe sjf       (Shortest Job First)
echo.
