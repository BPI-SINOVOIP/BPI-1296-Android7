/*
 * Copyright (C) 2007 The Android Open Source Project
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
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

#include <fs_mgr.h>
#include "mtdutils/mtdutils.h"
#include "mtdutils/mounts.h"
#include "roots.h"
#include "common.h"
#include "make_ext4fs.h"
#include "bootloader.h"
#include "wipe.h"
#include "cryptfs.h"

static struct fstab *fstab = NULL;

#define RECOVERY_FSTAB_PATH "/etc/recovery.fstab"
#define ASSUMED_RECOVERY_FSTAB_NAME "META-INF/com/google/android/recovery.fstab"
#define ASSUMED_LAYOUT_TXT_NAME "/tmp/factory/layout.txt"
#define ASSUMED_USBMNT_PATH "/mnt/udisk"

extern struct selabel_handle *sehandle;

/****** Start of Extra Extension by rtk ************/
typedef enum _EXTERN_TYPE
{
    ID_NONE = -1,
    ID_PART = 0,
    ID_SDCARD,
    ID_USB
} EXTERN_TYPE;

typedef struct _externStorage_Desc
{
    EXTERN_TYPE type;
    char storage_name[32];
    char hotplug_mntpah[32];
    char mount_point[32];
} externStorage_Desc;

// This path is synced from hotplug daemon.
externStorage_Desc storage_list[] =
{
    {ID_SDCARD, "sdcard","/mmnt/sdcard/mmcblk1p1", ""},
    {ID_USB, "udisk","/mmnt/usb",""},
};

void dump_volume_table()
{
    int i;
    LOGD("recovery filesystem table\n");
    LOGD("=========================\n");
    for (i = 0; i < fstab->num_entries; ++i)
    {
        Volume* v = &fstab->recs[i];
        LOGD("  %d %s %s %s %lld \n", i, v->mount_point, v->fs_type,
             v->blk_device, v->length);
    }
    LOGD("\n");
}

int load_volume_table_byLayoutFile()
{

    int entries = 0, ret, i, cnt;
    char buffer[512];
    FILE* fstab_file = fopen(ASSUMED_LAYOUT_TXT_NAME, "r");
    if (fstab_file == NULL)
    {
        LOGE("failed to open %s (%s)\n", ASSUMED_LAYOUT_TXT_NAME,strerror(errno));
        return -1;
    }

    if (fstab != NULL)
    {
        LOGI("Initial device_volumes to NULL ptr\n");
        fs_mgr_free_fstab(fstab);
        fstab = NULL;
    }

    while (fgets(buffer, sizeof(buffer)-1, fstab_file))
    {
        for (i = 0; buffer[i] && isspace(buffer[i]); ++i);
        if(0 == strncmp("#define PART", buffer, 12))
        {
            entries++;
        }
    }

    /* Allocate and init the fstab structure */
    fstab = (struct fstab*)calloc(1, sizeof(struct fstab));
    fstab->num_entries = entries;
    fstab->fstab_filename = strdup(ASSUMED_LAYOUT_TXT_NAME);
    fstab->recs = (struct fstab_rec *)calloc(fstab->num_entries, sizeof(struct fstab_rec));

    fseek(fstab_file, 0, SEEK_SET);

    char *parsedStrPtr = NULL, *pch = NULL;
    char *mount_point = NULL, *fs_type = NULL, *device = NULL, *size=NULL;

    cnt = 0;
    while (fgets(buffer, sizeof(buffer)-1, fstab_file))
    {
        for (i = 0; buffer[i] && isspace(buffer[i]); ++i);
        mount_point = fs_type = device = NULL;
        if(0 == strncmp("#define PART", buffer, 12))
        {
            parsedStrPtr = strchr(buffer, '"');
            if (parsedStrPtr)
            {
                pch = strtok (parsedStrPtr+1," ");
                pch = strtok (NULL, " ");
                size = strchr(pch, '=')+1;
                pch = strtok (NULL, " ");
                mount_point = strchr(pch, '=')+1;
                pch = strtok (NULL, " ");
                device = strchr(pch, '=')+1;
                pch = strtok (NULL, " ");
                fs_type = strchr(pch, '=')+1;
                printf ("\t (device:%s, mount_point:%s, fs:%s, size:%d) \n", device, mount_point, fs_type,  (int)strtol(size, NULL, 16));
            }
        }
        if (mount_point && fs_type && device)
        {
            fstab->recs[cnt].blk_device = strdup(device);
            fstab->recs[cnt].mount_point = strdup(mount_point);
            fstab->recs[cnt].fs_type = strdup(fs_type);
            fstab->recs[cnt].length = 0;

            ++cnt;
        }
        else
        {
            LOGE("skipping malformed layout.txt line: %s\n", buffer);
        }
    }

    fclose(fstab_file);
    ret = fs_mgr_add_entry(fstab, "/tmp", "ramdisk", "ramdisk");
    if (ret < 0 )
    {
        LOGE("failed to add /tmp entry to fstab\n");
        fs_mgr_free_fstab(fstab);
        fstab = NULL;
        return -1;
    }

    ret = fs_mgr_add_entry(fstab, "/sdcard", "vfat", "/dev/block/mmcblk1p1");
	if (ret < 0){
		LOGE("fail to add /sdcard entry to fstab\n");
		fs_mgr_free_fstab(fstab);
		fstab = NULL;
		return -1;
	}
    printf("mount /dev/block/mmcblk1p1 to /sdcard done!\n");
    return 0;
}

