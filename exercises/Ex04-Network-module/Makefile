# Which compiler to use
CC = gcc-4.8

# Compiler flags go here.
CFLAGS = -std=gnu11 -g -Wall -Wextra

# Linker flags go here.
LDFLAGS = -lcomedi -lm -lpthread -lstdc++

# list of sources
ELEVSRC = main.c networkhandler.c

# program executable file name.
TARGET = elevator

# top-level rule, to compile everything.
all: $(TARGET)

# Define all object files.
ELEVOBJ = $(ELEVSRC:.c=.o)  

# rule to link the program
$(TARGET): $(ELEVOBJ)
	$(CC) $^ -o $@ $(LDFLAGS) 

# Compile: create object files from C source files.
%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJ)

rebuild:	clean all


# for declaring targets as not actual files.
.PHONY: all rebuild clean
