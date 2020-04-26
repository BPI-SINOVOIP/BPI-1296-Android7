#ifndef __HDMI_DEFINE__
#define __HDMI_DEFINE__

#include <Application/AppClass/setupdef.h>
#include "../../IPC/generate/include/system/VideoRPC_System.h"
#include <Platform_Lib/HDMIControl/HDMITVSystemTypes.h>


#define be32_to_cpu(x)	do {x = __swap32((x));} while(0)

#define HDCP_VERSION_22                     (22)
#define HDCP_VERSION_14                     (14)
#define HDCP_VERSION_NONE                   ( 0)
#define HDCP_VERSION_DEFAULT                (-1)
#define HDCP_VERSION_NA                     (-2)

enum MEDIA_LINK_MODE {
	MEDIA_LINK_MODE_UNDEFINED = 0,
	MEDIA_LINK_MODE_HDMI = 1,
	MEDIA_LINK_MODE_MHL = 2
};

enum LINK_MODE {
	LINK_MODE_UNKNOWN = 0,
	LINK_HDMI_DISABLED = 1,
	LINK_HDMI_ENABLED = 2,
	LINK_MHL_ENABLED = 3,
	LINK_MODE_MAX
};

struct HDMI_Audio_desc
{
	unsigned char	coding_type;
	unsigned char	channel_count;
	unsigned char	sample_freq_all;
	unsigned char	sample_size_all;
	unsigned char	max_bit_rate_divided_by_8KHz;
};

struct HDMI_EDID_data
{
	int			VDB_length;	// Video Data Block
	unsigned char	VDB[132];
	char			ADB_length;	// Audio Data Block
	struct HDMI_Audio_desc	*ADB;
	unsigned char		*VSDB;
};

struct HDMI_TvSystemAndStandard
{
	ENUM_VIDEO_SYSTEM       videoSystem;
	ENUM_VIDEO_STANDARD     videoStandard;
	int                     bIsWidescreen;
};

struct HDMI_Setting {
	ENUM_VIDEO_SYSTEM       video_system;
	ENUM_VIDEO_STANDARD     video_standard;
	int	format3D;
	float fps;
};

struct DPTX_Setting {
	ENUM_VIDEO_SYSTEM       video_system;
	ENUM_VIDEO_STANDARD     video_standard;
	VO_INTERFACE_TYPE	videoType;
};

typedef enum CHECK_NAND_TYPE {
	UPDATE_NAND_CHECK_ONLY 			= 0,
	UPDATE_NAND_CHECK_WITH_WRITE 	= 1,
	UPDATE_NAND_WRITE_ONLY 		= 2,
	UPDATE_NAND_READ_ONLY 		= 3
} UPDATE_NAND_TYPE;

struct HDMI_EDID_Info
{
	int 		hdmiVersionMajor;
	int			hdmiVersionMinor;
	char		TV_Name[4];
	//u8 			prod_code[2];;
	//u32 		serial;
	//u8 			mfg_week;
	//u8 			mfg_year;
	int 		deepColor420;
	int 		maxTMDS;
};

bool getHDCPEnabled(void);
void setHDCPEnabled(bool state);
char getHDCPEnabled_by_AP(void);
void setHDCPEnabled_by_AP(char state);
bool getHDCPEnabled_by_MakeConfig(void);
void setHDCPEnabled_by_MakeConfig(bool state);
char getHDMISwitchTVSystem(void);
void setHDMISwitchTVSystem(char state);
int getHDMIPlugged(void);
void setHDMIPlugged(int state);
char getHDMIReady(void);
void setHDMIReady(char state);
char getSinkState(void);
void setSinkState(char state);
void setHDMIVideoColorMode(char state);
char getHDMIDeepColorMode(void);
void setHDMIDeepColorMode(char state);
char getHDMIAuthetication(void);
void setHDMIAuthetication(char state);
char getHDMIStatus(void);
void setHDMIStatus(char state);
int getHDMILipSyncMode(void);
void setHDMILipSyncMode(int state);
int getSinkHDCPVersion();
bool setManualHDCPVersion(int version);

