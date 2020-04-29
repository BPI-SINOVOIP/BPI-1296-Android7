#!/bin/sh
# All config needs to be done in /etc/config/rtkshaper

. /lib/functions.sh
. /lib/functions/network.sh
. /lib/rtkshaper_func.sh

[ -e /etc/config/network ] && {
        # only try to parse network config on openwrt

        find_ifname() {(
                reset_cb
                include /lib/network
                scan_interfaces
                config_get "$1" ifname
        )}
} || {
        find_ifname() {
                echo "Interface not found."
                exit 1
        }
}

set_tc_rules()
{
	local IFACE=$1
	local CEIL_RATE=$2
	local DIR=$3
	local cls_id=20
	local prio=

	PRIO_RATE=`expr $CEIL_RATE / 3` # Ceiling for prioirty
	BE_RATE=`expr $CEIL_RATE / 3`   # Min for best effort
	BK_RATE=`expr $CEIL_RATE / 3`   # Min for background
	BE_CEIL=`expr $CEIL_RATE - 16`  # A little slop at the top
	DEF_CLSID=12

        _tc qdisc add dev $IFACE root handle 1: htb default ${DEF_CLSID}
        _tc class add dev $IFACE parent 1: classid 1:1 htb rate ${CEIL_RATE}kbit ceil ${CEIL_RATE}kbit

	_tc class add dev $IFACE parent 1:1 classid 1:11 htb rate ${PRIO_RATE}kbit ceil ${PRIO_RATE}kbit prio 1
	_tc class add dev $IFACE parent 1:1 classid 1:12 htb rate ${BE_RATE}kbit ceil ${BE_CEIL}kbit prio 2
	_tc class add dev $IFACE parent 1:1 classid 1:13 htb rate ${BK_RATE}kbit ceil ${BE_CEIL}kbit prio 3

	_tc qdisc add dev $IFACE parent 1:11 handle 110: sfq perturb 10
	_tc qdisc add dev $IFACE parent 1:12 handle 120: sfq perturb 10

	prio=5
	for cls_section in $CLASSIFIES; do
		cls_id=$(($cls_id+1))
		for s in devip dllimit ullimit; do
			config_get $s $cls_section $s
		done

		[ ! -z $dllimit ] && [ $dllimit -lt ${CEIL_RATE} ] || dllimit=${CEIL_RATE}
		[ ! -z $ullimit ] && [ $ullimit -lt ${CEIL_RATE} ] || ullimit=${CEIL_RATE}

		if [ "$devip" = "all" ] || [ -z $devip ]; then
			devip="0.0.0.0/0"
		else
			devip="${devip}/32"
		fi

		if [ "$DIR" = "download" ]; then
			target="dst"
			bound=$dllimit
		else
			target=src
			bound=$ullimit
		fi
		_tc class add dev $IFACE parent 1:13 classid 1:${cls_id} htb rate $((PRIO_RATE/nCLASSIFIES))kbit ceil ${bound}kbit prio $prio
		_tc qdisc add dev $IFACE parent 1:$cls_id handle ${cls_id}0: sfq perturb 10
		_tc filter add dev $IFACE protocol ip parent 1: prio $prio u32 match ip $target $devip flowid 1:$cls_id

		prio=`expr $prio + 1`
	done

	# TOS Minimum Delay (ssh, NOT scp) in 1:11:
	_tc filter add dev $IFACE parent 1:0 protocol ip prio 10 u32 \
	      match ip tos 0x10 0xff  flowid 1:11

	_tc filter add dev $IFACE parent 1:0 protocol ip prio 10 u32 \
	        match ip protocol 1 0xff flowid 1:11

	_tc filter add dev $IFACE parent 1: protocol ip prio 10 u32 \
		match ip protocol 6 0xff \
		match u8 0x05 0x0f at 0 \
		match u16 0x0000 0xffc0 at 2 \
		match u8 0x10 0xff at 33 \
		flowid 1:11
}

download_rules()
{
	local IFACE=$1
	local LINKRATE=$2

        ifconfig $IFACE txqueuelen 1000

	set_tc_rules $IFACE $LINKRATE download
}

