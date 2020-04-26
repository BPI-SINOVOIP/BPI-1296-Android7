//Copyright (C) 2007-2013 Realtek Semiconductor Corporation.
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include "cec.h"

#ifdef CONFIG_CEC_CHARDEV
#include "cec_dev.h"
#endif  

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>

int cec_drv_suspend(struct device * dev, pm_message_t state);
int cec_drv_resume(struct device * dev);

int cec_device_en[2];
int *cec_device_pthis[2];
#if 0
#include <linux/kobject.h>
#include "../../base/base.h"
extern struct device_node * test_cec0_register_dev(cec_device* device, cec_driver* driver);
#endif
/*------------------------------------------------------------------
 * Func : cec_bus_match
 *
 * Desc : cec dev exit function
 *
 * Parm : dev    : device
 *        driver : driver
 *         
 * Retn : 1 : device & driver matched, 0 : device & driver not matched
 *------------------------------------------------------------------*/
int cec_bus_match(
    struct device*          dev, 
    struct device_driver*   drv
    )
{      
    int len = strlen(dev_name(dev));  
    // cec bus have no idea to match device & driver, return 1 to pass all   
    printk(KERN_ERR "[cec_bus_match name = %s,len=%d,drv_name=%s]\n",dev_name(dev),strlen(dev_name(dev)),drv->name);  
    //return 1;
    
    if (0 == strncmp(dev_name(dev), drv->name, 3));//len))
        return 1;
    return 0;
}

// cyhuang (2011/11/24) +++	        
#ifdef CONFIG_PM
/*------------------------------------------------------------------
 * Func : cec_bus_pm_suspend_noirq
 *
 * Desc : suspend cec dev
 *
 * Parm : dev    : device to be suspend 
 *         
 * Retn : 0
 *------------------------------------------------------------------*/
#if 1
int cec_check_device(struct device * dev)
{
    int device_en = 0;
    char device0[] = "cec0";
    char device1[] = "cec1";
    //cec_device* p_dev = to_cec_device(dev);    
    printk("[cec_check_device %s]\n",dev_name(dev));
    //if(p_dev->device_en)
    if (0 == strncmp(dev_name(dev), device0, strlen(dev_name(dev))))
        device_en = cec_device_en[0];//cec_tx_device_check();
    else if(0 == strncmp(dev_name(dev), device1, strlen(dev_name(dev))))
        device_en = cec_device_en[1];//cec_rx_device_check();
    return     device_en;
}

static int cec_bus_pm_suspend_noirq(struct device * dev)
{   
    int device_en = cec_check_device(dev);
    printk("cec_bus_pm_suspend_noirq\n");
    if(device_en)
    { 
        cec_device* p_dev = to_cec_device(dev);
        cec_driver* p_drv = to_cec_driver(dev->driver);    
        return (p_drv->suspend) ? p_drv->suspend(p_dev) : 0;
    }
    else
    {
        printk("suspend_noirq cec_dev '%s' not implement yet !!\n", dev_name(dev));
        return 0;
    }
}

/*------------------------------------------------------------------
 * Func : cec_bus_pm_resume_noirq
 *
 * Desc : resume cec bus
 *
 * Parm : dev    : device to be resumed 
 *         
 * Retn : 0
 *------------------------------------------------------------------*/
static int cec_bus_pm_resume_noirq(struct device * dev)
{
    int device_en = cec_check_device(dev);
    printk("cec_bus_pm_resume_noirq\n");
    if(device_en)
    {
        cec_device* p_dev = to_cec_device(dev);
        cec_driver* p_drv = to_cec_driver(dev->driver);    
        return (p_drv->resume) ? p_drv->resume(p_dev) : 0;
    }
    else
    {
        printk("resume_noirq cec_dev '%s' not implement yet !!\n", dev_name(dev));
        return 0;
    }
}
#else
#define cec_bus_pm_suspend_noirq	NULL
#define cec_bus_pm_resume_noirq	NULL
#endif

static const struct dev_pm_ops cec_bus_pm_ops = {
 //   .suspend_noirq    = cec_bus_pm_suspend_noirq,
 //   .resume_noirq     = cec_bus_pm_resume_noirq,
    .suspend = cec_drv_suspend,
    .resume = cec_drv_resume,
#ifdef CONFIG_HIBERNATION
    .freeze_noirq     = cec_bus_pm_suspend_noirq,
    .thaw_noirq       = cec_bus_pm_resume_noirq,
    .poweroff_noirq   = cec_bus_pm_suspend_noirq,
    .restore_noirq    = cec_bus_pm_resume_noirq,
#endif
};

#endif // CONFIG_PM
// cyhuang (2011/11/24) +++	  

