#ifndef __SETUPCLASS_VOLATILE_SQLITE_
#define __SETUPCLASS_VOLATILE_SQLITE_

#include <Application/AppClass/SetupClass.h>


class SetupClassVolatile : public SetupClass{
private:

    pthread_mutex_t m_active_mutex;
    pthread_mutex_t m_device_mutex;

    ENUM_VIDEO_SYSTEM mVideoSystem;
    ENUM_VIDEO_STANDARD mVideoStandard; // progressive or interlaced

    ENUM_SUBTITLE_LANGUAGE mSubtitleLang;
    ENUM_SUBTITLE_TEXT_COLOR mSubtitleTextColor;
    ENUM_TEXT_ENCODING mSubtitleTextEncoding;
    int mSubtitleXOffset;
    int mSubtitleYOffset;
    int mSubtitleSize;
    ENUM_ON_OFF mSubtitleEnabled;

    ENUM_AUDIO_LANGUAGE mAudioLang;

    ENUM_OSD_LANGUAGE mOSDLang;
    ENUM_MENU_LANGUAGE mMenuLang;
    ENUM_MINIKBD_LANGUAGE mMinikbdLang;

    int mTVE;
    ENUM_VIDEO_OUTPUT mVideoOutputFormat;
    ENUM_ASPECT_RATIO mAspectRatio;
    int mHdmiTVVid;
    ENUM_TV_SYSTEM_AUTO_MODE mTvSystemAutoMode;
    ENUM_ON_OFF mVideoZoomOutMode;
#ifdef HW_VGA
    ENUM_VIDEO_VGA_RESOLUTION mVideoVGAResolution;
#endif
    char mDeviceName[MAX_SIZE_DEVICE_NAME];
    ENUM_TV_SYSTEM_USER mUserTVSystem;

public:
    SetupClassVolatile();
    virtual ~SetupClassVolatile();
    virtual void Init();
    virtual void Create();
    virtual HRESULT Sync();
    virtual HRESULT AutoSync();
#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual void RestoreTVChannelDefault();
#endif
    virtual void RestoreAudioDefault();
    virtual void RestoreFactoryDefault();
    virtual int SetNetWlan0ProfileCopy(int DestProfile,int SrcProfile);
    virtual int SetNetWlan0ProfileToDefault(int profile);

    virtual ENUM_OSD_LANGUAGE GetOsdLanguage();

    virtual void SetOsdLanguage(ENUM_OSD_LANGUAGE osd_lang);

    virtual ENUM_AUDIO_LANGUAGE GetAudioLanguage();

    virtual void SetAudioLanguage(ENUM_AUDIO_LANGUAGE audio_lang);

    virtual ENUM_SUBTITLE_LANGUAGE GetSubtitleLanguage();

    virtual void SetSubtitleLanguage(ENUM_SUBTITLE_LANGUAGE subtitle_lang);

    virtual ENUM_MENU_LANGUAGE GetMenuLanguage();

    virtual void SetMenuLanguage(ENUM_MENU_LANGUAGE menu_lang);

    virtual ENUM_TEXT_ENCODING GetTextSubtitleEncoding();

    virtual void SetTextSubtitleEncoding(ENUM_TEXT_ENCODING coding);

    virtual ENUM_SUBTITLE_TEXT_COLOR GetTextSubtitleColor();

    virtual void SetTextSubtitleColor(ENUM_SUBTITLE_TEXT_COLOR color);

    virtual int GetTextSubtitleYOffset();

    virtual void SetTextSubtitleYOffset(int offset);

    virtual int GetTextSubtitleSize();

    virtual void SetTextSubtitleSize(int size);

    virtual int GetTextSubtitleOffset();

    virtual void SetTextSubtitleOffset(int offset);

    virtual ENUM_VIDEO_SYSTEM GetTvSystem();

    virtual void SetTvSystem(ENUM_VIDEO_SYSTEM video_system);

    virtual ENUM_VIDEO_STANDARD GetTvStandard();

    virtual void SetTvStandard(ENUM_VIDEO_STANDARD video_standard);

    virtual VIDEO_SYSTEM_INFO GetVideoSystem();

