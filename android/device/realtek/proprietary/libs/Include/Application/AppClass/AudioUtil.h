#ifndef _SYSTEM_AUDIO_UTIL_
#define _SYSTEM_AUDIO_UTIL_

#include <Util.h>
#include "OSAL.h"
#include <Application/AppClass/setupdef.h>
#include "AudioRPC_System.h"

#include "StreamClass/SType.h"
#include "StreamClass/EType.h"
#include <AudioRPCBaseDS_data.h>

// Play WAV sound file
typedef struct _RIFFHEADER
{
    uint32_t fcc;
    uint32_t cb;
    uint32_t format;
} RIFFHEADER;

typedef struct _WAV_FMT
{
    unsigned short FmtTag;
    unsigned short Channels;
    unsigned int   SamplesPerSec;
    unsigned int   AvgBytesPerSec;
    unsigned short BlockAlign;
    unsigned short BitsPerSample;
    unsigned short Optional;
} WAV_FMT;

typedef struct _riffchunk {
    uint32_t fcc;
    uint32_t  cb;
} RIFFCHUNK;

#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |     \
                  (((uint32_t)(ch4) & 0xFF00) << 8) |    \
                  (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
                  (((uint32_t)(ch4) & 0xFF000000) >> 24))


#ifdef __cplusplus
extern "C" {
#endif

// General Stuff
HRESULT SetAudioSpeakerMode(ENUM_SPEAKER_OUT mode);
HRESULT SetAudioAGCMode(ENUM_AUDIO_AGC_MODE mode);
HRESULT SetAudioOutputNch(ENUM_AUDIO_OUTPUT_CHANNEL channel);
HRESULT SetAudioDrcCutRoostValue(AUDIO_DRC_VALUES values);
HRESULT SetAudioTrueHdLosslessMode(ENUM_AUDIO_TRUEHD_LOSSLESSMODE_CFG mode);
HRESULT SetAudioDolbyScaleHILO(long scalehi, long scalelo);
HRESULT SetAudioDolbyCompressMode(long mode);
HRESULT SetAudioDolbyDownmixMode(long mode);
HRESULT SetAudioSPDIFMode(ENUM_SPDIF_MODE mode);
HRESULT SetAudioSpdifDelay(ENUM_AUDIO_DIGITAL_OUTPUT_MODE mode, int delayInMS);
HRESULT SetAudioHdmiOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE mode);
HRESULT SetAudioHdmiFreqMode(void);
HRESULT SetAudioSpdifOutputMode(ENUM_AUDIO_DIGITAL_OUTPUT_MODE mode);
HRESULT SetAudioDacSwitchMode (long mode);
HRESULT SetAudioVolume(int volume);
HRESULT SetAudioMute(bool bMute);
HRESULT SetAudioDelayRPMode(int mode);
HRESULT PlayAudioTone(AUDIO_SOUND_EVENT event);
HRESULT PlaySoundEvent(char *path);
HRESULT SetAudioHdmiMute(bool bMute);
HRESULT SetAudioHdmiRxVolume(int volume, long aoId, long aiId, long pinId);
HRESULT SetHDMIRXtoBTUsb(int flag, long aiId);

// Karaoke Stuff [Currently Enabled]
HRESULT SetAudioMicConfig(AUDIO_AO_MIC_CONFIG mode);
HRESULT SetAudioMic1Volume(int volume);
HRESULT SetAudioMic2Volume(int volume);
HRESULT SetAudioMicEchoReverb(AUDIO_AO_ECHO_REVERB_CONFIG mode,
				AUDIO_AO_ECHO_DELAY_MODE echo_delay,
				AUDIO_VOLUME_LEVEL echo_fdbk_gain,
				AUDIO_VOLUME_LEVEL echo_volume,
				ENUM_REVERB_MODE reverb_mode,
				AUDIO_VOLUME_LEVEL reverb_volume,
				AUDIO_VOLUME_LEVEL bypass_volume);
HRESULT SetAudioPPEqualizer(ENUM_EQUALIZER_ENA mode, AUDIO_RPC_EQUALIZER_MODE info);
HRESULT SetAudioDecVolume(int volume);

// Karaoke Stuff [Currently Disabled]
HRESULT SetAudioBassManagement(ENUM_BASS_MANAGEMENT_ENA mode, AUDIO_RPC_BASS_MANAGEMENT_MODE info);
HRESULT SetAudioAuxVolume(int volume);
HRESULT SetAudioAuxConfig(AUDIO_AO_AUX_CONFIG mode);
HRESULT SetAudioAuxEqualizer(ENUM_EQUALIZER_ENA mode, AUDIO_RPC_EQUALIZER_MODE info);

#define FORCE_DTS_2CH      (1 << 0)
#define FORCE_DTSHD_2CH    (1 << 1)
#define FORCE_AC3_2CH      (1 << 2)
#define FORCE_DDP_2CH      (1 << 3)
#define FORCE_MLP_2CH      (1 << 4)
#define FORCE_AAC_2CH      (1 << 5)
#define FORCE_WMAPRO_2CH   (1 << 6)
HRESULT SetAudioForceChannelCtrl(int ctrlBits);

#define ECHO_STRENGTH_MAX 7
#define ECHO_STRENGTH_MIN 0
// Karaoke Stuff [Encapsulated]
HRESULT SetAudioMicEcho(int strength);
HRESULT SetAudioMicReverb(bool bEnable, ENUM_REVERB_MODE mode);
HRESULT CheckAudioFormatParserCapability(ENUM_MEDIA_TYPE type);
HRESULT SetAudioMpgSpdifMode(bool raw);
HRESULT GetAudioDecoderCapability(uint64_t *capability);
HRESULT SetAudioKaraokeMode(AUDIO_KARAOKE_MODE kmode);
HRESULT SetAudioSurroundSoundMode(long ssMode);
HRESULT SetAudioNativeSR(int isNativeSR);

HRESULT GetAudioVolume(int* pVolume);
HRESULT GetAudioMute(int* pbMute);

HRESULT SetAudioPlaybackRate(long min, long max);

HRESULT SetAudioLowDelayParam(unsigned int queue_ms, unsigned int flush_ms);
HRESULT SetAudioJitterMs(unsigned int jitter_ms);

// for miracast HDCP
HRESULT SetAudioSPDIFOutputSwitch(bool bIsOn);
HRESULT SetAudioAnalogOutputSwitch(bool bIsOn);

HRESULT SetAudioI2SDelay(int delayInMS);
HRESULT SetAudioDelay(int delayInMS);

#ifdef __cplusplus
}
#endif

#endif
