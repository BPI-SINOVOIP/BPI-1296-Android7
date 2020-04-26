#include "LayerBase.h"

#define ATRACE_TAG (ATRACE_TAG_VIDEO)
#include <utils/Timers.h>
#include <utils/Trace.h>

LayerBaseRPC::LayerBaseRPC()
    :mClnt(prepareCLNT(BLOCK_MODE | USE_INTR_BUF | SEND_AUDIO_CPU, AUDIO_SYSTEM, VERSION)),
    mInstance(0)
{

}

LayerBaseRPC::~LayerBaseRPC() {
}

void LayerBaseRPC::createInstance(enum VIDEO_VF_TYPE type) {
    initRPCProxy();
    VIDEO_RPC_INSTANCE  instance = { .type = type };
    unsigned int outInstance = 0;
    ATRACE_NAME("VIDEO_RPC_ToAgent_Create_0");
    RPCRES_LONG* hr = VIDEO_RPC_ToAgent_Create_0(&instance, &mClnt);

    if (hr /*&& hr->result == S_OK*/)
        outInstance = hr->data;

    if (hr) {
        ALOGD("[%s] free hr!",__FUNCTION__);
        free(hr);
    }

    if (outInstance == 0)
        ALOGE("[VIDEO_RPC_ToAgent_Create_0]fail!\n");
    else
        ALOGD("[VIDEO_RPC_ToAgent_Create_0]success, VO instance id = 0x%x\n",outInstance);

    mInstance = outInstance;
}

int LayerBaseRPC::toAgentDisplay(enum VO_VIDEO_PLANE plane)
{
    VIDEO_RPC_VO_FILTER_DISPLAY disp = {
        .instanceID = mInstance,
        .videoPlane = plane,
        .zeroBuffer = 1,
        .realTimeSrc = 0,
    };

    HRESULT* h = VIDEO_RPC_VO_FILTER_ToAgent_Display_0(&disp, &mClnt);

    if (h && (*h == S_OK)) {
        ALOGD("[%s] (plane:%d) return OK!\n",__func__, plane);
        mPlane = plane;
        free(h);
        return 0;
    } 

    if (h)
        free(h);

    ALOGE("[%s] return ERROR!\n",__func__);
    return -1;
}

int LayerBaseRPC::toAgentState(enum AgentType cmd)
{
    HRESULT* h;

    switch (cmd) {
        case AgentRun:
            {
            ATRACE_NAME("VIDEO_RPC_ToAgent_Run_0");
            h = VIDEO_RPC_ToAgent_Run_0((uint32_t *)&mInstance, &mClnt);
            break;
            }
        case AgentPause:
            {
            ATRACE_NAME("VIDEO_RPC_ToAgent_Pause_0");
            h = VIDEO_RPC_ToAgent_Pause_0((uint32_t *)&mInstance, &mClnt);
            break;
            }
        case AgentStop:
            {
            ATRACE_NAME("VIDEO_RPC_ToAgent_Stop_0");
            h = VIDEO_RPC_ToAgent_Stop_0((uint32_t *)&mInstance, &mClnt);
            break;
            }
        case AgentDestroy:
            {
            ATRACE_NAME("VIDEO_RPC_ToAgent_Destroy_0");
            h = VIDEO_RPC_ToAgent_Destroy_0((uint32_t *)&mInstance, &mClnt);
            break;
            }
        default:
            ALOGE("Unknow CMD: %d",cmd);
            return -1;
    }

    if (h && (*h == S_OK)) {
        ALOGD("[%s] return OK!\n",__func__);
        free(h);
        return 0;
    } 

    if (h)
        free(h);

    ALOGE("[%s] return ERROR!\n",__func__);
    return -1;
}

int LayerBaseRPC::setRefClock(RefClockBase * refClock)
{
    if (mInstance == 0 || refClock == NULL)
        return -1;

    VIDEO_RPC_SET_REFCLOCK rpc_refclock = {
        .instanceID = (unsigned int) mInstance,
        .pRefClock  = (unsigned int) (((long)refClock->getRefClockPhyAddr()) & -1U),
    };

    ATRACE_NAME("VIDEO_RPC_ToAgent_SetRefClock_0");
    HRESULT* h = VIDEO_RPC_ToAgent_SetRefClock_0(&rpc_refclock, &mClnt);

    if (h && (*h == S_OK)) {
        ALOGD("[%s] return OK! (mInstance:0x%x RefClock.phyAddr:0x%lx)\n",__func__, mInstance, (unsigned long) refClock->getRefClockPhyAddr());
        free(h);
        return 0;
    } 

    if (h)
        free(h);

    ALOGE("[%s] return ERROR!\n",__func__);
    return -1;
}

