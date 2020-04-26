#include "ColorLookUpTable.h"

ColorLookUpTable::PixelBuffer::PixelBuffer()
    : mBuffer(NULL),
    mFormat(-1),mW(-1),mH(-1),mStride(-1),mScalerType(tNORMAL),
    mIsOverflow(false),
    mPixelTableStep(0),
    mReleaseTable(NULL),
    mPixelsIdMap(NULL),
    mStreamBuffer(NULL)
{
    resetBuffer();
}

ColorLookUpTable::PixelBuffer::~PixelBuffer() {
    resetBuffer();
}

void ColorLookUpTable::PixelBuffer::getYCbCrAlpha(int pixelOffset, tPhyYuvaData * data) {
    typedef struct PixelUnit {
        unsigned char Byte[4];
    } __attribute__((packed,aligned(1))) tPixelUnit;
    unsigned char R_Order = 2;
    unsigned char G_Order = 1;
    unsigned char B_Order = 0;
    unsigned char A_Order = 3;
    int px = pixelOffset%mW;
    int py = pixelOffset/mW;
    int pStride = mStride;
    switch (mScalerType) {
        case tDOUBLE:
            px /= 2;
            py /= 2;
            pStride /= 2;
            break;
        case tSEMI:
            px *= 2;
            py *= 2;
            pStride *= 2;
            break;
        case tNORMAL:
        default:
            break;
    };
    tPixelUnit * unitPixel = (tPixelUnit * ) mBuffer;
    unsigned char R = (unitPixel+((py*pStride) + px))->Byte[R_Order];
    unsigned char G = (unitPixel+((py*pStride) + px))->Byte[G_Order];
    unsigned char B = (unitPixel+((py*pStride) + px))->Byte[B_Order];
    unsigned char A = (unitPixel+((py*pStride) + px))->Byte[A_Order];

#if 1
    if (A > (0xff /2)) {
        R -= R & 0x3f;
        G -= G & 0x3f;
        B -= B & 0x3f;

        if ((A & 0x3f) > (0x3f/2))
            A |= 0x3f;
        else
            A -= A & 0x3f;
    } else {
        A = R = G = B = 0;
    }
#endif

    data->Y     = (unsigned char) (( 0.299 * R + 0.587 * G + 0.114 * B) * 219.0 / 255) + 16;
    data->Cb    = (unsigned char) ((-0.299 * R - 0.587 * G + 0.886 * B) * 224.0 / 1.772 / 255) + 128;
    data->Cr    = (unsigned char) (( 0.701 * R - 0.587 * G - 0.114 * B) * 224.0 / 1.402 / 255) + 128;
    data->Alpha = A;

#if 0
    bool debugPrint = false;

    if(debugPrint)
        ALOGI("[%s] (offset:%d) [R=%d G=%d B=%d A=%d] => [Y=%d U=%d V=%d A=%d]", __FUNCTION__,
                pixelOffset,
                R,G,B,A,
                data->Y,data->Cb,data->Cr,data->Alpha);
#endif
    return;
}

void ColorLookUpTable::PixelBuffer::resetBuffer() {
    if (mReleaseTable != NULL) {
        mReleaseTable->releaseTable(this);
        mReleaseTable = NULL;
    }
    mBuffer = NULL;
    mFormat = mW = mH = mStride = -1;
    mScalerType = tNORMAL;
    mIsOverflow = false;
    mPixelTableStep = 0;

    if (mPixelsIdMap != NULL) {
        free(mPixelsIdMap);
        mPixelsIdMap = NULL;
    }

    if (mStreamBuffer != NULL) {
        delete mStreamBuffer;
        mStreamBuffer = NULL;
    }
}

void ColorLookUpTable::PixelBuffer::setPixelBuffer(void * buffer, int format, int w, int h, int stride, enum ScalerType scalerType) {
    resetBuffer();
    mScalerType = scalerType;
    mBuffer = buffer;
    mFormat = format;
    mW = w;
    mH = h;
    mStride = stride;
    switch (mScalerType) {
        case tDOUBLE:
            mW *= 2;
            mH *= 2;
            mStride *= 2;
            break;
        case tSEMI:
            mW /= 2;
            mH /= 2;
            mStride /= 2;
            break;
        case tNORMAL:
        default:
            break;
    }
    sortPixel();
}

