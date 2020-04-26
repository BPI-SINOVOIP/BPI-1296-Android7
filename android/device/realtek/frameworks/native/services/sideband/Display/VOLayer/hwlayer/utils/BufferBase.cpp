#include "BufferBase.h"


BufferBase::BufferBase()
    : mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mState(FREE)
{
}

BufferBase::~BufferBase() {
}

int BufferBase::prepareBuffer(BufferBase * buffer) {
    int ret = -1;
    pthread_mutex_lock(&mLock);
    if (mState == FREE) {
        mState = DEQUEUE;
        ret = setReplicaFrom(buffer);
        if (ret == 0)
            ret = prepareBuffer_();
    } else
        ALOGE("[%s] ret:%d mState:%d", __FUNCTION__, ret, mState);
    pthread_mutex_unlock(&mLock);
    return ret;
}

int BufferBase::prepareBuffer(buffer_handle_t handle) {
    int ret = -1;
    pthread_mutex_lock(&mLock);
    if (mState == FREE) {
        mState = DEQUEUE;
        setBufferHandle(handle);
        ret = prepareBuffer_();
    } else
        ALOGE("[%s] ret:%d mState:%d", __FUNCTION__, ret, mState);
    pthread_mutex_unlock(&mLock);
    return ret;
}

int BufferBase::prepareBuffer(ExtBuffer * extBuffer) {
    int ret = -1;
    pthread_mutex_lock(&mLock);
    if (mState == FREE) {
        mState = DEQUEUE;
        setExtBuffer(extBuffer);
        ret = prepareBuffer_();
    } else
        ALOGE("[%s] ret:%d mState:%d", __FUNCTION__, ret, mState);
    pthread_mutex_unlock(&mLock);
    return ret;
}

void BufferBase::queueBuffer() {
    pthread_mutex_lock(&mLock);
    if (mState != DEQUEUE)
        ALOGE("[%s] mState:%d", __FUNCTION__, mState);

    mState = QUEUE;
    acquiredFenceFd();
    queueBuffer_();
    pthread_mutex_unlock(&mLock);
    return;
}

void BufferBase::cancelBuffer() {
    pthread_mutex_lock(&mLock);
    if (mState == DEQUEUE)
        ALOGE("[%s] mState:%d", __FUNCTION__, mState);

    mState = FREE;
    cancelBuffer_();
    //releaseFenceFd();
    clearBufferHandle();
    pthread_mutex_unlock(&mLock);
    return;
}

int BufferBase::releaseBuffer() {
    int ret = -1;
    pthread_mutex_lock(&mLock);
    if (mState == QUEUE && releaseBufferCheck()) {
        mState = FREE;
        releaseFenceFd();
        clearBufferHandle();
        ret = 0;
    }
#if 0
    else
        ALOGE("[%s] ret:%d mState:%d", __FUNCTION__, ret, mState);
#endif
    pthread_mutex_unlock(&mLock);
    return ret;
}

void BufferBase::removeBuffer() {
    if (mState == DEQUEUE)
        cancelBuffer();
    else if (mState == QUEUE)
        releaseBuffer();

    pthread_mutex_lock(&mLock);
    SWFence::Client::setSWFence();
    RefClockBase::Client::setRefClock();
    RingBufferBase::Client::setRingBuffer();
    GraphicBufferInfo::Client::setGraphicBufferInfo();
    pthread_mutex_unlock(&mLock);
}

void BufferBase::dump(android::String8& buf, const char* prefix) {
    android::String8 state;
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    SWFence::Client::dump(state);
    state.append(",");
    GraphicBufferInfo::Client::dump(state);
    buf.appendFormat("%s", state.string());
    if (prefix != NULL)
        buf.append("\n");
}
