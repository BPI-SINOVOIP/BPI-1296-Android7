#ifndef __SETUPCLASS_
#define __SETUPCLASS_

#include <Application/AppClass/setupdef.h>
#include <Application/AppClass/NetworkSet.h>
#include <string.h>

const RECTANGLE_DIMENSION DEFAULT_DIMENSION[VIDEO_SYSTEM_NUM] =
{
#if 0
    { 0, 0,  720,  480 },  // VIDEO_NTSC
    { 0, 0,  720,  576 },  // VIDEO_PAL
    { 0, 0, 1280,  720 },  // VIDEO_HD720_50HZ
    { 0, 0, 1280,  720 },  // VIDEO_HD720_60HZ
    { 0, 0, 1920, 1080 },  // VIDEO_HD1080_50HZ
    { 0, 0, 1920, 1080 },  // VIDEO_HD1080_60HZ
    { 0, 0, 1920, 1080 },  // VIDEO_HD1080_24HZ
    { 0, 0,  800,  600 },  // VIDEO_SVGA800x600
#else
#ifdef USE_HD_SURFACE_0
    { 0, 0,  720,  480 },  // VIDEO_NTSC
    { 0, 0,  720,  576 },  // VIDEO_PAL
    { 0, 0, 1280,  720 },  // VIDEO_HD720_50HZ
    { 0, 0, 1280,  720 },  // VIDEO_HD720_60HZ
    { 0, 0, 1280,  720 },  // VIDEO_HD1080_50HZ
    { 0, 0, 1280,  720 },  // VIDEO_HD1080_60HZ
    { 0, 0, 1280,  720 },  // VIDEO_HD1080_24HZ
    { 0, 0,  800,  600 },  // VIDEO_SVGA800x600
#else
    { 0, 0,  720,  480 },  // VIDEO_NTSC
    { 0, 0,  720,  480 },  // VIDEO_PAL
    { 0, 0,  720,  480 },  // VIDEO_HD720_50HZ
    { 0, 0,  720,  480 },  // VIDEO_HD720_60HZ
    { 0, 0,  720,  480 },  // VIDEO_HD1080_50HZ
    { 0, 0,  720,  480 },  // VIDEO_HD1080_60HZ
    { 0, 0,  720,  480 },  // VIDEO_HD1080_24HZ
    { 0, 0,  720,  480 },  // VIDEO_SVGA800x600
#endif
#endif
};


#define MAX_SIZE_DEVICE_NAME    512

#ifdef ANDROID
typedef enum {
	WIFI_CMD_QUERY_STATUS,
	WIFI_CMD_ENABLE,
	WIFI_CMD_DISABLE,
	WIFI_CMD_SEARCH_AP,
	WIFI_CMD_CONNECT,
	WIFI_CMD_DISCONNECT,
	WIFI_CMD_REMOVE,
	WIFI_CMD_QUERY_CONNECTION
} WIFI_CMD_E;
#endif

class SetupClass {
public:
    SetupClass();
    virtual ~SetupClass();
    virtual void Init()=0;
    virtual void Create()=0;
    virtual HRESULT Sync()=0;
    virtual HRESULT AutoSync()=0;
#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual void RestoreTVChannelDefault()=0;
#endif
    virtual void RestoreAudioDefault()=0;
    virtual void RestoreFactoryDefault()=0;
    virtual int SetNetWlan0ProfileCopy(int DestProfile,int SrcProfile)=0;
    virtual int SetNetWlan0ProfileToDefault(int profile)=0;

    static SetupClass *m_ptr;

    static void SetInstance(SetupClass *pInstance)
    {
        if (m_ptr == NULL)
        {
            m_ptr = pInstance;
            m_ptr->Init();
        }
        else
        {
            delete pInstance;
        }
    }

    static void DeleteInstance(void)
    {
        if (m_ptr)
        {
            delete m_ptr;
            m_ptr = NULL;
        }
    }

    static SetupClass *GetInstance(void)
    {
        return m_ptr;
    }

    virtual inline RECTANGLE_DIMENSION GetDimension(ENUM_VIDEO_SYSTEM video_system) { return DEFAULT_DIMENSION[video_system]; }

    virtual ENUM_OSD_LANGUAGE GetOsdLanguage()=0;

    virtual void SetOsdLanguage(ENUM_OSD_LANGUAGE osd_lang)=0;

    virtual ENUM_AUDIO_LANGUAGE GetAudioLanguage()=0;

    virtual void SetAudioLanguage(ENUM_AUDIO_LANGUAGE audio_lang)=0;

    virtual ENUM_SUBTITLE_LANGUAGE GetSubtitleLanguage()=0;

    virtual void SetSubtitleLanguage(ENUM_SUBTITLE_LANGUAGE subtitle_lang)=0;

    virtual ENUM_MENU_LANGUAGE GetMenuLanguage()=0;

