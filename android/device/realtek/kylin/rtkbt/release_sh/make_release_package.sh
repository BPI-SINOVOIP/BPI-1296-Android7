#!/bin/sh

GEN_DATE=$(date +%Y%m%d)
GEN_TIME=$(date +%H:%M:%S)

function usage()
{
	echo -e "$1 must be executed on ANDROID TOP DIRECTORY(eg, ~/workdir/Android7.0/)\n${ROOTFS_DIR}/system/etc/bluetooth/rtkbt.conf ==> not existed\n"
}

ANDROID_TOP_DIR=${PWD}
ROOTFS_DIR=${ANDROID_TOP_DIR}/out/target/product/kylin32
DEVICE_RTKBT_DIR=${ANDROID_TOP_DIR}/device/realtek/kylin/rtkbt
BDROID_DIR=${DEVICE_RTKBT_DIR}/code/bt

if [ ! -f ${ROOTFS_DIR}/system/etc/bluetooth/rtkbt.conf ]; then
	usage $0
	exit 1
fi

SUBVER_RTKBT_TOOLCHAIN=`cat ${BDROID_DIR}/realtek/include/rtkbt_api_ver_toolchain.h |grep SUBVER_RTKBT_TOOLCHAIN | awk '{print $3}' | sed 's/(//'|sed 's/)//'`
SUBVER_RTKBT_API=`cat ${BDROID_DIR}/realtek/include/rtkbt_api_ver.h |grep SUBVER_RTKBT_API | awk '{print $3}' | sed 's/(//'|sed 's/)//'`
SUBVER_RTKBT_REVISION=`cat ${BDROID_DIR}/realtek/include/rtkbt_api_ver.h |grep SUBVER_RTKBT_REVISION | awk '{print $3}' | sed 's/(//'|sed 's/)//'`
SUBVER_RTKBT_CUSTOMIZED_REVISION=`cat ${BDROID_DIR}/realtek/include/rtkbt_api_ver.h |grep SUBVER_RTKBT_CUSTOMIZED_REVISION | awk '{print $3}' | sed 's/(//'|sed 's/)//'`

RTKBT_API_VERSION=${SUBVER_RTKBT_TOOLCHAIN}.${SUBVER_RTKBT_API}.${SUBVER_RTKBT_REVISION}.${SUBVER_RTKBT_CUSTOMIZED_REVISION}
echo "RTKBT_API_VERSION=${RTKBT_API_VERSION}"

RELEASE_DIR=${ANDROID_TOP_DIR}/${GEN_DATE}_TV_ANDROID_7.x_V${RTKBT_API_VERSION}
rm -rf ${RELEASE_DIR}.bk
mv ${RELEASE_DIR} ${RELEASE_DIR}.bk

RELEASE_RTKBT_DIR=${RELEASE_DIR}/rtkbt
RELEASE_RTKBT_ADDON_DIR=${RELEASE_RTKBT_DIR}/addon
RELEASE_RTKBT_CODE_BDROID_REALTEK_DIR=${RELEASE_RTKBT_DIR}/code/bt/realtek

RELEASE_RTKBT_CLOSED_DIR=${RELEASE_DIR}/rtkbt-closed
RELEASE_RTKBT_CLOSED_CLOSED_DIR=${RELEASE_RTKBT_CLOSED_DIR}/closed
RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR=${RELEASE_RTKBT_CLOSED_DIR}/code/bt/realtek

#mkdir all directory
mkdir -p ${RELEASE_RTKBT_DIR} ${RELEASE_RTKBT_ADDON_DIR} ${RELEASE_RTKBT_CLOSED_CLOSED_DIR} ${RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR}

