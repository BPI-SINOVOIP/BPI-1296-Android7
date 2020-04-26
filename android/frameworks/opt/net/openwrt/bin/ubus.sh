#!/system/bin/sh
TMP_ARGS_PATH="/tmp/ubus-args"
ARG1=`cut -f1 $TMP_ARGS_PATH`
ARG2=`cut -f2 $TMP_ARGS_PATH`
ARG3=`cut -f3 $TMP_ARGS_PATH`
ARG4=`cut -f4 $TMP_ARGS_PATH`
if [ "$ARG4" != "" ];then
        /system/bin/ubus $ARG1 $ARG2 $ARG3 "$ARG4" > /tmp/ubus-ret 2>&1
else
        /system/bin/ubus $ARG1 $ARG2 $ARG3 > /tmp/ubus-ret 2>&1
fi
chmod 755 /tmp/ubus-ret
setprop tmp.exec_ubus 0
