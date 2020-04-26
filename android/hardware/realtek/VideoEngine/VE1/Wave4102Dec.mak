# ----------------------------------------------------------------------
#
# Project: C&M Video decoder sample
#
# ----------------------------------------------------------------------
.PHONY: LIBTHEORA CREATE_DIR

USE_FFMPEG  = no
USE_PTHREAD = yes

PLATFORM    = nativelinux

CROSS_CC_PREFIX = 
VDI_C           = vdi/linux/vdi.c
VDI_OSAL_C      = vdi/linux/vdi_osal.c
MM_C            = 
PLATFORM_FLAGS  = 
MAKEFILES       = Wave4102Dec.mak

ifeq ("$(BUILD_CONFIGURATION)", "NonOS")
    CROSS_CC_PREFIX = arm-none-eabi-
    VDI_C           = vdi/nonos/vdi.c
    VDI_OSAL_C      = vdi/nonos/vdi_osal.c
    MM_C            = vdi/mm.c
    USE_FFMPEG      = no
    USE_PTHREAD     = no
    PLATFORM        = none
    DEFINES         = -DLIB_C_STUB
    PLATFORM_FLAGS  = 
endif
ifeq ("$(BUILD_CONFIGURATION)", "EmbeddedLinux")
    CROSS_CC_PREFIX = arm-none-linux-gnueabi-
    PLATFORM        = armlinux
endif
ifeq ("$(BUILD_CONFIGURATION)", "RTD1295")
#    CROSS_CC_PREFIX = /home/fuchunyang/sourcecode/1295/gcc-linaro-aarch64-linux-gnu-4.8-2014.04_linux/bin/aarch64-linux-gnu-
	PLATFORM        = arm64
endif

CC  = $(CROSS_CC_PREFIX)gcc
CXX = $(CROSS_CC_PREFIX)g++
AR  = $(CROSS_CC_PREFIX)ar

INCLUDES = -I./theoraparser/include -I./vpuapi -I./ffmpeg/include -I./sample/helper 
DEFINES += -DDISABLE_KBHIT

CFLAGS  += -g -I. -Wno-implicit-function-declaration -Wno-format -Wl,--fatal-warning $(INCLUDES) $(DEFINES) $(PLATFORM_FLAGS)
ARFLAGS += cru

#LDFLAGS  = $(PLATFORM_FLAGS) -L./theoraparser/
#LDLIBS   = -ltheoraparser

ifeq ($(USE_FFMPEG), yes)
LDLIBS  += -lavformat -lavcodec -lavutil
LDFLAGS += -L./ffmpeg/lib/$(PLATFORM)
endif #USE_FFMPEG

ifeq ($(USE_PTHREAD), yes)
LDLIBS  += -lpthread
endif
LDLIBS  += -lm

BUILDLIST=DECTEST
DECTEST=w4_dec_test

OBJDIR=obj
ALLOBJS=*.o
ALLDEPS=*.dep
RM=rm -f
MKDIR=mkdir -p

SOURCES_COMMON = sample/helper/main_helper.c                \
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
		  sample/helper/display/hdmi_impl.c                 \
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
          vpuapi/wave/common/common.c                       \
		  vpuapi/wave/coda7q/coda7q.c                       \
		  vpuapi/wave/wave4/wave4.c



OBJECTNAMES_COMMON=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES_COMMON)))
OBJECTPATHS_COMMON=$(addprefix $(OBJDIR)/,$(notdir $(OBJECTNAMES_COMMON)))

SOURCES_DECTEST = sample/main_w4_dec_test.c
OBJECTNAMES_DECTEST=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES_DECTEST))) 
OBJECTPATHS_DECTEST=$(addprefix $(OBJDIR)/,$(notdir $(OBJECTNAMES_DECTEST))) $(OBJECTPATHS_COMMON)

all: $(BUILDLIST)

DECTEST: CREATE_DIR $(OBJECTPATHS_DECTEST) 
	$(CC) -o $(DECTEST) $(LDFLAGS) -Wl,-gc-section -Wl,--start-group $(OBJECTPATHS_DECTEST) $(LDLIBS) -Wl,--end-group -static


-include $(OBJECTPATHS:.o=.dep)

clean: 
	$(RM) $(TARGET)
	$(RM) $(OBJDIR)/$(ALLOBJS)
	$(RM) $(OBJDIR)/$(ALLDEPS)
#	$(RM) theoraparser/*.a
#	$(RM) theoraparser/*.o

LIBTHEORA: 
	#cd theoraparser; make
	make -C theoraparser BUILD_CONFIGURATION=$(BUILD_CONFIGURATION)

$(OBJDIR)/main_w4_dec_test.o : sample/main_w4_dec_test.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/main_helper.o : sample/helper/main_helper.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuhelper.o : sample/helper/vpuhelper.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/bitstreamfeeder.o : sample/helper/bitstream/bitstreamfeeder.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bitstreamreader.o : sample/helper/bitstream/bitstreamreader.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_fixedsize_impl.o : sample/helper/bitstream/bsfeeder_fixedsize_impl.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_framesize_impl.o : sample/helper/bitstream/bsfeeder_framesize_impl.c  $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bsfeeder_size_plus_es_impl.o : sample/helper/bitstream/bsfeeder_size_plus_es_impl.c $(MAKEFILE)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/bin_comparator_impl.o : sample/helper/comparator/bin_comparator_impl.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/comparator.o : sample/helper/comparator/comparator.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/md5_comparator_impl.o : sample/helper/comparator/md5_comparator_impl.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/yuv_comparator_impl.o : sample/helper/comparator/yuv_comparator_impl.c  $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/fbdev_impl.o : sample/helper/display/fbdev_impl.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/hdmi_impl.o : sample/helper/display/hdmi_impl.c  $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/mixer.o : sample/helper/display/mixer.c  $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/simplerenderer.o : sample/helper/display/simplerenderer.c  $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/cfgParser.o : sample/helper/misc/cfgParser.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/cnm_fpga.o : sample/helper/misc/cnm_fpga.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/cnm_video_helper.o : sample/helper/misc/cnm_video_helper.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/container.o : sample/helper/misc/container.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/datastructure.o : sample/helper/misc/datastructure.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/md5.o : sample/helper/misc/md5.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/platform.o : sample/helper/misc/platform.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)


$(OBJDIR)/yuvfeeder.o : sample/helper/yuv/yuvfeeder.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/yuvLoaderfeeder.o : sample/helper/yuv/yuvLoaderfeeder.c $(MAKEFILES)
	$(CC) $(CFLAGS) -Wall -Werror -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vdi.o : $(VDI_C) $(MAKEFILES)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vdi_osal.o : $(VDI_OSAL_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

ifneq ("$(MM_C)", "")
$(OBJDIR)/mm.o : $(MM_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)
endif

$(OBJDIR)/product.o : vpuapi/product.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuapifunc.o : vpuapi/vpuapifunc.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/vpuapi.o : vpuapi/vpuapi.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/coda9.o : vpuapi/coda9/coda9.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/common.o : vpuapi/wave/common/common.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/wave4.o : vpuapi/wave/wave4/wave4.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(OBJDIR)/coda7q.o : vpuapi/wave/coda7q/coda7q.c 
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

CREATE_DIR:
	-mkdir -p $(OBJDIR)
