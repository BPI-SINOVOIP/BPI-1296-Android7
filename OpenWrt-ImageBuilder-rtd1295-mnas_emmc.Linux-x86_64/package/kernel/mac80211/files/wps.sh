#!/bin/sh

#args: <method> [pin code] [wifi interface name ...]
# $0 status
# $0 genpin
# $0 pbc 00000000 wlanX wlanY
# $0 pin 12345670 wlanX wlanY

. /usr/bin/wps_lib.sh

wpsstatus=
randompin=

kill_handler_process()
{
	pids=`pgrep hostapd_cli`
	for pid in $pids
	do
		kill $pid
	done
}

hostapd_cli_wps()
{
	wps_method=$1; shift
	pincode=$1; shift

	set_wps_info $IFACES_ATTR $*

	while [ $# -gt 0 ]
	do
		wpsdev=$1
		if [ "$wps_method" = "pbc" ]; then
			$HOSTAPD_CLI -i $wpsdev wps_pbc 2>&1 > /dev/null
		else
			$HOSTAPD_CLI -i $wpsdev wps_pin any $pincode 2>&1 > /dev/null
		fi

		$HOSTAPD_CLI -i $wpsdev -a $WPS_EVENT_HANDLE &
		shift
	done

	set_wps_info $METHOD_ATTR $wps_method
	set_wps_info $TIME_ATTR `date +%s`
	set_wps_info $STATUS_ATTR $STATUS_ACTIVE
}

get_wps_status()
{
	wpsstatus=`get_wps_info $STATUS_ATTR`
}

is_WPS_in_progress()
{
	if [ ! -f $WPS_INFO_FILE ]; then
		set_wps_info $STATUS_ATTR $STATUS_DISABLED
		return 1
	fi

	curstatus=`get_wps_info $STATUS_ATTR`
	if [ "$curstatus" = "$STATUS_DISABLED" ]; then
		return 1
	fi

	starttime=`get_wps_info $TIME_ATTR`
	curtime="`date +%s`"
	walktime="`expr $curtime - $starttime`"

	if [ $walktime -ge $WPS_WALK_TIME ]; then
		set_wps_info $STATUS_ATTR $STATUS_DISABLED
		return 1
	fi

	return 0
}

gen_random_pin()
{
	pin=
	checksum=0
	total=0

	for i in $(seq 1 7)
	do
		digit=`shuf -i 0-9 -n 1`
	        pin=$(echo "$pin$digit")

	        if [ $((i%2)) -eq 1 ]; then
			total=$((${total}+${digit}*3))
		else
			total=$((${total}+${digit}))
		fi
	done
	checksum=$((10-${total}%10))
	checksum=$((${checksum}%10))

	randompin=$(echo "$pin$checksum")
	#echo "$randompin : $checksum : $total"
}

case $1 in
	status)
		is_WPS_in_progress
		if [ $? -eq 1 ]; then
			kill_handler_process
		fi
		get_wps_status
		echo $wpsstatus
		;;
	curcfg)
		echo `get_wps_info $METHOD_ATTR`
		;;
	genpin)
		gen_random_pin
		echo $randompin
		;;
	pbc | pin)
		if [ $# -le 2 ]; then
			exit 1
		fi

		if ! is_WPS_in_progress ; then
			hostapd_cli_wps $*
		fi
		;;
esac

