#!/bin/sh

. /usr/bin/wps_lib.sh

ifname=$1
event=$2

if [ "$event" = "WPS-SUCCESS" ]; then
	#wps success; stop wps in other interfaces and set status to Disabled
	wpsdevs=`get_wps_info $IFACES_ATTR`
	for wpsdev in $wpsdevs
	do
		if [ "$wpsdev" = "$ifname" ]; then continue; fi
		$HOSTAPD_CLI -i $wpsdev wps_cancel
	done
	set_wps_info $STATUS_ATTR $STATUS_DISABLED
	exit
fi
