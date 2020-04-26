
HOSTAPD_CLI="/usr/sbin/hostapd_cli"
WPS_INFO_FILE="/tmp/wps_info"
WPS_EVENT_HANDLE="/usr/bin/wps_event.sh"
WPS_WALK_TIME=120
METHOD_ATTR="METHOD"
STATUS_ATTR="STATUS"
TIME_ATTR="STARTTIME"
IFACES_ATTR="INTERFACES"
STATUS_ACTIVE="Active"
STATUS_DISABLED="Disabled"

set_wps_info()
{
	cfg=$1; shift
	value=$*
	grep -q "^$cfg" $WPS_INFO_FILE 2>/dev/null && sed -i -e s/^$cfg=.*/$cfg="$value"/g $WPS_INFO_FILE || echo "$cfg=$value" >> $WPS_INFO_FILE
}

get_wps_info()
{
	cfg=$1
	echo "`grep "^$cfg" $WPS_INFO_FILE 2>/dev/null | awk -F'=' '{print $2}'`"
}

