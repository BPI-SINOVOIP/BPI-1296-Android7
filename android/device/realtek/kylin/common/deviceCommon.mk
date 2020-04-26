PRODUCT_COPY_FILES += device/realtek/kylin/common/init.e2fsck.rc:root/init.e2fsck.rc
PRODUCT_COPY_FILES += device/realtek/kylin/common/init.nand.rc:root/init.nand.rc
PRODUCT_COPY_FILES += device/realtek/kylin/common/init.emmc.rc:root/init.emmc.rc
PRODUCT_COPY_FILES += device/realtek/kylin/common/init.sata.rc:root/init.sata.rc
PRODUCT_COPY_FILES += device/realtek/kylin/common/init.xen.rc:root/init.xen.rc
PRODUCT_COPY_FILES += device/realtek/kylin/common/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf
PRODUCT_COPY_FILES += device/realtek/kylin/common/media_profiles.xml:system/etc/media_profiles.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/media_codecs.xml:system/etc/media_codecs.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/media_codecs_rtk_video.xml:system/etc/media_codecs_rtk_video.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/media_codecs_rtk_audio.xml:system/etc/media_codecs_rtk_audio.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/media_codecs_performance.xml:system/etc/media_codecs_performance.xml
PRODUCT_COPY_FILES += frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml
PRODUCT_COPY_FILES += frameworks/av/media/libstagefright/data/media_codecs_google_audio_first.xml:system/etc/media_codecs_google_audio_first.xml
PRODUCT_COPY_FILES += frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/mixer_paths.xml:system/etc/mixer_paths.xml
PRODUCT_COPY_FILES += device/realtek/kylin/common/content-types.properties:system/lib/content-types.properties

# prebuilt binaries / modules / libraries
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/kylin/common/prebuilt/system,system)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/kylin/common/prebuilt/root/sbin,root/sbin)
PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/realtek/kylin/common/prebuilt/vendor,vendor)

# prebuilt mali library
PRODUCT_PACKAGES += libGLES_mali
PRODUCT_PACKAGES += gralloc.kylin

ifeq ($(ENABLE_VMX_BOOT_FLOW), YES)
PRODUCT_COPY_FILES += device/realtek/kylin/common/init.vmx.rc:root/init.vmx.rc
endif
