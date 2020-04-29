--[[
LuCI - Lua Configuration Interface

Copyright 2012 Luka Perkov <freecwmp at lukaperkov.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

]]--

m = Map("freecwmp", translate("freecwmp"))
m.on_after_commit = function() luci.sys.call("/etc/init.d/freecwmpd restart") end

l = m:section(TypedSection, "local", translate("Local"), translate("freecwmpd daemon options"))
l.anonymous = true
l.addremove = false

interfaces = l:option(Value, "interface", translate("Interface"))
for _, v in pairs(luci.sys.net.devices()) do                                    
  if v ~= "lo" then interfaces:value(v) end
end 
freecwmp_port = l:option(Value, "port", translate("Port"))
freecwmp_port.datatype = "portrange"
freecwmp_port.placeholder = "0-65535"

a = m:section(TypedSection, "acs", translate("ACS"), translate("ACS connection settings"))
a.anonymous = true
a.addremove = false

acs_scheme = a:option(Value, "scheme", translate("Transport scheme"))
acs_scheme:value("http")                                    
acs_scheme:value("https")
a:option(Value, "username", translate("Username"))
acs_password = a:option(Value, "password", translate("Password"))
acs_password.password = true
a:option(Value, "hostname", translate("Hostname"))
acs_port = a:option(Value, "port", translate("Port"))
acs_port.datatype = "portrange"
acs_port.placeholder = "0-65535"
a:option(Value, "path", translate("Path"))

d = m:section(TypedSection, "device", translate("Device"), translate("TR-069 device options"))
d.anonymous = true
d.addremove = false

d:option(Value, "manufacturer", translate("Manufacturer"))
d:option(Value, "oui", translate("OUI"))
d:option(Value, "product_class", translate("Product class"))
d:option(Value, "serial_number", translate("Serial number"))
d:option(Value, "hardware_version", translate("Hardware version"))
d:option(Value, "software_version", translate("Software version"))

return m