# Copy rtkbt&patch files
cp -arf ${DEVICE_RTKBT_DIR}/* ${RELEASE_RTKBT_DIR}/.
mv ${RELEASE_RTKBT_DIR}/closed ${RELEASE_RTKBT_CLOSED_DIR}/.
mv ${RELEASE_RTKBT_CODE_BDROID_REALTEK_DIR}/* ${RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR}/.
mv ${RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR}/include ${RELEASE_RTKBT_CODE_BDROID_REALTEK_DIR}/.
mv ${RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR}/src ${RELEASE_RTKBT_CODE_BDROID_REALTEK_DIR}/.
mv ${RELEASE_RTKBT_CLOSED_BDROID_REALTEK_DIR}/Android.mk ${RELEASE_RTKBT_CODE_BDROID_REALTEK_DIR}/.
mv ${RELEASE_RTKBT_DIR}/release_sh/template/* ${RELEASE_DIR}/.
cp ${ANDROID_TOP_DIR}/../phoenix/system/src/drivers/bluetooth/rtk_btusb/rtk_btusb.c ${RELEASE_DIR}/linux/drivers/bluetooth/.
cp ${ANDROID_TOP_DIR}/../phoenix/system/src/drivers/bluetooth/rtk_btusb/rtk_btusb.h ${RELEASE_DIR}/linux/drivers/bluetooth/.
rm -rf ${RELEASE_RTKBT_DIR}/release_sh

#Copy General TV bluetooth libraries
mkdir -p ${RELEASE_RTKBT_DIR}/system/lib/rtkbt/ ${RELEASE_RTKBT_DIR}/system/lib/hw/
cp -af ${ROOTFS_DIR}/system/lib/rtkbt/* ${RELEASE_RTKBT_DIR}/system/lib/rtkbt/
cp -af ${ROOTFS_DIR}/system/lib/hw/audio.vfremote.default.so ${RELEASE_RTKBT_DIR}/system/lib/hw/audio.vfremote.default.so
cp -af  ${ROOTFS_DIR}/system/lib/hw/audio*bee* ${RELEASE_RTKBT_DIR}/system/lib/hw/.

sed -i "s/^BtDeviceNode.*/BtDeviceNode=\/dev\/rtk_btusb/" ${RELEASE_RTKBT_DIR}/system/etc/bluetooth/rtkbt.conf 2>/dev/null
#Generate rtkbt.mk
echo -e "# this file is auto generated. \n# RELEASE_TIME: ${GEN_DATE} ${GEN_TIME}\n# RTKBT_API_VERSION=${RTKBT_API_VERSION}\n" > ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "BOARD_HAVE_BLUETOOTH := true" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "BOARD_HAVE_BLUETOOTH_RTK := true" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "BOARD_HAVE_BLUETOOTH_RTK_COEX := true" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "\n#BOARD_HAVE_BLUETOOTH_RTK_ADDON := bee1 rtkbtAutoPairService rtkbtAutoPairUIDemo " >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "#BOARD_HAVE_BLUETOOTH_RTK_ADDON := bee1 rtkbtAutoPairService rtkbtAutoPairUIDemo vr_bee1_hidraw_daemon" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "#BOARD_HAVE_BLUETOOTH_RTK_ADDON := bee2 rtkbtAutoPairService rtkbtAutoPairUIDemo " >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "#BOARD_HAVE_BLUETOOTH_RTK_ADDON := bee2 rtkbtAutoPairService rtkbtAutoPairUIDemo vr_bee2_hidraw_daemon" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "BOARD_HAVE_BLUETOOTH_RTK_ADDON := basic\n" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk

echo -e "ifneq (\$(BOARD_HAVE_BLUETOOTH_RTK_ADDON),)" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "\$(foreach item,\$(BOARD_HAVE_BLUETOOTH_RTK_ADDON),\$(call inherit-product,\$(LOCAL_PATH)/addon/\$(item)/addon.mk))" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "#Firmware For Tv" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "include \$(LOCAL_PATH)/Firmware/TV/TV_Firmware.mk" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "else" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "#Firmware For Tablet" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "include \$(LOCAL_PATH)/Firmware/BT/BT_Firmware.mk" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "endif\n" >>  ${RELEASE_RTKBT_DIR}/rtkbt.mk

echo -e "PRODUCT_COPY_FILES += \\" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk

function scandir(){

  for filename in `ls $1`
    do
        if [ -d $1'/'$filename ] ; then
                echo Scan: $1'/'${filename}
                scandir $1'/'${filename}
        else
                echo -e "\t\$(LOCAL_PATH)/$1/${filename}:$1/${filename} \\" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk
        fi
    done
}

# Copy Binary Files, Generate rtkbt.mk
cd ${RELEASE_RTKBT_DIR}
scandir system
cd -

echo -e "\n" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "PRODUCT_PACKAGES += \\" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "\tBluetooth \\" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk
echo -e "\taudio.a2dp.default" >> ${RELEASE_RTKBT_DIR}/rtkbt.mk
cat ${RELEASE_RTKBT_DIR}/rtkbt.mk


# Copy Private Apps(rtkbtAutoPairService)
mkdir -p ${RELEASE_RTKBT_ADDON_DIR}/rtkbtAutoPairService
cp -arf ${ROOTFS_DIR}/system/app/rtkbtAutoPairService ${RELEASE_RTKBT_ADDON_DIR}/rtkbtAutoPairService/.
echo -e "app := rtkbtAutoPairService" > ${RELEASE_RTKBT_ADDON_DIR}/rtkbtAutoPairService/rtkbtAutoPairService/Android.mk
cat ${DEVICE_RTKBT_DIR}/release_sh/Android.mk.template.app >> ${RELEASE_RTKBT_ADDON_DIR}/rtkbtAutoPairService/rtkbtAutoPairService/Android.mk
cat ${RELEASE_RTKBT_ADDON_DIR}/rtkbtAutoPairService/rtkbtAutoPairService/Android.mk

# Copy Private Apps(BluetoothRTK3ddService)
mkdir -p ${RELEASE_RTKBT_ADDON_DIR}/BluetoothRTK3ddService
cp -arf ${ROOTFS_DIR}/system/app/BluetoothRTK3ddService ${RELEASE_RTKBT_ADDON_DIR}/BluetoothRTK3ddService/
echo -e "app := BluetoothRTK3ddService" > ${RELEASE_RTKBT_ADDON_DIR}/BluetoothRTK3ddService/BluetoothRTK3ddService/Android.mk
cat ${DEVICE_RTKBT_DIR}/release_sh/Android.mk.template.app >> ${RELEASE_RTKBT_ADDON_DIR}/BluetoothRTK3ddService/BluetoothRTK3ddService/Android.mk
cat ${RELEASE_RTKBT_ADDON_DIR}/BluetoothRTK3ddService/BluetoothRTK3ddService/Android.mk

echo "Release Patch Done."
