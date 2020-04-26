#!/bin/sh
. /lib/netifd/netifd-wireless.sh
. /lib/netifd/hostapd.sh

init_wireless_driver "$@"

MP_CONFIG_INT="mesh_retry_timeout mesh_confirm_timeout mesh_holding_timeout mesh_max_peer_links
	       mesh_max_retries mesh_ttl mesh_element_ttl mesh_hwmp_max_preq_retries
	       mesh_path_refresh_time mesh_min_discovery_timeout mesh_hwmp_active_path_timeout
	       mesh_hwmp_preq_min_interval mesh_hwmp_net_diameter_traversal_time mesh_hwmp_rootmode
	       mesh_hwmp_rann_interval mesh_gate_announcements mesh_sync_offset_max_neighor
	       mesh_rssi_threshold mesh_hwmp_active_path_to_root_timeout mesh_hwmp_root_interval
	       mesh_hwmp_confirmation_interval mesh_awake_window mesh_plink_timeout"
MP_CONFIG_BOOL="mesh_auto_open_plinks mesh_fwding"
MP_CONFIG_STRING="mesh_power_mode"
RTL_INTF_NAME="`ls /sys/class/net/ | grep wlan`"

is_rtl_drv_phy() {
	local phy=$1
	local drvp=`readlink /sys/class/ieee80211/$phy/device/driver 2>/dev/null`
	local drvprefix=`echo ${drvp##*/} | cut -c 1-4`
	[ "$drvprefix" = "rtl8" ] && return 0
	return 1
}

check_rtl_phy_intf_name() {
	local phy=$1

	for intf in $RTL_INTF_NAME; do
		local phyidx="`cat /sys/class/net/$intf/phy80211/index`"
		[ "${phy#phy}" = "$phyidx" ] && echo "$intf";
	done
}

drv_mac80211_init_device_config() {
	hostapd_common_add_device_config

	config_add_string path phy 'macaddr:macaddr'
	config_add_string hwmode
	config_add_int beacon_int chanbw frag rts
	config_add_int rxantenna txantenna antenna_gain txpower distance
	config_add_boolean noscan ht_coex
	config_add_array ht_capab
	config_add_array channels
	config_add_boolean \
		rxldpc \
		short_gi_80 \
		short_gi_160 \
		tx_stbc_2by1 \
		su_beamformer \
		su_beamformee \
		mu_beamformer \
		mu_beamformee \
		vht_txop_ps \
		htc_vht \
		rx_antenna_pattern \
		tx_antenna_pattern
	config_add_int vht_max_a_mpdu_len_exp vht_max_mpdu vht_link_adapt vht160 rx_stbc tx_stbc
	config_add_boolean \
		ldpc \
		greenfield \
		short_gi_20 \
		short_gi_40 \
		max_amsdu \
		dsss_cck_40
}

drv_mac80211_init_iface_config() {
	hostapd_common_add_bss_config

	config_add_string 'macaddr:macaddr' ifname

	config_add_boolean wds powersave
	config_add_int maxassoc
	config_add_int max_listen_int
	config_add_int dtim_period
	config_add_int start_disabled

	# mesh
	config_add_string mesh_id
	config_add_int $MP_CONFIG_INT
	config_add_boolean $MP_CONFIG_BOOL
	config_add_string $MP_CONFIG_STRING
}

mac80211_add_capabilities() {
	local __var="$1"; shift
	local __mask="$1"; shift
	local __out= oifs

	oifs="$IFS"
	IFS=:
	for capab in "$@"; do
		set -- $capab

		[ "$(($4))" -gt 0 ] || continue
		[ "$(($__mask & $2))" -eq "$((${3:-$2}))" ] || continue
		__out="$__out[$1]"
	done
	IFS="$oifs"

	export -n -- "$__var=$__out"
}

