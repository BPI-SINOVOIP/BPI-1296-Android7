/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "fs/Vfat.h"
#include "fs/Ext4.h"
#include "fs/Ntfs.h"
#include "fs/Ext3.h"
#include "fs/Ext2.h"
#include "fs/ExFat.h"
#include "fs/HfsPlus.h"
#include "fs/Btrfs.h"
#include "PublicVolume.h"
#include "Utils.h"
#include "VolumeManager.h"
#include "ResponseCode.h"

#include <android-base/stringprintf.h>
#include <android-base/logging.h>
#include <cutils/fs.h>
#include <private/android_filesystem_config.h>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cutils/properties.h>

using android::base::StringPrintf;

namespace android {
namespace vold {

static const char* kFusePath = "/system/bin/sdcard";

static const char* kAsecPath = "/mnt/secure/asec";

PublicVolume::PublicVolume(dev_t device) :
        VolumeBase(Type::kPublic), mDevice(device), mFusePid(0) {
    setId(StringPrintf("public:%u,%u", major(device), minor(device)));
    mDevPath = StringPrintf("/dev/block/vold/%s", getId().c_str());
}

PublicVolume::~PublicVolume() {
}

status_t PublicVolume::readMetadata() {
    status_t res = ReadMetadataUntrusted(mDevPath, mFsType, mFsUuid, mFsLabel);
    notifyEvent(ResponseCode::VolumeFsTypeChanged, mFsType);
    notifyEvent(ResponseCode::VolumeFsUuidChanged, mFsUuid);
    notifyEvent(ResponseCode::VolumeFsLabelChanged, mFsLabel);
    return res;
}

status_t PublicVolume::initAsecStage() {
    std::string legacyPath(mRawPath + "/android_secure");
    std::string securePath(mRawPath + "/.android_secure");

    // Recover legacy secure path
    if (!access(legacyPath.c_str(), R_OK | X_OK)
            && access(securePath.c_str(), R_OK | X_OK)) {
        if (rename(legacyPath.c_str(), securePath.c_str())) {
            PLOG(WARNING) << getId() << " failed to rename legacy ASEC dir";
        }
    }

    if (TEMP_FAILURE_RETRY(mkdir(securePath.c_str(), 0700))) {
        if (errno != EEXIST) {
            PLOG(WARNING) << getId() << " creating ASEC stage failed";
            return -errno;
        }
    }

    BindMount(securePath, kAsecPath);

    return OK;
}

status_t PublicVolume::doCreate() {
    return CreateDeviceNode(mDevPath, mDevice);
}

status_t PublicVolume::doDestroy() {
    return DestroyDeviceNode(mDevPath);
}

status_t PublicVolume::doMount() {
    // TODO: expand to support mounting other filesystems
    readMetadata();

    if (mFsType != "ext4"    &&
        mFsType != "vfat"    &&
        mFsType != "ntfs"    &&
        mFsType != "ext3"    &&
        mFsType != "ext2"    &&
        mFsType != "exfat"   &&
        mFsType != "btrfs"   &&
        mFsType != "hfsplus"
        ) {
        LOG(ERROR) << getId() << " unsupported filesystem " << mFsType;
        return -EIO;
    }

    // Use UUID as stable name, if available
    std::string stableName = getId();
    if (!mFsUuid.empty()) {
        stableName = mFsUuid;
    }


    mRawPath = StringPrintf("/mnt/media_rw/%s", stableName.c_str());
    /*
     *if(mFsType.compare("ntfs")==0){
     *    if (ntfs::Check(mDevPath)) {
     *           LOG(ERROR) << getId() << " NTFS failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("ext4")==0){
     *    if (ext4::Check(mDevPath, mRawPath)) {
     *           LOG(ERROR) << getId() << " EXT4 failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("ext3")==0){
     *    if (ext3::Check(mDevPath, mRawPath)) {
     *           LOG(ERROR) << getId() << " EXT3 failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("ext2")==0){
     *    if (ext2::Check(mDevPath, mRawPath)) {
     *           LOG(ERROR) << getId() << " EXT2 failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("exfat")==0){
     *    if (exfat::Check(mDevPath, mRawPath)) {
     *           LOG(ERROR) << getId() << " EXFAT failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("hfsplus")==0){
     *    if (hfsplus::Check(mDevPath)) {
     *           LOG(ERROR) << getId() << " HFSPLUS failed filesystem check";
     *           return -EIO;
     *       }
     *}
     *else if(mFsType.compare("vfat")==0){
     *    if (vfat::Check(mDevPath)) {
     *           LOG(ERROR) << getId() << " VFAT failed filesystem check";
     *           return -EIO;
     *       }
     *}
     */
    mFuseDefault = StringPrintf("/mnt/runtime/default/%s", stableName.c_str());
    mFuseRead = StringPrintf("/mnt/runtime/read/%s", stableName.c_str());
    mFuseWrite = StringPrintf("/mnt/runtime/write/%s", stableName.c_str());

    setInternalPath(mRawPath);
    if (getMountFlags() & MountFlags::kVisible) {
        setPath(StringPrintf("/storage/%s", stableName.c_str()));
    } else {
        setPath(mRawPath);
    }

    if (fs_prepare_dir(mRawPath.c_str(), 0700, AID_ROOT, AID_ROOT)) {
        PLOG(ERROR) << getId() << " failed to create mount points";
        return -errno;
    }

    bool bCheckVolume = true;
    bool bFormatWhenError = true;
    bool bTryMount= true;
    char szCheckVolume_state[PROPERTY_VALUE_MAX];
    char szFormatWhenError_state[PROPERTY_VALUE_MAX];
    char szTryMount_state[PROPERTY_VALUE_MAX];
    property_get("persist.rtk.vold.checkvolume", szCheckVolume_state, "1");
    property_get("persist.rtk.vold.formatwhenerror", szFormatWhenError_state, "0");
    property_get("persist.rtk.vold.trymount", szTryMount_state, "1");
    bCheckVolume     = strcmp(szCheckVolume_state,     "1")==0 ? true:false;
    bFormatWhenError = strcmp(szFormatWhenError_state, "1")==0 ? true:false;
    bTryMount        = strcmp(szTryMount_state,        "1")==0 ? true:false;
    LOG(WARNING) << "trymount: " << szTryMount_state <<" , checkvolume: " << szCheckVolume_state << " , formatwhenerror: " << szFormatWhenError_state;

    char szAID[100]="";
    sprintf(szAID, "%d", AID_MEDIA_RW);
    if(mFsType.compare("vfat")==0){
        if (vfat::RepairMount(mDevPath, mRawPath, false, false, false,
                AID_MEDIA_RW, AID_MEDIA_RW, 0007, true, bCheckVolume, bFormatWhenError, bTryMount)) {
            PLOG(ERROR) << getId() << " failed to mount via VFAT" << mDevPath;
            return -EIO;
        }
    }
    else if(mFsType.compare("ntfs")==0){
        if (ntfs::RepairMount(mDevPath, mRawPath, false, false, false,
                AID_MEDIA_RW, AID_MEDIA_RW, 0007, true, bCheckVolume, bFormatWhenError, bTryMount)) {
                PLOG(ERROR) << getId() << " failed to mount via NTFS" << mDevPath;
                return -EIO;
            }
    }
    else if(mFsType.compare("ext4")==0){
        if (ext4::RepairMount(mDevPath, mRawPath, false, false, false, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via EXT4" << mDevPath;
                return -EIO;
            }
        sprintf(szAID, "%d", AID_ROOT);
    }
    else if(mFsType.compare("ext3")==0){
        if (ext3::RepairMount(mDevPath, mRawPath, false, false, false, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via EXT3" << mDevPath;
                return -EIO;
            }
        sprintf(szAID, "%d", AID_ROOT);
    }
    else if(mFsType.compare("ext2")==0){
        if (ext2::RepairMount(mDevPath, mRawPath, false, false, false, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via EXT2" << mDevPath;
                return -EIO;
            }
        sprintf(szAID, "%d", AID_ROOT);
    }
    else if(mFsType.compare("exfat")==0){
        if (exfat::RepairMount(mDevPath, mRawPath, false, false, false, AID_MEDIA_RW, AID_MEDIA_RW, 0007, true, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via EXFAT" << mDevPath;
                return -EIO;
            }
    }
    else if(mFsType.compare("hfsplus")==0){
        if (hfsplus::RepairMount(mDevPath, mRawPath, false, false, false, AID_MEDIA_RW, AID_MEDIA_RW, 0007, true, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via HFS+ " << mDevPath;
                return -EIO;
            }
    }
    else if(mFsType.compare("btrfs")==0){
        if (btrfs::RepairMount(mDevPath, mRawPath, false, false, false, bCheckVolume, bFormatWhenError, bTryMount)){
                PLOG(ERROR) << getId() << " failed to mount via BTRFS" << mDevPath;
                return -EIO;
            }
        sprintf(szAID, "%d", AID_ROOT);
    }

    if (getMountFlags() & MountFlags::kPrimary) {
        initAsecStage();
    }

    if (!(getMountFlags() & MountFlags::kVisible)) {
        // Not visible to apps, so no need to spin up FUSE
        return OK;
    }

    if (fs_prepare_dir(mFuseDefault.c_str(), 0700, AID_ROOT, AID_ROOT) ||
            fs_prepare_dir(mFuseRead.c_str(), 0700, AID_ROOT, AID_ROOT) ||
            fs_prepare_dir(mFuseWrite.c_str(), 0700, AID_ROOT, AID_ROOT)) {
        PLOG(ERROR) << getId() << " failed to create FUSE mount points";
        return -errno;
    }

    dev_t before = GetDevice(mFuseWrite);

    if (!(mFusePid = fork())) {
        if (getMountFlags() & MountFlags::kPrimary) {
            if (execl(kFusePath, kFusePath,
                    "-u", szAID, // AID_MEDIA_RW
                    "-g", szAID, // AID_MEDIA_RW
                    "-U", std::to_string(getMountUserId()).c_str(),
                    "-w",
                    mRawPath.c_str(),
                    stableName.c_str(),
                    NULL)) {
                PLOG(ERROR) << "Failed to exec";
            }
        } else {
            if (execl(kFusePath, kFusePath,
                    "-u", szAID,
                    "-g", szAID,
                    "-U", std::to_string(getMountUserId()).c_str(),
                    mRawPath.c_str(),
                    stableName.c_str(),
                    NULL)) {
                PLOG(ERROR) << "Failed to exec";
            }
        }

        LOG(ERROR) << "FUSE exiting";
        _exit(1);
    }

    if (mFusePid == -1) {
        PLOG(ERROR) << getId() << " failed to fork";
        return -errno;
    }

    while (before == GetDevice(mFuseWrite)) {
        LOG(VERBOSE) << "Waiting for FUSE to spin up...";
        usleep(50000); // 50ms
    }

    return OK;
}

status_t PublicVolume::doUnmount() {
    // Unmount the storage before we kill the FUSE process. If we kill
    // the FUSE process first, most file system operations will return
    // ENOTCONN until the unmount completes. This is an exotic and unusual
    // error code and might cause broken behaviour in applications.
    KillProcessesUsingPath(getPath());

    ForceUnmount(getPath()); //rtk: unmount /storage/[UUID]

    ForceUnmount(kAsecPath);

    ForceUnmount(mFuseDefault);
    ForceUnmount(mFuseRead);
    ForceUnmount(mFuseWrite);
    ForceUnmount(mRawPath);

    if (mFusePid > 0) {
        kill(mFusePid, SIGTERM);
        TEMP_FAILURE_RETRY(waitpid(mFusePid, nullptr, 0));
        mFusePid = 0;
    }

    rmdir(mFuseDefault.c_str());
    rmdir(mFuseRead.c_str());
    rmdir(mFuseWrite.c_str());
    rmdir(mRawPath.c_str());

    mFuseDefault.clear();
    mFuseRead.clear();
    mFuseWrite.clear();
    mRawPath.clear();

    return OK;
}

status_t PublicVolume::doFormat(const std::string& fsType) {
    if (fsType == "vfat" || fsType == "auto" || fsType == "ext4" || fsType == "ext3" || fsType == "btrfs" 
            || fsType == "ext2" || fsType == "exfat" || fsType == "ntfs") {
        if (WipeBlockDevice(mDevPath) != OK) {
            LOG(WARNING) << getId() << " failed to wipe";
        }
        if (vfat::Format(mDevPath, 0)) {
            LOG(ERROR) << getId() << " failed to format(VFAT)";
            return -errno;
        }
    } else {
        LOG(ERROR) << "Unsupported filesystem " << fsType;
        return -EINVAL;
    }

    return OK;
}

}  // namespace vold
}  // namespace android
