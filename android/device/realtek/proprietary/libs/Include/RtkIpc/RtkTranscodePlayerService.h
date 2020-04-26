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

    sp<IRtkTranscodePlayerServiceClient> create(int onlyTrack);
    bool disconnect();
    int getClientsCount();

    void increaseClientCount();
    void decreaseClientCount();

private:
    int mNumsOfClient;
};

}; // namespace android

#endif // RTKTRANSCODEPLAYERSERVICE_H
