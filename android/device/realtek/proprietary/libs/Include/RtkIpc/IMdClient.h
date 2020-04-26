/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 * IMdClient.h
 *
 *  Created on: June 25, 2015
 *      Author: calvinkuo
 */

#ifndef IMDCLIENT_H_
#define IMDCLIENT_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class IMdClient: public IInterface
{
public:
    DECLARE_META_INTERFACE(MdClient);
};

// ----------------------------------------------------------------------------

class BnMdClient: public BnInterface<IMdClient>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif /* IMDCLIENT_H_ */
