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

TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := cortex-a53

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic

TARGET_CPU_SMP := true
#TARGET_PREFER_32_BIT := true
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := kylin

BOARD_FLASH_BLOCK_SIZE := 4096
TARGET_USERIMAGES_USE_EXT4 := true
#BOARD_SYSTEMIMAGE_FILE_SYSTEM_TYPE := squashfs
#BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1073741824
BOARD_SYSTEMIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 704365056

#default is 8gb_emmc_partition
#BOARD_SYSTEMIMAGE_FILE_SYSTEM_TYPE := ext4
#BOARD_SYSTEMIMAGE_PARTITION_SIZE := 789577728

#BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := squashfs
#BOARD_VENDORIMAGE_PARTITION_SIZE := 134217728
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_VENDORIMAGE_PARTITION_SIZE := 133169152

TARGET_COPY_OUT_VENDOR := vendor

BOARD_CACHEIMAGE_PARTITION_SIZE := 419430400
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4

BOARD_XENIMAGE_PARTITION_SIZE := 66108864
BOARD_XENIMAGE_FILE_SYSTEM_TYPE := ext4

#System
TARGET_PRELINK_MODULE := true
TARGET_NO_BOOTLOADER := true
TARGET_NO_RECOVERY := true
TARGET_NO_KERNEL := true
TARGET_NO_RADIOIMAGE := true
USE_OPENGL_RENDERER := true
BOARD_USES_GENERIC_AUDIO := true
TARGET_PROVIDES_INIT_RC := true
OVERRIDE_RS_DRIVER := libRSDriverArm.so
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

# ---------------------------------------------------> Time
#
# |----------------------------|----------------------
# |<- A ->|----------------------------|--------------
#         |<- B ->|----------------------------|------
# ^       ^       ^
# |       |       |_ VSync for SurfaceFlinger
# |       |_ VSync for App UI
# |_ Hardware VSync
#
# A: VSYNC_EVENT_PHASE_OFFSET_NS
# B: SF_VSYNC_EVENT_PHASE_OFFSET_NS
VSYNC_EVENT_PHASE_OFFSET_NS := 0
SF_VSYNC_EVENT_PHASE_OFFSET_NS := 5000000

# SELinux
#BOARD_SEPOLICY_DIRS += device/realtek/kylin/common/sepolicy
