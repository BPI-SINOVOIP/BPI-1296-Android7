.PHONY: CREATE_DIR

PLATFORM        = arm64
MAKEFILES       = Vp9Dec.mk

CC  = $(CROSS_CC_PREFIX)gcc
CXX = $(CROSS_CC_PREFIX)g++
AR  = $(CROSS_CC_PREFIX)ar

INCLUDES = -I./
INCLUDES += -I./software/linux/memalloc
INCLUDES += -I./software/linux/ldriver
INCLUDES += -I./software/source/inc
INCLUDES += -I./software/source/config
INCLUDES += -I./software/source/common
INCLUDES += -I./software/source/vp9
INCLUDES += -I./software/test/common
INCLUDES += -I./software/test/common/swhw
INCLUDES += -I./software/test/vp9
DEFINES += -DDISABLE_KBHIT
CFLAGS  += -g -I. -Wno-implicit-function-declaration -Wno-unused-result -Wno-format -Wl,--fatal-warning $(INCLUDES) $(DEFINES)
CFLAGS += -DCONFIG_DEBUG_LEVEL=255
CFLAGS += -DFIFO_DATATYPE=void*
CFLAGS += -D_ASSERT_USED
CFLAGS += -DDOWN_SCALER
CFLAGS += -DCLEAR_OUT_BUFFER
CFLAGS += -DDEC_X170_OUTPUT_FORMAT=0
CFLAGS += -DDEC_X170_TIMEOUT_LENGTH=-1
CFLAGS += -DDWL_DISABLE_REG_PRINTS
CFLAGS += -DENABLE_VP9_SUPPORT
#CFLAGS += -D_DWL_DEBUG
CFLAGS += -DDWL_USE_DEC_IRQ
CFLAGS += -DUSE_EXTERNAL_BUFFER
#CFLAGS += -DMEMORY_USAGE_TRACE
CFLAGS += -DUSE_64BIT_ENV
#CFLAGS += -D_SW_DEBUG_PRINT

ARFLAGS += cru

LDLIBS  += -lm
LDLIBS  += -lpthread

TARGET=vp9dec
OBJDIR=obj
ALLOBJS=*.o
ALLDEPS=*.dep
RM=rm -f
MKDIR=mkdir -p

SOURCES = software/test/common/bytestream_parser.c
SOURCES += software/test/common/command_line_parser.c
SOURCES += software/test/common/error_simulator.c
SOURCES += software/test/common/file_sink.c
SOURCES += software/test/common/md5_sink.c
SOURCES += software/test/common/null_sink.c
SOURCES += software/test/common/null_trace.c
SOURCES += software/test/common/swhw/md5.c
SOURCES += software/test/common/swhw/tb_cfg.c
SOURCES += software/test/common/swhw/tb_md5.c
SOURCES += software/test/common/swhw/tb_params.c
SOURCES += software/test/common/swhw/tb_stream_corrupt.c
SOURCES += software/test/common/swhw/tb_tiled.c
SOURCES += software/test/common/vpxfilereader.c
SOURCES += software/test/common/yuvfilters.c
SOURCES += software/test/common/dectestbench.c
SOURCES += software/source/common/decapi.c
SOURCES += software/source/common/bqueue.c
SOURCES += software/source/common/commonconfig.c
SOURCES += software/source/common/fifo.c
SOURCES += software/source/common/raster_buffer_mgr.c
SOURCES += software/source/common/regdrv.c
SOURCES += software/source/common/sw_stream.c
SOURCES += software/source/common/sw_util.c
SOURCES += software/source/vp9/vp9_entropymode.c
SOURCES += software/source/vp9/vp9_entropymv.c
SOURCES += software/source/vp9/vp9_modecont.c
SOURCES += software/source/vp9/vp9_modecontext.c
SOURCES += software/source/vp9/vp9_treecoder.c
SOURCES += software/source/vp9/vp9decapi.c
SOURCES += software/source/vp9/vp9hwd_asic.c
SOURCES += software/source/vp9/vp9hwd_bool.c
SOURCES += software/source/vp9/vp9hwd_decoder.c
SOURCES += software/source/vp9/vp9hwd_buffer_queue.c
SOURCES += software/source/vp9/vp9hwd_headers.c
SOURCES += software/source/vp9/vp9hwd_output.c
SOURCES += software/source/vp9/vp9hwd_probs.c
SOURCES += software/linux/dwl/dwl_activity_trace.c
SOURCES += software/linux/dwl/dwl_linux.c
SOURCES += software/linux/dwl/dwl_linux_hw.c
SOURCES += software/linux/dwl/dwl_buf_protect.c

