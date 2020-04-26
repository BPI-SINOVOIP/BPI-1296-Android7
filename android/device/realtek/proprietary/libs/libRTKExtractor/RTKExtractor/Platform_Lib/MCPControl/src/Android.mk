LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../../chip.mk

LOCAL_SRC_FILES:=                 \
	mcp_api.cpp mcp_aux.cpp mcp_bi.cpp mcp_buff.cpp mcp_cipher.cpp mcp_cipher_cts_cbc.cpp \
	mcp_cipher_cts_ecb.cpp mcp_cipher_normal.cpp mcp_cipher_widevine_cts_cbc.cpp \
	mcp_dgst.cpp mcp_rsa.cpp MCPUtil.cpp openssl_cipher_normal.cpp rcp_api.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../inc \
	$(LOCAL_PATH)/../../../ \
	$(LOCAL_PATH)/../../../Include \
	$(LOCAL_PATH)/../../../Include/IPC \
	$(LOCAL_PATH)/../../../OSAL/include \
	$(LOCAL_PATH)/../../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../../common/IPC/include \
	$(TOP)/external/boringssl/include/

LOCAL_CFLAGS += -Wno-multichar
LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)

LOCAL_CFLAGS += -Wno-error=format-security
LOCAL_CFLAGS += -Wno-error=non-virtual-dtor
LOCAL_CFLAGS += -Wno-error=address
LOCAL_CFLAGS += -Wno-error=sequence-point
LOCAL_CFLAGS += -Wno-error=return-type

LOCAL_MODULE:= libRTKMCP

include $(BUILD_STATIC_LIBRARY)
