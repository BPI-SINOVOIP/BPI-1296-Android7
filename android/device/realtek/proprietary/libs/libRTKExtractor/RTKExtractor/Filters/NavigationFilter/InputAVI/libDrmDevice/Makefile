
include ./common.mk


vpath %c   crypto crypto/reference local/Reference/

SOURCES 	:= DrmKeyStore \
DrmMessage \
DataEncodingBits \
HeaderMem2Structs \
DataEncodingBase64 \
DivXPortableAPI \
DivXRng \
DivXPortableCryptoAPI \
DivXAesEncrypt \
bn_gcd \
bn_sqr \
DivXRsaEncrypt \
bn_lib \
rijndael-api-fst \
bn_mont \
bn_shift \
bn_exp \
bn_add \
DivXRsaDecrypt \
DivXAesDecrypt \
bn_recp \
bn_ctx \
DivXRsa \
bn_print \
rijndael-alg-fst \
DivXSha \
bn_mod \
bn_asm \
bn_div \
bn_word \
bn_mul \
sha256 \
DrmDataLoad \
DrmApi \
HeaderStructs2Mem \
DrmLocal
 

OBJS 		:= $(foreach src,$(SOURCES),$(src).o)


INCLUDE_DIRS	=       -Ilocal -I./common -Icrypto -I./

LIBS		=       


DEFINES += -DDIVXINT_NATIVE_64BIT -DDRM_HARDWARE_SIMULATED -DDRM_GO_LIVE_BASEKEYS
CFLAGS			+= $(INCLUDE_DIRS)  -g $(DEFINES)
CXXFLAGS 		+= $(INCLUDE_DIRS)  -g -DLINUX $(DEFINES)

LDFLAGS			+= -g


LIB = $(LIB_DIR)/libDrmDevice.a


all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
