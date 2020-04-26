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

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "cutils/misc.h"
#include "cutils/properties.h"
#include "edify/expr.h"
//#include "mincrypt/sha.h"
#include "minzip/DirUtil.h"
#include "minzip/Zip.h"
#include "updater.h"
#include "applypatch/applypatch.h"
#include "mcp_export_api.h"

#ifdef USE_EXT4
#include "make_ext4fs.h"
#endif

#include "install_rtk.h"
#include <sys/ioctl.h>

#define TIME_OUT 3.0
#define READ_BUFFER 1024

// spare_id, from uboot/examples/flash_writer_u/flashdev_n.h
/* block state */
#define BLOCK_BAD               0x00
#define BLOCK_HWSETTING         0X23
#define BLOCK_KEY_SIG		0x24
#define BLOCK_BOOTCODE          0x79
#define BLOCK_DATA              0x80
#define BLOCK_ENVPARAM_MAGICNO  0X81
#define BLOCK_FACTORY_SETTING   0X82
#define SECURE_FSBL_MAGIC_NUM   0x89        /* Identify secure boot loader in NAND flash spare area*/
#define SECURE_OS_MAGIC_NUM     0x68        /* Identify secure os loader in NAND flash spare area*/
#define BLOCK_OTHER_DATA	0xd0	// unknown data (user program into flash)
#define BLOCK_BBT		0xbb	// bad block table
#define BLOCK_CLEAN		0xff	// block is empty
#define BLOCK_UNDETERMINE	0x55	// block state is undetermined


struct t_FWDESC rtk_fw_list[]=
{
    {FW_RESCUE_DT      , "rescueDT"         , ""                        , 0x0},
#ifdef TEE_ENABLE
    {FW_TEE            , "tee"              , ""                        , 0x0},
    {FW_BL31           , "BL31"             , ""                        , 0x0},
#endif
    {FW_KERNEL_DT      , "kernelDT"         , ""                        , 0x0},
    {FW_RESCUE_ROOTFS  , "rescueRootFS"     , ""                        , 0x0},
    {FW_KERNEL_ROOTFS  , "kernelRootFS"     , ""                        , 0x0},
    {FW_KERNEL         , "linuxKernel"      , ""                        , 0x0},
    {FW_AUDIO          , "audioKernel"      , ""                        , 0x0},
#ifdef NAS_ENABLE
    {FW_NAS_KERNEL     , "NASrescueKernel"      , ""                    , 0x0},
    {FW_NAS_RESCUE_DT  , "NASrescueDT"          , ""                    , 0x0},
    {FW_NAS_RESCUE_ROOTFS, "NASrescueRootFS"    , ""                    , 0x0},
#endif

#ifdef NFLASH_LAOUT
    {FW_GOLD_KERNEL    , "GOLDKernel"       , ""                        , 0x0},
    {FW_GOLD_RESCUE_DT , "GOLDrescueDT"     , ""                        , 0x0},
    {FW_GOLD_RESCUE_ROOTFS, "GOLDRootFS"    , ""                        , 0x0},
    {FW_GOLD_AUDIO        , "GOLDaudio"     , ""                        , 0x0},
    {FW_UBOOT             , "UBOOT"         , ""                        , 0x0},
#endif
    {FW_AUDIO_BOOTFILE , "audioFile"        , ""                        , 0x0},
    {FW_IMAGE_BOOTFILE , "imageFile"        , ""                        , 0x0},
    {FW_VIDEO_BOOTFILE , "videoFile"        , ""                        , 0x0},
    {FW_AUDIO_CLOGO1 , "audioFile1"         , ""                        , 0x0},
    {FW_IMAGE_CLOGO1 , "imageFile1"         , ""                        , 0x0},
    {FW_VIDEO_CLOGO1 , "videoFile1"         , ""                        , 0x0},
    {FW_AUDIO_CLOGO2 , "audioFile2"         , ""                        , 0x0},
    {FW_IMAGE_CLOGO2 , "imageFile2"         , ""                        , 0x0},
    {FW_VIDEO_CLOGO2 , "videoFile2"         , ""                        , 0x0},
};

struct mtd_info_user
{
    uint8_t type;
    uint32_t flags;
    uint64_t size;   // Total size of the MTD
    uint32_t erasesize;
    uint32_t oobblock;  // Size of OOB blocks (e.g. 512)
    uint32_t oobsize;   // Amount of OOB data per block (e.g. 16)
    uint32_t ecctype;
    uint32_t eccsize;
};

