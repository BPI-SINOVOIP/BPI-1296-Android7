##############################################
# Tool Chain
##############################################
include ../../../include/MakeConfig
#MAKEFLAGS = --no-print-directory
MYCC ?= mipsel-linux-gcc

##############################################
# PATH
##############################################
MYLIB            ?= libMCP.a
EXPORT_LIB_PATH  ?= ../../../lib/
EXPORT_INC_PATH  ?= ../../Include/Platform_Lib/MCPControl
OSAL_INC         := ../../Include/
OPENSSL_INC      := ../openssl/include/
CONFIG_FILE      := mcp_config.h


##############################################
# Environment
##############################################
SRC         = 
OBJ         = 
INC         = 
DBG_DEF     =
INC_DEF     = -I inc/ -I $(OSAL_INC) 
SUBDIRS     = 
VPATH       = obj inc src 
EXPORT_INC  = 
CFLAGS     += -g
CFLAGS     += $(CHIP_MODEL_DEFS)
MCP_CONFIG  =

##############################################
# Depandency
##############################################

ifeq ($(USE_WEBKIT_VIDEO_EXTENSION),YES)
CFLAGS+= -fPIC
endif

RCP_API     = y
RCP_USE_PLI ?= n
MCP_HW_API  = y
MCP_CIPHER  = y
MCP_DGST    = y
MCP_RSA     = y
MCP_USE_PLI ?= y
MCP_OPENSSL_SUPPORT ?= n
MCP_WIDEVINE_CBC_CTS_SUPPORT ?= y

OBJ         +=  MCPUtil.o mcp_aux.o
EXPORT_INC  +=  MCPUtil.h mcp_aux.h mcp_config.h

ifeq ($(MCP_CIPHER), y)
OBJ         +=  mcp_cipher.o mcp_cipher_normal.o mcp_cipher_cts_cbc.o mcp_cipher_cts_ecb.o 
EXPORT_INC  +=  mcp_cipher.h
endif

ifeq ($(MCP_DGST), y)
OBJ         +=  mcp_dgst.o mcp_buff.o
EXPORT_INC  +=  mcp_dgst.h mcp_buff.h
endif

ifeq ($(MCP_RSA), y)
OBJ         +=  mcp_rsa.o mcp_bi.o
EXPORT_INC  +=  mcp_rsa.h mcp_bi.h
endif

ifeq ($(RCP_API), y)
OBJ         +=  rcp_api.o
EXPORT_INC  +=  rcp_api.h 
endif


ifeq ($(MCP_HW_API), y)
OBJ         +=  mcp_api.o
EXPORT_INC  +=  mcp_api.h
endif

ifeq ($(MCP_USE_PLI), y)  
  MCP_CONFIG += MCP_USE_PLI
endif

ifeq ($(MCP_OPENSSL_SUPPORT), y)
  INC_DEF += -I $(OPENSSL_INC) 
  OBJ     +=  openssl_cipher_normal.o
  MCP_CONFIG += MCP_OPENSSL_SUPPORT
endif

ifeq ($(MCP_WIDEVINE_CBC_CTS_SUPPORT), y)
  OBJ     +=  mcp_cipher_widevine_cts_cbc.o
  MCP_CONFIG += MCP_WIDEVINE_CBC_CTS_SUPPORT
endif

ifeq ($(RCP_USE_PLI), y)  
  CFLAGS  += -DRCP_USE_PLI
endif

##############################################
# Make
##############################################

.NOTPARALLEL:

all : create_config rm_lib export_header subdir $(OBJ)
	@for i in $(OBJ); do $(MYAR) rcv $(EXPORT_LIB_PATH)/$(MYLIB) obj/$$i; done;
	
.c.o:
	@$(MYCC) $(CFLAGS) $(INC_DEF) $(DBG_DEF) -c $< -o obj/$@
	
.cpp.o:
	@$(MYCC) $(CFLAGS) $(INC_DEF) $(DBG_DEF) -c $< -o obj/$@	
		
subdir:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done;			
	
export_header: 	
	@mkdir -p $(EXPORT_INC_PATH)
	@rm -f $(EXPORT_INC_PATH)/*
	@for i in $(EXPORT_INC); do cp -f inc/$$i $(EXPORT_INC_PATH); done;

create_config:
	@rm -f inc/$(CONFIG_FILE)
	@echo "#ifndef __MCP_CONFIG_H__" >> inc/$(CONFIG_FILE)
	@echo "#define __MCP_CONFIG_H__" >> inc/$(CONFIG_FILE)
	@echo "" >> inc/$(CONFIG_FILE)
	@set -e; for i in $(sort $(MCP_CONFIG)); do echo "#define "$$i >> inc/$(CONFIG_FILE); done;
	@echo "" >> inc/$(CONFIG_FILE)
	@echo "#endif //__MCP_CONFIG_H__" >> inc/$(CONFIG_FILE)
	@echo "" >> inc/$(CONFIG_FILE)

rm_lib:
	@$(RM) -f $(MYLIB)

clean: 	
	@set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done;	
	@$(RM) -f src/*~ src/*.bak 	
	@$(RM) -f inc/*~ inc/*.bak
	@$(RM) -f obj/*	
	@$(RM) -f *~ *.bak 

help:   
	@echo "make clean all MCP_USE_PLI=<n/y> EXPORT_LIB_PATH=<lib path> EXPORT_INC_PATH=<inc path>"
