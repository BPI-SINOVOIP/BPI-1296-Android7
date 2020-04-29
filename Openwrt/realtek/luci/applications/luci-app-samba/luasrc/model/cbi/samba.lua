-- Copyright 2008 Steven Barth <steven@midlink.org>
-- Copyright 2008 Jo-Philipp Wich <jow@openwrt.org>
-- Licensed to the public under the Apache License 2.0.

m = Map("samba", translate("Samba"))

s = m:section(TypedSection, "samba", "Samba")
s.anonymous = true

s:tab("general",  translate("General Settings"))
s:tab("template", translate("Edit Template"))

ea=s:taboption("general", Flag, "enabled", translate("Enabled"))
ea.rmempty = false
s:taboption("general", Value, "name", translate("Hostname"))
s:taboption("general", Value, "description", translate("Description"))
s:taboption("general", Value, "workgroup", translate("Workgroup"))
s:taboption("general", Value, "homes", translate("Share home-directories"),
        translate("Allow system users to reach their home directories via " ..
                "network shares"))
s:taboption("general", Flag, "disablenb", translate("Disable netbios"))

function ea.cfgvalue(self,section)
        return luci.sys.init.enabled("samba") and self.enabled or self.disabled
end

function ea.write(self, section, value)
        if value == "1" then
                luci.sys.init.enable("samba")
                luci.sys.call("/etc/init.d/samba start >/dev/null")
        else
                luci.sys.call("/etc/init.d/samba stop >/dev/null")
                luci.sys.init.disable("samba")
        end
        return Flag.write(self, section, value)
end

tmpl = s:taboption("template", Value, "_tmpl",
	translate("Edit the template that is used for generating the samba configuration."), 
	translate("This is the content of the file '/etc/samba/smb.conf.template' from which your samba configuration will be generated. " ..
		"Values enclosed by pipe symbols ('|') should not be changed. They get their values from the 'General Settings' tab."))

tmpl.template = "cbi/tvalue"
tmpl.rows = 20

function tmpl.cfgvalue(self, section)
	return nixio.fs.readfile("/etc/samba/smb.conf.template")
end

function tmpl.write(self, section, value)
	value = value:gsub("\r\n?", "\n")
	nixio.fs.writefile("//etc/samba/smb.conf.template", value)
end


share = m:section(TypedSection, "sambashare", translate("Shared Directories"))
share.anonymous = true
share.addremove = true
share.template = "cbi/tblsection"
share.extedit  = luci.dispatcher.build_url("admin/NAS/samba/share/%s")

share.create = function(...)
	local sid = TypedSection.create(...)
	if sid then
		luci.http.redirect(share.extedit % sid)
		return
	end
end


share:option(DummyValue, "name", translate("Name"))
share:option(DummyValue, "path", translate("Path"))
share:option(DummyValue, "users", translate("Allowed users"))
share:option(DummyValue, "read_only", translate("Read-only"))
share:option(DummyValue, "guest_ok", translate("Allow guests"))
share:option(DummyValue, "create_mask", translate("Create mask"),
        translate("Mask for new files"))
share:option(DummyValue, "dir_mask", translate("Directory mask"),
        translate("Mask for new directories"))

function m.on_commit(self,map)
	require("luci.sys").call('/sbin/reload_config')
end

return m
