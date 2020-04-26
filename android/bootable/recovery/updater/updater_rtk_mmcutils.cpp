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

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "updater_rtk_mmcutils.h"
#include "updater_rtk_common.h"

static char BLOCK_DEV_PATH[][50]=
{
    "/dev/mmcblk0",			//EMMC  for linux
    "/dev/mmcblk1",
    "/dev/block/mmcblk0",	//EMMC  for android
    "/dev/block/mmcblk1",
    "/dev/block/mtdblockdisc", //NAND
    "/dev/mtdblockdisc",
    "/dev/mtdblock/mtdblockdisc",
    "/dev/mtdblock/disc",
    "/dev/mtdblock0",
    "/dev/block/mtdblock0",
    "/dev/mtdblock/0",
    "/dev/mtdblock/mtdblock0"
};

static char MTD_CHAR_DEV_PATH[][20]=
{
    "/dev/mtd/disc",
    "/dev/mtd/mtddisc",
    "/dev/mtddisc",
    "/dev/mtd/mtd0",
    "/dev/mtd0",
    "/dev/mtd0ro",
    "/dev/mtd/0"
};

static char* pblock_name = NULL;

int rtk_open_block_dev(char** ppstr)
{
    int i;
    int dev_fd;

    i = 0;
    do
    {
        for( ; i < (int)(sizeof(BLOCK_DEV_PATH)/sizeof(BLOCK_DEV_PATH[0])); i++)
        {
            dev_fd = open(BLOCK_DEV_PATH[i], O_RDWR|O_SYNC);
            if(-1!=dev_fd) break;
        }
        if(dev_fd < 0)
        {
            install_debug("open block_dev fail\r\n");
            return -1;
        }
        install_info("Open BLOCK_DEV %s\r\n", BLOCK_DEV_PATH[i]);

        // device path for SD card is /dev/mmcblkX, same as MMC.
        // check if it is MMC or SD card..
        const char *blkptr = strstr(BLOCK_DEV_PATH[i], "mmcblk");
        if( blkptr )
        {
            int fd;
            char path[64], buf[16];
            memset( path, 0, sizeof(path) );
            sprintf(path, "/sys/block/mmcblk%c/device/type", *(blkptr+strlen("mmcblk")));
            install_debug("path(%s)\n", path);
            fd = open(path, O_RDONLY);
            if( fd != -1 )
            {
                memset(buf, 0, sizeof(buf));
                read(fd, buf, sizeof(buf) );
                close(fd);
                install_debug("path(%s) string(%s)\n", path, buf);
                if( strncmp(buf, "MMC",strlen("MMC"))==0 )
                {
                    break;
                }
                else
                {
                    close(dev_fd);
                    i++;
                }
            }
            else
            {
                install_debug("open path(%s) failed\n", path);
                close(dev_fd);
                i++;
            }
        }
        else
        {
            break;
        }
    }
    while( i < (int)(sizeof(BLOCK_DEV_PATH)/sizeof(BLOCK_DEV_PATH[0]))  );

    if( i < (int)(sizeof(BLOCK_DEV_PATH)/sizeof(BLOCK_DEV_PATH[0])) )
    {
        if(NULL != ppstr) *ppstr = BLOCK_DEV_PATH[i];
        pblock_name = BLOCK_DEV_PATH[i];
        return dev_fd;
    }

    return -1;
}

int rtk_open_mtd_char(char** ppstr)
{
    int i;
    int dev_fd;
    for(i = 0; i < (int)(sizeof(MTD_CHAR_DEV_PATH)/sizeof(MTD_CHAR_DEV_PATH[0])); i++)
    {
        dev_fd = open(MTD_CHAR_DEV_PATH[i], O_RDWR);
        if(-1!=dev_fd) break;
    }
    if(dev_fd < 0)
    {
        install_debug("open mtd char fail\r\n");
        return -1;
    }
    install_info("Open MTD_CHAR %s\r\n", MTD_CHAR_DEV_PATH[i]);
    if(NULL != ppstr) *ppstr = MTD_CHAR_DEV_PATH[i];
    return dev_fd;
}

void rtk_close_block_dev(int fd)
{
    close(fd);
}

