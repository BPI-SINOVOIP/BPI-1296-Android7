# ----------------------------------------------------------------------
#
# Project: C&M Video decoder sample
#
# ----------------------------------------------------------------------
.PHONY: TARGET LIBTHEORA CREATE_DIR

USE_FFMPEG  = yes
USE_PTHREAD = yes

PLATFORM    = nativelinux

CROSS_CC_PREFIX = 
VDI_C           = vdi/linux/vdi.c
VDI_OSAL_C      = vdi/linux/vdi_osal.c
MM_C            =

ifeq ("$(BUILD_CONFIGURATION)", "NonOS")
    CROSS_CC_PREFIX = arm-none-eabi-
    VDI_C           = vdi/nonos/vdi.c
    VDI_OSAL_C      = vdi/nonos/vdi_osal.c
    MM_C            = vdi/mm.c
    USE_FFMPEG      = no
    USE_PTHREAD     = no
    PLATFORM        = none
    DEFINES         = -DLIB_C_STUB
endif
ifeq ("$(BUILD_CONFIGURATION)", "EmbeddedLinux")
    CROSS_CC_PREFIX = arm-none-linux-gnueabi-
    PLATFORM        = armlinux
endif

CC  = $(CROSS_CC_PREFIX)gcc
CXX = $(CROSS_CC_PREFIX)g++
AR  = $(CROSS_CC_PREFIX)ar

INCLUDES = -I./theoraparser/include -I./vpuapi -I./ffmpeg/include -I./sample/helper
DEFINES += -DDISABLE_KBHIT
CFLAGS  += -g -I. -Wno-implicit-function-declaration -Wno-unused-result -Wno-format -Wl,--fatal-warning $(INCLUDES) $(DEFINES)
ARFLAGS += cru

LDFLAGS  = -L./theoraparser/
LDLIBS   = -ltheoraparser -lm 
ifeq ($(USE_FFMPEG), yes)
LDFLAGS += -L./ffmpeg/lib/$(PLATFORM)
LDLIBS  += -lavformat -lavcodec -lavutil
endif
ifeq ($(USE_PTHREAD), yes)
LDLIBS  += -lpthread
endif

TARGET=coda980_enc_test
OBJDIR=obj
ALLOBJS=*.o
ALLDEPS=*.dep
RM=rm -f
MKDIR=mkdir -p

SOURCES = sample/main_coda980_enc_test.c                    \
          sample/helper/main_helper.c                       \
          sample/helper/vpuhelper.c                         \
          sample/helper/bitstream/bitstreamfeeder.c         \
          sample/helper/bitstream/bitstreamreader.c         \
          sample/helper/bitstream/bsfeeder_fixedsize_impl.c \
          sample/helper/bitstream/bsfeeder_framesize_impl.c \
          sample/helper/bitstream/bsfeeder_size_plus_es_impl.c \
          sample/helper/comparator/bin_comparator_impl.c    \
          sample/helper/comparator/comparator.c             \
          sample/helper/comparator/md5_comparator_impl.c    \
          sample/helper/comparator/yuv_comparator_impl.c    \
          sample/helper/display/fbdev_impl.c                \
          sample/helper/display/simplerenderer.c            \
          sample/helper/misc/cfgParser.c                    \
          sample/helper/misc/cnm_fpga.c                     \
          sample/helper/misc/cnm_video_helper.c             \
          sample/helper/misc/container.c                    \
          sample/helper/misc/datastructure.c                \
          sample/helper/misc/platform.c                     \
          sample/helper/yuv/yuvfeeder.c                     \
          sample/helper/yuv/yuvLoaderfeeder.c               \
          $(VDI_C)                                          \
          $(VDI_OSAL_C)                                     \
          $(MM_C)                                           \
          vpuapi/product.c                                  \
          vpuapi/vpuapifunc.c                               \
          vpuapi/vpuapi.c                                   \
          vpuapi/coda9/coda9.c                              \
          vpuapi/wave4/wave4.c

OBJECTNAMES=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJECTPATHS=$(addprefix $(OBJDIR)/,$(notdir $(OBJECTNAMES)))

all: CREATE_OBJDIR LIBTHEORA $(TARGET) 

$(TARGET): $(OBJECTPATHS) 
	$(CC) -o $@ $(LDFLAGS) -Wl,-gc-section -Wl,--start-group $(OBJECTPATHS) $(LDLIBS) -Wl,--end-group

-include $(OBJECTPATHS:.o=.dep)

clean: 
	make -C theoraparser BUILD_CONFIGURATION=$(BUILD_CONFIGURATION) clean
	$(RM) $(OBJDIR)/$(ALLDEPS)
	$(RM) $(OBJDIR)/$(ALLOBJS)

LIBTHEORA: 
	make -C theoraparser BUILD_CONFIGURATION=$(BUILD_CONFIGURATION)


$(OBJDIR)/main_coda980_enc_test.o : sample/main_coda980_enc_test.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/main_helper.o : sample/helper/main_helper.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuhelper.o : sample/helper/vpuhelper.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/bitstreamfeeder.o : sample/helper/bitstream/bitstreamfeeder.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bitstreamreader.o : sample/helper/bitstream/bitstreamreader.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_fixedsize_impl.o : sample/helper/bitstream/bsfeeder_fixedsize_impl.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_framesize_impl.o : sample/helper/bitstream/bsfeeder_framesize_impl.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_size_plus_es_impl.o : sample/helper/bitstream/bsfeeder_size_plus_es_impl.c $(MAKEFILE)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bin_comparator_impl.o : sample/helper/comparator/bin_comparator_impl.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/comparator.o : sample/helper/comparator/comparator.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/md5_comparator_impl.o : sample/helper/comparator/md5_comparator_impl.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/yuv_comparator_impl.o : sample/helper/comparator/yuv_comparator_impl.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/fbdev_impl.o : sample/helper/display/fbdev_impl.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/mixer.o : sample/helper/display/mixer.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/simplerenderer.o : sample/helper/display/simplerenderer.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/cfgParser.o : sample/helper/misc/cfgParser.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/cnm_fpga.o : sample/helper/misc/cnm_fpga.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/cnm_video_helper.o : sample/helper/misc/cnm_video_helper.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/container.o : sample/helper/misc/container.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/datastructure.o : sample/helper/misc/datastructure.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/platform.o : sample/helper/misc/platform.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/yuvfeeder.o : sample/helper/yuv/yuvfeeder.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/yuvLoaderfeeder.o : sample/helper/yuv/yuvLoaderfeeder.c Coda980Enc.mak
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vdi.o : $(VDI_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vdi_osal.o : $(VDI_OSAL_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

ifneq ("$(MM_C)", "")
$(OBJDIR)/mm.o : $(MM_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)
endif

$(OBJDIR)/product.o : vpuapi/product.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuapifunc.o : vpuapi/vpuapifunc.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuapi.o : vpuapi/vpuapi.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/coda9.o : vpuapi/coda9/coda9.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/wave4.o : vpuapi/wave4/wave4.c Coda980Enc.mak 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

CREATE_OBJDIR:
	-mkdir -p $(OBJDIR)

force_dependency : 
	true
