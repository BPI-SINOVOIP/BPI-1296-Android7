CFLAGS += -Wall -Ofast -funroll-loops
CPPFLAGS += -DNO_PROTOTYPES=1
LDLIBS += -lm

all: Dom0tcpService

Dom0tcpService: Dom0tcpService.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o Dom0tcpService

.PHONY: all clean
