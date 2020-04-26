INCLUDE(../../include/config.txt)
 
SET(MYDEFS )
SET(rm " /bin/rm -f")
SET(cp " /bin/cp -f")
SET(CC " ${MYCC}")
SET(CXX " ${MYCXX}")
SET(AR " ${MYAR} cq")
SET(RANLIB " ${MYRANLIB}")
#SET(LIBNAME ../../lib/libOSAL.a)
SET(LIBNAME OSAL)
SET(LIBRARY_OUTPUT_PATH ../../lib)


SET(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.)
SET(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
SET(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../Include/OSAL)
SET(INCLUDES  ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/IPC/include)

IF(HOST STREQUAL mipsel-linux)
SET(MYDEFS ${MYDEFS} " -D_GNU_SOURCE -D_MIPSEL_LINUX")
ENDIF(HOST STREQUAL mipsel-linux)

IF(NOT TARGET_BOARD STREQUAL NO)
SET(MYDEFS ${MYDEFS} " -DTARGET_BOARD")
ENDIF(NOT TARGET_BOARD STREQUAL NO)

SET(CFLAGS " ${DEFINES} -DSYS_UNIX=1 ${MYDEFS}")
SET(SRCS OSAL.cpp CThread.cpp PLI.cpp Threads.cpp remote_malloc_svc.c
remote_malloc_svr.c remote_malloc_xdr.c debug_level_clnt.c debug_level_xdr.c
udf_check.c IO.cpp)
SET(OBJS OSAL.o CThread.o PLI.o Threads.o remote_malloc_svc.o
remote_malloc_svr.o remote_malloc_xdr.o debug_level_clnt.o debug_level_xdr.o
udf_check.o IO.o)



string(REGEX REPLACE  ";" " " APCFG_DVR  ${APCFG_DVR})
string(REGEX REPLACE  ";" " " CHIP_MODEL_DEFS ${CHIP_MODEL_DEFS})
string(REGEX REPLACE  ";" " " APCFG_DEFS ${APCFG_DEFS})
#string(REGEX REPLACE  ";" " " BUILD_OPTIONS ${BUILD_OPTIONS})
string(REGEX REPLACE  ";" " " DEFINES ${DEFINES})
string(REGEX REPLACE  ";" " " APCFG_FLAGS ${APCFG_FLAGS})

#message(${CFLAGS})
#message(${APCFG_FLAGS})
INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS} ${WARNING} ${DEBUGFLAG})
ADD_LIBRARY(${LIBNAME} ${SRCS})
