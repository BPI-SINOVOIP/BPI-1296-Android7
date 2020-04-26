# includes
REFCODE_INC        = ./Source/inc
_DRM_CRYPTO_DIR    = ./Source/crypto
CRYPTO_INC         = $(_DRM_CRYPTO_DIR)/bignum
TOOLS_INC          = ./Source/tools/common

DRM_TOOLS_DIR      = tools
DRM_TEST_DIR       = test

INCLUDES += -I$(REFCODE_INC) -I$(CRYPTO_INC) -I$(TOOLS_INC)

#####################################################################
#        BIGNUM PROJECT
#####################################################################
BGN_DIR = $(_DRM_CRYPTO_DIR)/bignum

BNG_SRCS = \
	$(BGN_DIR)/bigerrno.c		\
	$(BGN_DIR)/bignum.c		\
	$(BGN_DIR)/divide.c		\
	$(BGN_DIR)/ecex2001.c		\
	$(BGN_DIR)/ecexpon.c		\
	$(BGN_DIR)/ecppq.c		\
	$(BGN_DIR)/ecurve.c		\
	$(BGN_DIR)/field.c		\
	$(BGN_DIR)/kdiv.c		\
	$(BGN_DIR)/kinitpr.c		\
	$(BGN_DIR)/kinvert.c		\
	$(BGN_DIR)/kmuladd.c		\
	$(BGN_DIR)/lucas.c		\
	$(BGN_DIR)/modexp.c		\
	$(BGN_DIR)/modmulch1.c		\
	$(BGN_DIR)/modsqrt.c		\
	$(BGN_DIR)/modular.c		\
	$(BGN_DIR)/mpaddsubcmp.c	\
	$(BGN_DIR)/mpalloc.c		\
	$(BGN_DIR)/mpgcdex.c		\
	$(BGN_DIR)/mpinit.c		\
	$(BGN_DIR)/mpmul22.c		\
	$(BGN_DIR)/mprand.c		\
	$(BGN_DIR)/multlow.c

BNG_OBJS = \
	$(BGN_DIR)/bigerrno.o		\
	$(BGN_DIR)/bignum.o		\
	$(BGN_DIR)/divide.o		\
	$(BGN_DIR)/ecex2001.o		\
	$(BGN_DIR)/ecexpon.o		\
	$(BGN_DIR)/ecppq.o		\
	$(BGN_DIR)/ecurve.o		\
	$(BGN_DIR)/field.o		\
	$(BGN_DIR)/kdiv.o		\
	$(BGN_DIR)/kinitpr.o		\
	$(BGN_DIR)/kinvert.o		\
	$(BGN_DIR)/kmuladd.o		\
	$(BGN_DIR)/lucas.o		\
	$(BGN_DIR)/modexp.o		\
	$(BGN_DIR)/modmulch1.o		\
	$(BGN_DIR)/modsqrt.o		\
	$(BGN_DIR)/modular.o		\
	$(BGN_DIR)/mpaddsubcmp.o	\
	$(BGN_DIR)/mpalloc.o		\
	$(BGN_DIR)/mpgcdex.o		\
	$(BGN_DIR)/mpinit.o		\
	$(BGN_DIR)/mpmul22.o		\
	$(BGN_DIR)/mprand.o		\
	$(BGN_DIR)/multlow.o

#####################################################################
#        PKCRYPTO PROJECT
#####################################################################
PKC_DIR = $(_DRM_CRYPTO_DIR)/pkcrypto

PKC_SRCS = $(PKC_DIR)/pkcrypto.c
PKC_OBJS = $(PKC_DIR)/pkcrypto.o

#####################################################################
#        DRMCRYPTO PROJECT
#####################################################################

DRC_DIR = $(_DRM_CRYPTO_DIR)/drmcrypto

DRC_SRCS = \
	$(DRC_DIR)/deskey.c		\
	$(DRC_DIR)/rc4.c		

DRC_OBJS = \
	$(DRC_DIR)/deskey.o		\
	$(DRC_DIR)/rc4.o	

#####################################################################
#        OEM PROJECT
#####################################################################
OEM_DIR	= ./Source/oem/ansi

