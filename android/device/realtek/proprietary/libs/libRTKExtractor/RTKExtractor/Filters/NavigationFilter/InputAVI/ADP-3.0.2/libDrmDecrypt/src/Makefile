include ../../common.mk

LIB			:= $(LIB_DIR)libDrmDecrypt.a
SOURCES 	:= Bits DrmAdpApi DrmAdpHelper DrmLocal DrmMemory DrmMessage
OBJS 		:= $(foreach src,$(SOURCES),$(src).o)
INCLUDE_DIRS	= -I../../common -I../../libDrmCommon/src -I../../libAES/src

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