void dump_rawdata(u8 *p, u32 len)
{
    u32 i;
    for(i=0; i<len; i++)
    {
        printf("%02x ", p[i]);
        if(0==(i+1)%16)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
}

void dump_fw_desc_table(fw_desc_table_t *p)
{
    u32 i;
    printf("[%s]\r\n", __func__);
    printf("%16s: "        , "signature");
    for (i = 0; i < 8; i++)
    {
        printf("%02x ", p->signature[i]);
    };
    printf("\r\n");
    printf("%16s: %08x\r\n", "checksum"     , p->checksum);
    printf("%16s: %02x\r\n", "version"      , p->version);
    printf("%16s: "        , "reserved");
    for (i = 0; i < 7; i++)
    {
        printf("%02x ", p->reserved[i]);
    };
    printf("\r\n");
    printf("%16s: %08x\r\n", "paddings"     , p->paddings);
    printf("%16s: %08x\r\n", "part_list_len", p->part_list_len);
    printf("%16s: %08x\r\n", "fw_list_len"  , p->fw_list_len);

    printf("raw dump:\r\n");
    dump_rawdata((u8 *)p, sizeof(fw_desc_table_t));
    return;
}

void dump_part_desc_entry_v2(part_desc_entry_v2_t *p)
{
    u32 i;
    printf("[%s]\r\n", __func__);
    printf("%16s: %02x\r\n", "type"     , p->type);
    printf("%16s: %02x\r\n", "ro"           , p->ro);
    printf("%16s: %02x\r\n", "reserved"     , p->reserved);
    printf("%16s: %08x\r\n", "length"       , p->length);
    printf("%16s: %02x\r\n", "fw_count"     , p->fw_count);
    printf("%16s: %02x\r\n", "fw_type"      , p->fw_type);
    printf("%16s: "        , "reserved_1");
    for (i = 0; i < 8; i++)
    {
        printf("%02x ", p->reserved_1[i]);
    };
    printf("\r\n");
    printf("%16s: "        , "mount_point");
    for (i = 0; i < 32; i++)
    {
        printf("%02x ", p->mount_point[i]);
    };
    printf("\r\n");

    printf("raw dump:\r\n");
    dump_rawdata((u8 *)p, sizeof(part_desc_entry_v2_t));
    return;


}
void dump_rtk_fw_desc_entry_v22(rtk_fw_desc_entry_v22_t *p)
{
    u32 i;
    printf("[%s]\r\n", __func__);
    printf("%16s: %02x\r\n", "type"         , p->type);
    printf("%16s: %02x\r\n", "reserved"     , p->reserved);
    printf("%16s: %02x\r\n", "lzma"         , p->lzma);
    printf("%16s: %02x\r\n", "ro"           , p->ro);
    printf("%16s: %08x\r\n", "version"      , p->version);
    printf("%16s: %08x\r\n", "target_addr"  , p->target_addr);
    printf("%16s: %08x\r\n", "offset"       , p->offset);
    printf("%16s: %08x\r\n", "length"       , p->length);
    printf("%16s: %08x\r\n", "paddings"     , p->paddings);
    //printf("%16s: %08x\r\n", "checksum"     , p->checksum);
    printf("SHA256 hash==>\n");
    for (i=0; i<32; i++){
        printf("%02x ",  p->sha_hash[i]);   
    }
    printf("\n");
    printf("%16s: "        , "reserved_1");
    for (i = 0; i < 6; i++)
    {
        printf("%02x ", p->reserved_1[i]);
    };
    printf("\r\n");
    printf("%16s: %08x\r\n", "act_size"     , p->act_size);
    printf("%16s: %02x\r\n", "part_num"     , p->part_num);
    printf("%16s: "        , "RSA_sign");
    for (i = 0; i < 256; i++)
    {
        printf("%02x ", p->RSA_sign[i]);
    };
    printf("\r\n");
    printf("%16s: "        , "reserveded");
    for (i = 0; i < 27; i++)
    {
        printf("%02x ", p->reserveded[i]);
    };
    printf("\r\n");

    printf("raw dump:\r\n");
    dump_rawdata((u8 *)p, sizeof(rtk_fw_desc_entry_v22_t));
    return;
}

u32 get_checksum(u8 *p, u32 len)
{
    u32 checksum = 0;
    u32 i;

    for(i = 0; i < len; i++)
    {
        checksum += *(p+i);
    }
    return checksum;
}

int isready(int fd)
{
    int rc;
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    rc = select(fd+1, &fds, NULL, NULL, &tv);
    //rc = select(fd+1, &fds, NULL, NULL, NULL);
    if (rc < 0)
        return -1;

    return FD_ISSET(fd, &fds) ? 1 : 0;
}

int memcmp_count(const char *pa, const char *pb, int size)
{
    int i, error_count;

    error_count = 0;
    for (i = 0; i < size; i++, pa++, pb++)
    {
        if (*pa != *pb) error_count++;
    }
    return error_count;
}

int rtk_file_checksum(char* sBuf, unsigned int imglen, unsigned char *checksum)
{
    unsigned char*  pBuff_src = NULL;
    unsigned long   PhyAddr_src;

    unsigned char*  pBuff_dst = NULL;
    unsigned long   PhyAddr_dst;
    int i = 0;

    ion_user_handle_t ionhdl_headr_src;
    ion_user_handle_t ionhdl_headr_dst;
    int mapfd_src = RTKIon_alloc(imglen, &pBuff_src, &PhyAddr_src, &ionhdl_headr_src);
    int mapfd_dst = RTKIon_alloc(SHA256_SIZE, &pBuff_dst, &PhyAddr_dst, &ionhdl_headr_dst);

    memcpy(pBuff_src, sBuf, imglen);
    RTKIon_flush(mapfd_src);
    SHA256_dohash(PhyAddr_src, imglen, PhyAddr_dst, NULL);

    RTKIon_invalidate(mapfd_dst);

#if 1
    memcpy(checksum, pBuff_dst, SHA256_SIZE);
#else
    *checksum = get_checksum(pBuffVirt_dst, SHA256_SIZE);
    printf("[OTA_D]: get_checksum(%#x)\n", *checksum);
#endif
    if(pBuff_src || pBuff_dst) {
        printf("Release the memory\n");
    RTKIon_free(pBuff_src, imglen, mapfd_src, ionhdl_headr_src);
    RTKIon_free(pBuff_dst, SHA256_SIZE, mapfd_dst, ionhdl_headr_dst);
    }

    printf("SHA256 checksum==>\n");
    for (i=0; i<32; i++){
        printf("%02x ", checksum[i]);
    }
    printf("\n");
    return 0;
}

int buf_to_fd(char* sBuf, int dfd, unsigned int imgSize, unsigned int length, unsigned int* pchecksum, FILE* ffd)
{
    int ret, rret, wret;
    unsigned int rlen;
    unsigned int len;
    unsigned int progress_len, interval;
    time_t stime, etime;
    char data_buf[2048*4] = {0};
    char* pCurWBuf = sBuf;
    int restart;
    time_t start_time=time(NULL), end_time;
    stime = time(NULL);
    interval = length/10;
    progress_len = 0;
    len = 0;
    printf(" ========== Enter buf_to_fd ========== \n");

    unsigned int  checkSum = 0;
    if(pchecksum != NULL)
        *pchecksum = 0;

    //progressbar(0, length, NULL);
    //fflush(stdout);
    unsigned int actRlen = 0;
    int cpSize = 0;
    while(len < length)
    {
        rlen = ((length - len) < sizeof(data_buf)) \
               ? (length - len): sizeof(data_buf);

        actRlen = imgSize - len;
        if (actRlen >= sizeof(data_buf))
        {
            cpSize = sizeof(data_buf);
        }
        else
        {
            cpSize = actRlen;
        }
        //test 0424 memcpy(&data_buf, pCurWBuf, cpSize);
        memcpy(&data_buf, pCurWBuf, rlen);

        if(ffd == NULL)
        {
            wret = write(dfd, (void*) data_buf, rlen);
            //test0424wret = write(dfd, (void*) data_buf, cpSize);
            //wret = write(dfd, (void*) pCurWBuf, rlen);
            //wret = rlen;
            //install_debug("ffd is NULL, go write\r\n");

        }
        else
        {
            wret = fwrite(data_buf, 1, rlen, ffd);
            //test 0424 wret = fwrite(data_buf, 1, cpSize, ffd);
            //wret = rlen;
            // install_debug("ffd isn't NULL, go fwrite\r\n");

        }

        sync();

        if(wret < 0)
        {
            printf("write fail return (%d)\r\n", wret);
            goto dd_end;
        }

        if(/*cpSize*/rlen!=wret)
        {
            printf("cpSize(rlen) = %d, wret = %d\r\n", rlen/*cpSize*/, wret);
            goto dd_end;
        }
        if(pchecksum != NULL)
        {
            *pchecksum += get_checksum((u8 *)pCurWBuf, (u32) rlen);
            checkSum += get_checksum((u8 *)data_buf, (u32) rlen);
        }
        len = len + rlen;
        if(len >= progress_len)
        {
            // install_debug("now len:%d, length:%d\r\n", len, progress_len);
            //progressbar(len, length, NULL);
            //fflush(stdout);
            progress_len = progress_len + interval;
        }
        pCurWBuf += rlen;
    }
dd_end:
    etime = time(NULL);
    if(pchecksum != NULL)
    {
        printf("checksum:0x%08X (double checksum:0x%08X)\r\n\r\n", *pchecksum,checkSum);
    }

    //install_debug("\r\ntotal_time:%.0lf seconds\r\n", difftime(etime,stime));
    return len;
}


int fd_to_fd(int sfd, int dfd, unsigned int length, unsigned int* pchecksum, FILE* ffd)
{
    int ret, rret, wret;
    unsigned int rlen;
    unsigned int len;
    unsigned int progress_len, interval;
    char data_buf[2048*4] = {0};
    time_t stime, etime;
    int restart;
    time_t start_time=time(NULL), end_time;
    stime = time(NULL);
    interval = length/10;
    progress_len = 0;
    len = 0;
    if(pchecksum != NULL)
        *pchecksum = 0;

    restart = 1;
    //progressbar(0, length, NULL);
    //fflush(stdout);
    while(len < length)
    {
        rlen = ((length - len) < sizeof(data_buf)) \
               ? (length - len): sizeof(data_buf);
        ret = isready(sfd);
        // select error
        if(ret < 0)
        {
            install_debug("select error\r\n");
            goto dd_end;
        }
        if(ret == 0)
        {
            if(restart == 1)
            {
                install_debug("first timeout\r\n");
                start_time = time(NULL);
                restart = 0;
            }
            else
            {
                install_debug("second timeout\r\n");
                end_time = time(NULL);
                if(difftime(end_time, start_time) > TIME_OUT)
                {
                    goto dd_end;
                }
            }
            continue;
        }

        restart = 1;

        rret = read(sfd, (void*) data_buf, rlen);
        if(rret < 0)
        {
            install_debug("read source fd fail\r\n");
            goto dd_end;
        }
        else if(rret == 0)
        {
            install_debug("read source fd close\r\n");
            goto dd_end;
        }

        if(ffd == NULL)
        {
            wret = write(dfd, (void*) data_buf, rret);
        }
        else
        {
            wret = fwrite(data_buf, 1, rret, ffd);
        }

        sync();
        if(wret < 0)
        {
            install_debug("write fail\r\n");
            goto dd_end;
        }
        if(rret!=wret)
        {
            install_debug("rret = %d, wret = %d\r\n", rret, wret);
            goto dd_end;
        }
        if(pchecksum != NULL)
        {
            *pchecksum += get_checksum((u8 *)data_buf, (u32) wret);
            //*pchecksum += get_checksum((u8 *)data_buf, (u32) rret);
        }
        len = len + rret;
        if(len >= progress_len)
        {
            //install_debug("now len:%d, length:%d\r\n", len, progress_len);
            //progressbar(len, length, NULL);
            //fflush(stdout);
            progress_len = progress_len + interval;
        }
    }
dd_end:
    etime = time(NULL);
    if(pchecksum != NULL)
    {
        printf("checksum:0x%08X\r\n\r\n", *pchecksum);
    }

    //install_debug("\r\ntotal_time:%.0lf seconds\r\n", difftime(etime,stime));
    return len;
}

int file_to_flash(const char* filepath, unsigned long long soffset, const char* devpath, unsigned long long doffset, unsigned int len, unsigned int* checksum)
{
    int sfd, dfd;
    int ret;
    sfd = open(filepath, O_RDONLY);
    if(sfd<0)
    {
        install_err("Can't open %s\r\n", filepath);
        return -1;
    }
    dfd = open(devpath, O_RDWR|O_SYNC);
    if(dfd<0)
    {
        install_err("Can't open %s\r\n", devpath);
        return -1;
    }
    ret = lseek64(sfd, soffset, SEEK_SET);
    if (ret == (off64_t) -1)
    {
        install_err("lseek (%s) fail\r\n", filepath);
        close(sfd);
        close(dfd);
        return -1;
    }
    ret = lseek64(dfd, doffset, SEEK_SET);
    if (ret == (off64_t) -1)
    {
        install_err("lseek (%s) fail\r\n", devpath);
        close(sfd);
        close(dfd);
        return -1;
    }

    ret = fd_to_fd(sfd, dfd, len, checksum, NULL);
    close(sfd);
    close(dfd);

    if((unsigned int)ret != len)
    {
        install_err("fd_to_fd fail(%d)\r\n", ret);
        return -1;
    }
    return ret;
}

int ptr_to_flash(void* ptr, unsigned int len, const char* devpath, unsigned int doffset)
{
    int dfd, ret;
    dfd = open(devpath, O_RDWR|O_SYNC);
    if(dfd<0)
    {
        install_err("Can't open %s\r\n", devpath);
    }
    ret = lseek(dfd, doffset, SEEK_SET);
    if(ret < 0)
    {
        install_err("lseek (%s) fail\r\n", devpath);
        close(dfd);
        return -1;
    }

    ret = write(dfd, ptr, len);
    close(dfd);
    if((unsigned int)ret != len)
    {
        install_err("ret(%d)!=len(%u)\r\n", ret, len);
        return -1;
    }
    return 0;
}

int file_verify(const char* sfilepath, unsigned int soffset, const char* dfilepath, unsigned int doffset, unsigned int imglen, unsigned int *err_count)
{
    char sbuffer[READ_BUFFER] = {0};
    char dbuffer[READ_BUFFER] = {0};

    int vtempsread, vtempdread, error_count, i;
    unsigned int vslength, read_buffer, schecksum, dchecksum;
    FILE *vsfp, *vdfp;

    vsfp = fopen(sfilepath, "rb");
    vdfp = fopen(dfilepath, "rb");

    error_count = i = 0;
    schecksum = dchecksum = 0;

    if (vsfp == NULL)
    {
        install_err("cannot open %s!\r\n", sfilepath);
        return -1;
    }
    if (vdfp == NULL)
    {
        install_err("cannot open %s!\r\n", dfilepath);
        fclose(vsfp);
        return -1;
    }

    if (fseek(vsfp, soffset, SEEK_SET) < 0)
    {
        install_err("%s seek error!\r\n", sfilepath);
        fclose(vsfp);
        fclose(vdfp);
        return -1;
    }

    if (fseek(vdfp, doffset, SEEK_SET) < 0)
    {
        install_err("%s seek error!\r\n", dfilepath);
        fclose(vsfp);
        fclose(vdfp);
        return -1;
    }

    vslength = imglen;

    install_info("Verifying\r\n");

    do
    {
        read_buffer = (vslength > READ_BUFFER)? READ_BUFFER:vslength;
        vtempsread = fread(sbuffer, 1, read_buffer, vsfp);
        vtempdread = fread(dbuffer, 1, read_buffer, vdfp);
        schecksum += get_checksum((u8 *)sbuffer, (u32) vtempsread);
        dchecksum += get_checksum((u8 *)dbuffer, (u32) vtempdread);

        error_count += memcmp_count(sbuffer, dbuffer, vtempsread);

        vslength -= vtempsread;

        fflush(stdout);
    }
    while (!feof(vsfp) && !feof(vdfp) && vslength > 0);

    fclose(vsfp);
    fclose(vdfp);

    *err_count = error_count;

    if (error_count > 0)
    {
        install_err("Fail! total %u bytes checked, %d bytes error\r\n", imglen, error_count);
    }
    else
    {
        install_info("Pass! total %u bytes checked\r\n", imglen);
    }

    if (schecksum != dchecksum)
    {
        install_err("Checksum: src=0x%08X dst=0x%08X\r\n", schecksum, dchecksum);
        return -1;
    }

    return 0;
}

int ptr_verify(const char* sfilepath, unsigned int soffset, const char* dmemory, unsigned int doffset, unsigned int imglen, unsigned int *err_count)
{
    char sbuffer[READ_BUFFER] = {0};
    int vtempsread, error_count, i;
    unsigned int vslength, read_buffer, schecksum, dchecksum;
    FILE *vsfp;
    vsfp = fopen(sfilepath, "rb");

    error_count = i = 0;
    schecksum = dchecksum = 0;
    if (vsfp == NULL)
    {
        install_err("cannot open %s!\r\n", sfilepath);
        return -1;
    }

    if (fseek(vsfp, soffset, SEEK_SET) < 0)
    {
        install_err("%s seek error!\r\n", sfilepath);
        fclose(vsfp);
        return -1;
    }

    dmemory += doffset;
    vslength = imglen;

    install_info("[Verifying]\r\n");

    do
    {
        read_buffer = (vslength > READ_BUFFER)? READ_BUFFER:vslength;
        vtempsread = fread(sbuffer, 1, read_buffer, vsfp);
        schecksum += get_checksum((u8 *)sbuffer, (u32) vtempsread);
        dchecksum += get_checksum((u8 *)dmemory, (u32) vtempsread);
        error_count += memcmp_count(sbuffer, dmemory, vtempsread);

        dmemory += vtempsread;
        vslength-=vtempsread;
    }
    while (!feof(vsfp) && vslength > 0);

    fclose(vsfp);
    *err_count = error_count;

    if (error_count > 0)
    {
        install_err("Fail! total %u bytes checked, %d bytes error\r\n", imglen, error_count);
    }
    else
    {
        install_info("Pass! total %u bytes checked\r\n", imglen);
    }

    if (schecksum != dchecksum)
    {
        install_err("Checksum: src=0x%08X dst=0x%08X\r\n", schecksum, dchecksum);
        return -1;
    }
    return 0;
}

/*
**  Following code Copied from RTK_Config.cpp
*/
static inline char* skip_char(char* str, char c)
{
    do
    {
        if(*str==0) return NULL;
        if(*str==c) return str+1;
        str++;
    }
    while(1);
    return NULL;
}

static inline char* skip_space(char* str)
{
    do
    {
        if(*str==0) return NULL;
        if(*str != ' ') return str;
        str++;
    }
    while(1);
    return NULL;
}

static inline char* skip_tab_space(char* str)
{
    do
    {
        if(*str==0) return NULL;
        if(*str != 0x09 && *str != 0x20) return str;
        str++;
    }
    while(1);
    return NULL;
}

static inline int skip_quotation(char *str, char **string, int *string_len)
{
    char *find_first = NULL, *find_last = NULL;

    find_first = strchr(str, '\"');
    find_last = strrchr(str, '\"');

    if (find_first != NULL && find_last != NULL && find_first != find_last)
    {
        *string = find_first + 1;
        *string_len = find_last - find_first - 1;
        return 0;
    }
    else
    {
        *string = str;
        *string_len = strlen(str);
        return -1;
    }
}

/*
**  Above code Copied from RTK_Config.cpp
*/
struct t_FWDESC* find_fw_by_efwtype(int efwtype)
{
    int i;
    for(i = 0; i < (int)(sizeof(rtk_fw_list)/sizeof(struct t_FWDESC)); i++)
        if(efwtype == rtk_fw_list[i].efwtype)
            return &rtk_fw_list[i];
    return NULL;
}

struct t_FWDESC* find_fw_by_fw_name(const char* fw_name)
{
    int i;
    // return element directly if exist
    for(i = 0; i < (int)(sizeof(rtk_fw_list)/sizeof(struct t_FWDESC)); i++)
        if(0 == strcmp(rtk_fw_list[i].firmware_name, fw_name))
            return &rtk_fw_list[i];
    return NULL;
}

const char* inv_by_fwtype(int efwtype)
{
    int i;
    // firmware name
    for(i = 0; i < (int)(sizeof(rtk_fw_list)/sizeof(struct t_FWDESC)); i++)
        if(efwtype == rtk_fw_list[i].efwtype)
            return rtk_fw_list[i].firmware_name;
#if 0
    // so far, OTA not consider partition
    // partition name
    for(i = 0; i < (int)(sizeof(rtk_part_list)/sizeof(struct t_PARTDESC)); i++)
        if(efwtype == rtk_part_list[i].efwtype && strlen(rtk_part_list[i].partition_name) != 0)
            return rtk_part_list[i].partition_name;
#endif
    switch(efwtype)
    {
        case FW_BOOTCODE:
            return "bootcode";
        case FW_FACTORY:
            return "factory";
        case FW_FW_TBL:
            return "fw table";
        case FW_RESCUE:
            return "Rescue";
        case P_SROOTFS:
            return "srootfs";
        case FW_AUDIO_CLOGO1:
            return "caLogo1";
        case FW_VIDEO_CLOGO1:
            return "cvLogo1";
        case FW_AUDIO_CLOGO2:
            return "caLogo2";
        case FW_VIDEO_CLOGO2:
            return "cvLogo2";
        case FW_P_FREE_SPACE:
            return "FREE SPACE";
        case FW_P_SSU_WORKING:
            return "SSU WORKING";
        default:
            return "Unkonwn";
    }
    return "Unkonwn";
}

bool getExtractFileinZip(ZipArchive* za, char* targetFilePath, char* extractPath)
{
    bool ret;
    if (za == NULL || targetFilePath== NULL || extractPath == NULL)
        return -1;

    const ZipEntry* entry = mzFindZipEntry(za, targetFilePath);
    if (entry == NULL)
    {
        install_err("No %s in package\n",targetFilePath);
        return -1;
    }

    FILE* f = fopen(extractPath, "wb");
    if (f == NULL)
    {
        install_err("can't open %s for write: %s\n",extractPath,targetFilePath);
        return -1;
    }
    ret = mzExtractZipEntryToFile(za, entry, fileno(f));
    fclose(f);

    return ret;
}

int add_install_factory_command(char* str, struct t_rtkimgdesc* prtkimg)
{
    char *str_tmp = NULL;
    int str_len = 0;

    if (0 == strncmp("ifcmd0", str, strlen("ifcmd0")))
    {
        skip_quotation(skip_char(str, '='), &str_tmp, &str_len);
        strncpy(prtkimg->ifcmd0, str_tmp, str_len);
        prtkimg->ifcmd0[str_len] = '\0';
        install_log("Add ifcmd0(%d): \"%s\"\r\n", strlen(prtkimg->ifcmd0), prtkimg->ifcmd0);
    }
    else if (0 == strncmp("ifcmd1", str, strlen("ifcmd1")))
    {
        skip_quotation(skip_char(str, '='), &str_tmp, &str_len);
        strncpy(prtkimg->ifcmd1, str_tmp, str_len);
        prtkimg->ifcmd1[str_len] = '\0';
        install_log("Add ifcmd1(%d): \"%s\"\r\n", strlen(prtkimg->ifcmd1), prtkimg->ifcmd1);
    }
    return 0;
}

int add_firmware(char* str, CRTKImgDesc* prtkimg)
{
    struct t_FWDESC* rtk_fw;
    char firmware_name[128], filename[128];
    unsigned int target;
    // sanity-check
    if(str == NULL||*str==0)
        return -1;
    //printf("newline:%s", str);
    if(str[0] == ' ')
        str = skip_space(str);
    // firmware_name filename target
    sscanf(str, "%s %s %x", firmware_name, filename, &target);

    rtk_fw = find_fw_by_fw_name(firmware_name);
    // sanity-check
    if(rtk_fw == NULL)
    {
        install_err("Can't get firmare(%s)\r\n", str);
        return -1;
    }

    // show info
    install_info("Add firmware(%s), filename(%s), target(0x%08x)\r\n", firmware_name, filename, target);

    // fill rtk_fw
    sprintf(rtk_fw->firmware_name, "%s", firmware_name);
    sprintf(rtk_fw->filename, "%s", filename);
    rtk_fw->target = target;

    return 0;
}

int fill_rtk_fw(CRTKImgDesc* prtkimg, ZipArchive* za)
{
    int efwtype;
    struct t_FWDESC* rtk_fw;
    struct t_imgdesc* pimgdesc = NULL;

    char *dotpos = NULL;
    const ZipEntry* imgEntry = NULL;

    // firmware mem_offset from rtk_fw_list
    for(efwtype=FW_KERNEL; efwtype<FW_UNKNOWN; efwtype=efwtype+1)
    {
        if ((efwtype >= FW_AUDIO_CLOGO2) && (efwtype <= FW_SE_STORAGE))
            continue;
        rtk_fw = find_fw_by_efwtype(efwtype);
        if(rtk_fw == NULL)
            continue;

        // firmware target
        prtkimg->fw[efwtype].mem_offset = rtk_fw->target;
        snprintf(prtkimg->fw[efwtype].filename, sizeof(prtkimg->fw[efwtype].filename), "%s",rtk_fw->filename);

        // So far, we only support lzma
        if (strstr(prtkimg->fw[efwtype].filename, "lzma"))
            strcpy(prtkimg->fw[efwtype].compress_type, "lzma");
        else if (strlen(prtkimg->fw[efwtype].filename) != 0)
            strcpy(prtkimg->fw[efwtype].compress_type, "bin");

        // claire add
        imgEntry = mzFindZipEntry(za, prtkimg->fw[efwtype].filename);
        if (imgEntry == NULL)
        {
            install_err("No %s in package\n",prtkimg->fw[efwtype].filename);
        }
        else
        {
            prtkimg->fw[efwtype].img_size = mzGetZipEntryUncompLen(imgEntry);
        }

        install_info("rtk_fw->filename=%s\r\n", prtkimg->fw[efwtype].filename);
        install_info("prtkimg->fw[%d].compress_type=%s\r\n", efwtype, prtkimg->fw[efwtype].compress_type);
        install_info("prtkimg->fw[%d].img_size=%d\r\n", efwtype, prtkimg->fw[efwtype].img_size);

    }

    return 0;
}
int fill_rtk_install_av_count(struct t_rtkimgdesc* prtkimg)
{
    switch (prtkimg->install_avfile_count)
    {
        case 3:
            prtkimg->fw[FW_AUDIO_CLOGO2].mem_offset = CUSTOMER_AUDIO_LOGO_DEFAULT_TARGET_ADDRESS;
            if (prtkimg->fw[FW_AUDIO_CLOGO2].img_size == 0)
            {
                if (prtkimg->fw[FW_AUDIO_BOOTFILE].img_size != 0)
                {
                    memcpy(&prtkimg->fw[FW_AUDIO_CLOGO2], &prtkimg->fw[FW_AUDIO_BOOTFILE], sizeof(struct t_imgdesc));
                }
                if (prtkimg->install_avfile_audio_size != 0)
                {
                    prtkimg->fw[FW_AUDIO_CLOGO2].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->install_avfile_audio_size*1024, prtkimg->mtd_erasesize);
                }
            }
            prtkimg->fw[FW_VIDEO_CLOGO2].mem_offset = CUSTOMER_VIDEO_LOGO_DEFAULT_TARGET_ADDRESS;
            if (prtkimg->fw[FW_VIDEO_CLOGO2].img_size == 0)
            {
                if (prtkimg->fw[FW_VIDEO_BOOTFILE].img_size != 0)
                {
                    memcpy(&prtkimg->fw[FW_VIDEO_CLOGO2], &prtkimg->fw[FW_VIDEO_BOOTFILE], sizeof(struct t_imgdesc));
                }
                if (prtkimg->install_avfile_video_size != 0)
                {
                    prtkimg->fw[FW_VIDEO_CLOGO2].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->install_avfile_video_size*1024, prtkimg->mtd_erasesize);
                }
            }
        case 2:
            prtkimg->fw[FW_AUDIO_CLOGO1].mem_offset = CUSTOMER_AUDIO_LOGO_DEFAULT_TARGET_ADDRESS;
            if (prtkimg->fw[FW_AUDIO_CLOGO1].img_size == 0)
            {
                if (prtkimg->fw[FW_AUDIO_BOOTFILE].img_size != 0)
                {
                    memcpy(&prtkimg->fw[FW_AUDIO_CLOGO1], &prtkimg->fw[FW_AUDIO_BOOTFILE], sizeof(struct t_imgdesc));
                }
                if (prtkimg->install_avfile_audio_size != 0)
                {
                    prtkimg->fw[FW_AUDIO_CLOGO1].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->install_avfile_audio_size*1024, prtkimg->mtd_erasesize);
                }
            }
            prtkimg->fw[FW_VIDEO_CLOGO1].mem_offset = CUSTOMER_VIDEO_LOGO_DEFAULT_TARGET_ADDRESS;
            if (prtkimg->fw[FW_VIDEO_CLOGO1].img_size == 0)
            {
                if (prtkimg->fw[FW_VIDEO_BOOTFILE].img_size != 0)
                {
                    memcpy(&prtkimg->fw[FW_VIDEO_CLOGO1], &prtkimg->fw[FW_VIDEO_BOOTFILE], sizeof(struct t_imgdesc));
                }
                if (prtkimg->install_avfile_video_size != 0)
                {
                    prtkimg->fw[FW_VIDEO_CLOGO1].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->install_avfile_video_size*1024, prtkimg->mtd_erasesize);
                }
            }
            break;
        case 0:
            prtkimg->fw[FW_AUDIO_BOOTFILE].img_size = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_allo_size = 0;
            prtkimg->fw[FW_IMAGE_BOOTFILE].img_size = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size = 0;
            prtkimg->fw[FW_VIDEO_BOOTFILE].img_size = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_allo_size = 0;
            if (prtkimg->fw[FW_IMAGE_CLOGO1].img_size == 0)
            {
                prtkimg->fw[FW_IMAGE_CLOGO1].img_size = prtkimg->fw[FW_IMAGE_CLOGO1].flash_allo_size = 0;
            }
        case 1:
            prtkimg->fw[FW_AUDIO_CLOGO2].img_size = prtkimg->fw[FW_AUDIO_CLOGO2].flash_allo_size = 0;
            prtkimg->fw[FW_VIDEO_CLOGO2].img_size = prtkimg->fw[FW_VIDEO_CLOGO2].flash_allo_size = 0;
            prtkimg->fw[FW_AUDIO_CLOGO1].img_size = prtkimg->fw[FW_AUDIO_CLOGO1].flash_allo_size = 0;
            prtkimg->fw[FW_VIDEO_CLOGO1].img_size = prtkimg->fw[FW_VIDEO_CLOGO1].flash_allo_size = 0;
        default:
            break;
    }

    return 0;
}