OEM_SRCS = \
	$(OEM_DIR)/oemcriticalsection.c		\
	$(OEM_DIR)/oemfileio.c			\
	$(OEM_DIR)/oemimpl.c			\
	$(OEM_DIR)/oeminfo.c			\
	$(OEM_DIR)/oemtime.c			\
    	$(OEM_DIR)/rsa/DrmRsa.c 			\
    	$(OEM_DIR)/rsa/rsaimpl.c 

OEM_OBJS = \
	$(OEM_DIR)/oemcriticalsection.o		\
	$(OEM_DIR)/oemfileio.o			\
	$(OEM_DIR)/oemimpl.o			\
	$(OEM_DIR)/oeminfo.o			\
	$(OEM_DIR)/oemtime.o			\
    	$(OEM_DIR)/rsa/DrmRsa.o 			\
    	$(OEM_DIR)/rsa/rsaimpl.o 

#####################################################################
#        CORE PROJECT
#####################################################################
COR_DIR = ./Source/core

COR_SRCS = \
	$(COR_DIR)/drmbase64.c			\
	$(COR_DIR)/drmbytemanip.c               \
	$(COR_DIR)/drmcbc.c			\
	$(COR_DIR)/drmchain.c			\
	$(COR_DIR)/drmcipher.c			\
	$(COR_DIR)/drmcleanstore.c		\
	$(COR_DIR)/drmconstants.c		\
	$(COR_DIR)/drmcrt.c			\
	$(COR_DIR)/drmdebug.c			\
	$(COR_DIR)/drmexpreval.c		\
	$(COR_DIR)/drmexprfunc.c		\
	$(COR_DIR)/drmexprvariable.c		\
	$(COR_DIR)/drmhdrbuilder.c		\
	$(COR_DIR)/drmhdrparser.c		\
	$(COR_DIR)/drmhds.c			\
	$(COR_DIR)/drmhmac.c			\
	$(COR_DIR)/drmint64.c			\
	$(COR_DIR)/drmkeygen.c			\
	$(COR_DIR)/drmlicacqreq.c		\
	$(COR_DIR)/drmlicacqresp.c		\
	$(COR_DIR)/drmliceval.c			\
	$(COR_DIR)/drmlicparser.c		\
	$(COR_DIR)/drmlicstore.c		\
	$(COR_DIR)/drmmd5.c			\
	$(COR_DIR)/drmoutputlevels.c		\
	$(COR_DIR)/drmprofile.c		    	\
	$(COR_DIR)/drmsecstore.c		\
	$(COR_DIR)/drmsha1.c			\
	$(COR_DIR)/drmstkalloc.c		\
	$(COR_DIR)/drmutilities.c		\
	$(COR_DIR)/drmviewrights.c		\
	$(COR_DIR)/drmxmlbuilder.c		\
	$(COR_DIR)/drmxmlutilities.c		\
	$(COR_DIR)/drmrevocation.c		\
	$(COR_DIR)/drmxmlparser.c		\
	$(COR_DIR)/drmrsaex.c			\
	$(COR_DIR)/drmdevicestore.c

COR_OBJS = \
	$(COR_DIR)/drmbase64.o			\
	$(COR_DIR)/drmbytemanip.o               \
	$(COR_DIR)/drmcbc.o			\
	$(COR_DIR)/drmchain.o			\
	$(COR_DIR)/drmcipher.o			\
	$(COR_DIR)/drmcleanstore.o		\
	$(COR_DIR)/drmconstants.o		\
	$(COR_DIR)/drmcrt.o			\
	$(COR_DIR)/drmdebug.o			\
	$(COR_DIR)/drmexpreval.o		\
	$(COR_DIR)/drmexprfunc.o		\
	$(COR_DIR)/drmexprvariable.o		\
	$(COR_DIR)/drmhdrbuilder.o		\
	$(COR_DIR)/drmhdrparser.o		\
	$(COR_DIR)/drmhds.o			\
	$(COR_DIR)/drmhmac.o			\
	$(COR_DIR)/drmint64.o			\
	$(COR_DIR)/drmkeygen.o			\
	$(COR_DIR)/drmlicacqreq.o		\
	$(COR_DIR)/drmlicacqresp.o		\
	$(COR_DIR)/drmliceval.o			\
	$(COR_DIR)/drmlicparser.o		\
	$(COR_DIR)/drmlicstore.o		\
	$(COR_DIR)/drmmd5.o			\
	$(COR_DIR)/drmoutputlevels.o		\
	$(COR_DIR)/drmprofile.o		    	\
	$(COR_DIR)/drmsecstore.o		\
	$(COR_DIR)/drmsha1.o			\
	$(COR_DIR)/drmstkalloc.o		\
	$(COR_DIR)/drmutilities.o		\
	$(COR_DIR)/drmviewrights.o		\
	$(COR_DIR)/drmxmlbuilder.o		\
	$(COR_DIR)/drmxmlutilities.o		\
	$(COR_DIR)/drmrevocation.o		\
	$(COR_DIR)/drmxmlparser.o		\
	$(COR_DIR)/drmrsaex.o			\
	$(COR_DIR)/drmdevicestore.o