    virtual void SetMenuLanguage(ENUM_MENU_LANGUAGE menu_lang)=0;

    virtual int GetMiniKBDLanguage()=0;

    virtual void SetMiniKBDLanguage(int volume)=0;

    virtual ENUM_TEXT_ENCODING GetTextSubtitleEncoding()=0;

    virtual void SetTextSubtitleEncoding(ENUM_TEXT_ENCODING coding)=0;

    virtual int GetTextSubtitleYOffset()=0;

    virtual void SetTextSubtitleYOffset(int offset)=0;

    virtual int GetTextSubtitleSize()=0;

    virtual void SetTextSubtitleSize(int size)=0;

    virtual ENUM_SUBTITLE_TEXT_COLOR GetTextSubtitleColor()=0;

    virtual void SetTextSubtitleColor(ENUM_SUBTITLE_TEXT_COLOR color)=0;

    virtual int GetTextSubtitleOffset()=0;

    virtual void SetTextSubtitleOffset(int offset)=0;

    virtual ENUM_VIDEO_SYSTEM GetTvSystem()=0;

    virtual void SetTvSystem(ENUM_VIDEO_SYSTEM video_system)=0;

    virtual ENUM_VIDEO_STANDARD GetTvStandard()=0;

    virtual void SetTvStandard(ENUM_VIDEO_STANDARD video_standard)=0;

    virtual VIDEO_SYSTEM_INFO GetVideoSystem() = 0;

    virtual void SetVideoSystem(VIDEO_SYSTEM_INFO video_system_info) = 0;

    virtual VIDEO_SYSTEM_INFO GetVideoSystemManual() = 0;

    virtual void SetVideoSystemManual(VIDEO_SYSTEM_INFO video_system_info) = 0;

    virtual ENUM_VIDEO_SYSTEM GetVideoSDSystem() = 0;

    virtual void SetVideoSDSystem(ENUM_VIDEO_SYSTEM video_sd_system) = 0;

    virtual ENUM_VIDEO_HD_SYSTEM GetVideoHDSystem() = 0;

    virtual void SetVideoHDSystem(ENUM_VIDEO_HD_SYSTEM hd_system) = 0;

    virtual ENUM_ON_OFF GetVideoSystem1080P24HZ()=0;

    virtual void SetVideoSystem1080P24HZ(ENUM_ON_OFF system_1080p24hz)=0;

    virtual int GetErrorConcealmentLevel()=0;

    virtual void SetErrorConcealmentLevel(int level)=0;

    virtual ENUM_TV_SYSTEM_AUTO_MODE GetTvSystemAutoMode()=0;

    virtual void SetTvSystemAutoMode(ENUM_TV_SYSTEM_AUTO_MODE mode)=0;

    virtual ENUM_VIDEO_REC_SYSTEM GetRecTvSystem()=0;

    virtual void SetRecTvSystem(ENUM_VIDEO_REC_SYSTEM video_rec_system)=0;

    virtual ENUM_ON_OFF GetAutoTvSystem()=0;

    virtual void SetAutoTvSystem(ENUM_ON_OFF auto_video_system)=0;

    virtual ENUM_ON_OFF GetVideoPreprocessing()=0;

    virtual void SetVideoPreprocessing(ENUM_ON_OFF preprocessing)=0;

    virtual ENUM_ON_OFF GetCopyProtection()=0;

    virtual void SetCopyProtection(ENUM_ON_OFF aware_copy_protection)=0;

#if AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual ENUM_TV_INPUT_DTV GetTvInputDtv()=0;

    virtual void SetTvInputDtv(ENUM_TV_INPUT_DTV tv_input)=0;
#endif

#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual unsigned int GetTvSystemTypeByRegion(ENUM_TV_REGION region)=0;

    virtual bool IsAtvSystemTypeMN()=0;

    virtual void SetAtvSystemTypeMN(bool isMN)=0;

    virtual ENUM_TV_INPUT GetTvInput()=0;

    virtual void SetTvInput(ENUM_TV_INPUT tv_input)=0;

    virtual ENUM_TV_REGION GetTvRegion()=0;

    virtual void SetTvRegion(ENUM_TV_REGION tv_region)=0;

    virtual ENUM_ON_OFF GetAntennaPower()=0;

    virtual void SetAntennaPower(ENUM_ON_OFF antenna_power)=0;

    virtual void SetChannelStatus(int channel, unsigned char status)=0;

    virtual unsigned char GetChannelStatus(int channel)=0;

    virtual void SetChannelStandard(int channel, unsigned char standard)=0;

    virtual unsigned char GetChannelStandard(int channel)=0;

    virtual void SetChannelCNIStation(int channel, CNI_STATION cni_station)=0;

    virtual CNI_STATION GetChannelCNIStation(int channel)=0;

    virtual void SetChannelFrequency(int channel, unsigned int freq)=0;

    virtual unsigned int GetChannelFrequency(int channel)=0;

