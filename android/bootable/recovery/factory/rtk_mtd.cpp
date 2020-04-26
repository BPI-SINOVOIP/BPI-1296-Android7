#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
//#include <mtd/mtd-user.h>
#include <unistd.h>
#include <string.h>
#include <rtk_mtd.h>
#include <stdlib.h>
//#include <rtk_imgdesc.h>
//#include <rtk_fwdesc.h>


#define _128K_BYTE	128*1024U
#define _512K_BYTE	512*1024U
#define   _2M_BYTE	2*1024*1024U
#define   _4M_BYTE	4*1024*1024U
#define   _8M_BYTE	8*1024*1024U

static char MTD_CHAR_DEV_PATH[][20]=
{
    "/dev/mtd/disc",
    "/dev/mtd/mtddisc",
    "/dev/mtddisc",
    "/dev/mtd0",
    "/dev/mtd/mtd0",
    "/dev/mtd0ro",
    "/dev/mtd/0"
};
static char MTD_BLOCK_DEV_PATH[][50]=
{
    "/dev/mmcblk0",			// for linux
    "/dev/mmcblk1",
    "/dev/block/mmcblk0",	// for android
    "/dev/block/mmcblk1",
    "/dev/mtdblock4",
    "/dev/block/mtdblockdisc",
    "/dev/mtdblockdisc",
    "/dev/mtdblock/mtdblockdisc",
    "/dev/mtdblock/disc",
    "/dev/mtdblock0",
    "/dev/block/mtdblock0",
    "/dev/mtdblock/0",
    "/dev/mtdblock/mtdblock0",
    "/dev/block/xvda"
};
static char* pblock_name=NULL;
static char* pchar_name=NULL;

/* Get basic MTD characteristics info (better to use sysfs) */
#define MEMGETINFO       _IOR('M', 1, struct mtd_info_user)
/* Erase segment of MTD */
#define MEMERASE         _IOW('M', 2, struct erase_info_user)
/* Unlock a chip (for MTD that supports it) */
#define MEMUNLOCK	 _IOW('M', 6, struct erase_info_user)

