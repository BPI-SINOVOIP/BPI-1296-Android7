/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/watchdog.h>
#include "property_service.h"

#include "log.h"
#include "util.h"

#define REALTEK_WATCHDOG

#define DEV_NAME "/dev/watchdog"

int watchdogd_main(int argc, char **argv) {
    int success = 0;
    int bIsShutdown = 0;

    open_devnull_stdio();
    klog_init();
    klog_set_level(KLOG_NOTICE_LEVEL);

    int interval = 10;
    if (argc >= 2) interval = atoi(argv[1]);

    int margin = 10;
    if (argc >= 3) margin = atoi(argv[2]);

    NOTICE("started (interval %d, margin %d)!\n", interval, margin);

#ifndef REALTEK_WATCHDOG
    int fd = open(DEV_NAME, O_RDWR|O_CLOEXEC);
    if (fd == -1) {
        ERROR("Failed to open %s: %s\n", DEV_NAME, strerror(errno));
        return 1;
    }
#endif

#ifndef REALTEK_WATCHDOG
    int timeout = interval + margin;
    int ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
    if (ret) {
        ERROR("Failed to set timeout to %d: %s\n", timeout, strerror(errno));
        ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
        if (ret) {
            ERROR("Failed to get timeout: %s\n", strerror(errno));
        } else {
            if (timeout > margin) {
                interval = timeout - margin;
            } else {
                interval = 1;
            }
            WARNING("Adjusted interval to timeout returned by driver:"
                    " timeout %d, interval %d, margin %d\n",
                    timeout, interval, margin);
        }
    }
#endif

    while (true) {
#ifdef REALTEK_WATCHDOG
        if (success == 0)
        {
            if (property_get("dev.bootcomplete") == "1"
                || property_get("sys.boot_completed") == "1")
            {
                success = 1;
            }
        }
        else
        {
            if (bIsShutdown == 0)
            {
                system("echo kick > /proc/watchdog");
                if (property_get("rtk.power.shutdown") == "1")
                {
                    ERROR("watchdogd: power shutdown\n");
                    bIsShutdown = 1;
                }
            }
        }
#else
        write(fd, "", 1);
#endif
        sleep(interval);
    }
}
