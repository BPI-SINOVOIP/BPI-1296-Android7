device=`basename $DEVPATH`
except=`echo $device | grep -E "mmcblk|mtdblock"`
[ -z "$except" ] && get_uuid=`block info /dev/${device} | awk -F "UUID=" '{print $2}'| awk -F "\"" '{print $2}'`

if [ $device != "block" ] && [ -z "$except" ] && [ -n "$get_uuid" ]; then
    # Ignore nasroot(squashfs) and nasetc
    block info /dev/${device} | grep -q 'TYPE="squashfs"' && exit 0
    block info /dev/${device} | grep -q 'LABEL="nasetc"' && exit 0

    DAAPD_CONF='/etc/forked-daapd.conf'

    # Restart and rebuild database on plug-in
    if [ "$ACTION" = "add" ]; then
        #db_path="`uci get forked-daapd.config.db_path`"
        path="`uci get forked-daapd.config.path`"
        echo "/mnt/${device}" | grep "${path}" \
        && /etc/init.d/forked-daapd restart
    fi
fi
