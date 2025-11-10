# Basic Data Structures

A C library implementing fundamental data structures for educational purposes.

## Overview

This library provides clean, well-documented implementations of basic data structures in C. It's designed as a learning resource and can be used as a foundation for understanding how common data structures work under the hood.

## Data Structures Included

### Vector (Dynamic Array)
- Dynamic resizing array that grows as needed
- O(1) amortized insertion at the end
- O(1) random access by index
- Functions: `vector_create()`, `vector_push_back()`, `vector_pop_back()`, `vector_get()`, `vector_set()`, `vector_size()`, `vector_is_empty()`, `vector_clear()`, `vector_destroy()`

### Linked List
- Singly linked list with head and tail pointers
- O(1) insertion at front and back
- O(1) removal from front
- Functions: `list_create()`, `list_push_front()`, `list_push_back()`, `list_pop_front()`, `list_front()`, `list_back()`, `list_size()`, `list_is_empty()`, `list_clear()`, `list_destroy()`

### Stack
- LIFO (Last In First Out) data structure
- O(1) push and pop operations
- Functions: `stack_create()`, `stack_push()`, `stack_pop()`, `stack_peek()`, `stack_size()`, `stack_is_empty()`, `stack_clear()`, `stack_destroy()`

### Queue
- FIFO (First In First Out) data structure
- O(1) enqueue and dequeue operations
- Functions: `queue_create()`, `queue_enqueue()`, `queue_dequeue()`, `queue_front()`, `queue_rear()`, `queue_size()`, `queue_is_empty()`, `queue_clear()`, `queue_destroy()`

## Building

The library uses a simple Makefile for building:

```bash
# Build the library
make

# Build the library and examples
make examples

# Build the library and tests
make tests

# Clean build artifacts
make clean
```

## Usage

Include the appropriate header file and link against the library:

```c
#include "vector.h"
#include "linked_list.h"
#include "stack.h"
#include "queue.h"

// Compile with:
// gcc -Iinclude your_program.c -Llib -ldatastructures -o your_program
```

### Example: Using Vector

```c
#include "vector.h"

Vector *vec = vector_create(10);
int value = 42;
vector_push_back(vec, &value);
int *retrieved = (int *)vector_get(vec, 0);
vector_destroy(vec);
```

### Example: Using Stack

```c
#include "stack.h"

Stack *stack = stack_create();
int value = 100;
stack_push(stack, &value);
int *top = (int *)stack_peek(stack);
int *popped = (int *)stack_pop(stack);
stack_destroy(stack);
```

## Examples

The `examples/` directory contains complete working examples for each data structure:
- `vector_example.c` - Demonstrates vector operations
- `list_example.c` - Demonstrates linked list operations
- `stack_example.c` - Demonstrates stack operations
- `queue_example.c` - Demonstrates queue operations

Run examples:
```bash
make examples
./build/vector_example
./build/stack_example
./build/queue_example
./build/list_example
```

## Tests

Basic test programs are included in the `tests/` directory:
- `test_vector.c` - Vector unit tests
- `test_stack.c` - Stack unit tests

Run tests:
```bash
make tests
./build/test_vector
./build/test_stack
```

## Project Structure

```
.
├── include/          # Header files
│   ├── vector.h
│   ├── linked_list.h
│   ├── stack.h
│   └── queue.h
├── src/              # Implementation files
│   ├── vector.c
│   ├── linked_list.c
│   ├── stack.c
│   └── queue.c
├── examples/         # Example programs
├── tests/            # Test programs
├── Makefile          # Build configuration
└── README.md         # This file
```

## Memory Management

All data structures store `void*` pointers to data. The library manages the memory for the data structure itself, but **you are responsible for managing the lifetime of the data you store**. The library does not copy or free your data - it only stores pointers.

## License

See the LICENSE file for details.

## Learning Resources

This library is designed for learning. Each implementation is straightforward and follows common patterns:
- Clear naming conventions
- Consistent error handling (return -1 or NULL on error)
- Minimal dependencies (only standard C library)
- Well-commented code
- Comprehensive examples

Feel free to study, modify, and learn from this code!
