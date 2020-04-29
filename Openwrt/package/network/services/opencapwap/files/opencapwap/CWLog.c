/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
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
 *******************************************************************************************/

#include "CWCommon.h"
//#define WRITE_STD_OUTPUT 1 

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

FILE 	*gLogFile = NULL;
int 	gEnabledLog;
int 	gMaxLogFileSize;
char 	gLogFileName[128];

#ifdef RTK_SMART_ROAMING
FILE 	*gRoamLogFile = NULL;
char	gRoamLogFileName[128];
#endif

#ifndef CW_SINGLE_THREAD
	CWThreadMutex gFileMutex;
#ifdef RTK_SMART_ROAMING
	CWThreadMutex gRoamFileMutex;
#endif
#endif

char *gProgName = NULL;


FILE* ResetLogFile(char *LogFileName, FILE *LogFile)
{
	char *tmp_fileName = "/var/log/replica";
    FILE *tmp_fp;
    char ch;
    int line = 0;

    if((tmp_fp=fopen(tmp_fileName, "w")) == NULL){
		printf("%s: open tmp log file error!\n", __FUNCTION__);
    }
	
    
    while (ch = getc(LogFile) != EOF)
    {
        if (ch == '\n')
            line++;
        if (line > LOG_FILE_SIZE/1000)
            putc(ch, tmp_fp);
    }
	
	fclose(LogFile);
    remove(LogFileName);
	
    fclose(tmp_fp);
    rename(tmp_fileName, LogFileName);
	
	if((LogFile=fopen(LogFileName, "a+")) == NULL){
		printf("%s: open new log file error!\n", __FUNCTION__);
	}

//printf("(%d) stop reset log file\n\n", getpid());	
    return LogFile;
}

void CWLogInitFile(const char *fileName) {
	strcpy(gLogFileName, fileName);
	if(fileName == NULL) {
		printf("%s: Wrong File Name for Log File!\n", __FUNCTION__);
	}

	if((gLogFile = fopen(fileName, "a+")) == NULL) {
		printf("%s: Can't open log file: %s\n", __FUNCTION__, strerror(errno));
		exit(1);
	} 

#ifndef CW_SINGLE_THREAD
	if(!CWCreateThreadMutex(&gFileMutex)) {
		printf("%s: Can't Init File Mutex for Log\n", __FUNCTION__);
		exit(1);
	}
#endif

}


CWBool checkResetFile(){
	long fileSize=0;

	if((fileSize=ftell(gLogFile))==-1)
	{
		printf("%s: An error with log file occurred: %s", __FUNCTION__, strerror(errno));
		return CW_FALSE;
	}
	if (fileSize>=gMaxLogFileSize)
	{
#ifdef RTK_SMART_ROAMING
		gLogFile = ResetLogFile(gLogFileName, gLogFile);
#else
		fclose(gLogFile);
		if((gLogFile = fopen(gLogFileName, "w")) == NULL) 
		{
			printf("%s: Can't open log file: %s", __FUNCTION__, strerror(errno));
			return CW_FALSE;
		}
#endif
	}

	return CW_TRUE;
}

#ifdef RTK_SMART_ROAMING
void CWRoamLogInitFile(const char *fileName) {
	strcpy(gRoamLogFileName, fileName);
	
	if(fileName == NULL) {
		printf("%s: Wrong File Name for Roam Log File\n", __FUNCTION__);
	}

	if((gRoamLogFile = fopen(fileName, "a+")) == NULL) {
		printf("%s: Can't open roam log file: %s\n",__FUNCTION__, strerror(errno));
		exit(1);
	}

#ifndef CW_SINGLE_THREAD
	if(!CWCreateThreadMutex(&gRoamFileMutex)) {
		printf("%s: Can't Init File Mutex for Log\n", __FUNCTION__);
		exit(1);
	}
#endif

}
CWBool checkResetRoamFile(){
	long fileSize=0;

	if((fileSize=ftell(gRoamLogFile))==-1)
	{
		printf("%s: An error with log file occurred: %s", __FUNCTION__, strerror(errno));
		return CW_FALSE;
	}
	if (fileSize>=gMaxLogFileSize)
	{
#ifdef RTK_SMART_ROAMING
		gRoamLogFile = ResetLogFile(gRoamLogFileName, gRoamLogFile);
#else
		fclose(gRoamLogFile);
		if((gRoamLogFile = fopen(gRoamLogFileName, "w")) == NULL) 
		{
			printf("%s: Can't open log file: %s", __FUNCTION__, strerror(errno));
			return CW_FALSE;
		}
#endif
	}
	return CW_TRUE;
}
#endif


