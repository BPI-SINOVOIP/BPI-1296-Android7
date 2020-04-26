#ifndef _RTD_TYPES_H_
#define _RTD_TYPES_H_

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#endif

#define UINT32	unsigned int
#define UINT16	unsigned short
#define UINT8	unsigned char
#define INT32	int
#define INT16	short
#define INT8	char

#ifndef WIN32
typedef enum _BOOL {
	FALSE = 0,
	TRUE = 1
} BOOL;
#endif

#endif
