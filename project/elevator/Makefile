# Which compiler to use
CC = g++
CCPP = g++

TARGET = elevator

CFLAGS =  -g -Wall -Wextra
CPPFLAGS = -std=gnu++14 -g -Wall -Wextra
LDFLAGS = -lm -lcomedi -lphobos2 -lboost_system -lboost_thread -lpthread

SRC = 
SRCPP =  main.cpp Communication.cpp Network.cpp Generic_thread.cpp message_handler.cpp

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
	$(CCPP) -o $@ $(SIMTARGET)  $^ $(LDFLAGS) 

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@ 


%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean run
