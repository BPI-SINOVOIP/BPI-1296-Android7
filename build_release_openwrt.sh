#!/bin/bash

SCRIPTDIR=$PWD
OPENWRTDIR=$SCRIPTDIR/Openwrt
ANDROIDDIR=$SCRIPTDIR/android
KERNELDIR=$SCRIPTDIR/linux-4.1.7
MALIDIR=$SCRIPTDIR/mali
PHOENIXDIR=$SCRIPTDIR/phoenix
UBOOTDIR=$SCRIPTDIR/bootcode
TOOLCHAINDIR=$PHOENIXDIR/toolchain
PRODUCT_DEVICE_PATH=android/out/target/product/kylin
source $SCRIPTDIR/build_prepare.sh
config_get CUSTOMER
config_get GIT_SERVER_URL
config_get SDK_BRANCH
config_get USE_RTK_REPO
config_get BUILDTYPE_ANDROID
config_get IMAGE_DRAM_SIZE
config_get OPENWRT_CONFIG
config_get USE_RTK1295_EMMC_SWAP
config_get IMAGE_TARGET_BOARD 

ERR=0
VERBOSE=
NCPU=`grep processor /proc/cpuinfo | wc -l`
MULTI=`expr $NCPU + 2`

init_openwrt()
{
	[ ! -d "$OPENWRTDIR" ] && mkdir $OPENWRTDIR
	pushd $OPENWRTDIR > /dev/null
	menifest=openwrt.xml

	if [ "$USE_RTK_REPO" == true ]; then
		repo init -u ssh://$GIT_SERVER_URL:29418/$CUSTOMER/manifests -b $SDK_BRANCH -m $menifest --repo-url=ssh://$GIT_SERVER_URL:29418/git-repo
	else
		repo init -u ssh://$GIT_SERVER_URL:29418/$CUSTOMER/manifests -b $SDK_BRANCH -m $menifest
	fi
	popd
	return 0
}

sync_openwrt()
{
    ret=1
    [ ! -d "$OPENWRTDIR" ] && return 0
    pushd $OPENWRTDIR > /dev/null
        repo sync --force-sync
        ret=$?
        [ "$ret" = "0" ] && > .repo_ready
    popd > /dev/null
    return $ret
}

checkout_openwrt()
{
    [ -e "$OPENWRTDIR/.repo_ready" ] && return 0
    init_openwrt
    sync_openwrt
    return $?
}

build_openwrt()
{
    pushd $OPENWRTDIR > /dev/null
        yes "" | make oldconfig && ./netconfig.sh ott
        yes "" | make oldconfig && make -j8 V=s

	ERR=$?
    popd
    return $ERR
}

function build_cmd()
{
    $@
    ERR=$?
    printf "$* "
    if [ "$ERR" != "0" ]; then
        echo -e "\033[47;31m [ERROR] $ERR \033[0m"
        exit 1
    else
        echo "[OK]"
    fi
}
clean_openwrt()
{
	pushd $OPENWRTDIR > /dev/null
		make clean V=99 -j8;rm .config.old
	popd  > /dev/null

	pushd $KERNELDIR > /dev/null
		make distclean;rm .config*
	popd > /dev/null

	return $ERR

}

sync_log()
{
DATE=`cat $SCRIPTDIR/.build_config |grep SYNC_DATE: | awk '{print $2}'`
LOG_FILE=$SCRIPTDIR/change_log_openwrt_`date +%Y-%m-%d`.txt
pushd $OPENWRTDIR > /dev/null
repo list |awk 'BEGIN{system("rm /tmp/change_log.txt "); cnt=1}{printf "\n==== "cnt" === "$1" "$2" "$3" ===========\n" >> "/tmp/change_log.txt" ; cnt++; system("pushd "$1";  git log --stat --since='$DATE' >> /tmp/change_log.txt ; popd > /dev/null")} END{system("mv /tmp/change_log.txt '$LOG_FILE'")}'
popd > /dev/null
}

if [ "$1" = "" ]; then
    echo "$0 commands are:"
    echo "    build       "
else
    while [ "$1" != "" ] 
    do
	case "$1" in
         checkout)
				build_cmd checkout_openwrt
				;;
		   clean)
				build_cmd clean_openwrt
           		;;
		   build)
				build_cmd build_openwrt
				;;
			sync)
				build_cmd clean_openwrt
				build_cmd sync_openwrt
				sync_log
				;;
			*)
		echo -e "$0 \033[47;31mUnknown CMD: $1\033[0m"
		exit 1
		;;
	esac
	shift 1
    done
fi


exit $ERR
