/**
/////////////////////////////////////////////////////////////////////////////
//	@file InputDV.h
//
//	@brief Implementation for DV input plug-in 
//
//	@author Rey     7/7/2005
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
**/

#ifndef __INPUT_DV_H__
#define __INPUT_DV_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/PrivateInfo.h>
#include <hresult.h>
#include <assert.h>

#include "ohci1394/raw1394/raw1394.h"
#include "ohci1394/ohci_read.h"
                                                                                                                             
/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define BUFSIZE        (4096<<10) // 4M 
#define MAX_NAVFILE    2 // support decoder 2 file simutaneously

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif 

#define E_TRYAGAIN   10
#define E_ENDOFLIST  11

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_INPUT_PLUGIN
#else
    HRESULT openInputPluginDV
#endif
        (ENUM_MEDIA_TYPE majorType,
         ENUM_MEDIA_TYPE subType,
         osal_mutex_t* mutex,
         INPUTPLUGIN* pInputPlugin);

#ifdef __cplusplus
}
#endif

typedef enum {
	DV_STATUS,    
	DV_PTS,  
} 	DV_Info;

typedef struct dv_video_nav_s
{         
  // active        file handler  
  char             *mrl; // file name; shared pointer  
  int              fh;   // need to close
  char             *uri; // need to free
  
  // private info field 
  unsigned char       dv_status; // need to reset
#define  DV_STATUS_ACTIVE     (1<<0)   
#define  DV_STATUS_SECONDARY  (1<<1)
  unsigned char       decode_cmd_flag; // need to reset
#define  CMD_VIDEOCODECINIT     (1<<0)
#define  CMD_VIDEODECODECOMMAND (1<<1)
#define  CMD_VIDEOBLENDCOMMAND  (1<<2)
#define  CMD_VIDEONEWSEGMENT    (1<<3)
#define  CMD_VIDEOPREVIEW       (1<<4)
#define  CMD_VIDEOENDOFSEQ      (1<<5)
#define  CMD_VIDEOSTOP          (1<<6)

  NAVFILESESSIONINFO  *dv_session_info; // share pointer  
  VIDEODECODECOMMAND  *decode_command; // shared pointer
  int cnt_blend_cmd;

#define PREVIEW_BUF 32
  unsigned char preview_buf[PREVIEW_BUF];
} dv_video_nav_t;

typedef struct 
{    
    dv_video_nav_t nav[MAX_NAVFILE];
    unsigned char *  navbuf_buffer; // buffer for NAVBUF data
    int last_select_nav; // data send by this nav
    int last_load_nav;   // file is loaded into this nav
    int last_load_channel; // last used channel
    
    unsigned char private_state; 
#define NAV_DV_STATUS_INIT            (1<<0) 
#define NAV_DV_STATUS_START           (1<<1) 

    unsigned char private_info;
#define NAV_DV_STATUS_DV_MODE       (1<<0)
#define NAV_DV_STATUS_PLAYLIST_MODE   (1<<1)
    
  //script file
    struct stat script_fstat;
    int script_handle;
    
  // active status
    int64_t         m_baseTickTimer;      // reset when stop  
    bool            m_bRunning;           // whether presentation is running or paused
    int64_t         m_lastLocalPTS;       // last updated local PTS
    int64_t         m_lastPresentedPTS;   // last updated presented PTS
    int64_t         m_tickTimer;          // counting timer ticks (90khz)
    int64_t         m_tickSincePresented; // ticks since latest presented PTS (90khz)    
    uint64_t       m_rev_bytes; // have reveive how many bytes from One New-Segment
	int                m_fr_blocks; // to check where now the block locates inside frame 
	struct raw1394_handle*	 r1394_handler;
	bool    r1394_load;
	int       n1394_DV_Nodes;
	int       DVId;
	osal_mutex_t*  dv_mutex;
} dv_input_plugin_t;
static HRESULT dv_unload_file(void* pInstance,int chan) ;
static int dv_load_file(void* pInstance, NAVFILESESSIONINFO *info, char *path);
static unsigned int dv_getOutputType(void* pInstance) ;
static HRESULT dv_load_media(void* pInstance,char* path,bool* pbContinue);
static HRESULT dv_unload_media(void* pInstance);
static HRESULT dv_read(void *pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
static HRESULT dv_read_block(void *pInstance,NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
static HRESULT dv_handle_events(void* pInstance,NAV_CMD_ID id,void* cmdData,
                                 unsigned int cmdDataSize,unsigned int* pIsFlushRequired);
HRESULT dv_updatePlaybackPosition(void* pInstance, int64_t localPTS, int64_t presentedPTS);
HRESULT dv_privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
HRESULT dv_propertySetGet(void* pInstance,   NAV_PROP_ID id, void* inData, unsigned int inDataSize,
    void* outData, unsigned int outDataSize, unsigned int* returnedSize);
static HRESULT dv_dispose (void* pInstance);
#endif /*__INPUT_DV_H__*/
