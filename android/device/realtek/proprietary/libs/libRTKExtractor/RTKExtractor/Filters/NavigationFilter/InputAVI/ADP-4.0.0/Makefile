#Set up include dirs so lower level makefile can still find correct headers
include ./common.mk

INCLUDE_DIRS	+= -I./common -Isrc -I../import/DivXPrimitives/ -I../common/DivXVersion/all/all/include 
DEFINE		+=
PLATFORM 	+= -DADP_HARDWARE_SIMULATED
export

#all: USE_DPR_TAGS MAKE_TAGS
all: AdpKeyStoreLib AdpLib AdpLocalLib AudioDecryptLib CryptoLib CryptoRandomLib VideoDecryptLib

# Platform specific defines
USE_DPR_TAGS:
#copy platform specific ADP implementation to AdpLocalLib directory
ifdef USE_DPR
	cp --reply=yes -v ../command/AdpLocalDMF/DrmAdpLocalDMF.c src/AdpLocalLib/
	cp --reply=yes -v ../command/AdpLocalDMF/makefile.local src/AdpLocalLib/

ifeq ($(ARM_DEVICE),sigma8620)
include ../libDPR/DPR/import/DCCommon/all/all/make/sigma.mk
export INCLUDE_DIRS += -I../../../common -I../../src \
			-I../../../libDPR/DPR/export/all/all/include \
			-I../../../libDPR/DPR/export/$(ARM_DEVICE)/all/include
endif
# end of Platform specific defines
endif

ARFLAGS		= $(CC)  $(PLATFORM) 
HEADERS		:= $(DIR)*.h

#ADP_KEY_STORE_LIB	:= src/AdpKeyStoreLib/AdpKeyStoreLib.a
ADP_KEY_STORE_LIB	:= $(LIB_DIR)/AdpKeyStoreLib.a
ADP_KEY_STORE_LIB_SRC  	:= src/AdpKeyStoreLib/DrmAdpKeyStore

#ADP_LIB			:= src/AdpLib/AdpLib.a
ADP_LIB			:= $(LIB_DIR)/AdpLib.a
ADP_LIB_SRC         	:= src/AdpLib/DrmAdpApi \
			   src/AdpLib/DrmAdpMessage

#ADP_LOCAL_LIB		:= src/AdpLocalLib/AdpLocalLib.a
ADP_LOCAL_LIB		:= $(LIB_DIR)/AdpLocalLib.a
ADP_LOCAL_LIB_SRC	:= src/AdpLocalLib/DrmAdpKeyStoreLocal \
			   src/AdpLocalLib/DrmAdpLocal

#AUDIO_DECRYPT_LIB	:= src/AudioDecryptLib/audioDecryptLib.a
AUDIO_DECRYPT_LIB	:= $(LIB_DIR)/audioDecryptLib.a
AUDIO_DECRYPT_LIB_SRC 	:= src/AudioDecryptLib/AudioDecrypt

#CRYPTO_LIB		:= src/CryptoLib/CryptoLib.a
CRYPTO_LIB		:= $(LIB_DIR)/CryptoLib.a
CRYPTO_LIB_SRC        	:= src/CryptoLib/base64 \
			   src/CryptoLib/bn_add \
			   src/CryptoLib/bn_asm \
			   src/CryptoLib/bn_ctx \
			   src/CryptoLib/bn_div \
			   src/CryptoLib/bn_exp \
			   src/CryptoLib/bn_gcd \
			   src/CryptoLib/bn_lib \
			   src/CryptoLib/bn_mod \
			   src/CryptoLib/bn_mont \
			   src/CryptoLib/bn_mul \
			   src/CryptoLib/bn_print \
			   src/CryptoLib/bn_recp \
			   src/CryptoLib/bn_shift\
			   src/CryptoLib/bn_sqr\
			   src/CryptoLib/bn_word\
			   src/CryptoLib/Bits \
			   src/CryptoLib/rijndael-alg-fst \
			   src/CryptoLib/rijndael-api-fst \
			   src/CryptoLib/rsadecrypt \
			   src/CryptoLib/sha256

#CRYPTO_RANDOM_LIB	:= src/CryptoRandomLib/CryptoRandomLib.a
CRYPTO_RANDOM_LIB	:= $(LIB_DIR)/CryptoRandomLib.a
CRYPTO_RANDOM_LIB_SRC	:= src/CryptoRandomLib/CryptoRandom

