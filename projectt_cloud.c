CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./include -std=c99 -O2
LDFLAGS = -pthread -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/cloud_scheduler

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Build the executable
$(TARGET): $(OBJ_DIR) $(BIN_DIR) $(OBJECTS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "✓ Build successful! Executable: $(TARGET)"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Run the simulator with default (Priority Scheduling)
run: $(TARGET)
	@echo "Running simulator with Priority Scheduling..."
	@./$(TARGET) priority

# Run with SJF scheduling
run-sjf: $(TARGET)
	@echo "Running simulator with SJF Scheduling..."
	@./$(TARGET) sjf

# Clean build artifacts
clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@rm -f simulation_log.txt
	@echo "✓ Clean complete"

# Clean and rebuild
rebuild: clean all

# Help target
help:
	@echo "Cloud Resource Scheduler - Build Commands"
	@echo "=========================================="
	@echo "make              - Build the project"
	@echo "make run          - Run with Priority Scheduling"
	@echo "make run-sjf      - Run with SJF Scheduling"
	@echo "make rebuild      - Clean and rebuild"
	@echo "make clean        - Remove build artifacts"
	@echo "make help         - Show this help message"

.PHONY: all run run-sjf clean rebuild help
