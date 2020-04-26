#ifndef _SYSTEM_VIDEO_OUT_
#define _SYSTEM_VIDEO_OUT_
#include <sys/types.h>
//#include "IPC/include/xdr/xdr.h"
#include "IPC/generate/include/system/VideoRPCBaseDS_data.h"
#include "IPC/generate/include/system/VideoRPC_System.h"
#include "setupdef.h"
#include <Platform_Lib/HDMIControl/HDMITVSystemTypes.h>

class VoutUtil {
public:
#ifdef REALTEK_SKYPE
    enum { VIDEO_PLANE_NUM = VO_VIDEO_PLANE_WIN2+1, }; // V1, V2, NONE, WIN1, WIN2
#else
    enum { VIDEO_PLANE_NUM = 2, };
#endif

private:
    static VO_RECTANGLE m_VideoRect [VIDEO_PLANE_NUM][VIDEO_SYSTEM_NUM];
    static bool bIs4K2K3DMode;

public:
    static VoutUtil& instance(bool bNeedInitRPC = true);

private:
	VoutUtil(bool bNeedInitRPC);
	~VoutUtil();

    static void cleanup();
    static VoutUtil* mInstance;

    // not copyable
    VoutUtil(VoutUtil const&);
    void operator=(VoutUtil const&);

    void initRPC();

public:
    CLNT_STRUCT m_clnt;
    enum VO_STANDARD m_tv_standard;

public:
    HRESULT
    SetVideoStandard(
        enum VO_STANDARD standard,
        u_char enProg,
        u_char enDIF,
        u_char enCompRGB,
        enum VO_PEDESTAL_TYPE pedType,
        u_int  dataInt0 = 0,
        u_int  dataInt1 = 0,
        bool   bHdmi = true,
#if IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
        int    format3d = -1,
        VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig = NULL,
    	VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig = NULL,
		bool bUpdateVideo = true);
#else
	int    format3d = -1);
#endif

#if IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
	HRESULT SetTvSystem(
    	VIDEO_RPC_VOUT_CONFIG_VIDEO_STANDARD structSetVideoStandard,
    	bool bUpdateHdmi,
    	ENUM_VIDEO_SYSTEM hdmiSystem,
    	u_char enProg,
    	VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig = NULL,
    	VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig = NULL,
		bool bUpdateVideo = true);
#endif

        HRESULT setTvResolution(
                ENUM_VIDEO_SYSTEM videoSystem,
                ENUM_VIDEO_STANDARD videoStandard,
                ENUM_VIDEO_SYSTEM dp_videoSystem = VIDEO_NTSC,
                ENUM_VIDEO_STANDARD dp_videoStandard = VIDEO_INTERLACED,
                int videoType = 0);


public:
    void UpdateVoutRectangleSetup (VO_VIDEO_PLANE videoPlane, ENUM_VIDEO_SYSTEM videoSystem, VO_RECTANGLE rect);
    VO_RECTANGLE GetVoutRectangleSetup (VO_VIDEO_PLANE videoPlane, ENUM_VIDEO_SYSTEM videoSystem);
    HRESULT ApplyVoutDisplayWindowSetup (VO_COLOR borderColor, u_char enBorder = 0, ENUM_VIDEO_SYSTEM videoSystem = VIDEO_SYSTEM_NUM);
    #if IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
    HRESULT ApplyVideoStandardSetup (VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig = NULL,
    									VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig = NULL,
										bool bUpdateVideo = true,
										ENUM_VIDEO_SYSTEM vSystem = VIDEO_SYSTEM_NUM,
										bool bCheck = true);
    #else
    HRESULT ApplyVideoStandardSetup ();
    #endif

#ifdef REALTEK_SKYPE
	HRESULT ApplyVoutDisplayWindowSetup (VO_VIDEO_PLANE videoPlane, VO_COLOR borderColor, u_char enBorder = 0);
    HRESULT ConfigMixerWindow(int showWin1=1, int showWin2=1);
    HRESULT ResetMixerWindow();
#endif

public:
    HRESULT
    SetTVtype(VO_TV_TYPE tvType);

    HRESULT
    SetBackground(
        struct VO_COLOR bgColor,
        u_char bgEnable);

    HRESULT
    SetClosedCaption(
        u_char enCC_odd,
        u_char enCC_even);

    HRESULT
    SetAPS(
        u_char enExt,
        enum VO_VBI_APS APS);

    HRESULT
    SetCopyMode(
        u_char enExt,
        enum VO_VBI_COPY_MODE copyMode);

    HRESULT
    SetAspectRatio(
        u_char enExt,
        enum VO_VBI_ASPECT_RATIO aspectRatio);