int save_fwdesc_emmc(CRtkFWHeader* fw, CRTKImgDesc* prtkimg)
{
    int i, len=0;
    int dev_fd;
    int bytecnt=0;

    fw->fw_tab.checksum = 0;
    fw->fw_tab.part_list_len = fw->part_count*sizeof(part_desc_entry_v2_t);
    if(0 == fw->secure_boot)
    {
        fw->fw_tab.fw_list_len = fw->fw_count*sizeof(fw_desc_entry_v2_t);
    }
    else
    {
        fw->fw_tab.fw_list_len = fw->fw_count*sizeof(fw_desc_entry_v22_t);
    }
    install_debug("version:0x%02x fw_count:%d, part_count:%d\r\n",fw->fw_tab.version , fw->fw_count, fw->part_count);


    // part_count setting, checksum part_desc_entry
    for(i=0; i<(int)fw->part_count; i++)
    {
        fw->fw_tab.checksum += get_checksum((u8*) &fw->part_desc[i], sizeof(part_desc_entry_v2_t));
        bytecnt += sizeof(part_desc_entry_v2_t);
    }

    // fw_desc setting, checksum fw_desc_entry
    for(i=0; i<(int)fw->fw_count; i++)
    {
        if(0 == fw->secure_boot)
        {
            fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_desc[i], sizeof(fw_desc_entry_v2_t));
            bytecnt += sizeof(fw_desc_entry_v2_t);
        }
        else
        {
            fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_desc[i], sizeof(fw_desc_entry_v22_t));
            bytecnt += sizeof(fw_desc_entry_v22_t);
        }
    }

    // "paddings" is not real paddings. It controls how many bytes the bootcode reads.
    // padding is also calculated into checksum, so we pre-add the bytecnt and give a paddings value
    bytecnt += (sizeof(fw_desc_table_t)-12);
    fw->fw_tab.paddings = SIZE_ALIGN_BOUNDARY_MORE(bytecnt, prtkimg->mtd_erasesize);

    // fw_tab setting
    // checksum fw_tab
    fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_tab+12, sizeof(fw_desc_table_t)-12);

    install_debug("checksum:0x%08x\r\n", fw->fw_tab.checksum);

    if ((dev_fd = open(prtkimg->mtdblock_path, O_RDWR|O_SYNC)) < 0)
    {
        install_err("error! open file %s fail\r\n", prtkimg->mtdblock_path);
        return -1;
    }

    lseek(dev_fd, prtkimg->reserved_boot_size, SEEK_SET);

    // write fw_tab
    write(dev_fd, &fw->fw_tab, sizeof(fw_desc_table_t));
    len += sizeof(fw_desc_table_t);

#ifdef FWDES_DUMP
    install_log("fw table\r\n");
    dump_fw_desc_table(&fw->fw_tab);
#endif

    // write part_desc
    for(i=0; i<(int)fw->part_count; i++)
    {
        write(dev_fd, &fw->part_desc[i], sizeof(part_desc_entry_v2_t));
        len += sizeof(part_desc_entry_v2_t);
#ifdef FWDES_DUMP
        install_log("part desc[%d]\r\n",i);
        dump_part_desc_entry_v2(&fw->part_desc[i]);
#endif
    }

    // write fw_desc
    for(i=0; i<(int)fw->fw_count; i++)
    {
        if(0 == fw->secure_boot)
        {
            write(dev_fd, &fw->fw_desc[i], sizeof(fw_desc_entry_v2_t));
            len += sizeof(fw_desc_entry_v2_t);
#ifdef FWDES_DUMP
            install_log("fw desc[%d]\r\n",i);
            dump_rtk_fw_desc_entry_v22(&fw->fw_desc[i]);
#endif
        }
        else
        {
            write(dev_fd, &fw->fw_desc[i], sizeof(fw_desc_entry_v22_t));
            len += sizeof(fw_desc_entry_v22_t);
#ifdef FWDES_DUMP
            install_log("fw desc[%d]\r\n",i);
            dump_rtk_fw_desc_entry_v22(&fw->fw_desc[i]);
#endif
        }
    }

    close(dev_fd);
    install_log("Finishing Burn FW Description Table!\r\n");
    return 0;
}

int save_fwdesc(CRtkFWHeader* fw, CRTKImgDesc* prtkimg)
{
    int i;
    int dev_fd;

    fw->fw_tab.checksum = 0;
    fw->fw_tab.part_list_len = fw->part_count*sizeof(part_desc_entry_v2_t);
    if(0 == fw->secure_boot)
    {
        fw->fw_tab.fw_list_len = fw->fw_count*sizeof(fw_desc_entry_v2_t);
    }
    else
    {
        fw->fw_tab.fw_list_len = fw->fw_count*sizeof(fw_desc_entry_v22_t);
    }
    install_log("version:0x%02x fw_count:%d, part_count:%d\r\n",fw->fw_tab.version , fw->fw_count, fw->part_count);


    // part_count setting, checksum part_desc_entry
    for(i=0; i<(int)fw->part_count; i++)
    {
        fw->fw_tab.checksum += get_checksum((u8*) &fw->part_desc[i], sizeof(part_desc_entry_v2_t));
    }
    // fw_desc setting, checksum fw_desc_entry
    for(i=0; i<(int)fw->fw_count; i++)
    {
        if(0 == fw->secure_boot)
            fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_desc[i], sizeof(fw_desc_entry_v2_t));
        else
            fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_desc[i], sizeof(fw_desc_entry_v22_t));
    }

    // fw_tab setting
    // checksum fw_tab
    fw->fw_tab.checksum += get_checksum((u8*) &fw->fw_tab+12, sizeof(fw_desc_table_t)-12);
    install_log("checksum:0x%08x\r\n", fw->fw_tab.checksum);

    char path[128] = {0};
    snprintf(path, sizeof(path), "%s/%s", PKG_TEMP, FWDESCTABLE);
    if ((dev_fd = open(path, O_RDWR|O_SYNC|O_CREAT, 0644)) < 0)
    {
        install_log("error! open file %s fail\r\n", path);
        return -1;
    }

    // write fw_tab
    write(dev_fd, &fw->fw_tab, sizeof(fw_desc_table_t));
    //dump_rawdata((u_int8_t*)&fw->fw_tab, sizeof(fw_desc_table_t));
#ifdef FWDES_DUMP
    install_test("fw table\r\n");
    dump_fw_desc_table(&fw->fw_tab);
