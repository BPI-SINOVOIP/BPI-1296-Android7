//Copyright (C) 2007-2013 Realtek Semiconductor Corporation.
#ifndef __CEC_RPC_H__
#define __CEC_RPC_H__


#include <soc/realtek/kernel-rpc.h>

#include "uapi/ion.h"
#include "ion/ion.h"
#include "uapi/rtk_phoenix_ion.h"

typedef enum
{
    ENUM_KERNEL_RPC_CREATE_AGENT,
    ENUM_KERNEL_RPC_INIT_RINGBUF,
    ENUM_KERNEL_RPC_PRIVATEINFO,
    ENUM_KERNEL_RPC_RUN,
    ENUM_KERNEL_RPC_PAUSE,
    ENUM_KERNEL_RPC_SWITCH_FOCUS,
    ENUM_KERNEL_RPC_MALLOC_ADDR,
    ENUM_KERNEL_RPC_VOLUME_CONTROL,
    ENUM_KERNEL_RPC_FLUSH,
    ENUM_KERNEL_RPC_CONNECT,
    ENUM_KERNEL_RPC_SETREFCLOCK,
    ENUM_KERNEL_RPC_DAC_I2S_CONFIG,
    ENUM_KERNEL_RPC_DAC_SPDIF_CONFIG,
    ENUM_KERNEL_RPC_HDMI_OUT_EDID,
    ENUM_KERNEL_RPC_HDMI_OUT_EDID2,
    ENUM_KERNEL_RPC_HDMI_SET,
    ENUM_KERNEL_RPC_HDMI_MUTE,
    ENUM_KERNEL_RPC_ASK_DBG_MEM_ADDR,
    ENUM_KERNEL_RPC_DESTROY,
    ENUM_KERNEL_RPC_STOP,
    ENUM_KERNEL_RPC_CHECK_READY,   // check if Audio get memory pool from AP
    ENUM_KERNEL_RPC_GET_MUTE_N_VOLUME,   // get mute and volume level
    ENUM_KERNEL_RPC_EOS,
    ENUM_KERNEL_RPC_ADC0_CONFIG,
    ENUM_KERNEL_RPC_ADC1_CONFIG,
    ENUM_KERNEL_RPC_ADC2_CONFIG,
#if defined(AUDIO_TV_PLATFORM)
    ENUM_KERNEL_RPC_BBADC_CONFIG,
    ENUM_KERNEL_RPC_I2SI_CONFIG,
    ENUM_KERNEL_RPC_SPDIFI_CONFIG,
#endif // AUDIO_TV_PLATFORM
    ENUM_KERNEL_RPC_HDMI_OUT_VSDB,
    ENUM_VIDEO_KERNEL_RPC_CONFIG_TV_SYSTEM,
    ENUM_VIDEO_KERNEL_RPC_CONFIG_HDMI_INFO_FRAME,
} ENUM_AUDIO_KERNEL_RPC_CMD;