mac80211_hostapd_setup_base() {
	local phy="$1"

	json_select config

	[ "$auto_channel" -gt 0 ] && channel=acs_survey
	[ "$auto_channel" -gt 0 ] && json_get_values channel_list channels

	json_get_vars noscan ht_coex
	json_get_values ht_capab_list ht_capab

	ieee80211n=1
	ht_capab=
	case "$htmode" in
		VHT20|HT20) ;;
		HT40*|VHT40|VHT80|VHT160)
			case "$hwmode" in
				a)
					case "$(( ($channel / 4) % 2 ))" in
						1) ht_capab="[HT40+]";;
						0) ht_capab="[HT40-]";;
					esac
				;;
				*)
					case "$htmode" in
						HT40+) ht_capab="[HT40+]";;
						HT40-) ht_capab="[HT40-]";;
						*)
							if [ "$channel" -lt 7 ]; then
								ht_capab="[HT40+]"
							else
								ht_capab="[HT40-]"
							fi
						;;
					esac
				;;
			esac
			[ "$auto_channel" -gt 0 ] && ht_capab="[HT40+]"
		;;
		*) ieee80211n= ;;
	esac

	[ -n "$ieee80211n" ] && {
		append base_cfg "ieee80211n=1" "$N"

		set_default ht_coex 0
		append base_cfg "ht_coex=$ht_coex" "$N"

		json_get_vars \
			ldpc:1 \
			greenfield:0 \
			short_gi_20:1 \
			short_gi_40:1 \
			tx_stbc:1 \
			rx_stbc:3 \
			max_amsdu:1 \
			dsss_cck_40:1

		ht_cap_mask=0
		for cap in $(iw phy "$phy" info | grep 'Capabilities:' | cut -d: -f2); do
			ht_cap_mask="$(($ht_cap_mask | $cap))"
		done

		cap_rx_stbc=$((($ht_cap_mask >> 8) & 3))
		[ "$rx_stbc" -lt "$cap_rx_stbc" ] && cap_rx_stbc="$rx_stbc"
		ht_cap_mask="$(( ($ht_cap_mask & ~(0x300)) | ($cap_rx_stbc << 8) ))"

		mac80211_add_capabilities ht_capab_flags $ht_cap_mask \
			LDPC:0x1::$ldpc \
			GF:0x10::$greenfield \
			SHORT-GI-20:0x20::$short_gi_20 \
			SHORT-GI-40:0x40::$short_gi_40 \
			TX-STBC:0x80::$tx_stbc \
			RX-STBC1:0x300:0x100:1 \
			RX-STBC12:0x300:0x200:1 \
			RX-STBC123:0x300:0x300:1 \
			MAX-AMSDU-7935:0x800::$max_amsdu \
			DSSS_CCK-40:0x1000::$dsss_cck_40

		ht_capab="$ht_capab$ht_capab_flags"
		[ -n "$ht_capab" ] && append base_cfg "ht_capab=$ht_capab" "$N"
	}

	# 802.11ac
	enable_ac=0
	idx="$channel"
	case "$htmode" in
		VHT20) enable_ac=1;;
		VHT40)
			case "$(( ($channel / 4) % 2 ))" in
				1) idx=$(($channel + 2));;
				0) idx=$(($channel - 2));;
			esac
			enable_ac=1
			append base_cfg "vht_oper_chwidth=0" "$N"
			append base_cfg "vht_oper_centr_freq_seg0_idx=$idx" "$N"
		;;
		VHT80)
			case "$(( ($channel / 4) % 4 ))" in
				1) idx=$(($channel + 6));;
				2) idx=$(($channel + 2));;
				3) idx=$(($channel - 2));;
				0) idx=$(($channel - 6));;
			esac
			enable_ac=1
			append base_cfg "vht_oper_chwidth=1" "$N"
			append base_cfg "vht_oper_centr_freq_seg0_idx=$idx" "$N"
		;;
		VHT160)
			case "$channel" in
				36|40|44|48|52|56|60|64) idx=50;;
				100|104|108|112|116|120|124|128) idx=114;;
			esac
			enable_ac=1
			append base_cfg "vht_oper_chwidth=2" "$N"
			append base_cfg "vht_oper_centr_freq_seg0_idx=$idx" "$N"
		;;
	esac

	if [ "$enable_ac" != "0" ]; then
		json_get_vars \
			rxldpc:1 \
			short_gi_80:1 \
			short_gi_160:1 \
			tx_stbc_2by1:1 \
			su_beamformer:1 \
			su_beamformee:1 \
			mu_beamformer:1 \
			mu_beamformee:1 \
			vht_txop_ps:1 \
			htc_vht:1 \
			rx_antenna_pattern:1 \
			tx_antenna_pattern:1 \
			vht_max_a_mpdu_len_exp:7 \
			vht_max_mpdu:11454 \
			rx_stbc:4 \
			tx_stbc:4 \
			vht_link_adapt:3 \
			vht160:2

		append base_cfg "ieee80211ac=1" "$N"
		vht_cap=0
		for cap in $(iw phy "$phy" info | awk -F "[()]" '/VHT Capabilities/ { print $2 }'); do
			vht_cap="$(($vht_cap | $cap))"
		done

		cap_rx_stbc=$((($vht_cap >> 8) & 7))
		[ "$rx_stbc" -lt "$cap_rx_stbc" ] && cap_rx_stbc="$rx_stbc"
		ht_cap_mask="$(( ($vht_cap & ~(0x700)) | ($cap_rx_stbc << 8) ))"

		mac80211_add_capabilities vht_capab $vht_cap \
			RXLDPC:0x10::$rxldpc \
			SHORT-GI-80:0x20::$short_gi_80 \
			SHORT-GI-160:0x40::$short_gi_160 \
			TX-STBC-2BY1:0x80::$tx_stbc \
			SU-BEAMFORMER:0x800::$su_beamformer \
			SU-BEAMFORMEE:0x1000::$su_beamformee \
			MU-BEAMFORMER:0x80000::$mu_beamformer \
			MU-BEAMFORMEE:0x100000::$mu_beamformee \
			VHT-TXOP-PS:0x200000::$vht_txop_ps \
			HTC-VHT:0x400000::$htc_vht \
			RX-ANTENNA-PATTERN:0x10000000::$rx_antenna_pattern \
			TX-ANTENNA-PATTERN:0x20000000::$tx_antenna_pattern \
			RX-STBC1:0x700:0x100:1 \
			RX-STBC12:0x700:0x200:1 \
			RX-STBC123:0x700:0x300:1 \
			RX-STBC1234:0x700:0x400:1 \

		# supported Channel widths
		vht160_hw=0
		[ "$(($vht_cap & 12))" -eq 4 -a 1 -le "$vht160" ] && \
			vht160_hw=1
		[ "$(($vht_cap & 12))" -eq 8 -a 2 -le "$vht160" ] && \
			vht160_hw=2
		[ "$vht160_hw" = 1 ] && vht_capab="$vht_capab[VHT160]"
		[ "$vht160_hw" = 2 ] && vht_capab="$vht_capab[VHT160-80PLUS80]"

		# maximum MPDU length
		vht_max_mpdu_hw=3895
		[ "$(($vht_cap & 3))" -ge 1 -a 7991 -le "$vht_max_mpdu" ] && \
			vht_max_mpdu_hw=7991
		[ "$(($vht_cap & 3))" -ge 2 -a 11454 -le "$vht_max_mpdu" ] && \
			vht_max_mpdu_hw=11454
		[ "$vht_max_mpdu_hw" != 3895 ] && \
			vht_capab="$vht_capab[MAX-MPDU-$vht_max_mpdu_hw]"
			
		# maximum A-MPDU length exponent
		vht_max_a_mpdu_len_exp_hw=0
		[ "$(($vht_cap & 58720256))" -ge 8388608 -a 1 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=1
		[ "$(($vht_cap & 58720256))" -ge 16777216 -a 2 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=2
		[ "$(($vht_cap & 58720256))" -ge 25165824 -a 3 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=3
		[ "$(($vht_cap & 58720256))" -ge 33554432 -a 4 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=4
		[ "$(($vht_cap & 58720256))" -ge 41943040 -a 5 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=5
		[ "$(($vht_cap & 58720256))" -ge 50331648 -a 6 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=6
		[ "$(($vht_cap & 58720256))" -ge 58720256 -a 7 -le "$vht_max_a_mpdu_len_exp" ] && \
			vht_max_a_mpdu_len_exp_hw=7
		vht_capab="$vht_capab[MAX-A-MPDU-LEN-EXP$vht_max_a_mpdu_len_exp_hw]"

		# whether or not the STA supports link adaptation using VHT variant
		vht_link_adapt_hw=0
		[ "$(($vht_cap & 201326592))" -ge 134217728 -a 2 -le "$vht_link_adapt" ] && \
			vht_link_adapt_hw=2
		[ "$(($vht_cap & 201326592))" -ge 201326592 -a 3 -le "$vht_link_adapt" ] && \
			vht_link_adapt_hw=3
		[ "$vht_link_adapt_hw" != 0 ] && \
			vht_capab="$vht_capab[VHT-LINK-ADAPT-$vht_link_adapt_hw]"

		[ -n "$vht_capab" ] && append base_cfg "vht_capab=$vht_capab" "$N"
	fi

	hostapd_prepare_device_config "$hostapd_conf_file" nl80211
	cat >> "$hostapd_conf_file" <<EOF
${channel:+channel=$channel}
${channel_list:+chanlist=$channel_list}
${noscan:+noscan=$noscan}
$base_cfg

EOF
	json_select ..
}

mac80211_hostapd_setup_bss() {
	local phy="$1"
	local ifname="$2"
	local macaddr="$3"
	local type="$4"

	hostapd_cfg=
	append hostapd_cfg "$type=$ifname" "$N"

	hostapd_set_bss_options hostapd_cfg "$vif" || return 1
	json_get_vars wds dtim_period max_listen_int start_disabled

	set_default wds 0
	set_default start_disabled 0

	[ "$wds" -gt 0 ] && append hostapd_cfg "wds_sta=1" "$N"
	[ "$staidx" -gt 0 -o "$start_disabled" -eq 1 ] && append hostapd_cfg "start_disabled=1" "$N"

	cat >> /var/run/hostapd-$phy.conf <<EOF
$hostapd_cfg
bssid=$macaddr
${dtim_period:+dtim_period=$dtim_period}
${max_listen_int:+max_listen_interval=$max_listen_int}
EOF
}

mac80211_get_addr() {
	local phy="$1"
	local idx="$(($2 + 1))"

	head -n $(($macidx + 1)) /sys/class/ieee80211/${phy}/addresses | tail -n1
}

mac80211_generate_mac() {
	local phy="$1"
	local id="${macidx:-0}"

	local ref="$(cat /sys/class/ieee80211/${phy}/macaddress)"
	local mask="$(cat /sys/class/ieee80211/${phy}/address_mask)"

	[ "$mask" = "00:00:00:00:00:00" ] && {
		mask="ff:ff:ff:ff:ff:ff";

		[ "$(wc -l < /sys/class/ieee80211/${phy}/addresses)" -gt 1 ] && {
			addr="$(mac80211_get_addr "$phy" "$id")"
			[ -n "$addr" ] && {
				echo "$addr"
				return
			}
		}
	}

	local oIFS="$IFS"; IFS=":"; set -- $mask; IFS="$oIFS"

	local mask1=$1
	local mask6=$6

	local oIFS="$IFS"; IFS=":"; set -- $ref; IFS="$oIFS"

	macidx=$(($id + 1))
	[ "$((0x$mask1))" -gt 0 ] && {
		b1="0x$1"
		[ "$id" -gt 0 ] && \
			b1=$(($b1 ^ ((($id - 1) << 2) | 0x2)))
		printf "%02x:%s:%s:%s:%s:%s" $b1 $2 $3 $4 $5 $6
		return
	}

	[ "$((0x$mask6))" -lt 255 ] && {
		printf "%s:%s:%s:%s:%s:%02x" $1 $2 $3 $4 $5 $(( 0x$6 ^ $id ))
		return
	}

	off2=$(( (0x$6 + $id) / 0x100 ))
	printf "%s:%s:%s:%s:%02x:%02x" \
		$1 $2 $3 $4 \
		$(( (0x$5 + $off2) % 0x100 )) \
		$(( (0x$6 + $id) % 0x100 ))
}

find_phy() {
	[ -n "$phy" -a -d /sys/class/ieee80211/$phy ] && return 0
	[ -n "$path" ] && {
		for phy in $(ls /sys/class/ieee80211 2>/dev/null); do
			case "$(readlink -f /sys/class/ieee80211/$phy/device)" in
				*$path) return 0;;
			esac
		done
	}
	[ -n "$macaddr" ] && {
		for phy in $(ls /sys/class/ieee80211 2>/dev/null); do
			grep -i -q "$macaddr" "/sys/class/ieee80211/${phy}/macaddress" && return 0
		done
	}
	return 1
}

mac80211_prepare_rtkmesh() {
	json_select data
	json_get_vars ifname

	json_select config
	json_get_vars mode ssid encryption key mesh_portal smart_roaming
	json_get_vars sm_signal_min sm_speed_weight sm_signal_weight sm_load_weight sm_load_min sm_linktime

	[ -n "$ifname" ] || ifname="wlan${phy#phy}${if_idx:+-$if_idx}"

	json_select ..

	iwpriv $ifname set_mib mesh_id=$ssid
	iwpriv $ifname set_mib mesh_ap_enable=1
	iwpriv $ifname set_mib mesh_portal_enable=1
	iwpriv $ifname set_mib mesh_enable=1
	if [ "$encryption" = "psk2+ccmp" -a -n "$key" ]; then
		iwpriv $ifname set_mib mesh_privacy=4
		iwpriv $ifname set_mib mesh_passphrase=$key
	fi

	[ "$smart_roaming" -gt 0 ] && [ "$mesh_portal" -gt 0 ] && {
		[ "$sm_signal_min" -ge 0 ] && iwpriv $ifname set_mib signal_min=$sm_signal_min
		[ "$sm_speed_weight" -ge 0 ] && iwpriv $ifname set_mib speed_weight=$sm_speed_weight
		[ "$sm_signal_weight" -ge 0 ] && iwpriv $ifname set_mib signal_weight=$sm_signal_weight
		[ "$sm_load_weight" -ge 0 ] && iwpriv $ifname set_mib load_weight=$sm_load_weight
		[ "$sm_load_min" -ge 0 ] && iwpriv $ifname set_mib load_min=$sm_load_min
		[ "$sm_linktime" -ge 0 ] && iwpriv $ifname set_mib linktime_min=$sm_linktime
	}
}

mac80211_check_ap() {
	has_ap=1
}

set_rtk_wds_mib() {

	local wlname="$1"

	json_select_config

	json_get_vars \
	rtk_wds rtk_wds_num rtk_wds_privacy rtk_wds_wepkey rtk_wds_passphrase \
	rtk_wds_macaddr0 rtk_wds_macaddr1 rtk_wds_macaddr2 rtk_wds_macaddr3 \
	rtk_wds_macaddr4 rtk_wds_macaddr5 rtk_wds_macaddr6 rtk_wds_macaddr7

	echo "wds=$rtk_wds  wds_num=$rtk_wds_num  wds_mac=$rtk_wds_macaddr0" > /etc/debug_mib

	iwpriv "$wlname" set_mib wds_enable=0
	[ -n "$rtk_wds" ] && iwpriv "$wlname" set_mib wds_enable="$rtk_wds"

	iwpriv "$wlname" set_mib wds_num=0
	#[ -n "$rtk_wds_num" ] && iwpriv "$wlname" set_mib wds_num="$rtk_wds_num"

	iwpriv "$wlname" set_mib rtk_wds_privacy=0
	[ -n "$rtk_wds_privacy" ] && iwpriv "$wlname" set_mib wds_encrypt="$rtk_wds_privacy"

	iwpriv "$wlname" set_mib rtk_wds_wepkey=0
	[ -n "$rtk_wds_wepkey" ] && iwpriv "$wlname" set_mib wds_wepkey="$rtk_wds_wepkey"

	iwpriv "$wlname" set_mib rtk_wds_passphrase=0
	[ -n "$rtk_wds_passphrase" ] && iwpriv "$wlname" set_mib wds_passphrase="$rtk_wds_passphrase"

	[ -n "$rtk_wds_macaddr0" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr0"
	}

	[ -n "$rtk_wds_macaddr1" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr1"
	}

	[ -n "$rtk_wds_macaddr2" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr2"
	}

	[ -n "$rtk_wds_macaddr3" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr3"
	}

	[ -n "$rtk_wds_macaddr4" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr4"
	}

	[ -n "$rtk_wds_macaddr5" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr5"
	}

	[ -n "$rtk_wds_macaddr6" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr6"
	}

	[ -n "$rtk_wds_macaddr7" ] && {
		iwpriv "$wlname" set_mib wds_add="$rtk_wds_macaddr7"
	}
}

enable_rtk_wds_priv() {

	local wlname="$1"

	json_select config

	json_get_vars \
	rtk_wds rtk_wds_num \
	rtk_wds_macaddr0 rtk_wds_macaddr1 rtk_wds_macaddr2 rtk_wds_macaddr3 \
	rtk_wds_macaddr4 rtk_wds_macaddr5 rtk_wds_macaddr6 rtk_wds_macaddr7

	echo "wds=$rtk_wds  wds_num=$rtk_wds_num  wds_mac=$rtk_wds_macaddr0" > /etc/debug_enable

	[ -n "$rtk_wds_macaddr0" ] && {
		ifconfig "$wlname"-wds0 up
		brctl addif br-lan "$wlname"-wds0
	}

	[ -n "$rtk_wds_macaddr1" ] && {
		ifconfig "$wlname"-wds1 up
		brctl addif br-lan "$wlname"-wds1
	}

	[ -n "$rtk_wds_macaddr2" ] && {
		ifconfig "$wlname"-wds2 up
		brctl addif br-lan "$wlname"-wds2
	}

	[ -n "$rtk_wds_macaddr3" ] && {
		ifconfig "$wlname"-wds3 up
		brctl addif br-lan "$wlname"-wds3
	}

	[ -n "$rtk_wds_macaddr4" ] && {
		ifconfig "$wlname"-wds4 up
		brctl addif br-lan "$wlname"-wds4
	}

	[ -n "$rtk_wds_macaddr5" ] && {
		ifconfig "$wlname"-wds5 up
		brctl addif br-lan "$wlname"-wds5
	}

	[ -n "$rtk_wds_macaddr6" ] && {
		ifconfig "$wlname"-wds6 up
		brctl addif br-lan "$wlname"-wds6
	}

	[ -n "$rtk_wds_macaddr7" ] && {
		ifconfig "$wlname"-wds7 up
		brctl addif br-lan "$wlname"-wds7
	}

}

mac80211_prepare_vif() {
	json_select config

	json_get_vars ifname mode ssid wds powersave macaddr rtk_wds tdls_prohibited tdls_cs_prohibited ieee80211k txbf txbf_mu

	is_rtl_drv_phy $phy && ifname=$(check_rtl_phy_intf_name $phy)
	[ -n "$ifname" ] || ifname="wlan${phy#phy}${if_idx:+-$if_idx}"
	if_idx=$((${if_idx:-0} + 1))

	if [ "$rtk_wds" -gt 0 ]; then
		case "$ifname" in
			wlan0)
				set_rtk_wds_mib "$ifname"
		;;
			wlan1)
				set_rtk_wds_mib "$ifname"
		;;
		esac
	fi

	set_default txbf 0
	set_default txbf_mu 0
	set_default wds 0
	set_default powersave 0
	set_default tdls_prohibited 0
	set_default tdls_cs_prohibited 0
	set_default ieee80211k 0

	if [ -d /proc/$ifname ]; then
		chip_id=$(echo $(cat /proc/$ifname/mib_rf | grep chipVersion) | awk -F' ' '{print $2}' | cut -c 1-8)
		iwpriv $ifname set_mib txbf=$txbf
		if [ "$chip_id" = "RTL8822B" -a "$txbf" -gt 0 ]; then
			iwpriv $ifname set_mib txbf_mu=$txbf_mu
		else
			iwpriv $ifname set_mib txbf_mu=0
		fi
		iwpriv $ifname set_mib tdls_prohibited=$tdls_prohibited
		iwpriv $ifname set_mib tdls_cs_prohibited=$tdls_cs_prohibited
		iwpriv $ifname set_mib rm_activated=${ieee80211k}
		iwpriv $ifname set_mib rm_link_measure=${ieee80211k}
		iwpriv $ifname set_mib rm_beacon_passive=${ieee80211k}
		iwpriv $ifname set_mib rm_beacon_active=${ieee80211k}
		iwpriv $ifname set_mib rm_beacon_table=${ieee80211k}
		iwpriv $ifname set_mib rm_neighbor_report=${ieee80211k}
		iwpriv $ifname set_mib rm_ap_channel_report=${ieee80211k}
	fi

	json_select ..

	[ -n "$macaddr" -o "$mode" = "rtkmesh" ] || {
		macaddr="$(mac80211_generate_mac $phy)"
		macidx="$(($macidx + 1))"
	}

	json_add_object data
	json_add_string ifname "$ifname"
	json_close_object
	json_select config

	# It is far easier to delete and create the desired interface
	case "$mode" in
		adhoc)
			iw phy "$phy" interface add "$ifname" type adhoc
		;;
		ap)
			# Hostapd will handle recreating the interface and
			# subsequent virtual APs belonging to the same PHY
			if [ -n "$hostapd_ctrl" ]; then
				type=bss
			else
				type=interface
			fi

			mac80211_hostapd_setup_bss "$phy" "$ifname" "$macaddr" "$type" || return

			[ -n "$hostapd_ctrl" ] || {
				iw phy "$phy" interface add "$ifname" type __ap
				hostapd_ctrl="${hostapd_ctrl:-/var/run/hostapd/$ifname}"
			}

			[ "$ieee80211k" = "1" ] && {
				ifconfig $ifname down up
				dot11k_deamon -i $ifname
			}
		;;
		mesh)
			iw phy "$phy" interface add "$ifname" type mp
		;;
		monitor)
			iw phy "$phy" interface add "$ifname" type monitor
		;;
		sta)
			local wdsflag=
			staidx="$(($staidx + 1))"
			[ "$wds" -gt 0 ] && wdsflag="4addr on"
			iw phy "$phy" interface add "$ifname" type managed $wdsflag
			[ "$powersave" -gt 0 ] && powersave="on" || powersave="off"
			iw "$ifname" set power_save "$powersave"
		;;
	esac

	case "$mode" in
		monitor|mesh)
			[ "$auto_channel" -gt 0 ] || iw dev "$ifname" set channel "$channel" $htmode
		;;
	esac

	if [ "$mode" != "ap" ]; then
		# ALL ap functionality will be passed to hostapd
		# All interfaces must have unique mac addresses
		# which can either be explicitly set in the device
		# section, or automatically generated
		ip link set dev "$ifname" address "$macaddr"
	fi

	json_select ..
}

