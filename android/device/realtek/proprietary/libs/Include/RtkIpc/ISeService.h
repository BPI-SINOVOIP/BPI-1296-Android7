/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * ISeService.h
 *
 *  Created on: Mar 18, 2014
 *      Author: charleslin
 */

#ifndef ISESERVICE_H_
#define ISESERVICE_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#include "RtkIpc/SeOp.h"
#include "RtkIpc/SeClient.h"

namespace android {

class ISeService: public IInterface
{
public:
  DECLARE_META_INTERFACE(SeService);
  virtual unsigned int seInit(void) = 0;
  virtual void seUninit(unsigned int handler) = 0;
  virtual int seScaling(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue, unsigned int interlace) = 0;
  virtual int seScalingDstNo16Align(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue, unsigned int interlace) = 0;
  virtual int seHdmiRxYuv422ToYuyv(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue) = 0;
  virtual int seHdmiRxYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue) = 0;
  virtual int seMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned int hQueue) = 0;
  virtual int se2DMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int nDstStride, int nDstSliceHeight, int nSrcStride, int nSrcSliceHeight, int nWidth, int nHeight, int byteSwap, unsigned int hQueue) = 0;
  virtual int seYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned int hQueue) = 0;
  virtual int seJpegYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nWidth, int nHeight, unsigned int hQueue) = 0;

  virtual int selibWriteRegister(int byCommandQueue, int bRegisterModeSelect, int wRegisterAddress, uint32_t dwRegisterValue) = 0;
  virtual int selibBitBltEx(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo, int bySrcAlphaModeSelect, uint8_t bySrcAplhaValue, int byDestAlphaModeSelect, uint8_t byDestAplhaValue, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, int bSetupModeSelect, int byStartingCorner, int byGradientModeSelect, int byColorKeySelect, int byRopCode, void *pExtraParam) = 0;
  virtual int selibStretch(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byVerticalScalingType, uint32_t dwVerticalScalingRatio, int byVerticalTapNumber, int byHorizontalScalingType, uint32_t dwHorizontalScalingRatio, int byHorizontalTapNumber, int byScalingAlgorithmSelect, int byColorKeySelect, int bAlphaEnable, uint8_t iVerticalScalingOffset, void *pExtraParam) = 0;
  virtual int selibFetchCult(int byCommandQueue, int byFetchModeSelect, uint32_t dwStartingAddress) = 0;
  virtual int selibLockCult(int byCommandQueue, int bLock) = 0;
  //virtual int selibGetLockState(int byCommandQueue) = 0;
  virtual int selibFormatConversion(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byConversionModeSelect, int byDestinationFormatSelect, int bySourceFormatSelect, int byColorKeySelect, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, void *pExtraParam) = 0;
  //virtual int selibFelics(int byCommandQueue, void *hSourceHandle, void *pSeSourceProcInfo, int bCompressMode, int bConvertYuvEnable, int bAlphaEnable, uint32_t dwBitstreamBufferAddress0, uint32_t dwBitstreamLength0, uint32_t dwBitstreamBufferAddress1, uint32_t dwBitstreamLength1, uint32_t dwBitstreamBufferAddress2, uint32_t dwBitstreamLength2, uint32_t dwBitstreamBufferAddress3, uint32_t dwBitstreamBufferAddress3) = 0;
  virtual int selibMirror(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byMirrorDirection) = 0;
  //virtual int selibBitmap(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, int bBitmapLoadingLocation, int bFlashAddressDirection, int bPixelOrder, uint32_t dwBitmapStartingAddress, int bBackgroundColorEnable, uint32_t dwBackgroundColor, int bForegroundColorEnable, uint32_t dwForegroundColor, int bUse32BitColorFormat) = 0;
  //virtual int selibDvdSubtitleDecode(int byCommandQueue, void *byCommandQueue, void *pSeDestinationProcInfo, uint32_t dwPixelDataSubBlockAddress, uint16_t wPixelDataSubBlockLength, int bNoColorModification) = 0;
  virtual int selibRotate(int byCommandQueue, void *hDestinationHandle, void * pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byRotateMode) = 0;
  virtual int selibExecCmd(int byCommandQueue) = 0;
  virtual int selibWaitCmdDone(int byCommandQueue) = 0;
  virtual int selibSetDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress, uint32_t dwPitch, int bInterleavingModeEnable) = 0;
  virtual int selibSetFelicsDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress) = 0;
  virtual int selibSetClippingInfo(int byCommandQueue, int bClippingEnable, int byClippingChannel, uint16_t wXmin, uint16_t wYmin, uint16_t wXmax, uint16_t wYmax) = 0;
  virtual int selibSetColorFormat(int byCommandQueue, int byColorFormat, int byArgbAlphaLocation, int byOutputArgbAlphaLocation, int byRgbLocation, int byOutputRgbLocation, void *pExtraParam) = 0;
  virtual int selibSetColorKey(int byCommandQueue, uint32_t dwColorKey) = 0;
  virtual int selibSetDestColorKey(int byCommandQueue, uint32_t dwColorKey) = 0;
  virtual int selibSetFormatConvertBitblitColorKey(int byCommandQueue, uint32_t dwColorKey) = 0;
  virtual int selibSetSourceColor1(int byCommandQueue, uint32_t dwSourceColor) = 0;
  virtual int selibSetSourceColor2(int byCommandQueue, uint32_t dwSourceColor) = 0;
  virtual int selibSetSourceAlpha1(int byCommandQueue, uint32_t dwSourceAlpha) = 0;
  virtual int selibSetSourceAlpha2(int byCommandQueue, uint32_t dwSourceAlpha) = 0;
  virtual int selibSetDestAlpha(int byCommandQueue, uint32_t dwDestAlpha) = 0;
  virtual int selibSetConstAlpha(int byCommandQueue, uint32_t dwConstAlpha) = 0;
  //virtual int selibGetBitstreamLength(uint32_t dwBitStreamBufferAddress) = 0;
  virtual int selibWaitVSync(int hQueue, int count) = 0;
  virtual int selibRequestQueue(void) = 0;
  virtual int selibReleaseQueue(int hQueue) = 0;
  virtual int selibSetConvCoeff(int byCommandQueue) = 0;
  virtual int selibAlpha(int byCommandQueue, void *pSeSrcProcInfo, void *pSeDestProcInfo, void *pSeOutputProcInfo, int selAlpha, uint8_t srcAplhaValue, uint8_t srcChromaAddrIndex, uint8_t destChromaAddrIndex, uint8_t outputChromaAddrIndex, void *pExtraParam) = 0;
  virtual int selibCopy(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo) = 0;
};

sp<ISeService> defaultSeService();

/* --- Server side --- */

class BnSeService: public BnInterface<ISeService>
{
public:
  virtual status_t    onTransact( uint32_t code,
                                  const Parcel& data,
                                  Parcel* reply,
                                  uint32_t flags = 0);
};

}; // namespace android

#endif /* ISESERVICE_H_ */
