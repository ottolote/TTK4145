# Which compiler to use
# CC = gcc
CCPP = g++

TARGET = phoenix

CFLAGS = -std=gnu11 -g -Wall -Wextra
CPPFLAGS = -std=gnu++11 -g -Wall -Wextra
LDFLAGS = -lm -lboost_system -lboost_thread -lpthread

SRC = 
SRCPP = networkhandler.cpp main.cpp

OBJ = $(SRCPP:.cpp=.o) #$(SRC:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CCPP) -o $@ $(LDFLAGS) $^

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -c $< -o $@ 


%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all

.PHONY: all rebuild clean 
