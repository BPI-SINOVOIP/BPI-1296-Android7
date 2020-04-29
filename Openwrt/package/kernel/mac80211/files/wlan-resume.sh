#!/bin/sh

if [ "$ACTION" = online ]; then
	[ -n "$(echo $(ifconfig | grep $INTERFACE))" ] && {
		for netdev in radio0 radio1
		do
			[ -n "$(echo $(ubus call network.wireless status "{'device':'$netdev'}" | grep "\"$INTERFACE\""))" ] && {
				/sbin/wifi up $netdev
		}
		done
	}
fi
