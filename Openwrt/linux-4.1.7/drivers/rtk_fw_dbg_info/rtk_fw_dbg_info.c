

#include <linux/init.h>                   
#include <linux/module.h>                                      
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/debugfs.h>

#include <linux/platform_device.h>	
#include <linux/of.h>
#include <linux/of_device.h>

#include "rtk_fw_dbg_info_rpc.h"

ssize_t show_rtk_fw_opened_plane(struct device *cd, struct device_attribute *attr, char *buf);
ssize_t show_drop_repeat_info(struct device *cd, struct device_attribute *attr, char *buf);
ssize_t show_tv_system_info(struct device *cd, struct device_attribute *attr, char *buf);
ssize_t show_low_delay_setting_info(struct device *cd, struct device_attribute *attr, char *buf);
ssize_t show_video_info(struct device *cd, struct device_attribute *attr, char *buf);
ssize_t show_hdr_status(struct device *cd, struct device_attribute *attr, char *buf);


static DEVICE_ATTR(rtk_fw_opened_plane, S_IRUGO, show_rtk_fw_opened_plane, NULL);
static DEVICE_ATTR(drop_repeat_info, S_IRUGO, show_drop_repeat_info, NULL);
static DEVICE_ATTR(rtk_fw_tv_system_info, S_IRUGO, show_tv_system_info, NULL);
static DEVICE_ATTR(rtk_fw_low_delay_setting_info, S_IRUGO, show_low_delay_setting_info, NULL);
static DEVICE_ATTR(rtk_fw_video_info, S_IRUGO, show_video_info, NULL);
static DEVICE_ATTR(rtk_fw_hdr_status, S_IRUGO, show_hdr_status, NULL);

static const struct of_device_id rtk_fw_dbg_info_dt_ids[] = {
     { .compatible = "Realtek,rtk_fw_dbg_info", },
     {},
};
MODULE_DEVICE_TABLE(of, rtk_hdmitx_dt_ids);

typedef struct{  
    struct miscdevice  miscdev;

    struct device dev;   
}rtk_fw_dbg_info_device_t;


static rtk_fw_dbg_info_device_t rtk_fw_dbg_info_dev; 


static ssize_t drv_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
	FW_INFO_PRINT("[rtk_fw_dbg_info]: rtk_fw_dbg_info read\n");
	return count;
}
static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	FW_INFO_PRINT("[rtk_fw_dbg_info]:rtk_fw_dbg_info write\n");
	return count;
}
static int drv_open(struct inode *inode, struct file *filp)
{
	if (nonseekable_open(inode, filp))
		return -ENODEV;
	
	filp->private_data = &rtk_fw_dbg_info_dev;
	return 0;
}


static int drv_release(struct inode *inode, struct file *filp)
{
	FW_INFO_PRINT("rtk_fw_dbg_info:rtk_fw_dbg_info close\n");
	return 0;
}





int rtk_fw_dbg_info_ioctl(struct inode *inode, unsigned int cmd, unsigned long arg)
{
	
	
    switch (cmd) {
	   default:  
       
            FW_INFO_PRINT("rtk_fw_dbg_info:CMD not defined!!\n");
           
    }

	return 0;
}



