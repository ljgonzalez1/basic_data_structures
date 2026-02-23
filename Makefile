# basic_data_structures — GNU Makefile (C11, out-of-tree build)
# Layout:
#   include/...
#   src/...
#   examples/...
#   tests/...
#
# Outputs:
#   build/lib/libbds.a
#   build/bin/examples/<name>
#   build/bin/tests/<name>

SHELL := /bin/sh

# ---- Toolchain ----
CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

# ---- Project dirs ----
ROOT_DIR := .
SRC_DIR := src
INCLUDE_DIR := include
EXAMPLES_DIR := examples
TESTS_DIR := tests

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib

LIB_NAME := bds
LIB_A := $(LIB_DIR)/lib$(LIB_NAME).a

# ---- Build mode ----
MODE ?= debug

WARNINGS := -Wall -Wextra -Wpedantic
CPPFLAGS := -I$(INCLUDE_DIR) -I$(SRC_DIR)
CFLAGS := -std=c11 $(WARNINGS)

ifeq ($(MODE),release)
  CFLAGS += -O2 -DNDEBUG
else
  CFLAGS += -O0 -g3
endif

# Dependency files next to objects (.d)
DEPFLAGS := -MMD -MP

# ---- Recursive wildcard helper (pure make) ----
# Source: common pattern for recursive wildcards in make. :contentReference[oaicite:1]{index=1}
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# ---- Sources ----
LIB_SRCS := $(call rwildcard,$(SRC_DIR)/,*.c)
EXAMPLE_SRCS := $(call rwildcard,$(EXAMPLES_DIR)/,*.c)
TEST_SRCS := $(call rwildcard,$(TESTS_DIR)/,*.c)

# ---- Objects ----
LIB_OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))
EXAMPLE_OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(EXAMPLE_SRCS))
TEST_OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

LIB_DEPS := $(LIB_OBJS:.o=.d)
EXAMPLE_DEPS := $(EXAMPLE_OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

# ---- Binaries (one per .c) ----
EXAMPLE_BINS := $(patsubst $(EXAMPLES_DIR)/%.c,$(BIN_DIR)/examples/%,$(EXAMPLE_SRCS))
TEST_BINS := $(patsubst $(TESTS_DIR)/%.c,$(BIN_DIR)/tests/%,$(TEST_SRCS))

# ---- Default target ----
.PHONY: all
all: lib examples tests

# ---- Library ----
.PHONY: lib
lib: $(LIB_A)

$(LIB_A): $(LIB_OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^
	@command -v $(RANLIB) >/dev/null 2>&1 && $(RANLIB) $@ || true

# ---- Examples ----
.PHONY: examples
examples: $(EXAMPLE_BINS)

$(BIN_DIR)/examples/%: $(OBJ_DIR)/examples/%.o $(LIB_A)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -l$(LIB_NAME) -o $@

# ---- Tests ----
.PHONY: tests
tests: $(TEST_BINS)

$(BIN_DIR)/tests/%: $(OBJ_DIR)/tests/%.o $(LIB_A)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -l$(LIB_NAME) -o $@

.PHONY: test
test: tests
	@set -e; \
	for t in $(TEST_BINS); do \
	  echo "[RUN] $$t"; \
	  $$t; \
	done

# ---- Compile rule (mirrored build dir + deps) ----
# Creating output dirs on demand is the typical approach for out-of-tree builds. :contentReference[oaicite:2]{index=2}
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Include auto-generated deps (GNU make supports including generated prerequisite files). :contentReference[oaicite:3]{index=3}
-include $(LIB_DEPS) $(EXAMPLE_DEPS) $(TEST_DEPS)

# ---- Convenience targets ----
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run-demo
run-demo: $(BIN_DIR)/examples/demo
	$(BIN_DIR)/examples/demo

# ---- CLion / clangd: compilation database ----
# CLion can use compile_commands.json for proper navigation. :contentReference[oaicite:4]{index=4}
.PHONY: compdb
compdb:
	@echo "Generating compile_commands.json..."
	@command -v bear >/dev/null 2>&1 && { \
	  bear -- $(MAKE) clean all; \
	  echo "OK: compile_commands.json generated with bear"; \
	  exit 0; \
	} || true
	@command -v compiledb >/dev/null 2>&1 && { \
	  compiledb -n $(MAKE) all; \
	  echo "OK: compile_commands.json generated with compiledb"; \
	  exit 0; \
	} || true
	@echo "ERROR: install 'bear' (recommended) or 'compiledb' to generate compile_commands.json"

.PHONY: help
help:
	@echo "Targets:"
	@echo "  all        -> lib + examples + tests"
	@echo "  lib        -> build/lib/libbds.a"
	@echo "  examples   -> build/bin/examples/*"
	@echo "  tests      -> build/bin/tests/*"
	@echo "  test       -> build and run tests"
	@echo "  run-demo   -> run build/bin/examples/demo"
	@echo "  compdb     -> generate compile_commands.json (for CLion)"
	@echo "  clean      -> remove build/"