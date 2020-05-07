#!/bin/bash
SCRIPTDIR=$PWD
source $SCRIPTDIR/build_prepare.sh
ANDROIDDIR=$SCRIPTDIR/android
KERNELDIR=$SCRIPTDIR/linux-kernel
MALIDIR=$SCRIPTDIR/mali
PHOENIXDIR=$SCRIPTDIR/phoenix
UBOOTDIR=$SCRIPTDIR/bootcode
ERR=0
VERBOSE=
NCPU=`grep processor /proc/cpuinfo | wc -l`
MULTI=`expr $NCPU + 2`


# set umask here to prevent incorrect file permission
umask 0022

TOOLCHAIN=$PWD/toolchain
config_get CUSTOMER
config_get GIT_SERVER_URL
config_get SDK_BRANCH
config_get USE_RTK_REPO
config_get BUILDTYPE_ANDROID
config_get USE_RTK1295_EMMC_SWAP

#source java8.sh
export JAVA_HOME=$TOOLCHAIN/OpenJDK-1.8.0.112-x86_64-bin
export JRE_HOME=${JAVA_HOME}/jre
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib
export PATH=${JAVA_HOME}/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games
export PATH=~/bin:$PATH
echo $PATH
echo  "Java 8"

build_android()
{
    pushd $ANDROIDDIR > /dev/null
        source ./env.sh
        lunch rtk_$BUILDTYPE_ANDROID-userdebug
        make -j $MULTI $VERBOSE
        ERR=$?
    popd > /dev/null
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


clean_firmware()
{
    pushd $ANDROIDDIR
        make clean
        ERR=$?
    popd
}

case_for_USE_EMMC_SWAP()
{
if [ "$USE_RTK1295_EMMC_SWAP" = "true" ]; then
	pushd $ANDROIDDIR/out/target/product/$BUILDTYPE_ANDROID/ > /dev/null
        # -----------------------------------------------------------#
        # set vm.min_free_kbytes to 32768 if IMAGE_ADJ_VM is true    #
        # -----------------------------------------------------------#
        sed -i '/write \/proc\/sys\/vm\/min_free_kbytes 32768/d' root/init.rc
        sed -i 's/on late-init/on late-init\n    write \/proc\/sys\/vm\/min_free_kbytes 32768/' root/init.rc

        # ---------------------------------------------------------- #
        # set vm.extra_free_kbytes to 20480 if IMAGE_ADJ_VM is true  #
        # -----------------------------------------------------------#
        sed -i 's/#on property:sys.sysctl.extra_free_kbytes=*/on property:sys.sysctl.extra_free_kbytes=*/' root/init.rc
        sed -i 's/write \/proc\/sys\/vm\/extra_free_kbytes 20480/write \/proc\/sys\/vm\/extra_free_kbytes ${sys.sysctl.extra_free_kbytes}/' root/init.rc
        sed -i 's/write \/proc\/sys\/vm\/extra_free_kbytes ${sys.sysctl.extra_free_kbytes}/write \/proc\/sys\/vm\/extra_free_kbytes 20480/' root/init.rc
        # ---------------------------------------------------------- #
        # enable eMMC swap if IMAGE_LAYOUT_USE_EMMC_SWAP is true     #
        # -----------------------------------------------------------#
        sed -i 's/#\/dev\/block\/mmcblk0p11/\/dev\/block\/mmcblk0p11/' root/fstab.kylin
        sed -i 's/#start mk_emmc_swap/start mk_emmc_swap/' root/init.fschk.rc
        # -----------------------------------------------------------#
	popd > /dev/null
else
	echo "CONFIG_USE_RTK1295_EMMC_SWAP=$USE_RTK1295_EMMC_SWAP, do nothing"
fi
}

link_lib()
{
echo android link_lib ++++++++++++++++++++++++++++++++++++
PRODUCT_DEVICE_PATH=$ANDROIDDIR/out/target/product/$BUILDTYPE_ANDROID

[ ! -d "$PRODUCT_DEVICE_PATH/vendor/bin" ] && mkdir -p $PRODUCT_DEVICE_PATH/vendor/bin

cp $ANDROIDDIR/ext_bin/* $PRODUCT_DEVICE_PATH/vendor/bin/

pushd $PRODUCT_DEVICE_PATH/system/lib/ > /dev/null
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.3gpp.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.avc.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.avs.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.divx3.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.flv.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.hevc.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.mjpg.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.mp43.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.mpeg2.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.mpeg4.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.raw.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.rv30.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.rv40.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.rv.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.vc1.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.vp8.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.wmv3.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.wmv.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.wvc1.so
ln -sf libOMX.realtek.video.enc.so libOMX.realtek.video.enc.avc.so
ln -sf libOMX.realtek.video.enc.so libOMX.realtek.video.enc.mpeg4.so
ln -sf libOMX.realtek.audio.dec.so libOMX.realtek.audio.dec.raw.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.hevc.secure.so
ln -sf libOMX.realtek.video.dec.vp9.so libOMX.realtek.video.dec.vp9.secure.so
ln -sf libOMX.realtek.audio.dec.so libOMX.realtek.audio.dec.secure.so
ln -sf libOMX.realtek.video.dec.so libOMX.realtek.video.dec.secure.so

popd > /dev/null
exit $ERR
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
                build_cmd clean_firmware
                ;;
            build)
                build_cmd build_android
		build_cmd link_lib
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