mac80211_setup_supplicant() {
	wpa_supplicant_prepare_interface "$ifname" nl80211 || return 1
	wpa_supplicant_add_network "$ifname"
	wpa_supplicant_run "$ifname" ${hostapd_ctrl:+-H $hostapd_ctrl}
}

mac80211_setup_adhoc_htmode() {
	local freq_center
	local chan_width

	case "$htmode" in
		VHT20|HT20) chan_width=20;;
		HT40|VHT40)
			case "$channel" in
				1|2|3|4|5|6|7) freq_center=$(($freq + 10));;
				8|9|10|11) freq_center=$(($freq - 10));;
				36|44|52|60|100|108|116|124|132|140|149|157|165|173) freq_center=$(($freq+10));;
				40|48|56|64|104|112|120|128|136|144|153|161|169|177) freq_center=$(($freq-10));;
					esac
			chan_width=40
				;;
		VHT80)
			case "$channel" in
				36|40|44|48) freq_center=5210;;
				52|56|60|64) freq_center=5290;;
				100|104|108|112) freq_center=5530;;
				116|120|124|138) freq_center=5610;;
				132|136|140|144) freq_center=5690;;
				149|153|157|161) freq_center=5775;;
				165|169|173|177) freq_center=5855;;
					esac
			chan_width=80
	esac

	iw dev "$ifname" set freq $freq $chan_width $freq_center
}

