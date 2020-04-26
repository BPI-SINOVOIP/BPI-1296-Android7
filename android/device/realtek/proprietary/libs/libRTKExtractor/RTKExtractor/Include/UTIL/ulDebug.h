#ifndef __UTIL_DEBUG_H__
#define __UTIL_DEBUG_H__

#include <stdio.h>
#include <assert.h>


/*************** BEGIN - stuff relocated from system.h ****************/
/* @FIXME: arrange these utility functions in better form ??? */

/* Macro definition */
#ifdef RDDEBUG
#    define DASSERT(expr)                                       \
         if (!(expr)){                                          \
             printf("[DASSERT] %s:%d\n", __FILE__, __LINE__);   \
             assert(0);                                         \
         }
#else
#    define DASSERT(expr)               while (0) {}
#endif

#ifdef RDDEBUG
#    define DTRACE(format, args)                                            \
         printf("[DTRACE] %s:%d:\t" format, __FILE__, __LINE__, args);
#else
#    define DTRACE(expr)                while (0) {}
#endif

/* expr is true: no error, not to return */
/* expr is false: error, return the return value */
#define ERROR_DETECT(expr, ret)         {                       \
    if (!(expr)){                                               \
        DASSERT(0);                                             \
        printf("ERROR_DETECT in %s:%d\n", __FILE__, __LINE__);  \
        return (ret);                                           \
    }                                                           \
}

#define CHECKPOINTER(p, ret)            ERROR_DETECT(((p)!= NULL), ret)

/*************** END - stuff relocated from system.h ****************/


/* Debug Utility */
/* Using DP to print out information, warning or error */
/* Errors always print out
   Warning print out when warning is enabled and warning level smaller then global setting
   Infomation always print out if info is enabled
Example:
	DP(ERROR_LEVEL, stdout, "%s my messages %d", "ERROR", i); always send to stdout
	DP(INFO_LEVEL, stdout, "informations %d", i); send to stdout if global debug flag setting
	enable the DEBUG_INFO_ENABLE
	DP(WARNIONG_LEVEL(3), stdout, "%s my warnings %d", "WARNING", i); will send to stdout
	if global warning level is bigger then 3.

	User set the global debug flag by setDebugFlag function
Example:
    setDebugFlag(WARNING_LEVEL(2) | DEBUG_INFO_ENABLE);  means DP will take effect if DP
	use the type of INFO_LEVEL, or WARNIONG_LEVEL(n) where n <=2
	You can disable all the warning and information by setDebugFlag(0). if DP
	use the type of ERROR_LEVEL, the error message always send out, and cannot be disabled.

*  To enable module name print out in front of messages. Write "char *_getName()" function which
   return the module name in your class. Global functions will always has the name of "Global"

*  Module name will not print out in Windows platform, only in Linux you can see the Module name
*  All DP will disappear from the compiled result if _DEBUG is not defined.

*/

/* Debug print format */
/*
	31 30 29 28 | 27 ~ 16  |  15 ~ 8    |  7 ~ 0
	Type        | Module   |  Reserved  | Level   
*/

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_WARNING_ENABLE   0x1  << 31
#define DEBUG_ERROR_ENABLE     0x1  << 30
#define DEBUG_INFO_ENABLE      0x1  << 29
#define DEBUG_MODULE_MASK    0x3F << 16
#define DEBUG_LEVEL_MASK     0xFF
 
#define DEBUG_LEVEL(n)       (n & DEBUG_LEVEL_MASK)
#define DEBUG_ERROR(n)       (n & DEBUG_ERROR_ENABLE)
#define DEBUG_INFO(n)        (n & DEBUG_INFO_ENABLE)
#define DEBUG_WARNING(n)	 (n & DEBUG_WARNING_ENABLE)
#define DEBUG_MODULE(n)   	 (n & DEBUG_MODULE_MASK)


#define ERROR_LEVEL		 (DEBUG_ERROR_ENABLE)
#define WARNING_LEVEL(n)	 (DEBUG_WARNING_ENABLE | ( n & DEBUG_LEVEL_MASK) )
#define INFO_LEVEL   	 (DEBUG_INFO_ENABLE)

#define DF


char * _getName();
int    _compareFlag(unsigned long flag);

#ifndef WIN32
 #ifdef _DEBUG

  #define DP(flag, device, fmt, args...)     \
		  {if (_compareFlag(flag) == 1){          \
               fprintf(device,"%s:", _getName());  \
	           fprintf(device, fmt, args);         \
		  } \
		  }
 #else
  #define DP(flag, device, fmt, args...)     
 #endif
#else
  #include <stdarg.h>
  void DP(unsigned long flag, FILE *device, char *fmt, ...);
#endif

void setDebugFlag(unsigned long flag);

#ifdef __cplusplus
}
#endif

#endif /*__UTIL_DEBUG_H__*/