char getInvalidEDIDFound(void);
char getHDMIKeyFound(void);
char getInvalidHDMIKeyFound(void);
char getHDMICapacity(void);
char getHDMIEDIDReady(void);

int getTVMaxLuminace(void);

char getTV3DCapability(void);
void getTVSupport3D(unsigned char _3D_capability[2]);
void setTVSupport3D(unsigned char _3d_structure_high, unsigned char _3d_structure_low);
void getTVSupport3DResolution(unsigned char* _3D_capability_resolution);
void getTVSpecific3D(unsigned char* _Specific_3D);


int ReloadHDMI(void);

char getHDMILatency(unsigned char* HDMILatency);
char getHDMIAuth(void);
int getHDMILipSyncMode(void);
void setHDMIDeepColorState(int state);

int HDMI_SetHDCPState(char state);
int HDMI_GetVideoFormat(int *len, struct HDMI_TvSystemAndStandard *pTvSS);
int HDMI_GetNextVideoFormat(struct HDMI_TvSystemAndStandard *pTvSS);
int HDMI_GetNextVideoFormat_N(struct HDMI_TvSystemAndStandard *pTvSS,int next);
int HDMI_SetAudioFormat(void);
int HDMI_SetVideoFormatByTvSystem2(VIDEO_RPC_VOUT_CONFIG_VIDEO_STANDARD structSetVideoStandard, ENUM_VIDEO_SYSTEM videoSystem, ENUM_VIDEO_STANDARD videoStandard,
								VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig=NULL,VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig=NULL,bool bUpdateVideo=true);
int HDMI_GetAutoVideoFormat(struct HDMI_TvSystemAndStandard *pTvSS, bool lowResfirst=false);
char HDMI_CheckVideoFormat(void);
unsigned char HDMI_CheckAutoVideoFormat(int referenceVid);
int HDMI_SetHDMIAudioVideoFormat(void);
int HDMI_UpdateVDB(void);
int HDMI_GetAudioFormatEx(int *len, struct HDMI_Audio_desc *pAudioDesc);
char getWiDiMode(void);
void setWiDiMode(char state);
void HDMI_TurnOffVideo(void) ;
unsigned short getHDMISourceAddress(void);
bool checkNandTVSystem(VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig, UPDATE_NAND_TYPE updateType);
bool writeNandEDIDCheckSum(unsigned char checksum1, unsigned char checksum2);

void setUtilMediaLinkMode(int mode);
int getUtilMediaLinkMode(void);

bool isTVSupport3d(void);

int enableRCPThread(unsigned char OnOff);
int HDMI_ConvertVOtoVid(enum VO_STANDARD standard, u_char enProg);
void updateHDMISetting(int videoSystem, int videoStandard);
bool IsCurrentHDMI3D();
int CheckTVSystem(int videoSystem, int videoStandard, int color_space, int deep_color);


void updateDPTXSetting(int videoSystem, int videoStandard, int videoType);

#if defined(__LINARO_SDK__)
int HDMI_SetVideoFormatByTvSystem_woSetup(VIDEO_RPC_VOUT_CONFIG_VIDEO_STANDARD structSetVideoStandard,
		ENUM_VIDEO_ID_CODE vic, ENUM_COLOR_FORMAT colorFormat, ENUM_DEEP_COLOR deepColor, ENUM_COLOR_SPACE colorSpace);
void applyLipSync_woSetup(int vic, int HDMILipSyncMode);
#endif
int HDMI_GetCurrentVideoFormat(ENUM_VIDEO_ID_CODE *vic, ENUM_COLOR_FORMAT *colorFormat, ENUM_DEEP_COLOR *deepColor, ENUM_COLOR_SPACE *colorSpace);
int HDMI_GetCurrentEDIDFormat(HDMI_EDID_Info *pInfo);
bool Is420(ENUM_VIDEO_SYSTEM videosystem);
#endif
