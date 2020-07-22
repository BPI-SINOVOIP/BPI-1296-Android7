### defined in rtk_kylin32.mk and rtk_kylin64.mk
# PRODUCT_AAPT_CONFIG := normal large tvdpi mdpi hdpi
# PRODUCT_AAPT_PREF_CONFIG := hdpi
###

### defined in rtk_kylin32.mk and rtk_kylin32_tv.mk
# PRODUCT_CHARACTERISTICS := tablet
###

## A all realtek devices share overlay_common
DEVICE_PACKAGE_OVERLAYS := device/realtek/kylin/overlay_common

#PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/kylin/prebuilt/system,system)
#PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/kylin/prebuilt/root,root)

PRODUCT_COPY_FILES += system/core/rootdir/init.rc:root/init.rc
ifneq ($(ENABLE_VMX_BOOT_FLOW), YES)
PRODUCT_COPY_FILES += device/realtek/kylin/init.kylin.rc:root/init.kylin.rc
endif
PRODUCT_COPY_FILES += device/realtek/kylin/init.kylin.usb.rc:root/init.kylin.usb.rc
PRODUCT_COPY_FILES += device/realtek/kylin/ueventd.kylin.rc:root/ueventd.kylin.rc
#PRODUCT_COPY_FILES += device/realtek/kylin/audio_policy.conf:system/etc/audio_policy.conf
PRODUCT_COPY_FILES += device/realtek/kylin/fstab.kylin.emmc:root/fstab.kylin.emmc
PRODUCT_COPY_FILES += device/realtek/kylin/fstab.kylin.sata:root/fstab.kylin.sata
PRODUCT_COPY_FILES += device/realtek/kylin/fstab.kylin.xen:root/fstab.kylin.xen
PRODUCT_COPY_FILES += device/realtek/kylin/sys.part.rc.emmc:root/sys.part.rc.emmc
PRODUCT_COPY_FILES += device/realtek/kylin/sys.part.rc.sata:root/sys.part.rc.sata
PRODUCT_COPY_FILES += device/realtek/kylin/sys.part.rc.xen:root/sys.part.rc.xen
PRODUCT_COPY_FILES += device/realtek/kylin/init.fschk.rc:root/init.fschk.rc

PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.pppoe.xml:system/etc/permissions/android.hardware.pppoe.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.opengles.aep.xml:system/etc/permissions/android.hardware.opengles.aep.xml
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml
PRODUCT_TAGS += dalvik.gc.type-precise

# setup dalvik vm configs.
PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapstartsize=8m
PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heaptargetutilization=0.75
PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapminfree=2m
PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapmaxfree=8m

# setup dex2oat compiler options: everything|speed|balanced|space|interpret-only|verify-none
#PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.dex2oat-filter=speed
#PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.image-dex2oat-filter=speed

# pre-optimization options
#PRODUCT_DEX_PREOPT_BOOT_FLAGS := --compiler-filter=speed
#PRODUCT_DEX_PREOPT_DEFAULT_FLAGS := --compiler-filter=speed
#$(call add-product-dex-preopt-module-config,services,--compiler-filter=space)

PRODUCT_PROPERTY_OVERRIDES += persist.sys.timezone=Asia/Shanghai
PRODUCT_PROPERTY_OVERRIDES += wifi.interface=wlan0
PRODUCT_PROPERTY_OVERRIDES += wifi.supplicant_scan_interval=120
PRODUCT_PROPERTY_OVERRIDES += ro.opengles.version=196610
PRODUCT_PROPERTY_OVERRIDES += ro.zygote.disable_gl_preload=true
PRODUCT_PROPERTY_OVERRIDES += ro.kernel.android.checkjni=false
PRODUCT_PROPERTY_OVERRIDES += keyguard.no_require_sim=true
PRODUCT_PROPERTY_OVERRIDES += persist.sys.strictmode.visual=0
PRODUCT_PROPERTY_OVERRIDES += persist.sys.strictmode.disable=1
PRODUCT_PROPERTY_OVERRIDES += hwui.render_dirty_regions=false
PRODUCT_PROPERTY_OVERRIDES += net.dns1 = 168.95.192.1
PRODUCT_PROPERTY_OVERRIDES += net.dns2 = 8.8.8.8
PRODUCT_PROPERTY_OVERRIDES += persist.pppoe.coldboot.conn=true

NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
##USE_LAYOUT_UI_1080 := true
#ifeq ($(IMAGE_DRAM_SIZE), 1GB)
#USE_LAYOUT_MEM_LOW := true
#endif

USE_AFBC := true

# Set this to false will cause compilation error
USE_RT_MEDIA_PLAYER := true

ENABLE_LIGHTTPD := false
ENABLE_RTK_HDMI_SERVICE := true
USE_SETTINGS_SERVICE := true
ENABLE_MEDIA_QUERY := true
ENABLE_RTK_HDMIRX_SERVICE := true
ENABLE_RTK_DPTX_SERVICE := true
USE_RTK_GPIO := false
# For DRM code flow
ENABLE_DRM_PLUGIN_SERVICE := true
# For TEE OS, video used physical Address
ifeq ($(ENABLE_TEE_DRM_FLOW),)
	ENABLE_TEE_DRM_FLOW := false
endif

USE_RTK_HDMI_CONTROL := NO
USE_RTK_MHL_CONTROL := NO
ifeq ($(USE_RTK_HDCP1x_CONTROL),)
	# When not defined, the default is NO
	# ex : export USE_RTK_HDCP1x_CONTROL=YES && build android
	USE_RTK_HDCP1x_CONTROL := NO
endif
ifeq ($(USE_RTK_HDCP22_CONTROL),)
	# When not defined, the default is NO
	# ex : export USE_RTK_HDCP22_CONTROL=YES && build android
	USE_RTK_HDCP22_CONTROL := NO
endif
USE_RTK_CEC_CONTROL := NO
USE_RTK_VOUT_UTIL := YES
USE_RTK_AOUT_UTIL := YES
USE_RTK_SETUP_CLASS := NO

USE_RT_ION := true
USE_RTK_EXTRACTOR := false
ifeq ($(USE_VMX_RTK_EXTRACTOR_CONTROL),true)
        USE_RTK_EXTRACTOR := true
endif

USE_RTK_MEDIA_PROVIDER := YES
USE_DTCPIP_SIK := NO
USE_TV_MANAGER := YES
USE_TOGO_MANAGER := NO
USE_AIRFUN_MANAGER := NO

USE_RTK_UART_BT := YES
USE_JPU := NO
USE_RTK_PPPOE := YES
USE_RTK_OPENWRT := YES
ENABLE_RT_WIFIDISPLAY := true
# for DTV
ENABLE_1195_DTV_FLOW := YES

#ENABLE_FFMPEG := true

