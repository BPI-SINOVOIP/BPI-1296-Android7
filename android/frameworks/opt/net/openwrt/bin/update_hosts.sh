#!/system/bin/sh
File="/system/etc/hosts"
Hostname=$(cat /proc/sys/kernel/hostname)
IsRoPerm=$(mount|grep /system|grep ro)
echo $IsRoPerm
if [ ! -z "$IsRoPerm" -a "$IsRoPerm" != " " ]; then
        #echo "remount rw"
        mount -o remount,rw /system
fi
sed -i "/$Hostname/d" /system/etc/hosts
if [ -f /.ottwifi ]; then
        # echo "is 2in1"
        echo "127.0.0.1\t$Hostname" >> $File
else
        # echo "is 3in1"
        IP=$(ip -4 route list 0/0 | cut -d ' ' -f 3)
        echo "$IP\t$Hostname" >> $File
fi
if [ ! -z "$IsRoPerm" -a "$IsRoPerm" != " " ]; then
        #echo "remount ro"
        mount -o remount,ro /system
fi
