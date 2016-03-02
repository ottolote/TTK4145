TARGET = boost_testing

CC = gcc

CFLAGS = -g -Wall -std=gnu++11
LDFLAGS = -lcomedi -lrt -lboost_system -lboost_thread -lstdc++ 

SOURCES = thread_com.cpp

all:$(TARGET)

$(TARGET): 
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $< 

clean:
	rm -rf $(TARGET)

.PHONY: clean all