#####################################################################
#        BLACKBOX PROJECT
#####################################################################
BBX_DIR = ./Source/blackbox

BBX_SRCS = \
	$(BBX_DIR)/blackbox.c		

BBX_OBJS = \
	$(BBX_DIR)/blackbox.o		

#####################################################################
#        CMDPRSR PROJECT
#####################################################################
CMD_DIR = ./Source/cmdlnpars

CMD_SRCS = $(CMD_DIR)/cmdlnpars.c
CMD_OBJS = $(CMD_DIR)/cmdlnpars.o

#####################################################################
#        DRM MANAGER PROJECT
#####################################################################
MGR_DIR	= ./Source/drmmanager

MGR_SRCS = $(MGR_DIR)/drmmanager.c
MGR_OBJS = $(MGR_DIR)/drmmanager.o

#####################################################################
#        DEVICE DEVCERT (NON-INDIV) PROJECT
#####################################################################
DDC_DIR	= ./Source/devicedevcert

DDC_SRCS = $(DDC_DIR)/devicedevcert.c
DDC_OBJS = $(DDC_DIR)/devicedevcert.o

#####################################################################
#        DEVCERT PARSER PROJECT
#####################################################################
DCP_DIR = ./Source/devcertparser

DCP_SRCS = $(DCP_DIR)/devcert.c
DCP_OBJS = $(DCP_DIR)/devcert.o

#####################################################################
#        METERING PROJECT
#####################################################################
MTR_DIR = ./Source/metering

MTR_SRCS = \
	$(MTR_DIR)/drmmetering.c	\
	$(MTR_DIR)/drmmgrmeter.c

MTR_OBJS = \
	$(MTR_DIR)/drmmetering.o	\
	$(MTR_DIR)/drmmgrmeter.o

#####################################################################
#        SECURE CLOCK PROJECT
#####################################################################
SCK_DIR	= ./Source/secure_clock

SCK_SRCS = \
	$(SCK_DIR)/secureclockrequest.c	\
	$(SCK_DIR)/secureclockresponse.c

SCK_OBJS = \
	$(SCK_DIR)/secureclockrequest.o	\
	$(SCK_DIR)/secureclockresponse.o

#####################################################################
#        SYNCLIST PROJECT
#####################################################################
SYN_DIR = ./Source/synclist

SYN_SRCS = \
	$(SYN_DIR)/drmgensyncchallenge.o	\
	$(SYN_DIR)/drmsyncstore.o

SYN_OBJS = \
	$(SYN_DIR)/drmgensyncchallenge.o	\
	$(SYN_DIR)/drmsyncstore.o

#####################################################################
#                      WMDRMCERT PROJECT
#####################################################################
WMC_DIR = ./Source/wmdrmcert/certparser

WMC_SRCS = \
	$(WMC_DIR)/drmcertparser.c

WMC_OBJS = \
	$(WMC_DIR)/drmcertparser.o

#####################################################################
#        TOOLS COMMON PROJECT
#####################################################################
TCM_DIR = ./Source/tools/common

TCM_SCRS = 	$(TCM_DIR)/drmtoolsconstants.c	$(TCM_DIR)/drmtoolsutils.c
TCM_OBJS = 	$(TCM_DIR)/drmtoolsconstants.o	$(TCM_DIR)/drmtoolsutils.o

