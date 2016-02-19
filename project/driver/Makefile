# Which compiler to use
CC = gcc

TARGET = elevator

CFLAGS = -std=gnu11 -g -Wall -Wextra
LDFLAGS = -lcomedi -lm -lpthread -lphobos2


ELEVSRC = elev.c io.c main.c
ELEVOBJ = $(ELEVSRC:.c=.o)  


all: $(TARGET)


sim: $(SIMTARGET)

SIMTARGET = ../simulator/source/simelev.a
SIMSRC = ../simulator/source/sim_backend.d ../simulator/source/timer_event.d 
SIMLDFLAGS = -lib

$(SIMTARGET): $(SIMSRC)
	dmd $(SIMSRC) $(SIMLDFLAGS) -of$@



$(TARGET): $(ELEVOBJ) $(SIMTARGET)
	$(CC) $^ -o $@ $(SIMLIB) $(LDFLAGS) 

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 



clean:
	rm -f $(TARGET) $(ELEVOBJ) $(SIMTARGET)

rebuild:	clean all

.PHONY: all rebuild clean sim
