#!/bin/bash

export TOPDIR=`pwd`

#### Varable list, please add fix value under buildscript_conf/$TARGET_PLATFORM ####
CROSS_COMPILE=
DVRBOOT_PRJ=
DVRBOOT_DIR=
INCFILE_DIR=

TARGET_MAIN=
TARGET_PLATFORM=
TARGET_PRJ=
TARGET_CHIP_TYPE=
TARGET_HWSETTING=
TARGET_CUSTOMER_ID=
VMX=
VMX_3RD_PARTY=
VMX_BL=
DRM=
HAS_SECUREBOOT=

DEFAULT_CHIP_ID=
DEFAULT_CUSTOMER_ID=
DEFAULT_CHIP_TYPE=
DEFAULT_PRJ=

BUILD_CUSTOMER_ID_LIST=
BUILD_CHIP_TYPE_LIST=
BUILD_HWSETTING_LIST=

PINNED_CUSTOMER_ID_LIST=
PINNED_CHIP_TYPE_LIST=
PINNED_HWSETTING_LIST=
#### end of variable list ####

show_usage()
{
	echo 'Usage : ./build_rtk_lk.sh <platform> [all]'
	echo '	ex : ./build_rtk_lk.sh rtd1295 ==> build rtd1295 LK dvrboot.exe.bin'
	echo '		IC-REV : project/target/rtd1295.mk ==> CHIP_TYPE'
	echo '		HW_SETTING : tools/flash_writer/1295_force_emmc_nS_nE.inc ==> Board_HWSETTING'
	echo '	ex : ./build_rtk_lk.sh rtd1295 all ==> build all IC-REV / HWSETTING detectable'
}

setup_cross_compile()
{
	echo select cross-compiler $CROSS_COMPILE

	if [ ! -d $TOPDIR/tmp/$CROSS_COMPILE ]; then
		echo cross-compiler not found, installing...
		mkdir -p $TOPDIR/tmp
		tar xfj $TOPDIR/toolchain/$CROSS_COMPILE.tar.bz2 -C $TOPDIR/tmp
	fi

	export PATH=$TOPDIR/tmp/$CROSS_COMPILE/bin:$PATH
}

get_dvrboot_config()
{
	pushd $TOPDIR/build-$TARGET_PLATFORM

	if [ ! -e "dvrboot_config" ]; then
		echo "Can't find dvrboot_config"
		exit 1
	fi

	#cat dvrboot_config | while read line
	while read line
	do
		echo $line | grep "PRJ=" && TARGET_PRJ=$(echo $line | cut -f 2 -d '=')
		echo $line | grep "CHIP_TYPE=" && TARGET_CHIP_TYPE=$(echo $line | cut -f 2 -d '=')
		echo $line | grep "CUSTOMER_ID=" && TARGET_CUSTOMER_ID=$(echo $line | cut -f 2 -d '=')
	done < dvrboot_config

	TARGET_HW_SETTING=`cat $INCFILE_DIR/$TARGET_PRJ.inc | grep Board_HWSETTING | awk '{print $3}'`
	echo $TARGET_PRJ $TARGET_CHIP_TYPE $TARGET_HW_SETTING

	popd
}

get_default_config()
{
	pushd $TOPDIR/project/target

	while read line
	do
		echo $line | grep "TARGET" && TARGET_MAIN=$(echo $line | awk '{print $3}')
		echo $line | grep "CHIP_ID" && DEFAULT_CHIP_ID=$(echo $line | awk '{print $3}')
		echo $line | grep "CUSTOMER_ID" && DEFAULT_CUSTOMER_ID=$(echo $line | awk '{print $3}')
		echo $line | grep "CHIP_TYPE" && DEFAULT_CHIP_TYPE=$(echo $line | awk '{print $3}')
		echo $line | grep "PRJ" && DEFAULT_PRJ=$(echo $line | awk '{print $3}')
		echo $line | grep "VMX" && VMX=$(echo $line | awk '{print $3}')
		echo $line | grep "VMX_3RD_PARTY" && VMX_3RD_PARTY=$(echo $line | awk '{print $3}')
		echo $line | grep "VMX_BL" && VMX_BL=$(echo $line | awk '{print $3}')
	done < $1.mk

	echo @@@@@@ Default Chipset Config @@@@@@
	echo $DEFAULT_CHIP_ID $DEFAULT_CUSTOMER_ID $DEFAULT_CHIP_TYPE $DEFAULT_PRJ

	#FIXME : it'll be better to check $TOPDIR/project/<base.mk> to find out
	#        which mk file is uesed under  $TOPDIR/project/target.
	#		 The sub-platform may have the same tartget.	
	TARGET_PLATFORM=$1
	#TARGET_PLATFORM=`cat $1.mk | grep "TARGET :=" | awk '{print $3}'`

	if [ "$TARGET_PLATFORM" == "" ]; then
		echo "Can't Detect Platform!!!"
		exit 1
	fi

	# use default prj config if no target assigned
	DVRBOOT_PRJ=${DVRBOOT_PRJ:-$DEFAULT_PRJ}

	if [ ! -e $INCFILE_DIR/$DVRBOOT_PRJ.inc ]; then
		echo "Can't Detect PRJ file : $INCFILE_DIR/$DVRBOOT_PRJ.inc"
		exit 1
	fi

	popd
}

