#
# Copyright 2013 The Android Open-Source Project
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

# Bruce some bad workaround here, define overlay at the same place.
DEVICE_PACKAGE_OVERLAYS := device/realtek/kylin/overlay_common \
                        device/realtek/kylin/overlay

$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, device/realtek/kylin/device.mk)

PRODUCT_AAPT_CONFIG := normal large tvdpi mdpi hdpi
PRODUCT_AAPT_PREF_CONFIG := hdpi

$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_base.mk)
#rtkbt
$(call inherit-product, device/realtek/kylin/rtkbt/rtkbt.mk)

PRODUCT_CHARACTERISTICS := tablet

PRODUCT_DEVICE := kylin64
PRODUCT_NAME   := rtk_kylin64
PRODUCT_MODEL  := Realtek Kylin 64-bit OTT
PRODUCT_BRAND  := realtek
PRODUCT_MANUFACTURER := realtek

PRODUCT_COPY_FILES += frameworks/native/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml
PRODUCT_COPY_FILES += device/realtek/kylin/overlay/system/etc/permissions/tablet_extend_hardware.xml:system/etc/permissions/tablet_extend_hardware.xml
PRODUCT_COPY_FILES += device/realtek/kylin/audio_policy_tablet.conf:system/etc/audio_policy.conf

PRODUCT_PACKAGES += Calendar
PRODUCT_PACKAGES += RealtekSettings

PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=240