#endif

    // write part_desc
    for(i=0; i<(int)fw->part_count; i++)
    {
        write(dev_fd, &fw->part_desc[i], sizeof(part_desc_entry_v2_t));
        //dump_rawdata((u_int8_t*)&fw->part_desc[i], sizeof(part_desc_entry_v2_t));
#ifdef FWDES_DUMP
        install_log("part desc[%d]\r\n",i);
        dump_part_desc_entry_v2(&fw->part_desc[i]);
#endif
    }

    // write fw_desc
    for(i=0; i<(int)fw->fw_count; i++)
    {
        if(0 == fw->secure_boot)
        {
            write(dev_fd, &fw->fw_desc[i], sizeof(fw_desc_entry_v2_t));
            //dump_rawdata((u_int8_t*)&fw->fw_desc[i], sizeof(fw_desc_entry_v2_t));
#ifdef FWDES_DUMP
            install_log("fw desc[%d]\r\n",i);
            dump_rtk_fw_desc_entry_v22(&fw->fw_desc[i]);
#endif
        }
        else
        {
            write(dev_fd, &fw->fw_desc[i], sizeof(fw_desc_entry_v22_t));
            //dump_rawdata((u_int8_t*)&fw->fw_desc[i], sizeof(fw_desc_entry_v21_t));
#ifdef FWDES_DUMP
            install_log("fw desc[%d]\r\n",i);
            dump_rtk_fw_desc_entry_v22(&fw->fw_desc[i]);
#endif
        }
    }

    close(dev_fd);

    struct t_imgdesc *pimgdesc = &prtkimg->fw[FW_FW_TBL];
    char cmd[128];
    // flash_erase
    snprintf(cmd, sizeof(cmd), "%s %s %llu %llu"
             , FLASHERASE_BIN
             , prtkimg->mtd_path
             , pimgdesc->flash_offset
             , pimgdesc->sector);
    if( rtk_command(cmd, __LINE__, __FILE__, 1) < 0)
    {
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "%s -s %llu -p %s %s"
             , NANDWRITE_BIN
             , pimgdesc->flash_offset
             , prtkimg->mtd_path, path);

    if( rtk_command(cmd, __LINE__, __FILE__, 1) < 0)
    {
        return -1;
    }

    return 0;
}


int rtk_open_mtd_char(char** ppstr);

