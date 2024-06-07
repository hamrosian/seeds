# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -Wall -Wextra -I/usr/local/include

# Define the linker flags
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source and build directories
SRC_DIR = src
BUILD_DIR = builds

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Create object files for each .c file
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# The name of the executable
TARGET = $(BUILD_DIR)/seeds

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