    virtual void SetVideoSystem(VIDEO_SYSTEM_INFO video_system_info);

    virtual VIDEO_SYSTEM_INFO GetVideoSystemManual();

    virtual void SetVideoSystemManual(VIDEO_SYSTEM_INFO video_system_info);

    virtual ENUM_VIDEO_SYSTEM GetVideoSDSystem();

    virtual void SetVideoSDSystem(ENUM_VIDEO_SYSTEM video_sd_system);

    virtual ENUM_VIDEO_HD_SYSTEM GetVideoHDSystem();

    virtual void SetVideoHDSystem(ENUM_VIDEO_HD_SYSTEM hd_system);

    virtual ENUM_ON_OFF GetVideoSystem1080P24HZ();

    virtual void SetVideoSystem1080P24HZ(ENUM_ON_OFF system_1080p24hz);

    virtual int GetErrorConcealmentLevel();

    virtual void SetErrorConcealmentLevel(int level);

	virtual int GetErrorNotificationLevel();

	virtual void SetErrorNotificationLevel(int level);

    virtual ENUM_TV_SYSTEM_AUTO_MODE GetTvSystemAutoMode();

    virtual void SetTvSystemAutoMode(ENUM_TV_SYSTEM_AUTO_MODE mode);

    virtual ENUM_VIDEO_REC_SYSTEM GetRecTvSystem();

    virtual void SetRecTvSystem(ENUM_VIDEO_REC_SYSTEM video_rec_system);

    virtual ENUM_ON_OFF GetAutoTvSystem();

    virtual void SetAutoTvSystem(ENUM_ON_OFF auto_video_system);

    virtual ENUM_ON_OFF GetVideoPreprocessing();

    virtual void SetVideoPreprocessing(ENUM_ON_OFF preprocessing);

    virtual ENUM_ON_OFF GetCopyProtection();

    virtual void SetCopyProtection(ENUM_ON_OFF aware_copy_protection);

#if AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual ENUM_TV_INPUT_DTV GetTvInputDtv();

    virtual void SetTvInputDtv(ENUM_TV_INPUT_DTV tv_input);
#endif

#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
    virtual unsigned int GetTvSystemTypeByRegion(ENUM_TV_REGION region);

    virtual bool IsAtvSystemTypeMN() { return m_isAtvMN; }

    virtual void SetAtvSystemTypeMN(bool isMN) { m_isAtvMN = isMN; }

    virtual ENUM_TV_INPUT GetTvInput();

    virtual void SetTvInput(ENUM_TV_INPUT tv_input);

    virtual ENUM_TV_REGION GetTvRegion();

    virtual void SetTvRegion(ENUM_TV_REGION tv_region);

    virtual ENUM_ON_OFF GetAntennaPower();
    virtual void SetAntennaPower(ENUM_ON_OFF antenna_power);

    virtual void SetChannelStatus(int channel, unsigned char status);

    virtual unsigned char GetChannelStatus(int channel);

    virtual void SetChannelStandard(int channel, unsigned char standard);

    virtual unsigned char GetChannelStandard(int channel);

    virtual void SetChannelCNIStation(int channel, CNI_STATION cni_station);

    virtual CNI_STATION GetChannelCNIStation(int channel);

    virtual void SetChannelFrequency(int channel, unsigned int freq);

    virtual unsigned int GetChannelFrequency(int channel);

    virtual void SetChannelName(int channel, char *name);

    virtual char * GetChannelName(int channel);
    virtual void SetChannelPayTV(int channel, unsigned char pay_tv);

    virtual unsigned char GetChannelPayTV(int channel);
#endif

    virtual ENUM_REC_QUALITY GetRecQuality();

    virtual void SetRecQuality(ENUM_REC_QUALITY quality);

    virtual ENUM_AUTO_CHAPTER GetAutoChapter();

    virtual void SetAutoChapter(ENUM_AUTO_CHAPTER auto_chapter);


    virtual ENUM_REC_DEVICE GetRecDevice();

    virtual void SetRecDevice(ENUM_REC_DEVICE rec_device);

    virtual ENUM_REC_DEVICE GetTimeShiftDevice();

