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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/wait.h>

#include <linux/kdev_t.h>

#define LOG_TAG "Vold"

#include <cutils/log.h>
#include <cutils/properties.h>
#include <logwrap/logwrap.h>
#include <selinux/selinux.h>

#include "HfsPlus.h"
#include "Utils.h"
#include "VoldUtil.h"

namespace android {
namespace vold {
namespace hfsplus {

static const char* kMkfsPath = "/system/bin/mkhfs";
static const char* kFsckPath = "/system/bin/chkufsd";

bool IsSupported() {
    return access(kMkfsPath, X_OK) == 0
            && access(kFsckPath, X_OK) == 0
            && IsFilesystemSupported("hfsplus");
}

status_t Check(const std::string& source) {
    // The following is shamelessly borrowed from fs_mgr.c, so it should be
    // kept in sync with any changes over there.

    const char* c_source = source.c_str();

    /*
     * Some system images do not have e2fsck for licensing reasons
     * (e.g. recent SDK system images). Detect these and skip the check.
     */
    if (access(kFsckPath, X_OK)) {
        ALOGD("Not running %s on %s (executable not in system image)\n",
                kFsckPath, c_source);
    } else {
        ALOGD("Running %s on %s\n", kFsckPath, c_source);

        std::vector<std::string> cmd;
        cmd.push_back(kFsckPath);
        cmd.push_back("-fs:hfs");
        cmd.push_back("-f");
        cmd.push_back(c_source);

        int rc = ForkExecvp(cmd, sFsckContext);
        switch(rc) {
        case 0:
            SLOGI("-a flag was specified on the command line and the specified volume does not have dirty flag set. Consistency checks are not performed in that case.");
            SLOGI("Filesystem check completed OK");
            return 0;
        case 1:
            SLOGI("(RC:1)No errors were found on the specified volume");
            return 0;
        case 2:
            SLOGI("(RC:2)Errors were found and fixed");
            return 0;
        case 3:
            SLOGE("(RC:3)Only minor errors were found on the specified volume");
            return -1;
        case 4:
            SLOGE("(RC:4)Errors were found on the specified volume, but they could not be fixed despite that -f flag was specified on the command line.");
            return -1;
        case 5:
            SLOGE("(RC:5)Errors were found on the specified volume. Found errors are not fixed. The -f flag was not specified on the command line");
            return -1;
        case 6:
            SLOGE("(RC:6)Unexpected error is reported during volume check. Checking process is not finished.");
            return -1;
        case 7:
            SLOGE("(RC:7)Unexpected error is reported during volume check. Checking process is not finished.");
            return -1;
        case 10:
            SLOGE("(RC:10)Out of memory.");
            return 0;
        case 12:
            SLOGE("(RC:12)Error writing to the specified device. Was the device mounted as read-only?");
            return 0;
        case 13:
            SLOGE("(RC:13)Operation was interrupted with Ctrl+C");
            return 0;
        case 17:
            SLOGE("(RC:17)Media storage was detached.");
            return 0;
        default:
            SLOGE("Filesystem check failed (unknown exit return code %d)", rc);
            errno = EIO;
            return -1;
        }// switch
    }

    return 0;
}

status_t Mount(const std::string& source, const std::string& target, bool ro,
        bool remount, bool executable, int ownerUid, int ownerGid, int permMask, bool createLost) {
    int rc;
    unsigned long flags;
    char mountData[255];

    const char* c_source = source.c_str();
    const char* c_target = target.c_str();

    flags = MS_NOATIME | MS_NODEV | MS_NOSUID | MS_DIRSYNC;

    flags |= (executable ? 0 : MS_NOEXEC);
    flags |= (ro ? MS_RDONLY : 0);
    flags |= (remount ? MS_REMOUNT : 0);

    //try paragon HfsPlus driver first
    sprintf(mountData, "force,nls=utf8,uid=%d,gid=%d,umask=%o", ownerUid, ownerGid, permMask);
    rc = mount(c_source, c_target, "ufsd", flags, mountData);

    if (rc && errno == EROFS) {
        SLOGE("%s appears to be a read only filesystem - retrying mount RO(hfsplus of ufsd)", c_source);
        flags |= MS_RDONLY;
        rc = mount(c_source, c_target, "ufsd", flags, mountData);

        if (rc && errno == EROFS) {
            SLOGE("%s appears to be a read only filesystem - retrying mount RO", c_source);
            flags |= MS_RDONLY;
            rc = mount(c_source, c_target, "hfsplus", flags, mountData);
        }
    }else if(rc && errno == ENODEV){
        rc = mount(c_source, c_target, "hfsplus", flags, mountData);
        if (rc && errno == EROFS) {
            SLOGE("%s appears to be a read only filesystem - retrying mount RO", c_source);
            flags |= MS_RDONLY;
            rc = mount(c_source, c_target, "hfsplus", flags, mountData);
        }
    }

    return rc;
}

status_t Format(const std::string& source) {
    std::vector<std::string> cmd;
    cmd.push_back(kMkfsPath);
    cmd.push_back("-f");
    cmd.push_back("-j");
    cmd.push_back(source);

    return ForkExecvp(cmd);
}

status_t RepairMount(const std::string& source, const std::string& target,
        bool ro, bool remount, bool executable, int ownerUid, int ownerGid, int permMask, bool createLost,
        bool bCheckVolume, bool bFormatWhenError, bool bTryMount) {
    status_t ret = 0;
    if(bTryMount){
        ret = hfsplus::Mount(source, target, ro, remount, executable, ownerUid, ownerGid, permMask, createLost);
        if(ret == 0){
            SLOGD("HfsPlus try mount success.");
            return ret;
        }
    }
    if(bCheckVolume){
       if(hfsplus::Check(source)){
           SLOGE("HfsPlus check volume fail.");
           if(bFormatWhenError){
               if(hfsplus::Format(source)){
                   SLOGE("HfsPlus format volume fail.");
               }
           }
       }
    }
    return hfsplus::Mount(source, target, ro, remount, executable, ownerUid, ownerGid, permMask, createLost);
}

}  // namespace hfsplus
}  // namespace vold
}  // namespace android
