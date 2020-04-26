LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
	stagefright.cpp \
	jpeg.cpp	\
	SineSource.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia libutils libbinder libstagefright_foundation \
	libjpeg libgui libcutils liblog

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	frameworks/av/media/libstagefright/include \
	$(TOP)/frameworks/native/include/media/openmax \
	external/jpeg \

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true
LOCAL_CFLAGS += -DRTK_PLATFORM

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= stagefright

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
	SineSource.cpp    \
	record.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia liblog libutils libbinder libstagefright_foundation

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax \
	$(TOP)/frameworks/native/include/media/hardware

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= record

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
	SineSource.cpp    \
	recordvideo.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia liblog libutils libbinder libstagefright_foundation

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax \
	$(TOP)/frameworks/native/include/media/hardware

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= recordvideo

include $(BUILD_EXECUTABLE)


################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
	SineSource.cpp    \
	audioloop.cpp

LOCAL_SHARED_LIBRARIES := \
	libstagefright libmedia liblog libutils libbinder libstagefright_foundation

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= audioloop

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
        stream.cpp    \

LOCAL_SHARED_LIBRARIES := \
	libstagefright liblog libutils libbinder libgui \
	libstagefright_foundation libmedia libcutils

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= stream

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=         \
	sf2.cpp    \

LOCAL_SHARED_LIBRARIES := \
	libstagefright liblog libutils libbinder libstagefright_foundation \
	libmedia libgui libcutils libui

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= sf2

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
	codec.cpp               \
	SimplePlayer.cpp        \

LOCAL_SHARED_LIBRARIES := \
	libstagefright liblog libutils libbinder libstagefright_foundation \
	libmedia libgui libcutils libui

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= codec

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	filters/argbtorgba.rs \
	filters/nightvision.rs \
	filters/saturation.rs \
	mediafilter.cpp \

LOCAL_SHARED_LIBRARIES := \
	libstagefright \
	liblog \
	libutils \
	libbinder \
	libstagefright_foundation \
	libmedia \
	libgui \
	libcutils \
	libui \
	libRScpp \

LOCAL_C_INCLUDES:= \
	$(TOP)/frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax \
	$(TOP)/frameworks/rs/cpp \
	$(TOP)/frameworks/rs \

intermediates := $(call intermediates-dir-for,STATIC_LIBRARIES,libRS,TARGET,)
LOCAL_C_INCLUDES += $(intermediates)

LOCAL_STATIC_LIBRARIES:= \
	libstagefright_mediafilter

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= mediafilter

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
	muxer.cpp            \

LOCAL_SHARED_LIBRARIES := \
	libstagefright liblog libutils libbinder libstagefright_foundation \
	libmedia libgui libcutils libui libc

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= muxer

include $(BUILD_EXECUTABLE)

################################################################################

include $(CLEAR_VARS)

MS12_PLAYER_ENABLE = true
MS12_PLAYER_FILEWRITE_ENABLE = true
MS12_PLAYER_TXT_CMD_ENABLE = true
MS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST_ENABLE = false
MS12_PLAYER_V1_3_ENABLE = true

LOCAL_SRC_FILES:=               \
	ms12codec.cpp               \
	SimplePlayer.cpp        \

LOCAL_SHARED_LIBRARIES := \
	libstagefright liblog libutils libbinder libstagefright_foundation \
	libmedia libgui libcutils libui

LOCAL_C_INCLUDES:= \
	frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax

ifeq ($(MS12_PLAYER_ENABLE),true)
LOCAL_CFLAGS += -DMS12_PLAYER_EN -DDLB_GETPARAM_TIMESTAMP -DMS12_7_1_SUPPORT -DMS12_AC4_SUPPORT
LOCAL_CFLAGS += -DDLB_GETPARAM_MAXLEN=4096 -DDLB_GETPARAM_MAX_SWITCHES=40

ifeq ($(MS12_PLAYER_V1_1_ENABLE),true)
LOCAL_C_INCLUDES += ms12
LOCAL_CFLAGS += -DMS12V1_1_EN
LOCAL_SRC_FILES +=  ms12/ms12_user.cpp ms12/ms12_err.cpp ms12/ms12_getparam.cpp ms12/ms12_update_param.cpp ms12/ms12_cmd.cpp
endif

ifeq ($(MS12_PLAYER_V1_3_ENABLE),true)
LOCAL_C_INCLUDES += ms12v1_3
LOCAL_CFLAGS += -DMS12V1_3_EN -DSWITCH_FILE_EN
LOCAL_SRC_FILES +=  ms12v1_3/ms12_user.cpp ms12v1_3/ms12_err.cpp ms12v1_3/ms12_getparam.cpp ms12v1_3/ms12_update_param.cpp ms12v1_3/ms12_usage.cpp ms12v1_3/ms12_cmd.cpp
endif

