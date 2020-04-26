/*
 * Copyright 2016 Realtek Semiconductor crop
 */

#ifndef RTK_IPLAYERLISTENER_H
#define RTK_IPLAYERLISTENER_H

#include <binder/IInterface.h>

#include <utils/RefBase.h>
#include <media/stagefright/foundation/AHandler.h>

namespace android {

// event type [Player -> demo program]
enum {
    EVENT_TYPE_EOS,
    EVENT_TYPE_ERROR,
    EVENT_TYPE_CAM_ERROR,
};

enum {
    ID_NONE,
    // extend by request...
};

class PlayerListener : public virtual RefBase
{
public:
    PlayerListener() {}
    virtual ~PlayerListener() {}

    virtual void onSendMessage(int type, int id) = 0; // Asynchronous
    virtual void registerStatusHandler(sp<AHandler> id) = 0;
};

class IPlayerListener : public PlayerListener, public IInterface
{
public:
    DECLARE_META_INTERFACE(PlayerListener)
};

// BnInterface
class BnPlayerListener : public BnInterface<IPlayerListener>
{
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data,
            Parcel* reply, uint32_t flags = 0);

    void registerStatusHandler(sp<AHandler> id) { mStatusId = id; };

protected:
    sp<AHandler> mStatusId;
};

} // namespace android

#endif
