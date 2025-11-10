# ===== basic_data_structures - Makefile simple =====

CC      := cc
AR      := ar
CFLAGS  := -std=c11 -Wall -Wextra -O2 -fPIC -Iinclude
ARFLAGS := rcs

LIBNAME := bds
SRCDIR  := src
OBJDIR  := build/obj
LIBDIR  := build/lib

SRC := \
	$(SRCDIR)/array/array.c \
	$(SRCDIR)/array/array_sort.c

OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
LIB_STATIC := $(LIBDIR)/lib$(LIBNAME).a
LIB_SHARED := $(LIBDIR)/lib$(LIBNAME).so

# ========== Targets ==========

.PHONY: all clean
all: $(LIB_STATIC) $(LIB_SHARED)

make: all

$(LIB_STATIC): $(OBJ) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_SHARED): $(OBJ) | $(LIBDIR)
	$(CC) -shared -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

clean:
	rm -rf build