static void free_dirent_namelist(struct dirent **namelist, int entries)
{
    int i;
    if(namelist == NULL)
        return;
    for(i = 0; i < entries; i++)
    {
        if(namelist[i])
            free(namelist[i]);
    }
    free(namelist);
    return;
}
int util_DelCrlf(char* str)
{
    char *find_cr_last = NULL;
    char *find_lf_last = NULL;
    do
    {
        find_cr_last = strrchr(str, '\r');
        find_lf_last = strrchr(str, '\n');
        if (find_cr_last == NULL && find_lf_last == NULL)
        {
            return 0;
        }
        if (find_cr_last != NULL)
        {
            *find_cr_last = '\0';
        }
        if (find_lf_last != NULL)
        {
            *find_lf_last = '\0';
        }
    }
    while(1);
}

int GetHitExternRecord(const char* command, int* hitedIdx)
{
    unsigned int i, matchlen=0, MaxMatchLen=0;
    if (hitedIdx == NULL)
    {
        LOGE("%s:Invalid arguments", __FUNCTION__);
        return -1;
    }
    *hitedIdx = -1;
    for (i=0; i < sizeof(storage_list)/sizeof(externStorage_Desc); i++)
    {
        matchlen = strlen(storage_list[i].mount_point) ;
        if (matchlen>0)
        {
            if (strstr(command, storage_list[i].mount_point) != NULL)
            {
                LOGD("%s vs %s",storage_list[i].mount_point, command);
                if (matchlen > MaxMatchLen)
                {
                    MaxMatchLen = matchlen;
                    *hitedIdx = i;
                }
            }
        }
    }
    if (*hitedIdx == -1)
        return -1;
    LOGI("Final Hit StorageList[%d] len:%d", *hitedIdx, MaxMatchLen);
    return 0;
}

