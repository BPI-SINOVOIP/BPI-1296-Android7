
MODULES='sch_ingress sch_sfq sch_htb sch_fq_codel cls_u32 cls_fw em_u32 act_police act_connmark act_mirred'
DBG_FILE=/tmp/rtkshaper.log
DBG=true

_tc()
{
	if [ "$DBG" = "true" ]; then
		echo "tc $*" >> $DBG_FILE
		tc $* >> $DBG_FILE 2>&1
	else
		tc $*
	fi
}

_ipt()
{
	if [ "$DBG" = "true" ]; then
		echo "iptables $*" >> $DBG_FILE
		iptables $* >> $DBG_FILE 2>&1
	else
		iptables $*
	fi
}

_ip()
{
	if [ "$DBG" = "true" ]; then
		echo "ip $*" >> $DBG_FILE
		ip $* >> $DBG_FILE 2>&1
	else
		ip $*
	fi
}

[ -x /usr/sbin/modprobe ] && {
        insmod="modprobe"
	rmmod="rmmod"
} || {
        insmod="insmod"
        rmmod="rmmod"
}

add_insmod() {
        eval "export isset=\${insmod_$1}"
        case "$isset" in
                1) ;;
                *) {
                        [ "$2" ] && $rmmod $1 >&- 2>&-
                        $insmod $* >&- 2>&-
                };;
        esac
}

add_modules()
{
	for i in $MODULES ; do
        	add_insmod $i
	done
}

remove_modules()
{
	for i in $MODULES ; do
		$rmmod $i
	done
}

ifb_name() {
    local CUR_IF=$1
    local MAX_IF_NAME_LENGTH=15
    local IFB_PREFIX="ifb4"
    local NEW_IFB="${IFB_PREFIX}${CUR_IF}"
    local IFB_NAME_LENGTH=${#NEW_IFB}
    # IFB names can only be 15 chararcters, so we chop of excessive characters
    # at the start of the interface name
    if [ ${IFB_NAME_LENGTH} -gt ${MAX_IF_NAME_LENGTH} ];
    then
        local OVERLIMIT=$(( ${#NEW_IFB} - ${MAX_IF_NAME_LENGTH} ))
        NEW_IFB=${IFB_PREFIX}${CUR_IF:${OVERLIMIT}:$(( ${MAX_IF_NAME_LENGTH} - ${#IFB_PREFIX} ))}
    fi
    echo ${NEW_IFB}
}

reverse()
{
        local line
	if IFS= read -r line
	then
		reverse
		printf '%s\n' "$line"
	fi
}

#TC=_tc
#IPT=_ipt
#IP=_ip

if [ "$DBG" = "true" ]; then
	rm -f $DBG_FILE
fi
