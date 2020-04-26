/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTKFILTER_H_
#define _RTKFILTER_H_

namespace android {

struct ALooper;
struct AHandler;

struct RtkFilter : public AHandler
{
public:
    RtkFilter();
    void registerStatusHandler(sp<AHandler> handler) { mStatusId = handler; };
    void registerManagerHandler(sp<AHandler> handler) { mManagerId = handler; };
    void registerSourceHandler(sp<AHandler> handler) { mSourceId= handler; };
    void registerSinkHandler(sp<AHandler> handler) { mSinkId = handler; };

    void cleanup();

protected:
    sp<AHandler> mSourceId;
    sp<AHandler> mSinkId;
    sp<AHandler> mManagerId;
    sp<AHandler> mStatusId;

private:
    DISALLOW_EVIL_CONSTRUCTORS(RtkFilter);
};

} // namespace android

#endif //_RTKFILTER_H_