mac80211_setup_adhoc() {
	json_get_vars bssid ssid key mcast_rate

	keyspec=
	[ "$auth_type" = "wep" ] && {
		set_default key 1
		case "$key" in
			[1234])
				local idx
				for idx in 1 2 3 4; do
					json_get_var ikey "key$idx"

					[ -n "$ikey" ] && {
						ikey="$(($idx - 1)):$(prepare_key_wep "$ikey")"
						[ $idx -eq $key ] && ikey="d:$ikey"
						append keyspec "$ikey"
					}
				done
			;;
			*)
				append keyspec "d:0:$(prepare_key_wep "$key")"
			;;
		esac
	}

	brstr=
	for br in $basic_rate_list; do
		wpa_supplicant_add_rate brstr "$br"
	done

	mcval=
	[ -n "$mcast_rate" ] && wpa_supplicant_add_rate mcval "$mcast_rate"

	iw dev "$ifname" ibss join "$ssid" $freq $bssid \
		${beacon_int:+beacon-interval $beacon_int} \
		${brstr:+basic-rates $brstr} \
		${mcval:+mcast-rate $mcval} \
		${keyspec:+keys $keyspec}
}

mac80211_setup_vif() {
	local name="$1"
	local failed

	json_select data
	json_get_vars ifname
	json_select ..

	json_select config
	json_get_vars mode smart_roaming mesh_portal
	json_get_var vif_txpower txpower rtk_wds

	[ "$mode" = "rtkmesh" ] || {
		ip link set dev "$ifname" up || {
			wireless_setup_vif_failed IFUP_ERROR
			json_select ..
			return
		}
	}

	if [ "$rtk_wds" -gt 0 ]; then
		case "$ifname" in
			wlan0)
				enable_rtk_wds_priv "$ifname"
			;;
			wlan1)
				enable_rtk_wds_priv "$ifname"
			;;
		esac
	fi


	[ "$mode" = "rtkmesh" ] || {
		set_default vif_txpower "$txpower"
		[ -z "$txpower" ] || iw dev "$ifname" set txpower fixed "${txpower%%.*}00"
	}

	case "$mode" in
		mesh)
			# authsae or wpa_supplicant
			json_get_vars key
			if [ -n "$key" ]; then
				if [ -e "/lib/wifi/authsae.sh" ]; then
					. /lib/wifi/authsae.sh
					authsae_start_interface || failed=1
				else
					wireless_vif_parse_encryption
					mac80211_setup_supplicant || failed=1
				fi
			else
				json_get_vars mesh_id mcast_rate

				mcval=
				[ -n "$mcast_rate" ] && wpa_supplicant_add_rate mcval "$mcast_rate"

				iw dev "$ifname" mesh join "$mesh_id" ${mcval:+mcast-rate $mcval}
			fi

			for var in $MP_CONFIG_INT $MP_CONFIG_BOOL $MP_CONFIG_STRING; do
				json_get_var mp_val "$var"
				[ -n "$mp_val" ] && iw dev "$ifname" set mesh_param "$var" "$mp_val"
			done
		;;
		rtkmesh)
			ifconfig wlan-msh down up
			pathsel -P -d -i wlan-msh
			[ "$smart_roaming" -gt 0 ] && [ ! -d /etc/capwap ] && {
				mkdir /etc/capwap
				[ "$mesh_portal" -gt 0 ] && {
					echo "<MESH_PORTAL> 1" >> /etc/config/settings.wtp.txt
					AC /etc/capwap
					AACWTP /etc/capwap
				}
				WTP /etc/capwap
			}
		;;
		adhoc)
			wireless_vif_parse_encryption
			mac80211_setup_adhoc_htmode
			if [ "$wpa" -gt 0 -o "$auto_channel" -gt 0 ]; then
				mac80211_setup_supplicant || failed=1
			else
				mac80211_setup_adhoc
			fi
		;;
		sta)
			mac80211_setup_supplicant || failed=1
		;;
	esac

	json_select ..
	[ -n "$failed" ] || [ "$mode" = "rtkmesh" ] || wireless_add_vif "$name" "$ifname"
}