    virtual void SetTimeShiftDevice(ENUM_REC_DEVICE timeshift_device);

    virtual ENUM_REC_PRIORITY GetRecPriority();

    virtual void SetRecPriority(ENUM_REC_PRIORITY rec_priority);

    virtual char * GetLastRecFile();

    virtual void SetLastRecFile(char *file);

    virtual ENUM_ON_OFF GetAutoTimeshift();

    virtual void SetAutoTimeshift(ENUM_ON_OFF auto_timeshift);

    virtual ENUM_SLIDE_SHOW_TIME GetSlideShowTime();

    virtual void SetSlideShowTime(ENUM_SLIDE_SHOW_TIME slide_show_time) ;

    virtual ENUM_TRANS_EFFECT GetTeansEffect() ;

    virtual void SetTeansEffect(ENUM_TRANS_EFFECT trans_effect) ;

    virtual ENUM_MIX_PLAY GetMixPlay() ;

    virtual void SetMixPlay(ENUM_MIX_PLAY mix_play) ;
// new
    virtual ENUM_TIME_ZONE GetTimeZone();

    virtual void SetTimeZone(ENUM_TIME_ZONE time_zone);

    virtual ENUM_ON_OFF GetDayLightSaving();

    virtual void SetDayLightSaving(ENUM_ON_OFF day_light_saving);

    virtual ENUM_ON_OFF GetPowerSaving();

    virtual void SetPowerSaving(ENUM_ON_OFF power_saving);
//
    virtual int GetRatingIsBlockUnrated();

    virtual void SetRatingIsBlockUnrated(int isBlockUnrated);

    virtual int GetRatingIsEnable();

    virtual void SetRatingIsEnable(int isEnable);

    virtual ENUM_RATING_LEVEL GetRatingLevel();

    virtual void SetRatingLevel(ENUM_RATING_LEVEL rating_level);

    virtual ENUM_RATING_REGION GetRatingRegion();

    virtual void SetRatingRegion(ENUM_RATING_REGION rating_region);

    virtual ENUM_DTV_AUDIO_LANG GetDtvAudioLang();

    virtual void SetDtvAudioLang(ENUM_DTV_AUDIO_LANG dtvAudioLang);

	virtual ENUM_DTV_SUBTITLE_LANG GetDtvSubtitleLang1();

    virtual void SetDtvSubtitleLang1(ENUM_DTV_SUBTITLE_LANG dtvSubtitleLang);

	virtual ENUM_DTV_SUBTITLE_LANG GetDtvSubtitleLang2();

    virtual void SetDtvSubtitleLang2(ENUM_DTV_SUBTITLE_LANG dtvSubtitleLang);

	virtual ENUM_ON_OFF GetDtvSubtitleHardOfHearing();

    virtual void SetDtvSubtitleHardOfHearing(ENUM_ON_OFF dtvSubtitleHardOfHearing);

	virtual ENUM_ON_OFF GetDtvDisableLCNs();

    virtual void SetDtvDisableLCNs(ENUM_ON_OFF dtvDisableLCNs);

    virtual unsigned char GetRatingType(ENUM_RATING_LEVEL rating_level);

    virtual void SetRatingType(ENUM_RATING_LEVEL rating_level, unsigned char rating_type);

    virtual OPENVCHIP_TABLE_INFO* GetOpenVChipTable();

    virtual OPEN_VCHIP_RATING_INFO* GetOpenVChipRatingInfo();

    virtual void SetOpenVChip();

    virtual int GetRatingTimeout();

    virtual void SetRatingTimeout(int milliSeconds);

    virtual int GetRatingPasswd();

    virtual void SetRatingPasswd(int passwd);

    virtual bool IsRatingPasswdCorrect(int passwd);

    virtual int GetTVRatingBitwiseValue();

    virtual void SetTVRatingBitwiseValue(int tv_rating_level);

    virtual int GetTVxRatingBitwiseValue();

    virtual void SetTVxRatingBitwiseValue(int tv_x_rating_level);

    virtual bool GetAutoPlay();

    virtual void SetAutoPlay(bool bAutoPlay);

