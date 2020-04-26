#ifndef __BOARD_HEADER_FILE__
#define __BOARD_HEADER_FILE__
#include <Application/AppClass/setupdef.h>
#include <Platform_Lib/Graphics/user_cmd.h>
#include <VideoRPCBaseDS_data.h>
#include <AudioRPC_Agent.h>
#include <VideoRPC_Agent.h>
#if IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
  #include <Platform_Lib/JupiterDummyCommon.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _tags_codec_input_enum {
	FRONT_INPUT_1,
	FRONT_INPUT_2,
	REAR_INPUT_1,
	REAR_INPUT_2,
	MIC,
	TUNER,
} CODEC_INPUT_ENUM;

typedef enum _tags_audio_input_enum {
	AUDIO_INPUT_ADC0,
	AUDIO_INPUT_ADC1,
	AUDIO_INPUT_SPDIF,
	AUDIO_INPUT_BITSTREAM,
	AUDIO_INPUT_NONE,
} AUDIO_INPUT_ENUM;

typedef enum _video_in_format_enum {
       AUTO_DETECT,
       NTSC,
       NTSC_443,
       PAL_I,
       PAL_M,
       PAL_CN,
       SECAM,
       VIDEO_IN_FORMAT_NUM,
} VIDEO_IN_FORMAT;

typedef enum _video_in_adjust_enum {
       BRIGHTNESS,
       CONTRAST,
       SATURATION,
       HUE,
       VIDEO_IN_ADJUST_NUM,
} VIDEO_IN_ADJUST;

typedef enum {
	POWER_ON_COLD_BOOT,
	POWER_ON_WARM_BOOT,
	POWER_ON_TIME_BOOT
} POWER_ON_MODE;

typedef uint32_t USB_CAPABILITY;

// Common
#define    HOT_PLUG_OUT            0x00000001
#define    HOT_PLUG_IN             0x00000002

// Storage, "CMD_STORAGE_UPDATED"
#define    HOT_PLUG_SATA           0x00000004
#define    HOT_PLUG_CARD           0x00000008
#define    HOT_PLUG_USB            0x00000010
#define    HOT_PLUG_INTERNAL       0x00000020
#define    HOT_PLUG_PRIORITY       0x00000040
#define    HOT_PLUG_UNSAFE         0x00000080

// User Interface, "CMD_HID_UPDATED"
#define    HOT_PLUG_KBD            0x00000004
#define    HOT_PLUG_MICE           0x00000008
#define    HOT_PLUG_MICROPHONE     0x00000010
#define    HOT_PLUG_EARPHONE       0x00000020
#define    HOT_PLUG_CAMERA         0x00000040

typedef uint32_t HOT_PLUG_CAPABILITY;

typedef struct _storage_capability {
    int idx;
    HOT_PLUG_CAPABILITY capability;
    char id[32];
} STORAGE_CAPABILITY;

struct board_driver {
	char name[32];

	// Audio Codec Control
	AUDIO_INPUT_ENUM (*board_codec_switch_input)(SourceOption input);
	AUDIO_INPUT_ENUM (*board_codec_switch_mic)(void);
	void (*board_codec_mute)(bool bValue);
	void (*board_dac_mute)(bool bValue);

	// Video Input Control
	bool (*vin_exist)(SourceOption input);
	ENUM_TVD_INPUT_FORMAT (*getINFMT)(SourceOption input);

	// Vidio Output Availability test
	bool (*vout_exist)(OutputOption option);

	// TV Encoder
	enum TVE_BOARD_TYPE (*getTVE)(void);
	void (*getTVS)(video_system videoSystem, VO_STANDARD *standard, VO_PEDESTAL_TYPE *pedType);

    int (*getTveDecMode)(void);

	// Video In Parameter
	unsigned char (*getVideoInDefaultValue)(VIDEO_IN_ADJUST, VIDEO_IN_FORMAT);

	// SCART
	void (*setFormatSCART)(bool bIsRGB);
	void (*setAVModeSCART)(bool bIsAVMode);
	void (*setScaleModeSCART)(bool bIsWidescreen);
	bool (*getFormatSCART)(void);

	// LED
	void (*setPowerLED)(bool bLight);
	void (*setRecordLED)(bool bLight);
	void (*setHddLED)(bool bLight);
	void (*setDvdLED)(bool bLight);
	void (*setTimerRecordLED)(bool bLight);
	void (*setSATRecordLED)(bool bLight);
	void (*setEthernetLED)(bool bLight);
	void (*setWifiLED)(bool bLight);
	void (*setWifiAPLED)(bool bLight);

	// Keypad Controlled by VFD
	char (*KeypadTranslate)(char keypadNum);

	// USB
	int (*USB_GetCardreaderPortNumber)(void);
	int (*USB_GetCardreaderPortNumber_string)(char*);
	USB_CAPABILITY (*getCapabilityByUsbPort)(int port);