int Rtk_fill_imgdesc_meminfo(struct t_rtkimgdesc* prtkimg, unsigned int flash_type)
{
    char* dev_path;
    int dev_fd;
    unsigned int part_inv;
    int ret, rba_percentage;
    struct mtd_info_user meminfo;

    //open mmc block device
    dev_fd = rtk_open_block_dev(&dev_path);
    if(-1 == dev_fd)
        return -1;
    rtk_close_block_dev(dev_fd);
    prtkimg->mtdblock_path = dev_path;

    //TODO
    // need a ioctl to read flash info

    // flash info
    if (flash_type == MTD_EMMC)
    {
#if 0// TV
        prtkimg->flash_type = MTD_EMMC;
        prtkimg->flash_size = 0xffffffff;
        prtkimg->mtd_erasesize = 512;
#else
        prtkimg->flash_type = MTD_EMMC;
        prtkimg->mtd_erasesize = 512;
        prtkimg->flash_size = (unsigned long long)rtk_get_size_emmc()*prtkimg->mtd_erasesize;
#endif
    }
    else
    {
        //open mtd char device
        dev_fd = rtk_open_mtd_char(&dev_path);
        if(-1 == dev_fd)
            return -1;
        prtkimg->mtd_path = dev_path;

        /* Get MTD device capability structure */
        memset(&meminfo, 0, sizeof(struct mtd_info_user));
        if (ioctl(dev_fd, MEMGETINFO, &meminfo) == 0)
        {
            // flash info
            prtkimg->flash_type = meminfo.type;
            prtkimg->flash_size = meminfo.size;
            prtkimg->page_size = meminfo.oobblock;
            prtkimg->mtd_erasesize = meminfo.erasesize;
        }
        else
        {
            install_log("Get flash info error!\r\n");
            close(dev_fd);
            return -1;
        }

        if(prtkimg->flash_type == MTD_NANDFLASH)
        {
            if (ioctl(dev_fd, GETRBAPERCENTAGE, &rba_percentage) == 0)
            {
                if (prtkimg->rba_percentage != rba_percentage)
                {
                    install_log("RBA percentage setting is different between kernel (%d) and install package (%d)!\r\n", rba_percentage, prtkimg->rba_percentage);
                }
            }
            install_log("set RBA percentage to %d\r\n", prtkimg->rba_percentage);
        }

        close(dev_fd);
    }

    if( set_system_param( prtkimg ) )
    {
        printf("Unknown flash size...\n");
        return -1;
    }

    //install_log
    printf("MEMINFO flash_type:%d flash_size:%llu (%llu KB = %lluMB) mtd_erasesize:0x%08x (%u KB), page_size:0x%08x\r\n"
           , prtkimg->flash_type
           , prtkimg->flash_size
           , prtkimg->flash_size>>10
           , prtkimg->flash_size>>20
           , prtkimg->mtd_erasesize
           , prtkimg->mtd_erasesize>>10
           , prtkimg->page_size);

    if((ret = get_parameter_value("bootcode_start", &prtkimg->bootcode_start)) < 0)
    {
        install_err("bootcode_start not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_BOOTCODE;
    }
    if (prtkimg->bootcode_start >= prtkimg->flash_size)
    {
        install_err("bootcode_start parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_BOOTCODE;
    }

    if((ret = get_parameter_value("bootcode_size", &prtkimg->bootcode_size)) < 0)
    {
        install_err("bootcode_size not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_BOOTCODE;
    }
    if (prtkimg->bootcode_size >= prtkimg->flash_size)
    {
        install_err("bootcode_size parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_BOOTCODE;
    }
    if (prtkimg->rescue_size >= prtkimg->flash_size)
    {
        install_err("rescue_size parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_RESCUE;
    }

    if((ret = get_parameter_value("factory_start",  &prtkimg->factory_start)) < 0)
    {
        install_err("factory_start not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_FACTORY;
    }
    if (prtkimg->factory_start >= prtkimg->flash_size)
    {
        install_err("factory_start parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_FACTORY;
    }

    if((ret = get_parameter_value("factory_size",  &prtkimg->factory_size)) < 0)
    {
        install_err("factory_size not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_FACTORY;
    }
    if (prtkimg->factory_size >= prtkimg->flash_size)
    {
        install_err("factory_size parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_FACTORY;
    }

    if((ret = get_parameter_value("rescue_start", &prtkimg->rescue_start)) < 0)
    {
        install_err("rescue_start not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_RESCUE;
    }
    if (prtkimg->rescue_start >= prtkimg->flash_size)
    {
        install_err("rescue_start parameter error!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_RESCUE;
    }

    if((ret = get_parameter_value("rescue_size", &prtkimg->rescue_size)) < 0)
    {
        install_err("rescue_size not found!\r\n");
        return -_eFILL_RTKIMGDESC_MEMINFO_FAIL_RESCUE;
    }
    return 0;
}


int Rtk_fill_imgdesc_layout(CRTKImgDesc* prtkimg, ZipArchive* za)  //comes from rtk_imgdesc.cpp
{
    int i, j, ret, rba_div;
    int etc_index = 0, vssu_work_part = 0;
    int efwtype;
    CBootable boottable, *pboottable = NULL;
    char cmd[128] = {0}, path[128] = {0};

    unsigned long long flash_top_low_limit=0, flash_bottom_high_limit=0;
    unsigned long long flash_total_need_size=0;
    unsigned long long fw_part_flash_bottom_start=0, fw_part_flash_top_end=0;
    unsigned long long flash_start=0xffffffffffffffffLL;
    const unsigned int _32K_BYTE = 32*1024;
    const unsigned int _256K_BYTE = 256*1024;
    const unsigned int _4M_BYTE =   4*1024*1024U;
    const unsigned int _13M_BYTE = 13*1024*1024U;
    const unsigned int _16M_BYTE = 16*1024*1024U;

    // sanity check
    // not implemented

    // initial data structure
    // firmware and partition
    for(efwtype=FW_KERNEL; efwtype<FW_UNKNOWN; efwtype=efwtype+1)
        prtkimg->fw[efwtype].efwtype = (FWTYPE)efwtype;

    // default setting
    // memory target address
    // bootcode use this info to copy firmware into correct memory address.
    if(prtkimg->fw[FW_KERNEL].mem_offset == 0)
    {
        prtkimg->fw[FW_KERNEL].mem_offset = 0x108000;
        prtkimg->fw[FW_AUDIO].mem_offset = 0x81700000;
        prtkimg->fw[FW_VIDEO].mem_offset = 0x81a00000;
        prtkimg->fw[FW_VIDEO2].mem_offset = 0x81d00000;
        prtkimg->fw[FW_AUDIO_BOOTFILE].mem_offset = 0x83000000;
        prtkimg->fw[FW_VIDEO_BOOTFILE].mem_offset = 0x83100000;
    }

    // from rtk_part
    // flash_allo_size img_size
    // fill_rtk_part(prtkimg); egnore first

    // from rtk_fw
    // flash_allo_size mem_offset
    fill_rtk_fw(prtkimg,za);

    if(prtkimg->flash_type == MTD_NANDFLASH)
    {
        prtkimg->fw[FW_BOOTCODE].img_size = prtkimg->bootcode_size;
        prtkimg->fw[FW_BOOTCODE].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->fw[FW_BOOTCODE].img_size, prtkimg->mtd_erasesize);
        prtkimg->fw[FW_BOOTCODE].sector = prtkimg->fw[FW_BOOTCODE].flash_allo_size/prtkimg->mtd_erasesize;
        for(efwtype=FW_KERNEL; efwtype<FW_VIDEO_CLOGO2; efwtype++)
        {
            prtkimg->fw[efwtype].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->fw[efwtype].img_size, prtkimg->mtd_erasesize);
            prtkimg->fw[efwtype].sector = prtkimg->fw[efwtype].flash_allo_size/prtkimg->mtd_erasesize;
            install_info("prtkimg->fw[%d].flash_allo_size=%d\r\n", efwtype, prtkimg->fw[efwtype].flash_allo_size);
            install_info("prtkimg->fw[%d].sector=%d\r\n", efwtype, prtkimg->fw[efwtype].sector);
        }

        // reserve flash erase block size for RESCUE_DT/KERNEL_DT.
        prtkimg->fw[FW_RESCUE_DT].flash_allo_size = prtkimg->mtd_erasesize;
        prtkimg->fw[FW_KERNEL_DT].flash_allo_size = prtkimg->mtd_erasesize;

        // fill install av count firmware desc
        if ((ret = fill_rtk_install_av_count(prtkimg)) < 0)
        {
            return -1;
        }

        // we use ping-pong for factory partition
        prtkimg->factory_section_size = prtkimg->factory_size;
        prtkimg->fw[FW_FACTORY].flash_offset = prtkimg->factory_start;
        prtkimg->fw[FW_FACTORY].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->factory_section_size,prtkimg->mtd_erasesize);
        prtkimg->fw[FW_FACTORY].sector = prtkimg->fw[FW_FACTORY].flash_allo_size/prtkimg->mtd_erasesize;
        prtkimg->fw[FW_FACTORY].img_size = prtkimg->fw[FW_FACTORY].flash_allo_size;

        if (prtkimg->rba_percentage)
            rba_div = 100/prtkimg->rba_percentage;
        else
            rba_div = 1;

        //reserve x% of FLASH for remapping
        prtkimg->reserved_boot_size = prtkimg->bootcode_size + prtkimg->factory_section_size;
        prtkimg->reserved_boottable_size = prtkimg->mtd_erasesize;
        prtkimg->reserved_remapping_size = 0;
        prtkimg->reserved_remapping_sector = 0;
        prtkimg->reserved_remapping_offset = 0;
        install_log("reserved remapping size is excluded!\n");

        //firmware desc table, only for NAND
        prtkimg->fw[FW_FW_TBL].flash_offset = prtkimg->reserved_boot_size;
        prtkimg->fw[FW_FW_TBL].sector = 1;
        prtkimg->fw[FW_FW_TBL].flash_allo_size = prtkimg->mtd_erasesize * prtkimg->fw[FW_FW_TBL].sector;

        //firmware & partition flash start offset and flash end offset
        if (prtkimg->safe_upgrade == 1)
        {
            install_log("SSU function not implentment in NAND yet.\r\n");
            return -1;
        }
        else
        {
            fw_part_flash_bottom_start = prtkimg->fw[FW_FW_TBL].flash_offset + prtkimg->fw[FW_FW_TBL].flash_allo_size;
            fw_part_flash_top_end = prtkimg->flash_size - prtkimg->reserved_remapping_size;
        }

        // layout buttom up
        prtkimg->fw[FW_BOOTCODE].flash_offset = prtkimg->bootcode_start;

        prtkimg->fw[FW_RESCUE_DT].flash_offset = fw_part_flash_bottom_start;
        prtkimg->fw[FW_KERNEL_DT].flash_offset = prtkimg->fw[FW_RESCUE_DT].flash_offset + prtkimg->fw[FW_RESCUE_DT].flash_allo_size;
        prtkimg->fw[FW_AUDIO].flash_offset = prtkimg->fw[FW_KERNEL_DT].flash_offset + prtkimg->fw[FW_KERNEL_DT].flash_allo_size;
        prtkimg->fw[FW_KERNEL].flash_offset = prtkimg->fw[FW_AUDIO].flash_offset + prtkimg->fw[FW_AUDIO].flash_allo_size;
        prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset = prtkimg->fw[FW_KERNEL].flash_offset + prtkimg->fw[FW_KERNEL].flash_allo_size;
        prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset = prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_RESCUE_ROOTFS].flash_allo_size;
        prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset = prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset + prtkimg->fw[FW_KERNEL_ROOTFS].flash_allo_size;
        prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset + prtkimg->fw[FW_AUDIO_BOOTFILE].flash_allo_size;
        prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset + prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size;
        flash_bottom_high_limit = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset + prtkimg->fw[FW_VIDEO_BOOTFILE].flash_allo_size;
        install_info("prtkimg->fw[FW_KERNEL].flash_allo_size=0x%012llx\r\n", prtkimg->fw[FW_KERNEL].flash_offset);
        install_info("prtkimg->fw[FW_KERNEL].sector=%d\r\n", prtkimg->fw[FW_KERNEL].sector);
    }
    else if (prtkimg->flash_type == MTD_EMMC)
    {
#if 0 //TV
        prtkimg->fw[FW_BOOTCODE].img_size = prtkimg->bootcode_size;
        prtkimg->fw[FW_BOOTCODE].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->fw[FW_BOOTCODE].img_size,512);
        prtkimg->fw[FW_BOOTCODE].sector = prtkimg->fw[FW_BOOTCODE].flash_allo_size/prtkimg->mtd_erasesize;
#else
        prtkimg->fw[FW_BOOTCODE].img_size = 0x200000;
        prtkimg->fw[FW_BOOTCODE].flash_allo_size = 0x220000;
        prtkimg->fw[FW_BOOTCODE].sector = prtkimg->fw[FW_BOOTCODE].flash_allo_size/512;
#endif

        for(efwtype=FW_KERNEL; efwtype<FW_ROOTFS; efwtype++)
        {
            prtkimg->fw[efwtype].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->fw[efwtype].img_size, prtkimg->mtd_erasesize);
        }

        // reserve 32K byte for RESCUE_DT/KERNEL_DT.
        prtkimg->fw[FW_RESCUE_DT].flash_allo_size = _256K_BYTE;
        prtkimg->fw[FW_KERNEL_DT].flash_allo_size = _256K_BYTE;
#ifdef NFLASH_LAOUT
        // reserve fw_size for Golden fw.
        prtkimg->fw[FW_UBOOT].flash_allo_size = _4M_BYTE;
        prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size = _16M_BYTE;
        prtkimg->fw[FW_GOLD_RESCUE_DT].flash_allo_size = _256K_BYTE;
        prtkimg->fw[FW_GOLD_AUDIO].flash_allo_size = _4M_BYTE;
        prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size = _13M_BYTE;
        prtkimg->fw[FW_GOLD_KERNEL].flash_allo_size = _16M_BYTE;
#ifdef TEE_ENABLE
        prtkimg->fw[FW_BL31].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->fw[FW_BL31].img_size,512);
#endif
#endif

        // fill install av count firmware desc
        if ((ret = fill_rtk_install_av_count(prtkimg)) < 0)
        {
            return -1;
        }

#ifdef NFLASH_LAOUT
#ifdef USE_SE_STORAGE
        prtkimg->se_storage_section_size = prtkimg->se_storageSize; //prtkimg->se_storageSize * 2;
        prtkimg->fw[FW_SE_STORAGE].flash_offset = prtkimg->se_storage_Start;
        prtkimg->fw[FW_SE_STORAGE].flash_allo_size = SIZE_ALIGN_BOUNDARY_MORE(prtkimg->se_storage_section_size, 512);
        prtkimg->fw[FW_SE_STORAGE].img_size = prtkimg->fw[FW_SE_STORAGE].flash_allo_size;
#endif
#endif

        //firmware desc table, only for EMMC
        // we use ping-pong for factory partition
        prtkimg->factory_section_size = 0x400000;  //emmc passed whole factory size
        prtkimg->fw[FW_FACTORY].flash_offset = 0x220000;
        prtkimg->fw[FW_FACTORY].flash_allo_size = 0x400000;
        prtkimg->fw[FW_FACTORY].img_size = 0x300000;

        prtkimg->reserved_boot_size = prtkimg->fw[FW_FACTORY].flash_offset + prtkimg->fw[FW_FACTORY].flash_allo_size;
        prtkimg->reserved_boottable_size = _32K_BYTE;


        prtkimg->fw[FW_FW_TBL].flash_offset = prtkimg->reserved_boot_size;
        prtkimg->fw[FW_FW_TBL].flash_allo_size = prtkimg->reserved_boottable_size;

        prtkimg->fw[FW_GOLD_FW_TBL].flash_offset = prtkimg->fw[FW_FW_TBL].flash_offset + prtkimg->reserved_boottable_size;
        prtkimg->fw[FW_GOLD_FW_TBL].flash_allo_size = prtkimg->reserved_boottable_size;

        fw_part_flash_bottom_start = prtkimg->fw[FW_GOLD_FW_TBL].flash_offset + prtkimg->fw[FW_GOLD_FW_TBL].flash_allo_size;
        fw_part_flash_top_end = 0xe4800000;//prtkimg->flash_size;

        // layout buttom up

        prtkimg->fw[FW_BOOTCODE].flash_offset = 0x0;//prtkimg->bootcode_start;

#ifdef NFLASH_LAOUT
        prtkimg->fw[FW_GOLD_RESCUE_DT].flash_offset = fw_part_flash_bottom_start + prtkimg->mtd_erasesize;
        prtkimg->fw[FW_GOLD_AUDIO].flash_offset = prtkimg->fw[FW_GOLD_RESCUE_DT].flash_offset + prtkimg->fw[FW_GOLD_RESCUE_DT].flash_allo_size;
        prtkimg->fw[FW_GOLD_KERNEL].flash_offset = prtkimg->fw[FW_GOLD_AUDIO].flash_offset + prtkimg->fw[FW_GOLD_AUDIO].flash_allo_size;
        prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset = prtkimg->fw[FW_GOLD_KERNEL].flash_offset + prtkimg->fw[FW_GOLD_KERNEL].flash_allo_size;
        //prtkimg->fw[FW_UBOOT].flash_offset = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size; + _8K_BYTE;
        //prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset = prtkimg->fw[FW_UBOOT].flash_offset + prtkimg->fw[FW_UBOOT].flash_allo_size + _8K_BYTE;
#endif
#ifdef NAS_ENABLE
        /* Add one block for rescue fwdesc table */
#ifdef NFLASH_LAOUT
        prtkimg->fw[FW_NAS_RESCUE_DT].flash_offset = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size;
#else
        prtkimg->fw[FW_NAS_RESCUE_DT].flash_offset = fw_part_flash_bottom_start + prtkimg->mtd_erasesize;
#endif
        prtkimg->fw[FW_NAS_KERNEL].flash_offset = prtkimg->fw[FW_NAS_RESCUE_DT].flash_offset + prtkimg->fw[FW_NAS_RESCUE_DT].flash_allo_size;
        prtkimg->fw[FW_NAS_RESCUE_ROOTFS].flash_offset = prtkimg->fw[FW_NAS_KERNEL].flash_offset + prtkimg->fw[FW_NAS_KERNEL].flash_allo_size;
        prtkimg->fw[FW_RESCUE_DT].flash_offset = prtkimg->fw[FW_NAS_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_NAS_RESCUE_ROOTFS].flash_allo_size;
        /* no rescue fwdesc if there's no rescue DTB/kernel/rootfs */
        if(prtkimg->fw[FW_RESCUE_DT].flash_offset == prtkimg->fw[FW_NAS_RESCUE_DT].flash_offset)
#endif
#ifdef NFLASH_LAOUT
        prtkimg->fw[FW_RESCUE_DT].flash_offset = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size;
        //prtkimg->fw[FW_RESCUE_DT].flash_offset = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset + prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size;
        //prtkimg->fw[FW_RESCUE_DT].flash_offset = prtkimg->fw[FW_UBOOT].flash_offset + prtkimg->fw[FW_UBOOT].flash_allo_size;
#else
        prtkimg->fw[FW_RESCUE_DT].flash_offset = fw_part_flash_bottom_start;
#endif
        prtkimg->fw[FW_KERNEL_DT].flash_offset = prtkimg->fw[FW_RESCUE_DT].flash_offset + prtkimg->fw[FW_RESCUE_DT].flash_allo_size;
        prtkimg->fw[FW_AUDIO].flash_offset = prtkimg->fw[FW_KERNEL_DT].flash_offset + prtkimg->fw[FW_KERNEL_DT].flash_allo_size;
        prtkimg->fw[FW_KERNEL].flash_offset = prtkimg->fw[FW_AUDIO].flash_offset + prtkimg->fw[FW_AUDIO].flash_allo_size;
#ifdef TEE_ENABLE
        prtkimg->fw[FW_TEE].flash_offset = prtkimg->fw[FW_KERNEL].flash_offset + prtkimg->fw[FW_KERNEL].flash_allo_size;
        prtkimg->fw[FW_BL31].flash_offset = prtkimg->fw[FW_TEE].flash_offset + prtkimg->fw[FW_TEE].flash_allo_size;
        prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset = prtkimg->fw[FW_BL31].flash_offset + prtkimg->fw[FW_BL31].flash_allo_size;
#else
        prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset = prtkimg->fw[FW_KERNEL].flash_offset + prtkimg->fw[FW_KERNEL].flash_allo_size;
#endif
        prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset = prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset + prtkimg->fw[FW_RESCUE_ROOTFS].flash_allo_size;
        prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset = prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset + prtkimg->fw[FW_KERNEL_ROOTFS].flash_allo_size;
        prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset + prtkimg->fw[FW_AUDIO_BOOTFILE].flash_allo_size;
        flash_bottom_high_limit = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset + prtkimg->fw[FW_VIDEO_BOOTFILE].flash_allo_size;
    }

#ifdef INSTALL_DEBUG
    unsigned int temp_max, _temp, _sort[FW_UNKNOWN];
    for (i=FW_KERNEL; i<FW_UNKNOWN; i++)
    {
        _sort[i] = i;
    }
    for (i=FW_KERNEL; i<FW_UNKNOWN; i++)
    {
        temp_max = i;
        for (j=i+1; j<FW_UNKNOWN; j++)
        {
            if (prtkimg->fw[_sort[temp_max]].flash_offset < prtkimg->fw[_sort[j]].flash_offset)
            {
                temp_max = j;
            }
        }
        {
            _temp = _sort[i];
            _sort[i] = _sort[temp_max];
            _sort[temp_max] = _temp;
        }
    }
    install_info("\r\n[Layout View]\r\n");
    for(efwtype = FW_KERNEL; efwtype < FW_UNKNOWN; efwtype++)
    {
        if (0 == prtkimg->fw[_sort[efwtype]].flash_allo_size) continue;

        if (flash_start == 0xffffffffffffffffLL)
        {
            install_info("	 +-----------------------------+ 0x%012llx\r\n",
                         prtkimg->fw[_sort[efwtype]].flash_offset + prtkimg->fw[_sort[efwtype]].flash_allo_size);
        }
        else
            install_info("	 +-----------------------------+ 0x%012llx\r\n", flash_start);

        install_info("	 |%12s (%10lluKBytes)	|\r\n", inv_by_fwtype(_sort[efwtype]), prtkimg->fw[_sort[efwtype]].flash_allo_size/1024);

        flash_start = prtkimg->fw[_sort[efwtype]].flash_offset;
    }
    if (prtkimg->fw[FW_ECPU].flash_offset  < prtkimg->bootcode_start)
    {
        install_info(" 	+-----------------------------+ 0x%012x\r\n", prtkimg->fw[FW_ECPU].flash_offset);
    }
    else
    {
        install_info(" 	+-----------------------------+ 0x%012x\r\n", 0);
    }
#endif
    return 0;
}



/*
** Load config.txt info into RTKImgDesc structure
*/
int Rtk_load_config(CRTKImgDesc* prtkimg, ZipArchive* za)
{

    // Extract config.txt from zip
    char* cfgFullPath = "/tmp/config.txt";
    char* cfgOrigPath = "config.txt";
    FILE* pFile = NULL;
    char newline[512] = {0};
    if (getExtractFileinZip(za, cfgOrigPath, cfgFullPath) == false)
    {
        install_err("Load Config Fail!\n");
        return -1;
    }
    pFile = fopen(cfgFullPath, "r");
    if(pFile == NULL)
    {
        install_err("Can't open config.txt\r\n");
        return -1;
    }

    while(NULL != fgets(newline, sizeof(newline), pFile))
    {
        if((newline[0] == ';') || (newline[0] == '#')) continue;
        if(0 == strncmp("fw", newline, 2))
        {
            add_firmware(skip_char(newline+2, '='), prtkimg);
            continue;
        }
        if(0 == strncmp("ifcmd", newline, strlen("ifcmd")))
        {
            add_install_factory_command(newline, prtkimg);
            continue;
        }
        if(0 == strncmp("boot_av_num", newline, strlen("boot_av_num")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->install_avfile_count = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("install_bootcode", newline, strlen("install_bootcode")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->bootcode = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("install_factory", newline, strlen("install_factory")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->factory = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("verify", newline, strlen("verify")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->verify = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("install_avfile_video_size", newline, strlen("install_avfile_video_size")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->install_avfile_video_size = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("install_avfile_audio_size", newline, strlen("install_avfile_audio_size")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->install_avfile_audio_size = atoi(str_tmp);
            continue;
        }
        if(0 == strncmp("secure_boot", newline, strlen("secure_boot")))
        {
            char* str_tmp = NULL;
            int str_len = 0;
            skip_quotation(skip_char(newline, '='), &str_tmp, &str_len);
            prtkimg->secure_boot = atoi(str_tmp);
            continue;
        }

    }

//    rtk_load_fw(cfgFullPath,prtkimg);
    return 0;
}

static int rtk_burn_bootcode_part(CRTKImgDesc* prtkimgdesc, char *path, unsigned int offset, unsigned char spare_id)
{
    unsigned len, need_block_size, num_of_block, j;
    char cmd[128];
    int i;

    if( rtk_get_size_of_file(path, &len) < 0)
    {
        install_info("\"%s\" file not found\r\n", path);
        return -1;
    }
    need_block_size = SIZE_ALIGN_BOUNDARY_MORE(len, prtkimgdesc->mtd_erasesize);
    num_of_block = need_block_size/prtkimgdesc->mtd_erasesize;

    for (i=0; i<NAND_BOOT_BACKUP_COUNT; i++)
    {
        install_debug("offset:0x%08x, end:0x%08x, size:0x%08x(%u)\r\n", offset, offset+len-1, len, len);

        snprintf(cmd, sizeof(cmd), "%s %s %u %u"
                 , FLASHERASE_BIN
                 , prtkimgdesc->mtd_path
                 , offset
                 , num_of_block);
        if( rtk_command(cmd, __LINE__, __FILE__, 1) < 0)
        {
            install_debug("Exec command fail\r\n");
            return -1;
        }

        for(j=0; j<num_of_block; j++)
        {

            snprintf(cmd, sizeof(cmd), "%s -s %u -p %s --rtk_oob=%d --rtk_blk=%u %s"
                     , NANDWRITE_BIN
                     , offset
                     , prtkimgdesc->mtd_path
                     , spare_id
                     , j
                     , path);
            if( rtk_command(cmd, __LINE__, __FILE__, 1) < 0)
            {
                install_debug("Exec command fail\r\n");
                return -1;
            }
        }

        // verify
        if(prtkimgdesc->verify == 1)
        {
            unsigned error;//, checksum;
            int ret = file_verify(prtkimgdesc->mtdblock_path
                                  , offset
                                  , path
                                  , 0
                                  , len
                                  , &error);
            //, &checksum );
            if (ret < 0)
            {
                install_info("verify error!!!\r\n");
                return -1;
            }
        }

        offset = offset + need_block_size;
    }

    return offset;
}


int rtk_burn_bootcode_nand(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath)
{
    int offset;
    char buf[256];

    snprintf(buf, sizeof(buf), "cd /tmp;/sbin/busybox unzip %s bootloader.tar;rm -rf %s;mkdir -p %s;tar -xf %s.tar -C %s/", zipPath, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP);

    if( rtk_command(buf, __LINE__, __FILE__, 0) < 0)
    {
        install_info("untar %s.tar fail\r\n", BOOTCODE_TEMP);
        return -1;
    }

    offset = prtkimgdesc->mtd_erasesize*RTK_NAND_HWSETTING_START_BLOCK;

    snprintf(buf, sizeof(buf), "%s/%s", BOOTCODE_TEMP, HWSETTING_FILENAME);
    if( (offset=rtk_burn_bootcode_part(prtkimgdesc, buf, offset, BLOCK_HWSETTING)) < 0 )
    {
        install_info("burn hwsetting fail\n");
        return -1;
    }

    snprintf(buf, sizeof(buf), "%s/%s", BOOTCODE_TEMP, UBOOT_FILENAME);
    if( (offset=rtk_burn_bootcode_part(prtkimgdesc, buf, offset, BLOCK_BOOTCODE)) < 0 )
    {
        install_info("burn hwsetting fail\n");
        return -1;
    }

    return 0;
}

int rtk_burn_bootcode_emmc(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath)
{
    int ret;
    unsigned int len, start, error;
    char cmd[256] = {0}, path[256] = {0};
    int verify_flag = 1;
    int ChipVer = 0;

    int i = 0;
    static char BOOTCODE_AREA_FILE_PATH[][50]={
        "/tmp/bootloader/hw_setting.bin",
        "/tmp/bootloader/uboot.bin",
        "/tmp/bootloader/fsbl.bin",
        "/tmp/bootloader/tee.bin",
        "/tmp/bootloader/bl31.bin",
        "/tmp/bootloader/rsa_bin_fw.bin",
        "/tmp/bootloader/rsa_bin_tee.bin"
    };

    if (get_chip_rev_id() >= RTD129x_CHIP_REVISION_B00) {
        ChipVer = 1;
        install_info("Bootcode(LK).\n");
    }

    if (ChipVer)
        snprintf(cmd, sizeof(cmd), "cd /tmp;/sbin/busybox unzip %s bootloader_lk.tar;rm -rf %s;mkdir -p %s;tar -xf %s_lk.tar -C %s/", zipPath, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP);
    else
        snprintf(cmd, sizeof(cmd), "cd /tmp;/sbin/busybox unzip %s bootloader.tar;rm -rf %s;mkdir -p %s;tar -xf %s.tar -C %s/", zipPath, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP, BOOTCODE_TEMP);
    if( rtk_command(cmd, __LINE__, __FILE__, 0) < 0)
    {
        install_info("untar %s.tar fail\r\n", BOOTCODE_TEMP);
        return -1;
    }

    start = prtkimgdesc->bootcode_start;
    install_info("\r\n[Start Burn Bootcode area fw]\r\n");
    for (i=0; i<(int)(sizeof(BOOTCODE_AREA_FILE_PATH)/sizeof(BOOTCODE_AREA_FILE_PATH[0])); i++){
        if ((!prtkimgdesc->secure_boot) && (i >= 5))
            return 0;

        if (i > 0)
            start += SIZE_ALIGN_BOUNDARY_MORE(len, prtkimgdesc->mtd_erasesize);

        memset(path, 0, sizeof(path));
        if (ChipVer) {
            if (i == 1)
                strcpy(path, "/tmp/bootloader/lk.bin");
            else if(i == 3)
                strcpy(path, "/tmp/bootloader/tee_enc.bin");
            else if(i == 4)
                strcpy(path, "/tmp/bootloader/bl31_enc.bin");
            else
                strcpy(path, BOOTCODE_AREA_FILE_PATH[i]);
        }
        else
            strcpy(path, BOOTCODE_AREA_FILE_PATH[i]);

        ret = rtk_get_size_of_file(path, &len);
        if(ret < 0)
        {
            install_info("\"%s\" file not found\r\n", path);
            return -1;
        }

        install_info("\r\n[Burn fw(%d): %s]\r\n", i, path);
        install_debug("start:0x%08x, size:0x%08x(%u), end:0x%08x\r\n", start, len, len, start+len-1);
        ret = file_to_flash(path, 0, prtkimgdesc->mtdblock_path, start, len, NULL);
        if(ret < 0)
        {
            install_info("burn bootcode area fw(%s) fail\r\n", path);
            return -1;
        }

        // verify
        if(verify_flag) {
            ret = file_verify(prtkimgdesc->mtdblock_path
                            , start
                            , path
                            , 0
                            , len
                            , &error);

            if (ret < 0)
            {
                install_info("verify error!!!\r\n");
                return -1;
            }
        }
    }

    return 0;
}

int Rtk_burn_fw_emmc(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath, State* state)
{
    char* tmpBuffer=NULL;
    int efwtype;
    bool success = false;
    unsigned int checksum_tmp;

    printf("Rtk_burn_fw_emmc start.\n");

    UpdaterInfo* ui = (UpdaterInfo*)(state->cookie);
    fprintf(ui->cmd_pipe, "ui_print Starting burning RTK images\n");
    fprintf(ui->cmd_pipe, "ui_print\n");

    if (prtkimgdesc->bootcode)
    {
        if (rtk_burn_bootcode_emmc(prtkimgdesc, za, zipPath) < 0)
        {
            install_info("rtk_burn_bootcode_emmc failed!!\n");
            return 0;
        }
    }

    for(efwtype = FW_KERNEL; efwtype < FW_UNKNOWN; efwtype = efwtype+1)
    {
        const ZipEntry* imgEntry = mzFindZipEntry(za, prtkimgdesc->fw[efwtype].filename);
        if ((efwtype >= FW_ROOTFS) && (efwtype <= FW_SE_STORAGE))
            continue;

        if (imgEntry)
        {
#if 0
            FILE* pTmpImg = fopen("/tmp/img2write", "wb");
            if (pTmpImg)
            {
                success = mzExtractZipEntryToFile(za, imgEntry, fileno(pTmpImg));
            }
#else
            tmpBuffer = (char *)malloc(prtkimgdesc->fw[efwtype].img_size + 32);
            memset(tmpBuffer, 0, prtkimgdesc->fw[efwtype].img_size + 32);
            if (tmpBuffer != NULL)
            {
                install_debug("ready for extract %s\n", prtkimgdesc->fw[efwtype].filename);
                success = mzExtractZipEntryToBuffer(za, imgEntry,(unsigned char *)tmpBuffer);
            }
#endif
            if (success /*&& pTmpImg*/)
            {
#if 0
                FILE* f = fopen(prtkimgdesc->mtdblock_path, "wb");
                lseek(f, prtkimgdesc->fw[efwtype].flash_offset, SEEK_SET);
#endif
                rtk_file_checksum(tmpBuffer, prtkimgdesc->fw[efwtype].img_size, prtkimgdesc->fw[efwtype].sha_hash);
                int dfd = open(prtkimgdesc->mtdblock_path, O_RDWR|O_SYNC);
                if(dfd<0)
                {
                    install_err("Can't open %s\r\n", prtkimgdesc->mtdblock_path);
                    return 0;
                }
                int ret = lseek64(dfd, prtkimgdesc->fw[efwtype].flash_offset, SEEK_SET);
                if (ret == (off64_t) -1)
                {
                    install_err("lseek64 (%s) fail\r\n", prtkimgdesc->mtdblock_path);
                    install_err("lseek64 offset: 0x%llx.\r\n", prtkimgdesc->fw[efwtype].flash_offset);
                    close(dfd);
                    return 0;
                }
                install_debug("ready for burn %s\n", prtkimgdesc->fw[efwtype].filename);
#if 1
                install_debug("fd seek to %d\n", prtkimgdesc->fw[efwtype].flash_offset);
                int count = buf_to_fd(tmpBuffer, dfd, prtkimgdesc->fw[efwtype].img_size,prtkimgdesc->fw[efwtype].img_size,&checksum_tmp, NULL);
//               int count = fd_to_fd(pTmpImg, f, prtkimgdesc->fw[efwtype].img_size,&(prtkimgdesc->fw[efwtype].checksum), NULL);

#else
                int count = fwrite(tmpBuffer, prtkimgdesc->fw[efwtype].img_size, 1, f);
                if (count <= 0)
                {
                    install_err("Failed writing %s\n",prtkimgdesc->fw[efwtype].filename);
                    success = false;
                }
#endif
                if (count <= 0)
                {
                    install_err("Failed writing %s\n",prtkimgdesc->fw[efwtype].filename);
                    fprintf(ui->cmd_pipe, "ui_print Failed writing %s\n",prtkimgdesc->fw[efwtype].filename);
                    fprintf(ui->cmd_pipe, "ui_print\n");
                    success = false;
                }
                close(dfd);
#if 0
                if (fclose(f) != 0)
                {
                    install_err("Failed closing %s\n",prtkimgdesc->mtdblock_path);
                    success = false;
                }
#endif
            }
            else
            {
                install_err("Failed extract %s\n",prtkimgdesc->fw[efwtype].filename);
                fprintf(ui->cmd_pipe, "ui_print Failed extract %s\n",prtkimgdesc->fw[efwtype].filename);
                fprintf(ui->cmd_pipe, "ui_print\n");
            }
            free(tmpBuffer);
            tmpBuffer = NULL;
        }
    }
    if (success)
    {
        install_info("Burning RTK images finished");
        fprintf(ui->cmd_pipe, "ui_print Burning RTK images successed\n\n");
        fprintf(ui->cmd_pipe, "ui_print\n");
        return 1;
    }
    else
    {
        install_err("Burning RTK images failed");
        fprintf(ui->cmd_pipe, "ui_print Burning RTK images failed\n");
        fprintf(ui->cmd_pipe, "ui_print\n");
        return 0;
    }
}

int Rtk_burn_fw_nand(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath, State* state)
{
    char* tmpBuffer=NULL;
    int efwtype;
    bool success = false;
    int ret = 0;
    char cmd[256] = {0};

    printf("Rtk_burn_fw_nand start.\n");

    UpdaterInfo* ui = (UpdaterInfo*)(state->cookie);
    fprintf(ui->cmd_pipe, "ui_print Starting burning RTK images\n");
    fprintf(ui->cmd_pipe, "ui_print\n");
    if (prtkimgdesc->bootcode)
    {
        if (rtk_burn_bootcode_nand(prtkimgdesc, za, zipPath) < 0)
        {
            install_info("rtk_burn_bootcode_nand failed!!\n");
            return 0;
        }
    }

    for(efwtype = FW_KERNEL; efwtype <= FW_VIDEO_CLOGO2; efwtype = efwtype+1)
    {
        const ZipEntry* imgEntry = mzFindZipEntry(za, prtkimgdesc->fw[efwtype].filename);
        if (imgEntry)
        {
            tmpBuffer = (char *)malloc(prtkimgdesc->fw[efwtype].img_size + 32);
            memset(tmpBuffer, 0, prtkimgdesc->fw[efwtype].img_size + 32);
            if (tmpBuffer != NULL)
            {
                install_debug("ready for extract %s\n", prtkimgdesc->fw[efwtype].filename);
                success = mzExtractZipEntryToBuffer(za, imgEntry,(unsigned char *)tmpBuffer);
            }
            if (success /*&& pTmpImg*/)
            {
                // flash_erase
                install_info("prtkimg->fw[%d].flash_allo_size=0x%012llx\r\n", efwtype, prtkimgdesc->fw[efwtype].flash_offset);
                install_info("prtkimg->fw[%d].sector=%llu\r\n", efwtype, prtkimgdesc->fw[efwtype].sector);
                snprintf(cmd, sizeof(cmd), "%s %s %llu %llu"
                         , FLASHERASE_BIN
                         , prtkimgdesc->mtd_path
                         , prtkimgdesc->fw[efwtype].flash_offset
                         , prtkimgdesc->fw[efwtype].sector);
                if((ret = rtk_command(cmd, __LINE__, __FILE__, 1)) < 0)
                {
                    install_debug("Exec command fail\r\n");
                    //return -1;
                }
#if 1
                snprintf(cmd, sizeof(cmd), "cd /tmp ; /sbin/busybox unzip %s %s ; %s -s %llu -p %s /tmp/%s"
                         , zipPath
                         , prtkimgdesc->fw[efwtype].filename
                         , NANDWRITE_BIN
                         , prtkimgdesc->fw[efwtype].flash_offset
                         , prtkimgdesc->mtd_path
                         , prtkimgdesc->fw[efwtype].filename);

                if((ret = rtk_command(cmd, __LINE__, __FILE__, 1)) < 0)
                {
                    install_debug("Exec command fail\r\n");
                    //return -1;
                }
#else
                int dfd = open(prtkimgdesc->mtdblock_path, O_RDWR|O_SYNC);
                if(dfd<0)
                {
                    install_err("Can't open %s\r\n", prtkimgdesc->mtdblock_path);
                    return 0;
                }
                int ret = lseek(dfd, prtkimgdesc->fw[efwtype].flash_offset, SEEK_SET);
                if(ret < 0)
                {
                    install_err("lseek (%s) fail\r\n", prtkimgdesc->mtdblock_path);
                    close(dfd);
                    return 0;
                }
                install_debug("ready for burn %s\n", prtkimgdesc->fw[efwtype].filename);
                install_debug("fd seek to %d\n", prtkimgdesc->fw[efwtype].flash_offset);
                int count = buf_to_fd(tmpBuffer, dfd, prtkimgdesc->fw[efwtype].img_size,prtkimgdesc->fw[efwtype].img_size,&(prtkimgdesc->fw[efwtype].checksum), NULL);

                if (count <= 0)
                {
                    install_err("Failed writing %s\n",prtkimgdesc->fw[efwtype].filename);
                    fprintf(ui->cmd_pipe, "ui_print Failed writing %s\n",prtkimgdesc->fw[efwtype].filename);
                    fprintf(ui->cmd_pipe, "ui_print\n");
                    success = false;
                }
                close(dfd);
#endif
            }
            else
            {
                install_err("Failed extract %s\n",prtkimgdesc->fw[efwtype].filename);
                fprintf(ui->cmd_pipe, "ui_print Failed extract %s\n",prtkimgdesc->fw[efwtype].filename);
                fprintf(ui->cmd_pipe, "ui_print\n");
            }
            free(tmpBuffer);
            tmpBuffer = NULL;
        }
        //if (efwtype == FW_AUDIO)
        //return -1;
    }
    if (success)
    {
        install_info("Burning RTK images finished");
        fprintf(ui->cmd_pipe, "ui_print Burning RTK images successed\n\n");
        fprintf(ui->cmd_pipe, "ui_print\n");
        return 1;
    }
    else
    {
        install_err("Burning RTK images failed");
        fprintf(ui->cmd_pipe, "ui_print Burning RTK images failed\n");
        fprintf(ui->cmd_pipe, "ui_print\n");
        return 0;
    }
}


int Rtk_burn_fwdesc_emmc(CRTKImgDesc* prtkimg, State* state)
{
    int ret=0, dev_fd=0, i;
    u8 fw_count;
    struct rtk_fw_header rtk_fw_head;
    struct rtk_fw_header emmc_rtk_fw_head;
    int efwtype;
    struct t_PARTDESC* rtk_part;
    memset(&rtk_fw_head, 0, sizeof(rtk_fw_head));

    printf("Rtk_burn_fwdesc_emmc start.\n");
    // sanity-check
    if(prtkimg->fw[FW_KERNEL].img_size == 0 || prtkimg->fw[FW_RESCUE_DT].img_size == 0 \
            || prtkimg->fw[FW_KERNEL_DT].img_size == 0 || prtkimg->fw[FW_AUDIO].img_size == 0)
    {
        install_debug("FW_KERNEL(%u), FW_AUDIO(%u), FW_RESCUE_DT(%u), FW_KERNEL_DT(%u), FW_ETC(%u) zero check.\r\n"  \
                      , prtkimg->fw[FW_KERNEL].img_size	 \
                      , prtkimg->fw[FW_AUDIO].img_size \
                      , prtkimg->fw[FW_RESCUE_DT].img_size
                      , prtkimg->fw[FW_KERNEL_DT].img_size	\
                      , prtkimg->fw[FW_USR_LOCAL_ETC].img_size);
        return -1;
    }

    UpdaterInfo* ui = (UpdaterInfo*)(state->cookie);
    fprintf(ui->cmd_pipe, "ui_print Starting burning RTK FWDesc\n");
    fprintf(ui->cmd_pipe, "ui_print\n");

    // read fw_header from emmc first to fill partition info later
    if ((dev_fd = open(prtkimg->mtdblock_path, O_RDWR|O_SYNC)) < 0)
    {
        install_err("error! open file %s fail\r\n", prtkimg->mtdblock_path);
        return 0;
    }
    lseek(dev_fd, prtkimg->reserved_boot_size, SEEK_SET);
    read(dev_fd, (void*)&emmc_rtk_fw_head, sizeof(emmc_rtk_fw_head));

#ifdef FWDES_DUMP
    dump_fw_desc_table_v1(&(emmc_rtk_fw_head.fw_tab));
#endif

    install_log("fw cnt:%d ; part cnt:%d\r\n",emmc_rtk_fw_head.fw_count, emmc_rtk_fw_head.part_count);

    if (emmc_rtk_fw_head.fw_tab.part_list_len != 0)
    {
        emmc_rtk_fw_head.part_count = emmc_rtk_fw_head.fw_tab.part_list_len / sizeof(part_desc_entry_v2_t);
    }
    emmc_rtk_fw_head.fw_count = emmc_rtk_fw_head.fw_tab.fw_list_len / sizeof(fw_desc_entry_v2_t);
    install_log("fw cnt:%d ; part cnt:%d\r\n",emmc_rtk_fw_head.fw_count, emmc_rtk_fw_head.part_count);
#ifdef FWDES_DUMP
    for(i=0; i<(int)emmc_rtk_fw_head.part_count; i++)
    {
        install_log("part desc[%d]\r\n",i);
        dump_part_desc_entry_v2(&(emmc_rtk_fw_head.part_desc[i]));
    }
    for(i=0; i<(int)emmc_rtk_fw_head.fw_count; i++)
    {
        install_log("fw desc[%d]\r\n",i);
        dump_rtk_fw_desc_entry_v22(&(emmc_rtk_fw_head.fw_desc[i]));
    }
#endif
    close(dev_fd);

    // fw field
    // kernel fw_desc
    fw_count = 1;
    rtk_fw_head.fw_count = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL;
    if (!strcmp(prtkimg->fw[FW_KERNEL].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL].sha_hash, SHA256_SIZE);

    // Rescue_DT fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_RESCUE_DT;
    if (!strcmp(prtkimg->fw[FW_RESCUE_DT].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_RESCUE_DT].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_RESCUE_DT].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_RESCUE_DT].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_RESCUE_DT].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_RESCUE_DT].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_RESCUE_DT].sha_hash, SHA256_SIZE);

    // Kernel_DT fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL_DT;
    if (!strcmp(prtkimg->fw[FW_KERNEL_DT].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL_DT].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL_DT].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL_DT].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL_DT].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL_DT].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL_DT].sha_hash, SHA256_SIZE);

    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_RESCUE_ROOTFS;
    if (!strcmp(prtkimg->fw[FW_RESCUE_ROOTFS].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_RESCUE_ROOTFS].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_RESCUE_ROOTFS].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_RESCUE_ROOTFS].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_RESCUE_ROOTFS].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_RESCUE_ROOTFS].sha_hash, SHA256_SIZE);

    // NAS: kernel rootfs fw_desc
    if(prtkimg->fw[FW_KERNEL_ROOTFS].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL_ROOTFS;
        if (!strcmp(prtkimg->fw[FW_KERNEL_ROOTFS].compress_type, "lzma"))
            rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL_ROOTFS].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL_ROOTFS].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL_ROOTFS].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL_ROOTFS].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL_ROOTFS].sha_hash, SHA256_SIZE);
    }

    // audio fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_AUDIO;
    if (!strcmp(prtkimg->fw[FW_AUDIO].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_AUDIO].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_AUDIO].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_AUDIO].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_AUDIO].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_AUDIO].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_AUDIO].sha_hash, SHA256_SIZE);

    // audio bootfile fw_desc
    if(prtkimg->fw[FW_AUDIO_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_AUDIO_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_AUDIO_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_AUDIO_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_AUDIO_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_AUDIO_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    // image bootfile fw_desc
    if(prtkimg->fw[FW_IMAGE_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_IMAGE_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_IMAGE_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_IMAGE_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_IMAGE_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_IMAGE_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    // video bootfile fw_desc
    if(prtkimg->fw[FW_VIDEO_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_VIDEO_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_VIDEO_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_VIDEO_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_VIDEO_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_VIDEO_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    //if(prtkimg->fw[FW_UBOOT].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_UBOOT;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_UBOOT].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_UBOOT].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_UBOOT].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_UBOOT].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_UBOOT].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_UBOOT].sha_hash, SHA256_SIZE);
    }
#ifdef NFLASH_LAOUT
    if(prtkimg->fw[FW_GOLD_KERNEL].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_GOLD_KERNEL;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_GOLD_KERNEL].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_GOLD_KERNEL].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_GOLD_KERNEL].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_GOLD_KERNEL].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_GOLD_KERNEL].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_GOLD_KERNEL].sha_hash, SHA256_SIZE);
    }

    if(prtkimg->fw[FW_GOLD_RESCUE_DT].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_GOLD_RESCUE_DT;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_GOLD_RESCUE_DT].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_GOLD_RESCUE_DT].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_GOLD_RESCUE_DT].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_GOLD_RESCUE_DT].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_GOLD_RESCUE_DT].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_GOLD_RESCUE_DT].sha_hash, SHA256_SIZE);
    }

    if(prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_GOLD_RESCUE_ROOTFS;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].sha_hash, SHA256_SIZE);
    }

    if(prtkimg->fw[FW_GOLD_AUDIO].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_GOLD_AUDIO;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_GOLD_AUDIO].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_GOLD_AUDIO].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_GOLD_AUDIO].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_GOLD_AUDIO].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_GOLD_AUDIO].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_GOLD_AUDIO].sha_hash, SHA256_SIZE);
    }
