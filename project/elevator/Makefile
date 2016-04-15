# Which compiler to use
CC = gcc
CCPP = g++

TARGET = elevator

CFLAGS = -std=gnu14 -g -Wall -Wextra
CPPFLAGS = -std=gnu++14 -g -Wall -Wextra
LDFLAGS = -lm -lboost_system -lboost_thread -lpthread

SRC = 
SRCPP = Network.cpp Generic_thread.cpp Communication.cpp main.cpp message_handler.cpp

OBJ = $(SRCPP:.cpp=.o) $(SRC:.c=.o)


all: $(TARGET)

run: all
	./$(TARGET)

$(TARGET): $(OBJ) 
	$(CCPP) -o $@ $(LDFLAGS) $^

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@ 


%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean run