void ColorLookUpTable::PixelBuffer::sortPixel() {
    if (mPixelsIdMap != NULL)
        free(mPixelsIdMap);

    mPixelsIdMap = (struct PIXEL_ID_MAP *) malloc(sizeof(struct PIXEL_ID_MAP) * getPixelSize());
    memset(mPixelsIdMap, 0xff, sizeof(struct PIXEL_ID_MAP) * getPixelSize());

    for (int i=0; i<getPixelSize(); i++) {
        tPhyYuvaData data;
        getYCbCrAlpha(i, &data);
        bool match = false;
        for (int j=0; j<mPixelTableStep; j++) {
            if (    mPixelTable[j].Y     == data.Y &&
                    mPixelTable[j].Cb    == data.Cb &&
                    mPixelTable[j].Cr    == data.Cr &&
                    mPixelTable[j].Alpha == data.Alpha) {
                (mPixelsIdMap + i)->internalID = (unsigned char) j;
                match = true;
            }
            if(match)
                break;
        }

        if (!match) {
            if (mPixelTableStep < MaxColorRow) {
                int j = mPixelTableStep;
                mPixelTable[j].Y     = data.Y;
                mPixelTable[j].Cb    = data.Cb;
                mPixelTable[j].Cr    = data.Cr;
                mPixelTable[j].Alpha = data.Alpha;
                (mPixelsIdMap + i)->internalID = (unsigned char) j;
                mPixelTableStep++;
                match = true;
            } else {
                mIsOverflow = true;
                break;
            }
        }

        if (!match)
            ALOGE("[%s] ERROR!!!! i=%d", __func__, i);
    }

    //ALOGI("[%s] mPixelTableStep=%d", __FUNCTION__, mPixelTableStep);
}

unsigned char ColorLookUpTable::PixelBuffer::getIdByCLUT(int pixelOffset) {
    unsigned char ret = 0xff;
    unsigned char internalID = 0xff;
    if (mPixelsIdMap == NULL)
        goto done;

    internalID = (mPixelsIdMap + (pixelOffset))->internalID;
    if (internalID >= mPixelTableStep)
        goto done;

    ret = mPixelTableIdMap[internalID];

done:
    return ret;
}

void * ColorLookUpTable::PixelBuffer::getPixelBufferPhyAddr() {
    void * ret = NULL;
    if (mStreamBuffer != NULL)
        ret = mStreamBuffer->getPhyAddr();
    return ret;
}

void * ColorLookUpTable::PixelBuffer::getPixelBufferVirAddr() {
    void * ret = NULL;
    if (mStreamBuffer != NULL)
        ret = mStreamBuffer->getVirAddr();
    return ret;
}

void * ColorLookUpTable::PixelBuffer::getPixelBufferTablePhyAddr() {
    void * ret = NULL;
    if (mReleaseTable != NULL && mReleaseTable->mPhyTableBuffer != NULL)
        ret = mReleaseTable->mPhyTableBuffer->getPhyAddr();
    return ret;
}

void * ColorLookUpTable::PixelBuffer::getPixelBufferTableVirAddr() {
    void * ret = NULL;
    if (mReleaseTable != NULL && mReleaseTable->mPhyTableBuffer != NULL)
        ret = mReleaseTable->mPhyTableBuffer->getVirAddr();
    return ret;
}

void ColorLookUpTable::PixelBuffer::dump(android::String8& buf, const char* prefix) {
    android::String8 state;

    if (mReleaseTable == NULL)
        return;

    if (prefix != NULL)
        buf.appendFormat("%s", prefix);

    state.appendFormat("PixelBuffer=%p,CLUT=%p,mPixelTableStep=%d,",
            getPixelBufferPhyAddr(), getPixelBufferTablePhyAddr(), mPixelTableStep);

    buf.appendFormat("%s", state.string());

    if (prefix != NULL)
        buf.append("\n");
}

ColorLookUpTable::ColorLookUpTable() {
    mBufferLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    mPhyTableBuffer = new IonBuffer(IonBuffer::AllocBuffer, MaxColorRow * ColorRowSize, IonBuffer::MediaHeapMask|IonBuffer::AudioHeapMask);
    for (int i=0; i<MaxColorRow; i++)
        mColorRow[i] = new ColorRow(mPhyTableBuffer->getVirAddr(), i);
}

ColorLookUpTable::~ColorLookUpTable() {
    while (mBufferList.size() > 0) {
        PixelBuffer * slot = mBufferList.itemAt(0);
        slot->resetBuffer();
#if 1 /* debug */
        for (uint32_t i=0;i<mBufferList.size();i++)
            if (slot == mBufferList.itemAt(i))
                ALOGE("[%s] ERROR!!!!!!! CLUT=%p PB=%p PB->mCLUT=%p", __func__, this, slot, slot->mReleaseTable);
#endif
    }
    for (int i=0; i<MaxColorRow; i++)
        delete mColorRow[i];
    delete mPhyTableBuffer;
}

int ColorLookUpTable::getColorRowOccupiedCnt() {
    int ret = 0;
    for (int i=0; i<MaxColorRow; i++)
        if (mColorRow[i]->isOccupied())
            ret++;
    return ret;
}

void ColorLookUpTable::dump(android::String8& buf, const char* prefix) {
    android::String8 state;

    if (prefix != NULL)
        buf.appendFormat("%s", prefix);

    state.appendFormat("CLUT=%p", mPhyTableBuffer->getPhyAddr());

    if (prefix != NULL)
        state.append("\n");

    if (prefix != NULL) {
        for (int j=0; j<MaxColorRow; j++) {
            state.appendFormat("%s    ", prefix);
            state.appendFormat("[%2d] (%d) Y:0x%02x Cb:0x%02x Cr:0x%02x A:0x%02x\n",
                    j, mColorRow[j]->mRefCount,
                    mColorRow[j]->mData->Y,
                    mColorRow[j]->mData->Cb,
                    mColorRow[j]->mData->Cr,
                    mColorRow[j]->mData->Alpha);
        }
    }

    buf.appendFormat("%s", state.string());

}