#VIDEO_DECRYPT_LIB	:= src/VideoDecryptLib/VideoDecryptLib.a
VIDEO_DECRYPT_LIB	:= $(LIB_DIR)/VideoDecryptLib.a
VIDEO_DECRYPT_LIB_SRC 	:= src/VideoDecryptLib/VideoDecrypt

LIBS			:= $(ADP_KEY_STORE_LIB) $(ADP_LIB) $(ADP_LOCAL_LIB) $(AUDIO_DECRYPT_LIB) $(CRYPTO_LIB) $(CRYPTO_RANDOM_LIB) $(VIDEO_DECRYPT_LIB)
SOURCES 		:= $(ADP_LIB_SRC) $(ADP_KEY_STORE_LIB_SRC) $(ADP_LOCAL_LIB_SRC) $(AUDIO_DECRYPT_LIB_SRC) $(VIDEO_DECRYPT_LIB_SRC) $(CRYPTO_LIB_SRC) $(CRYPTO_RANDOM_LIB_SRC) 

OBJS			:= $(foreach src,$(SOURCES),$(src).o) 
ADP_OBJS		:= $(foreach src,$(ADP_LIB_SRC),$(src).o) 
ADP_LOCAL_OBJS		:= $(foreach src,$(ADP_LOCAL_LIB_SRC),$(src).o) 
ADP_KEY_STORE_OBJS	:= $(foreach src,$(ADP_KEY_STORE_LIB_SRC),$(src).o) 
AUDIO_DECRYPT_OBJS	:= $(foreach src,$(AUDIO_DECRYPT_LIB_SRC),$(src).o) 
VIDEO_DECRYPT_OBJS	:= $(foreach src,$(VIDEO_DECRYPT_LIB_SRC),$(src).o) 
CRYPTO_OBJS		:= $(foreach src,$(CRYPTO_LIB_SRC),$(src).o) 
CRYPTO_RANDOM_OBJS	:= $(foreach src,$(CRYPTO_RANDOM_LIB_SRC),$(src).o) 

%.o:	   %.c  $(HEADERS)			
	   $(CC) $(CFLAGS) $(PLATFORM) $(INCLUDE_DIRS) -c $< 
$(LIB):   $(OBJS) 
	   $(AR) rcs $(LIB) $(OBJS)

AdpLib: $(ADP_LIB)
$(ADP_LIB):   $(ADP_OBJS) 
	   $(AR) rcs $(ADP_LIB) $(ADP_OBJS)

AdpKeyStoreLib: $(ADP_KEY_STORE_LIB)
$(ADP_KEY_STORE_LIB):   $(ADP_KEY_STORE_OBJS) 
	   $(AR) rcs $(ADP_KEY_STORE_LIB) $(ADP_KEY_STORE_OBJS)

AdpLocalLib:  $(ADP_LOCAL_LIB)
$(ADP_LOCAL_LIB):   $(ADP_LOCAL_OBJS) 
	   $(AR) rcs $(ADP_LOCAL_LIB) $(ADP_LOCAL_OBJS)

AudioDecryptLib: $(AUDIO_DECRYPT_LIB)
$(AUDIO_DECRYPT_LIB):   $(AUDIO_DECRYPT_OBJS) 
	   $(AR) rcs $(AUDIO_DECRYPT_LIB) $(AUDIO_DECRYPT_OBJS)

VideoDecryptLib: $(VIDEO_DECRYPT_LIB)
$(VIDEO_DECRYPT_LIB):   $(VIDEO_DECRYPT_OBJS) 
	   $(AR) rcs $(VIDEO_DECRYPT_LIB) $(VIDEO_DECRYPT_OBJS)

CryptoLib: $(CRYPTO_LIB)
$(CRYPTO_LIB):   $(CRYPTO_OBJS) 
	   $(AR) rcs $(CRYPTO_LIB) $(CRYPTO_OBJS)

CryptoRandomLib: $(CRYPTO_RANDOM_LIB)
$(CRYPTO_RANDOM_LIB):   $(CRYPTO_RANDOM_OBJS) 
	   $(AR) rcs $(CRYPTO_RANDOM_LIB) $(CRYPTO_RANDOM_OBJS)

.PHONEY: clean
clean:
	rm -f $(OBJS) $(LIBS)