#endif
#ifdef TEE_ENABLE
    if(prtkimg->fw[FW_TEE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_TEE;
        if (!strcmp(prtkimg->fw[FW_TEE].compress_type, "lzma"))
            rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_TEE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_TEE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_TEE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_TEE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_TEE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_TEE].sha_hash, SHA256_SIZE);
    }
    if(prtkimg->fw[FW_BL31].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_BL31;
        if (!strcmp(prtkimg->fw[FW_BL31].compress_type, "lzma"))
            rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_BL31].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_BL31].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_BL31].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_BL31].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_BL31].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_BL31].sha_hash, SHA256_SIZE);
    }
#endif
    // fw_count
    rtk_fw_head.fw_count++;

    // partition info in fw_enrty
    // partition info refill
    rtk_fw_head.part_count = emmc_rtk_fw_head.part_count;
    memcpy(&(rtk_fw_head.part_desc), &(emmc_rtk_fw_head.part_desc), sizeof(rtk_fw_head.part_desc));

    /*
        install_log("========== Checking copied part ==========\r\n",i);

        for(i=0;i<(int)rtk_fw_head.part_count;i++) {
            install_log("part desc[%d]\r\n",i);
            dump_part_desc_entry_v1(&(rtk_fw_head.part_desc[i]));
        }
    */
    memcpy(rtk_fw_head.fw_tab.signature, FIRMWARE_DESCRIPTION_TABLE_SIGNATURE, 8);
    if(prtkimg->secure_boot == 0) {
        rtk_fw_head.fw_tab.version = FIRMWARE_DESCRIPTION_TABLE_VERSION_02; //non-secure boot
        rtk_fw_head.secure_boot = 0;
    }
    else {
        rtk_fw_head.fw_tab.version = FIRMWARE_DESCRIPTION_TABLE_VERSION_22; //secure boot
        rtk_fw_head.secure_boot = 1;
    }

    // save
    ret = save_fwdesc_emmc(&rtk_fw_head, prtkimg);
    if(ret < 0)
    {
        install_debug("save_fwdesc() fail\r\n");
        fprintf(ui->cmd_pipe, "ui_print Burning RTK FWDesc fail!\n");
        fprintf(ui->cmd_pipe, "ui_print\n");
        return 0;
    }
    fprintf(ui->cmd_pipe, "ui_print Burning RTK FWDesc success!\n");
    fprintf(ui->cmd_pipe, "ui_print\n");
    return 1;
}