struct bus_type cec_bus_type = {
    .name    = "cec",
    .match   = cec_bus_match,      
// cyhuang (2011/11/24) +++	        
#ifdef CONFIG_PM
    .pm      = &cec_bus_pm_ops,
#endif
// cyhuang (2011/11/24) +++	      
};



/*------------------------------------------------------------------
 * Func : register_cec_device
 *
 * Desc : register cec device
 *
 * Parm : device    : cec device to be registered
 *         
 * Retn : N/A
 *------------------------------------------------------------------*/
void cec_device_release(struct device* dev)
{    
    cec_device* p_dev = to_cec_device(dev);
    
    printk("cec dev %s released\n", p_dev->name);    
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                            cec_device                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

static int cec_device_count = 0;

/*------------------------------------------------------------------
 * Func : register_cec_device
 *
 * Desc : register cec device
 *
 * Parm : device    : cec device to be registered
 *         
 * Retn : 0
 *------------------------------------------------------------------*/
#if 0
static int cec_of_dev_node_match(struct device *dev, void *data)
{
        return dev->of_node == data;
}

static inline int cec_driver_match_device(struct device_driver *drv,
				      struct device *dev)
{
	return drv->bus->match ? drv->bus->match(dev, drv) : 1;
}

int register_cec_device(cec_device* device, cec_driver* driver)
{
    struct device_node *node;
    struct device *dev;
    int err = -ENODEV;
    char name[] = "cec0";
    
    printk("[register_cec_device start]\n");
    node = test_cec0_register_dev(device, driver);
//    dev = bus_find_device(&cec_bus_type, NULL, node,
//					 cec_of_dev_node_match);
    if (!node)
    {
        printk("[no cectx_node]\n");
        return;
    }  
    dev = bus_find_device_by_name(&cec_bus_type, NULL, name);					 
    printk("register cec device '%s' (%p) ,(%p)\n", device->name, dev, driver);					 
    if (dev && dev->driver == NULL && cec_driver_match_device(driver, dev)) {
		if (dev->parent)	/* Needed for USB */
			device_lock(dev->parent);
		device_lock(dev);
		err = driver_probe_device(driver, dev);
		device_unlock(dev);
		if (dev->parent)
			device_unlock(dev->parent);

		if (err > 0) {
			/* success */
			err = 1;//count;
		} else if (err == 0) {
			/* driver didn't accept device */
			err = -ENODEV;
		}
	}
	put_device(dev);
	//bus_put(&cec_bus_type);
	//if (bus)
		kset_put(&(cec_bus_type.p->subsys));
    printk("[register_cec_device end]\n");
}
#else
int register_cec_device(cec_device* device)
{
    struct device* dev = &device->dev;
    
    printk(KERN_ERR "register cec device '%s' (%p) to cec%d\n", device->name, dev, cec_device_count);
            
//    dev_set_name(dev, "cec%d",cec_device_count++);
	dev_set_name(dev, device->name);
    
    dev->bus = &cec_bus_type;
    
    dev->release  = cec_device_release;  
    
    return device_register(dev);
}
#endif



/*------------------------------------------------------------------
 * Func : unregister_cec_device
 *
 * Desc : unregister cec device
 *
 * Parm : device : cec device to be unregistered
 *         
 * Retn : N/A
 *------------------------------------------------------------------*/
void unregister_cec_device(cec_device* device)
{    
    device_unregister(&device->dev);
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                            cec_driver                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------
 * Func : cec_drv_probe
 *
 * Desc : probe cec device
 *
 * Parm : dev : cec device to be probed
 *         
 * Retn : 0 : if supportted, others : failed
 *------------------------------------------------------------------*/
int cec_drv_probe(struct device * dev)
{
    cec_device* p_dev = to_cec_device(dev);    
    cec_driver* p_drv = to_cec_driver(dev->driver);    
    printk(KERN_ERR "probe : cec_dev '%s' (%p), cec_drv '%s' (%p)\n", p_dev->name, dev,p_drv->name, dev->driver);    
    
    if (!p_drv->probe)
        return -ENODEV;
        
    if (p_drv->probe(p_dev)==0)        
    {
#ifdef CONFIG_CEC_CHARDEV
        create_cec_dev_node(p_dev);
#endif        
        return 0;    
    }

    return -ENODEV;
}



/*------------------------------------------------------------------
 * Func : cec_drv_remove
 *
 * Desc : this function was used to inform that a cec device has been 
 *        removed
 *
 * Parm : dev : cec device to be removed
 *         
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
int cec_drv_remove(struct device * dev)
{
    cec_device* p_dev = to_cec_device(dev);    
    cec_driver* p_drv = to_cec_driver(dev->driver);
    
    printk("remove cec_dev '%s'\n", p_dev->name);
    
    if (p_drv->remove) 
        p_drv->remove(p_dev);
        
#ifdef CONFIG_CEC_CHARDEV
    create_cec_dev_node(p_dev);
#endif  
        
    return 0;
}



/*------------------------------------------------------------------
 * Func : cec_drv_shutdown
 *
 * Desc : this function was used to short down a cec device 
 *
 * Parm : dev : cec device to be shut down
 *         
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
void cec_drv_shutdown(struct device * dev)
{
    cec_device* p_dev = to_cec_device(dev);    
    cec_driver* p_drv = to_cec_driver(dev->driver);    
    
    printk("shotdown cec_dev '%s'\n", p_dev->name);            
    
    if (p_drv->enable)
        p_drv->enable(p_dev, 0);    
}



/*------------------------------------------------------------------
 * Func : cec_drv_suspend
 *
 * Desc : this function was used to suspend a cec device 
 *
 * Parm : dev : cec device to be suspend
 *         
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
int cec_drv_suspend(struct device * dev, pm_message_t state)
{
    int device_en = cec_check_device(dev);
    
    if(device_en)
    {
        cec_device* p_dev = to_cec_device(dev); 
        cec_driver* p_drv = to_cec_driver(dev->driver);    
        
        printk("[cec_drv_suspend name = %s,len=%d,drv_name=%s]\n",dev_name(dev),strlen(dev_name(dev)),p_drv->name);  
        return (p_drv->suspend) ? p_drv->suspend(p_dev) : 0;
    }
    else
    {
        printk("suspend cec_dev '%s' not implement yet !!\n", dev_name(dev));
        return 0;
    }
}


/*------------------------------------------------------------------
 * Func : cec_drv_resume
 *
 * Desc : this function was used to resume a cec device 
 *
 * Parm : dev : cec device to be suspend
 *         
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
int cec_drv_resume(struct device * dev)
{
    int device_en = cec_check_device(dev);
    
    if(device_en)
    {
        cec_device* p_dev = to_cec_device(dev);    
        cec_driver* p_drv = to_cec_driver(dev->driver);    
    
        printk("resume cec_dev '%s'\n", p_dev->name);
        
        return (p_drv->resume) ? p_drv->resume(p_dev) : 0;    
    }
    else
    {
        printk("sresume cec_dev '%s' not implement yet !!\n", dev_name(dev));
        return 0;
    }
}


/*------------------------------------------------------------------
 * Func : register_cec_driver
 *
 * Desc : register cec device driver
 *
 * Parm : driver    : cec device driver to be registered
 *         
 * Retn : 0
 *------------------------------------------------------------------*/
int register_cec_driver(cec_driver* driver)
{
    struct device_driver* drv = &driver->drv;
    
    drv->name     = driver->name;
    drv->bus      = &cec_bus_type;
    drv->probe    = cec_drv_probe;
    drv->remove   = cec_drv_remove;
    drv->shutdown = cec_drv_shutdown;
    drv->suspend  = cec_drv_suspend;
    drv->resume   = cec_drv_resume;

    printk(KERN_ERR "register cec driver '%s' (%p)\n", drv->name, drv);
        
    return driver_register(drv);
}



/*------------------------------------------------------------------
 * Func : unregister_cec_driver
 *
 * Desc : unregister cec driver
 *
 * Parm : driver : cec driver to be unregistered
 *         
 * Retn : N/A
 *------------------------------------------------------------------*/
void unregister_cec_driver(cec_driver* driver)
{   
    struct device_driver* drv = &driver->drv;    
    printk("unregister cec driver '%s' (%p)\n", drv->name, &driver->drv); 
    driver_unregister(&driver->drv);
}



/*------------------------------------------------------------------
 * Func : cec_dev_module_init
 *
 * Desc : cec dev init function
 *
 * Parm : N/A
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
static int __init cec_core_init(void)
{                 
    printk(KERN_ERR "%s, register cec_bus %p\n",__FUNCTION__, &cec_bus_type);
	    
    return bus_register(&cec_bus_type);     // register cec bus type	        
}



/*------------------------------------------------------------------
 * Func : cec_dev_module_exit
 *
 * Desc : cec dev module exit function
 *
 * Parm : N/A
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
static void __exit cec_core_exit(void)
{       
    bus_unregister(&cec_bus_type);        // unregister cec bus
}



module_init(cec_core_init);
module_exit(cec_core_exit);

EXPORT_SYMBOL(register_cec_device);
EXPORT_SYMBOL(unregister_cec_device);
EXPORT_SYMBOL(register_cec_driver);
EXPORT_SYMBOL(unregister_cec_driver);