    virtual ENUM_ON_OFF GetSeamlessPlayback();

    virtual void SetSeamlessPlayback(ENUM_ON_OFF seamless_playback);

#if AP_CONFIG_WITH(APCFG_APP_UPNPDLNA1p5)
    virtual ENUM_ON_OFF GetDlnaDmr();

    virtual void SetDlnaDmr(ENUM_ON_OFF set);

	virtual ENUM_ON_OFF GetDlnaDms();

	virtual void SetDlnaDms(ENUM_ON_OFF set);

	virtual char *GetDlnaDmsDeviceName();

	virtual void SetDlnaDmsDeviceName(char *deviceName);

	virtual char *GetDlnaDmsDeviceUUID();

	virtual void SetDlnaDmsDeviceUUID(char *deviceUUID);
#endif /* APCFG_APP_UPNP_1p5 */

    virtual char *GetSystemDeviceFriendlyName();

    virtual void SetSystemDeviceFriendlyName(char *friendlyName);

    virtual char *GetSystemDeviceUUID();

    virtual void SetSystemDeviceUUID(char *uuid);

    virtual ENUM_ASPECT_RATIO GetAspectRatio();

    virtual void SetAspectRatio(ENUM_ASPECT_RATIO aspect_ratio);

    virtual ENUM_RESCALE_MODE GetRescaleMode();

    virtual void SetRescaleMode(ENUM_RESCALE_MODE rescale_mode);

	virtual int  GetAudioSpdifDelay();
	virtual void SetAudioSpdifDelay(int spdifDelay);

    virtual int GetBrightness();

    virtual void SetBrightness(int brightness);

    virtual int GetContrast();

    virtual void SetContrast(int contrast);

    virtual int GetHue();

    virtual void SetHue(int hue);

    virtual int GetSaturation();

    virtual void SetSaturation(int saturation);

    virtual ENUM_VIDEO_OUTPUT GetVideoOutput();

    virtual void SetVideoOutput(ENUM_VIDEO_OUTPUT video_output);

    virtual ENUM_ON_OFF GetVideoDNRMode();

    virtual void SetVideoDNRMode(ENUM_ON_OFF dnr_mode);

    virtual ENUM_ON_OFF GetVideoZoomOutMode();

    virtual void SetVideoZoomOutMode(ENUM_ON_OFF zoom_out_mode);
#ifdef USE_HDMI
    virtual int GetCurrentHdmiTVVid();

    virtual void SetCurrentHdmiTVVid(int CurrentHdmiTVVid);
#endif /* USE_HDMI */

    virtual ENUM_ON_OFF GetAngleMark();

    virtual void SetAngleMark(ENUM_ON_OFF angle_mark);

    virtual ENUM_REGION_SETTING GetRegionCode();

    virtual void SetRegionCode(ENUM_REGION_SETTING region);

    virtual ENUM_SECURITY_SETTING GetSecuritySetting();

    virtual void SetSecuritySetting(ENUM_SECURITY_SETTING security);

    virtual bool GetInitialFlag();

    virtual void SetInitialFlag(bool initial);

    virtual ENUM_SPEAKER_OUT GetSpeakerOut();

    virtual void SetSpeakerOut(ENUM_SPEAKER_OUT speaker_out);

    virtual ENUM_AUDIO_DIGITAL_OUTPUT_MODE GetHdmiAudioOutputMode();

