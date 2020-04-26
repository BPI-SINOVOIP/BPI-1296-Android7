#include <cstring>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <cutils/compiler.h>
#include "HDCPMessage.h"

void HDCPMessage::updataHDCPInfo(Client * client, bool isHDCP) {
    mHDCP = isHDCP;
}
