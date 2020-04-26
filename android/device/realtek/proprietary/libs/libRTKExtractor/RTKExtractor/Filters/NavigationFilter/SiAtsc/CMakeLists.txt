INCLUDE(../../../../include/config.txt)
SET(LIBNAME SiAtsc)
SET(LIBRARY_OUTPUT_PATH ../../../../lib/)

SET(rm " /bin/rm -f")
SET(CC " ${MYCC}")
SET(LINK " -=LINK=-")
SET(DEFS )
SET(AR " ${MYAR} cq")
SET(RANLIB " ${MYRANLIB}")

SET(INCLUDES ${INCLUDES} " ${CMAKE_CURRENT_SOURCE_DIR}/.")
SET(INCLUDES ${INCLUDES} " ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include")
SET(INCLUDES ${INCLUDES} " ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include/Filters/NavigationFilter/SiAtsc")
SET(INCLUDES ${INCLUDES} " ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include/Filters/NavigationFilter/")
SET(INCLUDES ${INCLUDES} " ${CMAKE_CURRENT_SOURCE_DIR}/../Include")


SET(DEFINES ${DEFINES} " ${DEFS} -DSI_USE_FULL_VCT")


SET(CFLAGS ${CFLAGS} " ${DEFINES} ${COMPILE_OPT}")
SET(LIBS )
SET(__NAV_USE_DLL_PLUGIN__ NO)

IF(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)
SET(LIBNAME ../../../../lib/SiAtsc.so)
SET(CFLAGS ${CFLAGS} " -fPIC -D__NAV_USE_DLL_PLUGIN__")
ELSE(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)
#SET(LIBNAME ../../../../lib/libSiAtsc.a)

ENDIF(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)

#ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/)
SET(CFLAGS ${CFLAGS} " -Wall")
#SET(OBJS siOs.o siUtil.o siSystem.o siTable.o siEvent.o siSection.o siDescriptor.o siRating.o \)

SET(SRCS siOs.c siUtil.c siSystem.c siTable.c siEvent.c siSection.c
siDescriptor.c siRating.c   siChannel.c siCache.c siChannelApi.c siApi.c
siTp.c siFilterMgr.c siBean.c siBeanGen.c  siEngine.c siDvrTsApi.c siRingBuf.c
siMessage.c siConfig.c)

IF(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)
ELSE(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)
ENDIF(__NAV_USE_DLL_PLUGIN__ STREQUAL YES)


INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS} ${DEBUGFLAG} ${WARNING})
ADD_LIBRARY(${LIBNAME} ${SRCS})
string(REGEX REPLACE  ";" " " APCFG_DVR  ${APCFG_DVR})
string(REGEX REPLACE  ";" " " CHIP_MODEL_DEFS ${CHIP_MODEL_DEFS})
string(REGEX REPLACE  ";" " " APCFG_DEFS ${APCFG_DEFS})
#string(REGEX REPLACE  ";" " " BUILD_OPTIONS ${BUILD_OPTIONS})
string(REGEX REPLACE  ";" " " DEFINES ${DEFINES})
string(REGEX REPLACE  ";" " " APCFG_FLAGS ${APCFG_FLAGS})
