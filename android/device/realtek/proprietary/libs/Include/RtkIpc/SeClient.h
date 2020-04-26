/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * SeClient.h
 *
 *  Created on: Mar 18, 2014
 *      Author: charleslin
 */

#ifndef __RTK_SECLIENT_H__
#define __RTK_SECLIENT_H__

#include <utils/Vector.h>
#include <binder/BinderService.h>

#include "RtkIpc/ISeClient.h"

namespace android {

class SeClient :
    public BnSeClient
{
public:
    SeClient();
    ~SeClient();
    void setHandler(unsigned int handler) { mHandler = handler; }
    unsigned int getHandler(void) { return mHandler; }

private:
    unsigned mHandler;
};

} // namespace android

#endif //__RTK_SECLIENT_H__
