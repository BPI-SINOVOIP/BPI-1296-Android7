#ifndef __SCD_DEBUG_H__
#define __SCD_DEBUG_H__


//-- scd debug messages
//#define CONFIG_SMARTCARD_DBG
//#define CONFIG_SCD_INT_DBG
//#define CONFIG_SCD_TX_DBG
//#define CONFIG_SCD_RX_DBG

#ifdef  CONFIG_SMARTCARD_DBG
#define SC_TRACE                   printk(KERN_WARNING "[SCD] TRACE, " fmt, ## args)  
#else
#define SC_TRACE(args...)    
#endif


#ifdef  CONFIG_SCD_INT_DBG
#define SC_INT_DBG(fmt, args...)        printk(KERN_WARNING "[SCD] INT, " fmt, ## args)  
#else
#define SC_INT_DBG(args...)    
#endif


#define SC_INFO(fmt, args...)           printk(KERN_WARNING "[SCD] Info, " fmt, ## args)  
#define SC_WARNING(fmt, args...)        printk(KERN_EMERG "[SCD] Warning, " fmt, ## args)  


#endif  //__SCD_DEBUG_H__



