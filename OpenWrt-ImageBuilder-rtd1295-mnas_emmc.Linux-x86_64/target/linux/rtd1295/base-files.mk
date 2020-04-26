define Package/base-files/install-target

 ifeq ($(CONFIG_RTD_1295_HWNAT),y)
	sed -i 's|192.168.0.9|192.168.1.9|g' $(1)/etc/config/network
	cat $(PLATFORM_DIR)/conf/network.nat >> $(1)/etc/config/network
 else
	sed -i 's|wan|lan|g' $(1)/etc/config/network
 endif

	cat $(PLATFORM_DIR)/conf/network.guest >> $(1)/etc/config/network

endef