upload_rules()
{
	local IFACE=$1
	local LINKRATE=$2

	ifbdev="`tc -p filter show parent ffff: dev ${IFACE} | grep -o -E ifb'[^)\ ]+'`"
	if [ -z $ifbdev ]; then
		ifbdev=$(ifb_name $IFACE)
		_ip link add name $ifbdev type ifb
	fi

	_ip link set dev $ifbdev up
	ifconfig $ifbdev txqueuelen 1000
	_tc qdisc add dev $IFACE handle ffff: ingress

	set_tc_rules $ifbdev $LINKRATE upload

	_tc filter add dev $IFACE parent ffff: protocol ip u32 match u32 0 0 action mirred egress redirect dev $ifbdev
}

start_traffic_shaping()
{
	local config="$1"

	for s in downlink uplink network; do
        	config_get $s $config $s
	done

	if ! network_get_device device "$network"; then
	        logger -t rtkshaper "Error: Could not find the device for network $network, aborting."
        	return 1
	fi

	if [ ! -z "$uplink" ] && [ "$uplink" -ne 0 ]; then
		upload_rules $device $uplink
	fi

	if [ ! -z "$downlink" ] && [ "$downlink" -ne 0 ]; then
		download_rules $device $downlink
	fi

	logger -t rtkshaper "rtkshaper was started on device $device."
}

stop_traffic_shaping()
{
	local config="$1"

	config_get network $config network

	if ! network_get_device device "$network"; then
	        logger -t rtkshaper "Error: Could not find the device for network $network, aborting."
        	return 1
	fi

	ifbdev="`tc -p filter show parent ffff: dev ${device} | grep -o -E ifb'[^)\ ]+'`"

	# Delete existing qdiscs (hide errors)
	_tc qdisc del dev $device ingress #>&- 2>&-
	_tc qdisc del dev $device root #>&- 2>&-
	
	if [ ! -z "$ifbdev" ]; then
		_tc qdisc del root dev $ifbdev #>&- 2>&-
		_ip link set dev $ifbdev down
		_ip link delete $ifbdev type ifb
	fi
}

traffic_shaping_status()
{
	local config="$1"

	config_get network $config network

	if ! network_get_device device "$network"; then
	        logger -t rtkshaper "Error: Could not find the device for network $network, aborting."
        	return 1
	fi

	ifbdev="`tc -p filter show parent ffff: dev ${device} | grep -o -E ifb'[^)\ ]+'`"
	echo "### Statistics ###"
	for dev in $device $ifbdev
	do
		echo "------$dev------"
		echo "# qdiscs #"
		tc -s qdisc show dev $dev
		echo "# class #"
		tc -s class show dev $dev 
		#echo "# filter #"
		#tc -s filter show dev $dev
	done
}

config_cb() {
        option_cb() {
                return 0
        }

        # Section start
        case "$1" in
                classify)
                        option_cb() {
                                append options "$1"
                        }
                ;;
        esac

    # Section end
        config_get TYPE "$CONFIG_SECTION" TYPE
        case "$TYPE" in
		rtkshaper)
			append INTERFACES "$CONFIG_SECTION"
		;;
		classify)
			append CLASSIFIES "$CONFIG_SECTION"
		;;
        esac
}

INTERFACES=""
CLASSIFIES=""
nCLASSIFIES=0

[ -e ./rtkshaper.conf ] && {
        . ./rtkshaper.conf
        config_cb
} || config_load rtkshaper

add_modules

for i in $CLASSIFIES
do
	nCLASSIFIES=`expr $nCLASSIFIES + 1`
done

case "$1" in
        start)
		for iface_section in $INTERFACES; do
                	stop_traffic_shaping $iface_section

			config_get_bool enabled "$iface_section" enabled 1
			if [ "$enabled" -eq 1 ]; then
		                start_traffic_shaping $iface_section
			fi
		done
        ;;
        stop)
		for iface_section in $INTERFACES; do
                	stop_traffic_shaping $iface_section
		done
		remove_modules
        ;;
        status)
		for iface_section in $INTERFACES; do
                	traffic_shaping_status $iface_section
		done
        ;;
esac

