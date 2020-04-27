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

#include <android-base/stringprintf.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <logwrap/logwrap.h>
#include <selinux/selinux.h>

#include "Btrfs.h"
#include "Utils.h"
#include "VoldUtil.h"

namespace android {
namespace vold {
namespace btrfs {

static const char* kResizefsPath = "/system/bin/resize2fs";
static const char* kMkfsPath = "/system/bin/mke2fs";
static const char* kFsckPath = "/system/bin/e2fsck";

bool IsSupported() {
    return access(kMkfsPath, X_OK) == 0
            && access(kFsckPath, X_OK) == 0
            && IsFilesystemSupported("btrfs");
}

status_t Check(const std::string& source, const std::string& target) {
    // The following is shamelessly borrowed from fs_mgr.c, so it should be
    // kept in sync with any changes over there.

    const char* c_source = source.c_str();
    const char* c_target = target.c_str();

    int status;
    int ret;
    long tmpmnt_flags = MS_NOATIME | MS_NOEXEC | MS_NOSUID;
    char *tmpmnt_opts = (char*) "nomblk_io_submit,errors=remount-ro";

    /*
     * First try to mount and unmount the filesystem.  We do this because
     * the kernel is more efficient than e2fsck in running the journal and
     * processing orphaned inodes, and on at least one device with a
     * performance issue in the emmc firmware, it can take e2fsck 2.5 minutes
     * to do what the kernel does in about a second.
     *
     * After mounting and unmounting the filesystem, run e2fsck, and if an
     * error is recorded in the filesystem superblock, e2fsck will do a full
     * check.  Otherwise, it does nothing.  If the kernel cannot mount the
     * filesytsem due to an error, e2fsck is still run to do a full check
     * fix the filesystem.
     */
    ret = mount(c_source, c_target, "btrfs", tmpmnt_flags, tmpmnt_opts);
    if (!ret) {
        int i;
        for (i = 0; i < 5; i++) {
            // Try to umount 5 times before continuing on.
            // Should we try rebooting if all attempts fail?
            int result = umount(c_target);
            if (result == 0) {
                break;
            }
            ALOGW("%s(): umount(%s)=%d: %s\n", __func__, c_target, result, strerror(errno));
            sleep(1);
        }
    }

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
        cmd.push_back("-y");
        cmd.push_back(c_source);

        //// ext4 devices are currently always trusted
        //return ForkExecvp(cmd, sFsckContext);
        int rc = ForkExecvp(cmd, sFsckContext);
        switch(rc) {
        case 0:
            SLOGI("Filesystem check completed OK");
            return 0;
        case 1:
            SLOGI("(RC:1)File system errors corrected");
            return 0;
        case 2:
            SLOGI("(RC:2)File system errors corrected, system should be rebooted");
            return 0;
        case 4:
            SLOGE("(RC:4)File system errors left uncorrected.");
            return -1;
        case 8:
            SLOGI("(RC:8)Operational error");
            return 0;
        case 16:
            SLOGI("(RC:16)Usage or syntax error.");
            return 0;
        case 32:
            SLOGI("(RC:32)E2fsck canceled by user request.");
            return 0;
        case 128:
            SLOGE("(RC:128)Shared library error.");
            return 0;
        default:
            SLOGE("Filesystem check failed (unknown exit return code %d)", rc);
            errno = EIO;
            return -1;
        } // switch
    }

    return 0;
}

status_t Mount(const std::string& source, const std::string& target, bool ro,
        bool remount, bool executable) {
    int rc;
    unsigned long flags;

    const char* c_source = source.c_str();
    const char* c_target = target.c_str();

    flags = MS_NOATIME | MS_NODEV | MS_NOSUID | MS_DIRSYNC;

    flags |= (executable ? 0 : MS_NOEXEC);
    flags |= (ro ? MS_RDONLY : 0);
    flags |= (remount ? MS_REMOUNT : 0);

    rc = mount(c_source, c_target, "btrfs", flags, NULL);

    if (rc && errno == EROFS) {
        SLOGE("%s appears to be a read only filesystem - retrying mount RO", c_source);
        flags |= MS_RDONLY;
        rc = mount(c_source, c_target, "btrfs", flags, NULL);
    }

    return rc;
}

status_t Resize(const std::string& source, unsigned int numSectors) {
    std::vector<std::string> cmd;
    cmd.push_back(kResizefsPath);
    cmd.push_back("-f");
    cmd.push_back(source);
    cmd.push_back(::android::base::StringPrintf("%u", numSectors));

    return ForkExecvp(cmd);
}

status_t Format(const std::string& source) {
    std::vector<std::string> cmd;
    cmd.push_back(kMkfsPath);
//    cmd.push_back("-J");

    cmd.push_back("-t");
    cmd.push_back("btrfs");
    // Always generate a real UUID
    // cmd.push_back("-u");
    cmd.push_back(source);

    return ForkExecvp(cmd);
}

status_t RepairMount(const std::string& source, const std::string& target,
        bool ro, bool remount, bool executable,
        bool bCheckVolume, bool bFormatWhenError, bool bTryMount) {
    status_t ret = 0;
    if(bTryMount){
        ret = btrfs::Mount(source, target, ro, remount, executable);
        if(ret == 0){
            SLOGD("BTRFS try mount success.");
            return ret;
        }
    }
    if(bCheckVolume){
       if(btrfs::Check(source, target)){
           SLOGE("BTRFS check volume fail.");
           if(bFormatWhenError){
               if(btrfs::Format(source)){
                   SLOGE("BTRFS format volume fail.");
               }
           }
       }
    }
    return btrfs::Mount(source, target, ro, remount, executable);
}

}  // namespace btrfs 
}  // namespace vold
}  // namespace android