    // SATA
    HOT_PLUG_CAPABILITY (*getCapabilityBySataPort)(int port);

    // MASS STORAGE
    STORAGE_CAPABILITY (*getSystemStorageDeviceCapability)(void);

    // CARD READER
    STORAGE_CAPABILITY (*getCardReaderCapability)(void);

	// HDMI
	bool (*HDMI_HotPlug)(void);
	bool (*getDeepColorMode)(void);

	// GPIO
	bool (*PCUSBConnection)(void);
	bool (*RegisterGPIOPanel)(int gpio_fd);
	bool (*ButtonPress)(void);

	// Keypad Controlled by GPIO
	char (*GPIOPanelTranslate)(char gpioNum);

	int (*getSartDecoderResult)(void);

	// Board-dependant RPC
	void (*RPC_HDMI_Mute)(long *bMute, RPC_STRUCT *rpc, HRESULT *retval);
	void (*RPC_HDMI_Setting)(AUDIO_HDMI_SETTING_INFO *pInfo, RPC_STRUCT *rpc, HRESULT *retval);
	void (*RPC_USB_AudioIn_Command)(USB_AUDIO_IN_INFO *pInfo, RPC_STRUCT *rpc, HRESULT *retval);

	POWER_ON_MODE (*getBootMode)(void);
	int (*getBootKey)(void);

	//External Power Control if any
	void (*ODDPowerSwitch)(bool bSwitch);
	void (*HDDPowerSwitch)(bool bSwitch);
	void (*SysFanPowerSwitch)(bool bSwitch);
	void (*TunerPowerSwitch)(bool bSwitch);
	void (*TunerPowerReset)(bool bReset);
	void (*AntennaFeedPowerSwitch)(bool bSwitch);
	void (*FrontPanelPowerSwitch) (bool bSwitch);
};

void board_init(void);
void board_uninit(void);

char *board_name(void);

AUDIO_INPUT_ENUM board_codec_switch_input(SourceOption input);
AUDIO_INPUT_ENUM board_codec_switch_mic(void);

void board_codec_mute(bool bValue);
void board_dac_mute(bool bValue);

bool vin_exist(SourceOption input);
bool vout_exist(OutputOption option);
ENUM_TVD_INPUT_FORMAT getINFMT(SourceOption input);
char KeypadTranslate(char keypadNum);

enum TVE_BOARD_TYPE getTVE(void);

void setFormatSCART(bool bIsRGB);
void setAVModeSCART(bool bIsAVMode);
void setScaleModeSCART(bool bIsWidescreen);
bool getFormatSCART(void); // true for RGB, false for CVBS

void setPowerLED(bool bLight);
void setRecordLED(bool bLight);
void setHddLED(bool bLight);
void setDvdLED(bool bLight);
void setTimerRecordLED(bool bLight);
void setSATRecordLED(bool bLight);
void setEthernetLED(bool bLight);
void setWifiLED(bool bLight);
void setWifiAPLED(bool bLight);

int USB_GetCardreaderPortNumber(void);
int USB_GetCardreaderPortNumber_string(char*);
int getSartDecoderResult(void);

unsigned char getVideoInDefaultValue(VIDEO_IN_ADJUST,VIDEO_IN_FORMAT);
USB_CAPABILITY getCapabilityByUsbPort(int port);
HOT_PLUG_CAPABILITY getCapabilityBySataPort(int port);
STORAGE_CAPABILITY getSystemStorageDeviceCapability(void);
STORAGE_CAPABILITY getCardReaderCapability(void);
void getTVS(video_system videoSystem, VO_STANDARD *standard, VO_PEDESTAL_TYPE *pedType);
int getTveDecMode(void); //TVE DEC mode, ##(#->DAC#123,DAC#456) (0->CVBS,YPbPr), (1->CVBS,CVBS), (2->YPbPr,YPbPr), (3->YPbPr,CVBS)
bool PCUSBConnection(void);
bool HDMI_HotPlug(void);
bool getDeepColorMode(void);
bool RegisterGPIOPanel(int gpio_fd);
char GPIOPanelTranslate(char gpioNum);
bool ButtonPress(void);

POWER_ON_MODE getBootMode(void);
int getBootKey(void);

//External Power Control
void switchOnODDPower(bool bSwitch);
void switchOnHDDPower(bool bSwitch);
void switchOnSysFanPower(bool bSwitch);
void switchOnTunerPower(bool bSwitch);
void resetTunerPower(bool bReset);
void switchAntennaFeedPower(bool bSwitch);
void switchFrontPanelPower(bool bSwitch);

// for board library
void BoardRegister(struct board_driver *);
void BoardUnregister(void);

#ifdef __cplusplus
}
#endif

#endif
