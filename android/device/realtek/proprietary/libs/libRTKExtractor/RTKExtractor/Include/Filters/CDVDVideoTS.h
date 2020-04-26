#ifndef __C_DVD_VIDEO_TS_H__
#define __C_DVD_VIDEO_TS_H__

#include <CStream.h>
#include <OSAL.h>
#include <Utility/database/ebase.h>
#include <io/ifo_types.h>
#include <io/ifo_parse.h>
#include <io/dvd_path_define.h>
#include <Filters/NavigationFilter/CNavigationFilter.h>
#include <Flows/CPlaybackFlowManager.h>
#include <Filters/VideoDecoder/CMPEG2Decoder.h>


#define TS_INFO_PATH "/VIDEO_TS/TS_INFO"
#define DVD_READER_PATH "/DVD_READER"
//#define THUMBNAIL_INFO_PATH "/THUMBNAIL_INFO"

typedef struct 
{
    ifo_handle_t* pIFO;
    osal_mutex_t* pLock;
    unsigned int  referenceCount;
}IFO_INFO;

typedef struct
{
  //Record Info
  char record_title_nr;               // -1 in STOP state
  unsigned char record_title_set_nr;  // 0  in STOP state
  unsigned char record_vts_pgcn;      // 0  in STOP state
  unsigned char record_cell_num;      // 0  in STOP state
  unsigned int record_vobu_end_addr;  // 0  in STOP state
  unsigned int record_total_fields;   //  in fields; 0 in STOP state
  unsigned int record_update_title_menu; //0 if the title menu and IFO are matched

  //Playback Info
  int playback_title_nr;     // -1 in STOP state, 0 in MENU domains

}ts_handle_t;

typedef struct
{
  ts_handle_t * pTS;
  osal_mutex_t* pLock;
  unsigned int  referenceCount;
} TS_INFO;

typedef struct
{
  dvd_info_t* pDVDReader;
  osal_mutex_t* pLock;
  unsigned int  referenceCount;
} DVDREADER_INFO;

/*
typedef struct
{
  CFlowManager*       pPlaybackFManager;
  CNavigationFilter*  pNavigatorFilter;
  CMPEG2Decoder*      pVDecoder;
  CReferenceClock*    pRefClock;
  osal_mutex_t*       pLock;
  unsigned int        referenceCount;
} THUMBNAIL_INFO;
*/

//EBase special data structure
typedef struct {
  unsigned int  size;  
  char          payload[64];
}EBASE_KEY_INFO;

class CDVDVideoTS
{
public:

    CDVDVideoTS();
    virtual ~CDVDVideoTS();

    HRESULT Init   (char* path);
    HRESULT Lock   ();
    HRESULT Unlock ();

    HRESULT GetIFO     (int vtsN, IFO_INFO* pIFO);
    HRESULT SetIFO     (int vtsN, IFO_INFO* pIFO);
    HRESULT ReleaseIFO (int vtsN);

    HRESULT GetTSInfo     (TS_INFO* pTSInfo);
    HRESULT SetTSInfo     (TS_INFO* pTSInfo);
    HRESULT ReleaseTSInfo ();

    HRESULT GetDVDReader(DVDREADER_INFO* pReaderInfo);
    HRESULT SetDVDReader(DVDREADER_INFO* pReaderInfo);
    HRESULT ReleaseDVDReader();

/*
    HRESULT GetThumbnailInfo(THUMBNAIL_INFO* pThumbnailInfo);
    HRESULT SetThumbnailInfo(THUMBNAIL_INFO* pThumbnailInfo);
    HRESULT ReleaseThumbnailInfo();
*/

protected:
    char  m_Locked;
    char* m_pPath;
    Ebase* m_pEbase;
};

#endif //__C_DVD_VIDEO_TS_H__
