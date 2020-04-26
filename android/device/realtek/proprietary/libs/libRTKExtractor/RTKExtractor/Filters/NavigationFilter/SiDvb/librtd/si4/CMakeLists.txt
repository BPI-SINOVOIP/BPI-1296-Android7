SET(DTV_ROOT ../..)
SET(${DTV_ROOT}/include)


INCLUDE(${DTV_ROOT}/pvr_make_config.txt)
INCLUDE(config.txt)


SET(INCLUDES ${INCLUDES} channel/ database/ util/ collector/ agent/ api/ tpInterface/${TARGET_SYSTEM})
SET(INCLUDES ${INCLUDES} agent/)

SET(T_SRCS  ${T_SRCS}  agent/SI_Agent.c)

SET(T_SRCS  ${T_SRCS}  api/SI_Api.c)
SET(T_SRCS  ${T_SRCS}  api/SI_Api_SSU.c)

SET(T_SRCS  ${T_SRCS}  channel/SI_Channel.c)

SET(T_SRCS  ${T_SRCS}  collector/SI_Collector.c)

SET(T_SRCS  ${T_SRCS}  database/SI_Descriptor.c)
SET(T_SRCS  ${T_SRCS}  database/SI_Database.c)
SET(T_SRCS  ${T_SRCS}  database/SI_DatabaseQuery.c)
SET(T_SRCS  ${T_SRCS}  database/SI_DatabaseSection.c)


SET(T_SRCS  ${T_SRCS}  util/siMessage.c)
SET(T_SRCS  ${T_SRCS}  util/siUtil_FromATSC.c)
SET(T_SRCS  ${T_SRCS}  util/split.c)


SET(T_SRCS  ${T_SRCS}  tpInterface/${TARGET_SYSTEM}/siEngine.c)
SET(T_SRCS  ${T_SRCS}  tpInterface/${TARGET_SYSTEM}/siRingBuf.c)
SET(T_SRCS  ${T_SRCS}  tpInterface/${TARGET_SYSTEM}/siBeanGen.c)
SET(T_SRCS  ${T_SRCS}  tpInterface/${TARGET_SYSTEM}/tp.c)


IF(ENABLE_SYSTEM_SOFTWARE_UPDATE STREQUAL YES)
#SET()
ENDIF(ENABLE_SYSTEM_SOFTWARE_UPDATE STREQUAL YES)


SET(LIBNAME SiDvb)
SET(LIBRARY_OUTPUT_PATH ${DTV_ROOT}/../../../../lib/)


ADD_SUBDIRECTORY(agent/siagent2_dvr)


INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS})
ADD_LIBRARY(${LIBNAME} ${T_SRCS})