int ColorLookUpTable::genTable(PixelBuffer * inBuffer) {
    int ret = -1;
    int mismatchCnt = inBuffer->mPixelTableStep;
    for (int i = 0; i<inBuffer->mPixelTableStep; i++) {
        for (int j=0; j<MaxColorRow; j++) {
            if (mColorRow[j]->isOccupied() && mColorRow[j]->isMatchData(&(inBuffer->mPixelTable[i])))
                mismatchCnt--;
        }
    }

    if ((mismatchCnt + getColorRowOccupiedCnt()) > MaxColorRow) {
        ALOGE("[%s:%p] not support! mismatchCnt:%d ColorRowOccupiedCnt:%d (MaxColorRow:%d)",
                __FUNCTION__, this, mismatchCnt, getColorRowOccupiedCnt(), MaxColorRow);
        goto done;
    }

    for (int i = 0; i<inBuffer->mPixelTableStep; i++) {
        bool match = false;
        for (int j=0; j<MaxColorRow; j++) {
            if (mColorRow[j]->isOccupied() && mColorRow[j]->isMatchData(&(inBuffer->mPixelTable[i]))) {
                inBuffer->mPixelTableIdMap[i] = (unsigned char) mColorRow[j]->getIndex() & 0xff;
                mColorRow[j]->plusOne();
                match = true;
                break;
            }
        }
        if (!match) {
            for (int j=0; j<MaxColorRow; j++) {
                if (!mColorRow[j]->isOccupied()) {
                    mColorRow[j]->setData(&(inBuffer->mPixelTable[i]));
                    mColorRow[j]->plusOne();
                    inBuffer->mPixelTableIdMap[i] = (unsigned char) mColorRow[j]->getIndex() & 0xff;
                    match = true;
                    break;
                }
            }
        }
        if (!match) {
            ALOGE("[%s] ERROR!!!! i=%d", __func__, i);
        }
    }

    pthread_mutex_lock(&mBufferLock);
    {
        bool found = false;
        for (uint32_t i=0;i<mBufferList.size();i++) {
            PixelBuffer * slot = mBufferList.itemAt(i);
            if (slot == inBuffer) {
                found = true;
                break;
            }
        }
        if (!found)
            mBufferList.add(inBuffer);
    }
    pthread_mutex_unlock(&mBufferLock);

    inBuffer->mReleaseTable = this;
    ret = 0;
done:
    if (ret)
        ALOGE("[%s:%d]", __FUNCTION__, __LINE__);
    return ret;
}

void ColorLookUpTable::releaseTable(PixelBuffer * inBuffer) {
    for (int i = 0; i<inBuffer->mPixelTableStep; i++)
        for (int j=0; j<MaxColorRow; j++)
            if (mColorRow[j]->isOccupied() && mColorRow[j]->isMatchData(&(inBuffer->mPixelTable[i]))) {
                mColorRow[j]->minusOne();
                break;
            }

    pthread_mutex_lock(&mBufferLock);
    {
        for (uint32_t i=0;i<mBufferList.size();i++) {
            PixelBuffer * slot = mBufferList.itemAt(i);
            if (slot == inBuffer) {
                mBufferList.removeAt(i);
                break;
            }
        }
    }
    pthread_mutex_unlock(&mBufferLock);
}

int ColorLookUpTable::genStream(PixelBuffer * inBuffer) {
    int ret = -1;
    unsigned char * pTwoPixels = NULL;
    int loopCnt = inBuffer->getPixelSize()/2;
    ret = genTable(inBuffer);
    if (ret != 0)
        goto done;

    if (inBuffer->mStreamBuffer != NULL)
        delete inBuffer->mStreamBuffer;

    inBuffer->mStreamBuffer = new IonBuffer(IonBuffer::AllocBuffer, GRALLOC_ALIGN(inBuffer->getPixelSize(), 2)/2, IonBuffer::MediaHeapMask|IonBuffer::AudioHeapMask);

    pTwoPixels = (unsigned char *) inBuffer->mStreamBuffer->getVirAddr();

    for (int i=0; i<loopCnt; i++) {
        const unsigned char evenIndex = inBuffer->getIdByCLUT(i*2);
        const unsigned char oddIndex = inBuffer->getIdByCLUT(i*2 + 1);
        *(pTwoPixels + i) = ((oddIndex & 0xf) << 0) | ((evenIndex & 0xf) << 4);
    }

    if (inBuffer->getPixelSize() & 0x1) {
        *(pTwoPixels + loopCnt) = (inBuffer->getIdByCLUT(inBuffer->getPixelSize()-1)&0xf) << 4;
    }

    ret = 0;
done:
    if (ret)
        ALOGE("[%s:%d]", __FUNCTION__, __LINE__);
    return ret;
}

