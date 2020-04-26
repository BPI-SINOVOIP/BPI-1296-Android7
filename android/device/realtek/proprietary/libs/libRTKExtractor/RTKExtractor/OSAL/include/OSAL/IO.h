#ifndef __OSAL_IO_H__
#define __OSAL_IO_H__

int osal_Unlink(const char *pathname);
int osal_ExclusiveCreateFile(const char *pathname);

#ifdef WIN32
	#define osal_snprintf _snprintf
	#include <time.h>
	#include <sys/types.h> 
	#include <sys/stat.h>
typedef struct _stat _STAT_;

struct tm *localtime_r(const time_t *timep, struct tm *result);
int stat(const char *path, _STAT_ *buf);

#else
	#define osal_snprintf snprintf
#endif

#endif
