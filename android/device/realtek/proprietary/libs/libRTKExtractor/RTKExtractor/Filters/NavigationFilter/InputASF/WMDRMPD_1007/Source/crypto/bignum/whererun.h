/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _WHERERUN_H
#define _WHERERUN_H 1

#include "bigenv.h"
#include <stdio.h>



#if TARGET_OS == OS_WCE || TARGET_OS == OS_ATTILA

static void where_running(const char *program_id)
{
    printf("%s running on %s\n", program_id, TARGET_NAME);
         // No machine name or time of day under Windows CE
    fflush(stdout);
} // where_running

#else
/*
    This program prints an informative line on
    standard output, describing the program
    being run plus the date and time.
    It is for testing purposes only.
*/
#include <windows.h>
#include <time.h>

static void DRM_API where_running(const char *program_id)
{
    char host_identification[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD lng_buffer = SIZEOF(host_identification);
    time_t tnow;


    host_identification[lng_buffer - 1] = 0;
    GetComputerName(host_identification, &lng_buffer);
    time(&tnow);

    printf("%s running on %s at %s",
            program_id, host_identification, ctime(&tnow));
    fflush(stdout);
} // where_running

#endif // TARGET_OS
#endif