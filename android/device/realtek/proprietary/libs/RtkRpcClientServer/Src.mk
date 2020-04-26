################################################################################
# Compiler Options
################################################################################
LOCAL_LDLIBS += -llog
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
MY_LOCAL_C_INCLUDES := 
LOCAL_C_INCLUDES := $(addprefix $(LOCAL_PATH)/,$(MY_LOCAL_C_INCLUDES)) $(STL_INC_DIR) 
LOCAL_CFLAGS := -Wall -g

################################################################################


################################################################################
# Framework source code - IPC
################################################################################
LOCAL_SRC_FILES += ipc/client/IpcClientFactory.cpp
LOCAL_SRC_FILES += ipc/client/pipe/IpcClientPipeImpl.cpp
LOCAL_SRC_FILES += ipc/general/pipe/IpcPipePath.cpp
LOCAL_SRC_FILES += ipc/general/pipe/IpcStreamerPipeImpl.cpp
LOCAL_SRC_FILES += ipc/general/pipe/IpcStreamerRefCnt.cpp

################################################################################


################################################################################
# Framework source code - OS
################################################################################
LOCAL_SRC_FILES += os/OSAL.cpp

################################################################################


################################################################################
# Framework source code - RPC
################################################################################
LOCAL_SRC_FILES += rpc/RpcClient.cpp
LOCAL_SRC_FILES += rpc/RpcCommandDemuxer.cpp
LOCAL_SRC_FILES += rpc/RpcCommandMuxer.cpp
LOCAL_SRC_FILES += rpc/RpcExecutorManager.cpp

################################################################################


################################################################################
# Framework source code - Tools
################################################################################
LOCAL_SRC_FILES += tools/LogTools.cpp
LOCAL_SRC_FILES += tools/StrLib.cpp
LOCAL_SRC_FILES += tools/TimerObject.cpp

################################################################################
