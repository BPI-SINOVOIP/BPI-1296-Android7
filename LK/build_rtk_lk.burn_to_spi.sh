#!/bin/bash
##### backup git source
cp tools/efuse_verify/enc.dat tmp/.
##### backup eMMC config (default)
cp project/target/rtd1295.mk project/target/rtd1295.git.mk
cp target/rtd1295/rules.mk target/rtd1295/rules.git.mk
##### build eMMC
cp project/target/rtd1295.emmc.mk project/target/rtd1295.mk
cp target/rtd1295/rules.emmc.mk target/rtd1295/rules.mk
./build_rtk_lk.sh rtd1295
##### save LK(eMMC)
cp build-rtd1295/lk.bin tmp/lk.emmc.bin
##### build SPI
cp project/target/rtd1295.spi.mk project/target/rtd1295.mk
cp target/rtd1295/rules.spi.mk target/rtd1295/rules.mk
./build_rtk_lk.sh rtd1295
##### restore eMMC config (just not change git source)
cp project/target/rtd1295.git.mk project/target/rtd1295.mk
cp target/rtd1295/rules.git.mk target/rtd1295/rules.mk
cp tmp/enc.dat tools/efuse_verify/enc.dat
##### remove tempary
rm project/target/rtd1295.git.mk
rm target/rtd1295/rules.git.mk
#####
ls -l tmp/lk.emmc.bin
ls -l build-rtd1295/lk.bin
diff tmp/lk.emmc.bin build-rtd1295/lk.bin
