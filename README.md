# BPI-1296-Android7
**Prepare**


Get the docker image from [Sinovoip Docker Hub](https://hub.docker.com/r/sinovoip/bpi-build-android-7),  Build the android source with this docker environment.

---------
**Build**

Build LK

    $ cd LK
    $ ./build_rtk_lk.sh rtd1295
    $ cp bootloader_lk.tar ../Openwrt/target/linux/rtd1295/image/image_file-r160868/packages/omv/

Build target image

    $ ./build_android.sh build
    $ ./build_openwrt.sh build

Target android image is Openwrt/bin/rtd1295-glibc/install.img, please read the [bpi wiki](http://wiki.banana-pi.org/Getting_Started_with_W2#Install_Android_image) to get how to flash the image to board.
