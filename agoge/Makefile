# Makefile for Agoge
# Minimal excerpt focusing on compiler logic.

# Allow overriding the compiler via environment variable, else default
CXX := mpic++
ifeq ($(CXX),)
    CXX := g++
endif

# OS detection (unchanged from your snippet)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    OS := Mac
    CXX := mpic++ # On Mac, prefer clang++
else ifeq ($(UNAME_S),Linux)
    OS := Linux
else
    OS := Unknown
endif

# C++ Standard (default to -std=c++2a)
CXX_STD ?= -std=c++2a

# Base flags (common to all compilers)
CXXFLAGS := $(CXX_STD) -O3 -Wall -Wextra -MMD -MP -g 

# -------------------------------
# Additional compiler-specific flags
# -------------------------------
ifeq ($(findstring icpc,$(CXX)),icpc)
    # Intel Classic C++ (icpc)
    CXXFLAGS += -xHost -shared-intel
    $(info Using Intel icpc compiler, adding -xHost)
else ifeq ($(findstring icpx,$(CXX)),icpx)
    # Intel oneAPI DPC++/C++
    CXXFLAGS += -xHost -shared-intel
    $(info Using Intel icpx compiler, adding -xHost)
else ifneq ($(findstring g++,$(CXX)),)  
    # If 'g++' found in compiler name => assume GNU
    CXXFLAGS += -march=native 
    $(info Using GNU g++ compiler, adding -march=native)
else
    # e.g., clang++
    $(info Using compiler: $(CXX) with generic flags)
endif

# HDF5 pkg-config logic, etc. (unchanged)...
#  HDF5_CFLAGS = ...
#  HDF5_LIBS = ...
#  ...
#  etc.

# Normal build rules below...
#  OBJ_FILES, $(TARGET), etc.
#  ...
# HDF5 Flags using pkg-config
HDF5_CFLAGS := $(shell pkg-config --cflags hdf5_cpp 2>/dev/null)
HDF5_LIBS := $(shell pkg-config --libs hdf5_cpp 2>/dev/null)

# Fallback if pkg-config fails
ifeq ($(strip $(HDF5_CFLAGS)),)
    HDF5_CFLAGS := -I/usr/local/include
endif
ifeq ($(strip $(HDF5_LIBS)),)
    HDF5_LIBS := -lhdf5_cpp -lhdf5
endif

# Include Paths
INCLUDES := -Iinclude/agoge $(HDF5_CFLAGS)

# Linker Flags
LDFLAGS := $(HDF5_LIBS) 

# ========================
# 2. Directories
# ========================

SRC_DIR := src
PROBLEMS_DIR := problems
BUILD_DIR := build
BUILD_SRC_DIR := $(BUILD_DIR)/src
BUILD_PROB_DIR := $(BUILD_DIR)/problems
INCLUDE_DIR := include/agoge

# ========================
# 3. Sources and Objects
# ========================

# Source Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
PROB_FILES := $(wildcard $(PROBLEMS_DIR)/*.cpp)

# Object Files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_SRC_DIR)/%.o,$(SRC_FILES)) \
             $(patsubst $(PROBLEMS_DIR)/%.cpp,$(BUILD_PROB_DIR)/%.o,$(PROB_FILES))

# Dependency Files
DEP_FILES := $(OBJ_FILES:.o=.d)

# ========================
# 4. Targets
# ========================

# Executable Name
TARGET := agoge_run

# Default Target
all: $(TARGET)

# Link Target
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile Source Files
$(BUILD_SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_SRC_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_PROB_DIR)/%.o: $(PROBLEMS_DIR)/%.cpp | $(BUILD_PROB_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create Build Directories
$(BUILD_SRC_DIR):
	mkdir -p $(BUILD_SRC_DIR)

$(BUILD_PROB_DIR):
	mkdir -p $(BUILD_PROB_DIR)

# Include Dependency Files
-include $(DEP_FILES)

# Clean Target
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# ========================
# 5. Phony Targets
# ========================

.PHONY: all clean