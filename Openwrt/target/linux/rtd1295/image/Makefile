#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/host.mk
include $(CURDIR)/Makefile.in
-include $(CURDIR)/../$(SUBTARGET)/Makefile.in
include $(CURDIR)/audiofw.mk

BANANAPI:=$(if $(findstring bananapi,$(PROFILE)),true,false)
SAOLA:=$(if $(findstring saola,$(PROFILE)),true,false)

ifeq ($(BANANAPI), true)
    $(warning "bananapi rtd-1296 chip selected")
    RTK_CHIP:=rtd-1296
else ifeq ($(SAOLA), true)
    $(warning "saola rtd-1296 chip selected")
    RTK_CHIP:=rtd-1296
else
    $(warning "giraffe rtd-1295 chip selected") 
    RTK_CHIP:=rtd-1295
endif

IMAGE_PATH:=$(LINUX_DIR)/arch/$(LINUX_KARCH)/boot/Image
IMAGE_FILE:=$(if $(wildcard $(IMAGE_PATH)), $(IMAGE_PATH), $(KERNEL_BUILD_DIR)/Image)
DTB_FILE:=$(DTS_DIR)/realtek/rtd-129x-openwrt.dtb
DTS_FILE:=$(patsubst %.dtb,%.dts,$(DTB_FILE))
RESCUE_DTB_FILE:=$(DTS_DIR)/realtek/rtd-129x-qa-rescue.dtb
RESCUE_DTS_FILE:=$(patsubst %.dtb,%.dts,$(RESCUE_DTB_FILE))
CMA_DTS_FILE:=$(RTK_CHIP)-$(basename $(subst GB-,GB.,$(PROFILE))).CMAx2.dts
CMA_DTS:=$(if $(wildcard $(DTS_DIR)/realtek/$(CMA_DTS_FILE)),$(CMA_DTS_FILE),)
#TFTP_ROOT:=/var/lib/tftpboot
WIFI_DIR:=$(KERNEL_BUILD_DIR)/ottwifi-*
WIFI_KO:=$(shell [ -d $(WIFI_DIR) ] && find $(WIFI_DIR) -name *.ko)
BTUSB_DIR:=$(KERNEL_BUILD_DIR)/ottbtusb-*
BTUSB_KO:=$(shell [ -d $(BTUSB_DIR) ] && find $(BTUSB_DIR) -name *.ko)

ifeq ($(CONFIG_RTD_1295_HWNAT),y)
LAN_ETH:=eth1
else
LAN_ETH:=eth0
endif

