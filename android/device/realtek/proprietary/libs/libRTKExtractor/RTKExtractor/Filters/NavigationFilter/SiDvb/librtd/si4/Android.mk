LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../../../chip.mk

LOCAL_SRC_FILES:=                 \
	channel/SI_Channel.c	\
	database/SI_Database.c database/SI_DatabaseQuery.c database/SI_DatabaseSection.c database/SI_Descriptor.c	\
	collector/SI_Collector.c	\
	agent/SI_Agent.c	\
	agent/siagent2_dvr/siAgentCustomize.c agent/siagent2_dvr/siAgentFilterTaskExecutor.c \
	agent/siagent2_dvr/siAgentInterface.c agent/siagent2_dvr/siAgentStateMachine.c agent/siagent2_dvr/siAgentUtil.c	\
	agent/siagent2_dvr/siAgentVirSecFilterMgr.c agent/siagent2_dvr/siFunctionWrapper.c agent/siagent2_dvr/siAgentObject.c \
	api/SI_Api.c api/SI_Api_SSU.c	\
	util/siMessage.c util/siUtil_FromATSC.c util/split.c	\
	tpInterface/PVR/siBeanGen.c tpInterface/PVR/siEngine.c tpInterface/PVR/siRingBuf.c tpInterface/PVR/tp.c

#ifdef SI_ENABLE_SYSTEM_SOFTWARE_UPDATE need add 
# agent/siagent2_dvr/ssu/ssuBlock.c agent/siagent2_dvr/ssu/ssuDcr.c agent/siagent2_dvr/ssu/ssuDdb.c agent/siagent2_dvr/ssu/ssuDdb.c
# agent/siagent2_dvr/ssu/ssuDii.c agent/siagent2_dvr/ssu/ssuDsi.c agent/siagent2_dvr/ssu/ssuDtr.c agent/siagent2_dvr/ssu/ssuFunctionWrapper.c
# agent/siagent2_dvr/ssu/ssuGroup.c agent/siagent2_dvr/ssu/ssuLib.c agent/siagent2_dvr/ssu/ssuModule.c agent/siagent2_dvr/ssu/ssuUnt.c

	

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/ \
	$(LOCAL_PATH)/../../../../../ \
	$(LOCAL_PATH)/../../../../../Include/Filters/NavigationFilter/ \
	$(LOCAL_PATH)/../../../../../Include/Filters/NavigationFilter/SiDvb/include	\
	$(LOCAL_PATH)/../../../../../Include \
	$(LOCAL_PATH)/../../../../../Include/IPC \
	$(LOCAL_PATH)/../../../../../OSAL/include \
	$(LOCAL_PATH)/../../../../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../../../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../../../../common/IPC/include \
	$(LOCAL_PATH)/../../../../../common/IPC/generate/include/system	\
	$(LOCAL_PATH)/database $(LOCAL_PATH)/tpInterface/PVR



LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)

LOCAL_CFLAGS += -Wno-error=format-security
LOCAL_CFLAGS += -Wno-error=non-virtual-dtor
LOCAL_CFLAGS += -Wno-error=address
LOCAL_CFLAGS += -Wno-error=sequence-point
LOCAL_CFLAGS += -Wno-error=return-type
LOCAL_CFLAGS += "-DCHECKCRC=0"
LOCAL_CFLAGS += -DREMOVE_GLOBAL_VARIABLE
# Debug Used 
#LOCAL_CFLAGS += "-D_SI_CHECK_TS_NODE -D_DEBUG_SIAGENT_SM_ -D_SI_TP_DEBUG_ -D_SI_DEBUG_SIAGENT_SM_DETAIL_ -D_SI_CHECK_CH_NODE -D_SI_CHECK_VC_NODE"
LOCAL_CFLAGS += "-DSI_CHECK_RINGBUFFER_WRITE_LEN"

# For SI Scan & Parser, very important
LOCAL_CFLAGS += "-DSUPPORT_UNDEFINED_SERVICE_TYPE"


LOCAL_MODULE:= libRTKSiDvb

include $(BUILD_STATIC_LIBRARY)