void CWLogCloseFile() {
#ifndef CW_SINGLE_THREAD
	CWDestroyThreadMutex(&gFileMutex);
#endif

	fclose(gLogFile);

#ifdef RTK_SMART_ROAMING
#ifndef CW_SINGLE_THREAD
	CWDestroyThreadMutex(&gRoamFileMutex);
#endif
	fclose(gRoamLogFile);
#endif
}

__inline__ void CWVLog(const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;
		
	if(format == NULL) return;
	
	now = time(NULL);
	nowReadable = ctime(&now);
	
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
	
	//sprintf(logStr, "[CAPWAP::%s]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%p\t %s %s\n", nowReadable, (unsigned long)CWThreadSelf(),gProgName, format);

	if(gLogFile != NULL) {
		char fileLine[256];
		
#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gFileMutex);
		fseek(gLogFile, 0L, SEEK_END);
#endif
		
		vsnprintf(fileLine, 255, logStr, args);
	
		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}
		
		fwrite(fileLine, strlen(fileLine), 1, gLogFile);
		fflush(gLogFile);
		
#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT
	vprintf(logStr, args);
#endif	
	
	CW_FREE_OBJECT(logStr);
}

__inline__ void CWLog(const char *format, ...) {
	va_list args;
	
	va_start(args, format);
	if (gEnabledLog)
		{CWVLog(format, args);}
	va_end(args);
}

__inline__ void CWDebugLog(const char *format, ...) {
#ifdef CW_DEBUGGING
	char *logStr = NULL;
	va_list args;
	time_t now;
	char *nowReadable = NULL;
	
	if (!gEnabledLog)		return;
	if(format == NULL)		return;
		
	now = time(NULL);
	nowReadable = ctime(&now);
	
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
	
	//sprintf(logStr, "[[CAPWAP::%s]]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%p\t %s %s\n", nowReadable, (unsigned long)CWThreadSelf(), gProgName, format);

	va_start(args, format);
		
	if(gLogFile != NULL) {
		char fileLine[256];
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gFileMutex);
		fseek(gLogFile, 0L, SEEK_END);
#endif
			
		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gLogFile);

		fflush(gLogFile);
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT	
	vprintf(logStr, args);
#endif
		
	va_end(args);
	CW_FREE_OBJECT(logStr);
#endif
}

#ifdef RTK_SMART_ROAMING
__inline__ void CWRoamLog(const char *format, ...) 
{
	char *logStr = NULL;
	va_list args;
	time_t now;
	char *nowReadable = NULL;
		
	if (!gEnabledLog)		return;
	if(format == NULL)		return;
		
	now = time(NULL);
	nowReadable = ctime(&now);		
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);		
	sprintf(logStr, "[CAPWAP::%s]\t%p\t %s %s\n", nowReadable, (unsigned long)CWThreadSelf(), gProgName, format);
	va_start(args, format);
		
	if(gRoamLogFile != NULL) {
		char fileLine[256];
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gRoamFileMutex);
		fseek(gRoamLogFile, 0L, SEEK_END);
#endif
		
		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetRoamFile()) 
		{
			CWThreadMutexUnlock(&gRoamFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gRoamLogFile);
		fflush(gRoamLogFile);
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gRoamFileMutex);
#endif
	}
#ifdef WRITE_STD_OUTPUT	
	vprintf(logStr, args);
#endif
		
	va_end(args);
	CW_FREE_OBJECT(logStr);
}
__inline__ void CWRoamDebugLog(const char *format, ...)
{

#ifdef RTK_DEBUG
	char *logStr = NULL;
	va_list args;
	time_t now;
	char *nowReadable = NULL;
		
	if (!gEnabledLog)		return;
	if(format == NULL)		return;
		
	now = time(NULL);
	nowReadable = ctime(&now);		
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);		
	sprintf(logStr, "[CAPWAP::%s]\t%p\t %s %s\n", nowReadable, (unsigned long)CWThreadSelf(), gProgName, format);
	va_start(args, format);
		
	if(gRoamLogFile != NULL) {
		char fileLine[256];
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexLock(&gRoamFileMutex);
		fseek(gRoamLogFile, 0L, SEEK_END);
#endif
		
		vsnprintf(fileLine, 255, logStr, args);

		if(!checkResetRoamFile()) 
		{
			CWThreadMutexUnlock(&gRoamFileMutex);
			exit (1);
		}

		fwrite(fileLine, strlen(fileLine), 1, gRoamLogFile);
		fflush(gRoamLogFile);
			
#ifndef CW_SINGLE_THREAD
		CWThreadMutexUnlock(&gRoamFileMutex);
#endif
	}

#ifdef WRITE_STD_OUTPUT	
	vprintf(logStr, args);
#endif
		
	va_end(args);
	CW_FREE_OBJECT(logStr);
#endif

}

#endif
