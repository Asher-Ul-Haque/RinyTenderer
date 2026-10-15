# - - - | Definitions | - - -

MODE          ?= debug

# - - - Directories - - -
INCLUDE_DIR   := include
SRC_DIR       := src
TEST_DIR      := tests
BIN_DIR       := bin/$(MODE)
BUILD_DIR     := .build/$(MODE)

# - - - Compiler & Flags - - -
CC            := clang
COMMON_FLAGS  := -std=c11 -Wall -Werror -Wpedantic -fPIC -fvisibility=hidden -march=native -fopenmp
DEBUG_FLAGS   := -O3 -g
RELEASE_FLAGS := -O3

CPPFLAGS      := -I$(INCLUDE_DIR) -MMD -MP -D_POSIX_C_SOURCE=200809L
LDFLAGS       := -L$(BIN_DIR) -Wl,-rpath=$(abspath $(BIN_DIR)) -lm

ifeq ($(MODE),debug)
  CFLAGS   := $(COMMON_FLAGS) $(DEBUG_FLAGS)
  CPPFLAGS += -DDEBUG
  $(info Build mode: DEBUG)
  $(info Flags : $(CFLAGS) $(CPPFLAGS))
else ifeq ($(MODE),release)
  CFLAGS   := $(COMMON_FLAGS) $(RELEASE_FLAGS)
  $(info Build mode: RELEASE)
  $(info Flags : $(CFLAGS) $(CPPFLAGS))
else
  $(error Unknown build mode: '$(MODE)'. Must be 'debug' or 'release')
endif
$(info )


# - - - File Finding - - -

MAIN_SRCS   := $(SRC_DIR)/main.c
RENDER_SRCS := $(shell find $(SRC_DIR)/rinyTenderer -name '*.c')
RAW_SRCS    := $(shell find $(SRC_DIR) -name '*.c')
COMMON_SRCS := $(filter-out $(MAIN_SRCS) $(RENDER_SRCS) , $(RAW_SRCS))
TEST_SRCS   := $(shell find $(TEST_DIR) -name '*.c')
ALL_SRCS    := $(COMMON_SRCS) $(MAIN_SRCS) $(TEST_SRCS) $(RENDER_SRCS)

COMMON_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(COMMON_SRCS))
MAIN_OBJS   := $(patsubst %.c, $(BUILD_DIR)/%.o, $(MAIN_SRCS))
RENDER_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(RENDER_SRCS))
TEST_OBJS   := $(patsubst %.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))
ALL_OBJS    := $(patsubst %.c, $(BUILD_DIR)/%.o, $(ALL_SRCS))

MAIN_BINS   := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(MAIN_SRCS))
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/tests/%, $(TEST_SRCS))
RENDER_LIB  := $(BIN_DIR)/libRinyTenderer.so

HEADER_DEPS := $(ALL_OBJS:.o=.d)


# - - - | Building | - - -

# - - - Targets - - -

.PHONY: all tests runTests clean remake

all: $(RENDER_LIB) $(MAIN_BINS) tests

tests: $(RENDER_LIB) $(TEST_BINS)

runTests: tests
	@for test_exec in $(TEST_BINS); do \
		echo ""; \
		echo "Running $$test_exec..."; \
		./$$test_exec || exit 1; \
		echo ""; \
	done

clean:
	@rm -rf $(BIN_DIR) $(BUILD_DIR)
	@echo "CLEANED"
	@echo ""

remake: clean all


# - - - Linking rules - - -

$(RENDER_LIB): $(RENDER_OBJS)
	@mkdir -p $(dir $@)
	@$(CC) -shared -o $@ $^
	@echo ""
	@echo "[SHARED LIB]: $@"
	@for obj in $(RENDER_OBJS); do \
		echo " └── $$obj"; \
	done
	@echo ""

$(MAIN_BINS): $(BIN_DIR)/%: $(BUILD_DIR)/$(SRC_DIR)/%.o $(COMMON_OBJS) $(RENDER_LIB)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(BUILD_DIR)/$(SRC_DIR)/$*.o $(COMMON_OBJS) -lRinyTenderer $(LDFLAGS) -o $@
	@echo ""
	@echo "[BINARY]: $@"
	@echo "  ├── $(BUILD_DIR)/$(SRC_DIR)/$*.o"
	@for obj in $(COMMON_OBJS); do \
		echo "  └── $$obj"; \
	done
	@echo "  └── $(RENDER_LIB)"
	@echo ""

$(TEST_BINS): $(BIN_DIR)/tests/%: $(BUILD_DIR)/$(TEST_DIR)/%.o $(COMMON_OBJS) $(RENDER_LIB)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(BUILD_DIR)/$(TEST_DIR)/$*.o $(COMMON_OBJS) -lRinyTenderer $(LDFLAGS) -o $@
	@echo ""
	@echo "[TEST]: $@"
	@echo "  ├── $(BUILD_DIR)/$(TEST_DIR)/$*.o"
	@for obj in $(COMMON_OBJS); do \
		echo "  └── $$obj"; \
	done
	@echo "  └── $(RENDER_LIB)"
	@echo ""


# - - - Object compilation rules - - -

$(RENDER_OBJS): CPPFLAGS += -DJUST_EXPORT

$(ALL_OBJS): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  [CC]   $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


-include $(HEADER_DEPS)