public:
    HRESULT
    ConfigureDisplayWindow(
        enum VO_VIDEO_PLANE videoPlane,
        struct VO_RECTANGLE videoWin,
        struct VO_RECTANGLE borderWin,
        struct VO_COLOR borderColor,
        u_char enBorder);

    HRESULT
    ConfigureDisplayWindowZoomWin(
        enum VO_VIDEO_PLANE videoPlane,
        struct VO_RECTANGLE videoWin,
        struct VO_RECTANGLE srcZoomWin,
        struct VO_RECTANGLE borderWin,
        struct VO_COLOR borderColor,
        u_char enBorder);

    HRESULT
    ConfigureDisplayWindowDispZoomWin(
        enum VO_VIDEO_PLANE videoPlane,
        struct VO_RECTANGLE videoWin,
        struct VO_RECTANGLE dispZoomWin,
        struct VO_RECTANGLE borderWin,
        struct VO_COLOR borderColor,
        u_char enBorder);

    HRESULT
    GetFirmwareVersion(u_int * version);

public:
    HRESULT
    SetV2alpha(u_char v2Alpha);

    HRESULT
    SetRescaleMode(
        enum VO_VIDEO_PLANE videoPlane,
        enum VO_RESCALE_MODE rescaleMode,
        struct VO_RECTANGLE rescaleWindow);

    HRESULT
    SetDeintMode(
        enum VO_VIDEO_PLANE videoPlane,
        enum VO_DEINT_MODE deintMode);

    HRESULT
    Zoom(
        enum VO_VIDEO_PLANE videoPlane,
        struct VO_RECTANGLE zoomWin);
#if (IS_CHIP(JUPITER)||IS_CHIP(SATURN))
	//zoom video based on actual video size
	HRESULT Zoom(
		enum VO_VIDEO_PLANE videoPlane,
		enum VO_ZOOM_TYPE type);
#endif
    HRESULT
    ConfigureOSD(
        enum VO_OSD_LPF_TYPE lpfType,
        short RGB2YUVcoeff[12]);

    //It the OSD window is successfully created, return the ID of the just created OSD window
	//    //(a positive integer). Otherwise return the error code (a negative integer).
    int
    CreateOSDwindow(
        struct VO_RECTANGLE winPos,
        enum VO_OSD_COLOR_FORMAT colorFmt,
        int colorKey,
        u_char alpha);

    HRESULT
    ModifyOSDwindow(
        u_char winID,
        u_char reqMask,
        struct VO_RECTANGLE winPos,
        enum VO_OSD_COLOR_FORMAT colorFmt,
        int colorKey,
        u_char alpha,
		u_short startX,
		u_short startY,
    	u_short imgPitch,
	    long pImage,
        bool bBatchJob=false);

    HRESULT
    DeleteOSDwindow(u_char winID, bool bBatchJob=false);

    HRESULT
    DrawOSDwindow(
        u_char winID,
        u_short startX,
        u_short startY,
        u_short imgPitch,
        long pImage,
        bool bBatchJob=false);

    HRESULT
    HideOSDwindow(u_char winID, bool bBatchJob=false);

    HRESULT
    ConfigOSDCanvas ( struct VO_RECTANGLE srcWin, struct VO_RECTANGLE dispWin, bool bBatchJob=false);

#if !IS_CHIP(VENUS)
    HRESULT
    ConfigGraphicCanvas (VO_GRAPHIC_PLANE plane, struct VO_RECTANGLE srcWin, struct VO_RECTANGLE dispWin, bool bBatchJob=false);
#endif

    HRESULT
    ConfigureCursor(
        char alpha,
        char colorKey,
        struct VO_COLOR colorMap[4],
        enum VO_OSD_LPF_TYPE lpfType,
        long pCursorImg);

    HRESULT
    DrawCursor( u_short x, u_short y);

    HRESULT
    HideCursor();

    HRESULT
    SetPeakingStrength(u_char peakingStrength);

    HRESULT
    SetBrightness(u_char brightness);

    HRESULT
    SetContrast(u_char contrast);

    HRESULT
    SetHue(u_char hue);

    HRESULT
    SetSaturation(u_char saturation);

    HRESULT
    GetTvDim (VO_STANDARD standard, int *pwidth, int *pheight);

#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	HRESULT
	SetSubtitleYoffset(short yOffset);
#endif

	HRESULT SetFormat3d(int format3d, float fps = 0, bool bCheck = true);
