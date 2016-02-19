TARGET = boost_testing

CFLAGS = -g -Wall 
LDFLAGS = -lcomedi -lrt -lboost_system -lboost_thread -lstdc++

SOURCES = asio_testing5.cpp

all:$(TARGET)

$(TARGET): 
	gcc $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $< 

clean:
	rm -rf $(TARGET)

.PHONY: clean all
