/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * SeService.h
 *
 *  Created on: Mar 18, 2014
 *      Author: charleslin
 */

#ifndef __RTK_SESERVICE_H__
#define __RTK_SESERVICE_H__

#include <utils/Vector.h>
#include <binder/IBinder.h>
#include <binder/BinderService.h>

#include "RtkIpc/ISeService.h"

namespace android {

class Mutex;

class SeService :
    public BinderService<SeService>,
    public BnSeService
{
    friend class BinderService<SeService>;

public:
    SeService();
    virtual             ~SeService();

    // Implementation of BinderService<T>
    static char const* getServiceName() { return "vendor.realtek.se"; }

    //Implementation of ISeService
    virtual unsigned int seInit(void);
    virtual void seUninit(unsigned int handler);
    virtual int seScaling(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue, unsigned int interlace);
    virtual int seScalingDstNo16Align(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue, unsigned int interlace);
    virtual int seHdmiRxYuv422ToYuyv(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue);
    virtual int seHdmiRxYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue);
    virtual int seMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned int hQueue);
    virtual int se2DMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int nDstStride, int nDstSliceHeight, int nSrcStride, int nSrcSliceHeight, int nWidth, int nHeight, int byteSwap, unsigned int hQueue);
    virtual int seYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue);
    virtual int seJpegYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nWidth, int nHeight, unsigned int hQueue);

    virtual int selibWriteRegister(int byCommandQueue, int bRegisterModeSelect, int wRegisterAddress, uint32_t dwRegisterValue);
    virtual int selibBitBltEx(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo, int bySrcAlphaModeSelect, uint8_t bySrcAplhaValue, int byDestAlphaModeSelect, uint8_t byDestAplhaValue, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, int bSetupModeSelect, int byStartingCorner, int byGradientModeSelect, int byColorKeySelect, int byRopCode, void *pExtraParam);
    virtual int selibStretch(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byVerticalScalingType, uint32_t dwVerticalScalingRatio, int byVerticalTapNumber, int byHorizontalScalingType, uint32_t dwHorizontalScalingRatio, int byHorizontalTapNumber, int byScalingAlgorithmSelect, int byColorKeySelect, int bAlphaEnable, uint8_t iVerticalScalingOffset, void *pExtraParam);
    virtual int selibFetchCult(int byCommandQueue, int byFetchModeSelect, uint32_t dwStartingAddress);
    virtual int selibLockCult(int byCommandQueue, int bLock);
    virtual int selibGetLockState(int byCommandQueue);
    virtual int selibFormatConversion(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byConversionModeSelect, int byDestinationFormatSelect, int bySourceFormatSelect, int byColorKeySelect, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, void *pExtraParam);
    //virtual int selibFelics(int byCommandQueue, void *hSourceHandle, void *pSeSourceProcInfo, int bCompressMode, int bConvertYuvEnable, int bAlphaEnable, uint32_t dwBitstreamBufferAddress0, uint32_t dwBitstreamLength0, uint32_t dwBitstreamBufferAddress1, uint32_t dwBitstreamLength1, uint32_t dwBitstreamBufferAddress2, uint32_t dwBitstreamLength2, uint32_t dwBitstreamBufferAddress3, uint32_t dwBitstreamBufferAddress3);
    virtual int selibMirror(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byMirrorDirection);
    //virtual int selibBitmap(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, int bBitmapLoadingLocation, int bFlashAddressDirection, int bPixelOrder, uint32_t dwBitmapStartingAddress, int bBackgroundColorEnable, uint32_t dwBackgroundColor, int bForegroundColorEnable, uint32_t dwForegroundColor, int bUse32BitColorFormat);
    //virtual int selibDvdSubtitleDecode(int byCommandQueue, void *byCommandQueue, void *pSeDestinationProcInfo, uint32_t dwPixelDataSubBlockAddress, uint16_t wPixelDataSubBlockLength, int bNoColorModification);
    virtual int selibRotate(int byCommandQueue, void *hDestinationHandle, void * pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byRotateMode);
    virtual int selibExecCmd(int byCommandQueue);
    virtual int selibWaitCmdDone(int byCommandQueue);
    virtual int selibSetDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress, uint32_t dwPitch, int bInterleavingModeEnable);
    virtual int selibSetFelicsDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress);
    virtual int selibSetClippingInfo(int byCommandQueue, int bClippingEnable, int byClippingChannel, uint16_t wXmin, uint16_t wYmin, uint16_t wXmax, uint16_t wYmax);
    virtual int selibSetColorFormat(int byCommandQueue, int byColorFormat, int byArgbAlphaLocation, int byOutputArgbAlphaLocation, int byRgbLocation, int byOutputRgbLocation, void *pExtraParam);
    virtual int selibSetColorKey(int byCommandQueue, uint32_t dwColorKey);
    virtual int selibSetDestColorKey(int byCommandQueue, uint32_t dwColorKey);
    virtual int selibSetFormatConvertBitblitColorKey(int byCommandQueue, uint32_t dwColorKey);
    virtual int selibSetSourceColor1(int byCommandQueue, uint32_t dwSourceColor);
    virtual int selibSetSourceColor2(int byCommandQueue, uint32_t dwSourceColor);
    virtual int selibSetSourceAlpha1(int byCommandQueue, uint32_t dwSourceAlpha);
    virtual int selibSetSourceAlpha2(int byCommandQueue, uint32_t dwSourceAlpha);
    virtual int selibSetDestAlpha(int byCommandQueue, uint32_t dwDestAlpha);
    virtual int selibSetConstAlpha(int byCommandQueue, uint32_t dwConstAlpha);
    //virtual int selibGetBitstreamLength(uint32_t dwBitStreamBufferAddress);
    virtual int selibWaitVSync(int hQueue, int count);
    virtual int selibRequestQueue(void);
    virtual int selibReleaseQueue(int hQueue);
    virtual int selibSetConvCoeff(int byCommandQueue);
    virtual int selibAlpha(int byCommandQueue, void *pSeSrcProcInfo, void *pSeDestProcInfo, void *pSeOutputProcInfo, int selAlpha, uint8_t srcAplhaValue, uint8_t srcChromaAddrIndex, uint8_t destChromaAddrIndex, uint8_t outputChromaAddrIndex, void *pExtraParam);
    virtual int selibCopy(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo);

private:
    Mutex mLock;
#if 0
    class DeathNotifier : public IBinder::DeathRecipient
    {
    public:
        DeathNotifier(SeService *service, unsigned int hQueue);
        virtual ~DeathNotifier();
        virtual void binderDied(const wp<IBinder> &who);
        unsigned int getHandler() { return mHandler; }
    private:
        SeService *mService;
        sp<IBinder> mClient;
        unsigned int mHandler;
    };

    friend class DeathNotifier;

    void clientDied(DeathNotifier *notifier, const wp<IBinder> &who);

    KeyedVector< sp<IBinder>, sp<DeathNotifier> > mMap;
#endif
};

} // namespace android

#endif //__RTK_SESERVICE_H__
