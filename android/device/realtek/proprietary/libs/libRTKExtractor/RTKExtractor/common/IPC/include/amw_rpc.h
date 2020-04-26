
#ifndef __AMW_RPC_H__
#define __AMW_RPC_H__


#if defined(SYS_ROS)
#include <rt/misc/types.h>
#endif


#if !defined(SYS_ROS) && defined(GNU_TOOL)
typedef long long					int64_t;
typedef unsigned long long			u_int64_t;
#endif

#if defined(WIN32)
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
typedef signed __int64	int64_t;
#endif

#include "AudioRPCBaseDS_data.h"
#include "RPCBaseDS_data.h"
#include "AudioInbandAPI.h"

#ifndef RPC_MECHANISM

/////////////////////////////////////////////////////
#ifdef  __cplusplus
extern "C" {
#endif


#define RPC_TOAGENT_CREATE(x,y,z)			RPC_Create(x,y,z)

#define RPC_TOAGENT_KARAOKE_CONTROL(x,y,z)		RPC_Karaoke_Control(x,y,z)
#define RPC_TOAGENT_PLAYSOUNDEVENT(x,y,z)		RPC_PlaySoundEvent(x,y,z)
#define RPC_TOAGENT_AOUT_HDMI_SET(x,y,z)		RPC_AOUT_HDMI_Set(x,y,z)
#define RPC_TOAGENT_HDMI_MUTE(x,y,z)			RPC_HDMI_Mute(x,y,z)
#define RPC_TOAGENT_AO_ONLY_SWITCHFOCUS(x,y,z)	RPC_AO_Only_Switchfocus(x,y,z)
#define RPC_AO_TOAGENT_MUTE(x,y,z)			RPC_AO_ToAgent_Mute(x,y,z)
#define RPC_TOAGENT_AOUT_SPDIF_SOURCE(x,y,z)		RPC_AOUT_SPDIF_Source(x,y,z)
#define RPC_TOAGENT_AOUT_VOLUME_CONTROL(x,y,z)		RPC_AOUT_Volume_Control(x,y,z)
#define RPC_TOAGENT_AOUT_COPY_SOURCE(x,y,z)		RPC_AOUT_Copy_Source(x,y,z)
#define RPC_TOAGENT_AIN_CONFIG_TSD(x,y,z)		RPC_AIN_Config_TSD(x,y,z)
#define RPC_TOAGENT_AIN_DATA_MEASUREMENT(x,y,z)	RPC_AIN_Data_Measurement(x,y,z)
#define RPC_TOAGENT_AIN_SWITCH_FOCUS(x,y,z)		RPC_AIN_Switch_Focus(x,y,z)
#define RPC_TOAGENT_AIN_MUTE(x,y,z)				RPC_AIN_Mute(x,y,z)
#define RPC_TOAGENT_ADC0_CONFIG(x,y,z)			RPC_ADC0_Config(x,y,z)
#define RPC_TOAGENT_ADC1_CONFIG(x,y,z)			RPC_ADC1_Config(x,y,z)
#define RPC_TOAGENT_ADC2_CONFIG(x,y,z)			RPC_ADC2_Config(x,y,z)
#define RPC_TOAGENT_AFC_CONFIG(x,y,z)			RPC_AFC_Config(x,y,z)
#define RPC_TOAGENT_SPDIF_CONFIG(x,y,z)                RPC_SPDIF_Config(x,y,z)
//#define RPC_TOAGENT_SAMPLE_CONFIG		RPC_Sample_Config
#define RPC_TOAGENT_SPDIF_ERR_THRESHOLD_CONFIG(x,y,z)	RPC_SPDIF_Err_Threshold_Config(x,y,z)
#define RPC_TOAGENT_AUDIO_AI_BACKDOOR_INIT(x,y,z)	RPC_Audio_AI_BackDoor_Init(x,y,z)

#define RPC_TOAGENT_DAC_I2S_CONFIG(x,y,z)		RPC_DAC_I2S_Config(x,y,z)
#define RPC_TOAGENT_AUDISTRY_CONFIG(x,y,z)		RPC_Audistry_Config(x,y,z)
#define RPC_TOAGENT_DAC_SPDIF_CONFIG(x,y,z)		RPC_DAC_SPDIF_Config(x,y,z)

#define RPC_TOAGENT_CONNECT(x,y,z)			RPC_Connect(x,y,z)
#define RPC_TOAGENT_DISCONNECT(x,y,z)
#define RPC_TOAGENT_DESTROY(x,y,z)			RPC_Destroy(x,y,z)
#define RPC_TOAGENT_RUN(x,y,z)				RPC_Run(x,y,z)
#define RPC_TOAGENT_PAUSE(x,y,z)			RPC_Pause(x,y,z)
#define RPC_TOAGENT_STOP(x,y,z)			RPC_Stop(x,y,z)
#define RPC_TOAGENT_GETSTATUS(x,y,z)		RPC_GetStatus(x,y,z)
#define RPC_TOAGENT_SEEK(x,y,z)			RPC_Seek(x,y,z)
#define RPC_TOAGENT_UPDATEWRITEPTR(x,y,z)	RPC_UpdateWritePtr(x,y,z)
#define RPC_TOAGENT_UPDATEREADPTR(x,y,z)	RPC_UpdateReadPtr(x,y,z)
#define RPC_TOAGENT_PRIVATEINFO(x,y,z)	RPC_PrivateInfo(x,y,z)

///////////////////////////////////////////////////////////////////////////////
#define RPC_TOAGENT_ENDOFSTREAM(x,y,z)		RPC_EndOfStream(x,y,z)
#define RPC_TOAGENT_RECEIVE(x,y,z)			RPC_Receive(x,y,z)

#define RPC_TOAGENT_FLUSH(x,y,z)			RPC_Flush(x,y,z)
#define RPC_TOAGENT_SET_SEEKING(x,y,z)		RPC_SetSeeking(x,y,z)

#define RPC_TOAGENT_PCMFORMAT(x,y,z)		RPC_PCMFormat(x,y,z)
#define RPC_TOAGENT_DECODERCONFIG(x,y,z)	RPC_DecoderConfig(x,y,z)
#define RPC_TOAGENT_EXTBSEXIST(x,y,z)		RPC_ExtBSExist(x,y,z)
#define RPC_TOAGENT_SWITCH_AGENT(x,y,z)	RPC_SwitchAgent(x,y,z)
#define RPC_TOAGENT_SETREFCLOCK(x,y,z)		RPC_SetRefClock(x,y,z)
#define RPC_TOAGENT_UPDATEPTS(x,y,z)		RPC_UpdatePTS(x,y,z)
#define RPC_TOAGENT_INITRINGBUFFER_HEADER(x,y,z)	RPC_InitRingBufferHeader(x,y,z)
#define RPC_TOAGENT_SWITCHFOCUS(x,y,z)			RPC_SwitchFocus(x,y,z)
#define RPC_TOAGENT_HDMI_OUT_EDID(x,y,z)	RPC_HDMI_OUT_EDID(x,y,z)
#define RPC_TOAGENT_HDMI_OUT_VSDB(x,y,z)	RPC_HDMI_OUT_VSDB(x,y,z)
#define RPC_TOAGENT_HDMI_INFO(x,y,z)		RPC_HDMI_INFO(x,y,z)

#define RPC_TOAGENT_PP_FUNCTION(x,y,z)		RPC_PP_Config(x,y,z)
#define RPC_TOAGENT_PP_SRC_CONFIG(x,y,z)	RPC_PP_SRC_Config(x,y,z)
#define RPC_TOAGENT_PP_PL2_CONFIG(x,y,z)	RPC_PP_PL2_Config(x,y,z)
#define RPC_TOAGENT_PP_MIXER_CONFIG(x,y,z)	RPC_PP_MIXER_Config(x,y,z)
#define RPC_TOAGENT_PP_DVS_CONFIG(x,y,z)	RPC_PP_DVS_Config(x,y,z)
#define RPC_TOAGENT_PP_BASS_MANAGEMENT_CONFIG(x,y,z)	RPC_PP_BASS_MANAGEMENT_Config(x,y,z)
#define RPC_TOAGENT_PP_KEY_SHIFT_CONFIG(x,y,z)		RPC_PP_KEY_SHIFT_Config(x,y,z)
#define RPC_TOAGENT_PP_REVERB_CONFIG(x,y,z)		RPC_PP_REVERB_Config(x,y,z)
#define RPC_TOAGENT_PP_EQ_CONFIG(x,y,z)		RPC_PP_EQ_Config(x,y,z)
#define RPC_TOAGENT_PP_VOCAL_REMOVER_CONFIG(x,y,z)	RPC_PP_VOCAL_REMOVER_Config(x,y,z)

#define RPC_TOAGENT_NIGHTMODE(x,y,z)		RPC_NightMode(x,y,z)
#define RPC_TOAGENT_PP_INIT_PIN(x,y,z)		RPC_PP_InitPin(x,y,z)

#define RPC_TOAGENT_AI_DEVICE_USBINFO(x,y,z)    RPC_AI_Device_USBinfo(x,y,z)


#define RPC_TOAGENT_CREATE_SVC			RPC_Create

#define RPC_TOAGENT_KARAOKE_CONTROL_SVC			RPC_Karaoke_Control
#define RPC_AO_TOAGENT_MUTE_SVC				RPC_AO_Toagent_Mute
#define RPC_TOAGENT_AOUT_SPDIF_SOURCE_SVC		RPC_AOUT_SPDIF_Source
#define RPC_TOAGENT_AOUT_VOLUME_CONTROL_SVC		RPC_AOUT_Volume_Control
#define RPC_TOAGENT_AOUT_DROP_SAMPLE_SVC		RPC_AOUT_Drop_Sample
#define RPC_TOAGENT_AOUT_PINK_WHITE_NOISE_SVC	RPC_AOUT_Pink_White_Noise
#define RPC_TOAGENT_AOUT_SEND_SPECTRUM_DATA_SVC	RPC_AOUT_Send_Spectrum_Data
#define RPC_TOAGENT_PLAYSOUNDEVENT_SVC			RPC_PlaySoundEvent
#define RPC_TOAGENT_AOUT_HDMI_SET_SVC			RPC_AOUT_HDMI_Set
#define RPC_TOAGENT_HDMI_MUTE_SVC				RPC_HDMI_Mute
#define RPC_TOAGENT_AO_ONLY_SWITCHFOCUS_SVC		RPC_AO_Only_Switchfocus

#define RPC_TOAGENT_AOUT_COPY_SOURCE_SVC		RPC_AOUT_Copy_Source
#define RPC_TOAGENT_AIN_CONFIG_TSD_SVC			RPC_AIN_Config_TSD
#define RPC_TOAGENT_AIN_DATA_MEASUREMENT_SVC	RPC_AIN_Data_Measurement
#define RPC_TOAGENT_AIN_SWITCH_FOCUS_SVC		RPC_AIN_Switch_Focus
#define RPC_TOAGENT_AIN_MUTE_SVC				RPC_AIN_Mute
#define RPC_TOAGENT_ADC0_CONFIG_SVC			RPC_ADC0_Config
#define RPC_TOAGENT_ADC1_CONFIG_SVC			RPC_ADC1_Config
#define RPC_TOAGENT_ADC2_CONFIG_SVC			RPC_ADC2_Config
#define RPC_TOAGENT_AFC_CONFIG_SVC			RPC_AFC_Config
#define RPC_TOAGENT_SPDIF_CONFIG_SVC                    RPC_SPDIF_Config
//#define RPC_TOAGENT_SAMPLE_CONFIG_SVC			RPC_Sample_Config
#define RPC_TOAGENT_SPDIF_ERR_THRESHOLD_CONFIG_SVC	RPC_SPDIF_Err_Threshold_Config
#define RPC_TOAGENT_AUDIO_AI_BACKDOOR_INIT_SVC		RPC_Audio_AI_BackDoor_Init

#define RPC_TOAGENT_DAC_I2S_CONFIG_SVC			RPC_DAC_I2S_Config
#define RPC_TOAGENT_AUDISTRY_CONFIG_SVC			RPC_Audistry_Config
#define RPC_TOAGENT_DAC_SPDIF_CONFIG_SVC		RPC_DAC_SPDIF_Config

#define RPC_TOAGENT_CONNECT_SVC			RPC_Connect
#define RPC_TOAGENT_DISCONNECT_SVC
#define RPC_TOAGENT_DESTROY_SVC			RPC_Destroy
#define RPC_TOAGENT_RUN_SVC				RPC_Run
#define RPC_TOAGENT_PAUSE_SVC			RPC_Pause
#define RPC_TOAGENT_STOP_SVC			RPC_Stop
#define RPC_TOAGENT_GETSTATUS_SVC		RPC_GetStatus
#define RPC_TOAGENT_SEEK_SVC			RPC_Seek
#define RPC_TOAGENT_UPDATEWRITEPTR_SVC	RPC_UpdateWritePtr
#define RPC_TOAGENT_UPDATEREADPTR_SVC	RPC_UpdateReadPtr
#define RPC_TOAGENT_PRIVATEINFO_SVC		RPC_PrivateInfo

///////////////////////////////////////////////////////////////////////////////
#define RPC_TOAGENT_ENDOFSTREAM_SVC		RPC_EndOfStream
#define RPC_TOAGENT_RECEIVE_SVC			RPC_Receive

#define RPC_TOAGENT_FLUSH_SVC			RPC_Flush
#define RPC_TOAGENT_SET_SEEKING_SVC		RPC_SetSeeking

#define RPC_TOAGENT_PCMFORMAT_SVC		RPC_PCMFormat
#define RPC_TOAGENT_DECODERCONFIG_SVC	RPC_DecoderConfig
#define RPC_TOAGENT_EXTBSEXIST_SVC		RPC_ExtBSExist
#define RPC_TOAGENT_SWITCH_AGENT_SVC	RPC_SwitchAgent
#define RPC_TOAGENT_SETREFCLOCK_SVC		RPC_SetRefClock
#define RPC_TOAGENT_UPDATEPTS_SVC		RPC_UpdatePTS
#define RPC_TOAGENT_INITRINGBUFFER_HEADER_SVC	RPC_InitRingBufferHeader
#define RPC_TOAGENT_SWITCHFOCUS_SVC		RPC_SwitchFocus
#define RPC_TOAGENT_HDMI_OUT_EDID_SVC		RPC_HDMI_OUT_EDID
#define RPC_TOAGENT_HDMI_OUT_VSDB_SVC		RPC_HDMI_OUT_VSDB
#define RPC_TOAGENT_HDMI_INFO_SVC		RPC_HDMI_INFO

#define RPC_TOAGENT_PP_FUNCTION_SVC		RPC_PP_Config
#define RPC_TOAGENT_PP_SRC_CONFIG_SVC	RPC_PP_SRC_Config
#define RPC_TOAGENT_PP_PL2_CONFIG_SVC	RPC_PP_PL2_Config
#define RPC_TOAGENT_PP_MIXER_CONFIG_SVC	RPC_PP_MIXER_Config
#define RPC_TOAGENT_DVS_CONFIG_SVC	    RPC_PP_DVS_Config
#define RPC_TOAGENT_PP_BASS_MANAGEMENT_CONFIG_SVC	RPC_PP_BASS_MANAGEMENT_Config
#define RPC_TOAGENT_PP_KEY_SHIFT_CONFIG_SVC	RPC_PP_KEY_SHIFT_Config
#define RPC_TOAGENT_PP_REVERB_CONFIG_SVC	RPC_PP_REVERB_Config
#define RPC_TOAGENT_PP_EQ_CONFIG_SVC	RPC_PP_EQ_Config
#define RPC_TOAGENT_PP_VOCAL_REMOVER_CONFIG_SVC	RPC_PP_VOCAL_REMOVER_Config

#define RPC_TOAGENT_NIGHTMODE_SVC		RPC_NightMode
#define RPC_TOAGENT_PP_INIT_PIN_SVC		RPC_PP_InitPin

#define RPC_TOAGENT_AI_DEVICE_USBINFO_SVC   RPC_AI_Device_USBinfo

///////////////////////////////////////////////////////////////////////////////////
#define PDEBUG(x)
#define NONBLOCK_MODE	0x00
#define	BLOCK_MODE		0x01
#define USE_POLL_BUF	0x00
#define USE_INTR_BUF	0x02
#define AUDIO_SYSTEM	0
#define AUDIO_AGENT		1
#define VERSION			0
#define SEND_VIDEO_CPU	0x00
#define SEND_AUDIO_CPU	0x04

#ifndef _RPC_STRUCT_H_
typedef int CLNT_STRUCT;
typedef CLNT_STRUCT RPC_STRUCT;
static CLNT_STRUCT prepareCLNT (int opt, long programID, long versionID)
{
	return 0;
}
#endif

//////////////////////////////////////////////////////////////
CLNT_STRUCT prepareCLNT (int opt, long programID, long versionID);

RPCRES_LONG*		RPC_Create(AUDIO_RPC_INSTANCE *instance, RPC_STRUCT* clnt, RPCRES_LONG* retval);
HRESULT*			RPC_InitRingBuffer(AUDIO_RPC_RINGBUFFER* ringBUffer, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_UpdateWritePtr(AUDIO_RPC_WRITEPACK* pWritePack, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_UpdateReadPtr(AUDIO_RPC_READPACK* pReadPack, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_Destroy(long* instanceID, RPC_STRUCT* pClnt, HRESULT* retval);
//HRESULT*			RPC_PrivateInfo(AUDIO_RPC_PRIVATEINFO* pPrivateInfo, RPC_STRUCT* clnt);
HRESULT*			RPC_SetSeeking(AUDIO_RPC_SEEK* pSeekInfo,  RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_EndOfStream(AUDIO_RPC_SENDPIN_LONG* pSendIO, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*	        RPC_Flush(AUDIO_RPC_SENDIO* pSendIO, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_Run(long* instanceID, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_Stop(long* instanceID, RPC_STRUCT* clnt, HRESULT* retval);

HRESULT*			RPC_ExtBSExist(AUDIO_RPC_EXTBS_EXIST* pExtbsExist, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_DecoderConfig(AUDIO_RPC_DECCFG* pDecCFG, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_PCMFormat(AUDIO_RPC_PCMFORMAT* pPCMFormat, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_AOUT_Copy_Source(AUDIO_OUT_COPY_SOURCE* ao_copy_info, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_Drop_Sample(AUDIO_DROP_SAMPLE* ao_drop_sample, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_Send_Spectrum_Data(AUDIO_SPECTRUM_CFG* ao_send_spectrum_cfg, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_Pink_White_Noise(AUDIO_P_W_NOISE_CFG* ao_pink_white_noise, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_Volume_Control(AUDIO_VOLUME_CONTROL* ao_copy_info, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_SPDIF_Source(AUDIO_OUT_SPDIF_SOURCE* ao_spdif_info, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AO_ToAgent_Mute(AUDIO_OUT_MUTE_INFO* ao_mute, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_Karaoke_Control(AUDIO_KARAOKE_CONTROL_INFO* ao_karaoke_control, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_PlaySoundEvent(AUDIO_SOUND_EVENT* audio_sound_event, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AOUT_HDMI_Set(AUDIO_HDMI_SET* audio_HDMI_set, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_TOAGENT_HDMI_MUTE_SVC(AUDIO_HDMI_MUTE_INFO* audio_hdmi_mute_info, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*            RPC_TOAGENT_AO_ONLY_SWITCHFOCUS_SVC(AUDIO_RPC_AO_FOCUS* pfocus, RPC_STRUCT * rpc, HRESULT *retval);

#ifdef SYS_ROS

//HRESULT* 			RPC_TOAGENT_AIN_CONFIG_TSD_SVC(AUDIO_IN_CONFIG_TSD* ai_config_tsd, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AIN_Switch_Focus(AUDIO_IN_SOURCE* ai_focus_in, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_AIN_Mute(AUDIO_IN_MUTE_INFO* audio_adcmuteConfig, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT* 			RPC_ADC0_Config(AUDIO_CONFIG_ADC* audio_adcConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_ADC1_Config(AUDIO_CONFIG_ADC* audio_adcConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_ADC2_Config(AUDIO_CONFIG_ADC* audio_adcConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_AFC_Config(AUDIO_CONFIG_AFC* audio_afcConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_SPDIF_Config(AUDIO_CONFIG_SPDIF* audio_spdifConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_SPDIF_Err_Threshold_Config(AUDIO_CONFIG_SPDIF_ERR* err_config, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_Audio_AI_BackDoor_Init(AUDIO_BACKDOOR_AI_INIT* audio_backdoor_init, RPC_STRUCT* clnt, HRESULT* retval);

HRESULT* 			RPC_DAC_I2S_Config(AUDIO_CONFIG_DAC_I2S* audio_dacConfig, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT* 			RPC_TOAGENT_AUDISTRY_CONFIG_SVC(AUDISTRY_CONFIG_COMMAND *pCmd, RPC_STRUCT *pRPCstruct, HRESULT *pResult);
HRESULT* 			RPC_DAC_SPDIF_Config(AUDIO_CONFIG_DAC_SPDIF* audio_spdifConfig, RPC_STRUCT* clnt, HRESULT* retval);

#endif
HRESULT* 			RPC_TOAGENT_AIN_DATA_MEASUREMENT_SVC(AUDIO_IN_DATA_MEASURE* ai_config_tsd, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_Connect(AUDIO_RPC_CONNECTION* pConnection, RPC_STRUCT* cInt, HRESULT* retval);
HRESULT*			RPC_Pause(long* instanceID, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_SwitchAgent(AUDIO_RPC_SWITCHAGENT* pSwitchInfo, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_SetRefClock(AUDIO_RPC_REFCLOCK* pRefClock, RPC_STRUCT* pClnt, HRESULT* retval);
HRESULT*			RPC_UpdatePTS(AUDIO_RPC_PTS_INFO* pPTSinfo,  RPC_STRUCT * rpc, HRESULT* retval);
HRESULT*			RPC_InitRingBufferHeader(AUDIO_RPC_RINGBUFFER_HEADER* ringBUffer, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_SwitchFocus(AUDIO_RPC_FOCUS* ringBUffer, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_TOAGENT_HDMI_OUT_EDID(AUDIO_HDMI_OUT_EDID_DATA *pEDID, RPC_STRUCT *pRPCstruct, HRESULT *pResult);
HRESULT*			RPC_TOAGENT_HDMI_OUT_VSDB(AUDIO_HDMI_OUT_VSDB_DATA *pVSDB, RPC_STRUCT *pRPCstruct, HRESULT *pResult);
HRESULT*			RPC_TOAGENT_HDMI_INFO(HDMI_INFO *hdmi_info, RPC_STRUCT *pRPCstruct, HRESULT *pResult);

HRESULT*			RPC_PP_Config(AUDIO_PP_CONFIG* audio_PP_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_SRC_Config(AUDIO_SRC_CONFIG* audio_SRC_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_PL2_Config(AUDIO_PROLOGIC2_CONFIG* audio_PL2_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_MIXER_Config(AUDIO_MIXER_CONFIG* audio_MIXER_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_DVS_Config(AUDIO_DVS_CONFIG* audio_dvs_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_BASS_MANAGEMENT_Config(AUDIO_BASS_MANAGEMENT_CONFIG* audio_bassmng_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_KEY_SHIFT_Config(AUDIO_KEY_SHIFT_CONFIG* audio_ksht_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_REVERB_Config(AUDIO_REVERB_CONFIG* audio_reverb_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_RPC_PP_EQ_Config(AUDIO_EQUALIZER_CONFIG* audio_EQ_Config, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*			RPC_PP_VOCAL_REMOVER_Config(AUDIO_VOCAL_REMOVER_CONFIG* audio_vr_Config, RPC_STRUCT* clnt, HRESULT *retval);

HRESULT*			RPC_NightMode(AUDIO_RPC_NIGHTMODE* NightMode, RPC_STRUCT* clnt, HRESULT* retval);
HRESULT*			RPC_PP_InitPin(long* instanceID, RPC_STRUCT* clnt, HRESULT* retval);

USB_DEVICE_CONFIG_INFO* RPC_ToAgent_AI_Device_USBinfo(AUDIO_AI_DEVICE_USB_INFO* audio_ai_device_usb, RPC_STRUCT* clnt, USB_DEVICE_CONFIG_INFO* retval);

////////////////////////////////////////////////////////////////////////////////

//
//HRESULT			RPC_Receive(RPC_RECEIVEINFO* pReceiveInfo, CLIENT* clnt);
//

#ifdef  __cplusplus
}
#endif


#else

#if defined(SYS_ROS)
#include <rt/misc/RPCstruct.h>
#else
#include "RPCstruct.h"
#endif


#define RPC_TOAGENT_CREATE(x,y,z)			AUDIO_RPC_ToAgent_Create_0(x,y)

#define RPC_TOAGENT_KARAOKE_CONTROL(x,y,z)		AUDIO_RPC_ToAgent_Karaoke_Control_0(x,y)
#define RPC_AO_TOAGENT_MUTE(x,y,z)			AUDIO_RPC_AO_ToAgent_Mute_0(x,y)
#define RPC_TOAGENT_AOUT_SPDIF_SOURCE(x,y,z)		AUDIO_RPC_ToAgent_AOUT_SPDIF_Source_0(x,y)
#define RPC_TOAGENT_AOUT_VOLUME_CONTROL(x,y,z)		AUDIO_RPC_ToAgent_AOUT_Volume_Control_0(x,y)
#define RPC_TOAGENT_AOUT_PINK_WHITE_NOISE(x,y,z)		AUDIO_RPC_ToAgent_AOUT_Pink_White_Noise_0(x,y)
#define RPC_TOAGENT_AOUT_SEND_SPECTRUM_DATA(x,y,z)		AUDIO_RPC_ToAgent_AOUT_Send_Spectrum_Data_0(x,y)
#define RPC_TOAGENT_PLAYSOUNDEVENT(x,y,z)				AUDIO_RPC_ToAgent_PlaySoundEvent_0(x,y)
#define RPC_TOAGENT_AOUT_HDMI_SET(x,y,z)				AUDIO_RPC_ToAgent_AOUT_HDMI_Set_0(x,y)
#define RPC_TOAGENT_HDMI_MUTE(x,y,z)					AUDIO_RPC_ToAgent_HDMI_Mute_0(x,y)
#define RPC_TOAGENT_AO_ONLY_SWITCHFOCUS(x,y,z)			AUDIO_RPC_ToAgent_AO_Only_Switchfocus_0(x,y)

#define RPC_TOAGENT_AOUT_DROP_SAMPLE(x,y,z)		AUDIO_RPC_ToAgent_AOUT_Drop_Sample_0(x,y)
#define RPC_TOAGENT_AOUT_COPY_SOURCE(x,y,z)		AUDIO_RPC_ToAgent_AOUT_Copy_Source_0(x,y)
#define RPC_TOAGENT_AIN_CONFIG_TSD(x,y,z)		AUDIO_RPC_ToAgent_AIN_Config_TSD_0(x,y)
#define RPC_TOAGENT_AIN_DATA_MEASUREMENT(x,y,z)	AUDIO_RPC_ToAgent_AIN_Data_Measurement_0(x,y)
#define RPC_TOAGENT_AIN_SWITCH_FOCUS(x,y,z)		AUDIO_RPC_ToAgent_AIN_Switch_Focus_0(x,y)
#define RPC_TOAGENT_AIN_MUTE(x,y,z)				AUDIO_RPC_ToAgent_AIN_Mute_0(x,y)
#define RPC_TOAGENT_ADC0_CONFIG(x,y,z)			AUDIO_RPC_ToAgent_ADC0_Config_0(x,y)
#define RPC_TOAGENT_ADC1_CONFIG(x,y,z)			AUDIO_RPC_ToAgent_ADC1_Config_0(x,y)
#define RPC_TOAGENT_ADC2_CONFIG(x,y,z)			AUDIO_RPC_ToAgent_ADC2_Config_0(x,y)
#define RPC_TOAGENT_AFC_CONFIG(x,y,z)			AUDIO_RPC_ToAgent_AFC_Config_0(x,y)
#define RPC_TOAGENT_SPDIF_CONFIG(x,y,z)               AUDIO_RPC_ToAgent_SPDIF_Config_0(x,y)
//#define RPC_TOAGENT_SAMPLE_CONFIG		AUDIO_RPC_ToAgent_Sample_Config_0
#define RPC_TOAGENT_SPDIF_ERR_THRESHOLD_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_SPDIF_Err_Threshold_Config_0(x,y)
#define RPC_TOAGENT_AUDIO_AI_BACKDOOR_INIT(x,y,z)	AUDIO_RPC_ToAgent_AI_BackDoor_Init_0(x,y)

#define RPC_TOAGENT_DAC_I2S_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_DAC_I2S_Config_0(x,y)
#define RPC_TOAGENT_AUDISTRY_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_Audistry_Config_0(x,y)
#define RPC_TOAGENT_DAC_SPDIF_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_DAC_SPDIF_Config_0(x,y)

#define RPC_TOAGENT_CONNECT(x,y,z)			AUDIO_RPC_ToAgent_Connect_0(x,y)
#define RPC_TOAGENT_DISCONNECT(x,y,z)
#define RPC_TOAGENT_DESTROY(x,y,z)			AUDIO_RPC_ToAgent_Destroy_0(x,y)
#define RPC_TOAGENT_RUN(x,y,z)			AUDIO_RPC_ToAgent_Run_0(x,y)
#define RPC_TOAGENT_PAUSE(x,y,z)			AUDIO_RPC_ToAgent_Pause_0(x,y)
#define RPC_TOAGENT_STOP(x,y,z)			AUDIO_RPC_ToAgent_Stop_0(x,y)
#define RPC_TOAGENT_GETSTATUS(x,y,z)		AUDIO_RPC_ToAgent_GetStatus_0(x,y)
#define RPC_TOAGENT_SEEK(x,y,z)			AUDIO_RPC_ToAgent_Seek_0(x,y)
#define RPC_TOAGENT_UPDATEWRITEPTR(x,y,z)	AUDIO_RPC_ToAgent_UpdateWritePtr_0(x,y)
#define RPC_TOAGENT_UPDATEREADPTR(x,y,z)	AUDIO_RPC_ToAgent_UpdateReadPtr_0(x,y)
#define RPC_TOAGENT_PRIVATEINFO(x,y,z)		AUDIO_RPC_ToAgent_PrivateInfo_0(x,y)

///////////////////////////////////////////////////////////////////////////////
#define RPC_TOAGENT_ENDOFSTREAM(x,y,z)		AUDIO_RPC_ToAgent_EndOfStream_0(x,y)
#define RPC_TOAGENT_RECEIVE(x,y,z)			AUDIO_RPC_ToAgent_Receive_0(x,y)

#define RPC_TOAGENT_FLUSH(x,y,z)           AUDIO_RPC_ToAgent_Flush_0(x,y)
#define RPC_TOAGENT_SET_SEEKING(x,y,z)     AUDIO_RPC_ToAgent_SetSeeking_0(x,y)

#define RPC_TOAGENT_DECODERCONFIG(x,y,z)	AUDIO_RPC_ToAgent_DecoderConfig_0(x,y)
#define RPC_TOAGENT_EXTBSEXIST(x,y,z)		AUDIO_RPC_ToAgent_ExtBSExist_0(x,y)
#define RPC_TOAGENT_PCMFORMAT(x,y,z)		AUDIO_RPC_ToAgent_PCMFormat_0(x,y)
#define RPC_TOAGENT_SWITCH_AGENT(x,y,z)	AUDIO_RPC_ToAgent_SwitchAgent_0(x,y)
#define RPC_TOAGENT_UPDATEPTS(x,y,z)		AUDIO_RPC_ToAgent_UpdatePTS_0(x,y)
#define RPC_TOAGENT_SETREFCLOCK(x,y,z)		AUDIO_RPC_ToAgent_SetRefClock_0	(x,y)
#define RPC_TOAGENT_INITRINGBUFFER_HEADER(x,y,z)	AUDIO_RPC_ToAgent_InitRingBufferHeader_0(x,y)
#define RPC_TOAGENT_SWITCHFOCUS(x,y,z)			AUDIO_RPC_ToAgent_SwitchFocus_0(x,y)
#define RPC_TOAGENT_HDMI_OUT_EDID(x,y,z)		AUDIO_RPC_ToAgent_HDMI_OUT_EDID_0(x,y)
#define RPC_TOAGENT_HDMI_OUT_VSDB(x,y,z)		AUDIO_RPC_ToAgent_HDMI_OUT_VSDB_0(x,y)
#define RPC_TOAGENT_HDMI_INFO(x,y,z)		AUDIO_RPC_ToAgent_HDMI_INFO_0(x,y)

#define RPC_TOAGENT_PP_FUNCTION(x,y,z)		AUDIO_RPC_ToAgent_PP_Config_0(x,y)
#define RPC_TOAGENT_PP_SRC_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_SRC_Config_0(x,y)
#define RPC_TOAGENT_PP_PL2_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_PL2_Config_0(x,y)
#define RPC_TOAGENT_PP_MIXER_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_MIXER_Config_0(x,y)
#define RPC_TOAGENT_PP_DVS_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_DVS_Config_0(x,y)
#define RPC_TOAGENT_PP_BASS_MANAGEMENT_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_BASS_MANAGEMENT_Config_0(x,y)
#define RPC_TOAGENT_PP_KEY_SHIFT_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_PP_KEY_SHIFT_Config_0(x,y)
#define RPC_TOAGENT_PP_REVERB_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_PP_REVERB_Config_0(x,y)
#define RPC_TOAGENT_PP_EQ_CONFIG(x,y,z)		AUDIO_RPC_ToAgent_PP_EQ_Config_0(x,y)
#define RPC_TOAGENT_PP_VOCAL_REMOVER_CONFIG(x,y,z)	AUDIO_RPC_ToAgent_PP_VOCAL_REMOVER_Config_0(x,y)

#define RPC_TOAGENT_NIGHTMODE(x,y,z)		AUDIO_RPC_ToAgent_NightMode_0(x,y)
#define RPC_TOAGENT_PP_INIT_PIN(x,y,z)		AUDIO_RPC_ToAgent_PP_InitPin_0(x,y)

#define RPC_TOAGENT_AI_DEVICE_USBINFO(x,y,z)    AUDIO_RPC_ToAgent_AI_Device_USBinfo(x,y)


#define RPC_TOAGENT_CREATE_SVC			AUDIO_RPC_ToAgent_Create_0_svc

#define RPC_TOAGENT_KARAOKE_CONTROL_SVC			AUDIO_RPC_ToAgent_Karaoke_Control_0_svc
#define RPC_AO_TOAGENT_MUTE_SVC				AUDIO_RPC_AO_ToAgent_Mute_0_svc
#define RPC_TOAGENT_AOUT_SPDIF_SOURCE_SVC		AUDIO_RPC_ToAgent_AOUT_SPDIF_Source_0_svc
#define RPC_TOAGENT_AOUT_VOLUME_CONTROL_SVC		AUDIO_RPC_ToAgent_AOUT_Volume_Control_0_svc
#define RPC_TOAGENT_AOUT_DROP_SAMPLE_SVC		AUDIO_RPC_ToAgent_AOUT_Drop_Sample_0_svc
#define RPC_TOAGENT_AOUT_PINK_WHITE_NOISE_SVC	AUDIO_RPC_ToAgent_AOUT_Pink_White_Noise_0_svc
#define RPC_TOAGENT_AOUT_SEND_SPECTRUM_DATA_SVC	AUDIO_RPC_ToAgent_AOUT_Send_Spectrum_Data_0_svc
#define RPC_TOAGENT_PLAYSOUNDEVENT_SVC			AUDIO_RPC_ToAgent_PlaySoundEvent_0_svc
#define RPC_TOAGENT_AOUT_HDMI_SET_SVC			AUDIO_RPC_ToAgent_AOUT_HDMI_Set_0_svc
#define RPC_TOAGENT_HDMI_MUTE_SVC				AUDIO_RPC_ToAgent_HDMI_Mute_0_svc
#define RPC_TOAGENT_AO_ONLY_SWITCHFOCUS_SVC		AUDIO_RPC_ToAgent_AO_Only_Switchfocus_0_svc

#define RPC_TOAGENT_AOUT_COPY_SOURCE_SVC		AUDIO_RPC_ToAgent_AOUT_Copy_Source_0_svc
#define RPC_TOAGENT_AIN_CONFIG_TSD_SVC		AUDIO_RPC_ToAgent_AIN_Config_TSD_0_svc
#define RPC_TOAGENT_AIN_DATA_MEASUREMENT_SVC	AUDIO_RPC_ToAgent_AIN_Data_Measurement_0_svc
#define RPC_TOAGENT_AIN_SWITCH_FOCUS_SVC		AUDIO_RPC_ToAgent_AIN_Switch_Focus_0_svc
#define RPC_TOAGENT_AIN_MUTE_SVC				AUDIO_RPC_ToAgent_AIN_Mute_0_svc
#define RPC_TOAGENT_ADC0_CONFIG_SVC			AUDIO_RPC_ToAgent_ADC0_Config_0_svc
#define RPC_TOAGENT_ADC1_CONFIG_SVC			AUDIO_RPC_ToAgent_ADC1_Config_0_svc
#define RPC_TOAGENT_ADC2_CONFIG_SVC			AUDIO_RPC_ToAgent_ADC2_Config_0_svc
#define RPC_TOAGENT_AFC_CONFIG_SVC			AUDIO_RPC_ToAgent_AFC_Config_0_svc
#define RPC_TOAGENT_SPDIF_CONFIG_SVC                    AUDIO_RPC_ToAgent_SPDIF_Config_0_svc
//#define RPC_TOAGENT_SAMPLE_CONFIG_SVC			AUDIO_RPC_ToAgent_Sample_Config_0_svc
#define RPC_TOAGENT_SPDIF_ERR_THRESHOLD_CONFIG_SVC	AUDIO_RPC_ToAgent_SPDIF_Err_Threshold_Config_0_svc
#define RPC_TOAGENT_AUDIO_AI_BACKDOOR_INIT_SVC		AUDIO_RPC_ToAgent_AI_BackDoor_Init_0_svc

#define RPC_TOAGENT_DAC_I2S_CONFIG_SVC			AUDIO_RPC_ToAgent_DAC_I2S_Config_0_svc
#define RPC_TOAGENT_AUDISTRY_CONFIG_SVC			AUDIO_RPC_ToAgent_Audistry_Config_0_svc
#define RPC_TOAGENT_DAC_SPDIF_CONFIG_SVC		AUDIO_RPC_ToAgent_DAC_SPDIF_Config_0_svc

#define RPC_TOAGENT_CONNECT_SVC			AUDIO_RPC_ToAgent_Connect_0_svc
#define RPC_TOAGENT_DISCONNECT_SVC
#define RPC_TOAGENT_DESTROY_SVC			AUDIO_RPC_ToAgent_Destroy_0_svc
#define RPC_TOAGENT_RUN_SVC				AUDIO_RPC_ToAgent_Run_0_svc
#define RPC_TOAGENT_PAUSE_SVC			AUDIO_RPC_ToAgent_Pause_0_svc
#define RPC_TOAGENT_STOP_SVC			AUDIO_RPC_ToAgent_Stop_0_svc
#define RPC_TOAGENT_GETSTATUS_SVC		AUDIO_RPC_ToAgent_GetStatus_0_svc
#define RPC_TOAGENT_SEEK_SVC			AUDIO_RPC_ToAgent_Seek_0_svc
#define RPC_TOAGENT_UPDATEWRITEPTR_SVC	AUDIO_RPC_ToAgent_UpdateWritePtr_0_svc
#define RPC_TOAGENT_UPDATEREADPTR_SVC	AUDIO_RPC_ToAgent_UpdateReadPtr_0_svc
#define RPC_TOAGENT_PRIVATEINFO_SVC		AUDIO_RPC_ToAgent_PrivateInfo_0_svc

///////////////////////////////////////////////////////////////////////////////
#define RPC_TOAGENT_ENDOFSTREAM_SVC		AUDIO_RPC_ToAgent_EndOfStream_0_svc
#define RPC_TOAGENT_RECEIVE_SVC			AUDIO_RPC_ToAgent_Receive_0_svc

#define RPC_TOAGENT_FLUSH_SVC           AUDIO_RPC_ToAgent_Flush_0_svc
#define RPC_TOAGENT_SET_SEEKING_SVC     AUDIO_RPC_ToAgent_SetSeeking_0_svc

#define RPC_TOAGENT_DECODERCONFIG_SVC	AUDIO_RPC_ToAgent_DecoderConfig_0_svc
#define RPC_TOAGENT_EXTBSEXIST_SVC		AUDIO_RPC_ToAgent_ExtBSExist_0_svc
#define RPC_TOAGENT_PCMFORMAT_SVC		AUDIO_RPC_ToAgent_PCMFormat_0_svc
#define RPC_TOAGENT_SWITCH_AGENT_SVC	AUDIO_RPC_ToAgent_SwitchAgent_0_svc
#define RPC_TOAGENT_UPDATEPTS_SVC		AUDIO_RPC_ToAgent_UpdatePTS_0_svc
#define RPC_TOAGENT_SETREFCLOCK_SVC		AUDIO_RPC_ToAgent_SetRefClock_0_svc
#define RPC_TOAGENT_INITRINGBUFFER_HEADER_SVC	AUDIO_RPC_ToAgent_InitRingBufferHeader_0_svc
#define RPC_TOAGENT_SWITCHFOCUS_SVC		AUDIO_RPC_ToAgent_SwitchFocus_0_svc
#define RPC_TOAGENT_HDMI_OUT_EDID_SVC		AUDIO_RPC_ToAgent_HDMI_OUT_EDID_0_svc
#define RPC_TOAGENT_HDMI_OUT_VSDB_SVC		AUDIO_RPC_ToAgent_HDMI_OUT_VSDB_0_svc
#define RPC_TOAGENT_HDMI_INFO_SVC		AUDIO_RPC_ToAgent_HDMI_INFO_0_svc

#define RPC_TOAGENT_PP_FUNCTION_SVC		AUDIO_RPC_ToAgent_PP_Config_0_svc
#define RPC_TOAGENT_PP_SRC_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_SRC_Config_0_svc
#define RPC_TOAGENT_PP_PL2_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_PL2_Config_0_svc
#define RPC_TOAGENT_PP_MIXER_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_MIXER_Config_0_svc
#define RPC_TOAGENT_PP_DVS_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_DVS_Config_0_svc
#define RPC_TOAGENT_PP_BASS_MANAGEMENT_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_BASS_MANAGEMENT_Config_0_svc
#define RPC_TOAGENT_PP_KEY_SHIFT_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_KEY_SHIFT_Config_0_svc
#define RPC_TOAGENT_PP_REVERB_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_REVERB_Config_0_svc
#define RPC_TOAGENT_PP_EQ_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_EQ_Config_0_svc
#define RPC_TOAGENT_PP_VOCAL_REMOVER_CONFIG_SVC	AUDIO_RPC_ToAgent_PP_VOCAL_REMOVER_Config_0_svc

#define RPC_TOAGENT_NIGHTMODE_SVC		AUDIO_RPC_ToAgent_NightMode_0_svc
#define RPC_TOAGENT_PP_INIT_PIN_SVC		AUDIO_RPC_ToAgent_PP_InitPin_0_svc

#define RPC_TOAGENT_AI_DEVICE_USBINFO_SVC   AUDIO_RPC_ToAgent_AI_Device_USBinfo_0_svc
#define RPC_TOAGENT_SET_CHANNEL_OUT_SWAP_SVC   AUDIO_RPC_ToAgent_SET_CHANNEL_OUT_SWAP_0_svc

#endif


#endif