int loadExternalLink()
{
    FILE* fp = fopen("/res/recovery.vold","r");
    char buffer[512] = {'\0'};
    unsigned int i,j;
    char* mount_dev ;
    char* mount_point;
#if 0
    if (fp == NULL)
    {
        return 0;
    }
    while(NULL != fgets(buffer, sizeof(buffer), fp))
    {
        for (i = 0; buffer[i] && isspace(buffer[i]); ++i);
        util_DelCrlf(buffer);
        mount_dev = strtok(buffer+i, " \t\n");
        mount_point = strtok(NULL, " \t\n");
        for (j=0; j< sizeof(storage_list)/sizeof(externStorage_Desc); j++)
        {
            if (strcmp(storage_list[j].storage_name, mount_dev) == 0)
            {
                sprintf(storage_list[j].mount_point,"%s", mount_point);
            }
        }
        LOGD("mount_dev:%s @ point %s\n", mount_dev, mount_point);
    }
    fclose(fp);
#else //for 1195 case.
    for (j=0; j< sizeof(storage_list)/sizeof(externStorage_Desc); j++)
    {
        if (strcmp(storage_list[j].storage_name, "sdcard") == 0)
        {
            sprintf(storage_list[j].mount_point,"/mnt/sdcard");
            sprintf(storage_list[j].hotplug_mntpah,"/mnt/sdcard");
        }
        else if (strcmp(storage_list[j].storage_name, "udisk") == 0)
        {
            sprintf(storage_list[j].mount_point,"/mnt/usb");
            sprintf(storage_list[j].hotplug_mntpah,"/mnt/usb");
        }
    }
    LOGD("mount_dev:%s @ point %s\n", mount_dev, mount_point);
#endif
    return 1;
}

