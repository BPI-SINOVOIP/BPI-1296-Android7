#ifndef RTKTRANSCODEPLAYERSERVICE_H
#define RTKTRANSCODEPLAYERSERVICE_H

#include "RtkIpc/IRtkTranscodePlayerService.h"

namespace android {

class RtkTranscodePlayerService: public BnRtkTranscodePlayerService
{
public:
    RtkTranscodePlayerService();
    ~RtkTranscodePlayerService();

    static void instantiate();

    sp<IRtkTranscodePlayerServiceClient> createConnection(int onlyNo);
    bool disconnect();
};

}; // namespace android

#endif // RTKTRANSCODEPLAYERSERVICE_H
