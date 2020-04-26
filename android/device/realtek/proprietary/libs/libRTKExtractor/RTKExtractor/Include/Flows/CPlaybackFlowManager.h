#ifndef __C_PLAYBACK_FLOW_MANAGER_H__
#define __C_PLAYBACK_FLOW_MANAGER_H__

#include <StreamClass/CFlowManager.h>
#include <Filters/NavigationFilter/NavDef.h>

class CPlaybackFlowManager: public CFlowManager
{
public:

    CPlaybackFlowManager(bool bNeedRefClock=false);
    virtual ~CPlaybackFlowManager();

protected:

    enum { RECEIVE_VIDEO_EOS    = 0x00000001,
           RECEIVE_AUDIO_EOS    = 0x00000002,
           RECEIVE_INDEXGEN_EOS = 0x00000004  };

    NAVAUTOSTOPINFO m_autoStopInfo;
    unsigned int    m_receivedEOS;

    virtual void HandleDefaultEvents(
        EVCode evCode,
        long*  pParams,
        long   paramSize
    );

    virtual bool HandleCriticalEvents(
        EVCode evCode,
        long*  pParams,
        long   paramSize
    );

    virtual bool HasDefaultHandling(
        EVCode evCode
    );

    void CheckFlowStopCondition();
};

#endif
