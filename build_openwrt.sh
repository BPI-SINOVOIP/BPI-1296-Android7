#!/bin/bash

SCRIPTDIR=$PWD
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

build_openwrt()
{
    pushd $OPENWRTDIR > /dev/null

	if [ ! -e .config ]; then
		cat ${IMAGE_TARGET_BOARD}-${OPENWRT_CONFIG}_defconfig > .config
		make defconfig
	fi

	if [ "${BUILDTYPE_ANDROID}" = "kylin32_tv" ]; then
		sed -i 's/kylin32"/kylin32_tv"/' .config
	fi

	make -j8 V=s
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
		make clean V=99 -j8;rm .config .config.old
	popd  > /dev/null

	pushd $OPENWRTKERNELDIR > /dev/null
		make distclean;rm .config*
	popd > /dev/null

	return $ERR

}

if [ "$1" = "" ]; then
    echo "$0 commands are:"
    echo "    build       "
    echo "    clean       "
else
    while [ "$1" != "" ] 
    do
	case "$1" in
	 clean)
		build_cmd clean_openwrt
        	;;
	 build)
		build_cmd build_openwrt
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