    virtual void SetChannelName(int channel, char *name)=0;

    virtual char * GetChannelName(int channel)=0;

    virtual void SetChannelPayTV(int channel, unsigned char pay_tv)=0;

    virtual unsigned char GetChannelPayTV(int channel)=0;
#endif

    virtual ENUM_REC_QUALITY GetRecQuality()=0;

    virtual void SetRecQuality(ENUM_REC_QUALITY quality)=0;

    virtual ENUM_AUTO_CHAPTER GetAutoChapter()=0;

    virtual void SetAutoChapter(ENUM_AUTO_CHAPTER auto_chapter)=0;

    virtual ENUM_REC_DEVICE GetRecDevice()=0;

    virtual void SetRecDevice(ENUM_REC_DEVICE rec_device)=0;

    virtual ENUM_REC_DEVICE GetTimeShiftDevice()=0;

    virtual void SetTimeShiftDevice(ENUM_REC_DEVICE timeshift_device)=0;

    virtual ENUM_REC_PRIORITY GetRecPriority()=0;

    virtual void SetRecPriority(ENUM_REC_PRIORITY rec_priority)=0;

    virtual char * GetLastRecFile()=0;

    virtual void SetLastRecFile(char *file)=0;

    virtual ENUM_ON_OFF GetAutoTimeshift()=0;

    virtual void SetAutoTimeshift(ENUM_ON_OFF auto_timeshift)=0;

    virtual ENUM_SLIDE_SHOW_TIME GetSlideShowTime() = 0;

    virtual void SetSlideShowTime(ENUM_SLIDE_SHOW_TIME slide_show_time) = 0;

    virtual ENUM_TRANS_EFFECT GetTeansEffect() = 0;

    virtual void SetTeansEffect(ENUM_TRANS_EFFECT trans_effect) = 0;

    virtual ENUM_MIX_PLAY GetMixPlay() = 0;

    virtual void SetMixPlay(ENUM_MIX_PLAY mix_play) = 0;

    virtual ENUM_TIME_ZONE GetTimeZone()=0;

    virtual void SetTimeZone(ENUM_TIME_ZONE time_zone)=0;

    virtual ENUM_ON_OFF GetDayLightSaving()=0;

    virtual void SetDayLightSaving(ENUM_ON_OFF day_light_saving) =0;

    virtual ENUM_ON_OFF GetPowerSaving()=0;

    virtual void SetPowerSaving(ENUM_ON_OFF power_saving) =0;

    virtual int GetRatingIsBlockUnrated() = 0;

    virtual void SetRatingIsBlockUnrated(int isBlockUnrated) = 0;

    virtual int GetRatingIsEnable()=0;

    virtual void SetRatingIsEnable(int isEnable)=0;

    virtual ENUM_RATING_LEVEL GetRatingLevel()=0;

    virtual void SetRatingLevel(ENUM_RATING_LEVEL rating_level)=0;

    virtual ENUM_RATING_REGION GetRatingRegion()=0;

    virtual void SetRatingRegion(ENUM_RATING_REGION rating_region)=0;

    virtual ENUM_DTV_AUDIO_LANG GetDtvAudioLang()=0;

    virtual void SetDtvAudioLang(ENUM_DTV_AUDIO_LANG dtvAudioLang)=0;

    virtual ENUM_DTV_SUBTITLE_LANG GetDtvSubtitleLang1()=0;

    virtual void SetDtvSubtitleLang1(ENUM_DTV_SUBTITLE_LANG dtvSubtitleLang)=0;

    virtual ENUM_DTV_SUBTITLE_LANG GetDtvSubtitleLang2()=0;

    virtual void SetDtvSubtitleLang2(ENUM_DTV_SUBTITLE_LANG dtvSubtitleLang)=0;

    virtual ENUM_ON_OFF GetDtvSubtitleHardOfHearing()=0;

    virtual void SetDtvSubtitleHardOfHearing(ENUM_ON_OFF dtvSubtitleHardOfHearing)=0;

    virtual ENUM_ON_OFF GetDtvDisableLCNs()=0;

    virtual void SetDtvDisableLCNs(ENUM_ON_OFF dtvDisableLCNs)=0;

    virtual unsigned char GetRatingType(ENUM_RATING_LEVEL rating_level)=0;

    virtual void SetRatingType(ENUM_RATING_LEVEL rating_level, unsigned char rating_type)=0;

    virtual OPENVCHIP_TABLE_INFO* GetOpenVChipTable()=0;

    virtual OPEN_VCHIP_RATING_INFO* GetOpenVChipRatingInfo()=0;

    virtual void SetOpenVChip()=0;

    virtual int GetRatingTimeout() = 0;

    virtual void SetRatingTimeout(int milliSeconds) = 0;

    virtual int GetRatingPasswd()=0;

    virtual void SetRatingPasswd(int passwd)=0;

