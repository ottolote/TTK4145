TARGET = networkhandler

CC = gcc

CFLAGS = -g -Wall -std=gnu++11
LDFLAGS = -lcomedi -lrt -lboost_system -lboost_thread -lpthread -lstdc++ 

SOURCES = asio_nethandler.cpp

all:$(TARGET)

$(TARGET): 
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $< 

clean:
	rm -rf $(TARGET)

.PHONY: clean all
