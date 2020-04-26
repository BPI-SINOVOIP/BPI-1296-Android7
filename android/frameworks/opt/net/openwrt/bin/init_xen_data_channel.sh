#!/sbin/busybox sh
busybox ifconfig eth1 192.168.253.10 netmask 255.255.255.0 up
busybox arp -s 192.168.253.9 FE:FF:FF:FF:FF:FF
start xen_tcp_server