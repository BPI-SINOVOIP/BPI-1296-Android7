#!/bin/bash
 
SCRIPTDIR=$PWD
source $SCRIPTDIR/build_prepare.sh
KERNELDIR=$SCRIPTDIR/linux-kernel
PHOENIXDIR=$SCRIPTDIR/phoenix
TOOLCHAINDIR=$PHOENIXDIR/toolchain
OPENWRTKERNELDIR=$OPENWRTDIR/build_dir/target-aarch64_cortex-a53+neon_glibc-2.19/linux-rtd1295_mnas_emmc/linux-4.1.7/
OPENWRTIMAGEDIR=$OPENWRTDIR/target/linux/rtd1295/image/image_file-r950546/
BACKUPDIR=$ANDROIDDIR/device/realtek/kylin/backup
RTKIMAGESDIR=$ANDROIDDIR/device/realtek/kylin/rtkimages
BoardConfigCommon=$ANDROIDDIR/device/realtek/kylin/common/BoardConfigCommon.mk
DeviceMK=$ANDROIDDIR/device/realtek/kylin/device.mk

ERR=0
SecureFlag=0
LogoFlag=0

config_get CUSTOMER
config_get GIT_SERVER_URL
config_get SDK_BRANCH
config_get USE_RTK_REPO
config_get BUILDTYPE_ANDROID
config_get UBOOT_BRANCH

[ ! -d "$BACKUPDIR" ] && mkdir -p $BACKUPDIR


function make_usage()
{
	PROGNAME=`basename $0`
	echo
	echo "The following items are optional:"
	echo "otp		: (M) make secure ota"
	echo "logo		: (M) include boot logo"
	echo "for example:"
	echo "./make_otapackage.sh otp logo"
	echo
}

function getOption()
{
	PROGNAME=`basename $0`
	for arg in $@
	do
		case $arg in
			"otp"*)
				SecureFlag=1
				;;
			"logo"*)
				LogoFlag=1
				;;
			"-h")
				make_usage
				exit
				;;
		esac
	done
	
	
	[ $SecureFlag == 1 ] && echo "[Make OTA package include secure]"
	[ $LogoFlag == 1 ] && echo "[Make OTA package include bootlogo]"
	
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

