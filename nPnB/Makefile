# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3 -fPIC -Iinclude # -O3: maximum optimization activated (~6 times faster than -O0)
LDFLAGS = -shared

# executable name
TARGET = bec

# >Folders
SRC_DIR = src
APP_DIR = app
BUILD_DIR = build
LIB_DIR = lib
BIN_DIR = bin

# library sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Main
MAIN_SRC = $(APP_DIR)/nPnBmain.cpp
MAIN_OBJ = $(BUILD_DIR)/nPnBmain.o

# Main targets
all: directories $(LIB_DIR)/libnpnb.so $(BIN_DIR)/$(TARGET)

# Creating folders
directories:
	mkdir -p $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR)

# Compilation of library objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilation main
$(BUILD_DIR)/nPnBmain.o: $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Creating a shared library
$(LIB_DIR)/libnpnb.so: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Create executable (link to library)
$(BIN_DIR)/$(TARGET): $(MAIN_OBJ)
	$(CXX) -o $@ $< -L$(LIB_DIR) -lnpnb -Wl,-rpath,'$$ORIGIN/../lib'

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR)

.PHONY: all clean directories
