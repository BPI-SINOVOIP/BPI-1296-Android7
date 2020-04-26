#ifndef __VBIDECODER_H__
#define __VBIDECODER_H__

#include <Filters/CVideoEncoderFilter.h>
#include <Filters/vbidef.h>

typedef struct _vbi_tt_filter {
	unsigned char   magazine;
	unsigned char   packet;
	unsigned char   inUse;
} VBI_TT_FILTER;

typedef struct _cni_info {
	unsigned short   vps_cni;
	unsigned short   pdc_cni;
	unsigned short   ni;
} CNI_INFO;

class VBIDecoder
{
public:
    VBIDecoder (CVideoEncoderFilter* pVideoEncoder);
    ~VBIDecoder ();

    static int RegisterFunc(void(*pFunc)(int));
    static void UnregisterFunc(void);
    static int AddDataFilter(ENUM_TVD_VBI_DATA vbiType, unsigned int *param);
    static int RemoveDataFilter(int id);
    static unsigned char *GetVbiDataPtr(void);

    void StartDeliver();
    void StopDeliver();

	void DeliverVbiData();
	void FlushVbiData();

    PDC_RECORD_EVENT QueryPDCTimerRecordEvent (int curr_ch, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute);
    void QueryCNI (CNI_INFO *cni_info);
    HRESULT QueryVbiTime (int *year, int *month, int *day, int *hour, int *minute, int *second);

    char *QueryVbiTitle ();

    static void (*m_pVbiDataReadyFunc) (int);
    static unsigned char *m_vbi_data_ptr;
    static VBI_TT_FILTER m_ttFilter[32];

    unsigned char  m_vbi_data[64];

private:
    static void VBIThread(void* pInstance);

    void InitVbiData();
    void DeHammed84(unsigned char *data);
    void BitReverse8(unsigned char *data);
    void MJDtoYMD(int mjd, int *yy, int *mm, int *dd);
    HRESULT UTCtoLTZ(int *year, int *month, int *day, int *hour, int *minute, int *second);

    CThread *m_pThread;
    bool m_bDelivering;

    CMemRingAllocator*  m_pVbiAllocator;
    long m_VbiBufferSize;
    unsigned char* m_pVbiRingUpper;
    unsigned char* m_pVbiRingLower;
    unsigned char* m_pVbiRingBufferRdPtr;

    int m_CurrYear;
    int m_CurrMonth;
    int m_CurrDay;
    int m_CurrHour;
    int m_CurrMinute;
    int m_CurrSecond;

    ENUM_XDS_CLASS m_xds_class;
    int xds_data_counter;
    int m_Pdc_Record_Label;

    PDC_DATA           m_pdc_data[5];
    NI_UTC_DATA        m_ni_utc_data;
    XDS_MISC_TIME_DATA m_xds_misc_time_data;
    XDS_CHANNEL_DATA   m_xds_channel_data;

    unsigned short m_PrevVpsCni[8];
    unsigned short m_PrevPdcCni[8];
    unsigned short m_PrevPdcNi[8];
};
#endif

