/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _AUDIORECORDREADER_H_
#define _AUDIORECORDREADER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <media/AudioTrack.h>
#include <media/AudioRecord.h>
#include "RtkTranscodeCommon.h"
#include "AudioSourceFilter.h"

namespace android {

struct ALooper;
struct AHandler;

class AudioRecordReader : public AudioSourceFilter
{
public:
    AudioRecordReader();

protected:
    virtual ~AudioRecordReader();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        INITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    enum {
        kMaxBufferSize = 2048,
    };

    State mState;

    bool mRenderAudio;
    bool mEncodeAudio;

    sp<AudioTrack> mAudioTrack;
    List< sp<ABuffer> > mAudioQueue;
    Mutex mAQueLock;

    sp<AudioRecord> mAudioRecord;
    int32_t mChannelCount;
    int32_t mSampleRate;
    int32_t mAudioSource;
    String16 mClientName;

    status_t startAudioRender();
    status_t stopAudioRender();
    status_t setPlayback(int renderAudio);
    status_t setEncode(int encodeAudio);
    status_t start();
    status_t stop();
    status_t prepare();

    static void AudioRecordCallback(int event, void *user, void *info);
    void AudioRecordDataCb(AudioRecord::Buffer *info);

    static void AudioTrackCallback(int event, void *user, void *info);
    void AudioTrackDataCb(AudioTrack::Buffer *info);

    DISALLOW_EVIL_CONSTRUCTORS(AudioRecordReader);
};

} // namespace android

#endif //_AUDIORECORDREADER_H_