    virtual void SetHdmiAudioOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE hdmi_output_mode);

    virtual ENUM_AUDIO_DIGITAL_OUTPUT_MODE GetSpdifAudioOutputMode();

    virtual void SetSpdifAudioOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE spdif_output_mode);

    virtual ENUM_SPDIF_MODE GetSpdifMode();

    virtual void SetSpdifMode(ENUM_SPDIF_MODE spdif_mode);

    virtual bool GetDolbyPrologicEnable();

    virtual void SetDolbyPrologicEnable(bool enable);

    virtual ENUM_AUDIO_AGC_MODE GetAudioAGCMode();

    virtual void SetAudioAGCMode(ENUM_AUDIO_AGC_MODE agc_mode);

    virtual ENUM_AUDIO_OUTPUT_CHANNEL GetAudioOutputChannel();

    virtual void SetAudioOutputChannel(ENUM_AUDIO_OUTPUT_CHANNEL channel);

	virtual int GetAudioDrcCutValue();

	virtual void SetAudioDrvCutValue(int value);

	virtual int GetAudioDrcBoostValue();

	virtual void SetAudioDrvBoostValue(int value);

    virtual ENUM_ON_OFF GetAudioLossLessMode();

    virtual void SetAudioLossLessMode(ENUM_ON_OFF mode);

    virtual void SetSortingModeHDD (SortMode mode);

    virtual SortMode GetSortingModeHDD ();

    virtual void SetSortingModeDISC (SortMode mode);

    virtual SortMode GetSortingModeDISC ();

    virtual void SetKaraokeMode (ENUM_ON_OFF mode);

    virtual ENUM_ON_OFF GetKaraokeMode ();

    virtual int GetMic1Volume();

    virtual void SetMic1Volume(int volume);

    virtual int  GetMic2Volume();

    virtual void SetMic2Volume(int volume);

    virtual int GetMusicVolume();

    virtual void SetMusicVolume(int volume);

    virtual int GetMicEcho();

    virtual void SetMicEcho(int echo);

    virtual VOCAL_REMOVAL_MODE GetVocalRemovalMode();

    virtual void SetVocalRemovalMode(VOCAL_REMOVAL_MODE mode);

    virtual ENUM_KEY_SHIFT_STONE GetKeyShiftStone();

    virtual void SetKeyShiftStone(ENUM_KEY_SHIFT_STONE stone);

    virtual ENUM_EQUALIZER_MODE GetEqualizerMode();

    virtual void SetEqualizerMode(ENUM_EQUALIZER_MODE mode);

    virtual KARAOKE_REVERB_MODE GetReverbMode();

    virtual void SetReverbMode(KARAOKE_REVERB_MODE mode);

    virtual ENUM_ON_OFF GetPassthroughMode();

    virtual void SetPassthroughMode(ENUM_ON_OFF mode);

    virtual SourceOption GetSource();

    virtual void SetSource(SourceOption source);

    virtual DeviceOption GetDevice();

    virtual void SetDevice(DeviceOption device);

    virtual int GetVolume();

    virtual void SetVolume(int volume);

    virtual int GetMiniKBDLanguage();

    virtual void SetMiniKBDLanguage(int volume);

    virtual int GetCurrChannel();

    virtual void SetCurrChannel(int channel);

    virtual int GetCurDtvChannelIndex();

    virtual void SetCurDtvChannelIndex(int index);

    virtual int GetDtvChannelIndex();

    virtual void SetDtvChannelIndex(int index);

    virtual int GetRadioChannelIndex();

    virtual void SetRadioChannelIndex(int index);

    virtual DTV_REGION GetDtvRegion();

    virtual void SetDtvRegion(DTV_REGION region);

    virtual DTV_CHANNEL_ALTER_MODE GetDtvChannelAlterMode();

    virtual void SetDtvChannelAlterMode(DTV_CHANNEL_ALTER_MODE mode);

    /* follow APIs added by <wucp@realtek.com.tw>, 2005-09-20 */
    virtual RESUME_SETTING GetResumeMode();

    virtual void SetResumeMode(RESUME_SETTING mode);

    virtual unsigned int GetSignature(void **signature);

    virtual void SetSignature(void *signature, unsigned int size);

    virtual unsigned int GetBookmark(void **state);

    virtual void SetBookmark(void *state, unsigned int size);

    virtual ENUM_NET_INTERFACE GetNetDefaultRoute(void);

    virtual void SetNetDefaultRoute(ENUM_NET_INTERFACE route);

    virtual bool GetNetEth0Enable(void);

    virtual void SetNetEth0Enable(bool bValue);

    virtual bool GetNetEth0DHCPEnable(void);

    virtual void SetNetEth0DHCPEnable(bool bValue);

    virtual IPV4* GetNetEth0IPAddr(void);

    virtual void SetNetEth0IPAddr(IPV4 *pipvValue);

    virtual IPV4* GetNetEth0Mask(void);

    virtual void SetNetEth0Mask(IPV4 *pipvValue);

    virtual IPV4* GetNetEth0Gateway(void);

    virtual void SetNetEth0Gateway(IPV4 *pipvValue);

    virtual ENUM_NET_MODE GetNetMode(void);

    virtual void SetNetMode(ENUM_NET_MODE mode);

    virtual ENUM_NET_MODE GetNetUiMode(void);

    virtual void SetNetUiMode(ENUM_NET_MODE mode);

