/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * ISeClient.h
 *
 *  Created on: Mar 18, 2014
 *      Author: charleslin
 */

#ifndef ISECLIENT_H_
#define ISECLIENT_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class ISeClient: public IInterface
{
public:
    DECLARE_META_INTERFACE(SeClient);
};

// ----------------------------------------------------------------------------

class BnSeClient: public BnInterface<ISeClient>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif /* ISECLIENT_H_ */
