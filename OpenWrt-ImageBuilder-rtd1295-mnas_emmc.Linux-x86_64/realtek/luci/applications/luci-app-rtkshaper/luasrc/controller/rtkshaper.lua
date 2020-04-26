-- Copyright 2011 Manuel Munz <freifunk somakoma de>
-- Licensed to the public under the Apache License 2.0.

module "luci.controller.rtkshaper"

function index()
	entry({"admin", "network", "rtkshaper"}, cbi("rtkshaper"), _("Traffic Shaper"), 80)
end