#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
	virtual ENUM_P2P_CONCURRENT_STATE GetP2pConcurrentState(void);

	virtual void SetP2pConcurrentState(ENUM_P2P_CONCURRENT_STATE state);

	virtual ENUM_ON_OFF GetP2pConcurrentSwitch(void);

	virtual void SetP2pConcurrentSwitch(ENUM_ON_OFF state);
#endif

    virtual IPV4* GetNetDNS(void);

    virtual void SetNetDNS(IPV4 *pipvValue);

    virtual bool GetEnableDHCPManualDNS(void);

    virtual void SetEnableDHCPManualDNS(bool bValue);

    virtual bool GetNetWlan0Enable(void);

    virtual void SetNetWlan0Enable(bool bValue);

    virtual int GetNetWlan0Profile();

    virtual void SetNetWlan0Profile(int iValue);

    virtual int GetNetWlan0ProfilePreSetting();

    virtual void SetNetWlan0ProfilePreSetting(int iValue);

    virtual char* GetNetWlan0ProfileName(void);

    virtual char* GetNetWlan0ProfileName(int index);

    virtual void SetNetWlan0ProfileName(char *ProfileName,int index);

    virtual ENUM_WIRELESS_MODE GetNetWlan0Mode();

    virtual ENUM_WIRELESS_MODE GetNetWlan0Mode(int index);

    virtual void SetNetWlan0Mode(ENUM_WIRELESS_MODE wlan0_mode,int index);

    virtual bool GetNetWlan0DHCPEnable(void);

    virtual bool GetNetWlan0DHCPEnable(int index);

    virtual void SetNetWlan0DHCPEnable(bool bValue,int index);

    virtual void SetNetWlan0DHCPEnable(bool bValue);

    virtual IPV4* GetNetWlan0IPAddr(void);

    virtual IPV4* GetNetWlan0IPAddr(int index);

    virtual void SetNetWlan0IPAddr(IPV4 *pipvValue,int index);

    virtual IPV4* GetNetWlan0Mask(void);

    virtual IPV4* GetNetWlan0Mask(int index);

    virtual void SetNetWlan0Mask(IPV4 *pipvValue,int index);

    virtual IPV4* GetNetWlan0Gateway(void);

    virtual IPV4* GetNetWlan0Gateway(int index);

    virtual void SetNetWlan0Gateway(IPV4 *pipvValue,int index);

    virtual IPV4* GetNetWlan0DNS(void);

    virtual IPV4* GetNetWlan0DNS(int index);

    virtual void SetNetWlan0DNS(IPV4 *pipvValue,int index);

    virtual char* GetNetWlan0ApName(void);
    virtual char* GetNetWlan0ApName(int index);
    virtual void SetNetWlan0ApName(char *ApName,int index);

    virtual ENUM_WIRELESS_SECURITY GetNetWlan0Security();

    virtual ENUM_WIRELESS_SECURITY GetNetWlan0Security(int index);

    virtual void SetNetWlan0Security(ENUM_WIRELESS_SECURITY wlan0_security,int index);

    virtual char* GetNetWlan0WepKey(void);
    virtual char* GetNetWlan0WepKey(int index);
    virtual void SetNetWlan0WepKey(char *WepKey,int index);

    virtual int GetNetWlan0ActiveWepKey(void);
    virtual int GetNetWlan0ActiveWepKey(int index);
    virtual void SetNetWlan0ActiveWepKey(int activeWepKey,int index);

    virtual IPV4* GetNetWlan0DhcpdHostIP(void);

    virtual IPV4* GetNetWlan0DhcpdHostIP(int index);

    virtual void SetNetWlan0DhcpdHostIP(IPV4 *pipvValue,int index);

    virtual IPV4* GetNetWlan0DhcpdStartIP(void);

    virtual IPV4* GetNetWlan0DhcpdStartIP(int index);

    virtual void SetNetWlan0DhcpdStartIP(IPV4 *pipvValue,int index);

    virtual IPV4* GetNetWlan0DhcpdEndIP(void);

    virtual IPV4* GetNetWlan0DhcpdEndIP(int index);

    virtual void SetNetWlan0DhcpdEndIP(IPV4 *pipvValue,int index);
