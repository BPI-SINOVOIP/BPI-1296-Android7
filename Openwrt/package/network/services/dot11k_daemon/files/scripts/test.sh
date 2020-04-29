#!/bin/sh

INTF="wlan0"
MAC_ADDR=48437c5189ab


do_beacon_measure () {
        ./test_11k -c beacon -i wlan0 -o 8 -n 0 -a $MAC_ADDR
}


do_neighbor_measure () {
         ./test_11k -c neighbor -i $INTF
}

case "$1" in
  ap)
        do_beacon_measure
        ;;
  client)
        do_neighbor_measure
        ;;               
  *)
        echo "Usage: $0 ap|client" >&2
        echo "change interface and sta mac" >&2
        exit 3
        ;;
esac

