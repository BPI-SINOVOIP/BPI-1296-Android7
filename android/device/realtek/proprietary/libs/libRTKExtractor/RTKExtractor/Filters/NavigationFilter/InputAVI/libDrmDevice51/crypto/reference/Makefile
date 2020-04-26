LIB			:= libxbn.a
SOURCES 	:=  bn_add  bn_div  bn_lib   bn_mul    bn_recp   bn_word \
bn_asm  bn_exp  bn_mod   bn_print  bn_shift \
bn_ctx  bn_gcd  bn_mont  bn_sqr

OBJS 		:= $(foreach src,$(SOURCES),$(src).o)
INCLUDE_DIRS	= -I../include

include common.mk

all: $(LIB)


$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