#ifdef ENABLE_WIFI_SOFT_AP
#ifdef ENABLE_WIFI_BRIDGE_MODE
	virtual void SetBrgTargetIface(EMUN_BRIDGE_INTERFACE brg_target_iface);

	virtual EMUN_BRIDGE_INTERFACE GetBrgTargetIface();
#endif	//ENABLE_WIFI_BRIDGE_MODE

#endif //ENABLE_WIFI_SOFT_AP
    virtual bool GetNetPPP0Enable(void);

    virtual void SetNetPPP0Enable(bool bValue);

    virtual char* GetNetPPP0UserName(void);

    virtual void SetNetPPP0UserName(char *UserName);

    virtual char* GetNetPPP0Password(void);

    virtual void SetNetPPP0Password(char *Password);

    virtual ENUM_NET_INTERFACE GetNetPPP0Interface(void);

    virtual void SetNetPPP0Interface(ENUM_NET_INTERFACE interface);

#ifdef VCRPLUS
    virtual void SetVcrPlusChMapping(int gcn_ch, SourceOption r_source, int r_ch);

    virtual int GetVcrPlusChMapping(int gcn_ch);

    virtual SourceOption GetVcrPlusSource(int gcn_ch);
#endif

/******************************************/
#if AP_CONFIG_WITH(APCFG_APP_BITTORRENT)
    virtual bool GetImsDaemonState(void);

    virtual void SetImsDaemonState(bool state);

    virtual int GetImsHomeDir(void);

    virtual void SetImsHomeDir(int choice);

    virtual int GetImsDownloadDir(void);

    virtual void SetImsDownloadDir(int choice);
#endif
/******************************************/
#if defined(CASABLANCA)
    virtual SCREEN_SAVER_TIMING GetScreenSaverTiming();

    virtual void SetScreenSaverTiming(SCREEN_SAVER_TIMING screen_saver_timing);

    virtual SCREEN_SAVER_OPTION GetScreenSaverOption();

    virtual void SetScreenSaverOption(SCREEN_SAVER_OPTION screen_saver_option);

    virtual int GetFolderPartitionId();

    virtual void SetFolderPartitionId(int folder_partition_id);

    virtual int GetFolderFieldId();

    virtual void SetFolderFieldId(int folder_field_id);

    virtual char* GetFolderDataPath();

    virtual void SetFolderDataPath(char* folder_data_path);
#else
    virtual ENUM_ON_OFF GetScreenSaver();

    virtual void SetScreenSaver(ENUM_ON_OFF screen_saver);
#endif

    /*Get Set Ken Burns 2009/6/17 kelly*/

    virtual ENUM_ON_OFF GetKenBurns();
    virtual void SetKenBurns(ENUM_ON_OFF ken_burns);

    virtual ENUM_ON_OFF GetMoviePreview() ;

    virtual void SetMoviePreview(ENUM_ON_OFF movie_preview) ;

    virtual bool GetDivXCode(char **code);

    virtual void SetDivXCode(char *code);

    virtual bool GetDisableExtHddDB(void);

    virtual void SetDisableExtHddDB(bool disable);

    virtual bool GetDisableAutoScanExtHdd(void);

    virtual void SetDisableAutoScanExtHdd(bool disable);

    virtual char *GetSkinName();

    virtual void SetSkinName(char *skinName);

#ifdef USE_HDMI
    virtual int GetHdmiDeepColor(void);

    virtual void SetHdmiDeepColor(int value);
#endif

