# this file is auto generated. 
# RELEASE_TIME: 20171011 17:50:00
# RTKBT_API_VERSION=3.1.1.0

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_RTK := true
BOARD_HAVE_BLUETOOTH_RTK_COEX := true
BOARD_HAVE_BLUETOOTH_RTK_ADDON := bee2 vr_bee_hidraw_daemon rtkbtAutoPairService rtkbtAutoPairUIDemo BluetoothRTK3ddService
#BOARD_HAVE_BLUETOOTH_RTK_ADDON := basic
ifneq ($(BOARD_HAVE_BLUETOOTH_RTK_ADDON),)
$(foreach item,$(BOARD_HAVE_BLUETOOTH_RTK_ADDON),$(call inherit-product,$(LOCAL_PATH)/addon/$(item)/addon.mk))
#Firmware For Tv
include $(LOCAL_PATH)/Firmware/TV/TV_Firmware.mk
else
#Firmware For Tablet
include $(LOCAL_PATH)/Firmware/BT/BT_Firmware.mk
endif

PRODUCT_COPY_FILES += \
       $(LOCAL_PATH)/system/etc/bluetooth/rtkbt.conf:system/etc/bluetooth/rtkbt.conf \
       $(LOCAL_PATH)/system/etc/permissions/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
       $(LOCAL_PATH)/system/etc/permissions/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
       $(LOCAL_PATH)/system/usr/keylayout/rtkbt_virtual_hid.kl:system/usr/keylayout/rtkbt_virtual_hid.kl \

# base bluetooth
PRODUCT_PACKAGES += \
    Bluetooth \
    audio.a2dp.default

#plugins
PRODUCT_PACKAGES += \
    test \
    heartbeat \
    autopair \
    autopair_huawei \
    autopair_stack \
    3dd_service \
    vr_bee_hidraw \
    vhid \
    fwlog \
    audio.vr_bee_hidraw.default \