ssize_t show_rtk_fw_opened_plane(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");

    int query_what = ENUM_QUERY_VO_PLANE;
	RPC_FW_QUERY_RETURN query_ret;	

    ret_count += sprintf(buf+ret_count, "Query: ENUM_QUERY_VO_PLANE------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "return: %s------\n",query_ret.query_return);
	return ret_count;
}



ssize_t show_drop_repeat_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");
    int query_what = ENUM_QUERY_DROP_REPEAT;
	RPC_FW_QUERY_RETURN query_ret;	
    ret_count += sprintf(buf+ret_count, "----- Query: ENUM_QUERY_DROP_REPEAT------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "%s\n",query_ret.query_return);
	return ret_count;
}

ssize_t show_low_delay_setting_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");
    int query_what = ENUM_QUERY_LOW_DELAY;
	RPC_FW_QUERY_RETURN query_ret;	
    ret_count += sprintf(buf+ret_count, "----- Query: ENUM_QUERY_LOW_DELAY------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "%s\n",query_ret.query_return);
	return ret_count;
}

ssize_t show_video_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");
    int query_what = ENUM_QUERY_VIDEO_INFO;
	RPC_FW_QUERY_RETURN query_ret;	
    ret_count += sprintf(buf+ret_count, "----- Query: ENUM_QUERY_VIDEO_INFO------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "%s\n",query_ret.query_return);
	return ret_count;
}

ssize_t show_hdr_status(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");
    int query_what = ENUM_QUERY_HDR_STATUS;
	RPC_FW_QUERY_RETURN query_ret;	
    ret_count += sprintf(buf+ret_count, "----- Query: ENUM_QUERY_HDR_STATUS------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "%s\n",query_ret.query_return);
	return ret_count;
}

ssize_t show_tv_system_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	ret_count = sprintf(buf, "------ 5280 FW Info -----\n");
    int query_what = ENUM_QUERY_VO_TV_SYSTEM_CONFIG;
	RPC_FW_QUERY_RETURN query_ret;	
    ret_count += sprintf(buf+ret_count, "----- Query: ENUM_QUERY_VO_TV_SYSTEM_CONFIG------\n");
	
	FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(&query_what, &query_ret);

    ret_count += sprintf(buf+ret_count, "%s\n",query_ret.query_return);
	return ret_count;
}



struct file_operations drv_fops={
		
	read:drv_read,
	write:drv_write,
	open:drv_open,
	release:drv_release,	
    unlocked_ioctl:rtk_fw_dbg_info_ioctl,
	
};

struct class *rtk_fw_dbg_info_class;
#define RTK_FW_DBG_INFO_DEV_NAME "rtk_fw_dbg_info"

int register_rtk_fw_dbg_info_miscdev(rtk_fw_dbg_info_device_t* device)
{
    struct miscdevice *miscdev = &device->miscdev;
           
	miscdev->minor = MISC_DYNAMIC_MINOR;
    miscdev->name = RTK_FW_DBG_INFO_DEV_NAME;
    miscdev->mode = 0666;
    miscdev->fops = &drv_fops;                  
    
    return misc_register(miscdev);
}

             
static int rtk_fw_dbg_info_probe(struct platform_device *pdev)
{   
   int ret; 
                   
   register_rtk_fw_dbg_info_miscdev(&rtk_fw_dbg_info_dev);


   FW_INFO_PRINT("[rtk_fw_dbg_info]: create device file!\n");

   //init_rtk_fw_dbg_info_debugfs();
   device_create_file(&pdev->dev, &dev_attr_rtk_fw_opened_plane);
   device_create_file(&pdev->dev, &dev_attr_drop_repeat_info);
   device_create_file(&pdev->dev, &dev_attr_rtk_fw_tv_system_info);
   device_create_file(&pdev->dev, &dev_attr_rtk_fw_low_delay_setting_info);
   device_create_file(&pdev->dev, &dev_attr_rtk_fw_video_info);
   device_create_file(&pdev->dev, &dev_attr_rtk_fw_hdr_status);
   
   FW_INFO_PRINT("[rtk_fw_dbg_info]:register_ion_client..\n");
   rtk_fw_dbg_info_register_ion_client("rtk_fw_dbg_info_driver");  




	
   FW_INFO_PRINT("[rtk_fw_dbg_info]:rtk_fw_dbg_info driver installed.\n"); 

	return 0;                                
}                                         


static struct platform_driver rtk_fw_dbg_info_driver = {
	.probe = rtk_fw_dbg_info_probe,
	.driver = {
		.name = "rtk_fw_dbg_info",
		.owner = THIS_MODULE, 
		.of_match_table = of_match_ptr(rtk_fw_dbg_info_dt_ids),
	}, 
};

static int __init rtk_fw_dbg_info_init(void)
{									
    if (platform_driver_register(&rtk_fw_dbg_info_driver)) {
        FW_INFO_ERROR_PRINT("Could not add character driver");
        goto err_register;
    }
						
	return 0;	
		
err_register:
        platform_driver_unregister(&rtk_fw_dbg_info_driver);

    return -EFAULT;
}




										  
static void rtk_fw_dbg_info_exit(void)              
{                                         
	FW_INFO_ERROR_PRINT( "[rtk_fw_dbg_info]:rtk_fw_dbg_info exit."); 
    device_unregister(rtk_fw_dbg_info_class);
	rtk_fw_dbg_info_deregister_ion_client("hdmitx_driver");
}                                         





                
module_init(rtk_fw_dbg_info_init);
module_exit(rtk_fw_dbg_info_exit);                  






										  
MODULE_LICENSE("Dual BSD/GPL");                     
MODULE_AUTHOR("RTK");           
MODULE_DESCRIPTION("This is rtk_fw_dbg_info module.");






