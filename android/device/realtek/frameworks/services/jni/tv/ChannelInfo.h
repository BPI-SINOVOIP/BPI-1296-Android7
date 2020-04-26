#ifndef __CHANNEL_INFO_H__
#define __CHANNEL_INFO_H__


typedef enum _RT_TV_SYSTEM {

	RT_TV_SYSTEM_UNKNOWN = 0,
	RT_TV_SYSTEM_NTSC,
	RT_TV_SYSTEM_PAL,
	RT_TV_SYSTEM_PAL_N,
	RT_TV_SYSTEM_SECAM,
	RT_TV_SYSTEM_ATSC,
	RT_TV_SYSTEM_DVB,
	RT_TV_SYSTEM_MAX

} RT_TV_SYSTEM;


typedef enum _RT_SERVICE_TYPE {

	RT_SERVICE_TYPE_UNKNOWN = 0,
	RT_SERVICE_TYPE_ATV,
	RT_SERVICE_TYPE_DTV,
	RT_SERVICE_TYPE_AUDIO,
	RT_SERVICE_TYPE_DATA,
	RT_SERVICE_TYPE_MAX

} RT_SERVICE_TYPE;


class ChannelInfo {
public:
    unsigned int iChNum;
    unsigned int iChIndex;
    unsigned int iFreq;
    RT_TV_SYSTEM TvSystem;
    RT_SERVICE_TYPE ServiceType;
	bool m_bAfcEnable;
};

#endif // __CHANNEL_INFO_H__
