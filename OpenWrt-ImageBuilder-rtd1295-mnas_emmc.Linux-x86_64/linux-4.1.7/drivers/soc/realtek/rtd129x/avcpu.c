/*
 * Realtek Audio & Video cpu driver
 *
 * Copyright (c) 2011 EJ Hsu <ejhsu@realtek.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 * History:
 *
 * 2011-12-02:  EJ Hsu: first version
 * 2013-05-02:  EJ Hsu: port to arm platform
 */

#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <soc/realtek/memory.h>
#include <soc/realtek/rtk_ipc_shm.h>

#include "avcpu.h"

#define CRT_BASE	0x98000000
#define EMMC_FW_TABLE_OFFSET  0x620000

#define HEADER_OFFSET         EMMC_FW_TABLE_OFFSET//0x2000000
#define EMMC_BLOCK_SIZE       (PAGE_SIZE / 8)

#define MIPS_KSEG0BASE 0x80000000

#define BYTE(d,n)       (((d) >> ((n) << 3)) & 0xFF)
#define SWAPEND32(d)    ((BYTE(d,0) << 24) | (BYTE(d,1) << 16) | (BYTE(d,2) << 8) | (BYTE(d,3) << 0))
#define SWAPEND64(d)    ((BYTE(d,0) << 56) | (BYTE(d,1) << 48) | (BYTE(d,2) << 40)| (BYTE(d,3) << 32) |\
        (BYTE(d,4) << 24) | (BYTE(d,5) << 16) | (BYTE(d,6) << 8) | (BYTE(d,7) << 0))

#define BE32_TO_CPU(value)      (value)
#define CPU_TO_BE32(value)      SWAPEND32(value)
#define BE64_TO_CPU(value)      (value)
#define CPU_TO_BE64(value)      SWAPEND64(value)

//#define av_be32_to_cpu(x)   htonl(x)

#define DEVICE_EMMC             1
#define BLK_READ_SIZE		SZ_256K

static void __iomem *crt_base = NULL;
static void __iomem *rpc_base = NULL;

#define AVCPU_IPC_SHM	(rpc_base + 0xc4)

#ifdef CONFIG_PM
static int avcpu_suspend(struct platform_device *dev, pm_message_t state);
static int avcpu_resume(struct platform_device *dev);
#endif

static BLOCKING_NOTIFIER_HEAD(avcpu_chain_head);

static avcpu_info avpriv;
static struct class *avcpu_class = NULL;
static struct cdev avcpu_cdev;
static struct platform_device *avcpu_platform_devs = NULL;
static struct platform_driver avcpu_device_driver = {
#ifdef CONFIG_PM
    .suspend        = avcpu_suspend,
    .resume         = avcpu_resume,
#endif
    .driver = {
        .name       = "avcpu",
        .bus        = &platform_bus_type,
    },
};

static int avcpu_ipc_shm_reset(void)
{
    int ret = 0;
    struct RTK119X_ipc_shm old_shm = {0};
    struct RTK119X_ipc_shm new_shm = {0};

    memcpy(&old_shm, AVCPU_IPC_SHM, sizeof(struct RTK119X_ipc_shm));

    /* start to copy some static variable from old shm */
    new_shm.sys_assign_serial = old_shm.sys_assign_serial;
    new_shm.pov_boot_vd_std_ptr = old_shm.pov_boot_vd_std_ptr; // maybe this item should be changed since it's one-step info?
    new_shm.u_boot_version_magic = old_shm.u_boot_version_magic;
    new_shm.u_boot_version_info = old_shm.u_boot_version_info;

    memcpy_toio(AVCPU_IPC_SHM, &new_shm, sizeof(struct RTK119X_ipc_shm));

    return ret;
}

int register_avcpu_notifier(struct notifier_block *nb)
{
    return blocking_notifier_chain_register(&avcpu_chain_head, nb);
}
EXPORT_SYMBOL_GPL(register_avcpu_notifier);

int unregister_avcpu_notifier(struct notifier_block *nb)
{
    return blocking_notifier_chain_unregister(&avcpu_chain_head, nb);
}
EXPORT_SYMBOL_GPL(unregister_avcpu_notifier);