ANDROID_OUT_DIR:=$(if $(shell [[ "$(CONFIG_TARGET_NAS_IMG_ANDROID_OUT_DIR)" = /* ]] && echo 1)\
    ,$(CONFIG_TARGET_NAS_IMG_ANDROID_OUT_DIR),$(TOPDIR)/$(CONFIG_TARGET_NAS_IMG_ANDROID_OUT_DIR))
ANDROID_PRODUCT_OUT:=$(ANDROID_OUT_DIR)/$(CONFIG_TARGET_NAS_IMG_ANDROID_TARGET_PATH)
ANDROID_HOST_OUT:=$(ANDROID_OUT_DIR)/host/linux-x86
ANDROID_MODULE_PATH:=$(shell [ -d "$(ANDROID_PRODUCT_OUT)/vendor" ] \
    && echo vendor/modules || echo system/vendor/modules)
ANDROID_DHCP_FILES:=$(shell [ -d "$(ANDROID_PRODUCT_OUT)/vendor" ] \
    && echo system/etc/init/dhcpcd.rc system/etc/init/iprenew.rc\
    || echo root/init.kylin.rc)

ifeq ($(CONFIG_XEN),y)
XEN_DIR:=$(TOPDIR)/../xen
HYP_IMG:=$(XEN_DIR)/xen/xen
XENOTT_IMGS_DIR=$(TOPDIR)/../image_file/components/tmp/pkgfile/package5
endif

IMG_NAME:=image_file
IMG_REV:=160868
IMG_VERSION:=r$(IMG_REV)

ifeq ($(CONFIG_PACKAGE_kmod-cfg80211),y)
USER_NAME:=$(shell id -u -n)
define Image/Prepare/Setconfig
	grep -q guest $(TARGET_DIR)/etc/config/dhcp || cat ../conf/dhcp.guest >> $(TARGET_DIR)/etc/config/dhcp
	sed -i 's/OpenWrt_2.4G$$$$/OpenWrt_2.4G_$(USER_NAME)/g' $(TARGET_DIR)/etc/config/wireless
	sed -i 's/OpenWrt_5G$$$$/OpenWrt_5G_$(USER_NAME)/g' $(TARGET_DIR)/etc/config/wireless
	sed -i 's/GuestWRT_5G$$$$/GuestWRT_5G_$(USER_NAME)/g' $(TARGET_DIR)/etc/config/wireless
	sed -i "/\b$(LAN_ETH)\b/a\	option type 'bridge'" $(TARGET_DIR)/etc/config/network
	if [ "$(SUBTARGET)" = "mnas_emmc" ]; then \
	grep -q veth $(TARGET_DIR)/etc/config/network || \
	    (sed -i "s/$(LAN_ETH)/$(LAN_ETH) veth1/g" $(TARGET_DIR)/etc/config/network); \
	grep -q force $(TARGET_DIR)/etc/config/dhcp || sed -i "/config dhcp lan/a\	option force '1'" $(TARGET_DIR)/etc/config/dhcp; \
	fi
	if [ "$(CONFIG_KERNEL_RTL_WAN_MAC5)" = "y" ]; then \
	grep -q 'eth0.100' $(TARGET_DIR)/etc/config/network || \
	    (sed -i "s/eth0/eth0.100/g" $(TARGET_DIR)/etc/config/network); \
	grep -q 'eth0.200' $(TARGET_DIR)/etc/config/network || \
	    (sed -i "s/eth1/eth0.200/g" $(TARGET_DIR)/etc/config/network); \
	fi
	if [ "$(CONFIG_KERNEL_RTL_CPU_TAG)" = "y" ]; then \
	grep -q 'eth2' $(TARGET_DIR)/etc/config/network || \
	    (sed -i "s/eth1/eth1 eth2/g" $(TARGET_DIR)/etc/config/network); \
	fi
	if [ "$(CONFIG_PACKAGE_luci-proto-3g)" = "y" ]; then \
		grep -q wan2 $(TARGET_DIR)/etc/config/network || cat ../conf/network.3gwan >> $(TARGET_DIR)/etc/config/network; \
		grep -q wan2 $(TARGET_DIR)/etc/config/firewall || sed -i "/'wan'/a\	list   network		'wan2'" $(TARGET_DIR)/etc/config/firewall; \
	fi
	if [ "$(CONFIG_PACKAGE_kmod-rtl8192cd)" = "y" ]; then \
		if [ "$(CONFIG_ENABLE_EFUSE)" = "" ]; then \
			$(CP) $(CURDIR)/files/load_calibration $(TARGET_DIR)/usr/local/sbin; \
			$(CP) $(CURDIR)/files/factory $(TARGET_DIR)/usr/local/bin; \
		fi; \
		if [ "$(CONFIG_RTL_MESH_SUPPORT)" = "y" ]; then \
			$(CP) $(CURDIR)/files/MAP.tar.gz $(TARGET_DIR)/usr/local/etc; \
			$(CP) $(CURDIR)/files/MPP.tar.gz $(TARGET_DIR)/usr/local/etc; \
		fi; \
	fi
endef
else
define Image/Prepare/Setconfig
	rm -f $(TARGET_DIR)/etc/config/wireless
	rm -f $(TARGET_DIR)/etc/resolv.conf
	$(CP) ../conf/resolv.conf $(TARGET_DIR)/etc/resolv.conf
endef
endif

define Image/Prepare/SetXEN
	if [ ! -d "$(XEN_DIR)/dist/install" ]; then \
		echo 'XEN Tool STACK NOT exist!'; \
		exit -1; \
	fi

	rsync -aH $(XEN_DIR)/dist/install/usr/ $(TARGET_DIR)/usr
	rsync -aH $(XEN_DIR)/dist/install/var/ $(TARGET_DIR)/tmp
	rsync -aH $(XEN_DIR)/dist/install/boot/ $(TARGET_DIR)/boot
	rsync -aH $(XEN_DIR)/dist/install/etc/ $(TARGET_DIR)/etc

	#Patch script
	sed -i 's/shopt -s extglob/shopt -s extglob\nmkdir -p \/var\/lib\/xen\/dump\nmkdir -p \/var\/log\/xen/' $(TARGET_DIR)/etc/init.d/xencommons

	# Setup xen env
	sed -i 's/:\/sbin/:\/sbin:\/usr\/local\/sbin:\/usr\/local\/bin/' $(TARGET_DIR)/etc/profile
	echo -e "\nexport LD_LIBRARY_PATH=/usr/local/lib" >> $(TARGET_DIR)/etc/profile

	# Enable hvc loggin console
	echo 'hvc0::askfirst:/bin/bash --login' >> $(TARGET_DIR)/etc/inittab

	# Create mount point for xen domu imgs partition
	mkdir -p $(TARGET_DIR)/domu

	# Adjust kernel memory reclaimation setting
	sed -i 's/vm\.min_free_kbytes=.*/vm\.min_free_kbytes=8192/' $(TARGET_DIR)/etc/sysctl.conf

	mkdir -p $(TARGET_DIR)/etc/hotplug.d/net
	$(CP) ../conf/ottnet-notify.hotplug $(TARGET_DIR)/etc/hotplug.d/net/10-ottnet-notify
	mkdir -p $(TARGET_DIR)/etc/hotplug.d/iface
	$(CP) ../conf/ottnet-notify.hotplug $(TARGET_DIR)/etc/hotplug.d/iface/10-ottnet-notify
endef

IMAGE_DIR:=$(CURDIR)/$(IMG_NAME)-$(IMG_VERSION)/packages/omv
LAYOUT_SUFFIX:=$(if $(CONFIG_TARGET_LAYOUT_SIZE_4GB),,_$(layout_size))
EMMC_PARTITION_LAYOUT:=$(IMAGE_DIR)/customer/rtk_generic_emmc$(LAYOUT_SUFFIX)/partition$(if $(CONFIG_TARGET_NAS_IMG_EMMC_SWAP),.emmc_swap_700MB,).txt
EMMC_PARTITION_LAYOUT_NAS:=$(IMAGE_DIR)/customer/rtk_generic_emmc_nas/partition.txt

define Image/Build/Profile
	$(CP) $(DTS_DIR)/realtek/$(RTK_CHIP)-$(1).dts $(DTS_FILE)
	$(CP) $(DTS_DIR)/realtek/$(RTK_CHIP)-qa-rescue.dts $(RESCUE_DTS_FILE)

 ifeq ($(CONFIG_XEN),y)
	$(CP) $(DTS_DIR)/realtek/$(RTK_CHIP)-$(1)-xendom0.dts $(DTS_FILE)
	sed -i "/ROOTFS_NORMAL_START/d" $(DTS_FILE)
 endif
 ifeq ($(CONFIG_PACKAGE_kmod-rtd1295cma),y)
	if [ -n "$(CMA_DTS)" ]; then \
	    $(CP) $(DTS_DIR)/realtek/$(CMA_DTS) $(DTS_FILE); \
	    sed -i "/ROOTFS_NORMAL_/d" $(DTS_FILE); \
	fi
 endif
	-(cd $(LINUX_DIR); git apply --whitespace=nowarn  $(CURDIR)/../$(SUBTARGET)/patches/*.patch)
	sed -i "/linux,initrd/d" $(DTS_FILE)
	if [ "$(SUBTARGET)" = "nas_emmc" ] && [ "$(CONFIG_XEN)" != y ] && [ "$(CONFIG_RTK_NAS_TRANSCODING)" != y ]; then \
	    sed -i "/ION_MEDIA_HEAP_PHYS/d" $(DTS_FILE); \
	    sed -i "/rtd-129x-ion/d" $(DTS_FILE); \
	fi
	# Use 4MB initrd on pure OpenWRT
	if [ "$(SUBTARGET)" = "nas_emmc" ] || [ "$(SUBTARGET)" = "nas_spi" ] && [ "$(CONFIG_XEN)" != "y" ]; then \
	    sed -i "/memory.h/a\#undef ROOTFS_RESCUE_SIZE\n#define ROOTFS_RESCUE_SIZE      (0x00400000)" $(RESCUE_DTS_FILE); \
	fi
	if [ "$(secure_boot)" = "y" ]; then \
	    sed -i "/memory.h/a\/memreserve/ ION_SECURE_HEAP_PHYS       ION_SECURE_HEAP_SIZE        ; /* 0x32800000 ~ 0x3effffff */" $(RESCUE_DTS_FILE); \
	    if [ "$(findstring 2GB,$(1))" = "2GB" ]; then \
		sed -i "/memory.h/a\/memreserve/ ION_SECURE_HEAP_PHYS       ION_SECURE_HEAP_SIZE        ; /* 0x32800000 ~ 0x3effffff */" $(DTS_FILE); \
	    fi; \
	    if [ -d "$(TOPDIR)/../TEE" ]; then \
		pushd "$(TOPDIR)/../TEE"; \
		    env -i ./build_gl_hello_world.sh; \
		popd; \
	    fi; \
	fi
	if [ "$(CONFIG_RTK_NAS_TRANSCODING)" = y ]; then \
	    sed -i "s/ION_MEDIA_HEAP_SIZE/ION_MEDIA_HEAP_TCSZ/" $(DTS_FILE); \
	    sed -i "s/cma-region-info = <0x00000000 0x02000000 0x08000000>/cma-region-info = <0x00000000 0x02000000 0x20000000>/" $(DTS_FILE); \
	    echo -e "\nexport GST_OMX_CONFIG_DIR=/usr/local/etc/xdg" >> $(TARGET_DIR)/etc/profile; \
	fi
	if [ "$(SUBTARGET)" = "mnas_emmc" ]; then \
		if [ "$(CONFIG_ANDROID_CONSOLE)" = "y" ]; then \
			sed -i "s/loglevel=/androidboot.selinux=permissive console.switch=android loglevel=/" $(DTS_FILE); \
		else \
			sed -i "s/loglevel=/androidboot.selinux=permissive console.switch=openwrt loglevel=/" $(DTS_FILE); \
		fi \
	fi
 ifeq ($(CONFIG_RTD_1295_HWNAT),y)
	sed -i '$$$$e cat $(CURDIR)/../dts/patches/001-Enable-router-mac-but-disable-umac.patch' $(DTS_FILE)
  ifeq ($(CONFIG_RTL_FASTPATH_HWNAT_SUPPORT_KERNEL_3_X),y)
	sed -i '/hwnat: gmac@0x98060000/a\              offload_enable = <1>; /* 0:disable, 1:enable HW NAT offload */' $(DTS_FILE)
  endif
 endif

	$(call Image/BuildDTB, $(DTS_FILE),$(DTB_FILE),"-p 8192")
	$(call Image/BuildDTB, $(RESCUE_DTS_FILE),$(RESCUE_DTB_FILE),"-p 8192")

 ifeq ($(SUBTARGET),mnas_emmc)
	sed -i 's|^.*\(part.*vendor.*ext4.*\)|$(if $(CONFIG_TARGET_ANDROID_N),,#)\1|g' $(EMMC_PARTITION_LAYOUT)
 else ifeq ($(CONFIG_XEN),y)
	# Only 7.0 contains "vendor" directory/partition
	if [ -d "$(ANDROID_PRODUCT_OUT)/vendor" ]; then \
		sed -i 's|^.*\(part.*vendor.*ext4.*\)|\1|g' $(EMMC_PARTITION_LAYOUT); \
	else \
		sed -i 's|^.*\(part.*vendor.*ext4.*\)|#\1|g' $(EMMC_PARTITION_LAYOUT); \
	fi
 else
	sed -i 's|^.*\(part.*vendor.*ext4.*\)|#\1|g' $(EMMC_PARTITION_LAYOUT)
 endif
endef

-include $(CURDIR)/image-dl.mk
-include $(CURDIR)/audiofw-dl.mk

#################################################
# Shared BuildImage defines
#################################################

define Image/Build/Rtd1295
	if [ "$(TFTP_ROOT)" != "" ]; then \
	$(CP) $(IMAGE_FILE) $(DTB_FILE) $(RESCUE_DTB_FILE) $(TFTP_ROOT); \
	fi
	$(CP) $(IMAGE_FILE)  $(IMAGE_DIR)/$(layout_type).uImage
	$(CP) $(DTB_FILE) $(IMAGE_DIR)/android.$(layout_type).dtb
	$(CP) $(RESCUE_DTB_FILE) $(IMAGE_DIR)/rescue.$(layout_type).dtb

	if [ "$(CONFIG_XEN)" == y ]; then \
		$(CP) $(HYP_IMG) $(IMAGE_DIR)/xen.img; \
	fi

	$(call Image/mkfs/prepare)

	$(TAR) -cvjBpf $(IMAGE_DIR)/root.$(layout_type).tar.bz2  --numeric-owner --owner=0 --group=0 -C $(TARGET_DIR)/ .

	if [ "$(layout_type)" = "nand" ]; then \
	    sed -i -r "s/^NAND_PAGE_SIZE=[0-9]+.*/NAND_PAGE_SIZE=$(NAND_PAGE_SIZE)/g" $(IMAGE_DIR)/Makefile.in; \
	    sed -i -r "s/^NAND_BLOCK_SIZE=[0-9]+.*/NAND_BLOCK_SIZE=$(NAND_BLOCK_SIZE)/g" $(IMAGE_DIR)/Makefile.in; \
	fi

	if [ "$(CONFIG_RTK_OVERRIDE_OVERLAY_FS)" = "y" ]; then \
	    sed -i 's|^.*\(part.*etc.*ext4.*bin\)\( [0-9]*\)|\1 $(CONFIG_OVERLAY_FS_SIZE)|g' $(EMMC_PARTITION_LAYOUT); \
	    sed -i 's|^.*\(part.*etc.*ext4.*bin\)\( [0-9]*\)|\1 $(CONFIG_OVERLAY_FS_SIZE)|g' $(EMMC_PARTITION_LAYOUT_NAS); \
	fi


	(cd $(IMG_NAME)-$(IMG_VERSION); \
	    export ANDROID_PRODUCT_OUT="$(ANDROID_PRODUCT_OUT)"; \
	    export ANDROID_HOST_OUT="$(ANDROID_HOST_OUT)"; \
	    make TARGET=$(PACKAGES) layout_type=$(layout_type) layout_size=$(layout_size) \
	    layout_use_emmc_swap=$(layout_use_emmc_swap) \
	    ANDROID_IMGS=$(ANDROID_IMGS) install_bootloader=$(install_bootloader) \
	    install_factory=$(install_factory) update_etc=$(update_etc) logger_level=$(logger_level) \
	    hash_imgfile=$(hash_imgfile) verify=$(verify) \
	    offline_gen=$(offline_gen) gen_install_binary=$(gen_install_binary) \
	    SECURE_BOOT=$(secure_boot) efuse_key=$(efuse_key) \
	    DUAL_BOOT=$(dual_boot) \
	    KERNELDT_MINSIZE=$(KERNELDT_MINSIZE) RESCUEDT_MINSIZE=$(RESCUEDT_MINSIZE) \
	    LINUX_MINSIZE=$(LINUX_MINSIZE) RESCUEROOTFS_MINSIZE=$(RESCUEROOTFS_MINSIZE) \
	    AUDIO_MINSIZE=$(AUDIO_MINSIZE) \
	    chip_rev=$(chip_rev) \
	    HYPERVISOR=$(HYPERVISOR) \
	    F_EXT=$(F_EXT) \
	    AUDIOADDR=$(AUDIOADDR) \
	    install_avfile_count=$(install_avfile_count) \
	    ; cd -)
	/bin/mv install.img $(BIN_DIR)
endef

INSTALL_A:=install_a
AUDIO_FW_NAME:=$(MNAS_AUDIO_FILE)
ifneq ($(SUBTARGET),mnas_emmc)
 ifneq ($(CONFIG_XEN),y)
#AUDIO_FW_NAME:=$(NAS_AUDIO_FILE)
AUDIO_FW_NAME:=$(if $(CONFIG_AUDIO_ADDR_LEGACY),$(NAS_AUDIO_FILE),$(MNAS_AUDIO_FILE))
 endif
endif
# Android: $(TOPDIR)/../android
# mali: $(KERNEL_BUILD_DIR)/mali/*.ko
# audio fw: CONFIG_UPDATE_AUDIO_FW, $(CURDIR)/files/bluecore.audio.zip
define Image/Build/nas
	diff $(CURDIR)/files/$(INSTALL_A) $(IMAGE_DIR)/installer/install_a \
	    || cp -fp $(CURDIR)/files/$(INSTALL_A) $(IMAGE_DIR)/installer/install_a
	if [ "$(SUBTARGET)" = "mnas_emmc" ]; then \
	    for dir in root system vendor; do \
	        if [ -d $(ANDROID_PRODUCT_OUT)/$$$$dir ]; then \
	            rm -f $(IMAGE_DIR)/$$$$dir; \
	            ln -sf $(ANDROID_PRODUCT_OUT)/$$$$dir $(IMAGE_DIR)/$$$$dir; \
	        fi; \
	    done; \
	    [ -d "$(KERNEL_BUILD_DIR)/mali" ] && \
	        cp $(KERNEL_BUILD_DIR)/mali/*.ko $(IMAGE_DIR)/$(ANDROID_MODULE_PATH)/; \
	    if [ "$(CONFIG_PACKAGE_kmod-cfg80211)" == "" ]; then \
		[ "$(WIFI_KO)" != "" ] && \
		cp $(WIFI_KO) $(IMAGE_DIR)/$(ANDROID_MODULE_PATH)/; \
		touch $(TARGET_DIR)/mnt/android/.ottwifi; \
		for file in $(ANDROID_DHCP_FILES); do \
			sed -i "s/_eth9/_eth0/g" $(IMAGE_DIR)/$$$$file; \
		done; \
	    else \
		rm -f $(TARGET_DIR)/mnt/android/.ottwifi; \
		for file in $(ANDROID_DHCP_FILES); do \
			sed -i "s/_eth0/_eth9/g" $(IMAGE_DIR)/$$$$file; \
		done; \
	    fi; \
	    if [ "$(CONFIG_PACKAGE_bluez-libs)" == "" ]; then \
		[ "$(BTUSB_KO)" != "" ] && \
		cp $(BTUSB_KO) $(IMAGE_DIR)/$(ANDROID_MODULE_PATH)/; \
	    fi; \
	    chmod 644 $(IMAGE_DIR)/$(ANDROID_MODULE_PATH)/*.ko; \
	fi

	# Also copy AudioFW if XEN is used
	if [ "$(CONFIG_XEN)" == y ]; then \
		if [ ! -f "$(CURDIR)/files/$(AUDIO_FW_NAME)" ]; then \
			echo 'Building XEN-Dom0 must have valid AudioFW!'; \
			exit -1; \
		fi; \
		for dir in system vendor; do \
		    if [ -d $(ANDROID_PRODUCT_OUT)/$$$$dir ]; then \
	                rm -f $(IMAGE_DIR)/$$$$dir; \
	                ln -sf $(ANDROID_PRODUCT_OUT)/$$$$dir $(IMAGE_DIR)/$$$$dir; \
		    fi; \
		done; \
		mkdir -p $(IMAGE_DIR)/xen; \
		cp $(XENOTT_IMGS_DIR)/emmc.uImage $(IMAGE_DIR)/xen/OTT_kernel; \
		cp $(XENOTT_IMGS_DIR)/android.emmc.dtb $(IMAGE_DIR)/xen/OTT_dtb; \
		cp $(XENOTT_IMGS_DIR)/android.root.emmc.cpio.gz_pad.img $(IMAGE_DIR)/xen/OTT_ramdisk; \
		if [ -e $(XENOTT_IMGS_DIR)/../../../packages/package5/rescue.root.domu.cpio.gz_pad.img ]; then \
			cp $(XENOTT_IMGS_DIR)/../../../packages/package5/rescue.root.domu.cpio.gz_pad.img $(IMAGE_DIR)/xen/OTT_recovery; \
		fi; \
	fi

	-[ -f "$(CURDIR)/files/$(AUDIO_FW_NAME)" ] && \
	    $(CP) $(CURDIR)/files/$(AUDIO_FW_NAME) $(IMAGE_DIR)/bluecore.audio.zip
	$(call Image/Build/Profile,$(patsubst %-B00,%,$(PROFILE)))
	$(call Image/Build/Rtd1295,$(1))
endef

define Image/Prepare
	$(call Image/Prepare/Setconfig)	
 ifeq ($(CONFIG_XEN),y)
	$(call Image/Prepare/SetXEN)
 endif
 ifeq ($(CONFIG_UPDATE_AUDIO_FW),y)
	for file in $(AUDIO_FILES); do \
	    svn export --force -r$(AUDIO_REVISION) $(AUDIO_URL)/$$$${file} $(CURDIR)/files/$$$${file}; \
	done

	if [ -n "$(MNAS_AUDIO_FILE)" ] && [ -f "$(CURDIR)/files/$(MNAS_AUDIO_FILE)" ]; then \
	    sed -i 's|^\(MNAS_AUDIO_FILE:=\).*$$$$|\1$(MNAS_AUDIO_FILE)|g' $(CURDIR)/audiofw.mk; \
	fi

 endif
	if [ "$(CONFIG_PACKAGE_zulu)" = "y" ]; then \
	    echo -e "JAVA_HOME=/usr/lib/jvm/zulu-8" >> $(TARGET_DIR)/etc/profile; \
	    echo -e 'PATH=$$$$PATH:$$$$JAVA_HOME/bin' >> $(TARGET_DIR)/etc/profile; \
	fi

	if [ "$(CONFIG_ZULU_32BIT_JVM)" = "y" ]; then \
	    sed -i 's|^RTLDLIST=\(.*\)|RTLDLIST="\1 /lib/ld-linux-armhf.so.3"|g' $(TARGET_DIR)/usr/bin/ldd; \
	fi

endef

define Image/Build
	$(if $(Image/Build/$(1)), \
		$(call Image/Build/$(1),$(1))
	)
endef

$(eval $(call BuildImage))
