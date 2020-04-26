
RM = rm -f
CC = gcc
SRC = bindAllBins.c
TARGET = bindAllBins

#OPTION += -static

ifeq ($(BOOT),yes)
DEFINE += -DBootPartition_StartUp
endif

ifeq ($(VMX), 1)
DEFINE += -DVMX
endif

ifeq ($(VMX_3RD_PARTY), 1)
DEFINE += -DVMX_3RD_PARTY
endif

ifeq ($(VMX_BL), 1)
DEFINE += -DVMX_BL
endif

all: $(TARGET)

$(TARGET): $(SRC)
#	$(CC) $(OPTION) $^ -o $(OUTPUT)
	$(CC) $^ -o $(TARGET) $(DEFINE)
	chmod +x $(TARGET)


.PHONY: clean
clean:
	$(RM) *.o $(TARGET)

