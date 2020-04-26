#!/system/bin/sh
#change performance mode
CPUID="0 1 2 3"
mode="performance"
for cpuid in $CPUID; do
    echo $mode > /sys/devices/system/cpu/cpu$cpuid/cpufreq/scaling_governor
done

