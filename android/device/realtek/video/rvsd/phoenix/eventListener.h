#ifndef _RTK_EVENT_LISTENER_H_
#define _RTK_EVENT_LISTENER_H_

#include <utils/RefBase.h>

namespace android {

enum {
    RTK_DECODER_READY = 1,
    RTK_FIRST_FRAME_READY = 2,
    RTK_DECODER_RELEASED = 3,
    RTK_RVSD_THREAD_READY = 4,
    RKT_RVSD_VIDEO_SIZE_EXCEED_LIMITATION = 5,
};
#if !defined(__LINARO_SDK__) || defined(RTMEDIAPLAYER_CPP_ONLY)
static const char* RTKNotifyNames[RKT_RVSD_VIDEO_SIZE_EXCEED_LIMITATION] = { 
    " ", // = 0
    "RTK_DECODER_READY",// = 1,
    "RTK_FIRST_FRAME_READY",// = 2,
    "RTK_DECODER_RELEASED",// = 3,
    "RTK_RVSD_THREAD_READY"// = 4,
};
#endif
class eventListener : public RefBase {
public:
    eventListener() {};
    virtual ~eventListener() {};

    virtual void notifyListener(int msg, int ext1 = 0, int ext2 = 0) = 0;

private:
    eventListener(const eventListener &);
    eventListener &operator=(const eventListener&);
};

} // namespace android
#endif // _RTK_EVENT_LISTENER_H_
