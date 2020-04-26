SET(DTV_ROOT_DIR ../../../..)
SET(DTV_ROOT ${DTV_ROOT_DIR})
SET(INC_DIR ${DTV_ROOT}/include)


INCLUDE(${DTV_ROOT_DIR}/pvr_make_config.txt)
INCLUDE(${DTV_ROOT}/librtd/si4/config.txt)

SET(CFLAGS " -g -Wall")
#SET(INCLUDES ${INCLUDES} ../)
SET(INCLUDES ${INCLUDES} ../../)
SET(INCLUDES ${INCLUDES} ${DTV_ROOT_DIR}/include)
SET(INCLUDES ${INCLUDES} ${DTV_ROOT_DIR}/../../../Include/Filters/NavigationFilter/SiDvb/include)
SET(INCLUDES ${INCLUDES} ${DTV_ROOT_DIR}/../../../Include/Filters/NavigationFilter)


SET(LIBNAME SiDvbAgent)
SET(LIBRARY_OUTPUT_PATH ${DTV_ROOT_DIR}/../../../../lib/)


SET(SRCS siAgentCustomize.c  siAgentInterface.c  siAgentUtil.c siAgentVirSecFilterMgr.c siAgentObject.c  siFunctionWrapper.c siAgentFilterTaskExecutor.c siAgentStateMachine.c)



IF(SI_ENABLE_SYSTEM_SOFTWARE_UPDATE STREQUAL YES)
  	FILE(GLOB SSU_SRCS ssu/*.c)
	 SET(SRCS ${SRCS} ${SSU_SRCS})
	message("@@@@@@@@@${SRCS}")
ENDIF(SI_ENABLE_SYSTEM_SOFTWARE_UPDATE STREQUAL YES)

#message("@@@@@@@@@${SRCS}")
INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS} ${DEFINES})
ADD_LIBRARY(${LIBNAME} ${SRCS})