int Rtk_burn_fwdesc_nand(CRTKImgDesc* prtkimg, State* state)
{
    int ret, dev_fd=0, i;
    u8 fw_count;
    struct rtk_fw_header rtk_fw_head;
    struct rtk_fw_header nand_rtk_fw_head;
    int efwtype;

    memset(&rtk_fw_head, 0, sizeof(rtk_fw_head));

    // sanity-check
    if(prtkimg->fw[FW_KERNEL].img_size == 0 || prtkimg->fw[FW_RESCUE_DT].img_size == 0 \
            || prtkimg->fw[FW_KERNEL_DT].img_size == 0 || prtkimg->fw[FW_AUDIO].img_size == 0)
    {
        install_debug("FW_KERNEL(%u), FW_RESCUE_DT(%u), FW_KERNEL_DT(%u), FW_AUDIO(%u) zero check.\r\n"  \
                      , prtkimg->fw[FW_KERNEL].img_size   \
                      , prtkimg->fw[FW_RESCUE_DT].img_size \
                      , prtkimg->fw[FW_KERNEL_DT].img_size   \
                      , prtkimg->fw[FW_AUDIO].img_size);
        return -1;
    }

    UpdaterInfo* ui = (UpdaterInfo*)(state->cookie);
    fprintf(ui->cmd_pipe, "ui_print Starting burning RTK FWDesc\n");
    fprintf(ui->cmd_pipe, "ui_print\n");

    // read fw_header from emmc first to fill partition info later
    if ((dev_fd = open(prtkimg->mtdblock_path, O_RDWR|O_SYNC)) < 0)
    {
        install_err("error! open file %s fail\r\n", prtkimg->mtdblock_path);
        return 0;
    }
    lseek(dev_fd, prtkimg->reserved_boot_size, SEEK_SET);
    read(dev_fd, (void*)&nand_rtk_fw_head, sizeof(nand_rtk_fw_head));

#ifdef FWDES_DUMP
    dump_fw_desc_table_v1(&(emmc_rtk_fw_head.fw_tab));
#endif

    install_log("fw cnt:%d ; part cnt:%d\r\n",nand_rtk_fw_head.fw_count, nand_rtk_fw_head.part_count);

    if (nand_rtk_fw_head.fw_tab.part_list_len != 0)
    {
        nand_rtk_fw_head.part_count = nand_rtk_fw_head.fw_tab.part_list_len / sizeof(part_desc_entry_v2_t);
    }
    nand_rtk_fw_head.fw_count = nand_rtk_fw_head.fw_tab.fw_list_len / sizeof(fw_desc_entry_v2_t);
    install_log("fw cnt:%d ; part cnt:%d\r\n",nand_rtk_fw_head.fw_count, nand_rtk_fw_head.part_count);
#ifdef FWDES_DUMP
    for(i=0; i<(int)nand_rtk_fw_head.part_count; i++)
    {
        install_log("part desc[%d]\r\n",i);
        dump_part_desc_entry_v1(&(nand_rtk_fw_head.part_desc[i]));
    }
    for(i=0; i<(int)nand_rtk_fw_head.fw_count; i++)
    {
        install_log("fw desc[%d]\r\n",i);
        dump_rtk_fw_desc_entry_v21(&(nand_rtk_fw_head.fw_desc[i]));
    }
#endif
    close(dev_fd);

    // "fw_count" is the number of firmare that reside in partition[0]
    // "rtk_fw_head.fw_count" is the total number of firmware that reside in the install image.

    // fw field
    // kernel fw_desc
    fw_count = 1;
    rtk_fw_head.fw_count = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL;
    if (!strcmp(prtkimg->fw[FW_KERNEL].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL].sha_hash, SHA256_SIZE);

    // Rescue_DT fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_RESCUE_DT;
    if (!strcmp(prtkimg->fw[FW_RESCUE_DT].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_RESCUE_DT].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_RESCUE_DT].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_RESCUE_DT].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_RESCUE_DT].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_RESCUE_DT].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_RESCUE_DT].sha_hash, SHA256_SIZE);

    // Kernel_DT fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL_DT;
    if (!strcmp(prtkimg->fw[FW_KERNEL_DT].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL_DT].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL_DT].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL_DT].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL_DT].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL_DT].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL_DT].sha_hash, SHA256_SIZE);

    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_RESCUE_ROOTFS;
    if (!strcmp(prtkimg->fw[FW_RESCUE_ROOTFS].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_RESCUE_ROOTFS].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_RESCUE_ROOTFS].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_RESCUE_ROOTFS].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_RESCUE_ROOTFS].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_RESCUE_ROOTFS].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_RESCUE_ROOTFS].sha_hash, SHA256_SIZE);

    // NAS: kernel rootfs fw_desc
    if(prtkimg->fw[FW_KERNEL_ROOTFS].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_KERNEL_ROOTFS;
        if (!strcmp(prtkimg->fw[FW_KERNEL_ROOTFS].compress_type, "lzma"))
            rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_KERNEL_ROOTFS].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_KERNEL_ROOTFS].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_KERNEL_ROOTFS].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_KERNEL_ROOTFS].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_KERNEL_ROOTFS].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_KERNEL_ROOTFS].sha_hash, SHA256_SIZE);
    }

    // audio fw_desc
    fw_count++;
    rtk_fw_head.fw_count++;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_AUDIO;
    if (!strcmp(prtkimg->fw[FW_AUDIO].compress_type, "lzma"))
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].lzma = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_AUDIO].mem_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_AUDIO].flash_offset;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_AUDIO].img_size;//
    rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_AUDIO].flash_allo_size;//
    //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_AUDIO].checksum;
    memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_AUDIO].sha_hash, SHA256_SIZE);

    // audio bootfile fw_desc
    if(prtkimg->fw[FW_AUDIO_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_AUDIO_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_AUDIO_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_AUDIO_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_AUDIO_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_AUDIO_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_AUDIO_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    // image bootfile fw_desc
    if(prtkimg->fw[FW_IMAGE_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_IMAGE_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_IMAGE_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_IMAGE_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_IMAGE_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_IMAGE_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    // video bootfile fw_desc
    if(prtkimg->fw[FW_VIDEO_BOOTFILE].img_size != 0)
    {
        fw_count++;
        rtk_fw_head.fw_count++;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].type = FW_TYPE_VIDEO_FILE;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].ro = 1;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].version = 0;
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].target_addr = prtkimg->fw[FW_VIDEO_BOOTFILE].mem_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].offset = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_offset;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].length = prtkimg->fw[FW_VIDEO_BOOTFILE].img_size;//
        rtk_fw_head.fw_desc[rtk_fw_head.fw_count].paddings = prtkimg->fw[FW_VIDEO_BOOTFILE].flash_allo_size;//
        //rtk_fw_head.fw_desc[rtk_fw_head.fw_count].checksum = prtkimg->fw[FW_VIDEO_BOOTFILE].checksum;
        memcpy(rtk_fw_head.fw_desc[rtk_fw_head.fw_count].sha_hash, prtkimg->fw[FW_VIDEO_BOOTFILE].sha_hash, SHA256_SIZE);
    }

    rtk_fw_head.fw_count++;

    // partition info in fw_enrty
    // partition info refill
    rtk_fw_head.part_count = nand_rtk_fw_head.part_count;
    memcpy(&(rtk_fw_head.part_desc), &(nand_rtk_fw_head.part_desc), sizeof(rtk_fw_head.part_desc));

