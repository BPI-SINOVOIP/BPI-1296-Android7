#!/bin/sh

do_iperf() {
    /usr/bin/iperf -s $IPERFARGS > /tmp/iperf.txt &
    echo -e "iperf server is started, please use a client to connect to server using:\n"
    echo -e "$IPERFCMD\n"
    echo -n "Wait for connection "
    First=0
    while :
    do
        Connected=`/usr/bin/cat /tmp/iperf.txt | /bin/grep connected | /usr/bin/awk '{print $9}'`
        Bandwidth=`/usr/bin/cat /tmp/iperf.txt | /bin/grep Mbits | /usr/bin/awk '{print $7}'`
        if [ ! -z $Bandwidth ]; then
            echo -e "\nGet network performance with ${Bandwidth}Mbps\n"
            /usr/bin/killall iperf
            break;
        elif [ ! -z $Connected ]; then
            [ $First -eq 0 ] && echo -e "" && echo -n "Connected with $Connected and start $IPERFPRO testing "
            First=`expr $First + 1`
            sleep 1
            [ $First -gt 0 ] && echo -n "#"
        else
            sleep 1
            echo -n "."
        fi
    done
}

if [ "$1" = "" ]; then
    echo "$0 commands are:"
    echo "    memtest      "
    echo "    membench     "
    echo "    iotest path  "
    echo "    nettest      "
else
    while [ "$1" != "" ]
    do
        case "$1" in
            memtest)
		sync && echo 3 > /proc/sys/vm/drop_caches	
                FreeMem=`/bin/grep MemFree /proc/meminfo | /usr/bin/awk '{print $2}'`
		TestMem=`expr $FreeMem / 1250`
		sync && echo 3 > /proc/sys/vm/drop_caches	
                /usr/sbin/memtester $TestMem 1
                ;;

            membench)
                /usr/sbin/tinymembench
                ;;

            iotest)
                if [ ! -d $2 ] || [ -z $2 ]; then
                    echo -e "\nPlease make sure testing path $1 exists\n"
                    return 1
                else
                    cd $2
                    TotalMem=`/bin/grep MemTotal /proc/meminfo | /usr/bin/awk '{print $2}'`
                    FileSize=`expr $TotalMem / 1000`
                    FreeSpace=`df -Phm . | tail -1 | awk '{print $4}'`
                    FreeSpace=`expr $FreeSpace / 4`
                    if [ $FileSize -gt $FreeSpace ];then
                        echo -e "Available disk space for path $2 is not enough\Please reserve more than ${FileSize}MB disk space for testing\n"
                    else
                        /sbin/iozone  -+u -s ${FileSize}m -i 0 -i 1 -t 4 -V 165
                    fi
                    cd -
                    shift 1
                fi
                ;;

            nettest)
                #Handling TCP testing first
                IPAddr=`/sbin/ifconfig eth0 | /bin/grep "inet addr:" | /usr/bin/cut -d: -f2 | /usr/bin/awk '{print $1}'`
		IPERFCMD="iperf -c $IPAddr -t 10 -i 1 -w 512k"
		IPERFPRO="TCP"
                do_iperf
                #UDP testing
		IPERFARGS="-u"
		IPERFCMD="iperf -c $IPAddr -T 32 -t 10 -i 1 -u -b 2g"
		IPERFPRO="UDP"
                do_iperf
                #UDP multicast testing
		IPERFARGS="-u -B 224.0.12.95 -O eth0"
		IPERFCMD="iperf -c 224.0.12.95 -T 32 -t 10 -i 1 -u -b 2g"
		IPERFPRO="UDP Multicast"
                do_iperf
                ;;

        esac
        shift 1
    done
fi