ifeq ($(MS12_PLAYER_FILEWRITE_ENABLE),true)
LOCAL_CFLAGS += -DMS12_FILE_WRITE_EN
endif
ifeq ($(MS12_PLAYER_TXT_CMD_ENABLE),true)
LOCAL_CFLAGS += -DMS12_PLAYER_USE_TXT_CMD
endif
ifeq ($(MS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST_ENABLE),true)
LOCAL_CFLAGS += -DMS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST
endif
endif

LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
LOCAL_CLANG := true

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= ms12codec

include $(BUILD_EXECUTABLE)


################################################################################
#
#include $(CLEAR_VARS)
#
#MS12_PLAYER_ENABLE = true
#MS12_PLAYER_FILEWRITE_ENABLE = true
#MS12_PLAYER_TXT_CMD_ENABLE = true
#MS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST_ENABLE = false
#MS12_PLAYER_V1_1_ENABLE = false
#MS12_PLAYER_V1_3_ENABLE = true
#
#LOCAL_SRC_FILES:=       \
#	ms12player.cpp \
#
#LOCAL_SHARED_LIBRARIES := \
#	libstagefright libmedia libutils libbinder libstagefright_foundation \
#        libjpeg libgui libcutils liblog
#
#LOCAL_C_INCLUDES:= \
#	frameworks/av/media/libstagefright \
#	frameworks/av/media/libstagefright/include \
#	$(TOP)/frameworks/native/include/media/openmax \
#	external/jpeg \
#
#ifeq ($(MS12_PLAYER_ENABLE),true)
#LOCAL_CFLAGS += -DMS12_PLAYER_EN -DDLB_GETPARAM_TIMESTAMP -DMS12_7_1_SUPPORT -DMS12_AC4_SUPPORT
#LOCAL_CFLAGS += -DDLB_GETPARAM_MAXLEN=4096 -DDLB_GETPARAM_MAX_SWITCHES=40
##LOCAL_SRC_FILES +=  ms12/ms12_user.cpp ms12/ms12_err.cpp ms12/ms12_getparam.cpp ms12/ms12_update_param.cpp
##LOCAL_C_INCLUDES += ms12
#
#ifeq ($(MS12_PLAYER_V1_1_ENABLE),true)
#LOCAL_C_INCLUDES += ms12
#LOCAL_CFLAGS += -DMS12V1_1_EN
#LOCAL_SRC_FILES +=  ms12/ms12_user.cpp ms12/ms12_err.cpp ms12/ms12_getparam.cpp ms12/ms12_update_param.cpp ms12/ms12_cmd.cpp
#endif
#
#ifeq ($(MS12_PLAYER_V1_3_ENABLE),true)
#LOCAL_C_INCLUDES += ms12v1_3
#LOCAL_CFLAGS += -DMS12V1_3_EN
#LOCAL_SRC_FILES +=  ms12v1_3/ms12_user.cpp ms12v1_3/ms12_err.cpp ms12v1_3/ms12_getparam.cpp ms12v1_3/ms12_update_param.cpp ms12v1_3/ms12_usage.cpp ms12v1_3/ms12_cmd.cpp
#endif
#
#ifeq ($(MS12_PLAYER_FILEWRITE_ENABLE),true)
#LOCAL_CFLAGS += -DMS12_FILE_WRITE_EN
#endif
#ifeq ($(MS12_PLAYER_TXT_CMD_ENABLE),true)
#LOCAL_CFLAGS += -DMS12_PLAYER_USE_TXT_CMD
#endif
#ifeq ($(MS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST_ENABLE),true)
#LOCAL_CFLAGS += -DMS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST
#endif
#endif
#
#LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
#LOCAL_CLANG := true
#
#LOCAL_MODULE_TAGS := optional
#
#LOCAL_MODULE:= ms12player
#
#include $(BUILD_EXECUTABLE)
#
#################################################################################
#
#include $(CLEAR_VARS)
#
#LOCAL_SRC_FILES:=       \
#	udcplayer.cpp \
#
#LOCAL_SHARED_LIBRARIES := \
#	libstagefright libmedia libutils libbinder libstagefright_foundation \
#        libjpeg libgui libcutils liblog
#
#LOCAL_C_INCLUDES:= \
#	frameworks/av/media/libstagefright \
#	frameworks/av/media/libstagefright/include \
#	$(TOP)/frameworks/native/include/media/openmax \
#	external/jpeg \
#
#
#LOCAL_CFLAGS += -Wno-multichar -Werror -Wall
#LOCAL_CLANG := true
#
#LOCAL_MODULE_TAGS := optional
#
#LOCAL_MODULE:= udcplayer
#
#include $(BUILD_EXECUTABLE)
#
