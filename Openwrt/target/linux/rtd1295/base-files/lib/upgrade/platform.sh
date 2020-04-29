
# bins for install_a
RAMFS_COPY_BIN="/usr/bin/cat /usr/bin/chmod /usr/bin/cp /usr/bin/dd \
/usr/bin/echo /usr/bin/mkdir /usr/bin/rm"

DEFAULT_IMAGEPATH="/tmp/firmware.img"

[ "$SAVE_CONFIG" -eq 1 ] && OPT="-o UPGR____ -r" || OPT="-r"

is_nand() {
	[ -f /proc/mtd ] &&	return 0 || return 1
}

platform_check_image() {
	v "platform_check_image"
	v "image path/url: $1"

	if [ "$KEEP_ALIVE" -ne 0 ]; then
		case "$1" in
			http://*|ftp://*)
				eval "wget -O$DEFAULT_IMAGEPATH -q $1"
				IMAGEPATH="$DEFAULT_IMAGEPATH"
				;;
			*)	IMAGEPATH="$1";;
		esac
		tar -Oxf $IMAGEPATH config.txt | grep nas_dual=y || return 1
	fi

	return 0
}

kill_essential_process() {
	local sig="${1:-TERM}"
	local stat

	echo -n "Kill processes which might occupy mmc device ... "
	for stat in /proc/[0-9]*/stat; do
		[ -f "$stat" ] || continue

		local pid name state ppid rest
		read pid name state ppid rest < $stat
		name="${name#(}"; name="${name%)}"

		local cmdline
		read cmdline < /proc/$pid/cmdline

		# Skip kernel threads
		[ -n "$cmdline" ] || continue

		# Skip self process
		[ $pid -ne $$ ] && [ $ppid -ne $$ ] || (echo -n "S($name) " && continue)

		case "$name" in
			*ash*|*init*|*watchdog*|*ssh*|*dropbear*|*telnet*|*login*|*hostapd*|*wpa_supplicant*|*nas*)
				echo -n "$name($pid) "
				kill -$sig $pid 2>/dev/null
			;;
			*)	: ;;
		esac
	done
	echo ""
}

platform_do_upgrade() {
	# $1: image path/url
	v "platform_do_upgrade start"
	v "image path/url: $1"

	case "$1" in
		http://*|ftp://*)
			eval "wget -O$DEFAULT_IMAGEPATH -q $1"
			IMAGEPATH="$DEFAULT_IMAGEPATH"
			;;
		*)	IMAGEPATH="$1";;
	esac

	if [ "$KEEP_ALIVE" -eq 0 ] && ! is_nand; then
		# kill processes which might occupy mmc device
		kill_essential_process TERM
		sleep 3
		kill_essential_process KILL
		v "send SIGQUIT to make procd fall into infinite sleep"
		kill -QUIT 1 2>/dev/null
	fi

	cd / && \
	tar -C /tmp -xvf $IMAGEPATH install_a \
	&& v "/tmp/install_a $OPT $IMAGEPATH" \
	&& /tmp/install_a $OPT $IMAGEPATH

	v "platform_do_upgrade end"
}

