#!/bin/bash

set -x

TARGETS="router ott"
if [ "$1" = "" ]; then
    echo "Please select config target:"
    select TARGET in $TARGETS;
    do
        echo "$TARGET selected!"
        break
    done
else
    TARGET=$1
fi

# OPTION ON/OFF
config(){
OPTION="CONFIG_$1"

    case "$2" in
        on|ON)
            sed -i -r "s/^#.?$OPTION is not set/$OPTION=y/g" ./.config
        ;;
        off|OFF)
            sed -i "s/^$OPTION=y/# $OPTION is not set/g" ./.config
        ;;
        *)
            echo "Unsupported state:$2 for config option:$2 is selected!"
            exit 1
        ;;
    esac
}

ROUTER_MOD="rtkwifiu mac80211 cfg80211 lib80211"
ROUTER_MOD+=" rtd1295hwnat"
OTT_MOD="ottrtl8188eu ottrtl8189es ottrtl8189fs ottrtl8192cu ottrtl8192eu ottrtl8723bs \
         ottrtl8723bu ottrtl8723ds ottrtl8811au ottrtl8812au ottrtl8812bu ottrtl8814au \
         ottrtl8821as ottrtl8821au ottrtl8821cs ottrtl8822be ottrtl8822bs ottrtl8822bu \
         ottrtl8723du ottrtl8188fu ottrtl8821ce ottrtl8821cu \
         ottbtusb rtl8169soc"
OTT_MOD+=" ottrtl8192ee ottrtl8812ae ottrtl8822be"
NETWORK_PKG="netifd firewall luci-app-firewall dnsmasq"

case "$TARGET" in
    router)
        # Remove generated ipks
        rm -r ./bin/*/packages

        # Update package configs
        for mod in $ROUTER_MOD; do
            config PACKAGE_kmod-$mod ON
        done
        for mod in $OTT_MOD; do
            config PACKAGE_kmod-$mod OFF
        done

        for pkg in $NETWORK_PKG; do
            config PACKAGE_$pkg ON
        done

        make oldconfig
    ;;
    ott)
        # Remove generated ipks
        rm -r ./bin/*/packages

        # Update package configs
        for mod in $OTT_MOD; do
            config PACKAGE_kmod-$mod ON
        done
        for mod in $ROUTER_MOD; do
            config PACKAGE_kmod-$mod OFF
        done

        for pkg in $NETWORK_PKG; do
            config PACKAGE_$pkg OFF
        done

        make oldconfig
    ;;
    *)
        echo "Unsupported target:$TARGET is selected!"
        exit 1
    ;;
esac