    virtual bool IsRatingPasswdCorrect(int passwd)=0;

    virtual int GetTVRatingBitwiseValue()=0;

    virtual void SetTVRatingBitwiseValue(int tv_rating_level)=0;

    virtual int GetTVxRatingBitwiseValue()=0;

    virtual void SetTVxRatingBitwiseValue(int tv_x_rating_level)=0;

    virtual bool GetAutoPlay()=0;

    virtual void SetAutoPlay(bool bAutoPlay)=0;

    virtual ENUM_ON_OFF GetSeamlessPlayback()=0;

    virtual void SetSeamlessPlayback(ENUM_ON_OFF seamless_playback)=0;

#if AP_CONFIG_WITH(APCFG_APP_UPNPDLNA1p5)
    virtual ENUM_ON_OFF GetDlnaDmr()=0;

    virtual void SetDlnaDmr(ENUM_ON_OFF set)=0;

	virtual ENUM_ON_OFF GetDlnaDms()=0;

	virtual void SetDlnaDms(ENUM_ON_OFF set)=0;

	virtual char *GetDlnaDmsDeviceName()=0;

	virtual void SetDlnaDmsDeviceName(char *deviceName)=0;

	virtual char *GetDlnaDmsDeviceUUID()=0;

	virtual void SetDlnaDmsDeviceUUID(char *deviceUUID)=0;
#endif /* APCFG_APP_UPNP_1p5 */

    virtual char *GetSystemDeviceFriendlyName()=0;

    virtual void SetSystemDeviceFriendlyName(char *friendlyName)=0;

    virtual char *GetSystemDeviceUUID()=0;

    virtual void SetSystemDeviceUUID(char *uuid)=0;

    virtual ENUM_ASPECT_RATIO GetAspectRatio()=0;

    virtual void SetAspectRatio(ENUM_ASPECT_RATIO aspect_ratio)=0;

    virtual ENUM_RESCALE_MODE GetRescaleMode()=0;

    virtual void SetRescaleMode(ENUM_RESCALE_MODE rescale_mode)=0;

    virtual int GetBrightness()=0;

    virtual void SetBrightness(int brightness)=0;

    virtual int GetContrast()=0;

    virtual void SetContrast(int contrast)=0;

    virtual int GetHue()=0;

    virtual void SetHue(int hue)=0;

    virtual int GetSaturation()=0;

    virtual void SetSaturation(int saturation)=0;

    virtual ENUM_VIDEO_OUTPUT GetVideoOutput()=0;

    virtual void SetVideoOutput(ENUM_VIDEO_OUTPUT video_output)=0;

    virtual ENUM_ON_OFF GetVideoDNRMode()=0;

    virtual void SetVideoDNRMode(ENUM_ON_OFF dnr_mode)=0;

    virtual ENUM_ON_OFF GetVideoZoomOutMode()=0;

    virtual void SetVideoZoomOutMode(ENUM_ON_OFF zoom_out_mode)=0;

#ifdef USE_HDMI
    virtual int GetCurrentHdmiTVVid()=0;

    virtual void SetCurrentHdmiTVVid(int CurrentHdmiTVVid)=0;
#endif /* USE_HDMI */

    virtual ENUM_ON_OFF GetAngleMark()=0;

    virtual void SetAngleMark(ENUM_ON_OFF angle_mark)=0;

    virtual ENUM_REGION_SETTING GetRegionCode()=0;

    virtual void SetRegionCode(ENUM_REGION_SETTING region)=0;

    virtual ENUM_SECURITY_SETTING GetSecuritySetting()=0;

    virtual void SetSecuritySetting(ENUM_SECURITY_SETTING security)=0;

    virtual bool GetInitialFlag()=0;

    virtual void SetInitialFlag(bool initial)=0;

    virtual ENUM_SPEAKER_OUT GetSpeakerOut()=0;

    virtual void SetSpeakerOut(ENUM_SPEAKER_OUT speaker_out)=0;

    virtual ENUM_AUDIO_DIGITAL_OUTPUT_MODE GetHdmiAudioOutputMode()=0;

