--[[
LuCI - Lua Configuration Interface

Copyright 2012 Luka Perkov <freecwmp at lukaperkov.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

]]--

module("luci.controller.easycwmp", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/easycwmp") then
		return
	end

	entry({"admin", "services", "easycwmp"}, cbi("easycwmp") , _("EasyCwmp"), 60)
end
