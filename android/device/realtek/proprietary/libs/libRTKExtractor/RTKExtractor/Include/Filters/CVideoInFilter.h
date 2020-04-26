#ifndef __CVIDEO_IN_FILTER_H__
#define __CVIDEO_IN_FILTER_H__

#include <CStream.h>

#ifdef TARGET_BOARD
  #include <VideoRPC_System.h>
#endif

class CVideoInFilter : public CBaseFilter
{
public:
    CVideoInFilter();
    virtual ~CVideoInFilter();

    //derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);

    HRESULT    Configure(ENUM_TVD_VIDEO_FORMAT video_fmt, ENUM_TVD_INPUT_FORMAT input_fmt,
                          ENUM_TVD_YC_SEP_MODE ycsep_mode, u_char default_ycsep,
                          u_char mv_workaround_en);
    HRESULT    SelectInputFormat(ENUM_TVD_INPUT_FORMAT format);
    HRESULT    YCSeparation(ENUM_TVD_YC_SEP_MODE mode);
    HRESULT    VBIControl(u_char VBI_slicer_en);
    HRESULT    AdjustContrast(u_char value);
    HRESULT    AdjustBrightness(u_char value);
    HRESULT    AdjustSaturation(u_char value);
    HRESULT    AdjustHue(u_char value);
    HRESULT    BlueScreenCtrl(u_char mode);
    CHANNEL_RES ChangeChannel_Start(long value);
private:
    long           m_agentInstanceID;
#ifdef TARGET_BOARD
    CLNT_STRUCT    m_rpcclnt;
#else
    int            m_rpcclnt;
#endif
    CBaseOutputPin* m_pOutputPin;
};

#endif  // __CVIDEO_IN_FILTER_H__


