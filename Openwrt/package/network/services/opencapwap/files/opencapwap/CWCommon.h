/*******************************************************************************************
 * Copyright (c) 2006-9 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *           Antonio Davoli (antonio.davoli@gmail.com)                                     *
 *******************************************************************************************/

 
#ifndef __CAPWAP_CWCommon_HEADER__
#define __CAPWAP_CWCommon_HEADER__


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#ifdef MACOSX
	#include <netinet/if_ether.h>
#else
	#include <linux/if_ether.h>
#endif
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include "wireless_copy.h"

#define AC_MAX_STA_NUM						50
#define AC_MAX_WTP_NUM						10
#define STA_MAX_11K_NUM						AC_MAX_WTP_NUM
#define AC_MAX_NOTIFY_NUM				    20

#ifdef RTK_CAPWAP
#if defined(CONFIG_RTL_92D_SUPPORT)||defined(CONFIG_RTL_DUAL_PCIESLOT_BIWLAN)
#define RTK_RADIOS		2
#else
#define RTK_RADIOS		1
#endif
#define RTK_VAPS 		4
#else
#define RTK_RADIOS 	2
#define RTK_VAPS 	4
#endif

#ifdef RTK_SMART_ROAMING
#define ADD_STA_ELEM_LEN					12		/* mac+rssi+rate+link */
#define UPD_STA_ELEM_LEN					7		/* mac+rssi */
#define UPD_STA_DOT11K_ELEM_LEN				8		/* mac+util+rcpi*/
#define NEW_STA_ELEM_LEN					7		/* mac+rssi */


#define DEL_STA_ELEM_LEN					7
#define FREE_STA_ELEM_LEN					7
#define SILENT_STA_ELEM_LEN					7
#define UNSILENT_STA_ELEM_LEN				7
#define DOT11V_STA_ELEM_LEN					7
#define	DUAL_STA_ELEM_LEN					7
#endif

// make sure the types really have the right sizes
#define CW_COMPILE_TIME_ASSERT(name, x)               typedef int CWDummy_ ## name[(x) * 2 - 1]

// if you get a compile error, change types (NOT VALUES!) according to your system
CW_COMPILE_TIME_ASSERT(int_size, sizeof(int) == 4);
CW_COMPILE_TIME_ASSERT(char_size, sizeof(char) == 1);

#define		CW_BUFFER_SIZE					8192 //65536
#define		CW_ZERO_MEMORY					bzero
#define		CW_COPY_MEMORY(dst, src, len)			bcopy(src, dst, len)
#define		CW_REPEAT_FOREVER				while(1)

#ifdef RTK_DEBUG
	#define LOG_FILE_SIZE		1000000
#else
	#define LOG_FILE_SIZE		100000
#endif

#ifdef CONFIG_OPENWRT_SDK
#define BR_NAME "br-lan"
#else
#define BR_NAME "br0"
#endif

#ifdef RTK_129X_PLATFORM
#define GETPID_IOCTL 0x8BB2
#endif

typedef enum {
	CW_FALSE = 0,
	CW_TRUE = 1
} CWBool;

typedef enum {
	CW_ENTER_SULKING,
	CW_ENTER_DISCOVERY,
	CW_ENTER_JOIN,
	CW_ENTER_IMAGE_DATA,
	CW_ENTER_CONFIGURE,
	CW_ENTER_DATA_CHECK,
	CW_ENTER_RUN,
	CW_ENTER_RESET,
	CW_QUIT
} CWStateTransition;

extern const char *CW_CONFIG_FILE;
extern int gCWForceMTU;
extern int gCWRetransmitTimer;
extern int gCWNeighborDeadInterval;
extern int gCWMaxRetransmit; 
extern int gMaxLogFileSize;
extern int gEnabledLog;

#define	CW_FREE_OBJECT(obj_name)		do{if(obj_name){free((obj_name)); (obj_name) = NULL;}}while(0)
#define	CW_FREE_OBJECTS_ARRAY(ar_name, ar_size)	do{int _i; for(_i = ((ar_size)-1); _i >= 0; _i--) {if(((ar_name)[_i]) != NULL){ free((ar_name)[_i]);}} free(ar_name); (ar_name) = NULL;}while(0)
#define	CW_PRINT_STRING_ARRAY(ar_name, ar_size)	do{int i; for(i = 0; i < (ar_size); i++) printf("[%d]: **%s**\n", i, (ar_name)[i]);}while(0)

// custom error
#define	CW_CREATE_OBJECT_ERR(obj_name, obj_type, on_err)	do{(obj_name) = (obj_type*) (malloc(sizeof(obj_type))); if(!(obj_name)) {on_err}}while(0)
#define	CW_CREATE_OBJECT_SIZE_ERR(obj_name, obj_size,on_err)	do{(obj_name) = (malloc(obj_size)); if(!(obj_name)) {on_err}} while(0)
#define	CW_CREATE_ARRAY_ERR(ar_name, ar_size, ar_type, on_err)	do{(ar_name) = (ar_type*) (malloc(sizeof(ar_type) * (ar_size))); if(!(ar_name)) {on_err}}while(0)
#define	CW_CREATE_STRING_ERR(str_name, str_length, on_err)	do{(str_name) = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(!(str_name)) {on_err}}while(0)
#define	CW_CREATE_STRING_FROM_STRING_ERR(str_name, str, on_err)	do{CW_CREATE_STRING_ERR((str_name), strlen(str), on_err); strcpy((str_name), (str));}while(0)

#ifdef CW_DEBUGGING

#define	CW_CREATE_ARRAY_ERR2(ar_name, ar_size, ar_type, on_err)		{ar_name = (ar_type*) (malloc(sizeof(ar_type) * (ar_size))); if((ar_name)) {on_err}}
#define	CW_CREATE_OBJECT_ERR2(obj_name, obj_type, on_err)		{obj_name = (obj_type*) (malloc(sizeof(obj_type))); if((obj_name)) {on_err}}
#define	CW_CREATE_OBJECT_SIZE_ERR2(obj_name, obj_size,on_err)		{obj_name = (malloc(obj_size)); if((obj_name)) {on_err}}
#define	CW_CREATE_STRING_ERR2(str_name, str_length, on_err)		{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if((str_name)) {on_err}}
#define	CW_CREATE_STRING_FROM_STRING_ERR2(str_name, str, on_err)	{CW_CREATE_STRING_ERR2(str_name, strlen(str), on_err); strcpy((str_name), str);}

#endif

#include "CWStevens.h"
#include "config.h"
#include "CWLog.h"
#include "CWErrorHandling.h"

#include "CWRandom.h"
//#include "CWTimer.h"
#include "timerlib.h"
#include "CWThread.h"
#include "CWNetwork.h"
#include "CWList.h"
#include "CWSafeList.h"

#include "CWProtocol.h"
#ifndef CW_NO_DTLS
#include "CWSecurity.h"
#endif
#include "CWConfigFile.h"

int CWTimevalSubtract(struct timeval *res, const struct timeval *x, const struct timeval *y);
unsigned int CWGetTimeInMins();
unsigned int CWGetTimeInSeconds();
unsigned long CWGetTimeInMilliSeconds();
CWBool CWParseSettingsFile();
void CWErrorHandlingInitLib();
int pidfile_acquire(char *pidfile);
void pidfile_write_release(int pid_fd);
pid_t rtk_getpid();
#endif