#ifdef RTK_VMX
typedef struct {
    unsigned char magicNum[8];
    unsigned long long imSize;
    unsigned char others[496];
}VMX_Block_Info;
#endif

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
    //install_info("Open MTD_CHAR %s\r\n", MTD_CHAR_DEV_PATH[i]);
    if(NULL != ppstr) *ppstr = MTD_CHAR_DEV_PATH[i];
    return dev_fd;
}
int rtk_open_mtd_block(char** ppstr)
{
    int i;
    int dev_fd;

    i = 0;
    do
    {
        for(; i<sizeof(MTD_BLOCK_DEV_PATH)/sizeof(MTD_BLOCK_DEV_PATH[0]); i++)
        {
            dev_fd = open(MTD_BLOCK_DEV_PATH[i], O_RDWR|O_SYNC);
            if(-1!=dev_fd) break;
        }
        if(dev_fd < 0)
        {
            install_debug("open mtd block fail\r\n");
            return -1;
        }
        //install_info("Open MTD_BLOCK %s\r\n", MTD_BLOCK_DEV_PATH[i]);

        // device path for SD card is /dev/mmcblkX, same as MMC.
        // check if it is MMC or SD card..
        const char *blkptr = strstr(MTD_BLOCK_DEV_PATH[i], "mmcblk");
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
    while( i < (int)(sizeof(MTD_BLOCK_DEV_PATH)/sizeof(MTD_BLOCK_DEV_PATH[0]))  );

    if( i < (int)(sizeof(MTD_BLOCK_DEV_PATH)/sizeof(MTD_BLOCK_DEV_PATH[0])) )
    {
        if(NULL!=ppstr) *ppstr = MTD_BLOCK_DEV_PATH[i];
        pblock_name = MTD_BLOCK_DEV_PATH[i];
        return dev_fd;
    }

    return -1;
}
int get_mtd_block_name(char** ppstr)
{
    int ret;
    char* p;
    if(pblock_name != NULL)
    {
        *ppstr = pblock_name;
        return 0;
    }
    ret = rtk_open_mtd_block(&p);
    if(ret < 0)
    {
        return -1;
    }
    close(ret);
    *ppstr = p;
    return 0;
}

char* get_mtd_block_name_str(void)
{
    int ret;
    char* p;
    ret = rtk_open_mtd_block(&p);
    if(ret < 0)
    {
        return NULL;
    }
    close(ret);
    return p;
}

char *get_mtd_char_name(void)
{

    return pchar_name;
}
int rtk_open_mtd_block_with_offset(unsigned int offset)
{
    int i;
    int dev_fd;
    int ret;
    for(i=0; i<sizeof(MTD_BLOCK_DEV_PATH)/sizeof(MTD_BLOCK_DEV_PATH[0]); i++)
    {
        dev_fd = open(MTD_BLOCK_DEV_PATH[i], O_RDWR|O_SYNC);
        if(dev_fd != -1) break;
    }
    if(dev_fd < 0)
    {
        install_debug("open mtd block fail\r\n");
        return -1;
    }
    pblock_name = MTD_BLOCK_DEV_PATH[i];
    //install_info("Open MTD_BLOCK %s with %u(0x%08X) offset\r\n", MTD_BLOCK_DEV_PATH[i], offset, offset);
    ret = lseek(dev_fd, offset, SEEK_SET);
    if(ret < 0)
    {
        install_debug("lseek mtd block fail\r\n");
        return -1;
    }

    return dev_fd;
}

int rtk_mtd_erase(int fd, int start, int len)
{

    struct erase_info_user erase_u;
    erase_u.start =  start;
    erase_u.length = len;

    if (ioctl(fd, MEMERASE, &erase_u))
    {
        return -1;
    }
    return 0;
}
int rtk_mtd_char_erase(unsigned int address, unsigned int size)
{

    int dev_fd;
    struct erase_info_user erase;
    dev_fd = open(get_mtd_char_name(), O_RDWR | O_SYNC);
    if(-1 == dev_fd)
    {
        printf("open %s fail\n", get_mtd_char_name());
        return -1;
    }
    erase.start = address;
    erase.length = size;

    if (ioctl(dev_fd, MEMERASE, &erase) != 0)
    {
        printf("erase fail %x %x\n", address, size);
        close(dev_fd);
        return -1;
    }

    close(dev_fd);

    return 0;
}

int rtk_mtd_char_program(unsigned char *buf, unsigned int address, unsigned size)
{


    int dev_fd;
    int ret = 0;
    dev_fd = open(get_mtd_char_name(), O_RDWR | O_SYNC);

    if(-1 == dev_fd)
    {
        printf("open %s fail\n", get_mtd_char_name());
        return -1;
    }

    if (lseek(dev_fd, address, SEEK_SET) != address )
    {
        ret = -1;
        goto end_of_program;
    };
//    system("echo 3 > /sys/realtek_boards/rtice_enable");
    if (write(dev_fd, buf, size) != size)
    {
        printf("write fail %x %x\n", address, size);
        ret = -1;
        goto end_of_program;
    }

end_of_program:
//	system("echo 0 > /sys/realtek_boards/rtice_enable");
    close(dev_fd);
    return ret;

}


int rtk_mtd_char_verify(unsigned char *buf, unsigned int address, unsigned size)
{


    int dev_fd;
    unsigned char tmp[512];
    int ret = 0;
    int count;
    int data_size, data_size_read;
    dev_fd = open(get_mtd_char_name(), O_RDWR | O_SYNC);

    if(-1 == dev_fd)
    {
        printf("open %s fail\n", get_mtd_char_name());
        return -1;
    }

    if (lseek(dev_fd, address, SEEK_SET) != address)
    {
        ret = -1;
        goto end_of_verify;
    }
    for (count = 0; count < size; )
    {
        if ((size - count) < sizeof(tmp))
        {
            data_size = size - count;
        }
        else
        {
            data_size = sizeof(tmp);
        }
//		system("echo 3 > /sys/realtek_boards/rtice_enable");
        if ((data_size_read=read(dev_fd, tmp, data_size)) <= 0)
        {
            printf("read fail %x %x\n", address, size);
            ret = -1;
//			system("echo 0 > /sys/realtek_boards/rtice_enable");
            goto end_of_verify;
        }
        if (data_size_read != data_size)
        {
            printf("data_size is not equal %x %x\n", data_size_read, data_size);
            ret = -1;
            goto end_of_verify;
        }
        if (memcmp(tmp, buf, data_size))
        {
            printf("compare fail \n");
            ret = -1;
            goto end_of_verify;
        }
        count += data_size;
    }

end_of_verify:
//	system("echo 0 > /sys/realtek_boards/rtice_enable");
    close(dev_fd);

    return ret;

}

int rtk_get_meminfo(struct mtd_info_user* meminfo)
{
    int dev_fd;
    dev_fd = rtk_open_mtd_char(NULL);
    if(-1 == dev_fd)
        return -1;
    /* Get MTD device capability structure */
    if (ioctl(dev_fd, MEMGETINFO, meminfo))
    {
        close(dev_fd);
        return -1;
    }

    close(dev_fd);
    return 0;
}
uint32_t rtk_get_erasesize(void)
{
    int ret;
    struct mtd_info_user meminfo;
    ret = rtk_get_meminfo(&meminfo);
    if(ret < 0)
    {
        install_debug("rtk_get_meminfo fail\r\n");
        return 0;
    }
    //install_info("erasesize=0x%08x\r\n", meminfo.erasesize);
    return meminfo.erasesize;
}

int modify_addr_signature(unsigned int startAddress, unsigned int reserved_boot_size)
{
    int dev_fd;
    int ret;
    dev_fd = rtk_open_mtd_block_with_offset(reserved_boot_size);
    if(dev_fd < 0)
    {
        install_debug("rtk_open_mtd_block_with_offset() fail\r\n");
        return -1;
    }
    ret = write(dev_fd, "IMG_", 4);
    if(ret != 4)
    {
        install_debug("write signature fail\r\n");
        close(dev_fd);
        return -1;
    }
    ret = write(dev_fd, (const void *) &startAddress, 4);
    if(ret != 4)
    {
        install_debug("modify signature's num fail\r\n");
        close(dev_fd);
        return -1;
    }
    close(dev_fd);
    return 0;
}

int modify_signature(unsigned int reserved_boot_size, unsigned int flash_type)
{
    char* block_path;
    int ret;
    char command[128];
    ret = get_mtd_block_name(&block_path);
    if(ret < 0)
    {
        install_debug("get_mtd_block_name fail\r\n");
        return -1;
    }
    if(flash_type == MTD_NANDFLASH)
    {
        // modify the signature (first 8 bytes of boot table)
        sprintf(command, "echo -n RESCUE__ | dd of=%s bs=1 seek=%u", block_path, reserved_boot_size);
        ret = rtk_command(command, __LINE__, __FILE__);
    }
    else if(flash_type == MTD_NORFLASH || flash_type == MTD_DATAFLASH)
    {
        // modify the signature (first 8 bytes of boot table)
        sprintf(command, "echo -n RESCUE__ | dd of=%s bs=1 seek=0", block_path);
        ret = rtk_command(command, __LINE__, __FILE__);
    }
    else
    {
        install_debug("Unknown MTD TYPE\r\n");
        return -1;
    }
    return 0;
}

inline unsigned long long SIZE_ALIGN_BOUNDARY_MORE(unsigned long long len, unsigned long long size)
{
    return (((len - 1) & ~((size) - 1)) + size);
}

int get_flsh_info(BOOTTYPE* flash_type, unsigned int* factory_start, unsigned int* factory_size, unsigned int* erasesize)
{
    char* dev_path;
    int dev_fd;
    int ret;
    unsigned int reservedSize, ubootSize;
    struct mtd_info_user meminfo;

    //open mtd block device

    dev_fd = rtk_open_mtd_block(&dev_path);
    if(-1 == dev_fd)
    {
        return -1;
    }
    close(dev_fd);

    if (strstr(dev_path, "mmcblk")||strstr(dev_path, "xvda"))
    {
        *flash_type = BOOT_EMMC;
        *factory_start = 0x220000;
        *factory_size = _4M_BYTE;
        *erasesize = 512;
        return 0;
    }
    else
    {
        //open mtd char device
        dev_fd = rtk_open_mtd_char(&dev_path);
        if(-1 == dev_fd)
        {
            return -1;
        }
        /* Get MTD device capability structure */
        memset(&meminfo, 0, sizeof(struct mtd_info_user));
        if (ioctl(dev_fd, MEMGETINFO, &meminfo) == 0)
        {
            // flash info
            if (meminfo.type == MTD_NANDFLASH)
            {
                *flash_type = BOOT_NAND;
                *erasesize = meminfo.erasesize;
                ubootSize = _512K_BYTE;
                *factory_size = _8M_BYTE;
                reservedSize = SIZE_ALIGN_BOUNDARY_MORE(ubootSize, meminfo.erasesize)*4;
                reservedSize += SIZE_ALIGN_BOUNDARY_MORE(*factory_size, meminfo.erasesize);
                reservedSize += (6+1*4) * meminfo.erasesize; //NF profile+BBT=6blocks, hwsetting=1block
                // add extra 20% space for safety.
                reservedSize = SIZE_ALIGN_BOUNDARY_MORE(reservedSize*1.2, meminfo.erasesize);
                *factory_start = reservedSize - *factory_size;
            }
            else if(meminfo.type == MTD_NORFLASH)
            {
                *flash_type = BOOT_SPI;
            }
            close(dev_fd);
        }
        else
        {
            install_fail("Get flash info error!\r\n");
            close(dev_fd);
            return -1;
        }
        return 0;
    }
}

#ifdef RTK_VMX
int rtk_mtd_write_factory_install_flag(unsigned long long file_size){
    VMX_Block_Info VmxBlock;
    unsigned char magicNum_tmp[8] = {0x99, 0x88, 0x77, 0x78, 0x90, 0x56, 0x34, 0x12};
    int ret = 0;
    unsigned int reserved_boot_size = 0x99*0x200;

    int dfd;
    memset(&VmxBlock, 0, sizeof(VmxBlock));
    memcpy(VmxBlock.magicNum, magicNum_tmp, sizeof(VmxBlock.magicNum));

    VmxBlock.imSize=file_size;

    dfd = open("/dev/block/mmcblk0", O_RDWR|O_SYNC);
    if(dfd<0)
    {
        install_fail("Can't open /dev/block/mmcblk0\r\n");
        return -1;
    }

    ret = lseek64(dfd, reserved_boot_size, SEEK_SET);
    if (ret == (off64_t) -1)
    {
        install_fail("lseek (/dev/block/mmcblk0) fail\r\n");
        close(dfd);
        return -1;
    }
    ret = write(dfd, (void*) &VmxBlock, sizeof(VmxBlock));
    if(ret < 0)
    {
        install_fail("write fail\r\n");
        close(dfd);
        return -1;
    }

    close(dfd);
    return 0;
}
#endif