    virtual void SetHdmiAudioOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE hdmi_output_mode)=0;

    virtual ENUM_AUDIO_DIGITAL_OUTPUT_MODE GetSpdifAudioOutputMode()=0;

    virtual void SetSpdifAudioOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE spdif_output_mode)=0;

	virtual int  GetAudioSpdifDelay()=0;
	virtual void SetAudioSpdifDelay(int spdifDelay)=0;

    virtual ENUM_SPDIF_MODE GetSpdifMode()=0;

    virtual void SetSpdifMode(ENUM_SPDIF_MODE spdif_mode)=0;

    virtual bool GetDolbyPrologicEnable()=0;

    virtual void SetDolbyPrologicEnable(bool enable)=0;

    virtual ENUM_AUDIO_AGC_MODE GetAudioAGCMode()=0;

    virtual void SetAudioAGCMode(ENUM_AUDIO_AGC_MODE agc_mode)=0;

    virtual ENUM_AUDIO_OUTPUT_CHANNEL GetAudioOutputChannel()=0;

    virtual void SetAudioOutputChannel(ENUM_AUDIO_OUTPUT_CHANNEL mode)=0;

	virtual int GetAudioDrcCutValue()=0;

	virtual void SetAudioDrvCutValue(int value)=0;

	virtual int GetAudioDrcBoostValue()=0;

	virtual void SetAudioDrvBoostValue(int value)=0;

    virtual ENUM_ON_OFF GetAudioLossLessMode()=0;

    virtual void SetAudioLossLessMode(ENUM_ON_OFF mode)=0;

    virtual void SetSortingModeHDD (SortMode mode)=0;

    virtual SortMode GetSortingModeHDD ()=0;

    virtual void SetSortingModeDISC (SortMode mode)=0;

    virtual SortMode GetSortingModeDISC ()=0;

    virtual void SetKaraokeMode (ENUM_ON_OFF mode)=0;

    virtual ENUM_ON_OFF GetKaraokeMode ()=0;

    virtual int GetMic1Volume()=0;

    virtual void SetMic1Volume(int volume)=0;

    virtual int  GetMic2Volume()=0;

    virtual void SetMic2Volume(int volume)=0;

    virtual int GetMusicVolume()=0;

    virtual void SetMusicVolume(int volume)=0;

    virtual int GetMicEcho()=0;

    virtual void SetMicEcho(int echo)=0;

    virtual VOCAL_REMOVAL_MODE GetVocalRemovalMode()=0;

    virtual void SetVocalRemovalMode(VOCAL_REMOVAL_MODE mode)=0;

    virtual ENUM_KEY_SHIFT_STONE GetKeyShiftStone()=0;

    virtual void SetKeyShiftStone(ENUM_KEY_SHIFT_STONE stone)=0;

    virtual ENUM_EQUALIZER_MODE GetEqualizerMode()=0;

    virtual void SetEqualizerMode(ENUM_EQUALIZER_MODE mode)=0;

    virtual KARAOKE_REVERB_MODE GetReverbMode()=0;

    virtual void SetReverbMode(KARAOKE_REVERB_MODE mode)=0;

    virtual ENUM_ON_OFF GetPassthroughMode()=0;

    virtual void SetPassthroughMode(ENUM_ON_OFF mode)=0;

    virtual SourceOption GetSource()=0;

    virtual void SetSource(SourceOption source)=0;

    virtual DeviceOption GetDevice()=0;

    virtual void SetDevice(DeviceOption device)=0;

    virtual int GetVolume()=0;

    virtual void SetVolume(int volume)=0;

    virtual int GetCurrChannel()=0;

    virtual void SetCurrChannel(int channel)=0;

    virtual int GetCurDtvChannelIndex()=0;

    virtual void SetCurDtvChannelIndex(int index)=0;

    virtual int GetDtvChannelIndex()=0;

    virtual void SetDtvChannelIndex(int index)=0;

    virtual int GetRadioChannelIndex()=0;

    virtual void SetRadioChannelIndex(int index)=0;

    virtual DTV_REGION GetDtvRegion()=0;

    virtual void SetDtvRegion(DTV_REGION region)=0;

    virtual DTV_CHANNEL_ALTER_MODE GetDtvChannelAlterMode()=0;

    virtual void SetDtvChannelAlterMode(DTV_CHANNEL_ALTER_MODE mode)=0;

    /* follow APIs added by <wucp@realtek.com.tw>, 2005-09-20 */
    virtual RESUME_SETTING GetResumeMode()=0;

    virtual void SetResumeMode(RESUME_SETTING mode)=0;

    virtual unsigned int GetSignature(void **signature)=0;

    virtual void SetSignature(void *signature, unsigned int size) =0;

    virtual unsigned int GetBookmark(void **state)=0;

    virtual void SetBookmark(void *state, unsigned int size)=0;

// network start
    virtual ENUM_NET_INTERFACE GetNetDefaultRoute(void)=0;

    virtual void SetNetDefaultRoute(ENUM_NET_INTERFACE route)=0;

    virtual bool GetNetEth0Enable(void)=0;

    virtual void SetNetEth0Enable(bool bValue)=0;

    virtual bool GetNetEth0DHCPEnable(void)=0;

    virtual void SetNetEth0DHCPEnable(bool bValue)=0;

    virtual IPV4* GetNetEth0IPAddr(void)=0;

    virtual void SetNetEth0IPAddr(IPV4 *pipvValue)=0;

    virtual IPV4* GetNetEth0Mask(void)=0;

    virtual void SetNetEth0Mask(IPV4 *pipvValue)=0;

    virtual IPV4* GetNetEth0Gateway(void)=0;

    virtual void SetNetEth0Gateway(IPV4 *pipvValue)=0;

    virtual ENUM_NET_MODE GetNetMode(void)=0;

    virtual void SetNetMode(ENUM_NET_MODE mode)=0;

    virtual ENUM_NET_MODE GetNetUiMode(void)=0;

    virtual void SetNetUiMode(ENUM_NET_MODE mode)=0;