int avcpu_notifier_call_chain(unsigned long val)
{
    return (blocking_notifier_call_chain(&avcpu_chain_head, val, NULL) == NOTIFY_BAD) ? -EINVAL : 0;
}

#ifdef CONFIG_PM
static int avcpu_suspend(struct platform_device *dev, pm_message_t state)
{
#if 1
    avcpu_notifier_call_chain(AVCPU_SUSPEND);
#else
    avpriv.status = AVSTAT_SUSPEND;
    pr_info("\033[33m%s status:%d\033[m\n", __func__, avpriv.status);
    //avcpu_notifier_call_chain(AVCPU_SUSPEND);
    //avcpu_notifier_call_chain(AVCPU_RESET_PREPARE);
    //resetav_lock(1);
    //load_av_image(NULL, 0, NULL, 0, NULL, 0);
#endif
    return 0;
}

static int avcpu_resume(struct platform_device *dev)
{
#if 1
    avcpu_notifier_call_chain(AVCPU_RESUME);
#else
    avpriv.status = AVSTAT_RESUME;
    pr_info("\033[33m%s status:%d\033[m\n", __func__, avpriv.status);
    wake_up_interruptible(&avpriv.status_wq);
    //resetav_unlock(1);
    //avcpu_notifier_call_chain(AVCPU_RESET_DONE);
    //avcpu_notifier_call_chain(AVCPU_RESUME);
#endif
    return 0;
}
#endif

static int avcpu_reset(resetav_struct *rs)
{
    blocking_notifier_call_chain(&avcpu_chain_head, AVCPU_RESET_PREPARE, NULL);
    //resetav_lock(0);
    load_av_image(rs->audio_image, rs->audio_start,
            rs->video_image_1, rs->video_start_1, rs->video_image_2, rs->video_start_2);
    //resetav_unlock(0);
    blocking_notifier_call_chain(&avcpu_chain_head, AVCPU_RESET_DONE, NULL);
    avpriv.status = AVSTAT_NORMAL;
    pr_info("\033[33m%s:%d status:%d\033[m\n", __func__, __LINE__, avpriv.status);
    return 0;
}

static int avcpu_stop(void)
{
    unsigned int mask = 0;

    blocking_notifier_call_chain(&avcpu_chain_head, AVCPU_RESET_PREPARE, NULL);

    writel(0x4, crt_base + 0x264); //PLL_VODMA2
    writel(0x0, crt_base + 0x24);  //DISP_PLL_DIV2
    writel(0x0, crt_base + 0x190); //PLL_HDMI

    /* Turn off CLK */
    writel(readl(crt_base + 0x10) & ~BIT(4), crt_base + 0x10); //CLK : ACPU
    mask = ~(unsigned int)(BIT(17) | BIT(15) | BIT(14) | BIT(8));
    writel(readl(crt_base + 0x0c) & mask, crt_base + 0x0c); //CLK : SE, VO, TVE, HDMI

    /* Hold RST */
    writel(readl(crt_base + 0x04) & ~BIT(0), crt_base + 0x04); //RST : ACPU
    mask = ~(unsigned int)(BIT(28) | BIT(22) | BIT(20) | BIT(19) | BIT(12));
    writel(readl(crt_base) & mask, crt_base); // RST : AE, SE, VO, TVE, HDMI
    writel(readl(crt_base + 0x50) & ~BIT(15), crt_base + 0x50); //RST : DISP
    pr_info("\033[33mstop avcpu\033[m\n");
    return 0;
}

static int avcpu_start(void)
{
    writel(readl(crt_base + 0x04) | BIT(0), crt_base + 0x04);
    writel(readl(crt_base + 0x10) | BIT(4), crt_base + 0x10);
    pr_info("\033[33mstart avcpu\033[m\n");
    return 0;
}

