#ifndef ANDROID_INBANDSERVICE_H
#define ANDROID_INBANDSERVICE_H

#include "IRtkHWMBinderService.h"

namespace android {

class RtkHWMBinderService : public BnRtkHWMBinderService{
public:
    static void instantiate();
    status_t otpWriteFunc(char *cmd, int param1, int param2, char *value);
    status_t otpReadFunc(char *cmd, int param1, int param2, char *value);
    status_t cwKeyFunc(char *cmd, int param1, int param2, int param3, char *value);
    status_t aesEcbEnFunc(char *cmd, char *key, int param2, int param3, char *value);
    status_t dpaWriteFunc(char *cmd, int param1, int param2, int param3);
private:
    RtkHWMBinderService();
    ~RtkHWMBinderService();

};

}

#endif