#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
    virtual ENUM_P2P_CONCURRENT_STATE GetP2pConcurrentState(void)=0;
    virtual void SetP2pConcurrentState(ENUM_P2P_CONCURRENT_STATE state)=0;

	// 2013/03/20 add:
	// If SET_ON -> p2p Concurrent auto switch for P2P/DLNA,
	//      It don't need to reference to GetP2pConcurrentState(), always STATE_P2P_CONCURRENT
	// If SET_OFF -> p2p Concurrent only in STATE_P2P_CONCURRENT,
	//      But if any connect and playback start, need to stop other side of connection
	//      to keep best performance of playback side.
	virtual ENUM_ON_OFF GetP2pConcurrentSwitch(void)=0;
	virtual void SetP2pConcurrentSwitch(ENUM_ON_OFF state)=0;
#endif

    virtual IPV4* GetNetDNS(void)=0;

    virtual void SetNetDNS(IPV4 *pipvValue)=0;

    virtual bool GetEnableDHCPManualDNS(void)=0;

    virtual void SetEnableDHCPManualDNS(bool bValue)=0;

    virtual bool GetNetWlan0Enable(void)=0;

    virtual void SetNetWlan0Enable(bool bValue)=0;

    virtual int GetNetWlan0Profile()=0;

    virtual void SetNetWlan0Profile(int iValue)=0;

    virtual int GetNetWlan0ProfilePreSetting()=0;

    virtual void SetNetWlan0ProfilePreSetting(int iValue)=0;

    virtual char* GetNetWlan0ProfileName(void)=0;

    virtual char* GetNetWlan0ProfileName(int index)=0;

    virtual void SetNetWlan0ProfileName(char *ProfileName,int index)=0;

    virtual ENUM_WIRELESS_MODE GetNetWlan0Mode()=0;

    virtual ENUM_WIRELESS_MODE GetNetWlan0Mode(int index)=0;

    virtual void SetNetWlan0Mode(ENUM_WIRELESS_MODE wlan0_mode,int index)=0;

    virtual bool GetNetWlan0DHCPEnable(void)=0;

    virtual bool GetNetWlan0DHCPEnable(int index)=0;

    virtual void SetNetWlan0DHCPEnable(bool bValue,int index)=0;

    virtual void SetNetWlan0DHCPEnable(bool bValue)=0;

    virtual IPV4* GetNetWlan0IPAddr(void)=0;

    virtual IPV4* GetNetWlan0IPAddr(int index)=0;

    virtual void SetNetWlan0IPAddr(IPV4 *pipvValue,int index)=0;

    virtual IPV4* GetNetWlan0Mask(void)=0;

    virtual IPV4* GetNetWlan0Mask(int index)=0;

    virtual void SetNetWlan0Mask(IPV4 *pipvValue,int index)=0;

    virtual IPV4* GetNetWlan0Gateway(void)=0;

    virtual IPV4* GetNetWlan0Gateway(int index)=0;

    virtual void SetNetWlan0Gateway(IPV4 *pipvValue,int index)=0;

    virtual IPV4* GetNetWlan0DNS(void)=0;

    virtual IPV4* GetNetWlan0DNS(int index)=0;

    virtual void SetNetWlan0DNS(IPV4 *pipvValue,int index)=0;

    virtual char* GetNetWlan0ApName(void)=0;
    virtual char* GetNetWlan0ApName(int index)=0;
    virtual void SetNetWlan0ApName(char *ApName,int index)=0;

    virtual ENUM_WIRELESS_SECURITY GetNetWlan0Security()=0;

    virtual ENUM_WIRELESS_SECURITY GetNetWlan0Security(int index)=0;

    virtual void SetNetWlan0Security(ENUM_WIRELESS_SECURITY wlan0_security,int index)=0;

    virtual char* GetNetWlan0WepKey(void)=0;
    virtual char* GetNetWlan0WepKey(int index)=0;
    virtual void SetNetWlan0WepKey(char *WepKey,int index)=0;

    virtual int GetNetWlan0ActiveWepKey(void)=0;
    virtual int GetNetWlan0ActiveWepKey(int index)=0;
    virtual void SetNetWlan0ActiveWepKey(int activeWepKey,int index)=0;

    virtual IPV4* GetNetWlan0DhcpdHostIP(void)=0;

    virtual IPV4* GetNetWlan0DhcpdHostIP(int index)=0;

    virtual void SetNetWlan0DhcpdHostIP(IPV4 *pipvValue,int index)=0;

    virtual IPV4* GetNetWlan0DhcpdStartIP(void)=0;

    virtual IPV4* GetNetWlan0DhcpdStartIP(int index)=0;

    virtual void SetNetWlan0DhcpdStartIP(IPV4 *pipvValue,int index)=0;

    virtual IPV4* GetNetWlan0DhcpdEndIP(void)=0;

    virtual IPV4* GetNetWlan0DhcpdEndIP(int index)=0;

    virtual void SetNetWlan0DhcpdEndIP(IPV4 *pipvValue,int index)=0;