long avcpu_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //avcpu_info *info = (avcpu_info *)filp->private_data;
    resetav_struct rs;
    int ret = 0;

    switch (cmd) {
        case AVCPU_IOCTRESET:
            ret = copy_from_user(&rs, (void *)arg, sizeof(rs));
            if (ret != 0)
                break;
            avcpu_reset(&rs);
            break;
        case AVCPU_IOCTSTOP:
            avcpu_stop();
            break;
        case AVCPU_IOCTSTART:
            avcpu_start();
            break;
        case AVCPU_IOCGSTATUS:
            {
                status_struct *st = (status_struct *)arg;
                ret = wait_event_interruptible_timeout(avpriv.status_wq, (avpriv.status >= AVSTAT_RESUME), st->timeout * HZ / 1000);
                st->status = avpriv.status;
                if(avpriv.status == AVSTAT_RESUME)
                    avpriv.status = AVSTAT_RESETAV;
                //pr_debug("\033[33m%s:%d status:%d\033[m\n", __func__, __LINE__, avpriv.status);
                break;
            }
        default:
            ret = -EINVAL;
            break;
    };

    return ret;
}

struct file_operations avcpu_fops = {
    .owner                  = THIS_MODULE,
    .unlocked_ioctl         = avcpu_ioctl,
};

static char *avcpu_devnode(struct device *dev, umode_t *mode)
{
    return NULL;
}

static unsigned int get_device_type(void)
{
    return DEVICE_EMMC;
}

static int load_av_from_file(int cpu, char *load_file, unsigned int load_addr)
{
#if 1
    pr_err("avcpu: load from file not supported yet!\n");
    return -1;
#else
    struct file *file;
    int file_size;

    printk("avcpu: load cpu %d from file %s to %x...\n", cpu, load_file, load_addr);
    file = filp_open(load_file, O_RDONLY, 0444);
    if (IS_ERR(file))
        return PTR_ERR(file);
    file_size = file->f_dentry->d_inode->i_size;
    printk("file: %p size: %d \n", file, file_size);
    if (kernel_read(file, 0, (char *)load_addr, file_size) != file_size)
        BUG();
    filp_close(file, 0);

    switch (cpu) {
        case AUDIO_CPU:
            *((unsigned long *)AUDIO_ENTRY_ADDR) = htonl(load_addr);
            printk("audio entry value: %08lx \n", *((unsigned long *)AUDIO_ENTRY_ADDR));
            break;
        case VIDEO_CPU_1:
            *((unsigned long *)VIDEO_ENTRY_ADDR_1) = htonl(load_addr);
            printk("video 1 entry value: %08lx \n", *((unsigned long *)VIDEO_ENTRY_ADDR_1));
            break;
        case VIDEO_CPU_2:
            *((unsigned long *)VIDEO_ENTRY_ADDR_2) = htonl(load_addr);
            printk("video 2 entry value: %08lx \n", *((unsigned long *)VIDEO_ENTRY_ADDR_2));
            break;
        default:
            printk("avcpu: unknown cpu...\n");
            return -ENODEV;
    }

    // flush the data cache...
#ifdef CONFIG_MIPS
    data_cache_flush(load_addr, file_size);
#endif

    return 0;
#endif
}

#ifdef CONFIG_MMC_RTK_EMMC
extern int mmc_fast_read(unsigned int, unsigned int, unsigned char *);
#else
int mmc_fast_read( unsigned int blk_addr,
        unsigned int data_size,
        unsigned char * buffer )
{
    /*TODO : avcpu porting by victor 20140211*/
    return -1;
}
#endif

static int read_emmc_data(unsigned int blk_offset, size_t size, void *buf)
{
    int ret = 0;
    unsigned int blk_end = 0;
    size_t blk_read_size = 0;

    size = (size + (EMMC_BLOCK_SIZE -1)) & ~(EMMC_BLOCK_SIZE - 1);
    blk_end = blk_offset + (size / EMMC_BLOCK_SIZE);
    blk_read_size = BLK_READ_SIZE / EMMC_BLOCK_SIZE;
    pr_info("avcpu: %s offset: %x, size: %lx, buf: %p \n", __func__, blk_offset, size / EMMC_BLOCK_SIZE, buf);

    while (blk_offset < blk_end) {
        if (blk_end - blk_offset < (BLK_READ_SIZE / EMMC_BLOCK_SIZE))
            blk_read_size = blk_end - blk_offset;
        if ((ret = mmc_fast_read(blk_offset, blk_read_size, buf)) < 0)
            break;
        blk_offset += BLK_READ_SIZE / EMMC_BLOCK_SIZE;
        buf += BLK_READ_SIZE;
    }

    return ret;
}

