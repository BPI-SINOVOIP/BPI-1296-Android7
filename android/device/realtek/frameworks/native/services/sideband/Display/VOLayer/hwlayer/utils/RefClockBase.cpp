#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <cutils/compiler.h>
#include "RefClockBase.h"
#include <sys/mman.h>
#include <rpc_common.h>
#include <arpa/inet.h>

RefClockBase::RefClockBase(int audioHwSync)
    : mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mClientLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mContext(1),
    mIonBuffer(NULL)
{
    mAudioHwSync = audioHwSync;

    if (mAudioHwSync <= 0)
        mIonBuffer = new IonBuffer(IonBuffer::AllocBuffer, sizeof(REFCLOCK),
                IonBuffer::AudioHeapMask,
                IonBuffer::NONCACHED|IonBuffer::SCPUACC|IonBuffer::ACPUACC);
    else
        mIonBuffer = new IonBuffer(IonBuffer::ImportBuffer, (long long) mAudioHwSync);

    if (mAudioHwSync <= 0 && mIonBuffer != NULL) {
        REFCLOCK* m_core = (REFCLOCK*) mIonBuffer->getVirAddr();
        m_core->RCD = htonq(-1LL);
        m_core->masterGPTS = htonq(-1LL);
        m_core->GPTSTimeout = htonq(0LL);
        m_core->videoSystemPTS = htonq(-1LL);
        m_core->audioSystemPTS = htonq(-1LL);
        m_core->videoRPTS = htonq(-1LL);
        m_core->audioRPTS = htonq(-1LL);
        m_core->videoContext = htonl(-1);
        m_core->audioContext = htonl(-1);
        m_core->videoEndOfSegment = htonl(-1);
        m_core->videoFreeRunThreshold = htonl(0x7FFFFFFF);
        m_core->audioFreeRunThreshold = htonl(0x7FFFFFFF);
        m_core->VO_Underflow = htonl(0);
        m_core->AO_Underflow = htonl(0);

        m_core->mastership.systemMode  = (unsigned char)AVSYNC_FORCED_SLAVE;
        m_core->mastership.videoMode   = (unsigned char)AVSYNC_FORCED_MASTER;
        m_core->mastership.audioMode   = (unsigned char)AVSYNC_FORCED_MASTER;
        m_core->mastership.masterState = (unsigned char)AUTOMASTER_NOT_MASTER;
    }

}

RefClockBase::~RefClockBase()
{
    while (mClientList.size() > 0) {
        Client * client = mClientList.itemAt(0);
        removeClient(client);
#if 1 // debug
        if (client == mClientList.itemAt(0))
            ALOGE("[%s] ERROR! client=%p", __func__, client);
#endif
    }

    if (mIonBuffer != NULL)
        delete mIonBuffer;
}

void * RefClockBase::getRefClockPhyAddr(Client * /*client*/) {
    void * ret = NULL;
    pthread_mutex_lock(&mClientLock);
    ret = mIonBuffer->getPhyAddr();
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

unsigned int RefClockBase::getRefClockUnusedContext(Client * /*client*/) {
    unsigned int ret = -1U;
    pthread_mutex_lock(&mClientLock);
    ret = mContext++;
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

unsigned int RefClockBase::getRefClockVideoContext(Client * /*client*/) {
    unsigned int ret = -1U;
    REFCLOCK* m_core = NULL;
    pthread_mutex_lock(&mClientLock);
    if (mIonBuffer != NULL)
        m_core = (REFCLOCK*) mIonBuffer->getVirAddr();

    if (m_core != NULL)
        ret = htonl(m_core->videoContext);
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

unsigned int RefClockBase::getRefClockAudioContext(Client * /*client*/) {
    unsigned int ret = -1U;
    REFCLOCK* m_core = NULL;
    pthread_mutex_lock(&mClientLock);
    if (mIonBuffer != NULL)
        m_core = (REFCLOCK*) mIonBuffer->getVirAddr();

    if (m_core != NULL)
        ret = htonl(m_core->audioContext);
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

void RefClockBase::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found) {
        if (client != NULL) {
            mClientList.add(client);
            client->mService = this;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void RefClockBase::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            mClientList.removeAt(i);
            client->mService = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void RefClockBase::dump(android::String8& buf, const char* prefix) {
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    buf.appendFormat("RefClockBase %p virAddr=0x%08x phyAddr=0x%08x mAudioHwSync=%d", this,
            (int) ((long) mIonBuffer->getVirAddr() & -1U),
            (int) ((long) mIonBuffer->getPhyAddr() & -1U),
            mAudioHwSync);
    if (prefix != NULL)
        buf.append("\n");
    if (prefix != NULL) {
        REFCLOCK* m_core = (REFCLOCK*) mIonBuffer->getVirAddr();
        buf.appendFormat("%s    RCD                     = %lld\n" , prefix, htonq(m_core->RCD                     ));
        buf.appendFormat("%s    masterGPTS              = %lld\n" , prefix, htonq(m_core->masterGPTS              ));
        buf.appendFormat("%s    GPTSTimeout             = %lld\n" , prefix, htonq(m_core->GPTSTimeout             ));
        buf.appendFormat("%s    videoSystemPTS          = %lld\n" , prefix, htonq(m_core->videoSystemPTS          ));
        buf.appendFormat("%s    audioSystemPTS          = %lld\n" , prefix, htonq(m_core->audioSystemPTS          ));
        buf.appendFormat("%s    videoRPTS               = %lld\n" , prefix, htonq(m_core->videoRPTS               ));
        buf.appendFormat("%s    audioRPTS               = %lld\n" , prefix, htonq(m_core->audioRPTS               ));
        buf.appendFormat("%s    videoContext            = %d\n"   , prefix, htonl(m_core->videoContext            ));
        buf.appendFormat("%s    audioContext            = %d\n"   , prefix, htonl(m_core->audioContext            ));
        buf.appendFormat("%s    videoEndOfSegment       = %d\n"   , prefix, htonl(m_core->videoEndOfSegment       ));
        buf.appendFormat("%s    videoFreeRunThreshold   = %d\n"   , prefix, htonl(m_core->videoFreeRunThreshold   ));
        buf.appendFormat("%s    audioFreeRunThreshold   = %d\n"   , prefix, htonl(m_core->audioFreeRunThreshold   ));
        buf.appendFormat("%s    VO_Underflow            = %d\n"   , prefix, htonl(m_core->VO_Underflow            ));
        buf.appendFormat("%s    AO_Underflow            = %d\n"   , prefix, htonl(m_core->AO_Underflow            ));
        buf.appendFormat("%s    mastership.systemMode   = %d\n"   , prefix, m_core->mastership.systemMode          );
        buf.appendFormat("%s    mastership.videoMode    = %d\n"   , prefix, m_core->mastership.videoMode           );
        buf.appendFormat("%s    mastership.audioMode    = %d\n"   , prefix, m_core->mastership.audioMode           );
        buf.appendFormat("%s    mastership.masterState  = %d\n"   , prefix, m_core->mastership.masterState         );
    }
}
