#include "CursorTexture.h"

CursorTexture::Client::Client()
    : mService(NULL)
{
}

CursorTexture::Client::~Client()
{
}

int CursorTexture::Client::setCursorTextureBuffer(void * buffer, int format, int w, int h, int stride,
        enum ColorLookUpTable::PixelBuffer::ScalerType scalerType) {

    int ret = -1;
    if (mService == NULL)
        goto done;

    setPixelBuffer(buffer, format, w, h, stride, scalerType);

    if (isOverflow()) {
        ALOGW("[%s] isOverflow() is true!", __func__);
        goto done;
    }

    ret = mService->genStream(this);

done:
    if (ret)
        ALOGE("[%s:%d]", __FUNCTION__, __LINE__);
    return ret;
}

void * CursorTexture::Client::getCursorTexturePhyAddr() {
    return getPixelBufferPhyAddr();
}

void * CursorTexture::Client::getCursorTextureVirAddr() {
    return getPixelBufferVirAddr();
}

void * CursorTexture::Client::getCursorTextureTablePhyAddr() {
    return getPixelBufferTablePhyAddr();
}

void * CursorTexture::Client::getCursorTextureTableVirAddr() {
    return getPixelBufferTableVirAddr();
}

CursorTexture::CursorTexture()
{
    mLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    mClientLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    mCLUTLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
}

CursorTexture::~CursorTexture()
{
    while (mClientList.size() > 0) {
        Client * client = mClientList.itemAt(0);
        removeClient(client);
#if 1 // debug
        if (client == mClientList.itemAt(0))
            ALOGE("[%s] ERROR! client=%p", __func__, client);
#endif
    }
    pthread_mutex_lock(&mCLUTLock);
    for (size_t i=0;i<mCLUTList.size();i++) {
        ColorLookUpTable * table = mCLUTList.itemAt(i);
        delete table;
    }
    mCLUTList.clear();
    pthread_mutex_unlock(&mCLUTLock);
}

void CursorTexture::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found && client != NULL) {
        mClientList.add(client);
        client->mService = this;
    }
    pthread_mutex_unlock(&mClientLock);
}

void CursorTexture::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++)
        if (mClientList.itemAt(i) == client) {
            mClientList.removeAt(i);
            client->mService = NULL;
            break;
        }
    pthread_mutex_unlock(&mClientLock);
}

int CursorTexture::genStream(Client * client) {
    int ret = -1;
    bool found = false;
    bool reCheck = false;
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++)
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }

    if (!found)
        goto done;

    pthread_mutex_lock(&mCLUTLock);
    do {
        for (size_t i=0;i<mCLUTList.size();i++) {
            ColorLookUpTable * table = mCLUTList.itemAt(i);
            ret = table->genStream(client);
            if (ret == 0)
                break;
        }

        if (reCheck || ret == 0)
            reCheck = false;
        else if (ret != 0 && mCLUTList.size() <= MAX_CLUT) {
            mCLUTList.add( new ColorLookUpTable());
            reCheck = true;
        }
    } while (reCheck);
    pthread_mutex_unlock(&mCLUTLock);

done:
    pthread_mutex_unlock(&mClientLock);
    if (ret)
        ALOGE("[%s:%d]", __FUNCTION__, __LINE__);
    return ret;
}

void CursorTexture::Client::dump(android::String8& buf, const char* prefix) {
    android::String8 state;
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    state.appendFormat("isOverflow()=%d,", isOverflow());
    ColorLookUpTable::PixelBuffer::dump(state);
    buf.appendFormat("%s", state.string());
    if (prefix != NULL)
        buf.append("\n");
}

void CursorTexture::dump(android::String8& buf, const char* prefix) {
    pthread_mutex_lock(&mCLUTLock);
    for (size_t i=0;i<mCLUTList.size();i++) {
        ColorLookUpTable * table = mCLUTList.itemAt(i);
        table->dump(buf, prefix);
    }
    pthread_mutex_unlock(&mCLUTLock);
}
