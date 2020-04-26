#ifndef __OSAL_OSAL_H__
#define __OSAL_OSAL_H__

/* OSAL - Include ALL OS-Abstraction-Layer Definitions */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
//#include <errno-base.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef ANDROID
#define system(abc) mysystem(abc)

#ifdef __cplusplus
extern "C" {
#endif
int mysystem(const char *command);
#ifdef __cplusplus
}
#endif
#endif // not define ANDROID

/* constant definitions */
#define TIME_INFINITY (-1)

/* basic data types */
#include "OSAL/Types.h"

/* thread and synchronization functions */
#include "OSAL/Threads.h"

/* platform library */
#include "OSAL/PLI.h"

#ifdef __cplusplus
/* smart classes for better automation */
#include "OSAL/SmartClass.h"
#endif

/* variable and function to handle user input */

extern unsigned long g_userInputTotalCount;    // the count of total input keys user has pressed
extern unsigned long g_userInputHandledCount;  // the count of total input keys that have been handled by processKey

extern short g_userInputKeyType;	//the type of input key
extern int64_t g_userInputTime; // the time when the user has pressed the input key (in 90k clock)
// isKeyPressed
// function return 0 means no pending user input. Otherwise, return the number pending for process.
long osal_isKeyPressed();
//function returns type of the key pending for handle
short osal_getKeyType();



/* general functions */

void*   osal_OpenLibrary(char* path, int bLazy);                /* load DLL/SO library */
HRESULT osal_CloseLibrary(void* lib_handle);                    /* unload DLL/SO library */
void*   osal_GetFuncAddress(void* lib_handle, char* func_name); /* get function address from DLL/SO library */




#endif /*__OSAL_OSAL_H__*/
