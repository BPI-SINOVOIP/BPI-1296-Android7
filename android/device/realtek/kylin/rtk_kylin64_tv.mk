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

## currently, kyline_tv only uses overlay settings under overlay_common, the order of inherit is important,
## we inherit device.mk first, so overlay_common is applied first, then we inherit atv_generic


## bad coding style
DEVICE_PACKAGE_OVERLAYS := \
    device/realtek/kylin/overlay_common \
    device/realtek/kylin/overlay_atv

$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, device/realtek/kylin/device.mk)
$(call inherit-product, device/google/atv/products/atv_generic.mk)

#rtkbt
$(call inherit-product, device/realtek/kylin/rtkbt/rtkbt.mk)

PRODUCT_CHARACTERISTICS := tv

PRODUCT_DEVICE := kylin64_tv
PRODUCT_NAME   := rtk_kylin64_tv
PRODUCT_MODEL  := Realtek Kylin 64-bit TV
PRODUCT_BRAND  := realtek
PRODUCT_MANUFACTURER := realtek

PRODUCT_PACKAGES += RealtekTvSettings
#PRODUCT_PACKAGES += RealtekTvQuickSettings
PRODUCT_PACKAGES += RealtekAppDrawer

PRODUCT_PACKAGES += UseHDMITvInputService
PRODUCT_PACKAGES += SampleTvInput

### Work-around to make home key works, refer to ATM-327 for detailed information
PRODUCT_PACKAGES += Provision

# DHCKYLIN-38, Need DocumentsUI to handle android.provider.action.BROWSE_DOCUMENT_ROOT
PRODUCT_PACKAGES += DocumentsUI

PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=320

PRODUCT_COPY_FILES += device/realtek/kylin/audio_policy.conf:system/etc/audio_policy.conf

