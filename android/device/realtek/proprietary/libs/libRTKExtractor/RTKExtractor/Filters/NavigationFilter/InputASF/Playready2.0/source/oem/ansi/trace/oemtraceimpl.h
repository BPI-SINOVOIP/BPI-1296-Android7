/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMTRACEIMPL_H__
#define __OEMTRACEIMPL_H__

ENTER_PK_NAMESPACE;

#if DRM_SUPPORT_TRACING

/* Maximum length of the string that can be records in one tracing record. */
#define TRACE_MAX_STRING_BUFFER         1024

/* Maximum number of DWORD parameters that can be used for a single format string. */
#define TRACE_MAX_STRING_PARAM          3

typedef enum
{
    TRACE_UNKNOWN_PACKET = 0,   
    TRACE_STRING_PACKET = 1,
    TRACE_STRING_ID_PACKET = 2,
    TRACE_EVENT_PACKET = 3,
} eTracePacketType;

/* Data structure used for tracing record header. */
typedef struct __tagTraceHeader
{
    /* Tracing record type. */
    eTracePacketType m_eType;

    /* Scope ID of the tracing record. */
    DRM_DWORD m_dwScopeID;
    
    /* System time of the tracing record. */
    DRMSYSTEMTIME m_tmTraceTime;
} TRACE_HEADER;
    
/* Data structure used for tracing of a string. */
typedef struct __tagTraceStringData
{
    /* Length in characters of the string data (excluding trailing NULL). */
    DRM_DWORD m_cchData;
    
    /* Buffer to store the string data. */
    DRM_CHAR m_rgchData[ TRACE_MAX_STRING_BUFFER + 1 ];
} TRACE_STRING_DATA;

/* Data structure used for tracing of a format string ID with parameters. */
typedef struct __tagTraceStringIDData
{
    /* ID of the format string. */
    DRM_DWORD m_dwFormatID;
    
    /* Status code associated with the format string. */
    DRM_RESULT m_dr;
    
    /* Parameters used for the format string. */
    DRM_DWORD m_cParams;
    DRM_DWORD m_dwParams[ TRACE_MAX_STRING_PARAM ];
} TRACE_STRING_ID_DATA;

/* Data structure used for tracing of an event record. */
typedef struct __tagTraceEventData
{
    /* Event ID. */
    DRM_DWORD m_dwEventID;

    /* Status code associated with the event. */    
    DRM_RESULT m_dr;
} TRACE_EVENT_DATA;

/* Data structure used for tracing record. */
typedef struct __tagTrace_Packet
{
    /* Header of the tracing record. */
    TRACE_HEADER m_oHeader;

    /* Body that is shared by various types of tracing record. */
    union
    {
        TRACE_STRING_DATA m_oString;
        
        TRACE_STRING_ID_DATA m_oStringID;

        TRACE_EVENT_DATA m_oEvent;    
    } m_oBody;
} TRACE_PACKET;

#endif  /* DRM_SUPPORT_TRACING */

EXIT_PK_NAMESPACE;

#endif  /* __OEMTRACEIMPL_H__ */
