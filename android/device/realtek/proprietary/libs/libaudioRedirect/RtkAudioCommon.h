/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _AUDIOCOMMON_H
#define _AUDIOCOMMON_H

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/AudioTrack.h>

namespace android {

#define AMESSAGE_REDIRECT_ERR_STATUS "err"
#define ABUFFER_REDIRECT_BUFFER      "buffer"
#define AMESSAGE_AUDIO_CHANNELS      "channels"
#define AMESSAGE_AUDIO_SAMPLERATE    "samplerate"
#define AMESSAGE_AUDIO_IS_RENDER     "isRender"
#define AMESSAGE_AUDIO_MODE          "mode"
#define AMESSAGE_AUDIO_DELAY         "delay"
#define AMESSAGE_AUDIO_CARD          "card"
#define AMESSAGE_AUDIO_DEVICES       "devices"

enum
{
    AUDIO_BT_CONNECTED = 0,
    AUDIO_USB_CONNECTED
};

enum
{
    AUDIO_RENDER_NORMARL_MODE = 0, //use AudioTrack
    AUDIO_RENDER_BYPASS_MODE       //passthrough Audioflinger
};

static inline void ReplyCapResponse(const sp<AMessage> &msg, const sp<AMessage> &response)
{
    sp<AReplyToken> replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}

static inline void ReplyCapErrorStatus(const sp<AMessage> &msg, int32_t err)
{
    sp<AMessage> response = new AMessage;
    response->setInt32(AMESSAGE_REDIRECT_ERR_STATUS, err);
    ReplyCapResponse(msg, response);
}

static inline status_t CapPostAndAwaitResponse(const sp<AMessage> &msg, sp<AMessage> *response)
{
    status_t err = msg->postAndAwaitResponse(response);

    if (err != OK)
        return err;
    if (!(*response)->findInt32(AMESSAGE_REDIRECT_ERR_STATUS, &err))
        err = OK;
    return err;
}

}
#endif //_AUDIOCOMMON_H
