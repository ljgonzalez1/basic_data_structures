CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
AR = ar
ARFLAGS = rcs

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
LIB_DIR = lib
EXAMPLES_DIR = examples
TESTS_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
LIBRARY = $(LIB_DIR)/libdatastructures.a

EXAMPLE_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLES = $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/%,$(EXAMPLE_SOURCES))

TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.c)
TESTS = $(patsubst $(TESTS_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SOURCES))

.PHONY: all clean examples tests

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIBRARY) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -ldatastructures -o $@

$(BUILD_DIR)/%: $(TESTS_DIR)/%.c $(LIBRARY) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -ldatastructures -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

examples: $(LIBRARY) $(EXAMPLES)

tests: $(LIBRARY) $(TESTS)

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
