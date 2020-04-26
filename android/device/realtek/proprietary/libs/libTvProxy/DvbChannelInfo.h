#ifndef _DVB_CHANNEL_INFO_H_
#define _DVB_CHANNEL_INFO_H_


class DvbChannelInfo
{
public:
	DvbChannelInfo();
	~DvbChannelInfo();

	int m_logicalChNum;
	int m_userChNum;
	int m_frequency;
	int m_bandwidth;
	int m_type;
	int m_flags;
	int m_channelNameLen;
	char m_channelName[256];
};




#endif	// #define _DVB_CHANNEL_INFO_H_