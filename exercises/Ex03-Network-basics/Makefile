TARGET = networkhandler

CC = gcc

CFLAGS = -g -Wall -std=gnu++11
LDFLAGS = -lcomedi -lrt -lboost_system -lboost_thread -lpthread -lstdc++ 

SOURCES = asio_nethandler.cpp

all:$(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) 

clean:
	rm -rf $(TARGET)

rebuild: clean all

.PHONY: clean all rebuild