#ifdef ENABLE_WIFI_SOFT_AP
#ifdef ENABLE_WIFI_BRIDGE_MODE
	virtual void SetBrgTargetIface(EMUN_BRIDGE_INTERFACE mode)=0;

	virtual EMUN_BRIDGE_INTERFACE GetBrgTargetIface()=0;
#endif //ENABLE_WIFI_BRIDGE_MODE

#endif //ENABLE_WIFI_SOFT_AP
    virtual bool GetNetPPP0Enable(void)=0;

    virtual void SetNetPPP0Enable(bool bValue)=0;

    virtual char* GetNetPPP0UserName(void)=0;

    virtual void SetNetPPP0UserName(char *UserName)=0;

    virtual char* GetNetPPP0Password(void)=0;

    virtual void SetNetPPP0Password(char *Password)=0;

    virtual ENUM_NET_INTERFACE GetNetPPP0Interface(void)=0;

    virtual void SetNetPPP0Interface(ENUM_NET_INTERFACE interface)=0;

#ifdef ENABLE_WIFI_SOFT_AP
#if 0
	virtual char* GetSoftwareAPProfileName(void)=0;
	virtual void SetSoftwareAPProfileName(char *ApName)=0;
#endif
	virtual ENUM_SOFT_AP_MODE GetSoftwareAPMode()=0;
	virtual void SetSoftwareAPMode(ENUM_SOFT_AP_MODE wlan0_mode)=0;

	virtual char* GetSoftwareAPName(void)=0;
	virtual void SetSoftwareAPName(char *ApName)=0;

	virtual ENUM_WIRELESS_SECURITY GetSoftwareAPSecurity()=0;
	virtual void SetSoftwareAPSecurity(ENUM_WIRELESS_SECURITY wlan0_security)=0;

	virtual char* GetSoftwareAPWepKey(void)=0;
	virtual void SetSoftwareAPWepKey(char *WepKey)=0;

	virtual ENUM_NET_REGION GetSoftwareAPRegion()=0;
	virtual void SetSoftwareAPRegion(ENUM_NET_REGION net_region)=0;

//	virtual void SetConcurrentMode(bool on)=0;
//	virtual bool GetConcurrentMode()=0;
#endif
	virtual void SetWiFiFreq(ENUM_WIFI_FREQ value)=0;
	virtual ENUM_WIFI_FREQ GetWiFiFreq()=0;

#ifdef VCRPLUS
    virtual void SetVcrPlusChMapping(int gcn_ch, SourceOption r_source, int r_ch)=0;

    virtual int GetVcrPlusChMapping(int gcn_ch)=0;

    virtual SourceOption GetVcrPlusSource(int gcn_ch)=0;
#endif

/******************************************/
#if AP_CONFIG_WITH(APCFG_APP_BITTORRENT)
    virtual bool GetImsDaemonState(void)=0;

    virtual void SetImsDaemonState(bool state)=0;

    virtual int GetImsHomeDir(void)=0;

    virtual void SetImsHomeDir(int choice)=0;

    virtual int GetImsDownloadDir(void)=0;

    virtual void SetImsDownloadDir(int choice)=0;
#endif

#if defined(CASABLANCA)
    /* Get the setting of Screen Saver. */
    virtual SCREEN_SAVER_TIMING GetScreenSaverTiming()=0;
    virtual void SetScreenSaverTiming(SCREEN_SAVER_TIMING screen_saver_timing)=0;
    virtual SCREEN_SAVER_OPTION GetScreenSaverOption()=0;
    virtual void SetScreenSaverOption(SCREEN_SAVER_OPTION screen_saver_option)=0;
    virtual int GetFolderPartitionId()=0;
    virtual void SetFolderPartitionId(int id)=0;
    virtual int GetFolderFieldId()=0;
    virtual void SetFolderFieldId(int id)=0;
    virtual char* GetFolderDataPath()=0;
    virtual void SetFolderDataPath(char* path)=0;
#else
    /* Get the setting of Screen Saver. The Setting is ON or OFF */
    virtual ENUM_ON_OFF GetScreenSaver()=0;
    virtual void SetScreenSaver(ENUM_ON_OFF screen_saver)=0;
