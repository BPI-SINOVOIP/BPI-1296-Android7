
CC = $(CROSS_COMPILE)gcc

CFLAGS += -s -Wall -c -Os
LDFLAGS += -s -Wall

ifeq ($(CONFIG_RTL_11V_SUPPORT),y)
CFLAGS += -DCONFIG_IEEE80211V
endif

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -s -Wl,--gc-sections
endif

all: dot11k_deamon test_11k test_11v

dot11k_deamon: dot11k_deamon.o
	$(CC) $(LDFLAGS) -o dot11k_deamon dot11k_deamon.o

test_11k: test_11k.o
	$(CC) $(LDFLAGS) -o test_11k test_11k.o

test_11v: test_11v.o
	$(CC) $(LDFLAGS) -o test_11v test_11v.o

dot11k_deamon.o: dot11k_deamon.c
	$(CC) $(CFLAGS) -o dot11k_deamon.o dot11k_deamon.c

test_11k.o: test_11k.c
	$(CC) $(CFLAGS) -o test_11k.o test_11k.c

test_11v.o: test_11v.c
	$(CC) $(CFLAGS) -o test_11v.o test_11v.c

clean:
	rm -f *.o
	rm -f dot11k_deamon test_11k test_11v