bool LayerBase::loop() {
    bool ret = releaseLoop();
    if (ret)
        usleep(5000);
    return ret;
}

struct LayerBase::LooperThread : public android::Thread {
    LooperThread(LayerBase * layer)
        : mLayer(layer),
        mThreadId(NULL) {
    }

    virtual android::status_t readyToRun() {
        mThreadId = androidGetThreadId();
        return android::Thread::readyToRun();
    }       

    virtual bool threadLoop() {
        //ALOGD("[%s]", __FUNCTION__);
        return mLayer->loop();
    }           

    bool isCurrentThread() const {
        return mThreadId == androidGetThreadId();
    }           

    protected:      
    virtual ~LooperThread() {}

    private:        
    LayerBase *             mLayer;
    android_thread_id_t     mThreadId;

};  

LayerBase::LayerBase()
    : mConnect(false),
    mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mRPC(NULL),
    mRingBuffer(NULL),
    mRefClock(NULL),
    mSWFence(NULL),
    mGBInfo(NULL),
    mBufferLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mBufferContext(0)
{
    mRPC = new LayerBaseRPC();
}

LayerBase::~LayerBase() {
    if (mConnect)
        disconnect();
    delete mRPC;
}

int LayerBase::connect() {
    int ret = -1;
    int bufferCount;
    bool needDisconnect = false;

    pthread_mutex_lock(&mLock);

    if (mConnect)
        goto err;

    mRPC->createInstance(VF_TYPE_VIDEO_OUT);
    bufferCount = getMaxBufferCount();

    mSWFence    = new SWFence();
    mGBInfo     = new GraphicBufferInfo();
    mRefClock   = new RefClockBase(getAudioHwSyncFd());
    mRingBuffer = new RingBufferBase(mRPC->getInstance(),mRPC->getClint());

    mBufferContext = 0;

    mRPC->toAgentDisplay(getPlane());
    mRPC->setRefClock(mRefClock);
    mRPC->toAgentState(LayerBaseRPC::AgentPause);
    mRPC->toAgentState(LayerBaseRPC::AgentRun);

    pthread_mutex_lock(&mBufferLock);
    for (int i=0;i<bufferCount;i++) {
        BufferSlot * slot = new BufferSlot(createBuffer(), this);
        slot->mBuffer->setSWFence(mSWFence);
        slot->mBuffer->setRefClock(mRefClock);
        slot->mBuffer->setRingBuffer(mRingBuffer);
        slot->mBuffer->setGraphicBufferInfo(mGBInfo);
        mBufferList.push_back(slot);
    }
    pthread_mutex_unlock(&mBufferLock);

    mConnect = true;

    mThread = new LooperThread(this);
    if (mThread != NULL) {
        mThread->run("Release Thread");
        ret = 0;
    }

    if (ret) {
        ALOGE("%s::pthread_create() failed : %s", __func__, strerror(ret));
        needDisconnect = true;
        goto err;
    }

    ret = 0;

err:
    pthread_mutex_unlock(&mLock);
    if (needDisconnect)
        disconnect();
    return ret;
}

int LayerBase::disconnect() {
    int ret = -1;

    pthread_mutex_lock(&mLock);
    if (!mConnect)
        goto err;

    mConnect = false;

    mRPC->toAgentDisplay(VO_VIDEO_PLANE_NONE);
    mRPC->toAgentState(LayerBaseRPC::AgentPause);
    mRPC->toAgentState(LayerBaseRPC::AgentStop);
    mRPC->toAgentState(LayerBaseRPC::AgentDestroy);

    usleep(1000);

    mThread->requestExit();
    mThread->requestExitAndWait();
    mThread.clear();
    mThread = NULL;

    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        delete slot;
    }
    mBufferList.clear();
    pthread_mutex_unlock(&mBufferLock);

    delete mSWFence;
    delete mGBInfo;
    delete mRefClock;
    delete mRingBuffer;

    mSWFence    = NULL;
    mGBInfo     = NULL;
    mRefClock   = NULL;
    mRingBuffer = NULL;

    ret = 0;