unsigned long mips_to_arm(unsigned long addr)
{
    return (unsigned long)__va(addr & 0x1fffffff);
}

//#define DUBUG_FW_DESC_TABLE
#ifdef DUBUG_FW_DESC_TABLE
void dump_fw_desc_table_v1(fw_desc_table_v1_t *fw_desc_table_v1)
{
    if (fw_desc_table_v1 != NULL) {
        printk("## Fw Desc Table ##############################\n");
        printk("## fw_desc_table_v1                = 0x%08lx\n", (unsigned long)fw_desc_table_v1);
        printk("## fw_desc_table_v1->signature     = %s\n", fw_desc_table_v1->signature);
        printk("## fw_desc_table_v1->checksum      = 0x%08x\n", fw_desc_table_v1->checksum);
        printk("## fw_desc_table_v1->version       = 0x%08x\n", fw_desc_table_v1->version);
        printk("## fw_desc_table_v1->paddings      = 0x%08x\n", fw_desc_table_v1->paddings);
        printk("## fw_desc_table_v1->part_list_len = 0x%08x\n", fw_desc_table_v1->part_list_len);
        printk("## fw_desc_table_v1->fw_list_len   = 0x%08x\n", fw_desc_table_v1->fw_list_len);
        printk("###############################################\n\n");
    }
    else {
        printk("[ERR] %s:%d fw_desc_table_v1 is NULL.\n", __FUNCTION__, __LINE__);
    }
}

void dump_part_desc_entry_v1(part_desc_entry_v1_t *part_entry)
{
    if (part_entry != NULL) {
        printk("## Part Desc Entry ############################\n");
        printk("## part_entry                      = 0x%08lx\n", (unsigned long)part_entry);
        printk("## part_entry->type                = 0x%08x\n", part_entry->type);
        printk("## part_entry->ro                  = 0x%08x\n", part_entry->ro);
        printk("## part_entry->length              = %llu\n", part_entry->length);
        printk("## part_entry->fw_count            = 0x%08x\n", part_entry->fw_count);
        printk("## part_entry->fw_type             = 0x%08x\n", part_entry->fw_type);
        printk("###############################################\n\n");
    }
    else {
        printk("[ERR] %s:%d part_entry is NULL.\n", __FUNCTION__, __LINE__);
    }
}

void dump_fw_desc_entry_v1(fw_desc_entry_v1_t *fw_entry)
{
    if (fw_entry != NULL) {
        printk("## Fw Desc Entry ##############################\n");
        printk("## fw_entry                        = 0x%08lx\n", (unsigned long)fw_entry);
        printk("## fw_entry->type                  = 0x%08x\n", fw_entry->type);
        printk("## fw_entry->lzma                  = 0x%08x\n", fw_entry->lzma);
        printk("## fw_entry->ro                    = 0x%08x\n", fw_entry->ro);
        printk("## fw_entry->version               = 0x%08x\n", fw_entry->version);
        printk("## fw_entry->target_addr           = 0x%08x\n", fw_entry->target_addr);
        printk("## fw_entry->offset                = 0x%08x\n", fw_entry->offset);
        printk("## fw_entry->length                = 0x%08x\n", fw_entry->length);
        printk("## fw_entry->paddings              = 0x%08x\n", fw_entry->paddings);
        printk("## fw_entry->checksum              = 0x%08x\n", fw_entry->checksum);
        printk("###############################################\n\n");
    }
    else {
        printk("[ERR] %s:%d fw_entry is NULL.\n", __FUNCTION__, __LINE__);
    }
}
#endif


