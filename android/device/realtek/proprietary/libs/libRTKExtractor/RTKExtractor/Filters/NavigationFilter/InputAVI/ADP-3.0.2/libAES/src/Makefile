include ../../common.mk

LIB			:= $(LIB_DIR)libAES.a
SOURCES 	:= rijndael-alg-fst rijndael-api-fst
OBJS 		:= $(foreach src,$(SOURCES),$(src).o)
INCLUDE_DIRS	= -I../../common -I../../libAES/src


all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
