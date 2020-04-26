#
# Copyright (C) 2015-2016 Realtek
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=docker
PKG_VERSION:=17.06.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

define Package/docker
  SECTION:=utils
  CATEGORY:=Utilities
  URL:=https://www.docker.com/
  USERID:=docker=999:docker=999
  TITLE:=Docker
  KCONFIG:= \
	CONFIG_ADVISE_SYSCALLS=y \
	CONFIG_BLK_DEV_THROTTLING=y \
	CONFIG_BRIDGE_NETFILTER=y \
	CONFIG_CFQ_GROUP_IOSCHED=y \
	CONFIG_DEVPTS_MULTIPLE_INSTANCES=y \
	CONFIG_IP_VS=y \
	CONFIG_IP_VS_DEBUG=n \
	CONFIG_IP_VS_DH=n \
	CONFIG_IP_VS_FO=n \
	CONFIG_IP_VS_IPV6=n \
	CONFIG_IP_VS_LBLC=n \
	CONFIG_IP_VS_LBLCR=n \
	CONFIG_IP_VS_LC=n \
	CONFIG_IP_VS_NFCT=y \
	CONFIG_IP_VS_NQ=n \
	CONFIG_IP_VS_OVF=n \
	CONFIG_IP_VS_PROTO_AH=n \
	CONFIG_IP_VS_PROTO_ESP=n \
	CONFIG_IP_VS_PROTO_SCTP=n \
	CONFIG_IP_VS_PROTO_TCP=n \
	CONFIG_IP_VS_PROTO_UDP=n \
	CONFIG_IP_VS_RR=y \
	CONFIG_IP_VS_SED=n \
	CONFIG_IP_VS_SH=n \
	CONFIG_IP_VS_SH_TAB_BITS=8 \
	CONFIG_IP_VS_TAB_BITS=12 \
	CONFIG_IP_VS_WLC=n \
	CONFIG_IP_VS_WRR=n \
	CONFIG_POSIX_MQUEUE=y
  DEPENDS:=@TARGET_rtd1295 +libc +libdevmapper
  DEPENDS+= +ca-certificates +iptables-mod-extra \
	    +namespace-utils \
	    +kmod-ipt-extra +kmod-veth
  DEPENDS+= +@KERNEL_PERF_EVENTS +@KERNEL_CGROUPS +@KERNEL_FREEZER \
	    +@KERNEL_CGROUP_FREEZER +@KERNEL_CGROUP_DEVICE +@KERNEL_CGROUP_DEVICE \
	    +@KERNEL_CGROUP_PIDS +@KERNEL_CPUSETS +@KERNEL_CGROUP_CPUACCT \
	    +@KERNEL_MM_OWNER +@KERNEL_MEMCG +@KERNEL_MEMCG_SWAP +@KERNEL_MEMCG_SWAP_ENABLED \
	    +@KERNEL_MEMCG_KMEM +@KERNEL_CGROUP_PERF +@KERNEL_CGROUP_SCHED \
	    +@KERNEL_BLK_CGROUP +@KERNEL_NET_CLS_CGROUP +@KERNEL_CGROUP_NET_PRIO \
	    +@KERNEL_NAMESPACES +@KERNEL_UTS_NS +@KERNEL_IPC_NS +@KERNEL_USER_NS \
	    +@KERNEL_PID_NS +@KERNEL_NET_NS +@KERNEL_SECCOMP_FILTER +@KERNEL_SECCOMP
endef

define Package/docker/description
  Docker prebuilt binaries based on version 17.06.0.
endef

define Build/Prepare
        mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default
endef

define Package/docker/install
	$(INSTALL_DIR) $(1)/usr/local/bin/
	$(INSTALL_BIN) ./files/bin/* $(1)/usr/local/bin/
	ln -s dockerd-$(PKG_VERSION)-dev $(1)/usr/local/bin/dockerd
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/docker.init $(1)/etc/init.d/docker
endef

$(eval $(call BuildPackage,docker))