get_freq() {
	local phy="$1"
	local chan="$2"
	iw "$phy" info | grep -E -m1 "(\* ${chan:-....} MHz${chan:+|\\[$chan\\]})" | grep MHz | awk '{print $2}'
}

mac80211_interface_cleanup() {
	local phy="$1"

	is_rtl_drv_phy $phy && wlist=$(check_rtl_phy_intf_name $phy)

	[ -n "$wlist" ] || wlist=$(list_phy_interfaces "$phy")

	#disable mesh interface
	[ -n "$(ifconfig | grep wlan-msh)" ] && {
		ifconfig wlan-msh down
	}

	#delete smart roaming daemon
	[ -d /etc/capwap ] && {
		[ -f /var/log/ac.log.txt ] && rm -f /var/log/ac.log.txt
		[ -f /var/log/wtp.log.txt ] && rm -f /var/log/wtp.log.txt
		[ -f /var/log/smart_roaming.ac.log.txt ] && rm -f /var/log/smart_roaming.ac.log.txt
		[ -f /var/log/smart_roaming.wtp.log.txt ] && rm -f /var/log/smart_roaming.wtp.log.txt
		[ -f /var/log/aac.wtp.log.txt ] && rm -f /var/log/aac.wtp.log.txt
		[ -f /var/log/ping.txt ] && rm -f /var/log/ping.txt

		[ -f "/etc/capwap/AC.pid" ] && {
			killall AC
			rm /etc/capwap/AC.pid
		}
		[ -f "/etc/capwap/AACWTP.pid" ] && {
			killall AACWTP
			rm /etc/capwap/AACWTP.pid
		}
		[ -f "/etc/capwap/WTP.pid" ] && {
			killall WTP
			rm /etc/capwap/WTP.pid
		}

		sed -i '/MESH_PORTAL/d' /etc/config/settings.wtp.txt
		rm -rf /etc/capwap
	}

	#delete mesh pathsel daemon
	[ -f "/var/run/pathsel-wlan-msh.pid" ] && {
		kill $(cat /var/run/pathsel-wlan-msh.pid)
		rm /var/run/pathsel-wlan-msh.pid
	}

	for wdev in $wlist; do
		ip link set dev "$wdev" down 2>/dev/null
		iw dev "$wdev" del
		[ -f "/var/run/dot11k-${wdev}.pid" ] && kill $(cat /var/run/dot11k-${wdev}.pid)

		[ "$wdev" == "wlan0" -o "$wdev" == "wlan1" ] && [ -f "/proc/$wdev/mesh_root_info" ] && {
			mesh_enable=$(echo $(iwpriv $wdev get_mib mesh_enable) | awk -F':' '{print $2}')
			[ "$mesh_enable" == "1" ] && {
				iwpriv $wdev set_mib mesh_enable=0
				iwpriv $wdev set_mib mesh_ap_enable=0
				iwpriv $wdev set_mib mesh_portal_enable=0
				iwpriv $wdev set_mib mesh_id=
				iwpriv $wdev set_mib mesh_privacy=0
				iwpriv $wdev set_mib mesh_passphrase=
			}
		}
	done
}

