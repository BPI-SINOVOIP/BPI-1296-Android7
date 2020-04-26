-- Copyright 2011 Manuel Munz <freifunk at somakoma dot de>
-- Licensed to the public under the Apache License 2.0.

local wa = require "luci.tools.webadmin"

function add_knownips(field)
	for i, dataset in ipairs(luci.sys.net.arptable()) do
		field:value(dataset["IP address"] .. " (" .. dataset["HW address"] .. ")")
	end
end

m = Map("rtkshaper", translate("Traffic Shaper"),
	translate("Shapes traffic to limit the bandwidth"))

s = m:section(NamedSection, "settings", "rtkshaper", translate("Settings"))
s.anonymous = true

network = s:option(ListValue, "network", translate("Interface"))
luci.tools.webadmin.cbi_add_networks(network)

e = s:option(Flag, "enabled", translate("Enable"))
e.rmempty = false

uplink = s:option(Value, "uplink", translate("Uplink"), translate("Upstream bandwidth in kbit/s"))
uplink.optional = false
uplink.datatype = "and(uinteger,min(1))"
uplink.default = "10240"

uplink = s:option(Value, "downlink", translate("Downlink"), translate("Downstream bandwidth in kbit/s"))
uplink.optional = false
uplink.datatype = "and(uinteger,min(1))"
uplink.default = "10240"

s = m:section(TypedSection, "classify", translate("Shaping to devices"))
s.template = "cbi/tblsection"
s.anonymous = true
s.addremove = true
s.sortable  = true

devip = s:option(Value, "devip", translate("IP/MAC"))
devip.rmempty = true
--devip:value("", translate("all"))
wa.cbi_add_knownips(devip)
--add_knownips(devip)

dl = s:option(Value, "dllimit", translate("Download speed (kbit/s)"))
dl.datatype = "and(uinteger,min(1))"

ul = s:option(Value, "ullimit", translate("Upload speed (kbit/s)"))
ul.datatype = "and(uinteger,min(1))"

return m
