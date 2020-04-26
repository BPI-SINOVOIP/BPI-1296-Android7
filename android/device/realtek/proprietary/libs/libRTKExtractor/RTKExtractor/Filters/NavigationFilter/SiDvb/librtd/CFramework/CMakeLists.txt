SET(DTV_ROOT ../..)
SET(INC_DIR ${DTV_ROOT}/include)


INCLUDE(${DTV_ROOT}/pvr_make_config.txt)

SET(CFLAGS " -g -Wall -DCF_LOGGER_COLOR")
SET(INCLUDES ${INC_DIR} ${DTV_ROOT}/../../../Include/Filters/NavigationFilter/SiDvb/include)


SET(SRCS ${SRCS} CFramework.c)
SET(SRCS ${SRCS} memory/CF_Memory.c)
SET(SRCS ${SRCS} timer/CF_Timer.c)
SET(SRCS ${SRCS} message/CF_Message.c)
SET(SRCS ${SRCS} logger/CF_Logger.c)

SET(LIBNAME CF)
SET(LIBRARY_OUTPUT_PATH ${DTV_ROOT}/../../../../lib/)

INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS})
ADD_LIBRARY(${LIBNAME} ${SRCS})