err:
    pthread_mutex_unlock(&mLock);
    ALOGI("[%s] - (ret:%d)", __FUNCTION__, ret);
    return ret;
}

int LayerBase::render(buffer_handle_t handle, int *pFenceFd) {
    int ret = -1;
    BufferBase * buffer = NULL;
    BufferBase * lastBuffer = NULL;
    pthread_mutex_lock(&mLock);

    if (!mConnect)
        goto err;

    for (int i=0;i<getRenderedBufferCount();i++) {
        lastBuffer = getLastRenderedBuffer(i);
        if (lastBuffer != NULL && lastBuffer->getBufferHandle() == handle) {
            ret = 0;
            if (pFenceFd)
                *pFenceFd = -1;
            goto err;
        }
    }

    buffer = dequeueBuffer();
    if (buffer == NULL)
        goto err;

    ret = buffer->prepareBuffer(handle);

    if (ret) {
        buffer->cancelBuffer();
        cancelBuffer(buffer);
    } else {
        buffer->queueBuffer();
        if (pFenceFd)
            *pFenceFd = buffer->getFenceFd();
        queueBuffer(buffer);
    }

err:
    pthread_mutex_unlock(&mLock);
    if (ret) {
        ALOGE("[%s] buffer:%p", __FUNCTION__, buffer);
        if (pFenceFd)
            *pFenceFd = -1;
    }
    return ret;
}

int LayerBase::freeze(bool enable) {
    int ret = -1;
    BufferBase * lastBuffer = NULL;
    BufferBase * buffer = NULL;
    pthread_mutex_lock(&mLock);

    if (!mConnect)
        goto err;

    if (!enable) {
        ret = 0;
        goto err;
    }

    for (int i=0;i<getRenderedBufferCount();i++) {
        lastBuffer = getLastRenderedBuffer(i);
        buffer = dequeueBuffer();
        if (buffer == NULL || lastBuffer == NULL)
            goto err;

        ret = buffer->prepareBuffer(lastBuffer);

        if (ret) {
            buffer->cancelBuffer();
            cancelBuffer(buffer);
        } else {
            buffer->queueBuffer();
            queueBuffer(buffer);
            break;
        }
    }

err:
    pthread_mutex_unlock(&mLock);
    if (ret)
        ALOGE("[%s] buffer:%p", __FUNCTION__, buffer);
    return ret;
}

bool LayerBase::releaseLoop() {
    //pthread_mutex_lock(&mLock);
    BufferBase * frontBuffer = NULL;
    bool loopContinue = false;

    if (!mConnect) {
        ALOGI("[%s] EXIT!", __FUNCTION__);
        goto done;
    }

    if (getRenderedBufferCount() <= 1) {
        loopContinue = true;
        goto done;
    }

    frontBuffer = getFrontRenderedBuffer();

    if (frontBuffer == NULL) {
        loopContinue = true;
        goto done;
    }

    if (frontBuffer->releaseBuffer() == 0)
        releaseBuffer(frontBuffer);

    loopContinue = true;
done:
    //pthread_mutex_unlock(&mLock);
    return loopContinue;
};

void LayerBase::dump(android::String8& buf, const char* prefix) {
    android::String8 mPrefix(prefix);
    if (!mConnect)
        return;

    buf.appendFormat("%s", mPrefix.string());
    buf.appendFormat("LayerBase (%p) mConnect(%d) bufferCount(%zu) mBufferContext(%lld) renderCount(%d)\n",
            this, mConnect, mBufferList.size(), mBufferContext, getRenderedBufferCount());

    mPrefix.append("    ");

    for (size_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        slot->mBuffer->dump(buf, mPrefix.string());
    }

    mRefClock->dump(buf, mPrefix.string());
}

BufferBase * LayerBase::dequeueBuffer() {
    BufferSlot * found = NULL;
    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        if (slot->mState == BufferSlot::FREE) {
            if (found == NULL)
                found = slot;
            else if (slot->mContext < found->mContext)
                found = slot;
        };
    }
    if (found)
        found->mState = BufferSlot::DEQUEUE;
    pthread_mutex_unlock(&mBufferLock);
    if (found)
        return found->mBuffer;
    else
        return NULL;
}