#endif

    /*Get Set Ken Burns 2009/6/17 kelly*/
    virtual ENUM_ON_OFF GetKenBurns()=0;
    virtual void SetKenBurns(ENUM_ON_OFF ken_burns)=0;

    /* Get the setting of Movie Preview in GBrowser Menu. The Setting is ON or OFF */
    virtual ENUM_ON_OFF GetMoviePreview() = 0;

    virtual void SetMoviePreview(ENUM_ON_OFF movie_preview) = 0;

    /* API added by wucp@realtek.com, for DivX 1080p v1.1 */
    virtual bool GetDivXCode(char **code)=0;

    virtual void SetDivXCode(char *code) =0;

    virtual bool GetDisableExtHddDB(void)=0;

    virtual void SetDisableExtHddDB(bool disable) =0;

    virtual bool GetDisableAutoScanExtHdd(void)=0;

    virtual void SetDisableAutoScanExtHdd(bool disable) =0;

    virtual char *GetSkinName()=0;

    virtual void SetSkinName(char *skinName)=0;

#ifdef USE_HDMI
    virtual int GetHdmiDeepColor(void)=0;

    virtual void SetHdmiDeepColor(int value)=0;
#endif

#ifdef TVE_DEC_SWITCH
    virtual int GetTveSwitch(void)=0;

    virtual void SetTveSwitch(int value)=0;
#endif

#ifdef ENABLE_HDMV_PLAYBACK
    virtual ENUM_ON_OFF GetHdmvNavigation(void)=0;

    virtual void SetHdmvNavigation(ENUM_ON_OFF hdmv_navigation)=0;
#endif

    virtual ENUM_ON_OFF GetBtProcess(void)=0;

    virtual void SetBtProcess(ENUM_ON_OFF bt_process)=0;

    virtual ENUM_ON_OFF GetSambaProcess(void)=0;

    virtual void SetSambaProcess(ENUM_ON_OFF samba_process)=0;

    virtual ENUM_ON_OFF GetNeighborSecurity(void)=0;

    virtual void SetNeighborSecurity(ENUM_ON_OFF security)=0;

    virtual KEY_TONE_VOLUME GetKeyToneVolume(void)=0;

    virtual void SetKeyToneVolume(KEY_TONE_VOLUME key_tone_volume)=0;

#ifdef USE_CEC
    virtual bool GetEnableHdmiCec(void)=0;

    virtual void SetEnableHdmiCec(bool value)=0;

    virtual bool GetEnableHdmiCecOneTouchPlay()=0;

    virtual void SetEnableHdmiCecOneTouchPlay(bool one_touch_play)=0;

    virtual bool GetEnableHdmiCecSystemStandby()=0;

    virtual void SetEnableHdmiCecSystemStandby(bool system_standby)=0;

    virtual bool GetEnableHdmiCecAutoOsdLanguage()=0;

    virtual void SetEnableHdmiCecAutoOsdLanguage(bool auto_osd_language)=0;

    virtual  bool GetEnableHdmiCecAutoPowerOnFromTv()=0;

    virtual  void SetEnableHdmiCecAutoPowerOnFromTv(bool auto_power_from_tv)=0;

    virtual  int GetCECPreferredLogicalAddress()=0;

    virtual  void SetCECPreferredLogicalAddress(int logicAddress)=0;

    virtual bool GetSystemAudioControl()=0;

    virtual void SetSystemAudioControl(bool SystemAudioControl)=0;

#endif
#ifdef USE_HDMI
    virtual void SetHdmiLipSync(ENUM_LIP_SYNC_SETTING type, int value) = 0;

    virtual ENUM_LIP_SYNC_SETTING GetHdmiLipSync(int *value) = 0;
#endif
    virtual ENUM_ON_OFF GetPhotoActualSize(void)=0;
    virtual void SetPhotoActualSize(ENUM_ON_OFF actualSize)=0;

#ifdef HW_VGA
    virtual ENUM_VIDEO_VGA_RESOLUTION GetVgaResolution(void)=0;
    virtual void SetVgaResolution(ENUM_VIDEO_VGA_RESOLUTION res)=0;

	virtual ENUM_VIDEO_SYSTEM GetRealTvSystem()=0;
	virtual void SetRealTvSystem(ENUM_VIDEO_SYSTEM video_system)=0;
#endif
    virtual const char* GetDeviceName()=0;
    virtual void  SetDeviceName(const char *device_name)=0;

#ifdef ANDROID
    virtual const char *getProperty(const char *prop, const char *def) {return def;};
    virtual bool setProperty(const char *prop, const char *value) {return false;};
    virtual const char* wifiCmd(WIFI_CMD_E wifiCmd, ...) {return NULL;};
#endif

    virtual ENUM_TV_SYSTEM_USER GetUserTvSystem()=0;
    virtual void SetUserTvSystem(ENUM_TV_SYSTEM_USER tv_system_user)=0;

protected:
    /* Generate a uuid for unique device name, the returned char* should be freed later 2009/12/23 yuyu */
    static char *UUIDGenerate();

};

#endif