source "buildscript_conf/$1.conf"
if [ "$?" != "0" ]; then
	echo "can't load target config"
	exit 1
fi

if [ "$#" -gt 2 ]; then
	echo Error input format!!
	show_usage
	exit 1
fi

get_default_config $1
setup_cross_compile
rm -rf $TOPDIR/DVRBOOT_OUT
mkdir -p $TOPDIR/DVRBOOT_OUT/hwsetting
make spotless

#use git commit as build id
BUILD_TIME=`date | tr ' ' '-'`
BUILD_ID=`git log --pretty=format:'%h' -n 1`
if [ "$?" != "0" ]; then
	BUILD_ID=
else
	BUILD_ID="$BUILD_ID-$BUILD_TIME"
fi

# if you want to burn fw to boot partition 1,add:
# BOOT=yes
# following by make 

case $# in
	"1")
		PINNED_CHIP_TYPE_LIST=$DEFAULT_CHIP_TYPE
		secure_img_setup
		make $1 -j4 PRJ=$DVRBOOT_PRJ BUILDID="$BUILD_ID" 
		get_dvrboot_config
		cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
		if [ "$TARGET_MAIN" == "rtd1395" ]; then
			cp $DVRBOOT_DIR/install_a/hw_setting.bin $TOPDIR/DVRBOOT_OUT/hwsetting/$TARGET_HW_SETTING.bin
			cp $DVRBOOT_DIR/Bind/emmc.bind.bin $TOPDIR/DVRBOOT_OUT/hwsetting/Recovery-$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
		else
			cp $DVRBOOT_DIR/image/hw_setting/$TARGET_HW_SETTING.bin $TOPDIR/DVRBOOT_OUT/hwsetting/
		fi
		;;
	"2")

		if [ "$2" == "drm" ] || [ "$2" == "atv" ] || [ "$2" == "vmx" ]; then
			if [ "$2" == "drm" ]; then
				DRM=1
			elif [ "$2" == "atv" ]; then
				DRM=2
			elif [ "$2" == "vmx" ]; then
				DRM=3
			fi

			PINNED_CHIP_TYPE_LIST=$DEFAULT_CHIP_TYPE
			secure_img_setup
			make $1 -j4 PRJ=$DVRBOOT_PRJ BUILDID="$BUILD_ID"
			get_dvrboot_config

			if [ "$2" == "drm" ]; then
				cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-drm.bin
			elif [ "$2" == "atv" ]; then
				cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-atv.bin
			elif [ "$2" == "vmx" ]; then
				cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-vmx.bin
			fi
			if [ "$TARGET_MAIN" == "rtd1395" ]; then
				cp $DVRBOOT_DIR/install_a/hw_setting.bin $TOPDIR/DVRBOOT_OUT/hwsetting/$TARGET_HW_SETTING.bin
				cp $DVRBOOT_DIR/Bind/emmc.bind.bin $TOPDIR/DVRBOOT_OUT/hwsetting/Recovery-$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
			else
				cp $DVRBOOT_DIR/image/hw_setting/$TARGET_HW_SETTING.bin $TOPDIR/DVRBOOT_OUT/hwsetting/
			fi
			exit 0
		elif [ "$2" == "all" ]; then
			echo "#### Start build all combination ####"
		else
			show_usage
			exit 1
		fi

		#pushd $TOPDIR/tools/flash_writer/image/hw_setting/$DEFAULT_CHIP_ID
		HWSETTING_DIR=$DVRBOOT_DIR/image/hw_setting/$DEFAULT_CHIP_ID

		if [ "$PINNED_CUSTOMER_ID_LIST" == "" ]; then
			BUILD_CUSTOMER_ID_LIST=`ls $HWSETTING_DIR`
		else
			BUILD_CUSTOMER_ID_LIST=$PINNED_CUSTOMER_ID_LIST
		fi

		for customer_id in $BUILD_CUSTOMER_ID_LIST
		do
			if [ "$PINNED_CHIP_TYPE_LIST" == "" ]; then
				BUILD_CHIP_TYPE_LIST=`ls $HWSETTING_DIR/$customer_id`
			else
				BUILD_CHIP_TYPE_LIST=$PINNED_CHIP_TYPE_LIST
			fi

			for chip_type in $BUILD_CHIP_TYPE_LIST
			do
				if [ "$PINNED_HWSETTING_LIST" == "" ]; then
					BUILD_HWSETTING_LIST=`ls $HWSETTING_DIR/$customer_id/$chip_type`
				else
					BUILD_HWSETTING_LIST=$PINNED_HWSETTING_LIST
				fi

				for hwsetting in $BUILD_HWSETTING_LIST
				do
					echo %%%%%%%% $customer_id -- $chip_type -- $hwsetting %%%%%%
					if [[ $hwsetting == *"NAND"* ]]; then
						echo "NAND hwsetting skip"
						continue
					fi
					hwsetting=`echo $hwsetting | cut -d '.' -f 1`
					sed -i "s/Board_HWSETTING =.*$/Board_HWSETTING = $hwsetting/" $INCFILE_DIR/$DVRBOOT_PRJ.inc
					# Build normal boot version first
					DRM=
					PINNED_CHIP_TYPE_LIST=$chip_type
					secure_img_setup
					make $1 -j4 PRJ=$DVRBOOT_PRJ CUSTOMER_ID=$customer_id CHIP_TYPE=$chip_type BUILDID="$BUILD_ID"
					get_dvrboot_config
					cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
					if [ "$TARGET_MAIN" == "rtd1395" ]; then
						cp $DVRBOOT_DIR/install_a/hw_setting.bin $TOPDIR/DVRBOOT_OUT/hwsetting/$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
						cp $DVRBOOT_DIR/Bind/emmc.bind.bin $TOPDIR/DVRBOOT_OUT/hwsetting/Recovery-$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
					else
						cp $DVRBOOT_DIR/image/hw_setting/$TARGET_HW_SETTING.bin $TOPDIR/DVRBOOT_OUT/hwsetting/
					fi

					# Next, build secure boot version if needed
					if [ "$HAS_SECUREBOOT" == "" ] ; then
						echo DRM version not supported
						continue
					fi

					echo %%%%%%%% $customer_id -- $chip_type -- $hwsetting : DRM %%%%%%
					if [[ $hwsetting == *"NAND"* ]]; then
						echo "NAND hwsetting skip"
						continue
					fi
					DRM=1
					PINNED_CHIP_TYPE_LIST=$chip_type
					secure_img_setup
					make $1 -j4 PRJ=$DVRBOOT_PRJ CUSTOMER_ID=$customer_id CHIP_TYPE=$chip_type BUILDID="$BUILD_ID"
					get_dvrboot_config
					cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-drm.bin
					
					if [ "$TARGET_MAIN" == "rtd1395" ]; then
						DRM=2
						PINNED_CHIP_TYPE_LIST=$chip_type
						secure_img_setup
						make $1 -j4 PRJ=$DVRBOOT_PRJ CUSTOMER_ID=$customer_id CHIP_TYPE=$chip_type BUILDID="$BUILD_ID"
						get_dvrboot_config
						cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-atv.bin
						DRM=3
						PINNED_CHIP_TYPE_LIST=$chip_type
						secure_img_setup
						make $1 -j4 PRJ=$DVRBOOT_PRJ CUSTOMER_ID=$customer_id CHIP_TYPE=$chip_type BUILDID="$BUILD_ID"
						get_dvrboot_config
						cp $TOPDIR/build-$TARGET_PLATFORM/dvrboot.exe.bin $TOPDIR/DVRBOOT_OUT/$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-vmx.bin
					fi

					if [ "$TARGET_MAIN" == "rtd1395" ]; then
						cp $DVRBOOT_DIR/install_a/hw_setting.bin $TOPDIR/DVRBOOT_OUT/hwsetting/$TARGET_CHIP_TYPE-$TARGET_HW_SETTING.bin
						cp $DVRBOOT_DIR/Bind/emmc.bind.bin $TOPDIR/DVRBOOT_OUT/hwsetting/Recovery-$TARGET_CUSTOMER_ID-$TARGET_CHIP_TYPE-$TARGET_HW_SETTING-drm.bin
					else
						cp $DVRBOOT_DIR/image/hw_setting/$TARGET_HW_SETTING.bin $TOPDIR/DVRBOOT_OUT/hwsetting/
					fi
				done
			done
		done

		git checkout $INCFILE_DIR/$DVRBOOT_PRJ.inc
		;;
	*)
		echo cant reach here!!
		exit -1
		;;
esac