void LayerBase::queueBuffer(BufferBase * buffer) {
    BufferSlot * found = NULL;
    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        if (slot->mBuffer == buffer) {
            found = slot;
            break;
        }
    }
    if (found) {
        if (found->mState != BufferSlot::DEQUEUE)
            ALOGE("[%s] Error State!!!! found:%p state:%d buffer:%p", __FUNCTION__,
                    found, found->mState, found->mBuffer);
        found->mState = BufferSlot::QUEUE;
        found->mContext = ++mBufferContext; /* begin : 1 */
        //ALOGI("[%s] found:%p mContext:%lld", __FUNCTION__, found, found->mContext);
    } else
        ALOGE("[%s] buffer:%p is not found", __FUNCTION__, buffer);
    pthread_mutex_unlock(&mBufferLock);
}

void LayerBase::cancelBuffer(BufferBase * buffer) {
    BufferSlot * found = NULL;
    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        if (slot->mBuffer == buffer) {
            found = slot;
            break;
        }
    }
    if (found) {
        if (found->mState != BufferSlot::DEQUEUE)
            ALOGE("[%s] Error State!!!! found:%p state:%d buffer:%p", __FUNCTION__,
                    found, found->mState, found->mBuffer);
        found->mState = BufferSlot::FREE;
    } else
        ALOGE("[%s] buffer:%p is not found", __FUNCTION__, buffer);

    pthread_mutex_unlock(&mBufferLock);
}

void LayerBase::releaseBuffer(BufferBase * buffer) {
    BufferSlot * found = NULL;
    //ALOGI("[%s]", __FUNCTION__);
    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        if (slot->mBuffer == buffer) {
            found = slot;
            break;
        }
    }
    if (found) {
        if (found->mState != BufferSlot::QUEUE)
            ALOGE("[%s] Error State!!!! found:%p state:%d buffer:%p", __FUNCTION__,
                    found, found->mState, found->mBuffer);
        found->mState = BufferSlot::FREE;
    } else
        ALOGE("[%s] buffer:%p is not found", __FUNCTION__, buffer);

    pthread_mutex_unlock(&mBufferLock);
}

BufferBase * LayerBase::getLastRenderedBuffer(int stackCnt) {
    BufferSlot * found = NULL;
    unsigned long long stackContext = -1ULL;
    pthread_mutex_lock(&mBufferLock);
    do {
        for (uint32_t i=0;i<mBufferList.size();i++) {
            BufferSlot * slot = mBufferList.itemAt(i);
            if (slot->mState == BufferSlot::QUEUE) {
                if (found == NULL)
                    found = slot;
                else if (   slot->mContext > found->mContext &&
                            slot->mContext < stackContext)
                    found = slot;
            }
        }

        if (found == NULL)
            break;

        if (stackContext == found->mContext)
            break;

        if (stackCnt == 0)
            break;

        stackCnt--;
        stackContext = found->mContext;
    } while (found != NULL && stackCnt > 0);
    
    pthread_mutex_unlock(&mBufferLock);
    if (found == NULL)
        return NULL;
    //ALOGI("[%s] context:%lld", __FUNCTION__, found->mContext);
    return found->mBuffer;
}

BufferBase * LayerBase::getFrontRenderedBuffer(int stackCnt) {
    BufferSlot * found = NULL;
    unsigned long long stackContext = 0;
    pthread_mutex_lock(&mBufferLock);
    do {
        for (uint32_t i=0;i<mBufferList.size();i++) {
            BufferSlot * slot = mBufferList.itemAt(i);
            if (slot->mState == BufferSlot::QUEUE) {
                if (found == NULL)
                    found = slot;
                else if (   slot->mContext < found->mContext &&
                            slot->mContext > stackContext)
                    found = slot;
            }
        }

        if (found == NULL)
            break;

        if (stackContext == found->mContext)
            break;

        if (stackCnt == 0)
            break;

        stackCnt--;
        stackContext = found->mContext;
    } while (found != NULL && stackCnt > 0);
    pthread_mutex_unlock(&mBufferLock);
    if (found == NULL)
        return NULL;
    //ALOGI("[%s] context:%lld", __FUNCTION__, found->mContext);
    return found->mBuffer;
}

int LayerBase::getRenderedBufferCount() {
    int queueCount = 0;
    pthread_mutex_lock(&mBufferLock);
    for (uint32_t i=0;i<mBufferList.size();i++) {
        BufferSlot * slot = mBufferList.itemAt(i);
        if (slot->mState == BufferSlot::QUEUE)
            queueCount++;
    }
    pthread_mutex_unlock(&mBufferLock);
    return queueCount;;
}