drv_mac80211_cleanup() {
	hostapd_common_cleanup
}

drv_mac80211_setup() {
	json_select config
	json_get_vars \
		phy macaddr path \
		country chanbw distance \
		txpower antenna_gain \
		rxantenna txantenna \
		frag rts beacon_int htmode
	json_get_values basic_rate_list basic_rate
	json_select ..

	find_phy || {
		echo "Could not find PHY for device '$1'"
		wireless_set_retry 0
		return 1
	}

	wireless_set_data phy="$phy"
	mac80211_interface_cleanup "$phy"

	# convert channel to frequency
	[ "$auto_channel" -gt 0 ] || freq="$(get_freq "$phy" "$channel")"

	[ -n "$country" ] && {
		iw reg get | grep -q "^country $country:" || {
			iw reg set "$country"
			sleep 1
		}
	}

	hostapd_conf_file="/var/run/hostapd-$phy.conf"

	no_ap=1
	macidx=0
	staidx=0

	[ -n "$chanbw" ] && {
		for file in /sys/kernel/debug/ieee80211/$phy/ath9k/chanbw /sys/kernel/debug/ieee80211/$phy/ath5k/bwmode; do
			[ -f "$file" ] && echo "$chanbw" > "$file"
		done
	}

	set_default rxantenna all
	set_default txantenna all
	set_default distance 0
	set_default antenna_gain 0

	iw phy "$phy" set antenna $txantenna $rxantenna >/dev/null 2>&1
	iw phy "$phy" set antenna_gain $antenna_gain
	iw phy "$phy" set distance "$distance"

	[ -n "$frag" ] && iw phy "$phy" set frag "${frag%%.*}"
	[ -n "$rts" ] && iw phy "$phy" set rts "${rts%%.*}"

	has_ap=
	hostapd_ctrl=
	for_each_interface "ap" mac80211_check_ap

	for_each_interface "rtkmesh" mac80211_prepare_rtkmesh

	rm -f "$hostapd_conf_file"
	[ -n "$has_ap" ] && mac80211_hostapd_setup_base "$phy"

	for_each_interface "ap" mac80211_prepare_vif
	for_each_interface "sta adhoc mesh monitor rtkmesh" mac80211_prepare_vif

	[ -n "$hostapd_ctrl" ] && {
		/usr/sbin/hostapd -P /var/run/wifi-$phy.pid -B "$hostapd_conf_file"
		ret="$?"
		wireless_add_process "$(cat /var/run/wifi-$phy.pid)" "/usr/sbin/hostapd" 1
		[ "$ret" != 0 ] && {
			wireless_setup_failed HOSTAPD_START_FAILED
			return
		}
	}

	for_each_interface "ap sta adhoc mesh monitor rtkmesh" mac80211_setup_vif

	wireless_set_up
}

list_phy_interfaces() {
	local phy="$1"
	if [ -d "/sys/class/ieee80211/${phy}/device/net" ]; then
		ls "/sys/class/ieee80211/${phy}/device/net" 2>/dev/null;
	else
		ls "/sys/class/ieee80211/${phy}/device" 2>/dev/null | grep net: | sed -e 's,net:,,g'
	fi
}

drv_mac80211_teardown() {
	wireless_process_kill_all

	json_select data
	json_get_vars phy
	json_select ..

	mac80211_interface_cleanup "$phy"
}

add_driver mac80211
