#
# Copyright (C) 2011 The Android Open-Source Project
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

# common setting
include device/realtek/kylin/common/BoardConfigCommon.mk

# wifi setting
BOARD_HAVE_WIFI := false
BOARD_WIFI_VENDOR := realtek
ifeq ($(BOARD_WIFI_VENDOR), realtek)
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
#CONFIG_DRIVER_WEXT :=y
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_rtl
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_rtl
BOARD_WLAN_DEVICE := rtl8192cu
#BOARD_WLAN_DEVICE := rtl8192du
#BOARD_WLAN_DEVICE := rtl8192ce
#BOARD_WLAN_DEVICE := rtl8192de
#BOARD_WLAN_DEVICE := rtl8723as
#BOARD_WLAN_DEVICE := rtl8723au
#BOARD_WLAN_DEVICE := rtl8189es
#BOARD_WLAN_DEVICE := rtl8723bs
#BOARD_WLAN_DEVICE := rtl8723bu
WIFI_MULTI_MODULE := true
ifeq ($(WIFI_MULTI_MODULE), true)
# In WIFI_MULTI_MODULE, this is the order to insert wifi module. If there are
# multiple wifi devices on your platform. Android WiFi will be the one that
# its module is inserted first.
WIFI_DRIVER_MODULE_NAME         := "8814au 8812au 8192eu 8192cu 8188eu 8811au 8189es 8723bs"
WIFI_DRIVER_MODULE_PATH         := "/vendor/modules"
else
#WIFI_DRIVER_MODULE_NAME		:= "wlan"
#WIFI_DRIVER_MODULE_PATH 	:= "/vendor/modules/wlan.ko"
WIFI_DRIVER_MODULE_NAME		:= "8811au"
WIFI_DRIVER_MODULE_PATH 	:= "/vendor/modules/8811au.ko"
endif
WIFI_DRIVER_MODULE_ARG 		:= "ifname=wlan0 if2name=p2p0"

BOARD_SEPOLICY_DIRS += device/realtek/kylin/sepolicy

WIFI_FIRMWARE_LOADER := "rtw_fwloader"
WIFI_DRIVER_FW_PATH_STA := "STA"
WIFI_DRIVER_FW_PATH_AP := "AP"
WIFI_DRIVER_FW_PATH_P2P := "P2P"
WIFI_DRIVER_FW_PATH_PARAM := "/dev/null"

endif

# For Widevine library level
BOARD_WIDEVINE_OEMCRYPTO_LEVEL:= 1

#FORCE_ARM_DEBUGGING := true

# pre-optimization options
#WITH_DEXPREOPT := true
#DONT_DEXPREOPT_PREBUILTS := true
#WITH_DEXPREOPT_BOOT_IMG_ONLY := true
#WITH_DEXPREOPT_PIC := true
#WITH_ART_SMALL_MODE := true

