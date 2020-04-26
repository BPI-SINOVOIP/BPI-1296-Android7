/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 * SeOp.h
 *
 *  Created on: Mar 20, 2014
 *      Author: charleslin
 */

#ifndef SEOP_H_
#define SEOP_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned long SeInit();
void SeUninit(unsigned long handler);
int SeScaling(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned long handler, unsigned int interlace);
int SeScalingDstNo16Align(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned long handler, unsigned int interlace);
int SeHdmiRxYuv422ToYuyv(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned long handler);
int SeHdmiRxYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned long handler);
int SeMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned long handler);
int Se2DMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int nDstStride, int nDstSliceHeight, int nSrcStride, int nSrcSliceHeight, int nWidth, int nHeight, int byteSwap, unsigned long handler);
int SeYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nDestWidth, int nDestHeight, int nSrcWidth, int nSrcHeight, unsigned long handler);
int SeJpegYuv422ToYuv420(unsigned int dstPAddr, unsigned int srcPAddr, int nWidth, int nHeight, unsigned long handler);

int SeLibWriteRegister(int byCommandQueue, int bRegisterModeSelect, int wRegisterAddress, uint32_t dwRegisterValue);
int SeLibBitBltEx(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo, int bySrcAlphaModeSelect, uint8_t bySrcAplhaValue, int byDestAlphaModeSelect, uint8_t byDestAplhaValue, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, int bSetupModeSelect, int byStartingCorner, int byGradientModeSelect, int byColorKeySelect, int byRopCode, void *pExtraParam);
int SeLibStretch(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byVerticalScalingType, uint32_t dwVerticalScalingRatio, int byVerticalTapNumber, int byHorizontalScalingType, uint32_t dwHorizontalScalingRatio, int byHorizontalTapNumber, int byScalingAlgorithmSelect, int byColorKeySelect, int bAlphaEnable, uint8_t iVerticalScalingOffset, void *pExtraParam);
int SeLibFetchCult(int byCommandQueue, int byFetchModeSelect, uint32_t dwStartingAddress);
int SeLibLockCult(int byCommandQueue, int bLock);
//int SeLibGetLockState(int byCommandQueue);
int SeLibFormatConversion(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byConversionModeSelect, int byDestinationFormatSelect, int bySourceFormatSelect, int byColorKeySelect, int byOutputAlphaModeSelect, uint8_t byOutputAlphaValue, void *pExtraParam);
//int SeLibFelics(int byCommandQueue, void *hSourceHandle, void *pSeSourceProcInfo, int bCompressMode, int bConvertYuvEnable, int bAlphaEnable, uint32_t dwBitstreamBufferAddress0, uint32_t dwBitstreamLength0, uint32_t dwBitstreamBufferAddress1, uint32_t dwBitstreamLength1, uint32_t dwBitstreamBufferAddress2, uint32_t dwBitstreamLength2, uint32_t dwBitstreamBufferAddress3, uint32_t dwBitstreamBufferAddress3);
int SeLibMirror(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byMirrorDirection);
//int SeLibBitmap(int byCommandQueue, void *hDestinationHandle, void *pSeDestinationProcInfo, int bBitmapLoadingLocation, int bFlashAddressDirection, int bPixelOrder, uint32_t dwBitmapStartingAddress, int bBackgroundColorEnable, uint32_t dwBackgroundColor, int bForegroundColorEnable, uint32_t dwForegroundColor, int bUse32BitColorFormat);
//int SeLibDvdSubtitleDecode(int byCommandQueue, void *byCommandQueue, void *pSeDestinationProcInfo, uint32_t dwPixelDataSubBlockAddress, uint16_t wPixelDataSubBlockLength, int bNoColorModification);
int SeLibRotate(int byCommandQueue, void *hDestinationHandle, void * pSeDestinationProcInfo, void *hSourceHandle, void *pSeSourceProcInfo, int byRotateMode);
int SeLibExecCmd(int byCommandQueue);
int SeLibWaitCmdDone(int byCommandQueue);
int SeLibSetDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress, uint32_t dwPitch, int bInterleavingModeEnable);
int SeLibSetFelicsDmaInfo(int byCommandQueue, int byDmaChannel, uint32_t dwBaseAddress);
int SeLibSetClippingInfo(int byCommandQueue, int bClippingEnable, int byClippingChannel, uint16_t wXmin, uint16_t wYmin, uint16_t wXmax, uint16_t wYmax);
int SeLibSetColorFormat(int byCommandQueue, int byColorFormat, int byArgbAlphaLocation, int byOutputArgbAlphaLocation, int byRgbLocation, int byOutputRgbLocation, void *pExtraParam);
int SeLibSetColorKey(int byCommandQueue, uint32_t dwColorKey);
int SeLibSetDestColorKey(int byCommandQueue, uint32_t dwColorKey);
int SeLibSetFormatConvertBitblitColorKey(int byCommandQueue, uint32_t dwColorKey);
int SeLibSetSourceColor1(int byCommandQueue, uint32_t dwSourceColor);
int SeLibSetSourceColor2(int byCommandQueue, uint32_t dwSourceColor);
int SeLibSetSourceAlpha1(int byCommandQueue, uint32_t dwSourceAlpha);
int SeLibSetSourceAlpha2(int byCommandQueue, uint32_t dwSourceAlpha);
int SeLibSetDestAlpha(int byCommandQueue, uint32_t dwDestAlpha);
int SeLibSetConstAlpha(int byCommandQueue, uint32_t dwConstAlpha);
//int SeLibGetBitstreamLength(uint32_t dwBitStreamBufferAddress);
int SeLibWaitVSync(int hQueue, int count);
int SeLibRequestQueue(void);
int SeLibReleaseQueue(int hQueue);
int SeLibSetConvCoeff(int byCommandQueue);
int SeLibAlpha(int byCommandQueue, void *pSeSrcProcInfo, void *pSeDestProcInfo, void *pSeOutputProcInfo, int selAlpha, uint8_t srcAplhaValue, uint8_t srcChromaAddrIndex, uint8_t destChromaAddrIndex, uint8_t outputChromaAddrIndex, void *pExtraParam);
int SeLibCopy(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SEOP_H_ */
