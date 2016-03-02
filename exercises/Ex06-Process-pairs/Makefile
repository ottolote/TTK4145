TARGET = boost_testing

CFLAGS = -g -Wall 
LDFLAGS = -lcomedi -lboost_system -lpthread -lstdc++ 
#-lrt #-lboost_thread

SOURCES = boost_udp_test3.cpp

all:$(TARGET)

$(TARGET): 
	gcc $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $< 

clean:
	rm -rf $(TARGET)

.PHONY: clean all