#ifdef TVE_DEC_SWITCH
    virtual int GetTveSwitch(void);

    virtual void SetTveSwitch(int value);
#endif

#ifdef ENABLE_HDMV_PLAYBACK
    virtual ENUM_ON_OFF GetHdmvNavigation(void);

    virtual void SetHdmvNavigation(ENUM_ON_OFF hdmv_navigation);
#endif

    virtual ENUM_ON_OFF GetBtProcess(void);

    virtual void SetBtProcess(ENUM_ON_OFF bt_process);

    virtual ENUM_ON_OFF GetSambaProcess(void);

    virtual void SetSambaProcess(ENUM_ON_OFF samba_process);

    virtual ENUM_ON_OFF GetNeighborSecurity(void);

    virtual void SetNeighborSecurity(ENUM_ON_OFF security);

    virtual KEY_TONE_VOLUME GetKeyToneVolume(void);

    virtual void SetKeyToneVolume(KEY_TONE_VOLUME key_tone_volume);

#ifdef USE_CEC
    virtual bool GetEnableHdmiCec(void);

    virtual void SetEnableHdmiCec(bool value);

    virtual bool GetEnableHdmiCecOneTouchPlay();

    virtual void SetEnableHdmiCecOneTouchPlay(bool one_touch_play);

    virtual bool GetEnableHdmiCecSystemStandby();

    virtual void SetEnableHdmiCecSystemStandby(bool system_standby);

    virtual bool GetEnableHdmiCecAutoOsdLanguage();

    virtual void SetEnableHdmiCecAutoOsdLanguage(bool auto_osd_language);

    virtual  bool GetEnableHdmiCecAutoPowerOnFromTv();

    virtual  void SetEnableHdmiCecAutoPowerOnFromTv(bool auto_power_from_tv);

    virtual int	GetCECPreferredLogicalAddress();
    virtual  void SetCECPreferredLogicalAddress(int logicAddress);

    virtual bool  GetSystemAudioControl();

    virtual void SetSystemAudioControl(bool SystemAudioControl);

#endif
#ifdef USE_HDMI
    virtual void SetHdmiLipSync(ENUM_LIP_SYNC_SETTING type, int value);

    virtual ENUM_LIP_SYNC_SETTING GetHdmiLipSync(int *value);
#endif

#ifdef ENABLE_WIFI_SOFT_AP
	virtual ENUM_SOFT_AP_MODE GetSoftwareAPMode();
	virtual void SetSoftwareAPMode(ENUM_SOFT_AP_MODE softApMode);

	virtual char* GetSoftwareAPName(void);
	virtual void SetSoftwareAPName(char *ApName);

	virtual ENUM_WIRELESS_SECURITY GetSoftwareAPSecurity();
	virtual void SetSoftwareAPSecurity(ENUM_WIRELESS_SECURITY wlan0_security);

	virtual char* GetSoftwareAPWepKey(void);
	virtual void SetSoftwareAPWepKey(char *WepKey);

	virtual ENUM_NET_REGION GetSoftwareAPRegion();
	virtual void SetSoftwareAPRegion(ENUM_NET_REGION net_region);

#endif
	virtual ENUM_WIFI_FREQ GetWiFiFreq();
	virtual void SetWiFiFreq(ENUM_WIFI_FREQ value);

	virtual ENUM_ON_OFF GetPhotoActualSize(void);

	virtual void SetPhotoActualSize(ENUM_ON_OFF actualSize);

#ifdef HW_VGA
    virtual ENUM_VIDEO_VGA_RESOLUTION GetVgaResolution(void);
    virtual void SetVgaResolution(ENUM_VIDEO_VGA_RESOLUTION res);

	virtual ENUM_VIDEO_SYSTEM GetRealTvSystem();
	virtual void SetRealTvSystem(ENUM_VIDEO_SYSTEM video_system);
#endif
    virtual const char* GetDeviceName();
    virtual void  SetDeviceName(const char *device_name);

	virtual ENUM_TV_SYSTEM_USER GetUserTvSystem();
	virtual void SetUserTvSystem(ENUM_TV_SYSTEM_USER tv_system_user);
};
#endif

