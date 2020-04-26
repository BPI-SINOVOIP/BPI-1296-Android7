/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * SeClient.h
 *
 *  Created on: Mar 18, 2014
 *      Author: charleslin
 */

#ifndef __RTK_MDCLIENT_H__
#define __RTK_MDCLIENT_H__

#include <utils/Vector.h>
#include <binder/BinderService.h>

#include "RtkIpc/IMdClient.h"

namespace android {

class MdClient :
    public BnMdClient
{
public:
    MdClient();
    ~MdClient();
    void setHandler(unsigned int handler) { mHandler = handler; }
    unsigned int getHandler(void) { return mHandler; }

private:
    unsigned mHandler;
};

} // namespace android

#endif //__RTK_MDCLIENT_H__
