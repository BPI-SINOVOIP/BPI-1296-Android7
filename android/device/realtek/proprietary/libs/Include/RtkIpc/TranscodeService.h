#ifndef RTK_TRANSCODE_SERVICE_H
#define RTK_TRANSCODE_SERVICE_H

#include <utils/Singleton.h>
#include <utils/StrongPointer.h>
#include "RtkIpc/IRtkTranscodePlayerService.h"

namespace android {

class IRtkTranscodePlayerService;


/* This is a simple class to create connection with Platform TranscodeService,
 * currently, it is RtkTranscodePlayerService on Realtek device.
 */
class TranscodeService : public Singleton<TranscodeService>
{
    sp<IRtkTranscodePlayerService> mTranscodeService;
    sp<IBinder::DeathRecipient> mDeathObserver;
    Mutex mLock;

    TranscodeService();
    void connectLocked();
    void transcodeServiceDied();
    friend class Singleton<TranscodeService>;

public:
    static sp<IRtkTranscodePlayerService> getTranscodeService();

};

}; // namespace android

#endif // RTK_TRANSCODE_SERVICE_H