enum AUDIO_ENUM_PRIVAETINFO
{
    ENUM_PRIVATEINFO_AUDIO_FORMAT_PARSER_CAPABILITY, /* input: noneed instanceID privateInfo[0]=AUDIO_DEC_TYPE  output: privateInfo[0]=0 for fail, 1 for ok */
    ENUM_PRIVATEINFO_AUDIO_DECODER_CAPABILITY,       /* no input, output: privateInfo[0] = low 32  privateInfo[1] = high 32  of AudioCodecControlBit */
    ENUM_PRIVATEINFO_AUDIO_CONFIG_CMD_BS_INFO,
    ENUM_PRIVATEINFO_AUDIO_CHECK_LPCM_ENDIANESS,      /* input: noneed instanceID privateInfo[0]=buf privateInfo[1]=size privateInfo[2]:chnum privateInfo[3]:bitwidth output: privateInfo[0]= LPCM_ENDIANESS */
    ENUM_PRIVATEINFO_AUDIO_CONFIG_CMD_AO_DELAY_INFO,  /*{privateInfo[0] =LEFT_delay_en,
														privateInfo[1] =LEFT_delay_ms,
														privateInfo[2] =CNTR_delay_en,
														privateInfo[3] =CNTR_delay_ms,
														privateInfo[4] =RGHT_delay_en,
														privateInfo[5] =RGHT_delay_ms,
														privateInfo[6] =LSUR_delay_en,
														privateInfo[7] =LSUR_delay_ms,
														privateInfo[8] =RSUR_delay_en,
														privateInfo[9] =RSUR_delay_ms,
														privateInfo[10] =LFE_delay_en,
														privateInfo[11] =LFE_delay_ms} */
    ENUM_PRIVATEINFO_AUDIO_AO_CHANNEL_VOLUME_LEVEL,   /* input: noneed instanceID ,
                                                        privateInfo[0]= Left Front              channel volume level
                                                        privateInfo[1]= Right Front             channel volume level
                                                        privateInfo[2]= Left Surround Rear      channel volume level
                                                        privateInfo[3]= Right Surround Rear     channel volume level
                                                        privateInfo[4]= Center Front            channel volume level
                                                        privateInfo[5]= LFE                     channel volume level
                                                        privateInfo[6]= Left Outside Front      channel volume level
                                                        privateInfo[7]= Right Outside Front     channel volume level */
    ENUM_PRIVATEINFO_AUDIO_GET_FLASH_PIN,       /* request flash pin ID */
    ENUM_PRIVATEINFO_AUDIO_RELEASE_FLASH_PIN,    /* release flash pinID */
    ENUM_PRIVATEINFO_AUDIO_GET_MUTE_N_VOLUME,     /*  privateInfo[0] = mute flag ,privateInfo[1] = volume level */
    ENUM_PRIVATEINFO_AUDIO_AO_MONITOR_FULLNESS,   /* privateInfo[0]= 0 disable or 1 enable ao to monitor queue fullness and do add drop sample */
    ENUM_PRIVATEINFO_AUDIO_CONTROL_FLASH_VOLUME,    /* control the volume of flash pin */
    ENUM_PRIVATEINFO_AUDIO_CONTROL_DAC_SWITCH, /* decide whether DAC switch */
    ENUM_PRIVATEINFO_AUDIO_PREPROCESS_CONFIG, /*privateInfo[0]=preprocess enable
												privateInfo[1]=re-sync enable
												privateInfo[2]=acoustic echo cancelling(AEC) enable
												privateInfo[3]=beamforming enable
												privateInfo[4]=noise suppresion enable
												privateInfo[5]=shared_mem_addr_for_mic_and_AI_DEVICE*/
	ENUM_PRIVATEINFO_AUDIO_CHECK_SECURITY_ID,
	ENUM_PRIVATEINFO_AUDIO_LOW_DELAY_PARAMETERS,
	ENUM_PRIVATEINFO_AUDIO_SET_NETWORK_JITTER,   /* for WFD case */
	ENUM_PRIVATEINFO_AUDIO_GET_QUEUE_DATA_SIZE,  /* get audio PCM in ringbuf(decoder&PP&AO) */
    ENUM_PRIVATEINFO_AUDIO_GET_SHARE_MEMORY_FROM_ALSA,
    ENUM_PRIVATEINFO_AUDIO_AI_CONNECT_ALSA,
    ENUM_PRIVATEINFO_AUDIO_SET_PCM_FORMAT,  /* for DEC_SRC_OUT_EN, privateInfo[0]= sample rate, privateInfo[1]= chnum */
    ENUM_PRIVATEINFO_AUDIO_DO_SELF_DESTROY_FLOW, /* for DvdPlayer crash, signal audio to do self destroy flow, privateInfo[0]= sysPID */
	ENUM_PRIVATEINFO_AUDIO_GET_SAMPLING_RATE,
	ENUM_PRIVATEINFO_AUDIO_SLAVE_TIMEOUT_THRESHOLD,
	ENUM_PRIVATEINFO_AUDIO_GET_GLOBAL_AO_INSTANCEID,
	ENUM_PRIVATEINFO_AUDIO_SET_CEC_PARAMETERS
};

struct AUDIO_RPC_PRIVATEINFO_PARAMETERS
{
 int instanceID;
 enum    AUDIO_ENUM_PRIVAETINFO type;
 int privateInfo[16];
};

#endif  //__CEC_RPC_H__

