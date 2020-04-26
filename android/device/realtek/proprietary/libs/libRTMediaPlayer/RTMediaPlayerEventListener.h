#ifndef _RTMEDIAPLAYER_EVENT_LISTENER_H_
#define _RTMEDIAPLAYER_EVENT_LISTENER_H_

#include <eventListener.h>

namespace android {

class RTMediaPlayer;

class RTMediaPlayerEventListener : public eventListener {
public:
    RTMediaPlayerEventListener();
    virtual ~RTMediaPlayerEventListener();
    virtual void setListener(const wp<RTMediaPlayer> &listener);
    virtual void notifyListener(int msg, int ext1 = 0, int ext2 = 0);

private:
    wp<RTMediaPlayer> mPlayer;

    RTMediaPlayerEventListener(const RTMediaPlayerEventListener &);
    RTMediaPlayerEventListener &operator=(const RTMediaPlayerEventListener &);
};

} // namespace android 
#endif // _RTMEDIAPLAYER_EVENT_LISTENER_H_
