# Which compiler to use
CC = gcc
CCPP = g++

TARGET = elevator

CFLAGS =  -g -Wall -Wextra
CPPFLAGS = -std=gnu++14 -g -Wall -Wextra
LDFLAGS = -lm -lcomedi -lphobos2 -lboost_system -lboost_thread -lpthread 

SRC = 
SRCPP =  main.cpp Hardware.cpp io_old.cpp Generic_thread.cpp Control.cpp elevator.cpp Communication.cpp Network.cpp message_handler.cpp helper_functions.cpp

OBJ = $(SRCPP:.cpp=.o) $(SRC:.c=.o)


all: $(TARGET)

sim: $(SIMTARGET)

SIMTARGET = ../simulator/source/simelev.a
SIMSRC = ../simulator/source/sim_backend.d ../simulator/source/timer_event.d 
SIMLDFLAGS = -lib

$(SIMTARGET): $(SIMSRC)
	dmd $(SIMSRC) $(SIMLDFLAGS) -of$@

run: all
	./$(TARGET)

$(TARGET): $(OBJ) 
	$(CCPP) -o $@   $^ simelev.a $(LDFLAGS) 

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@ 

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean run
