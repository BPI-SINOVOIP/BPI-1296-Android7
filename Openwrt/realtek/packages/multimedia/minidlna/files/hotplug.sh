device=`basename $DEVPATH`
except=`echo $device | grep -E "mmcblk|mtdblock"`
[ -z "$except" ] && get_uuid=`block info /dev/${device} | awk -F "UUID=" '{print $2}'| awk -F "\"" '{print $2}'`

if [ $device != "block" ] && [ -z "$except" ] && [ -n "$get_uuid" ]; then
    # Ignore nasroot(squashfs) and nasetc
    block info /dev/${device} | grep -q 'TYPE="squashfs"' && exit 0
    block info /dev/${device} | grep -q 'LABEL="nasetc"' && exit 0

    MINIDLNA_CONFIG_FILE="/tmp/minidlna.conf"
    # Restart and rebuild database on plug-in
    if [ "$ACTION" = "add" ]; then
        db_dir="`grep -s -w -i ^db_dir= ${MINIDLNA_CONFIG_FILE} | sed 's/db_dir=//'`"
        for dir in `grep -s -w -i ^media_dir= ${MINIDLNA_CONFIG_FILE} | sed 's/media_dir=//'`; do
            echo "/mnt/${device}" | grep "${dir}" && rm -f ${db_dir}/files.db \
            && /etc/init.d/minidlna restart && break
        done
    fi
fi
