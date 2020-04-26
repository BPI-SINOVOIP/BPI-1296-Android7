#!/system/bin/sh

/system/bin/logcat -b kernel -b main -b system -b crash > /data/logcat.txt &

