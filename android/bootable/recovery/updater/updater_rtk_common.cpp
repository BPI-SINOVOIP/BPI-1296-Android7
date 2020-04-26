/*
 * Copyright (C) 2009 The Android Open Source Project
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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "include_rtk.h"
#include "updater_rtk_common.h"

#define _128K_BYTE	128*1024U
#define _512K_BYTE	512*1024U
#define   _2M_BYTE	2*1024*1024U
#define   _4M_BYTE	4*1024*1024U
#define   _8M_BYTE	8*1024*1024U

#define UIO_DEV "/dev/uio250"
#define RBUS_MAP_INFO   "/sys/class/uio/uio250/maps/map0"

static char sys_param[256];

inline unsigned long long SIZE_ALIGN(unsigned long long len, unsigned long long size)
{
    return (((len - 1) & ~((size) - 1)) + size);
}

int set_system_param(struct t_rtkimgdesc* prtkimg)
{
    unsigned int bootCodeStart, reservedSize, factorySize, ubootSize;
    char *flashType;

    switch( prtkimg->flash_type )
    {
        case MTD_NANDFLASH:
            flashType = "nand";
            bootCodeStart = 0;
            ubootSize = _512K_BYTE;
            factorySize = _8M_BYTE;
            reservedSize = SIZE_ALIGN(ubootSize, prtkimg->mtd_erasesize)*4;
            reservedSize += SIZE_ALIGN(factorySize, prtkimg->mtd_erasesize);
            reservedSize += (6+1*4) * prtkimg->mtd_erasesize; //NF profile+BBT=6blocks, hwsetting=1block
            // add extra 20% space for safety.
            reservedSize = SIZE_ALIGN(reservedSize*1.2, prtkimg->mtd_erasesize);
            break;

#ifdef EMMC_SUPPORT
        case MTD_EMMC:
            flashType = "emmc";
            bootCodeStart = 0x20000;
            reservedSize = 0x620000;
            factorySize = _4M_BYTE;
            break;
#endif
        default:
            install_info("Non-supported flash type\n");
            return -1;
    }

    sprintf(sys_param, "boot_flash=%s bootcode_start=0x%x bootcode_size=0x%x "
            "factory_start=0x%x factory_size=0x%x",
            flashType, bootCodeStart, reservedSize-factorySize,
            reservedSize-factorySize, factorySize);

    install_info("sys_param(%s)\n", sys_param);
    return 0;
}

int get_parameter_value(const char* keystring, unsigned int *keyval)
{
    static int is_read_file = 0;
    static char sys_parameters[256];
    char sys_para_tmp[256] = {0};
    int ret;
    char *ptr, *ptr1;
    const char *sep = ": \t\n";

#ifdef USING_SYSTEM_FILE
    if(is_read_file == 0)
    {
        memset(sys_parameters, 0 , sizeof(sys_parameters));
        ret = rtk_file_to_string(SYSTEM_PARAMETERS, sys_parameters);
        if(ret < 0)
        {
            install_debug("Can not read system parameters(%s).\n", sys_parameters);
            return -1;
        }
        is_read_file = 1;
    }
#else
    strcpy(sys_parameters, sys_param);
#endif
    // find keystring
    //install_info("system_para:[%s]\r\n", sys_parameters);
    ptr = strstr(sys_parameters, keystring);
    if(!ptr)
    {
        install_err("Can't find string(%s)\r\n", keystring);
        return -1;
    }

    sprintf(sys_para_tmp, "%s", sys_parameters);
    ptr = strtok(sys_para_tmp, sep);
    while(ptr)
    {
        ptr = strtok(NULL, sep);
        if(!ptr)
            continue;

        if(strstr(ptr, keystring))
        {
            ptr1 = strstr(ptr, "=");
            ptr1++;
            *keyval = strtol(ptr1, &ptr, 16);
            install_debug("Got system_parameter: %s 0x%x\r\n", keystring, *keyval);
        }
    }
    return 0;
}

int get_parameter_string(const char* keystring, char *string, int str_maxlen)
{
    static int is_read_file = 0;
    static char sys_parameters[2048];
    char sys_para_tmp[256] = {0};
    int ret;
    char *ptr, *ptr1, *ptr2;
    const char *sep = ": \t\n";
    int length = 0;

    if (string == NULL) return -1;
    if (str_maxlen == 0) return -1;

    if(is_read_file == 0)
    {
        memset(sys_parameters, 0 , sizeof(sys_parameters));
        ret = rtk_file_to_string(SYSTEM_PARAMETERS, sys_parameters);
        if(ret < 0)
        {
            install_debug("Can not read system parameters(%s).\n", sys_parameters);
            return -1;
        }
        is_read_file = 1;
    }

    // find keystring
    //install_info("system_para:[%s]\r\n", sys_parameters);
    ptr = strstr(sys_parameters, keystring);
    if(!ptr)
    {
        install_info("Can't find string(%s)\r\n", keystring);
        return -1;
    }

    sprintf(sys_para_tmp, "%s", sys_parameters);
    ptr = strtok(sys_para_tmp, sep);
    while(ptr)
    {
        ptr = strtok(NULL, sep);
        if(!ptr)
            continue;

        if(strstr(ptr, keystring))
        {
            ptr1 = strstr(ptr, "=");
            ptr1++;
            ptr2 = strtok(NULL, sep);
            if (ptr2)
            {
                length = ptr2 - ptr1;
            }
            else
            {
                length = strlen(ptr1);
            }
            //printf("ptr1 = %s\n", ptr1);
            string[0] = 0;
            if (length)
            {
                strcpy(string, ptr1);
            }
            //printf("string = %s\n", string);
            return 0;
        }
    }
    return -1;
}

int rtk_file_to_string(const char* path, char* str)
{

    FILE* filep = NULL;
    char cmd[128] = {0};
    int ret;
    snprintf(cmd, sizeof(cmd), "dd if=%s of=/tmp/str.txt", path);
    ret = rtk_command(cmd, __LINE__, __FILE__, 0);
    if(ret < 0)
    {
        install_debug("cmd fail\r\n");
        return -1;
    }
    filep = fopen("/tmp/str.txt", "r");
    if(filep == NULL)
    {
        install_log("Can't open (%s)\r\n", path);
        return -1;
    }

    if(NULL != fgets(str, 256, filep))
    {
        install_info("Got String(%s)\r\n", str);
    }
    fclose(filep);
    return 0;
}

int rtk_find_file_in_dir(const char *pdirpath, const char *pkeyword, char *filename, const int filename_maxlen)
{
    struct dirent *ventry = NULL;
    int found;

    DIR *dh = opendir(pdirpath);

    install_debug("[%s]\r\n", __func__);
    found = 0;
    while ((ventry = readdir(dh)) != NULL)
    {
        install_debug("%s\r\n", ventry->d_name);
        if (!strncmp(ventry->d_name, pkeyword, strlen(pkeyword)))
        {
            install_log("find %s in %s\r\n",ventry->d_name, pdirpath);
            snprintf(filename, filename_maxlen, "%s", ventry->d_name);
            found = 1;
        }
    }
    closedir(dh);

    if (found == 0)
    {
        install_err("error! file %s not found in %s\r\n", pkeyword, pdirpath);
        filename[0] = '\0';
        return -1;
    }
    return 0;
}

int rtk_get_size_of_file(const char* file_path, unsigned int *file_size)
{
    struct stat stat_buf;
    int ret = 0;
    if (file_path == NULL)
        return -1;
    memset(&stat_buf, 0, sizeof(struct stat));
    ret = stat(file_path, &stat_buf);
    if(ret < 0)
    {
        install_debug("Can't find %s\r\n", file_path);
        *file_size = 0;
        return -1;
    }
    *file_size = stat_buf.st_size;
    return 0;
}


int rtk_command(const char* command, int line, const char* file, int show)
{
    int ret;

    if(1 == show)
        install_info("Command:%s\r\n", command);
    else
        install_debug("Command:%s\r\n", command);

    ret = system(command);

    fflush(stdout);
    fflush(stderr);
    if(ret != 0 || WEXITSTATUS(ret))
    {
        install_debug("Can't exec command:%s, (line=%d) (file=%s) (ret=%d)\r\n", command, line, file, ret);
        return -1;
    }
    return 0;
}

unsigned long read_value(const char *entry)
{
    char fn[128];
    FILE *fd = NULL;
    unsigned int val = 0;

    snprintf(fn, 128, "%s/%s", RBUS_MAP_INFO, entry);
    fd = fopen(fn, "r");
    if(fd == NULL){
        install_debug("open file:%s\n", fn);
        exit(1);
    }
    if(fscanf(fd, "%x", &val) != 1){
        install_debug("failed to get value\n");
        exit(1);
    }

    fclose(fd);
    return val;
}

unsigned int get_chip_rev_id()
{
    int uio_fd;
    char uio_size_buf[16];
    void *mapBuf;

    unsigned int  *SB2_CHIP_INFO;
    unsigned long rbus_ofs = 0;
    unsigned long rbus_size = 0;

    uio_fd = open( UIO_DEV, O_RDWR | O_SYNC);
    if(uio_fd < 0) {
        install_debug("%s, open failed, uio_fd(%d)\n", __func__, uio_fd);
        return -1;
    }

    rbus_size = read_value("size");
    rbus_ofs  = read_value("offset");
    install_debug("rbus_ofs=%x.\n", rbus_ofs);
    install_debug("rbus_size=%x.\n", rbus_size);
    mapBuf = mmap(0, rbus_size, PROT_READ | PROT_WRITE, MAP_SHARED, uio_fd, rbus_ofs);
    close(uio_fd);

    if( mapBuf == MAP_FAILED ) {
        install_debug("%s, mmap failed\n", __func__);
        return -1;
    }

    SB2_CHIP_INFO = (unsigned int*) (mapBuf) + (0x1a204/sizeof(unsigned int));
    install_info("Start read SB2_CHIP_INFO....\n");

    unsigned int chipVer = REG32(SB2_CHIP_INFO);
    install_debug("Read done(rev.%x).\n", chipVer);
    munmap(mapBuf, rbus_size);

    return chipVer;
}

