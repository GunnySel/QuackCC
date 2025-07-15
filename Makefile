# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g -I./src/_include -MMD -MP

# Project structure
SRC_DIR := src
BUILD_DIR := build
BIN := $(BUILD_DIR)/quackcc

# Find all .cpp files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
# Create corresponding .o file paths in build/
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

# Default target
all: $(BIN)

# Link final binary
$(BIN): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@

# Compile source to object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files for incremental builds
-include $(DEP_FILES)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