int rtk_mmc_write(unsigned int offset, char *data, int size)
{
    int dev_fd;

    if (pblock_name == NULL)
        return -1;

    dev_fd = open(pblock_name, O_RDWR|O_SYNC);
    if(dev_fd < 0)
    {
        install_debug("open mmc block fail\r\n");
        return -1;
    }

    lseek(dev_fd, offset*512, SEEK_SET);
    write(dev_fd, data, size*512);
    close(dev_fd);

    return 0;
}

int rtk_get_mbr(unsigned int offset)
{
#if (1)	//Chuck TODO
    return 0;
#else
    char command[128];
    int ret=0;
    int dev_fd;
    FILE *writer;
    char data[512];

    if (pblock_name == NULL)
        return -1;

#if(0)
    memset(data, 0, sizeof(data));

    dev_fd = open(pblock_name, O_RDWR|O_SYNC);
    if(dev_fd < 0)
    {
        install_debug("open mmc block fail\r\n");
        return -1;
    }

    cmd.write_flag= MMC_READ_CMD;
    cmd.opcode = MMC_AUTO_MODE;
    cmd.arg = offset;
    cmd.blksz = 0x200;
    cmd.blocks = 1;
    cmd.data_ptr = (unsigned long)data;
    printf("rtk_get_mbr.....\n");
    ioctl(MMC_IOC_CMD, (long unsigned int)&cmd);
    close(dev_fd);

    writer = fopen(MBR_FILE_TEMP, "wb");
    if (writer == NULL)
    {
        install_debug("read %s failed\r\n", MBR_FILE_TEMP);
        return -1;
    }
    fwrite(data, 1, 512, writer);
    fclose(writer);

#else
    sprintf(command, "dd if=%s of=%s skip=%d bs=512 count=1", pblock_name, MBR_FILE_TEMP, offset);
    ret=rtk_command(command, __LINE__, __FILE__);
#endif
    return ret;
#endif
}

int rtk_set_mbr(unsigned int blkoffset, char *mbr)
{
    int ret=0, dev_fd;
    off64_t off = (off64_t)blkoffset*512;;

    if (pblock_name == NULL)
        return -1;

    // re-read the partition table, and trigger hotplug to cread related device nodes
    dev_fd = open(pblock_name, O_RDWR);
    if (dev_fd < 0)
    {
        install_debug("open %s block fail\r\n", BLOCK_DEV_PATH[0]);
        return -1;
    }

    // lseek() is not enough, and we need lseek64()
    ret = lseek64(dev_fd, off, SEEK_SET);
    if (ret == (off64_t) -1)
        install_err("lseek %x failed\r\n", blkoffset*512);

    write(dev_fd, mbr, 512);

    ioctl(dev_fd, BLKRRPART);
    close(dev_fd);

    return ret;
}

unsigned long rtk_get_size_emmc(void)
{
    int fd;
    unsigned long long v64;
    unsigned long longsectors;

    if (pblock_name == NULL)
        return -1;

    fd = open(pblock_name, O_RDWR|O_SYNC);
    if(fd < 0)
    {
        install_err("open mmc block %s fail\r\n", pblock_name);
        return -1;
    }

    if (ioctl(fd, BLKGETSIZE64, &v64) == 0)
    {
        /* Got bytes, convert to 512 byte sectors */
        v64 >>= 9;
        if (v64 != (unsigned long)v64)
        {

        ret_trunc:
            /* Not only DOS, but all other partition tables
             * we support can't record more than 32 bit
             * sector counts or offsets
             */
            install_err("device has more than 2^32 sectors, can't use all of them");
            v64 = (unsigned long)-1L;
        }
        close(fd);
        return v64;
    }
    /* Needs temp of type long */
    if (ioctl(fd, BLKGETSIZE, &longsectors))
    {
        /* Perhaps this is a disk image */
        unsigned long sz = lseek(fd, 0, SEEK_END);
        longsectors = 0;
        if (sz > 0)
            longsectors = (unsigned long)sz / 512;
        lseek(fd, 0, SEEK_SET);
    }
    if (sizeof(long) > sizeof(unsigned long)
            && longsectors != (unsigned long)longsectors
       )
    {
        goto ret_trunc;
    }
    close(fd);
    return longsectors;
}