prepart_ota_image(){

	NASIMGAE=$OPENWRTDIR/bin/rtd1295-glibc/install.img
	OTATMPDIR=$OPENWRTDIR/bin/rtd1295-glibc/tmp
	
	if [ "$UBOOT_BRANCH" == LK ]; then
		#OTABOOTCODE=$BOOTCODEDIR/bootloader_lk.tar
		OTABOOTCODE=$OTATMPDIR/omv/bootloader_lk.tar
	else
		#OTABOOTCODE=$BOOTCODEDIR/bootloader.tar
		OTABOOTCODE=$OTATMPDIR/omv/bootloader.tar
	fi
	
	#Modify BoardConfigCommon.mk
	cp $BoardConfigCommon $BACKUPDIR/
	cp $DeviceMK $BACKUPDIR/
	sed -i 's/TARGET_NO_RECOVERY := true/TARGET_NO_RECOVERY := false/g' $BoardConfigCommon
	sed -i 's/TARGET_NO_KERNEL := true/TARGET_NO_KERNEL := false/g' $BoardConfigCommon
	sed -i 's/NAS_ENABLE := NO/NAS_ENABLE := YES/g' $DeviceMK
	if [ $SecureFlag == 1 ]; then
		sed -i 's/RECOVERY_SECUREBOOT := 0/RECOVERY_SECUREBOOT := 1/g' $DeviceMK
	fi
	
	if [ -f $OTABOOTCODE ]; then
		sed -i 's/TARGET_NO_BOOTLOADER := true/TARGET_NO_BOOTLOADER := false/g' $BoardConfigCommon
	fi

	echo "TARGET_PREFER_32_BIT :=" >> $BoardConfigCommon

	#copy and backup release tools
	#cp -rf  $ANDROIDDIR/device/realtek/kylin/releasetools.emmc.nas/* $ANDROIDDIR/device/realtek/kylin/releasetools/
	#cp $ANDROIDDIR/device/realtek/kylin/recovery.fstab.emmc $ANDROIDDIR/device/realtek/kylin/recovery.fstab
	cp $ANDROIDDIR/build/core/Makefile.emmc $ANDROIDDIR/build/core/Makefile

	#Copy RTK FW image to /device/realtek/kylin/rtkimages
	OTAKernel=$OTATMPDIR/omv/emmc.uImage
	OTAAudio=$OTATMPDIR/omv/bluecore.audio
	OTAANDROIDDT=$OTATMPDIR/omv/android.emmc.dtb
	OTARESUCEDT=$OTATMPDIR/omv/rescue.emmc.dtb
	OTARESUCEROOTFS=$OTATMPDIR/omv/rescue.root.emmc.cpio.gz_pad.img
	OTALOGOFILE=$OTATMPDIR/omv/bootfile.image
	OTAFACTORYFILE=$OTATMPDIR/omv/factory.tar
	
	
	if [ $SecureFlag == 1 ]; then
	OTAKernel=$OTATMPDIR/omv/emmc.uImage.aes
	OTAAudio=$OTATMPDIR/omv/bluecore.audio.aes
	OTARESUCEROOTFS=$OTATMPDIR/omv/rescue.root.emmc.cpio.gz_pad.img.aes
	fi
	
	OTASQUASHFS=$OTATMPDIR/omv/squashfs1.img
	#bootimage(video) :  bootfile.image

	[ -f "$NASIMGAE" ] && mkdir -p $OTATMPDIR
	pushd $OTATMPDIR > /dev/null
	tar xvf $NASIMGAE
	popd > /dev/null

	#Delete temporary file
	rm -rf $RTKIMAGESDIR/emmc.uImage*
	rm -rf $RTKIMAGESDIR/bluecore.audio*
	rm -rf $RTKIMAGESDIR/rescue.root.emmc.cpio.gz_pad.img*
	rm -rf $RTKIMAGESDIR/bootloader*.tar
	rm -rf $RTKIMAGESDIR/bootfile.image
	rm -rf $RTKIMAGESDIR/factory.tar


	build_cmd /bin/cp -f $OTAKernel $RTKIMAGESDIR/
	build_cmd /bin/cp -f $OTAAudio $RTKIMAGESDIR/
	build_cmd /bin/cp -f $OTAANDROIDDT $RTKIMAGESDIR/
	build_cmd /bin/cp -f $OTARESUCEDT $RTKIMAGESDIR/
	build_cmd /bin/cp -f $OTARESUCEROOTFS $RTKIMAGESDIR/
	build_cmd /bin/cp -f $OTASQUASHFS $RTKIMAGESDIR/../nasroot/

	
	[ -f "$OTABOOTCODE" ] && /bin/cp -f $OTABOOTCODE $RTKIMAGESDIR/
	
	if [ $LogoFlag == 1 ]; then
		if [ -f "$OTALOGOFILE" ] && [ -f "$OTAFACTORYFILE" ]; then 
			build_cmd /bin/cp -f $OTALOGOFILE $RTKIMAGESDIR/
			build_cmd /bin/cp -f $OTAFACTORYFILE $RTKIMAGESDIR/
		else
			[ !-f "$OTALOGOFILE" ] && echo -e "\033[47;31m Can't find logo file (bootfile.image) !!! \033[0m"
			[ !-f "$OTAFACTORYFILE" ] && echo -e "\033[47;31m Can't find factory file	(factory.tar)  !!! \033[0m"
		fi
	fi

}

make_ota_package(){
	pushd $ANDROIDDIR > /dev/null
		source ./env.sh
		lunch rtk_$BUILDTYPE_ANDROID-eng
		make otapackage
	popd > /dev/null

	if [ $SecureFlag == 1 ]; then
		cp -f $ANDROIDDIR/out/target/product/kylin32/rtk_kylin32-ota-eng."$USER".zip $SCRIPTDIR/rtk_kylin32-ota-eng."$USER"_secure.zip 
	else
		cp -f $ANDROIDDIR/out/target/product/kylin32/rtk_kylin32-ota-eng."$USER".zip $SCRIPTDIR
	fi

}

revert_file(){
	rm -rf $OTATMPDIR
	cp -f $BACKUPDIR/BoardConfigCommon.mk $BoardConfigCommon
	cp -f $BACKUPDIR/device.mk $DeviceMK
	rm -rf $BACKUPDIR
}

getOption $@
prepart_ota_image
make_ota_package
revert_file