# used for recovery mode - apply for factory update
# example: RECOVERY_FACTORY_CMD0 := rm factory_test.txt
RECOVERY_FACTORY_CMD0 := cd ../../;cp -rf tmp/install_factory/* tmp/factory/;
RECOVERY_FACTORY_CMD1 :=

RECOVERY_SECUREBOOT := 0
LAYOUT := emmc
DTB_ENC := false

# Used for NAS OTA package
NAS_ENABLE := NO

# Used for XEN OTA package
XEN_ENABLE := NO

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

ifeq ($(ENABLE_RTK_DPTX_SERVICE), true)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enabledptxservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enabledptxservice=false
endif

ifeq ($(ENABLE_RTK_HDMI_SERVICE), true)
USE_RTK_HDMI_CONTROL := YES
# Enable the following USE_RTK_MHL_CONTROL := YES when MHL is enabled.
USE_RTK_MHL_CONTROL := YES
# Enable the following USE_RTK_HDCP1x_CONTROL := YES when HDCP is enabled.

#PRODUCT_PACKAGES += libteec
#USE_RTK_HDCP1x_CONTROL := YES
#USE_RTK_HDCP22_CONTROL := YES
#USE_RTK_TEE := YES
USE_RTK_VOUT_UTIL := YES
USE_RTK_SETUP_CLASS := YES
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmiservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdmiservice=false
endif

ifeq ($(USE_RTK_MHL_CONTROL), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemhlserivce=false #FIXME
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablemhlserivce=false
endif

ifeq ($(USE_RTK_HDCP1x_CONTROL), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdcp=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablehdcp=false
endif

PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.hdmi.cec.xml:system/etc/permissions/android.hardware.hdmi.cec.xml

ifeq ($(USE_RT_MEDIA_PLAYER), true)
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RTMediaPlayer=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RtkExtractor=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTSP_USE_RTPLAYER=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.MMS_USE_RTPLAYER=false
else
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RTMediaPlayer=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.USE_RtkExtractor=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTSP_USE_RTPLAYER=false
PRODUCT_PROPERTY_OVERRIDES += persist.media.MMS_USE_RTPLAYER=false
endif
PRODUCT_PROPERTY_OVERRIDES += persist.media.DisableInband=false

ifeq ($(USE_RTK_MEDIA_PROVIDER), YES)
PRODUCT_PACKAGES += RtkMediaProvider
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTKMediaProvider=true
else
PRODUCT_PACKAGES += MediaProvider
PRODUCT_PROPERTY_OVERRIDES += persist.media.RTKMediaProvider=false
endif
ifeq ($(ENABLE_DRM_PLUGIN_SERVICE), true)
PRODUCT_PROPERTY_OVERRIDES += drm.service.enabled=true
PRODUCT_PACKAGES += libOMX.realtek.video.dec.avc.secure
else
PRODUCT_PROPERTY_OVERRIDES += drm.service.enabled=false
endif

ifeq ($(USE_AIRFUN_MANAGER), YES)
PRODUCT_PROPERTY_OVERRIDES += ro.config.with_airfun=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.with_airfun=false
endif

PRODUCT_PROPERTY_OVERRIDES += persist.sys.RunRTKIniSettings=false
PRODUCT_PROPERTY_OVERRIDES += ro.config.eventdelayms=0
PRODUCT_PROPERTY_OVERRIDES += ro.arch = arm
PRODUCT_PROPERTY_OVERRIDES += persist.storage.resizefs=0
PRODUCT_PROPERTY_OVERRIDES += rtk.screen.forceOrientation=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.mb.dlna.disable=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.mb.samba.disable=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.abstract_socket=true
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.device_type=4
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.cec_vendor_id=32640
PRODUCT_PROPERTY_OVERRIDES += ro.hdmi.cec_osd_name=DMP
PRODUCT_PROPERTY_OVERRIDES += persist.sys.hdmi.addr.playback=4
PRODUCT_PROPERTY_OVERRIDES += persist.sys.hdmi.resolution=Max
PRODUCT_PROPERTY_OVERRIDES += persist.sys.hdmi.keep_awake=false
PRODUCT_PROPERTY_OVERRIDES += rtk.omx.hevc_performance_mode=0
PRODUCT_PROPERTY_OVERRIDES += persist.camera.no_deinterlace=0

#For 4k playback app, ex: netflix...
PRODUCT_PROPERTY_OVERRIDES += sys.display-size=3840x2160

user_variant := $(filter user userdebug,$(TARGET_BUILD_VARIANT))
ifneq (,$(user_variant))
# ro.adb.secure is required for passing CTS
PRODUCT_PROPERTY_OVERRIDES += ro.adb.secure=1
else
# for eng build, no auth is required
PRODUCT_PROPERTY_OVERRIDES += ro.adb.secure=0
endif

# wifi switching profile:
#   native: native Android behavior, STA/P2P & SoftAP switches are independent
#   toggle: WiFi is toggled between STA/P2P & SoftAP mode
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.wifi.switchProfile=native
# wifi initial mode: sta / softap
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.wifi.initial_mode=sta
# wifi mac as default device name : true or false
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.wifi.mac.as.devicename=false
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.wifi.fisrt.boot=true
PRODUCT_PROPERTY_OVERRIDES += ro.boot.wificountrycode=US

PRODUCT_PROPERTY_OVERRIDES += persist.rtk.performancemode=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.ctstest=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.video.maxAcqCnt=3
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.cameraMode=1
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.deepcolor=off
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.hdmirx.raw=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.hdmirx.version=1.4
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.vold.trymount=1
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.vold.checkvolume=0
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.vold.formatonerror=0
#RTK USB Type C contorller
PRODUCT_PROPERTY_OVERRIDES += sys.usb.controller="98020000.dwc3_drd"

# USE S3 Suspend as default
PRODUCT_PROPERTY_OVERRIDES += persist.rtk.suspend.mode=suspend

# GPU AFBC
ifeq ($(USE_AFBC), true)
PRODUCT_PROPERTY_OVERRIDES += persist.gralloc.afbc.fb=1
PRODUCT_PROPERTY_OVERRIDES += persist.gralloc.afbc.texture=1
else
PRODUCT_PROPERTY_OVERRIDES += persist.gralloc.afbc.fb=0
PRODUCT_PROPERTY_OVERRIDES += persist.gralloc.afbc.texture=0
endif
###################################################
# built-in libraries
###################################################
PRODUCT_PACKAGES += audio.usb.default
PRODUCT_PACKAGES += audio.a2dp.default
PRODUCT_PACKAGES += audio.r_submix.default
PRODUCT_PACKAGES += audio.primary.kylin
PRODUCT_PACKAGES += camera.kylin
PRODUCT_PACKAGES += camera.arm
PRODUCT_PACKAGES += hdmi.kylin
PRODUCT_PACKAGES += dptx.kylin
PRODUCT_PACKAGES += mhl.kylin
#PRODUCT_PACKAGES += rtk_cec.kylin
PRODUCT_PACKAGES += hdmi_cec.kylin
PRODUCT_PACKAGES += tv_input.kylin
PRODUCT_PACKAGES += hdcp.kylin
PRODUCT_PACKAGES += libhwse.kylin
PRODUCT_PACKAGES += hwcomposer.kylin
PRODUCT_PACKAGES += power.kylin

PRODUCT_PACKAGES += libsrec_jni
PRODUCT_PACKAGES += libaudioroute
PRODUCT_PACKAGES += libstagefrighthw
PRODUCT_PACKAGES += libRTK_lib
#PRODUCT_PACKAGES += libRTK_OSAL_memory
#PRODUCT_PACKAGES += libRTKOMX_OSAL
PRODUCT_PACKAGES += libRTKOMX_OSAL_RTK
PRODUCT_PACKAGES += libOMX_Core
PRODUCT_PACKAGES += libjpu
PRODUCT_PACKAGES += libvpu
PRODUCT_PACKAGES += libve3
PRODUCT_PACKAGES += libvp9
PRODUCT_PACKAGES += libOMX.realtek.video.dec
PRODUCT_PACKAGES += libOMX.realtek.video.enc
PRODUCT_PACKAGES += libOMX.realtek.video.dec.vp9
PRODUCT_PACKAGES += libOMX.realtek.image.dec
PRODUCT_PACKAGES += libOMX.realtek.image.enc
PRODUCT_PACKAGES += libOMX.realtek.audio.dec
PRODUCT_PACKAGES += libRTKHDMIControl
PRODUCT_PACKAGES += libRTKVoutUtil
PRODUCT_PACKAGES += libRTKAoutUtil
PRODUCT_PACKAGES += libRTKSetupClass
PRODUCT_PACKAGES += libRT_IPC
PRODUCT_PACKAGES += libRtkTranscodePlayer
PRODUCT_PACKAGES += libRTMediaPlayer
PRODUCT_PACKAGES += libRTDtvMediaPlayer
PRODUCT_PACKAGES += lib_driver_cmd_rtl
PRODUCT_PACKAGES += librvsd
PRODUCT_PACKAGES += libsettingsclient
PRODUCT_PACKAGES += libRtkRpcClientServer
PRODUCT_PACKAGES += librealtek_jni
PRODUCT_PACKAGES += librealtek_runtime
PRODUCT_PACKAGES += libaudioParser
PRODUCT_PACKAGES += libRtkAudioRedirect
PRODUCT_PACKAGES += libiconv
PRODUCT_PACKAGES += librtksmb
PRODUCT_PACKAGES += libDLNADMPClass
PRODUCT_PACKAGES += libDLNADMRClass
PRODUCT_PACKAGES += libMCPControl
PRODUCT_PACKAGES += libRTKASFExtractor
PRODUCT_PACKAGES += libRTKAIFFExtractor
PRODUCT_PACKAGES += libRTKAllocator
PRODUCT_PACKAGES += libRTKFLVExtractor
PRODUCT_PACKAGES += libRTKDSDExtractor
PRODUCT_PACKAGES += libRTKAPEExtractor
PRODUCT_PACKAGES += libRTKHEIFExtractor
PRODUCT_PACKAGES += libRtkHDMICallbackNative
PRODUCT_PACKAGES += libRtkHWMBinder
PRODUCT_PACKAGES += libRtkHWMBinderservice
PRODUCT_PACKAGES += libRtkRtsp
PRODUCT_PACKAGES += libRtkScreenRecord
PRODUCT_PACKAGES += libRTKVOWriteback
PRODUCT_PACKAGES += libTEEapi
PRODUCT_PACKAGES += libteec
PRODUCT_PACKAGES += librtk_ion

ifeq ($(USE_RTK_EXTRACTOR), true)
PRODUCT_PACKAGES += libRTKExtractor
endif

ifeq ($(ENABLE_TEE_DRM_FLOW),true)
PRODUCT_PACKAGES += libteec
PRODUCT_PACKAGES += libTEEapi
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.drm.addrMode=3
else
PRODUCT_PROPERTY_OVERRIDES += ro.rtk.drm.addrMode=3
endif

ifeq ($(USE_RTK_GPIO), true)
PRODUCT_PACKAGES += rtk_gpio.kylin
PRODUCT_PACKAGES += libRTKGPIO
PRODUCT_PACKAGES += gpioproxy
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablegpioservice=true
else
PRODUCT_PROPERTY_OVERRIDES += ro.config.enablegpioservice=false
endif

PRODUCT_PACKAGES += mpclient
PRODUCT_PACKAGES += mpclient2
PRODUCT_PACKAGES += mpDTVclient
PRODUCT_PACKAGES += TestRtkHDMICallbackNative
PRODUCT_PACKAGES += libwpa_client
PRODUCT_PACKAGES += wpa_cli
PRODUCT_PACKAGES += wpa_supplicant
PRODUCT_PACKAGES += wpa_supplicant.conf
PRODUCT_PACKAGES += rtwpriv
PRODUCT_PACKAGES += rtwpriv_org

PRODUCT_PACKAGES += libsideband
PRODUCT_PACKAGES += sideband

PRODUCT_PACKAGES += libRTKHWMBinderapi
PRODUCT_PACKAGES += RtkHWMBinder
#PRODUCT_PACKAGES += audio_firmware
PRODUCT_PACKAGES += libfw_socket_client

PRODUCT_PACKAGES += libwfdisplay
PRODUCT_PACKAGES += rtk_wfd

ifeq ($(ENABLE_VMX_CA_CONTROL), YES)
PRODUCT_PACKAGES += librtksdk
PRODUCT_PACKAGES += librtksdk_jni
PRODUCT_PACKAGES += librtkALP
PRODUCT_PACKAGES += libvmxrelease
PRODUCT_PACKAGES += libvmxca
PRODUCT_PACKAGES += libvmxcatest
PRODUCT_PACKAGES += libvmxca_jni
PRODUCT_PACKAGES += librtksocket
endif
###################################################
# built-in binaries
###################################################
ifeq ($(ENABLE_VMX_CA_CONTROL), YES)
PRODUCT_PACKAGES += RTK_MIDDLEWARE_TEST_ANDROID
PRODUCT_PACKAGES += RTK_VMX_TEST
PRODUCT_PACKAGES += RTK_VMX_TEST_CNT
endif
PRODUCT_PACKAGES += coda980_dec_test
PRODUCT_PACKAGES += coda980_enc_test
PRODUCT_PACKAGES += w4_dec_test
PRODUCT_PACKAGES += vp9dec
PRODUCT_PACKAGES += vpuinit
PRODUCT_PACKAGES += jpurun

PRODUCT_PACKAGES += factory
PRODUCT_PACKAGES += multilan
PRODUCT_PACKAGES += settingsproxy

ifeq ($(TARGET_BUILD_VARIANT), eng)
PRODUCT_PACKAGES += gatord
endif

## add for Android L
#ifeq ($(BOARD_WIFI_VENDOR), realtek)
PRODUCT_PACKAGES += rtw_fwloader
PRODUCT_PACKAGES += hostapd
PRODUCT_PACKAGES += hostapd_cli
#endif

ifeq ($(USE_RTK_PPPOE), YES)
PRODUCT_PACKAGES += pppoe
PRODUCT_PACKAGES += com.android.server.pppoe
endif

ifeq ($(USE_RTK_OPENWRT), YES)
PRODUCT_PACKAGES += com.rtk.net.ubusrpc
endif

###################################################
# built-in host binaries
###################################################
PRODUCT_PACKAGES += mkbootimg

###################################################
# built-in files
###################################################
PRODUCT_PACKAGES += dhcpcd.conf

###################################################
# built-in applications
###################################################
PRODUCT_PACKAGES += TvProvider
PRODUCT_PACKAGES += RTKSourceIn
PRODUCT_PACKAGES += DMR
PRODUCT_PACKAGES += RealtekGallery2
#PRODUCT_PACKAGES += RealtekInitialSettings
PRODUCT_PACKAGES += RealtekSoftwareUpdater
PRODUCT_PACKAGES += Music
PRODUCT_PACKAGES += MediaBrowser
PRODUCT_PACKAGES += Browser2
#PRODUCT_PACKAGES += RealtekQuickSearchBox
#PRODUCT_PACKAGES += RTKControlPanel
#PRODUCT_PACKAGES += ApplicationsProvider
ifeq ($(USE_TOGO_MANAGER), YES)
PRODUCT_PACKAGES += ToGo
PRODUCT_PROPERTY_OVERRIDES += ro.with_togoproxy=false
endif
PRODUCT_PACKAGES += RTKMiracastSink
#PRODUCT_PACKAGES += RealtekSystemUI
PRODUCT_PACKAGES += RealtekSuspendHandler
#PRODUCT_PACKAGES += RealtekPIP
PRODUCT_PACKAGES += RealtekMediaRecoderDemo
PRODUCT_PACKAGES += RealtekFloatingWindowDemo
PRODUCT_PACKAGES += RealtekEncoder
PRODUCT_PACKAGES += Contacts
PRODUCT_PACKAGES += DeskClock
PRODUCT_PACKAGES += DownloadProvider
PRODUCT_PACKAGES += DownloadProviderUi

PRODUCT_PACKAGES += RealtekHDMIRxTvInput
PRODUCT_PACKAGES += RealtekTVDemo

ifeq ($(USE_VMX_DEMO_APK), true)
PRODUCT_PACKAGES += VmxRtkDemo
endif

PRODUCT_LOCALES := en_US zh_TW zh_CN

# add verity dependencies
$(call inherit-product, build/target/product/verity.mk)
PRODUCT_SUPPORTS_BOOT_SIGNER := false
PRODUCT_SYSTEM_VERITY_PARTITION := /dev/block/mmcblk0p1

PRODUCT_PACKAGES += \
    slideshow \
    verity_warning_images

# seamless update

PRODUCT_PACKAGES += \
    bootctrl.kylin
PRODUCT_PACKAGES_DEBUG += \
    bootctl

AB_OTA_UPDATER := false
AB_OTA_PARTITIONS := \
  system
PRODUCT_PACKAGES += \
  update_engine \
  update_verifier
PRODUCT_PACKAGES_DEBUG += update_engine_client

AB_OTA_POSTINSTALL_CONFIG += \
  RUN_POSTINSTALL_system=true \
  POSTINSTALL_PATH_system=usr/bin/postinst \
  FILESYSTEM_TYPE_system=ext4

PRODUCT_PACKAGES += otapreopt_script

AB_OTA_POSTINSTALL_CONFIG += \
    RUN_POSTINSTALL_system=true \
    POSTINSTALL_PATH_system=system/bin/otapreopt_script \
    FILESYSTEM_TYPE_system=ext4 \
    POSTINSTALL_OPTIONAL_system=true


PRODUCT_COPY_FILES += device/realtek/kylin/venus_IR_input.kl:system/usr/keylayout/venus_IR_input.kl
PRODUCT_COPY_FILES += device/realtek/kylin/venus_IR_input.kcm:system/usr/keychars/venus_IR_input.kcm

PRODUCT_COPY_FILES += device/realtek/kylin/gpio_cmd.kl:system/usr/keylayout/gpio_cmd.kl

#Wifi Module Mapper
#PRODUCT_COPY_FILES += hardware/realtek/wlan/wifi_auto_insmod/wifi_module_list.cfg:system/etc/wifi/wifi_module_list.cfg
PRODUCT_COPY_FILES += hardware/realtek/wlan/wifi_auto_insmod/wifi_module_mapper.json:system/etc/wifi/wifi_module_mapper.json

ifeq ($(ENABLE_LIGHTTPD), true)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*.html,device/realtek/www/rtCGI/html,system/data/www/htdocs)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/www/rtCGI/images,system/data/www/htdocs/images)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/www/rtCGI/css,system/data/www/htdocs/css)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/www/rtCGI/javascript,system/data/www/htdocs/javascript)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/www/rtCGI/jslib,system/data/www/htdocs/jslib)
endif

#ifeq ($(USE_LAYOUT_MEM_LOW), true)
#  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapsize=192m
#  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapgrowthlimit=128m
#
#  PRODUCT_PROPERTY_OVERRIDES += ro.config.low_ram=true
#  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.jit.codecachesize=0
#else
#  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapsize=384m
#  PRODUCT_PROPERTY_OVERRIDES += dalvik.vm.heapgrowthlimit=128m
#endif

#gms
$(call inherit-product-if-exists, vendor/google/products/gms.mk)
PRODUCT_PROPERTY_OVERRIDES += ro.com.google.clientidbase=android-acme
PRODUCT_PROPERTY_OVERRIDES += ro.product.first_api_level=25

ifeq ($(ENABLE_VMX_BOOT_FLOW), YES)
PRODUCT_PROPERTY_OVERRIDES += rtk.vmx.boot=1
PRODUCT_COPY_FILES += device/realtek/kylin/fstab.kylin.vmx:root/fstab.kylin.vmx
PRODUCT_COPY_FILES += device/realtek/kylin/sys.part.rc.vmx:root/sys.part.rc.vmx
PRODUCT_COPY_FILES += device/realtek/kylin/init.kylin_vmx.rc:root/init.kylin.rc
endif

# Include deviceCommon.mk at end of file for purpose
# If DvdPlayer exists in both common & kylin directoy,
# only DvdPlayer in kylin will be copied to /system/bin
include device/realtek/kylin/common/deviceCommon.mk