#if IS_CHIP(SATURN)
	HRESULT SetShiftOffset3d(bool exchange_eyeview, bool shift_direction, int delta_offset, VO_VIDEO_PLANE targetPlane);
#endif
	HRESULT set3Dto2D(VO_3D_MODE_TYPE srcformat3D);
	HRESULT set3DSub(u_char sub);

#if IS_CHIP(JUPITER)
	HRESULT SetComposite3d(long sbsEnable);
#endif

#if IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	HRESULT SetAnaglyphConversion(long enable, long switchSrcEye = 0, VO_3D_SOURCE_FORMAT srcFormat = VO_SIDE_BY_SIDE);
#endif
#if defined(BITLAND_VGA)
	HRESULT SetVGAOutput();
#endif
#if IS_CHIP(SATURN)
	HRESULT ConfigFullWin3dMode(int mode);
#endif

	HRESULT ConfigLowDelayMode(int mode);

    HRESULT Configure_Z_Order(VIDEO_RPC_VOUT_SET_MIXER_ORDER * z_config);
#if IS_CHIP(SATURN)
    /* If return is S_OK, you can check 'out' content to know the detailed info
     * If return is S_FALSE, it means the display plan is disabled or invalid.
     *     out->result == 0: display plane is disabled.
     *     out->result == -1: display plane is invalid.
     * If return is E_FAIL, it means the function failed. Ignore 'out' result.
     */
    HRESULT QueryDisplayWin(VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id, VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT *out);

    /* If return is S_OK, you can check 'out' content to know the detailed info.
     * If return is S_FAILE, it means the display plane or graphic win id is invalid.
     * If return is E_FAIL, it means the function failed. Ignore 'out' result.
     */
    HRESULT QueryGraphicWinInfo(VIDEO_RPC_VOUT_QUERY_GRAPHIC_WIN_INFO_IN graphic_win_id, VIDEO_RPC_VOUT_QUERY_GRAPHIC_WIN_INFO_OUT *out);

    /* Call VIDEO_RPC_VOUT_ToAgent_QueryConfigTVSystem_0
     */
    HRESULT QueryConfigTVSystem(VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *out);

#endif
    HRESULT ShowVideoWindow(long videoPlaneInstanceId, enum VO_VIDEO_PLANE videoPlane = VO_VIDEO_PLANE_V1);
    HRESULT HideVideoWindow(long videoPlaneInstanceId, enum VO_VIDEO_PLANE videoPlane = VO_VIDEO_PLANE_V1);
    void getTVS(video_system videoSystem, VO_STANDARD *standard, VO_PEDESTAL_TYPE *pedType);
    HRESULT SetDisplayRatio(int ratio);
    HRESULT SetDisplayPosition(int x, int y, int w, int h);
    HRESULT getHWCV1Rect(VO_RECTANGLE* rect);

    HRESULT get3DFormat(float fps, ENUM_VIDEO_SYSTEM* video_system, ENUM_VIDEO_STANDARD* video_standard);
    bool isHdmi3dSupported(ENUM_VIDEO_ID_CODE videoFormat3D, unsigned char *capabilityResolution);

#if defined(__LINARO_SDK__)
    HRESULT ApplyTVSystem(ENUM_VIDEO_ID_CODE vic, ENUM_COLOR_FORMAT colorFormat,ENUM_DEEP_COLOR deepColor, ENUM_COLOR_SPACE colorSpace);
    HRESULT ApplyVoutDisplayWindow(ENUM_VIDEO_ID_CODE vic);
private:
    void getVOStandard(ENUM_VIDEO_ID_CODE vic, VO_STANDARD *standard, VO_PEDESTAL_TYPE *pedType, unsigned char *enProg);
    void updateVOStandard(VO_STANDARD *standard, int format3d = -1);
#endif

public:
    HRESULT SetEnhancedSDR(ENUM_SDRFLAG flag);
    HRESULT QueryTVCapability(VIDEO_RPC_VOUT_QUERYTVCAP *tvCap);
    bool isCVBSOn();
    HRESULT setCVBSOff(int cvbs_power_off);
    HRESULT SetPeakLuminance(int flag);
    HRESULT SetHdrSaturation(int flag);
    HRESULT setHdmiRange(int rangeMode);
    HRESULT SetCVBSDisplayRatio(int ratio);
    HRESULT setEmbedSubDisplayFixed(int sub_fixed);
    HRESULT SetSuperResolutionOff(int sr_off);
    HRESULT SetHdrtoSDRgma(int mode);
    HRESULT SetMaxCLLMaxFALLDisable(int disable);
};

#endif

