CC = dmd

SIMSRC = sim_backend.d timer_event.d

LDFLAGS = -lib

TARGET = simelev

all: $(TARGET).a

$(TARGET).a: $(SIMSRC)
	$(CC) $(SIMSRC) $(LDFLAGS) -of$@

clean: 
	rm -f $(TARGET).a
