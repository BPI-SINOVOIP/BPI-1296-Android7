#ifndef _SETUP_CLASS_ANDROID_H_
#define _SETUP_CLASS_ANDROID_H_
#include <Application/AppClass/setupdef.h>
#include <Application/AppClass/SetupClassVolatile.h>

class settingsclient;

class SetupClassAndroid : public SetupClassVolatile {
/**
 * These table names must be consistent with the names
 * defined in device/realtek/frameworks/services/java/com/realtek/server/SettingsService.java
 */
#define TABLE_SYSTEM "system"
#define TABLE_GLOBAL "global"
#define TABLE_SECURE "secure"

private:
    settingsclient *mClient;

    /**
     * For all get methods, you need to check its return value to confirm
     * whether the result can be referenced.
     * If the result value is casted to int and its value is -22, then it
     * means the SettingsService is not ready such that you can't get the
     * real value stored in database.
     */
public:
    SetupClassAndroid();
    ~SetupClassAndroid();

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

	void SetTextSubtitleOffset(int offset);

	ENUM_VIDEO_SYSTEM GetTvSystem();

	void SetTvSystem(ENUM_VIDEO_SYSTEM video_system);

	ENUM_VIDEO_STANDARD GetTvStandard();

	void SetTvStandard(ENUM_VIDEO_STANDARD video_standard);

	VIDEO_SYSTEM_INFO GetVideoSystem();

	void SetVideoSystem(VIDEO_SYSTEM_INFO video_system_info);

	ENUM_TV_SYSTEM_AUTO_MODE GetTvSystemAutoMode();

	void SetTvSystemAutoMode(ENUM_TV_SYSTEM_AUTO_MODE mode);

	ENUM_ASPECT_RATIO GetAspectRatio();

	void SetAspectRatio(ENUM_ASPECT_RATIO aspect_ratio);

	ENUM_VIDEO_OUTPUT GetVideoOutput();

	void SetVideoOutput(ENUM_VIDEO_OUTPUT video_output);

	ENUM_ON_OFF GetVideoZoomOutMode();

	void SetVideoZoomOutMode(ENUM_ON_OFF zoom_out_mode);
#ifdef USE_CEC
     bool GetEnableHdmiCec(void);

     void SetEnableHdmiCec(bool value);

     bool GetEnableHdmiCecOneTouchPlay();

     void SetEnableHdmiCecOneTouchPlay(bool one_touch_play);

     bool GetEnableHdmiCecSystemStandby();

     void SetEnableHdmiCecSystemStandby(bool system_standby);

     bool GetEnableHdmiCecAutoOsdLanguage();

     void SetEnableHdmiCecAutoOsdLanguage(bool auto_osd_language);

     bool GetEnableHdmiCecAutoPowerOnFromTv();

     void SetEnableHdmiCecAutoPowerOnFromTv(bool auto_power_from_tv);

     int  GetCECPreferredLogicalAddress();
     void SetCECPreferredLogicalAddress(int logicAddress);

     bool GetSystemAudioControl();

     void SetSystemAudioControl(bool SystemAudioControl);
#endif

#ifdef USE_HDMI
	int GetCurrentHdmiTVVid();

	void SetCurrentHdmiTVVid(int CurrentHdmiTVVid);
#endif /* USE_HDMI */

	int GetMiniKBDLanguage();

	void SetMiniKBDLanguage(int minikb_lang);

#ifdef TVE_DEC_SWITCH
	int GetTveSwitch(void);

	void SetTveSwitch(int value);
#endif

#ifdef HW_VGA
	ENUM_VIDEO_VGA_RESOLUTION GetVgaResolution(void);

	void SetVgaResolution(ENUM_VIDEO_VGA_RESOLUTION res);

	ENUM_VIDEO_SYSTEM GetRealTvSystem();

	void SetRealTvSystem(ENUM_VIDEO_SYSTEM video_system);
#endif
    const char* GetDeviceName();
    void  SetDeviceName(const char *device_name);

    ENUM_TV_SYSTEM_USER GetUserTvSystem();
    void SetUserTvSystem(ENUM_TV_SYSTEM_USER tv_system_user);

#ifdef ANDROID
    const char *getProperty(const char *prop, const char *def);
    bool setProperty(const char *prop, const char *value);
    const char *wifiCmd(WIFI_CMD_E wifiCmd, ... );
#endif

private:
    int writeInt32ToField(const char *table, const char *fieldName, const int32_t value);
    int readInt32ToField(const char *table, const char *fieldName);

    const char *readStringToField(const char* table, const char *fieldName);
    bool writeStringToField(const char* table, const char *fieldName, const char *value);
};

#endif /* _SETUP_CLASS_ANDROID_H_ */