int load_av_from_emmc(char *audio_image, unsigned int audio_start,
        char *video_image_1, unsigned int video_start_1, char *video_image_2, unsigned int video_start_2)
{
    fw_desc_table_v1_t *fw_desc_table_v1;
    fw_desc_entry_v1_t *fw_entry, *fw_entry_v1;
    fw_desc_entry_v1_t *audio_fw_entry = NULL;
    fw_desc_entry_v11_t *fw_entry_v11;
    fw_desc_entry_v21_t *fw_entry_v21;
    part_desc_entry_v1_t *part_entry;
    unsigned int part_count = 0;
    unsigned long fw_desc_table_base = 0;
    unsigned int fw_total_len = 0;
    unsigned int fw_total_paddings = 0;
    unsigned int fw_entry_num = 0;
    //unsigned int eMMC_bootcode_area_size = 0x220000;        // eMMC bootcode area size
    unsigned int eMMC_fw_desc_table_start = 0x620000;
    unsigned char *ptr = NULL;
    int i, ret = 0;
    unsigned int __iomem *afwPtr = ioremap(ACPU_FIREWARE_PHYS, ACPU_FIREWARE_SIZE);

    if (!afwPtr) {
        pr_err("avcpu: ioremap fail for ACPU FW region!\n");
        return -EFAULT;
    }

    fw_desc_table_base = __get_free_page(GFP_KERNEL);
    if (!fw_desc_table_base) {
        pr_info("avcpu: can not allocate enough memory...\n");
	iounmap(afwPtr);
        return -ENOMEM;
    }

    ret = read_emmc_data(HEADER_OFFSET / EMMC_BLOCK_SIZE, PAGE_SIZE, (void *)fw_desc_table_base);
    if (ret < 0)
        goto out;

    fw_desc_table_v1 = (fw_desc_table_v1_t*)fw_desc_table_base;
    ptr = (unsigned char *)fw_desc_table_base;
    printk("%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]); 

    printk("avcpu: fw_desc_table_v1->signature = %s \n", fw_desc_table_v1->signature);
    printk("avcpu: fw_desc_table_v1->version = 0x%x \n", fw_desc_table_v1->version);

    fw_desc_table_v1->checksum = BE32_TO_CPU(fw_desc_table_v1->checksum);
    fw_desc_table_v1->paddings = BE32_TO_CPU(fw_desc_table_v1->paddings);
    fw_desc_table_v1->part_list_len = BE32_TO_CPU(fw_desc_table_v1->part_list_len);
    fw_desc_table_v1->fw_list_len = BE32_TO_CPU(fw_desc_table_v1->fw_list_len);

    printk("avcpu: fw_desc_table_v1->paddings = %d!!\n", fw_desc_table_v1->paddings);
#ifdef DUBUG_FW_DESC_TABLE
    dump_fw_desc_table_v1(fw_desc_table_v1);
#endif

    /* Check partition existence */
    if (fw_desc_table_v1->part_list_len == 0)
    {
        printk("[ERR] %s:No partition found!\n", __FUNCTION__);
        //return RTK_PLAT_ERR_PARSE_FW_DESC;
    }
    else
    {
        part_count = fw_desc_table_v1->part_list_len / sizeof(part_desc_entry_v1_t);
    }

    printk("avcpu: part_count = %d\n", part_count);
    fw_entry_num = fw_desc_table_v1->fw_list_len / sizeof(fw_desc_entry_v1_t);
    printk("[INFO] fw desc table base: 0x%08x, count: %d\n", eMMC_fw_desc_table_start, fw_entry_num);

    part_entry = (part_desc_entry_v1_t *)((unsigned long)fw_desc_table_base + sizeof(fw_desc_table_v1_t));
    fw_entry = (fw_desc_entry_v1_t *)((unsigned long)fw_desc_table_base +
            sizeof(fw_desc_table_v1_t) +
            fw_desc_table_v1->part_list_len);

    for (i = 0; i < part_count; i++)
    {
        part_entry[i].length = BE32_TO_CPU(part_entry[i].length);
        printk("===> i=%d  part_entry[%d].type=%d\n", i, i, part_entry[i].type);
#ifdef DUBUG_FW_DESC_TABLE
        dump_part_desc_entry_v1(&(part_entry[i]));
#endif
    }

    fw_total_len = 0;
    fw_total_paddings = 0;

    /* Parse firmware entries and compute fw_total_len */
    printk("@@@@ fw_desc_table_v1->version = 0x%.8x\n", fw_desc_table_v1->version);
    switch (fw_desc_table_v1->version)
    {
        case FW_DESC_TABLE_V1_T_VERSION_1:
            fw_entry_num = fw_desc_table_v1->fw_list_len / sizeof(fw_desc_entry_v1_t);

            for (i = 0; i < fw_entry_num; i++)
            {
                fw_entry[i].version = BE32_TO_CPU(fw_entry[i].version);
                fw_entry[i].target_addr = BE32_TO_CPU(fw_entry[i].target_addr);
                fw_entry[i].offset = BE32_TO_CPU(fw_entry[i].offset) - eMMC_fw_desc_table_start;    /* offset from fw_desc_table_base */
                fw_entry[i].length = BE32_TO_CPU(fw_entry[i].length);
                fw_entry[i].paddings = BE32_TO_CPU(fw_entry[i].paddings);
                fw_entry[i].checksum = BE32_TO_CPU(fw_entry[i].checksum);

                printk("[OK] fw_entry[%d] type=%d offset = 0x%x length = 0x%x (paddings = 0x%x)\n",
                        i, fw_entry[i].type, fw_entry[i].offset, fw_entry[i].length, fw_entry[i].paddings);

                if (fw_entry[i].type == FW_TYPE_AUDIO)
                {
                    audio_fw_entry = &fw_entry[i];
                    printk("##### got fw_entry[%d].type == FW_TYPE_AUDIO ###\n", i);
                    printk("avcpu: Audio FW found\n");
                }

#ifdef DUBUG_FW_DESC_TABLE
                dump_fw_desc_entry_v1(&(fw_entry[i]));
#endif

                fw_total_len += fw_entry[i].length;
                fw_total_paddings += fw_entry[i].paddings;
            }


            break;

        case FW_DESC_TABLE_V1_T_VERSION_11:
            fw_entry_v11 = (fw_desc_entry_v11_t*)fw_entry;
            fw_entry_num = fw_desc_table_v1->fw_list_len / sizeof(fw_desc_entry_v11_t);
            for(i = 0; i < fw_entry_num; i++) {
                fw_entry_v1 = &fw_entry_v11[i].v1;
                fw_entry_v1->version = BE32_TO_CPU(fw_entry_v1->version);
                fw_entry_v1->target_addr = BE32_TO_CPU(fw_entry_v1->target_addr);
                fw_entry_v1->offset = BE32_TO_CPU(fw_entry_v1->offset) - eMMC_fw_desc_table_start;  /* offset from fw_desc_table_base */
                fw_entry_v1->length = BE32_TO_CPU(fw_entry_v1->length);
                fw_entry_v1->paddings = BE32_TO_CPU(fw_entry_v1->paddings);
                fw_entry_v1->checksum = BE32_TO_CPU(fw_entry_v1->checksum);

                printk("[OK] fw_entry[%d] offset = 0x%x length = 0x%x (paddings = 0x%x) act_size = %d part_num = %d\n",
                        i, fw_entry_v1->offset, fw_entry_v1->length, fw_entry_v1->paddings, fw_entry_v11[i].act_size, fw_entry_v11[i].part_num);

#ifdef DUBUG_FW_DESC_TABLE
                dump_fw_desc_entry_v1(&(fw_entry[i]));
#endif

                fw_total_len += fw_entry_v1->length;
                fw_total_paddings += fw_entry_v1->paddings;
            }


            break;

        case FW_DESC_TABLE_V1_T_VERSION_21:
            fw_entry_v21 = (fw_desc_entry_v21_t*)fw_entry;
            fw_entry_num = fw_desc_table_v1->fw_list_len / sizeof(fw_desc_entry_v21_t);
            for(i = 0; i < fw_entry_num; i++) {
                fw_entry_v1 = &fw_entry_v21[i].v1;
                fw_entry_v1->version = BE32_TO_CPU(fw_entry_v1->version);
                fw_entry_v1->target_addr = BE32_TO_CPU(fw_entry_v1->target_addr);
                fw_entry_v1->offset = BE32_TO_CPU(fw_entry_v1->offset) - eMMC_fw_desc_table_start;  /* offset from fw_desc_table_base */
                fw_entry_v1->length = BE32_TO_CPU(fw_entry_v1->length);
                fw_entry_v1->paddings = BE32_TO_CPU(fw_entry_v1->paddings);
                fw_entry_v1->checksum = BE32_TO_CPU(fw_entry_v1->checksum);

                printk("[OK] fw_entry[%d] offset = 0x%x length = 0x%x (paddings = 0x%x) act_size = %d part_num = %d\n",
                        i, fw_entry_v1->offset, fw_entry_v1->length, fw_entry_v1->paddings, fw_entry_v21[i].act_size, fw_entry_v21[i].part_num);

#ifdef DUBUG_FW_DESC_TABLE
                dump_fw_desc_entry_v1(&(fw_entry[i]));
#endif

                fw_total_len += fw_entry_v1->length;
                fw_total_paddings += fw_entry_v1->paddings;
            }


            break;

        default:
            printk("[ERR] %s:unknown version:%d\n", __FUNCTION__, fw_desc_table_v1->version);
            ret = RTK_PLAT_ERR_PARSE_FW_DESC;
            goto out;
    }

    printk("====> audio_image=0x%.8lx audio_start=0x%.8x\n", (unsigned long)audio_image, audio_start);

    if (audio_image) {
        if ((ret = load_av_from_file(AUDIO_CPU, audio_image, audio_start))) {
            printk("avcpu: load audio image error %d...\n", ret);
            goto out;
        }
    } else {
        if (audio_fw_entry == NULL) {
            printk("avcpu: can not find audio firmware from NAND flash...\n");
            ret = -ENODEV;
            goto out;
        }
        printk("avcpu: audio_fw_entry->offset = 0x%x \n", audio_fw_entry->offset);
        printk("avcpu: audio_fw_entry->length = 0x%x \n", audio_fw_entry->length);
        printk("avcpu: audio_fw_entry->target_addr = 0x%x \n", audio_fw_entry->target_addr);

        // currently we only support align access...
        if (audio_fw_entry->offset & (EMMC_BLOCK_SIZE - 1))
            BUG();

#ifdef CONFIG_RTK_IPCSHM_RESET // perform IPC_SHM reset before setting entry value
       printk("avcpu: reseting IPC share-mem\n");
       if ((ret = avcpu_ipc_shm_reset()) != 0) {
           pr_err("avcpu: error occured during reset IPC_SHM\n");
           goto out;
       }
#endif
        writel(CPU_TO_BE32(audio_fw_entry->target_addr | MIPS_KSEG0BASE), rpc_base + 0xe0);
        printk("audio entry value: %08x \n", *((unsigned int *)(rpc_base + 0xe0)));

        audio_fw_entry->target_addr = audio_fw_entry->target_addr;

        ret = read_emmc_data((audio_fw_entry->offset+eMMC_fw_desc_table_start) / EMMC_BLOCK_SIZE,
                audio_fw_entry->length, (void *)(afwPtr));

        if (ret < 0)
            goto out;
        printk("ret: %lx length: %x \n", ret * EMMC_BLOCK_SIZE, audio_fw_entry->length);

        //dmac_flush_range((void *)afwPtr, audio_fw_entry->length);
        //outer_flush_range(afwPtr, audio_fw_entry->length);
        // flush the data cache...
        //#ifdef CONFIG_MIPS
        //      data_cache_flush(audio_fw_entry->target_addr, audio_fw_entry->length);
        //#endif
    }

    //  dmac_flush_range((void *)PAGE_OFFSET, (void *)(PAGE_OFFSET + PAGE_SIZE));
    //      outer_flush_range(0, 0 + PAGE_SIZE);

out:
    free_page(fw_desc_table_base);
    iounmap(afwPtr);

    return ret;
}

int load_av_image(char *audio_image, unsigned int audio_start,
        char *video_image_1, unsigned int video_start_1, char *video_image_2, unsigned int video_start_2)
{
    unsigned int flash_type;
    int ret;

    flash_type = get_device_type();
    printk("avcpu: flash_type: %d \n", flash_type);

    // load firmware image
    if ((audio_image != 0) && (video_image_1 != 0) && (video_image_2 != 0)) {
        if ((ret = load_av_from_file(AUDIO_CPU, audio_image, audio_start)))
            printk("avcpu: load audio image error %d...\n", ret);
    } else if (flash_type == DEVICE_EMMC) {
        load_av_from_emmc(audio_image, audio_start, video_image_1, video_start_1, video_image_2, video_start_2);
    } else {
        printk("avcpu: unsupported operation...\n");
        BUG();
    }

    return 0;
}

static ssize_t store_reset_cmd(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    unsigned long val;
    char *endp;

    if(count == 0)
        return -EINVAL;

    val = simple_strtoul(buf, &endp, 16);
    if(endp == buf)
        return -EINVAL;

    if(val != 0x20141209)
        return -EINVAL;

    avpriv.status = AVSTAT_RESUME;
    pr_info("\033[33m%s status:%d\033[m\n", __func__, avpriv.status);
    wake_up_interruptible(&avpriv.status_wq);

    return count;
}

static DEVICE_ATTR(reset, S_IWUSR, NULL, store_reset_cmd);

static int avcpu_init(void)
{
    int result;
    struct device *avdev = NULL;
    dev_t dev = MKDEV(AVCPU_MAJOR, 0);

    printk("avcpu: audio & video cpu driver for Realtek Media Processors\n");

    if (!(crt_base = ioremap(CRT_BASE, PAGE_SIZE))) {
        pr_err("avcpu: ioremap CRT fail!\n");
        return -1;
    }

    if (!(rpc_base = ioremap(RPC_COMM_PHYS, PAGE_SIZE))) {
        pr_err("avcpu: ioremap CRT fail!\n");
        return -1;
    }

    result = register_chrdev_region(dev, 1, "avcpu");
    if (result < 0) {
        printk("avcpu: can not get chrdev region...\n");
        return result;
    }

    avcpu_class = class_create(THIS_MODULE, "avcpu");
    if (IS_ERR(avcpu_class)) {
        printk("avcpu: can not create class...\n");
        result = PTR_ERR(avcpu_class);
        goto fail_class_create;
    }

    avcpu_class->devnode = avcpu_devnode;

    avcpu_platform_devs = platform_device_register_simple("avcpu", -1, NULL, 0);
    if (platform_driver_register(&avcpu_device_driver) != 0) {
        printk("avcpu: can not register platform driver...\n");
        result = -EINVAL;
        goto fail_platform_driver_register;
    }

    cdev_init(&avcpu_cdev, &avcpu_fops);
    avcpu_cdev.owner = THIS_MODULE;
    avcpu_cdev.ops = &avcpu_fops;
    result = cdev_add(&avcpu_cdev, dev, 1);
    if (result < 0) {
        printk("avcpu: can not add character device...\n");
        goto fail_cdev_init;
    }
    avdev = device_create(avcpu_class, NULL, dev, NULL, "avcpu");
    device_create_file(avdev, &dev_attr_reset);

    memset(&avpriv, 0, sizeof(avpriv));
    init_waitqueue_head(&avpriv.status_wq);

    return 0;

fail_cdev_init:
    platform_driver_unregister(&avcpu_device_driver);
fail_platform_driver_register:
    platform_device_unregister(avcpu_platform_devs);
    avcpu_platform_devs = NULL;
    class_destroy(avcpu_class);
fail_class_create:
    avcpu_class = NULL;
    unregister_chrdev_region(dev, 1);
    return result;
}

static void avcpu_exit(void)
{
    dev_t dev = MKDEV(AVCPU_MAJOR, 0);

    //  if ((avcpu_platform_devs == NULL) || (class_destroy == NULL))
    if (avcpu_platform_devs == NULL)    //TODO : fix compile warming by victor 20140211
        BUG();

    if (crt_base)
        iounmap(crt_base);

    if (rpc_base)
        iounmap(rpc_base);

    device_destroy(avcpu_class, dev);
    cdev_del(&avcpu_cdev);

    platform_driver_unregister(&avcpu_device_driver);
    platform_device_unregister(avcpu_platform_devs);
    avcpu_platform_devs = NULL;

    class_destroy(avcpu_class);
    avcpu_class = NULL;

    unregister_chrdev_region(dev, 1);
}

late_initcall(avcpu_init);
module_exit(avcpu_exit);