// fw_tab
    memcpy(rtk_fw_head.fw_tab.signature, FIRMWARE_DESCRIPTION_TABLE_SIGNATURE, 8);
    //rtk_fw_head.fw_tab.checksum
    if(prtkimg->secure_boot == 0) {
        rtk_fw_head.fw_tab.version = FIRMWARE_DESCRIPTION_TABLE_VERSION_02; //non-secure boot
        rtk_fw_head.secure_boot = 0;
    }
    else {
        rtk_fw_head.fw_tab.version = FIRMWARE_DESCRIPTION_TABLE_VERSION_22; //secure boot
        rtk_fw_head.secure_boot = 1;
    }
    rtk_fw_head.fw_tab.paddings = prtkimg->mtd_erasesize;

    // save
    ret = save_fwdesc(&rtk_fw_head, prtkimg);
    if(ret < 0)
    {
        install_debug("save_fwdesc() fail\r\n");
        return -1;
    }

    fprintf(ui->cmd_pipe, "ui_print Burning RTK FWDesc success!\n");
    fprintf(ui->cmd_pipe, "ui_print\n");
    return 1;
}


int Rtk_burn_factory(CRTKImgDesc* prtkimgdesc, ZipArchive* za,State* state)
{
    char cmd[128] = {0}, path[128] = {0};
    char workpath[128] = {0};
    int fd,ret;

    if (!prtkimgdesc->factory)
        return 0;

    const ZipEntry* factoryEntry = mzFindZipEntry(za, "factory.tar");
    if (factoryEntry == NULL)
    {
        install_info("no factory.tar in pkg\n");
        return 0;
    }
    snprintf(path, sizeof(path), "%s.tar", FACTORY_INSTALL_TEMP);
    fd = creat(path, 0755);

    memset(&cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "/sbin/factory load");
    rtk_command(cmd, __LINE__, __FILE__, 1);


    if (mzExtractZipEntryToFile(za,factoryEntry,fd))
    {
        snprintf(cmd, sizeof(cmd), "rm -rf %s;mkdir -p %s;tar -xf %s -C %s/"
                 , FACTORY_INSTALL_TEMP
                 , FACTORY_INSTALL_TEMP
                 , path
                 , FACTORY_INSTALL_TEMP);
        if((ret = rtk_command(cmd, __LINE__, __FILE__, 0)) < 0)
        {
            install_err("untar %s.tar fail\r\n", FACTORY_INSTALL_TEMP);
            return 0;
        }
    }

    getcwd(workpath, 128);
    chdir(FACTORY_TEMP);
    if (strlen(prtkimgdesc->ifcmd0) > 0)
    {
        install_log("\r\n[ifcmd0]\r\n");
        if((ret = rtk_command(prtkimgdesc->ifcmd0, __LINE__, __FILE__, 1)) < 0)
        {
            install_err("install factory command0 fail!\r\n");
        }
    }
    if (strlen(prtkimgdesc->ifcmd1) > 0)
    {
        install_log("\r\n[ifcmd1]\r\n");
        if((ret = rtk_command(prtkimgdesc->ifcmd1, __LINE__, __FILE__, 1)) < 0)
        {
            install_debug("install factory command1 fail!\r\n");
        }
    }
    chdir(workpath);
    memset(&cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "/sbin/factory save");
    rtk_command(cmd, __LINE__, __FILE__, 1);

    return 1;
}

int Rtk_load_fwdesc_emmc(CRTKImgDesc* prtkimg, State* state)
{
    int ret=0, dev_fd=0, i;
    fw_desc_table_t fw_tab;
    part_desc_entry_v2_t part_desc[10];
    rtk_fw_desc_entry_v22_t fw_desc[15];
    unsigned int part_count;
    unsigned int fw_count;
    fw_type_code_t fwtype;
    unsigned int reserved_size = 0x620000;

    memset(&fw_tab, 0, sizeof(fw_desc_table_t));
    memset(&part_desc, 0, sizeof(part_desc_entry_v2_t));
    memset(&fw_desc, 0, sizeof(rtk_fw_desc_entry_v22_t));

    printf("Rtk_load_fwdesc_emmc start.\n");

    UpdaterInfo* ui = (UpdaterInfo*)(state->cookie);
    fprintf(ui->cmd_pipe, "ui_print Starting load RTK FWDesc\n");
    fprintf(ui->cmd_pipe, "ui_print\n");

    // read fw_header from emmc first to fill partition info later
    if ((dev_fd = open(prtkimg->mtdblock_path, O_RDWR|O_SYNC)) < 0)
    {
        install_err("error! open file %s fail\r\n", prtkimg->mtdblock_path);
        return 0;
    }
    lseek(dev_fd, reserved_size, SEEK_SET);
    read(dev_fd, (void*)&fw_tab, sizeof(fw_desc_table_t));
    //dump_fw_desc_table(&fw_tab);
    part_count = fw_tab.part_list_len / sizeof(part_desc_entry_v2_t);
    if (prtkimg->secure_boot == 0)
        fw_count = fw_tab.fw_list_len / sizeof(fw_desc_entry_v2_t);
    else {
        install_info("Secure boot.....\r\n");
        fw_count = fw_tab.fw_list_len / sizeof(fw_desc_entry_v22_t);
    }
    install_info("fw cnt:%d ; part cnt:%d\r\n",fw_count, part_count);

    lseek(dev_fd, fw_tab.part_list_len, SEEK_CUR);
    for(i=0; i<(int)fw_count; i++)
    {
        if (prtkimg->secure_boot == 0)
            read(dev_fd, (void*)&fw_desc[i], sizeof(fw_desc_entry_v2_t));
        else
            read(dev_fd, (void*)&fw_desc[i], sizeof(rtk_fw_desc_entry_v22_t));
        //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));

        fwtype = (fw_type_code_t)fw_desc[i].type;
        if (fwtype == FW_TYPE_IMAGE_FILE)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_IMAGE_BOOTFILE].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_IMAGE_BOOTFILE].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_IMAGE_BOOTFILE].img_size = fw_desc[i].length;
            prtkimg->fw[FW_IMAGE_BOOTFILE].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_IMAGE_BOOTFILE].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_IMAGE_BOOTFILE].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_IMAGE_BOOTFILE].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_IMAGE_BOOTFILE].compress_type, "bin");
        }
        else if (fwtype == FW_TYPE_UBOOT)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_UBOOT].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_UBOOT].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_UBOOT].img_size = fw_desc[i].length;
            prtkimg->fw[FW_UBOOT].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_UBOOT].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_UBOOT].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_UBOOT].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_UBOOT].compress_type, "bin");
        }
#ifdef NFLASH_LAOUT
        else if (fwtype == FW_TYPE_GOLD_KERNEL)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_GOLD_KERNEL].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_GOLD_KERNEL].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_GOLD_KERNEL].img_size = fw_desc[i].length;
            prtkimg->fw[FW_GOLD_KERNEL].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_GOLD_KERNEL].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_GOLD_KERNEL].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_GOLD_KERNEL].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_GOLD_KERNEL].compress_type, "bin");
        }
        else if (fwtype == FW_TYPE_GOLD_RESCUE_DT)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_GOLD_RESCUE_DT].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_GOLD_RESCUE_DT].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_GOLD_RESCUE_DT].img_size = fw_desc[i].length;
            prtkimg->fw[FW_GOLD_RESCUE_DT].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_GOLD_RESCUE_DT].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_GOLD_RESCUE_DT].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_GOLD_RESCUE_DT].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_GOLD_RESCUE_DT].compress_type, "bin");
        }
        else if (fwtype == FW_TYPE_GOLD_RESCUE_ROOTFS)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].img_size = fw_desc[i].length;
            prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_GOLD_RESCUE_ROOTFS].compress_type, "bin");
        }
        else if (fwtype == FW_TYPE_GOLD_AUDIO)
        {
            install_info("fw_type=[%d]\r\n]", fwtype);
            //dump_rtk_fw_desc_entry_v22(&(fw_desc[i]));
            prtkimg->fw[FW_GOLD_AUDIO].mem_offset = fw_desc[i].target_addr;
            prtkimg->fw[FW_GOLD_AUDIO].flash_offset = fw_desc[i].offset;
            prtkimg->fw[FW_GOLD_AUDIO].img_size = fw_desc[i].length;
            prtkimg->fw[FW_GOLD_AUDIO].flash_allo_size = fw_desc[i].paddings;
            //prtkimg->fw[FW_GOLD_AUDIO].checksum = fw_desc[i].checksum;
            memcpy(prtkimg->fw[FW_GOLD_AUDIO].sha_hash, fw_desc[i].sha_hash, SHA256_SIZE);
            if (fw_desc[i].lzma == 1)
                strcpy(prtkimg->fw[FW_GOLD_AUDIO].compress_type, "lzma");
            else
                strcpy(prtkimg->fw[FW_GOLD_AUDIO].compress_type, "bin");
        }
#endif
    }
    close(dev_fd);
    sync();

    if ((fw_tab.version == FIRMWARE_DESCRIPTION_TABLE_VERSION_01) || (fw_tab.version == FIRMWARE_DESCRIPTION_TABLE_VERSION_21))
    {
        install_err("Internal flash data vesion is not support upgrade.\n");
        return 0;
    }
    return 1;
}

