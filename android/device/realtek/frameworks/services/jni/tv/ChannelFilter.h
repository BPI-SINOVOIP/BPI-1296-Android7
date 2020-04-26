#ifndef __CHANNEL_FILTER_H__
#define __CHANNEL_FILTER_H__

#include "types.h"

typedef unsigned char UINT8;

class CChannelNode;


class CChannelFilter
{
public:
    CChannelFilter() { }
    virtual ~CChannelFilter() { }


public:
    UINT8                   m_keepSelected:1;

    UINT8                   m_scrambled:1;
    UINT8                   m_isFake:1;
    UINT8                   m_nonTv:1;          // this filter is for any channel without audio/video.
    UINT8                   m_atv:1;            // this filter is for analog TV
    UINT8                   m_dtv:1;            // this filter is for dtv
    UINT8                   m_avDtv:1;          // this filter is for dtv with both video + audio.
    UINT8                   m_audioDtv:1;       // this filter is for dtv with only audio.
    UINT8                   m_hidden:1;
    UINT8                   m_hiddenGuide:1;
    UINT8                   m_oneSegment:1;
    UINT8                   m_nonPrimaryChannel:1;

    UINT8                   m_userDeleted:1;
    UINT8                   m_userFavorite:1;
    UINT8                   m_userSkipped:1;
    UINT8                   m_userLocked:1;
    UINT8                   m_userAdded:1;

};

#endif // __CHANNEL_FILTER_H__
