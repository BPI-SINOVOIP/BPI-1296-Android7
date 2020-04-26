/**
/////////////////////////////////////////////////////////////////////////////
//	@file InputDV_mips.h
//
//	@brief Implementation for DV input plug-in 
//	@author Rey    12/16/2005
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
**/
#ifndef __INPUT_DV_MIPS_H__
#define __INPUT_DV_MIPS_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/PrivateInfo.h>
#include <hresult.h>
#include <assert.h>
                                                                                                                             
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

#ifdef __cplusplus
extern "C" {
#endif

#define NO_FLUSH        1
#define AUDIO_MASTER    1
#define DYNAMIC_AVSYNC  1

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

typedef enum{
  play_mode,
  stop_mode,
  rec_mode,
  media_mode,
  idle_mode,
  open_mode,
} DV_MODE;


typedef enum{
    DV_OP_STATE_DISCONNECT,
    DV_OP_STATE_CONNECT,
    DV_OP_STATE_DATA,    
}DV_OP_STATE;

// DV Input Plugin Sub State
typedef enum{
    DATA_NEW_SEGMENT,
    DATA_READING,
    DATA_END_SEGMENT_1, 
    DATA_END_SEGMENT_2,  
    DATA_FLUSH,
    DATA_EXIT
}DV_OP_SUB_STATE_DATA;

typedef enum{
    CONNECT_NOT_READED,
    CONNECT_READED,
}DV_OP_SUB_STATE_CONNECT;


#define AV_NODEID                    0x50
                                     
#define AV_RX_DMA_CONTROL            0x800
                                     
#define AV_RX_DMA_RING_TOP           0x844
#define AV_RX_DMA_RING_BOTTOM        0x848
#define AV_RX_DMA_RING_RP            0x84C//firmware read pointer for ring buffer
#define AV_RX_DMA_RING_WP            0x850//firmware write pointer for ring buffer
#define AV_RX_DMA_RING_CP            0x854//Hardware write pointer for ring buffer(current pointer)
#define AV_RX_DMA_PACKET_COUNT       0x858//packet count arrived 192 or 480
#define AV_RX_DMA_PACKET_DROPPED     0x85C//packet count dropped
#define AV_RX_DMA_PACKET_ERROR       0x860//packet error count detected
#define AV_RX_DMA_INT_ERROR          0x864
#define AV_RX_DMA_INT_TS_ENABLE      0x868
#define AV_RX_DMA_SRAM_SIZE          0x86C
#define AV_RX_DMA_COUNT_SELECT       0x870
#define AV_RX_DMA_FIFO_COUNT         0x874
#define AV_RX_DMA_ENABLE             0x878
#define AV_RX_DMA_FIFO_THRESHOLD     0x87C
#define AV_RX_DMA_RING_LENGTH        0x884//available data in ring buffer
#define AV_RX_DMA_RING_THRESHOLD     0x888//in bytes size will compare with AV_RX_DMA_RING_LENGTH
#define AV_RX_DMA_INT_STATUS         0x88C
#define AV_RX_DMA_RINGBUF_INT_ENABLE 0x890

#define READ_BLOCK_SIZE 480

typedef struct RB{
    char  has_sleep;            // 0 :none , 1:have
    ULONG top;                  //top address
    ULONG bot;                  //top address
    ULONG old_cp;               //old ringbuffer cp address( HW current pointer)
    ULONG cp;                   // ringbuffer cp address( HW current pointer)
    char  folded;               // to mark if the data in ringbuffer restart from top.      
    ULONG rdptr;                // SW read pointer
    unsigned char *pUserBuf;    // to to connect the str = mmap(); to record where the write pointer of str 	
}RingBuffer;

typedef struct dv_video_nav_s
{         
    // active        file handler  
    char             *mrl; // file name; shared pointer  
    int              fh;   // need to close
    char             *uri; // need to free    
    // private info field 
    unsigned char    dv_status; // need to reset
    unsigned char    decode_cmd_flag; // need to reset
#define PREVIEW_BUF 32
    unsigned char preview_buf[PREVIEW_BUF];
}dv_video_nav_t;

typedef struct dv_state_n_time{
    char  time[4];
    DV_STATE  state;
}dv_state_n_time_t;

typedef struct DmaChInfo1
{
	unsigned char 	uchIsoRxType;
	unsigned char	uchChannel;	
}DmaChInfo_t1;

typedef struct _bus_connection_info    //add by rey  for auto-detect
{
    unsigned int  node_num;
    unsigned short node_id;
}bus_info;

typedef struct 
{    
    dv_video_nav_t nav;
    unsigned char * navbuf_buffer; // buffer for NAVBUF data
    int             last_select_nav; // data send by this nav
    int             last_load_nav;   // file is loaded into this nav
    int             last_load_channel; // last used channel
        
    // active status
    int64_t         m_pts;
    int64_t         m_baseTickTimer;      // reset when stop  
    bool            m_bRunning;           // whether presentation is running or paused
    int64_t         m_lastLocalPTS;       // last updated local PTS
    int64_t         m_lastPresentedPTS;   // last updated presented PTS
    int64_t         m_tickTimer;          // counting timer ticks (90khz)
    int64_t         m_tickSincePresented; // ticks since latest presented PTS (90khz)    
    uint64_t        m_rev_bytes;          // have reveive how many bytes from One New-Segment
    int             m_fr_blocks;          // to check where now the block locates inside frame 
    bool    		r1394_load;
    int             n1394_DV_Nodes;
    int             DVId;
    int             state_thread_exit;
    pthread_t       state_thread_id;
    int             busreset_thread_exit;
    pthread_t       busreset_thread_id;
    RingBuffer      m_rb;
    osal_mutex_t*   dv_mutex;
    unsigned int    mmap_offset;
    int             handle;
    int             FrameSize;
    char            fBufferNearFull;      
    char            reset;    
    char            ts[4];
    DV_MODE         dv_mode; 
    char            signal_mode;
    char            AVSyncMode;
    unsigned short  usDestnId;
    DV_OP_STATE     dv_op_state;
}dv_input_plugin_t;

typedef uint8_t  byte_t;
typedef uint32_t quadlet_t;

HRESULT dv_getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
HRESULT dv_load_media(void* pInstance,char* path,bool* pbContinue);
HRESULT dv_unload_media(void* pInstance);
HRESULT dv_read(void *pInstance,NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
HRESULT dv_read_block(void *pInstance,NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
HRESULT dv_handle_events(void* pInstance,NAV_CMD_ID id,void* cmdData,
                         unsigned int cmdDataSize,unsigned int* pIsFlushRequired);
HRESULT dv_updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
HRESULT dv_privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
HRESULT dv_propertySetGet(void* pInstance,   NAV_PROP_ID id, void* inData, unsigned int inDataSize,
                          void* outData, unsigned int outDataSize, unsigned int* returnedSize);
HRESULT dv_dispose(void* pInstance);
int AvDMAread(RingBuffer *rb,unsigned char **ptrBuf,unsigned int *data_amount,char dv_mode);
void RB_Ptr_Update(RingBuffer  *rb,unsigned char **ptrBuf,unsigned int data_amount);
char GetPlayState(char ts_state);
char GetWindState(char ts_state);

// reset_type
#define RESET_DVIN          0x01
#define SEND_EOS            0x02

// DV SCENARIOR
#define DV_SCENARIO_CAMERA     0        
#define DV_SCENARIO_VCR        1

// Transport Mode Definition
#define SIGMOD_UNKNOWN        -1
#define SIGMOD_NTSC            0
#define SIGMOD_PAL             1

// Sub Unit Type
#define SUNIT_TAPE_RECORDER 0x04
#define SUNIT_CAMERA        0x07
#define SUNIT_UNIT          0x1F

// Transport Mode Definition
#define TM_LOADMEDIA        0xC1
#define TM_REC              0xC2
#define TM_PLAY             0xC3
#define TM_WIND             0xC4

// DV Connection Status
#define STATUS_DV_FOUND        1
#define STATUS_NO_CONNECT      0
#define STATUS_INVALID_ID     -1
#define STATUS_NO_DV          -2
#define STATUS_AVC_ERR        -3

/* VCR subunit command operands */
#define VCR_LOAD_MEDIUM_EJECT      0x60
#define VCR_LOAD_MEDIUM_OPEN_TRAY  0x31
#define VCR_LOAD_MEDIUM_CLOSE_TRAY 0x32
#define VCR_PLAY_NEXT_FRAME        0x30
#define VCR_PLAY_SLOWEST_FORWARD   0x31
#define VCR_PLAY_SLOW_FORWARD_6    0x32
#define VCR_PLAY_SLOW_FORWARD_5    0x33
#define VCR_PLAY_SLOW_FORWARD_4    0x34
#define VCR_PLAY_SLOW_FORWARD_3    0x35
#define VCR_PLAY_SLOW_FORWARD_2    0x36
#define VCR_PLAY_SLOW_FORWARD_1    0x37
#define VCR_PLAY_X1_FORWARD        0x38
#define VCR_PLAY_FAST_FORWARD_1    0x39
#define VCR_PLAY_FAST_FORWARD_2    0x3A
#define VCR_PLAY_FAST_FORWARD_3    0x3B
#define VCR_PLAY_FAST_FORWARD_4    0x3C
#define VCR_PLAY_FAST_FORWARD_5    0x3D
#define VCR_PLAY_FAST_FORWARD_6    0x3E
#define VCR_PLAY_FASTEST_FORWARD   0x3F
#define VCR_PLAY_PREVIOUS_FRAME    0x40
#define VCR_PLAY_SLOWEST_REVERSE   0x41
#define VCR_PLAY_SLOW_REVERSE_6    0x42
#define VCR_PLAY_SLOW_REVERSE_5    0x43
#define VCR_PLAY_SLOW_REVERSE_4    0x44
#define VCR_PLAY_SLOW_REVERSE_3    0x45
#define VCR_PLAY_SLOW_REVERSE_2    0x46
#define VCR_PLAY_SLOW_REVERSE_1    0x47
#define VCR_PLAY_X1_REVERSE        0x48
#define VCR_PLAY_FAST_REVERSE_1    0x49
#define VCR_PLAY_FAST_REVERSE_2    0x4A
#define VCR_PLAY_FAST_REVERSE_3    0x4B
#define VCR_PLAY_FAST_REVERSE_4    0x4C
#define VCR_PLAY_FAST_REVERSE_5    0x4D
#define VCR_PLAY_FAST_REVERSE_6    0x4E
#define VCR_PLAY_FASTEST_REVERSE   0x4F
#define VCR_PLAY_REVERSE           0x65
#define VCR_PLAY_REVERSE_PAUSE     0x6D
#define VCR_PLAY_FORWARD           0x75
#define VCR_PLAY_FORWARD_PAUSE     0x7D

#define VCR_RECORD_UNSPEC_INSERTED   0x30
#define VCR_RECORD_AREA_23_INSERTED  0x31
#define VCR_RECORD_AREA_1_INSERTED   0x32
#define VCR_RECORD_AREA_123_INSERTED 0x33
#define VCR_RECORD_UNSPEC_INSERTED_PAUSE   0x40
#define VCR_RECORD_AREA_23_INSERTED_PAUSE  0x41
#define VCR_RECORD_AREA_1_INSERTED_PAUSE   0x42
#define VCR_RECORD_AREA_123_INSERTED_PAUSE 0x43
#define VCR_RECORD_RECORD 0x75
#define VCR_RECORD_PAUSE  0x7D

#define VCR_TRANSPORT_STATE 0x7F
#define VCR_WIND_HIGH_SPEED_REWIND 0x45
#define VCR_WIND_STOP   0x60
#define VCR_WIND_REWIND 0x65
#define VCR_WIND_FAST_FORWARD 0x75
#define VCR_TIME_CODE_CONTROL 0x20
#define VCR_TIME_CODE_STATUS  0x71
#define VCR_RELATIVE_TIME_COUNTER_CONTROL 0x20
#define VCR_RELATIVE_TIME_COUNTER_STATUS  0x71
#define VCR_TRANSPORT_STATE 0x7F
#define VCR_RECORDING_TIME_STATUS 0x71

#ifdef DV_MSG_PRINT
  #undef  DV_MSG_PRINT  
#endif
#ifdef DV_WARNING_PRINT
  #undef  DV_WARNING_PRINT  
#endif
#ifdef _WARNING_PREFIX_
  #undef  _WARNING_PREFIX_  
#endif
#ifdef _FATAL_PREFIX_
  #undef  _FATAL_PREFIX_  
#endif

#define _DVIN_PREFIX_       "[DV_IN] "
#define _WARNING_PREFIX_    "WARNING : "
#define _FATAL_PREFIX_      "FATAL : "

#define _DVIN_MSG_          _DVIN_PREFIX_
#define _DVIN_WARNING_      _DVIN_PREFIX_ _WARNING_PREFIX_
#define _DVIN_FATAL_        _DVIN_PREFIX_ _FATAL_PREFIX_

// Lv1 Msg : Debug Message <Maskable>
#ifdef DBG_MSG_EN
  #define DV_MSG_PRINT  printf
#else   
  #define DV_MSG_PRINT  (void)
#endif  /* DEBUG */

// Lv2 Msg : Warning Message <unMaskable>
#define DV_WARNING_PRINT  printf    // unMaskable Printf

#ifdef DBG_NAVBUFFER_EN  
  #define DBG_NAVBUFFER_PRINT DV_MSG_PRINT
#else   
  #define DBG_NAVBUFFER_PRINT (void)
#endif  /* DBG_NAVBUFFER_EN */

#endif /*__INPUT_DV_H__*/
