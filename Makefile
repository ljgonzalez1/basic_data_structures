# ===== basic_data_structures - Makefile simple + demo =====

CC      := cc
AR      := ar
CFLAGS  := -std=c11 -Wall -Wextra -O2 -fPIC -Iinclude
ARFLAGS := rcs

LIBNAME := bds
SRCDIR  := src
EXAMPLEDIR := examples
OBJDIR  := build/obj
LIBDIR  := build/lib
BINDIR  := build/bin

SRC := \
	$(SRCDIR)/array/array.c \
	$(SRCDIR)/array/array_sort.c

OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

LIB_STATIC := $(LIBDIR)/lib$(LIBNAME).a
LIB_SHARED := $(LIBDIR)/lib$(LIBNAME).so
DEMO_BIN   := $(BINDIR)/demo

# ========== Targets ==========
.PHONY: all clean
all: $(LIB_STATIC) $(LIB_SHARED) $(DEMO_BIN)

make: all

# ---- Librerías ----
$(LIB_STATIC): $(OBJ) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_SHARED): $(OBJ) | $(LIBDIR)
	$(CC) -shared -o $@ $^

# ---- Objetos ----
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Demo ----
$(DEMO_BIN): $(EXAMPLEDIR)/demo.c $(LIB_STATIC) | $(BINDIR)
	$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# ---- Carpetas ----
$(OBJDIR) $(LIBDIR) $(BINDIR):
	@mkdir -p $@

# ---- Limpieza ----
clean:
	rm -rf build
