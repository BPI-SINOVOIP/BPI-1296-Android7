-- Copyright 2008 Steven Barth <steven@midlink.org>
-- Copyright 2008 Jo-Philipp Wich <jow@openwrt.org>
-- Licensed to the public under the Apache License 2.0.

module("luci.controller.samba", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/samba") then
		return
	end

	entry({"admin", "NAS", "samba"}, cbi("samba"), _("Samba")).dependent = true
	entry({"admin", "NAS", "samba", "share"}, cbi("samba/share"), nil).leaf = true
end
