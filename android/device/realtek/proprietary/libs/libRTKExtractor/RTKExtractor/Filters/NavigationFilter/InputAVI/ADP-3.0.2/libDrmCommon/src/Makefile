include ../../common.mk

LIB			:= $(LIB_DIR)libDrmCommon.a
SOURCES 	:= base64 crypt_util DrmHash master_key sha1
OBJS 		:= $(foreach src,$(SOURCES),$(src).o)
INCLUDE_DIRS	= -I. -I../../common -I../../libAES/src

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
