#include <cstring>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <cutils/compiler.h>
#include "PLockBase.h"

#define ATRACE_TAG (ATRACE_TAG_VIDEO)
#include <utils/Timers.h>
#include <utils/Trace.h>

#define PLOCK_PRINT ALOGD

void PLockBase::Client::dump(android::String8& buf, const char* prefix) {
    enum PlockStatus status = getPLockStatus();
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    buf.appendFormat("PLock[%s, 0x%08x]",
            (status == PLOCK_STATUS_RESET)?     "RESET " :
            (status == PLOCK_STATUS_Q_PEND)?    "QPEND " :
            (status == PLOCK_STATUS_VO_LOCK)?   "LOCK  " :
            (status == PLOCK_STATUS_VO_LOCK2)?  "LOCK2 " :
            (status == PLOCK_STATUS_VO_UNLOCK)? "UNLOCK" :
            (status == PLOCK_ERR)?              "ERROR " : "UNKNOWN",
            mPhyaddr);
    if (prefix != NULL)
        buf.append("\n");
}

PLockBase::PLockBase(unsigned int maxIndex, unsigned int version)
    : mMaxIndex(maxIndex),
    mPSize(sizeof(unsigned char)),
    mVersion(version),
    mIonBuffer(NULL)
{
    mLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    mClientLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&mLock);
    if (mMaxIndex != 0) {
        mIonBuffer = new IonBuffer(IonBuffer::AllocBuffer,
                (long long) mPSize * mMaxIndex,
                IonBuffer::AudioHeapMask,
                IonBuffer::NONCACHED|IonBuffer::ACPUACC|IonBuffer::SCPUACC);

        if (mIonBuffer != NULL) {
            for (size_t j=0;j<mMaxIndex;j++) {
                unsigned char * pState = (unsigned char *) mIonBuffer->getVirAddr() + (mPSize * j);
                *pState = PLOCK_STATUS_RESET;
            }
        }
    }
    pthread_mutex_unlock(&mLock);
}

PLockBase::~PLockBase()
{
    while (mClientList.size() > 0) {
        Client * client = mClientList.itemAt(0);
        removeClient(client);
#if 1 // debug
        if (client == mClientList.itemAt(0))
            ALOGE("[%s] ERROR! client=%p", __func__, client);
#endif
    }

    pthread_mutex_lock(&mLock);
    if (mIonBuffer != NULL)
        delete mIonBuffer;
    pthread_mutex_unlock(&mLock);
}

unsigned int PLockBase::getFreeIndex() {
    unsigned int retIndex;
    pthread_mutex_lock(&mLock);
    for (size_t j=0;j<=mMaxIndex;j++) {
        retIndex = j;
        for (size_t i=0;i<mClientList.size();i++) {
            Client * client = mClientList.itemAt(i);
            if (client->mIndex == retIndex) {
                retIndex = -1U;
                break;
            }
        }
        if (retIndex != -1U)
            break;
    }
    if (retIndex == mMaxIndex)
        retIndex = -1U;
    pthread_mutex_unlock(&mLock);
    return retIndex;
}

void PLockBase::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found) {
        if (client != NULL && mIonBuffer != NULL) {
            unsigned int targetIndex = getFreeIndex();
            if (targetIndex != -1U) {
                mClientList.add(client);
                client->mService = this;
                client->mIndex = targetIndex;
                client->mStatus = (unsigned char *) mIonBuffer->getVirAddr() + (targetIndex * mPSize);
                client->mPhyaddr = (unsigned int) ((long)mIonBuffer->getPhyAddr() & -1U) + (targetIndex * mPSize);
                ALOGI("[%s] client:%p [%d] PhyAddr:0x%08lx Status:%d", __func__, client,
                        targetIndex, (long)client->mPhyaddr, *client->mStatus);
            } else
                ALOGE("[%s] getFreeIndex() return -1", __FUNCTION__);
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void PLockBase::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            mClientList.removeAt(i);
            client->mService = NULL;
            client->mStatus = NULL;
            client->mPhyaddr = -1U;
            client->mIndex = -1U;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void PLockBase::setPLockStatus(Client * client, enum PlockStatus status) {
    pthread_mutex_lock(&mClientLock);
    if (client != NULL && client->mStatus != NULL) {
        *client->mStatus = (unsigned char) status;
    }
    pthread_mutex_unlock(&mClientLock);
    atraceDump();
}

PLockBase::PlockStatus PLockBase::getPLockStatus(Client * client) {
    enum PlockStatus status = PLOCK_ERR;
    pthread_mutex_lock(&mClientLock);
    if (client != NULL && client->mStatus != NULL)
        status = (enum PlockStatus) *client->mStatus;
    pthread_mutex_unlock(&mClientLock);
    atraceDump();
    return status;
}

unsigned int PLockBase::getPLockPhyAddr(Client * client) {
    unsigned int phyAddr = -1U;
    pthread_mutex_lock(&mClientLock);
    if (client != NULL && client->mPhyaddr != -1U)
        phyAddr = client->mPhyaddr;
    pthread_mutex_unlock(&mClientLock);
    return phyAddr;
}

void PLockBase::atraceDump() {
    int vo_lock_cnt = 0;
    int vo_q_pend_cnt = 0;
    int vo_unlock_cnt = 0;
    int vo_reset_cnt = 0;
    int vo_err_cnt = 0;

    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i)->mStatus != NULL) {
            enum PlockStatus status = (enum PlockStatus) *mClientList.itemAt(i)->mStatus;
            switch(status) {
                case PLOCK_STATUS_VO_LOCK:
                case PLOCK_STATUS_VO_LOCK2:
                    vo_lock_cnt++;
                    break;
                case PLOCK_STATUS_Q_PEND:
                    vo_q_pend_cnt++;
                    break;
                case PLOCK_STATUS_RESET:
                    vo_reset_cnt++;
                    break;
                case PLOCK_STATUS_VO_UNLOCK:
                    vo_unlock_cnt++;
                    break;
                case PLOCK_ERR:
                    vo_err_cnt++;
                    break;
            }
        }
    }
    pthread_mutex_unlock(&mClientLock);

    ATRACE_INT("SB VO_LOCK", vo_lock_cnt);
    ATRACE_INT("SB VO_Q_PEND", vo_q_pend_cnt);
    ATRACE_INT("SB VO_ERR", vo_err_cnt);
}
