# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Directories
SRC_DIR = .
INC_DIR = imaging
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(INC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
EXEC = $(BIN_DIR)/RinyTenderer

# Build rule
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: clean

