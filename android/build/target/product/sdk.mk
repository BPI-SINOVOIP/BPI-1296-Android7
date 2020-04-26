#
# Copyright (C) 2014 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Don't modify this file - It's just an alias!

$(warning sdk.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/sdk_phone_armv7.mk)

PRODUCT_NAME := sdk

NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
##USE_LAYOUT_UI_1080 := true
##USE_LAYOUT_MEM_LOW := true

USE_RT_MEDIA_PLAYER := true
ENABLE_LIGHTTPD := true
ENABLE_RTK_HDMI_SERVICE := true
USE_SETTINGS_SERVICE := true
ENABLE_MEDIA_QUERY := true
ENABLE_RTK_HDMIRX_SERVICE := true
# For DRM code flow
ENABLE_DRM_PLUGIN_SERVICE := true
# For TEE OS, video used physical Address
ENABLE_TEE_DRM_FLOW	:= false

USE_RTK_HDMI_CONTROL := NO
USE_RTK_MHL_CONTROL := NO
USE_RTK_HDCP1x_CONTROL := NO
USE_RTK_HDCP22_CONTROL := NO
USE_RTK_CEC_CONTROL := NO
USE_RTK_VOUT_UTIL := YES
USE_RTK_AOUT_UTIL := YES
USE_RTK_SETUP_CLASS := NO

USE_RT_ION := true
USE_RTK_EXTRACTOR := true

USE_RTK_MEDIA_PROVIDER := YES
USE_DTCPIP_SIK := NO
USE_TV_MANAGER := YES
USE_TOGO_MANAGER := YES
USE_AIRFUN_MANAGER := YES

USE_RTK_UART_BT := YES
USE_JPU := NO
USE_RTK_PPPOE := YES
ENABLE_RT_WIFIDISPLAY := true
# for DTV
ENABLE_1195_DTV_FLOW := YES

#ENABLE_FFMPEG := true

# used for recovery mode - apply for factory update
# example: RECOVERY_FACTORY_CMD0 := rm factory_test.txt
RECOVERY_FACTORY_CMD0 := cd ../../;cp -rf tmp/install_factory/* tmp/factory/;
RECOVERY_FACTORY_CMD1 :=

RECOVERY_SECUREBOOT := 0
LAYOUT := nand

# Used for NAS OTA package
NAS_ENABLE := NO

ifeq ($(USE_SETTINGS_SERVICE), true)
USE_RTK_SETUP_CLASS := YES
PRODUCT_PROPERTY_OVERRIDES += ro.config.use_settingsservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.use_settingsservice=false
endif

ifeq ($(ENABLE_MEDIA_QUERY), true)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemediaquery=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemediaquery=false
endif

ifeq ($(USE_RTK_VOUT_UTIL), YES)
USE_RTK_HDMI_CONTROL := YES
USE_RTK_SETUP_CLASS := YES
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablevoututilservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablevoututilservice=false
endif

ifeq ($(USE_RTK_AOUT_UTIL), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enableaoututilservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enableaoututilservice=false
endif

ifeq ($(ENABLE_RTK_HDMIRX_SERVICE), true)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmirxservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmirxservice=false
endif

ifeq ($(ENABLE_RTK_HDMI_SERVICE), true)
USE_RTK_HDMI_CONTROL := YES
# Enable the following USE_RTK_MHL_CONTROL := YES when MHL is enabled.
USE_RTK_MHL_CONTROL := YES
# Enable the following USE_RTK_HDCP1x_CONTROL := YES when HDCP is enabled.
# USE_RTK_HDCP1x_CONTROL := YES
# USE_RTK_HDCP22_CONTROL := YES
USE_RTK_VOUT_UTIL := YES
USE_RTK_SETUP_CLASS := YES
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmiservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmiservice=false
endif

ifeq ($(USE_RTK_MHL_CONTROL), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemhlserivce=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemhlserivce=false
endif

ifeq ($(USE_RTK_HDCP1x_CONTROL), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdcp=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdcp=false
endif

ifeq ($(USE_RT_MEDIA_PLAYER), true)
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RTMediaPlayer=true
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RtkExtractor=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTSP_USE_RTPLAYER=true
PRODUCT_PROPERTY_OVERRIDES += persist.media.MMS_USE_RTPLAYER=true
else
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RTMediaPlayer=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RtkExtractor=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTSP_USE_RTPLAYER=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.MMS_USE_RTPLAYER=false
endif
PRODUCT_PROPERTY_OVERRIDES += persist.media.DisableInband=false

ifeq ($(USE_RTK_MEDIA_PROVIDER), YES)
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTKMediaProvider=true
else
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTKMediaProvider=false
endif
ifeq ($(ENABLE_DRM_PLUGIN_SERVICE), true)
PRODUCT_PROPERTY_OVERRIDES += drm.service.enabled=true
else
PRODUCT_PROPERTY_OVERRIDES += drm.service.enabled=false
endif

PRODUCT_PROPERTY_OVERRIDES += persist.sys.RunRTKIniSettings=false
PRODUCT_PROPERTY_OVERRIDES += ro.config.eventdelayms=33
PRODUCT_PROPERTY_OVERRIDES += persist.storage.resizefs=0
PRODUCT_PROPERTY_OVERRIDES += rtk.screen.forceOrientation=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.mb.dlna.disable=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.mb.samba.disable=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.abstract_socket=false
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.device_type=4
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.cec_vendor_id=32640
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.cec_osd_name=DMP
PRODUCT_PROPERTY_OVERRIDES += persist.sys.hdmi.addr.playback=4
PRODUCT_PROPERTY_OVERRIDES += persist.sys.hdmi.resolution=Max

# wifi switching profile:
#   native: native Android behavior, STA/P2P & SoftAP switches are independent
#   toggle: WiFi is toggled between STA/P2P & SoftAP mode
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.wifi.switchProfile=native
# wifi initial mode: sta / softap
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.wifi.initial_mode=sta
# wifi mac as default device name : true or false
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.wifi.mac.as.devicename=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.wifi.fisrt.boot=true

PRODUCT_PROPERTY_OVERRIDES += persist.rtk.ctstest=0

#ifeq ($(TARGET_BUILD_VARIANT), eng)
PRODUCT_LOCALES := en_US zh_TW zh_CN
#else # user build
#PRODUCT_LOCALES := en_US zh_TW zh_CN
#endif

ifeq ($(USE_LEANBACK_UI), true)
ifeq ($(USE_LAYOUT_UI_1080), true)
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=320
else
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=200
endif
else
ifeq ($(USE_LAYOUT_UI_1080), true)
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=240
else
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=160
endif
endif


ifeq ($(USE_LAYOUT_MEM_LOW), true)
  PRODUCT_PROPERTY_OVERRIDES += \
        dalvik.vm.heapsize=192m \
        dalvik.vm.heapgrowthlimit=192m

  PRODUCT_PROPERTY_OVERRIDES += ro.config.low_ram=true
  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.jit.codecachesize=0
else
## default ##
  PRODUCT_PROPERTY_OVERRIDES += \
        dalvik.vm.heapsize=384m \
        dalvik.vm.heapgrowthlimit=128m
endif