OBJECTNAMES=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJECTPATHS=$(addprefix $(OBJDIR)/,$(notdir $(OBJECTNAMES)))

all: CREATE_OBJDIR $(TARGET) 

$(TARGET): $(OBJECTPATHS) 
	$(CC) -o $@ $(LDFLAGS) -Wl,-gc-section -Wl,--start-group $(OBJECTPATHS) $(LDLIBS) -Wl,--end-group -static

-include $(OBJECTPATHS:.o=.dep)

clean: 
#	make -C theoraparser BUILD_CONFIGURATION=$(BUILD_CONFIGURATION) clean
	$(RM) $(OBJDIR)/$(ALLDEPS)
	$(RM) $(OBJDIR)/$(ALLOBJS)

$(OBJDIR)/bytestream_parser.o   : software/test/common/bytestream_parser.c      
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/command_line_parser.o : software/test/common/command_line_parser.c    
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/error_simulator.o     : software/test/common/error_simulator.c        
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/file_sink.o           : software/test/common/file_sink.c                
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/md5_sink.o            : software/test/common/md5_sink.c                 
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/null_sink.o           : software/test/common/null_sink.c                
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/null_trace.o          : software/test/common/null_trace.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/md5.o                 : software/test/common/swhw/md5.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/tb_cfg.o              : software/test/common/swhw/tb_cfg.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/tb_md5.o              : software/test/common/swhw/tb_md5.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/tb_params.o           : software/test/common/swhw/tb_params.c         
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/tb_stream_corrupt.o   : software/test/common/swhw/tb_stream_corrupt.c 
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/tb_tiled.o            : software/test/common/swhw/tb_tiled.c          
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vpxfilereader.o       : software/test/common/vpxfilereader.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/yuvfilters.o          : software/test/common/yuvfilters.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/dectestbench.o        : software/test/common/dectestbench.c           
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/decapi.o              : software/source/common/decapi.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/bqueue.o              : software/source/common/bqueue.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/commonconfig.o        : software/source/common/commonconfig.c         
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/fifo.o                : software/source/common/fifo.c                 
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/raster_buffer_mgr.o   : software/source/common/raster_buffer_mgr.c    
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/regdrv.o              : software/source/common/regdrv.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/sw_stream.o           : software/source/common/sw_stream.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/sw_util.o             : software/source/common/sw_util.c              
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9_entropymode.o     : software/source/vp9/vp9_entropymode.c         
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9_entropymv.o       : software/source/vp9/vp9_entropymv.c           
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9_modecont.o        : software/source/vp9/vp9_modecont.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9_modecontext.o     : software/source/vp9/vp9_modecontext.c         
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9_treecoder.o       : software/source/vp9/vp9_treecoder.c           
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9decapi.o           : software/source/vp9/vp9decapi.c               
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_asic.o         : software/source/vp9/vp9hwd_asic.c             
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_bool.o         : software/source/vp9/vp9hwd_bool.c             
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_decoder.o      : software/source/vp9/vp9hwd_decoder.c          
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_buffer_queue.o : software/source/vp9/vp9hwd_buffer_queue.c     
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_headers.o      : software/source/vp9/vp9hwd_headers.c          
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_output.o       : software/source/vp9/vp9hwd_output.c           
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/vp9hwd_probs.o        : software/source/vp9/vp9hwd_probs.c            
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/dwl_activity_trace.o  : software/linux/dwl/dwl_activity_trace.c       
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/dwl_linux.o           : software/linux/dwl/dwl_linux.c                
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/dwl_linux_hw.o        : software/linux/dwl/dwl_linux_hw.c             
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)
$(OBJDIR)/dwl_buf_protect.o     : software/linux/dwl/dwl_buf_protect.c             
	$(CC) $(CFLAGS) -Wall -c $< -o $@ -MD -MF $(@:.o=.dep)

CREATE_OBJDIR:
	-mkdir -p $(OBJDIR)

force_dependency :
	true