int add_externDev_volume_table(const char *path)
{
    int entries, newVolCnt=0, i=0, n, actualEntries=0;
    int externDevID;
    int result = INSTALL_SUCCESS;
    struct dirent **namelist =  {0};
    char actCmd[256] = {'\0'};
    char usbMntPath[128] = {'\0'};

    char* prevP = NULL;
    char* nextSlash = NULL;

    char mntPath[128]= {'\0'};
    char mntDev[128]= {'\0'};
    char blockName[128] = {'\0'};

    if (loadExternalLink())
    {
        char prevPath[1024]= {'\0'};
        if (GetHitExternRecord(path, &externDevID)  == -1)
        {
            LOGE("%s (line%d): Shouldn't mis-match\n", __FUNCTION__, __LINE__);
            return -1;
        }

        if((entries = scandir(storage_list[externDevID].hotplug_mntpah, &namelist, 0,(int (*)(const dirent**, const dirent**))alphasort)) < 2 )
        {
            /* No sub-directory found. */
            LOGI("No Extern Storag  inserted!(%d)\n",entries);
            return INSTALL_ERROR_NOUSB;
        }

        // Test if the symlink has been created by loader_m
        if( access( path, F_OK ) != -1 )
        {
            // file exists means the link has been created
            if (storage_list[externDevID].type == ID_SDCARD)
            {
                // So far sdcard only support one part at Android AP level. So I put the ap mount path to be mount_path of volum table
                sprintf(mntPath, "%s",storage_list[externDevID].mount_point);
                sprintf(mntDev, "dev/block/mmcblk1p1");
            }
            else
            {
                // If usb case, it may be orig solution(only support 1usb1part  or multi-part/multi-usb )
                // We assume the mulit-part is use "sdx" in Android AP. However, if it doesnt, need customized!
                prevP = strstr(path, "sd");
                if (prevP)
                {
                    char* nextSlash = strstr(prevP,"/");
                    if (nextSlash)
                    {
                        sprintf(mntPath, "%s",nextSlash);
                    }
                    snprintf(blockName, nextSlash-prevP, "%s", prevP);
                    sprintf(mntDev,"dev/block/%s", blockName);
                }
                else
                {
                    // single partition case
                    sprintf(mntPath, "%s",storage_list[externDevID].mount_point);
                    for (i=0; i<entries; i++)
                    {
                        if(*(namelist[i]->d_name) == '.')
                        {
                            LOGI("[%d] skipped!\n", i);
                            continue;
                            //skip invisibles.
                        }
                        sprintf(blockName, "%s", namelist[i]->d_name );
                        sprintf(mntDev,"dev/block/%s", blockName);
                        break;
                    }
                }
            }
            goto start_addMntPath;
        }

        // The link has not been created case
        if (strcmp(storage_list[externDevID].hotplug_mntpah, storage_list[externDevID].mount_point) != 0)
        {
//			if (access( storage_list[externDevID].mount_point, F_OK )  != 0) {
            // before do link to mount point(ap view), we should check the upper folder existed
            prevP = strrchr(storage_list[externDevID].mount_point, '/');
            if (prevP)
            {
                snprintf(prevPath,   strlen(storage_list[externDevID].mount_point) - strlen(prevP) + 1, "%s",storage_list[externDevID].mount_point);
                LOGI("prevPath:%s\n", prevPath);
                if (access( prevPath, F_OK ) != 0)
                {
                    memset(actCmd,0, sizeof(actCmd));
                    sprintf(actCmd,"mkdir -p %s", prevPath);
                    LOGI("do %s\n", actCmd);
                    system(actCmd);
                }
            }
            if (storage_list[externDevID].type == ID_USB)
            {
                prevP = strstr(path, "sd");
                if (prevP == NULL)
                {
                    //find external mount point infomation
                    nextSlash = (char*)path + strlen(storage_list[externDevID].mount_point) + 1;
                    for (i=0; i<entries; i++)
                    {
                        if(*(namelist[i]->d_name) == '.')
                        {
                            LOGI("[%d] skipped!\n", i);
                            continue;
                            //skip invisibles.
                        }
                        memset(&usbMntPath, '\0', sizeof(usbMntPath));
                        sprintf(usbMntPath, "%s/%s/%s",storage_list[externDevID].hotplug_mntpah , namelist[i]->d_name, nextSlash);
                        LOGI("Check Path:%s \n", usbMntPath);
                        if (access( usbMntPath, F_OK )  == 0)
                        {
                            // exist!
                            memset(&actCmd, '\0', sizeof(actCmd));
                            sprintf(actCmd,"ln -s %s/%s %s",storage_list[externDevID].hotplug_mntpah,namelist[i]->d_name, storage_list[externDevID].mount_point);
                            LOGI("do %s\n", actCmd);
                            system(actCmd);
                            sprintf(blockName, "%s", namelist[i]->d_name);

                            sprintf(mntPath,"%s", storage_list[externDevID].mount_point);
                            sprintf(mntDev,"dev/block/%s", blockName);
                            break;
                        }
                        else
                        {
                            LOGE("Can not find the mapping update file in any block!\n");
                            return INSTALL_CORRUPT;
                        }
                    }
                }
                else
                {
                    // double check the sdx is mapping with the ap level
                    nextSlash = strstr(prevP, "/");
                    for (i=0; i<entries; i++)
                    {
                        if(*(namelist[i]->d_name) == '.')
                        {
                            LOGI("[%d] skipped!\n", i);
                            continue;
                            //skip invisibles.
                        }
                        memset(&usbMntPath, '\0', sizeof(usbMntPath));
                        sprintf(usbMntPath, "%s/%s/%s",storage_list[externDevID].hotplug_mntpah , namelist[i]->d_name, nextSlash+1);
                        LOGI("Check Path:%s\n", usbMntPath);
                        if (access( usbMntPath, F_OK )  == 0)
                        {
                            // exist!
                            if (strstr(path, namelist[i]->d_name) != NULL)
                            {
                                LOGI(" The right mapping of USB\n");

                                sprintf(mntPath, "%s", storage_list[externDevID].mount_point);
                                sprintf(blockName, "%s", namelist[i]->d_name);
                                sprintf(mntDev, "dev/block/%s", blockName);
                            }
                            else
                            {
                                // different sdx, do link!
                                char tmpPath[128] = {'\0'};
                                snprintf(tmpPath,strlen(nextSlash), "%s", path );
                                memset(actCmd,0, sizeof(actCmd));
                                sprintf(actCmd,"mkdir -p %s", storage_list[externDevID].mount_point);
                                LOGI("do %s\n", actCmd);
                                system(actCmd);
                                memset(actCmd,0, sizeof(actCmd));
                                sprintf(actCmd,"ln -s %s/%s %s", storage_list[externDevID].hotplug_mntpah, namelist[i]->d_name, tmpPath);
                                LOGI("do %s\n", actCmd);
                                system(actCmd);

                                sprintf(mntPath,"%s", tmpPath);
                                sprintf(blockName, "%s", namelist[i]->d_name);
                                sprintf(mntDev, "dev/block/%s", blockName);
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                // sdcard case
                memset(actCmd,0, sizeof(actCmd));
                sprintf(actCmd,"ln -s %s/mmcblk1p1 %s", storage_list[externDevID].hotplug_mntpah, storage_list[externDevID].mount_point);
                LOGI("do %s\n", actCmd);
                system(actCmd);

                sprintf(mntPath, "%s", storage_list[externDevID].mount_point);
                sprintf(mntDev,"dev/block/mmcblk1p1");
            }
//			}
        }
    }
    else
    {
        LOGE("Can't get vold.recovery! There is something wrong!\n");
        return INSTALL_CORRUPT;
    }

start_addMntPath:
    LOGI("The Adding Entry mntPath:%s / mntDev:%s\n", mntPath, mntDev);
    fs_mgr_add_entry(fstab, mntPath, "vfat", mntDev);
    dump_volume_table();
    free_dirent_namelist(namelist, entries);
    return INSTALL_SUCCESS;
}

static const char* PERSISTENT_PATH = "/persistent";
/****** End of Extra Extension by rtk ************/

// Entended by rtk
void load_volume_table(char* filePath)
{
    int i;
    int ret;

    if (filePath)
        fstab = fs_mgr_read_fstab(filePath);
    else
        fstab = fs_mgr_read_fstab("/etc/recovery.fstab");

    if (!fstab)
    {
        LOGE("failed to read /etc/recovery.fstab\n");
        return;
    }

    ret = fs_mgr_add_entry(fstab, "/tmp", "ramdisk", "ramdisk");
    if (ret < 0 )
    {
        LOGE("failed to add /tmp entry to fstab\n");
        fs_mgr_free_fstab(fstab);
        fstab = NULL;
        return;
    }
    dump_volume_table();
}

Volume* volume_for_path(const char* path)
{
    return fs_mgr_get_entry_for_mount_point(fstab, path);
}

// Mount the volume specified by path at the given mount_point.
int ensure_path_mounted_at(const char* path, const char* mount_point) {
    Volume* v = volume_for_path(path);

    if (v == NULL)
    {
        LOGE("unknown volume for path [%s]\n", path);
        return -1;
    }

    if (strcmp(v->fs_type, "ramdisk") == 0)
    {
        // the ramdisk is always mounted.
        return 0;
    }
    if (v->length < 0)
    {
        // skip usb check
        return 0;
    }

    int result;
    result = scan_mounted_volumes();
    if (result < 0)
    {
        LOGE("failed to scan mounted volumes\n");
        return -1;
    }

    if (!mount_point) {
        mount_point = v->mount_point;
    }

    const MountedVolume* mv =
        find_mounted_volume_by_mount_point(mount_point);
    if (mv) {
        // volume is already mounted
        LOGI("volume is already mounted\n");
        return 0;
    }

    mkdir(mount_point, 0755);  // in case it doesn't already exist

    if (strcmp(v->fs_type, "yaffs2") == 0)
    {
        // mount an MTD partition as a YAFFS2 filesystem.
        mtd_scan_partitions();
        const MtdPartition* partition;
        partition = mtd_find_partition_by_name(v->blk_device);
        if (partition == NULL)
        {
            LOGE("failed to find \"%s\" partition to mount at \"%s\"\n",
                 v->blk_device, mount_point);
            return -1;
        }
        return mtd_mount_partition(partition, mount_point, v->fs_type, 0);
    } else if (strcmp(v->fs_type, "ext4") == 0 ||
               strcmp(v->fs_type, "squashfs") == 0 ||
               strcmp(v->fs_type, "vfat") == 0) {
        result = mount(v->blk_device, mount_point, v->fs_type,
                       v->flags, v->fs_options);
        if (result == 0) return 0;

        LOGE("failed to mount %s (%s)\n", mount_point, strerror(errno));
        return -1;
    }
    else if (strcmp(v->fs_type, "ubifs") == 0)
    {
        char cmd[256] = {0};
        char mtd_path[32] = {0};
        char ubi_path[32] = {0};
        int i;
        for (i=1; i<=20; i++)
        {
            snprintf(mtd_path, sizeof(mtd_path), "/dev/mtd/mtd%d", i);
            if (strcmp(v->blk_device, mtd_path) == 0)
            {
                snprintf(ubi_path, sizeof(ubi_path), "/dev/ubi%d_0", i-1);
                break;
            }
        }
        //printf("/sbin/busybox mount -t %s %s %s", v->fs_type, ubi_path, v->mount_point);
        snprintf(cmd, sizeof(cmd), "/sbin/busybox mount -t %s %s %s"
                 , v->fs_type
                 , ubi_path
                 , v->mount_point);
        if((system(cmd)) < 0)
        {
            LOGE("failed to mount %s\n", v->mount_point);
            return -1;
        }
        else
            return 0;
    }

    LOGE("unknown fs_type \"%s\" for %s\n", v->fs_type, mount_point);
    return -1;
}

int ensure_path_mounted(const char* path) {
    dump_volume_table();
    // Mount at the default mount point.
    return ensure_path_mounted_at(path, nullptr);
}

int ensure_path_unmounted(const char* path) {
    Volume* v = volume_for_path(path);
    if (v == NULL)
    {
        LOGE("unknown volume for path [%s]\n", path);
        return -1;
    }
    if (strcmp(v->fs_type, "ramdisk") == 0)
    {
        // the ramdisk is always mounted; you can't unmount it.
        return -1;
    }
    if (strcmp(v->mount_point, "/res") == 0)
    {
        // the res partition need to be mounted at this moment!
        return 0;
    }
    if (v->length <  0)
    {
        // the res partition need to be mounted at this moment!
        return 0;
    }


    int result;
    result = scan_mounted_volumes();
    if (result < 0)
    {
        LOGE("failed to scan mounted volumes\n");
        return -1;
    }

    const MountedVolume* mv =
        find_mounted_volume_by_mount_point(v->mount_point);
    if (mv == NULL)
    {
        // volume is already unmounted
        return 0;
    }

    return unmount_mounted_volume(mv);
}

static int exec_cmd(const char* path, char* const argv[]) {
    int status;
    pid_t child;
    if ((child = vfork()) == 0) {
        execv(path, argv);
        _exit(-1);
    }
    waitpid(child, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        LOGE("%s failed with status %d\n", path, WEXITSTATUS(status));
    }
    return WEXITSTATUS(status);
}

int format_volume(const char* volume, const char* directory) {
    Volume* v = volume_for_path(volume);
    if (v == NULL)
    {
        LOGE("unknown volume \"%s\"\n", volume);
        return -1;
    }
    if (strcmp(v->fs_type, "ramdisk") == 0)
    {
        // you can't format the ramdisk.
        LOGE("can't format_volume \"%s\"", volume);
        return -1;
    }
    if (strcmp(v->mount_point, volume) != 0)
    {
        LOGE("can't give path \"%s\" to format_volume\n", volume);
        return -1;
    }

    if (ensure_path_unmounted(volume) != 0)
    {
        LOGE("format_volume failed to unmount \"%s\"\n", v->mount_point);
        return -1;
    }

    if (strcmp(v->fs_type, "yaffs2") == 0 || strcmp(v->fs_type, "mtd") == 0)
    {
        mtd_scan_partitions();
        const MtdPartition* partition = mtd_find_partition_by_name(v->blk_device);
        if (partition == NULL)
        {
            LOGE("format_volume: no MTD partition \"%s\"\n", v->blk_device);
            return -1;
        }

        MtdWriteContext *write = mtd_write_partition(partition);
        if (write == NULL)
        {
            LOGW("format_volume: can't open MTD \"%s\"\n", v->blk_device);
            return -1;
        }
        else if (mtd_erase_blocks(write, -1) == (off_t) -1)
        {
            LOGW("format_volume: can't erase MTD \"%s\"\n", v->blk_device);
            mtd_write_close(write);
            return -1;
        }
        else if (mtd_write_close(write))
        {
            LOGW("format_volume: can't close MTD \"%s\"\n", v->blk_device);
            return -1;
        }
        return 0;
    }

    if (strcmp(v->fs_type, "ext4") == 0 || strcmp(v->fs_type, "f2fs") == 0) {
        // if there's a key_loc that looks like a path, it should be a
        // block device for storing encryption metadata.  wipe it too.
        if (v->key_loc != NULL && v->key_loc[0] == '/') {
            LOGI("wiping %s\n", v->key_loc);
            int fd = open(v->key_loc, O_WRONLY | O_CREAT, 0644);
            if (fd < 0) {
                LOGE("format_volume: failed to open %s\n", v->key_loc);
                return -1;
            }
            wipe_block_device(fd, get_file_size(fd));
            close(fd);
        }

        ssize_t length = 0;
        if (v->length != 0) {
            length = v->length;
        } else if (v->key_loc != NULL && strcmp(v->key_loc, "footer") == 0) {
            length = -CRYPT_FOOTER_OFFSET;
        }
        int result;
        if (strcmp(v->fs_type, "ext4") == 0) {
            result = make_ext4fs_directory(v->blk_device, length, volume, sehandle, directory);
        } else {   /* Has to be f2fs because we checked earlier. */
            if (v->key_loc != NULL && strcmp(v->key_loc, "footer") == 0 && length < 0) {
                LOGE("format_volume: crypt footer + negative length (%zd) not supported on %s\n", length, v->fs_type);
                return -1;
            }
            if (length < 0) {
                LOGE("format_volume: negative length (%zd) not supported on %s\n", length, v->fs_type);
                return -1;
            }
            char *num_sectors;
            if (asprintf(&num_sectors, "%zd", length / 512) <= 0) {
                LOGE("format_volume: failed to create %s command for %s\n", v->fs_type, v->blk_device);
                return -1;
            }
            const char *f2fs_path = "/sbin/mkfs.f2fs";
            const char* const f2fs_argv[] = {"mkfs.f2fs", "-t", "-d1", v->blk_device, num_sectors, NULL};

            result = exec_cmd(f2fs_path, (char* const*)f2fs_argv);
            free(num_sectors);
        }
        if (result != 0) {
            LOGE("format_volume: make %s failed on %s with %d(%s)\n", v->fs_type, v->blk_device, result, strerror(errno));
            return -1;
        }
        return 0;
    }

    if (strcmp(v->fs_type, "ubifs") == 0)
    {
        printf("ubifs Format start.\n");
        ensure_path_mounted(volume);
        char cmd[256] = {0};
        snprintf(cmd, sizeof(cmd), "/sbin/busybox rm -rf %s/*", v->mount_point);
        if((system(cmd)) < 0)
        {
            LOGE("Exec command ubifs Format fail\r\n");
            ensure_path_unmounted(volume);
            return -1;
        }
        ensure_path_unmounted(volume);
        return 0;
    }

    LOGE("format_volume: fs_type \"%s\" unsupported\n", v->fs_type);
    return -1;
}

int format_volume(const char* volume) {
    return format_volume(volume, NULL);
}

int setup_install_mounts() {
    if (fstab == NULL) {
        LOGE("can't set up install mounts: no fstab loaded\n");
        return -1;
    }

    for (int i = 0; i < fstab->num_entries; ++i)
    {
        Volume* v = fstab->recs + i;
        printf("Get v->fs_type:%s\n, v->mount_point:%s; dev:%s", v->fs_type,v->mount_point, v->blk_device);
        if (strcmp(v->mount_point, "/tmp") == 0 ||
            strcmp(v->mount_point, "/cache") == 0) {
            if (ensure_path_mounted(v->mount_point) != 0) {
                LOGE("failed to mount %s\n", v->mount_point);
                return -1;
            }

        } else {
            if (ensure_path_unmounted(v->mount_point) != 0) {
                LOGE("failed to unmount %s\n", v->mount_point);
//                return -1;
                return 0;
            }
        }
    }
    return 0;
}
