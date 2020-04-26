#!/bin/sh

#   Samba...........................PandoraBox..........................
#=======================================================================
#
#   D-Team Technology Co.,Ltd. ShenZhen
#   ......:Vic
#
#
# ......:.....................,.........................................
#=======================================================================


set_samba(){
	uci set samba.${section}="sambashare"
	uci set samba.${section}.name="Disk_${device}"
	uci set samba.${section}.path="${mountpoint}"
	uci set samba.${section}.state="a"
	uci set samba.${section}.read_only="no"
	uci set samba.${section}.guest_ok="yes"
	uci commit samba
}

set_samba_path(){
	uci set samba.${section}.path="${mountpoint}"
	uci set samba.${section}.state="m"
	uci commit samba
}

device=`basename $DEVPATH`
timeout=2
count=0

case "$ACTION" in
	add)
	
       case "$device" in
                sd*) ;;
                sata*) ;;
                md*) ;;
                hd*) ;;     
                *) return;;
        esac   

	# Ignore nasroot(squashfs) and nasetc
	block info /dev/${device} | grep -q 'TYPE="squashfs"' && exit 0
	block info /dev/${device} | grep -q 'LABEL="nasetc"' && exit 0
	block info /dev/${device} | grep -q 'TYPE="ntfs"' && timeout=10

	while [ "$count" -lt "$timeout" ]; do
		mountpoint=`sed -ne "s|^[^ ]*/$device ||; T; s/ .*//p" /proc/self/mounts`
		[ -n "$mountpoint" ] && break
		sleep 1
		count=`expr $count + 1`
	done

	get_uuid=`block info /dev/${device} | awk -F "UUID=" '{print $2}'| awk -F "\"" '{print $2}'`
	section=$(echo $get_uuid | sed 's/-//g')
	have_uuid=$(uci show samba | grep -c "$section")
	[ "$have_uuid" = "0" ] && set_samba
	[ "$have_uuid" -gt "0" ] && set_samba_path
	/etc/init.d/samba restart
	;;

	remove)

       case "$device" in
                sd*) ;;
                sata*) ;;
                md*) ;;
                hd*) ;;     
                *) return;;
        esac   
        
	section=$(uci show samba | grep "'/mnt/${device}'"  | awk 'BEGIN { FS = "." } ; { print $2 }')
	state=$(uci get samba.${section}.state)
	if [ "$state" = "a" ]; then
		uci delete samba.${section}
		uci commit samba
	elif [ "$state" = "m" ]; then
		uci set samba.${section}.state="d"
		uci commit samba
	fi
	/etc/init.d/samba restart
	;;
esac
