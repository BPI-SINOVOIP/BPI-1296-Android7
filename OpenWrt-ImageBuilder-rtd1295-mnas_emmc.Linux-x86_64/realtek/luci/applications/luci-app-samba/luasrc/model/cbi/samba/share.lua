-- Copyright 2010 Jo-Philipp Wich <jow@openwrt.org>
-- Licensed to the public under the Apache License 2.0.


m = Map("samba", translate("Samba - Share Entry"))
m.redirect = luci.dispatcher.build_url("admin/NAS/samba")

if not arg[1] or m.uci:get("samba", arg[1]) ~= "sambashare" then
	luci.http.redirect(m.redirect)
	return
end


share = m:section(NamedSection, arg[1], "sambashare", translate("Share Entry"))
share.anonymous = true
share.addremove = false

name = share:option(Value, "name", translate("Name"))
function name.write(self, s, val)
	if val and #val > 0 then
		local command = "/sbin/uci set samba."..s..".state='m'"
		luci.sys.call(command)
		Value.write(self, s, val)
	end
end

pth = share:option(Value, "path", translate("Path"))
if nixio.fs.access("/etc/config/fstab") then
        pth.titleref = luci.dispatcher.build_url("admin", "system", "fstab")
end

share:option(Value, "users", translate("Allowed users")).rmempty = true

ro = share:option(Flag, "read_only", translate("Read-only"))
ro.rmempty = false
ro.enabled = "yes"
ro.disabled = "no"

go = share:option(Flag, "guest_ok", translate("Allow guests"))
go.rmempty = false
go.enabled = "yes"
go.disabled = "no"

cm = share:option(Value, "create_mask", translate("Create mask"),
        translate("Mask for new files"))
cm.rmempty = true
cm.size = 4

dm = share:option(Value, "dir_mask", translate("Directory mask"),
        translate("Mask for new directories"))
dm.rmempty = true
dm.size = 4


return m
