local fs   = require "nixio.fs"
local nw   = require "luci.model.network"
local uci  = require "luci.model.uci".cursor()
local http = require "luci.http"

local function set_wireless_cmssid_uci(option, value)
	local name = "cmssid"
	uci:set("wireless", name, "setting")
	uci:set("wireless", name, option, value)
	uci:commit("wireless")
end

m = SimpleForm("network", translate("Configure SSID"),
			translate("Setup the same SSID for all Access Points, except GUEST network.")
)
m.submit = translate("Save & Apply")
m.reset = false

function m.on_cancel()
	http.redirect(luci.dispatcher.build_url(
			"admin/network/wireless"
	))
end

s = m:field(Value, "cmssid", translate("ESSID"))
s.default = "Openwrt"

function s.cfgvalue(self, section)
	local name = Value.cfgvalue(self, section)
	local ssid = uci:get("wireless", "cmssid", "ssid")
	
	if not ssid then
		ssid = self.default
	end
	
	set_wireless_cmssid_uci("ssid", ssid)
	return ssid
end

function s.write(self, section, value)
	set_wireless_cmssid_uci("ssid", value)
end

e = m:field(Flag, "enable", translate("Enable"))
e.rmempty = false

function e.cfgvalue(self, section)
	local x = uci:get("wireless", "cmssid", "enabled")
	if not x then
		set_wireless_cmssid_uci("enabled", "0")
	end
	
	return (x == "1") and self.enabled or self.disabled
end

function e.write(self, section, value)
	set_wireless_cmssid_uci("enabled", (value == "1") and "1" or "0")
end

return m
