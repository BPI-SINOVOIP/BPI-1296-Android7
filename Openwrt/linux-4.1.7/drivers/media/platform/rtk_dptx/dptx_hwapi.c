/*
 *  dptx_hwapi.c - RTK display port driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>

//#include "dptx_reg.h"
#include "dptx_hwapi.h"

#define RTK_VO_SET 0
#define AUDIO_ON 1
//#define SS_EN
#define LINK_TRAIN_RETRY 50

//#define TESTING 1

typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

BYTE tDPTX_DRV_TABLE[48] =
{
	// Pre-emphasis->        0                    1                    2                    3
	// VoltageSwing
	/*    0    */      0x00, 0x00, 0x03,    0x00, 0x07, 0x05,    0x00, 0x0C, 0x05,    0x01, 0x09, 0x06,
	/*    1    */      0x00, 0x00, 0x05,    0x00, 0x07, 0x05,    0x00, 0x0C, 0x05,    0x00, 0x0C, 0x05,
	/*    2    */      0x00, 0x00, 0x09,    0x00, 0x0C, 0x09,    0x00, 0x0C, 0x09,    0x00, 0x0C, 0x09,
	/*    3    */      0x00, 0x00, 0x0E,    0x00, 0x00, 0x0E,    0x00, 0x00, 0x0E,    0x00, 0x00, 0x0E,
};


BYTE tDPTX_DRV_TABLE1[30] =
{
	0x00, 0x00, 0x03, 	//SP_00 (400mV)
	0x00, 0x07, 0x05, 	//SP_01
	0x00, 0x0C, 0x05, 	//SP_02
	0x01, 0x09, 0x06, 	//SP_03
	0x00, 0x00, 0x05, 	//SP_10 (600mV)
	0x00, 0x07, 0x05, 	//SP_11
	0x00, 0x0C, 0x05, 	//SP_12
	0x00, 0x00, 0x09, 	//SP_20 (800mV)
	0x00, 0x0C, 0x09, 	//SP_21
	0x00, 0x00, 0x0E, 	//SP_30 (1200mV)
};

void DpTxTPInitialSet(struct rtk_dptx_hwinfo *hwinfo);

#define _DP_TX_SST_TU_SIZE 32

#define GET_DATA_BIT0(x) (x & 0x1)
#define GET_DATA_BIT1(x) ((x & (0x1<<1))>>1)
#define GET_DATA_BIT2(x) ((x & (0x1<<2))>>2)
#define GET_DATA_BIT3(x) ((x & (0x1<<3))>>3)
#define GET_DATA_BIT4(x) ((x & (0x1<<4))>>4)
#define GET_DATA_BIT5(x) ((x & (0x1<<5))>>5)
#define GET_DATA_BIT6(x) ((x & (0x1<<6))>>6)
#define GET_DATA_BIT7(x) ((x & (0x1<<7))>>7)

#define HIBYTE(x) ((x & 0xFF00)>>8)
#define LOBYTE(x) (x & 0x00FF)

#define GET_DP_TX_AUDIO_READY_TO_PLAY(x) (x->TxInputInfo.b1AudioReadyToPlay)
#define SET_DP_TX_AUDIO_READY_TO_PLAY(x) (x->TxInputInfo.b1AudioReadyToPlay = _TRUE)
#define CLR_DP_TX_AUDIO_READY_TO_PLAY(x) (x->TxInputInfo.b1AudioReadyToPlay = _FALSE)

#define GET_DP_TX_CURRENT_AUDIO_FREQUENCY(x) (x->TxInputInfo.ucCurrnetAudioFrequency)
#define SET_DP_TX_CURRENT_AUDIO_FREQUENCY(x,y) (x->TxInputInfo.ucCurrnetAudioFrequency = (y))
#define CLR_DP_TX_CURRENT_AUDIO_FREQUENCY(x) (x->TxInputInfo.ucCurrnetAudioFrequency = _DP_TX_AUDIO_FREQ_NO_AUDIO)

#define GET_DP_TX_AUDIO_CHANNEL_COUNT(x) (x->TxInputInfo.b3AudioChannelCount)
#define SET_DP_TX_AUDIO_CHANNEL_COUNT(x,y)(x->TxInputInfo.b3AudioChannelCount = (y))

#define GET_DP_TX_INPUT_COLOR_DEPTH(x) (x->TxInputInfo.b3ColorDepth)
#define SET_DP_TX_INPUT_COLOR_DEPTH(x,y) (x->TxInputInfo.b3ColorDepth = (y))

#define GET_DP_TX_INPUT_COLOR_SPACE(x) (x->TxInputInfo.b3ColorSpace)
#define SET_DP_TX_INPUT_COLOR_SPACE(x,y) (x->TxInputInfo.b3ColorSpace = (y))

#define GET_DP_TX_INPUT_PIXEL_CLK(x) (x->TxInputInfo.usInputPixelClk)
#define SET_DP_TX_INPUT_PIXEL_CLK(x,y) (x->TxInputInfo.usInputPixelClk = (y))
#define CLR_DP_TX_INPUT_PIXEL_CLK(x) (x->TxInputInfo.usInputPixelClk = 0)

#define GET_DP_TX_INPUT_INTERLACE_MODE(x) (x->TxInputInfo.b1InterlaceMode)
#define SET_DP_TX_INPUT_INTERLACE_MODE(x,y) (x->TxInputInfo.b1InterlaceMode = (y))

#define GET_DP_TX_INPUT_INTERLACE_VTOTAL_NUMBER(x) (x->TxInputInfo.b1InterlaceVTotalNumber)
#define SET_DP_TX_INPUT_INTERLACE_VTOTAL_NUMBER(x,y) (x->TxInputInfo.b1InterlaceVTotalNumber = (y))

#define GET_DP_TX_HORIZONTAL_DE_PERIOD(x) (x->TxInputInfo.usHorizontalDataEnablePeriod)
#define SET_DP_TX_HORIZONTAL_DE_PERIOD(x,y) (x->TxInputInfo.usHorizontalDataEnablePeriod = (y))


//--------------------------------------------------
// Macros of DP Tx Downstream Infomation
//--------------------------------------------------
#define GET_DP_TX_DOWNSTREAM_DPCD_REV(x) (x->DownStreamInfo.ucDPCDRev)
#define SET_DP_TX_DOWNSTREAM_DPCD_REV(x,y) (x->DownStreamInfo.ucDPCDRev = (y))

#define GET_DP_TX_DOWNSTREAM_PEERDEVICE_TYPE(x) (x->DownStreamInfo.b3PeerDeviceType)
#define SET_DP_TX_DOWNSTREAM_PEERDEVICE_TYPE(x,y) (x->DownStreamInfo.b3PeerDeviceType = (y))

#define GET_DP_TX_DOWNSTREAM_LINK_RATE(x) (x->DownStreamInfo.ucLinkRate)
#define SET_DP_TX_DOWNSTREAM_LINK_RATE(x,y) (x->DownStreamInfo.ucLinkRate = (y))

#define GET_DP_TX_DOWNSTREAM_LANE_NUM(x) (x->DownStreamInfo.b3LaneNum)
#define SET_DP_TX_DOWNSTREAM_LANE_NUM(x,y) (x->DownStreamInfo.b3LaneNum = (y))

#define GET_DP_TX_DOWNSTREAM_MSGCAP_STATUS(x) (x->DownStreamInfo.b1MSGCapStatus)
#define SET_DP_TX_DOWNSTREAM_MSGCAP_STATUS(x,y) (x->DownStreamInfo.b1MSGCapStatus = (y))

#define GET_DP_TX_DOWNSTREAM_DPPLUG_STATUS(x) (x->DownStreamInfo.b1DPPlugStatus)
#define SET_DP_TX_DOWNSTREAM_DPPLUG_STATUS(x,y) (x->DownStreamInfo.b1DPPlugStatus = (y))

#define GET_DP_TX_DOWNSTREAM_DPPLUG_CHANGE(x) (x->DownStreamInfo.b1DPPlugChange)
#define SET_DP_TX_DOWNSTREAM_DPPLUG_CHANGE(x) (x->DownStreamInfo.b1DPPlugChange = _TRUE)
#define CLR_DP_TX_DOWNSTREAM_DPPLUG_CHANGE(x) (x->DownStreamInfo.b1DPPlugChange = _FALSE)

#define GET_DP_TX_DOWNSTREAM_LEGACYPLUG_STATUS(x) (x->DownStreamInfo.b1LegacyPlugStatus)
#define SET_DP_TX_DOWNSTREAM_LEGACYPLUG_STATUS(x,y) (x->DownStreamInfo.b1LegacyPlugStatus = (y))

#define GET_DP_TX_DOWNSTREAM_MAX_LINK_RATE(x) (x->DownStreamInfo.ucMaxLinkRate)
#define SET_DP_TX_DOWNSTREAM_MAX_LINK_RATE(x,y) (x->DownStreamInfo.ucMaxLinkRate = (y))

#define GET_DP_TX_DOWNSTREAM_ENHANCE_FRAMING(x) (x->DownStreamInfo.b1EnhanceFraming)
#define SET_DP_TX_DOWNSTREAM_ENHANCE_FRAMING(x,y) (x->DownStreamInfo.b1EnhanceFraming = (y))

#define GET_DP_TX_DOWNSTREAM_DOWN_SPREAD(x) (x->DownStreamInfo.b1SSCSupport)
#define SET_DP_TX_DOWNSTREAM_DOWN_SPREAD(x,y) (x->DownStreamInfo.b1SSCSupport = (y))

#define GET_DP_TX_DOWNSTREAM_TP3_SUPPORT(x) (x->DownStreamInfo.b1TP3Support)
#define SET_DP_TX_DOWNSTREAM_TP3_SUPPORT(x) (x->DownStreamInfo.b1TP3Support = _TRUE)
#define CLR_DP_TX_DOWNSTREAM_TP3_SUPPORT(x) (x->DownStreamInfo.b1TP3Support = _FALSE)

#define GET_DP_TX_DOWNSTREAM_NUMBER_OF_SDP(x) (x->DownStreamInfo.b1NumberOfSDP)
#define SET_DP_TX_DOWNSTREAM_NUMBER_OF_SDP(x,y) (x->DownStreamInfo.b1NumberOfSDP = (y))

#define GET_DP_TX_DOWNSTREAM_NUMBER_OF_SDP_SINK(x) (x->DownStreamInfo.b1NumberOfSDPSink)
#define SET_DP_TX_DOWNSTREAM_NUMBER_OF_SDP_SINK(x,y) (x->DownStreamInfo.b1NumberOfSDPSink = (y))

#define GET_DP_TX_DOWNSTREAM_INFO_READY(x) (x->DownStreamInfo.b1DownStreamInfoReady)
#define SET_DP_TX_DOWNSTREAM_INFO_READY(x) (x->DownStreamInfo.b1DownStreamInfoReady = _TRUE)
#define CLR_DP_TX_DOWNSTREAM_INFO_READY(x) (x->DownStreamInfo.b1DownStreamInfoReady = _FALSE)

//--------------------------------------------------
// Macros of DP Tx LinkTraining Infomation
//--------------------------------------------------
#define GET_DP_TX_LT_STATE(x) (x->TxLTInfo.b2LinkTrainingState)
#define SET_DP_TX_LT_STATE(x,y) (x->TxLTInfo.b2LinkTrainingState = (y))
#define GET_DP_TX_LT_RESULT(x) (x->TxLTInfo.b4LinkTrainingResult)
#define SET_DP_TX_LT_RESULT(x,y) (x->TxLTInfo.b4LinkTrainingResult = (y))
#define GET_DP_TX_LINK_PBN_PER_TIMESOLT(x) (x->TxLTInfo.ucLinkPBNPerTimeSlot)
#define SET_DP_TX_LINK_PBN_PER_TIMESOLT(x,y) (x->TxLTInfo.ucLinkPBNPerTimeSlot = (y))
#define GET_DP_TX_TP1_LANE0_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane0SameVolCnt)
#define ADD_DP_TX_TP1_LANE0_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane0SameVolCnt++)
#define CLR_DP_TX_TP1_LANE0_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane0SameVolCnt = 0)
#define GET_DP_TX_TP1_LANE1_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane1SameVolCnt)
#define ADD_DP_TX_TP1_LANE1_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane1SameVolCnt++)
#define CLR_DP_TX_TP1_LANE1_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane1SameVolCnt = 0)
#define GET_DP_TX_TP1_LANE2_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane2SameVolCnt)
#define ADD_DP_TX_TP1_LANE2_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane2SameVolCnt++)
#define CLR_DP_TX_TP1_LANE2_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane2SameVolCnt = 0)
#define GET_DP_TX_TP1_LANE3_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane3SameVolCnt)
#define ADD_DP_TX_TP1_LANE3_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane3SameVolCnt++)
#define CLR_DP_TX_TP1_LANE3_SAME_VOL_CNT(x) (x->TxLTInfo.b3TP1Lane3SameVolCnt = 0)

#define GET_DP_TX_TP1_CNT(x) (x->TxLTInfo.b3TP1count)
#define ADD_DP_TX_TP1_CNT(x) (x->TxLTInfo.b3TP1count++)
#define CLR_DP_TX_TP1_CNT(x) (x->TxLTInfo.b3TP1count = 0)

#define GET_DP_TX_TP2_CNT(x) (x->TxLTInfo.b3TP2count)
#define ADD_DP_TX_TP2_CNT(x) (x->TxLTInfo.b3TP2count++)
#define CLR_DP_TX_TP2_CNT(x) (x->TxLTInfo.b3TP2count = 0)

#define GET_DP_TX_LANE0_CURRENT_VOL_SWING(x) (x->TxLTInfo.ucLane0Adjust & 0x03)
#define SET_DP_TX_LANE0_CURRENT_VOL_SWING(x,y) (x->TxLTInfo.ucLane0Adjust = ((x->TxLTInfo.ucLane0Adjust & 0xFC) | (y)))
#define SET_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane0Adjust |= _BIT2)
#define CLR_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane0Adjust &= ~_BIT2)

#define GET_DP_TX_LANE1_CURRENT_VOL_SWING(x) (x->TxLTInfo.ucLane1Adjust & 0x03)
#define SET_DP_TX_LANE1_CURRENT_VOL_SWING(x,y) (x->TxLTInfo.ucLane1Adjust = ((x->TxLTInfo.ucLane1Adjust & 0xFC) | (y)))
#define SET_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane1Adjust |= _BIT2)
#define CLR_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane1Adjust &= ~_BIT2)

#define GET_DP_TX_LANE2_CURRENT_VOL_SWING(x) (x->TxLTInfo.ucLane2Adjust & 0x03)
#define SET_DP_TX_LANE2_CURRENT_VOL_SWING(x,y) (x->TxLTInfo.ucLane2Adjust = ((x->TxLTInfo.ucLane2Adjust & 0xFC) | (y)))
#define SET_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane2Adjust |= _BIT2)
#define CLR_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane2Adjust &= ~_BIT2)

#define GET_DP_TX_LANE3_CURRENT_VOL_SWING(x) (x->TxLTInfo.ucLane3Adjust & 0x03)
#define SET_DP_TX_LANE3_CURRENT_VOL_SWING(x,y) (x->TxLTInfo.ucLane3Adjust = ((x->TxLTInfo.ucLane3Adjust & 0xFC) | (y)))
#define SET_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane3Adjust |= _BIT2)
#define CLR_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(x) (x->TxLTInfo.ucLane3Adjust &= ~_BIT2)

#define GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(x) ((x->TxLTInfo.ucLane0Adjust & (_BIT4 | _BIT3)) >> 3)
#define SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucLane0Adjust = ((x->TxLTInfo.ucLane0Adjust & ~(_BIT4 | _BIT3)) | ((y) << 3)))
#define SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane0Adjust |= _BIT5)
#define CLR_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane0Adjust &= ~_BIT5)

#define GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(x) ((x->TxLTInfo.ucLane1Adjust & (_BIT4 | _BIT3)) >> 3)
#define SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucLane1Adjust = ((x->TxLTInfo.ucLane1Adjust & ~(_BIT4 | _BIT3)) | ((y) << 3)))
#define SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane1Adjust |= _BIT5)
#define CLR_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane1Adjust &= ~_BIT5)

#define GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(x) ((x->TxLTInfo.ucLane2Adjust & (_BIT4 | _BIT3)) >> 3)
#define SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucLane2Adjust = ((x->TxLTInfo.ucLane2Adjust & ~(_BIT4 | _BIT3)) | ((y) << 3)))
#define SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane2Adjust |= _BIT5)
#define CLR_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane2Adjust &= ~_BIT5)

#define GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(x) ((x->TxLTInfo.ucLane3Adjust & (_BIT4 | _BIT3)) >> 3)
#define SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucLane3Adjust = ((x->TxLTInfo.ucLane3Adjust & ~(_BIT4 | _BIT3)) | ((y) << 3)))
#define SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane3Adjust |= _BIT5)
#define CLR_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(x) (x->TxLTInfo.ucLane3Adjust &= ~_BIT5)

#define GET_DP_TX_LANE0_CR_DONE(x) ((x->TxLTInfo.ucLane01LTStatus & _BIT0) == _BIT0)
#define GET_DP_TX_LANE1_CR_DONE(x) ((x->TxLTInfo.ucLane01LTStatus & _BIT4) == _BIT4)
#define GET_DP_TX_LANE2_CR_DONE(x) ((x->TxLTInfo.ucLane23LTStatus & _BIT0) == _BIT0)
#define GET_DP_TX_LANE3_CR_DONE(x) ((x->TxLTInfo.ucLane23LTStatus & _BIT4) == _BIT4)

#define GET_DP_TX_LANE0_EQ_DONE(x) ((x->TxLTInfo.ucLane01LTStatus & (_BIT1 | _BIT2)) == (_BIT1 | _BIT2))
#define GET_DP_TX_LANE1_EQ_DONE(x) ((x->TxLTInfo.ucLane01LTStatus & (_BIT5 | _BIT6)) == (_BIT5 | _BIT6))
#define GET_DP_TX_LANE2_EQ_DONE(x) ((x->TxLTInfo.ucLane23LTStatus & (_BIT1 | _BIT2)) == (_BIT1 | _BIT2))
#define GET_DP_TX_LANE3_EQ_DONE(x) ((x->TxLTInfo.ucLane23LTStatus & (_BIT5 | _BIT6)) == (_BIT5 | _BIT6))

#define GET_DP_TX_LANE_ALIGN_DONE(x) ((x->TxLTInfo.ucLaneAlignStatus & _BIT0) == _BIT0)

#define GET_DP_TX_LANE0_ADJUST_VOL_SWING(x) (x->TxLTInfo.ucAdjustReqLane01 & (_BIT1 | _BIT0))
#define SET_DP_TX_LANE0_ADJUST_VOL_SWING(x,y) (x->TxLTInfo.ucAdjustReqLane01 = ((x->TxLTInfo.ucAdjustReqLane01 & ~(_BIT1 | _BIT0)) | (y)))

#define GET_DP_TX_LANE1_ADJUST_VOL_SWING(x) ((x->TxLTInfo.ucAdjustReqLane01 & (_BIT5 | _BIT4)) >> 4)
#define SET_DP_TX_LANE1_ADJUST_VOL_SWING(x,y) (x->TxLTInfo.ucAdjustReqLane01 = ((x->TxLTInfo.ucAdjustReqLane01 & ~(_BIT5 | _BIT4)) | ((y) << 4)))

#define GET_DP_TX_LANE2_ADJUST_VOL_SWING(x) (x->TxLTInfo.ucAdjustReqLane23 & (_BIT1 | _BIT0))
#define SET_DP_TX_LANE2_ADJUST_VOL_SWING(x,y) (x->TxLTInfo.ucAdjustReqLane23 = ((x->TxLTInfo.ucAdjustReqLane23 & ~(_BIT1 | _BIT0)) | (y)))

#define GET_DP_TX_LANE3_ADJUST_VOL_SWING(x) ((x->TxLTInfo.ucAdjustReqLane23 & (_BIT5 | _BIT4)) >> 4)
#define SET_DP_TX_LANE3_ADJUST_VOL_SWING(x,y) (x->TxLTInfo.ucAdjustReqLane23 = ((x->TxLTInfo.ucAdjustReqLane23 & ~(_BIT5 | _BIT4)) | ((y) << 4)))

#define GET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(x) ((x->TxLTInfo.ucAdjustReqLane01 & (_BIT3 | _BIT2)) >> 2)
#define SET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucAdjustReqLane01 = ((x->TxLTInfo.ucAdjustReqLane01 & ~(_BIT3 | _BIT2)) | ((y) << 2)))

#define GET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(x) ((x->TxLTInfo.ucAdjustReqLane01 & (_BIT7 | _BIT6)) >> 6)
#define SET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucAdjustReqLane01 = ((x->TxLTInfo.ucAdjustReqLane01 & ~(_BIT7 | _BIT6)) | ((y) << 6)))

#define GET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(x) ((x->TxLTInfo.ucAdjustReqLane23 & (_BIT3 | _BIT2)) >> 2)
#define SET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucAdjustReqLane23 = ((x->TxLTInfo.ucAdjustReqLane23 & ~(_BIT3 | _BIT2)) | ((y) << 2)))

#define GET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(x) ((x->TxLTInfo.ucAdjustReqLane23 & (_BIT7 | _BIT6)) >> 6)
#define SET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(x,y) (x->TxLTInfo.ucAdjustReqLane23 = ((x->TxLTInfo.ucAdjustReqLane23 & ~(_BIT7 | _BIT6)) | ((y) << 6)))




static int GetBit(void __iomem *base, unsigned int offset, unsigned int mask)
{
	unsigned int reg;

	reg = readl(base + offset);
	reg &= mask;
	return reg;
}

static unsigned int GetReg(void __iomem *base, unsigned int offset)
{
	return readl(base + offset);
}

static void SetBit(void __iomem *base, unsigned int offset, unsigned int mask, unsigned int value)
{
	unsigned int reg;

	reg = readl(base + offset);
	reg &= mask;
	reg |= value;
	writel(reg, base + offset);
}

void SetReg(void __iomem *base, unsigned int offset, unsigned int value)
{
	writel(value, base + offset);
}

static void DpTxAuxReset(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;

	SetReg(base, PBB_A1_DP_RESET_CTRL, 0x20);
	SetReg(base, PBB_A1_DP_RESET_CTRL, 0x0);
}


static bool DpTxAuxWrite(struct rtk_dptx_hwinfo *hwinfo,
							unsigned int ucAddr,
							unsigned char ucLength,
							unsigned char *pucWriteArray)
{
	void __iomem *base = hwinfo->reg_base;

	int i;
	unsigned char ucAddrH, ucAddrM, ucAddrL;

	ucAddrH = (ucAddr>>16) & 0xFF;
	ucAddrM = (ucAddr>>8) & 0xFF;
	ucAddrL = ucAddr & 0xFF;
	//Clear fifo & interrupt
	SetBit(base, PBD_AB_AUX_FIFO_CTRL, ~(_BIT1 | _BIT0), (_BIT1 | _BIT0));
	SetReg(base, PBD_B1_AUX_IRQ_EVENT, 0x3F);
	//Command request
	SetReg(base, PBD_A4_AUXTX_REQ_CMD, ucAddrH);
	SetReg(base, PBD_A5_AUXTX_REQ_ADDR_M, ucAddrM);
	SetReg(base, PBD_A6_AUXTX_REQ_ADDR_L, ucAddrL);
	SetReg(base, PBD_A7_AUXTX_REQ_LEN, ((ucLength > 0) ? (ucLength - 1) : 0));
	for(i = 0; i < ucLength; i++)
		SetReg(base, PBD_A8_AUXTX_REQ_DATA, pucWriteArray[i]);
	//Start transfer
	SetBit(base, PBD_A3_AUXTX_TRAN_CTRL, ~_BIT0, _BIT0);
	
	if(down_timeout(&hwinfo->sem, 10)) {
		printk(KERN_ERR "%s time out\n", __FUNCTION__);
		return false;
	}

	if(hwinfo->aux_status)
		return true;
	else
		return false;
}

static bool DpTxAuxRead(struct rtk_dptx_hwinfo *hwinfo,
						unsigned int ucAddr,
						unsigned char ucLength,
						unsigned char *pucReadArray)
{
	void __iomem *base = hwinfo->reg_base;
	bool ret;
	int i;

	unsigned char ucAddrH, ucAddrM, ucAddrL;

	ucAddrH = (ucAddr>>16) & 0xFF;
	ucAddrM = (ucAddr>>8) & 0xFF;
	ucAddrL = ucAddr & 0xFF;
	
	SetReg(base, PBD_B1_AUX_IRQ_EVENT, 0x3F);
	SetBit(base, PBD_AB_AUX_FIFO_CTRL, ~(_BIT1 | _BIT0), (_BIT1 | _BIT0));
	SetReg(base, PBD_A4_AUXTX_REQ_CMD, ucAddrH);
	SetReg(base, PBD_A5_AUXTX_REQ_ADDR_M, ucAddrM);
	SetReg(base, PBD_A6_AUXTX_REQ_ADDR_L, ucAddrL);
	
	SetReg(base, PBD_A7_AUXTX_REQ_LEN, ((ucLength > 0) ? (ucLength - 1) : 0));
	SetBit(base, PBD_A3_AUXTX_TRAN_CTRL, ~_BIT0, _BIT0);

	if(down_timeout(&hwinfo->sem, 10)) {
		printk(KERN_ERR "%s time out\n", __FUNCTION__);
		ret = false;
	}

	if(hwinfo->aux_status) {
		for(i=0; i<ucLength; i++) {
			*pucReadArray = GetReg(base, PBD_AA_AUX_REPLY_DATA);
			pucReadArray++;
		}
		ret = true;
	} else
		ret = false;

	return ret;
}


bool DpTxAuxNativeWrite(struct rtk_dptx_hwinfo *hwinfo,
						unsigned int ucAddr,
						unsigned char ucLength,
						unsigned char *pucWriteArray)
{
	ucAddr = ucAddr | (0x80 << 16);
	return DpTxAuxWrite(hwinfo, ucAddr, ucLength, pucWriteArray);
}

bool DpTxAuxNativeRead(struct rtk_dptx_hwinfo *hwinfo,
						unsigned int ucAddr,
						unsigned char ucLength,
						unsigned char *pucReadArray)
{
	ucAddr = ucAddr | (0x90 << 16);	
	return DpTxAuxRead(hwinfo, ucAddr, ucLength, pucReadArray);
}

bool DpTxAuxI2CoverWrite(struct rtk_dptx_hwinfo *hwinfo,
						unsigned char ucSlaveAddress,
						unsigned char ucLength,
						unsigned char *pucReadArray,
						unsigned char bMOT)
{
	unsigned int ucAddr;

	ucAddr = ((0x00 | bMOT<<6) << 16) | 0x00 << 8 | ucSlaveAddress;

	if(DpTxAuxWrite(hwinfo, ucAddr, ucLength, pucReadArray)==false) {
		if(DpTxAuxWrite(hwinfo, ucAddr, ucLength, pucReadArray)==false) {
			return false;
		}
	}
	return true;
}

bool DpTxAuxI2CoverRead(struct rtk_dptx_hwinfo *hwinfo,
						unsigned char ucSlaveAddress,
						unsigned char ucLength,
						unsigned char *pucReadArray,
						unsigned char bMOT)
{
	unsigned int ucAddr;

	ucAddr = ((0x10 | bMOT<<6) << 16) | 0x00 << 8 | ucSlaveAddress;

	if(DpTxAuxRead(hwinfo, ucAddr, ucLength, pucReadArray)==false) {
		if(DpTxAuxRead(hwinfo, ucAddr, ucLength, pucReadArray)==false) {
			return false;
		}
	}
	return true;
}

bool DpTxLinkConfig(struct rtk_dptx_hwinfo *hwinfo)
{
	unsigned char data[16];

	memset(data, 0, sizeof(data));

	data[0] = 0x4C;
	data[1] = 0xE0;
	data[2] = 0x00;

	if(DpTxAuxNativeWrite(hwinfo, 0x300, 3, data)==false)
		return false;
	if(DpTxAuxNativeRead(hwinfo, 0x0, 16, data)==false)
		return false;
	
	// Step1:Get DownStream infromation
	if((data[0] != 0x10) && (data[0] != 0x11) && (data[0] != 0x12))
		data[0] = 0x11;

	SET_DP_TX_DOWNSTREAM_DPCD_REV(hwinfo, data[0]);

	if((data[1] == 0x06) || (data[1] == 0x0A) || ((data[0] == 0x12) && (data[1] == 0x14))) {
		if(data[1]>0xA)
			data[1] = 0xA;
		SET_DP_TX_DOWNSTREAM_MAX_LINK_RATE(hwinfo, data[1]);
	} else
		SET_DP_TX_DOWNSTREAM_MAX_LINK_RATE(hwinfo, 0x6);

//	SET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo, (data[2] & 0x1F));	
	if(hwinfo->out_type == DP_FORMAT_1080P_60 || hwinfo->out_type == DP_FORMAT_1440_768
			|| hwinfo->out_type == DP_FORMAT_1280_800 || hwinfo->out_type == DP_FORMAT_1440_900
			|| hwinfo->out_type == DP_FORMAT_960_544)
		SET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo, _DP_TWO_LANE);
	else if(hwinfo->out_type == DP_FORMAT_2160P_30 ||
			hwinfo->out_type == DP_FORMAT_2160P_60)
		SET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo, _DP_FOUR_LANE);
	else if(hwinfo->out_type == DP_FORMAT_720P_60 ||
			hwinfo->out_type == DP_FORMAT_1024_768)
		SET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo, _DP_ONE_LANE);

	SET_DP_TX_DOWNSTREAM_ENHANCE_FRAMING(hwinfo, (data[2] & _BIT7)>>7);

	return true;
}

int ReadEDID(struct rtk_dptx_hwinfo *hwinfo, unsigned char *ptr, int length)
{
	int i, ret;

	for(i=0; i<length; i++)
		*(ptr+i) = 0;
	
	ret = DpTxAuxI2CoverWrite(hwinfo, 0x50, 1, ptr, 0);
	if(ret==false)
		return -1;
	DpTxAuxI2CoverWrite(hwinfo, 0x50, 1, ptr, 1);
	for(i=0; i<length/16; i++)
		DpTxAuxI2CoverRead(hwinfo, 0x50, 16, (ptr+i*16), 1);
	return 0;
}

void DpTxPhyTxSetSignalLevel(struct rtk_dptx_hwinfo *hwinfo, unsigned char enumLaneX, unsigned char ucVoltageSwing, unsigned char ucPreEmphasis)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	unsigned int val;
	unsigned char *tbl, idx;
	
	tbl = tDPTX_DRV_TABLE;

	if(ucVoltageSwing + ucPreEmphasis > 3) {
		if(ucVoltageSwing < 4)
			ucPreEmphasis = 3 - ucVoltageSwing;
		else {
			ucVoltageSwing = 3;
			ucPreEmphasis = 0;
		}
	}
	idx = (ucVoltageSwing * 4 + ucPreEmphasis) * 3;
	val = readl(lvds_base+0x44);
	val = val & ~((0x1 << (16+enumLaneX)) | 0xF<<(enumLaneX*4));
	val = val | tbl[idx]<<(16+enumLaneX) | tbl[idx+1]<<(enumLaneX*4) | 0x00f00000;
	writel(val, lvds_base+0x44);
	
	val = readl(lvds_base+0x48);
	val = val & ~(0xF<<(enumLaneX*4)) & ~0xFF000000;
	val = val | tbl[idx+2]<<(enumLaneX*4) | 0x1F000000;
	writel(val, lvds_base+0x48);

	return;
}

void DpTxPhyTxSetSignalLevel1(struct rtk_dptx_hwinfo *hwinfo, int lanX, int swing, int deemp)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	unsigned int val, i;
	unsigned char *tbl, lv;
	
	tbl = tDPTX_DRV_TABLE1;
	lv = deemp;
	for(i=0; i<swing; i++)
		lv += 5-i;
	val = readl(lvds_base+0x44);
	val = val & ~(0x10000 | 0xF<<(lanX*4));
	val = val | tbl[lv*3]<<16 | tbl[lv*3+1]<<(lanX*4);
	writel(val, lvds_base+0x44);
//	printk(KERN_ERR "PhySet0 = 0x%x\n", val);

	val = readl(lvds_base+0x48);
	val = val & ~(0xF<<(lanX*4));
	val = val | tbl[lv*3+2]<<(lanX*4);
	writel(val, lvds_base+0x48);
//	printk(KERN_ERR "PhySet1 = 0x%x\n", val);
}

void DpTxPhyTxMLPHYSet(struct rtk_dptx_hwinfo *hwinfo)
{

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo))
	{
		case _DP_FOUR_LANE:
			DpTxPhyTxSetSignalLevel(hwinfo, _DP_TX_LANE_3,
							GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo),
							GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo));
			DpTxPhyTxSetSignalLevel(hwinfo, _DP_TX_LANE_2,
							GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo),
							GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo));
		case _DP_TWO_LANE:
			DpTxPhyTxSetSignalLevel(hwinfo, _DP_TX_LANE_1,
							GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo),
							GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo));
		case _DP_ONE_LANE:
		default:
			DpTxPhyTxSetSignalLevel(hwinfo, _DP_TX_LANE_0,
							GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo),
							GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo));
			break;
	}
}

void DpTxTP2Lane0Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo) + GET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo)));
	else {
		SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(hwinfo));
		SET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo));
	}
	if(GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);

	if(GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
}

void DpTxTP2Lane1Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo) + GET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo)));
	else {
		SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(hwinfo));
		SET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo));
	}
	if(GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(hwinfo);

	if(GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
}

void DpTxTP2Lane2Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo) + GET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo)));
	else {
		SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(hwinfo));
		SET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo));
	}
	if(GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(hwinfo);

	if(GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
}

void DpTxTP2Lane3Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo) + GET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo)));
	else {
		SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(hwinfo));
		SET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo));
	}
	if(GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);

	if(GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
}

bool DpTxTP1Lane0Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo) + GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo)));

	if(GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo) == GET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo)) {
		ADD_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo);
		if((GET_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo) == 5) || 
			((GET_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo) == 1) && (GET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3))) {
			CLR_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo);
			return false;
		}
	} else
		CLR_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo);

	SET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE0_ADJUST_VOL_SWING(hwinfo));
	SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE0_ADJUST_PRE_EMPHASIS(hwinfo));

	if(GET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	
	if(GET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	return true;
}

bool DpTxTP1Lane1Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo) + GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo)));

	if(GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo) == GET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo)) {
		ADD_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo);
		if((GET_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo) == 5) || 
			((GET_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo) == 1) && (GET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3))) {
			CLR_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo);
			return false;
		}
	} else
		CLR_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo);

	SET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE1_ADJUST_VOL_SWING(hwinfo));
	SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE1_ADJUST_PRE_EMPHASIS(hwinfo));

	if(GET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	
	if(GET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	return true;
}

bool DpTxTP1Lane2Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo) + GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo)));

	if(GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo) == GET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo)) {
		ADD_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo);
		if((GET_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo) == 5) || 
			((GET_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo) == 1) && (GET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3))) {
			CLR_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo);
			return false;
		}
	} else
		CLR_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo);

	SET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE2_ADJUST_VOL_SWING(hwinfo));
	SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE2_ADJUST_PRE_EMPHASIS(hwinfo));

	if(GET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	
	if(GET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	return true;
}

bool DpTxTP1Lane3Adjust(struct rtk_dptx_hwinfo *hwinfo)
{
	if((GET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo) + GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo)) > _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo, (_DP_TX_LEVEL_3 - GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo)));

	if(GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo) == GET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo)) {
		ADD_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo);
		if((GET_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo) == 5) || 
			((GET_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo) == 1) && (GET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3))) {
			CLR_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo);
			return false;
		}
	} else
		CLR_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo);

	SET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo, GET_DP_TX_LANE3_ADJUST_VOL_SWING(hwinfo));
	SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo, GET_DP_TX_LANE3_ADJUST_PRE_EMPHASIS(hwinfo));

	if(GET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	
	if(GET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo) == _DP_TX_LEVEL_3)
		SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);
	else
		CLR_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	return true;
}

void DpTxSignalReset(struct rtk_dptx_hwinfo *hwinfo)
{
	CLR_DP_TX_TP1_LANE0_SAME_VOL_CNT(hwinfo);
	CLR_DP_TX_TP1_LANE1_SAME_VOL_CNT(hwinfo);
	CLR_DP_TX_TP1_LANE2_SAME_VOL_CNT(hwinfo);
	CLR_DP_TX_TP1_LANE3_SAME_VOL_CNT(hwinfo);

	SET_DP_TX_LANE0_CURRENT_VOL_SWING(hwinfo, _DP_TX_LEVEL_0);
	CLR_DP_TX_LANE0_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	SET_DP_TX_LANE0_CURRENT_PRE_EMPHASIS(hwinfo, _DP_TX_LEVEL_0);
    CLR_DP_TX_LANE0_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	SET_DP_TX_LANE1_CURRENT_VOL_SWING(hwinfo, _DP_TX_LEVEL_0);
	CLR_DP_TX_LANE1_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	SET_DP_TX_LANE1_CURRENT_PRE_EMPHASIS(hwinfo, _DP_TX_LEVEL_0);
    CLR_DP_TX_LANE1_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	SET_DP_TX_LANE2_CURRENT_VOL_SWING(hwinfo, _DP_TX_LEVEL_0);
	CLR_DP_TX_LANE2_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	SET_DP_TX_LANE2_CURRENT_PRE_EMPHASIS(hwinfo, _DP_TX_LEVEL_0);
    CLR_DP_TX_LANE2_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	SET_DP_TX_LANE3_CURRENT_VOL_SWING(hwinfo, _DP_TX_LEVEL_0);
	CLR_DP_TX_LANE3_CURRENT_VOL_SWING_MAX_REACH(hwinfo);
	SET_DP_TX_LANE3_CURRENT_PRE_EMPHASIS(hwinfo, _DP_TX_LEVEL_0);
    CLR_DP_TX_LANE3_CURRENT_PRE_EMPHASIS_MAX_REACH(hwinfo);

	DpTxPhyTxMLPHYSet(hwinfo);
}

void DpTxSignalInitialSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	DpTxSignalReset(hwinfo);

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo))
	{
		case _DP_ONE_LANE:

			break;
	}
}

void DpTxTPInitialSet(struct rtk_dptx_hwinfo *hwinfo)
{
	unsigned char link_rate, link_count;
	
	SET_DP_TX_LT_RESULT(hwinfo, _DP_TX_TRAINING_NO_RESULT);
	SET_DP_TX_LT_STATE(hwinfo, _DP_TX_TRAINING_PATTERN1);

	link_rate = GET_DP_TX_DOWNSTREAM_MAX_LINK_RATE(hwinfo);
	SET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo, link_rate);

	DpTxSignalInitialSetting(hwinfo);

	//Set Link Rate
	DpTxAuxNativeWrite(hwinfo, 0x100, 1, &link_rate);

	link_count = GET_DP_TX_DOWNSTREAM_ENHANCE_FRAMING(hwinfo) << 7 | GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo);

	DpTxAuxNativeWrite(hwinfo, 0x101, 1, &link_count);
}

unsigned char DpTxTrainPattern1(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	unsigned char data[20];
	int i;

//	SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x13);
	for(i=0; i<20; i++)
		data[i] = 0;

	if(GET_DP_TX_LT_RESULT(hwinfo) == _DP_TX_TRAINING_NO_RESULT) {
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x10);
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x11);
		data[0] = _DP_TP_1 | _DP_SCRAMBLING_DISABLE;
		data[1] = hwinfo->TxLTInfo.ucLane0Adjust;
		data[2] = hwinfo->TxLTInfo.ucLane1Adjust;
		data[3] = hwinfo->TxLTInfo.ucLane2Adjust;
		data[4] = hwinfo->TxLTInfo.ucLane3Adjust;
		DpTxAuxNativeWrite(hwinfo, 0x102, 5, data);
	} else {
		DpTxPhyTxMLPHYSet(hwinfo);
		data[0] = hwinfo->TxLTInfo.ucLane0Adjust;
		data[1] = hwinfo->TxLTInfo.ucLane1Adjust;
		data[2] = hwinfo->TxLTInfo.ucLane2Adjust;
		data[3] = hwinfo->TxLTInfo.ucLane3Adjust;
		DpTxAuxNativeWrite(hwinfo, 0x103, 4, data);
	}

	mdelay(5);

	DpTxAuxNativeRead(hwinfo, 0x202, 6, data);
	printk(KERN_ERR "pattern1 : 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", data[0], data[1], data[2], data[4], data[5]);

	hwinfo->TxLTInfo.ucLane01LTStatus = data[0];
	hwinfo->TxLTInfo.ucLane23LTStatus = data[1];
	hwinfo->TxLTInfo.ucAdjustReqLane01 = data[4];
	hwinfo->TxLTInfo.ucAdjustReqLane23 = data[5];

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo)) {
		case _DP_FOUR_LANE:
			if((GET_DP_TX_LANE3_CR_DONE(hwinfo) & GET_DP_TX_LANE2_CR_DONE(hwinfo) 
				& GET_DP_TX_LANE1_CR_DONE(hwinfo) & GET_DP_TX_LANE0_CR_DONE(hwinfo)) == true)
				return _DP_TX_TRAINING_PATTERN1_PASS;
			break;
		case _DP_TWO_LANE:
			if((GET_DP_TX_LANE1_CR_DONE(hwinfo) & GET_DP_TX_LANE0_CR_DONE(hwinfo)) == true)
				return _DP_TX_TRAINING_PATTERN1_PASS;
			break;
		case _DP_ONE_LANE:
		default:
			if(GET_DP_TX_LANE0_CR_DONE(hwinfo) == true)
				return _DP_TX_TRAINING_PATTERN1_PASS;
			break;
	}

	if(GET_DP_TX_TP1_CNT(hwinfo) >= 5) {
		CLR_DP_TX_TP1_CNT(hwinfo);
		return _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL;
	} else
		ADD_DP_TX_TP1_CNT(hwinfo);

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo)) {
		case _DP_FOUR_LANE:
			if(DpTxTP1Lane3Adjust(hwinfo) == false)
				return _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL;
			if(DpTxTP1Lane2Adjust(hwinfo) == false)
				return _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL;
		case _DP_TWO_LANE:
			if(DpTxTP1Lane1Adjust(hwinfo) == false)
				return _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL;
		case _DP_ONE_LANE:
		default:
			if(DpTxTP1Lane0Adjust(hwinfo) == false)
				return _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL;
			break;
	}

	return _DP_TX_TRAINING_PATTERN1_FAIL;
}

unsigned char DpTxTrainPattern2(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	unsigned char data[20];

	if(GET_DP_TX_LT_RESULT(hwinfo) == _DP_TX_TRAINING_PATTERN1_PASS) {
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x20);
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x21);

		data[0] = _DP_TP_2 | _DP_SCRAMBLING_DISABLE;
		data[1] = hwinfo->TxLTInfo.ucLane0Adjust;
		data[2] = hwinfo->TxLTInfo.ucLane1Adjust;
		data[3] = hwinfo->TxLTInfo.ucLane2Adjust;
		data[4] = hwinfo->TxLTInfo.ucLane3Adjust;
		DpTxAuxNativeWrite(hwinfo, 0x102, 5, data);
	} else {
		DpTxPhyTxMLPHYSet(hwinfo);
		data[0] = hwinfo->TxLTInfo.ucLane0Adjust;
		data[1] = hwinfo->TxLTInfo.ucLane1Adjust;
		data[2] = hwinfo->TxLTInfo.ucLane2Adjust;
		data[3] = hwinfo->TxLTInfo.ucLane3Adjust;
		DpTxAuxNativeWrite(hwinfo, 0x103, 4, data);
	}

	mdelay(5);

	DpTxAuxNativeRead(hwinfo, 0x202, 6, data);
	hwinfo->TxLTInfo.ucLane01LTStatus = data[0];
	hwinfo->TxLTInfo.ucLane23LTStatus = data[1];
	hwinfo->TxLTInfo.ucLaneAlignStatus = data[2];
	hwinfo->TxLTInfo.ucAdjustReqLane01 = data[4];
	hwinfo->TxLTInfo.ucAdjustReqLane23 = data[5];
	printk(KERN_ERR "pattern2 : 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", data[0], data[1], data[2], data[4], data[5]);

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo)) {
		case _DP_FOUR_LANE:
			if((GET_DP_TX_LANE3_CR_DONE(hwinfo) & GET_DP_TX_LANE2_CR_DONE(hwinfo) 
				& GET_DP_TX_LANE1_CR_DONE(hwinfo) & GET_DP_TX_LANE0_CR_DONE(hwinfo)) != true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_ADJUST_FAIL;
			}
			if((GET_DP_TX_LANE3_EQ_DONE(hwinfo) & GET_DP_TX_LANE2_EQ_DONE(hwinfo) 
				& GET_DP_TX_LANE1_EQ_DONE(hwinfo) & GET_DP_TX_LANE0_EQ_DONE(hwinfo)) == true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_PASS;
			}
			break;
		case _DP_TWO_LANE:
			if((GET_DP_TX_LANE1_CR_DONE(hwinfo) & GET_DP_TX_LANE0_CR_DONE(hwinfo)) != true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_ADJUST_FAIL;
			}
			if((GET_DP_TX_LANE1_EQ_DONE(hwinfo) & GET_DP_TX_LANE0_EQ_DONE(hwinfo)) == true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_PASS;
			}
			break;
		case _DP_ONE_LANE:
		default:
			if(GET_DP_TX_LANE0_CR_DONE(hwinfo) != true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_ADJUST_FAIL;
			}
			if(GET_DP_TX_LANE0_EQ_DONE(hwinfo) == true) {
				CLR_DP_TX_TP2_CNT(hwinfo);
				return _DP_TX_TRAINING_PATTERN2_PASS;
			}
			break;
	}
	if(GET_DP_TX_TP2_CNT(hwinfo) >= 5) {
		CLR_DP_TX_TP2_CNT(hwinfo);
		return _DP_TX_TRAINING_PATTERN2_ADJUST_FAIL;
	} else
		ADD_DP_TX_TP2_CNT(hwinfo);

	switch(GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo)) {
		case _DP_FOUR_LANE:
			DpTxTP2Lane3Adjust(hwinfo);
			DpTxTP2Lane2Adjust(hwinfo);
		case _DP_TWO_LANE:
			DpTxTP2Lane1Adjust(hwinfo);
		case _DP_ONE_LANE:
		default:
			DpTxTP2Lane0Adjust(hwinfo);
			break;
	}

	return _DP_TX_TRAINING_PATTERN2_FAIL;
}

void DpTxTrainPatternEnd(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	unsigned char data[20];

	data[0] = _DP_TP_NONE;
	DpTxAuxNativeWrite(hwinfo, 0x102, 1, data);

	if(GET_DP_TX_LT_RESULT(hwinfo) == _DP_TX_TRAINING_PASS) {
#if RTK_VO_SET
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x40);
		SetReg(base, PBB_01_DPTX_ML_PAT_SEL, 0x41);
#endif
	}
}

int DpTxLinkTraining(struct rtk_dptx_hwinfo *hwinfo)
{
	unsigned char data[20];
	unsigned char ret;
	
	if(DpTxLinkConfig(hwinfo)==false) {
		printk(KERN_ERR "DpTxLinkConfig fail\n");
		return -1;
	}
	//Power up Downstream
	data[0] = 0x1;
	DpTxAuxNativeWrite(hwinfo, 0x600, 1, data);
	
//	DpTxAuxNativeRead(hwinfo, 0x202, 3, data);

	DpTxTPInitialSet(hwinfo);
	while(GET_DP_TX_LT_RESULT(hwinfo)!=_DP_TX_TRAINING_PASS && 
			GET_DP_TX_LT_RESULT(hwinfo)!=_DP_TX_TRAINING_FAIL)
	{
		switch(GET_DP_TX_LT_STATE(hwinfo))
		{
			case _DP_TX_TRAINING_PATTERN1:
				ret = DpTxTrainPattern1(hwinfo);
				SET_DP_TX_LT_RESULT(hwinfo, ret);
				if(ret == _DP_TX_TRAINING_PATTERN1_PASS) {
					printk(KERN_ERR "[dptx] pattern1 success\n");
					SET_DP_TX_LT_STATE(hwinfo, _DP_TX_TRAINING_PATTERN2);
				} else if(ret == _DP_TX_TRAINING_PATTERN1_ADJUST_FAIL) {
					printk(KERN_ERR "[dptx] pattern1 fail\n");
					SET_DP_TX_LT_RESULT(hwinfo, _DP_TX_TRAINING_FAIL);
					DpTxTrainPatternEnd(hwinfo);
					return -1;
				}
				break;
			case _DP_TX_TRAINING_PATTERN2:
				ret = DpTxTrainPattern2(hwinfo);
				SET_DP_TX_LT_RESULT(hwinfo, ret);
				if(ret == _DP_TX_TRAINING_PATTERN2_PASS) {
					printk(KERN_ERR "[dptx] pattern2 success\n");
					SET_DP_TX_LT_STATE(hwinfo, _DP_TX_TRAINING_PATTERN_END);
				} else if(ret == _DP_TX_TRAINING_PATTERN2_ADJUST_FAIL) {
					printk(KERN_ERR "[dptx] pattern2 fail\n");
					SET_DP_TX_LT_RESULT(hwinfo, _DP_TX_TRAINING_FAIL);
					DpTxTrainPatternEnd(hwinfo);
					return -1;
				}
				break;
			case _DP_TX_TRAINING_PATTERN_END:
				printk(KERN_ERR "[dptx] pass link training\n");
				SET_DP_TX_LT_RESULT(hwinfo, _DP_TX_TRAINING_PASS);
				DpTxTrainPatternEnd(hwinfo);
				break;
		}
	}
	return 0;
}

void DpTxPixelPLLSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *pll_base = hwinfo->pll_base;
	unsigned int reg;

	// Pixel PLL setting
	reg = readl(pll_base + 0x190);
	reg |= (0x1 << 3);
	writel(reg, pll_base + 0x190);
	mdelay(1);

	reg = readl(pll_base + 0x230);
	reg &= ~(0x1 << 6);
	writel(reg, pll_base + 0x230);
	mdelay(1);

	if(hwinfo->out_type == DP_FORMAT_2160P_60) {
		writel(0x93470182, pll_base + 0x250);
		mdelay(1);
		writel(0x93474182, pll_base + 0x250);
		mdelay(1);
		writel(0x1b815, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_2160P_30) {
		writel(0x93470182, pll_base + 0x250);
		mdelay(1);
		writel(0x93474182, pll_base + 0x250);
		mdelay(1);
		writel(0xF000, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_1080P_60) {
		writel(0x93470192, pll_base + 0x250);
		mdelay(1);
		writel(0x93474192, pll_base + 0x250);
		mdelay(1);
		writel(0xF000, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_1024_768) {
		writel(0x934701a2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741a2, pll_base + 0x250);
		mdelay(1);
		writel(0xcf1c, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_1440_768) {
		writel(0x934701a2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741a2, pll_base + 0x250);
		mdelay(1);
		writel(0xe800, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_1440_900) {
		writel(0x934701a2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741a2, pll_base + 0x250);
		mdelay(1);
		writel(0x11800, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_1280_800) {
		writel(0x934701a2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741a2, pll_base + 0x250);
		mdelay(1);
		writel(0xdaab, pll_base + 0x604);
		mdelay(1);
	} else if(hwinfo->out_type == DP_FORMAT_960_544) {
		writel(0x934701b2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741b2, pll_base + 0x250);
		mdelay(1);
		writel(0xD800, pll_base + 0x604);
		mdelay(1);
	} else {
		writel(0x934701a2, pll_base + 0x250);
		mdelay(1);
		writel(0x934741a2, pll_base + 0x250);
		mdelay(1);
		writel(0xF000, pll_base + 0x604);
		mdelay(1);
	}

	writel(0x1e01, pll_base + 0x254);
	mdelay(1);
	writel(0x501008, pll_base + 0x608);
	mdelay(1);
	writel(0xd, pll_base + 0x600);
	mdelay(1);
}

void DpTxDpPLLSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *pll_base = hwinfo->pll_base;
	void __iomem *lvds_base = hwinfo->lvds_base;
	unsigned int reg;

	// Disable iso control
	writel(0x440000, pll_base + 0x24);
	mdelay(1);
	writel(0x1003301, lvds_base);
	mdelay(1);
	writel(0x420000, pll_base + 0x24);
	mdelay(1);
	
	// DP PLL setting
//	writel(0x9A333007, pll_base + 0x248);
	writel(0x9A356007, pll_base + 0x248);
	mdelay(1);
	writel(0x3, pll_base + 0x24c);
	mdelay(1);
	writel(0xc, pll_base + 0x5e0);
	mdelay(1);
	writel(0x30800, pll_base + 0x5e4);
	mdelay(1);
	writel(0x30480, pll_base + 0x5ec);
	mdelay(1);
//	writel(0x61<<11, pll_base + 0x5e4);
//	mdelay(1);
//	reg = readl(pll_base+0x5ec);
//	reg |= 1152 | 96<<11;
//	writel(reg, pll_base + 0x5ec);
//	mdelay(1);

//	writel(0x1300, pll_base + 0x5f0);
	writel(0x1100, pll_base + 0x5f0);
	mdelay(1);

	writel(0x501068, pll_base + 0x5e8);
	mdelay(1);
	writel(0xd, pll_base + 0x5e0);
	mdelay(1);
	reg = readl(pll_base + 0x5fc);
	mdelay(1);
	printk(KERN_ERR "reg = 0x%x\n", reg);
#ifdef SS_EN
	writel(0xc, pll_base + 0x5e0);
	writel(0xe, pll_base + 0x5e0);
	SET_DP_TX_DOWNSTREAM_DOWN_SPREAD(hwinfo, 1);
#else
	SET_DP_TX_DOWNSTREAM_DOWN_SPREAD(hwinfo, 0);
#endif
}

static void DpTxAuxPHYSet(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	
	SetBit(base, PBD_61_AUX_1, ~(_BIT7 | _BIT6 | _BIT5), _BIT7);
	SetBit(base, PBD_61_AUX_1, ~(_BIT3 | _BIT2 | _BIT1 | _BIT0), (_BIT3 | _BIT2 | _BIT1 | _BIT0));
	SetBit(base, PBD_62_AUX_2, ~(_BIT5 | _BIT0), _BIT5 |_BIT0);
	SetBit(base, PBD_66_AUX_6, ~(_BIT4 | _BIT3 | _BIT2 | _BIT1 | _BIT0), (_BIT4 | _BIT3 | _BIT2 | _BIT1 | _BIT0));
	SetReg(base, PBD_67_DIG_TX_03, 0xFF);
}

void DpTxInitial(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;

	DpTxAuxPHYSet(hwinfo);
//	SetReg(base, PBD_64_AUX_4, 0xCA);
	// Set ECF by FW mode
	SetBit(base, PBB_64_HDCP_ECF_BYTE0, ~_BIT7, 0x00);
	// Set Aux Timeout
	SetReg(base, PBD_A1_AUX_TIMEOUT, 0x95);
	// Power On Aux CH
	SetBit(base, PBD_A0_AUX_TX_CTRL, ~_BIT0, _BIT0);

//	SetBit(base, PBD_65_AUX_5, ~_BIT7, 0);
	// Sync end Pattern Error Handle Disable
	SetBit(base, PBD_F6_AUX_DIG_PHY8, ~_BIT1, _BIT1);
	// Enable Short IRQ and Disable Long IRQ
	SetBit(base, PBB_72_HPD_IRQ_EN, ~(_BIT6 | _BIT5), _BIT6);
	// Set DP TX CLK divider
	SetReg(base, PBB_A3_DPTX_IRQ_CTRL, 0x80);

	SetBit(base, PBB_C9_ARBITER_CTRL, ~_BIT0, _BIT0);

//	SetReg(base, PBB_E0_DPTX_CLK_GEN, 0x21);
	SetBit(base, PBD_B2_AUX_IRQ_EN, 0, _BIT0 | _BIT1 | _BIT2 | _BIT3 | _BIT4 | _BIT5 | _BIT6 | _BIT7);

	SetBit(base, PBB_C7_VBID, ~_BIT1, _BIT1);
//	SetReg(base, PBD_AF_AUX_RETRY_1, 0x2);
//	SetBit(base, PBD_B0_AUX_RETRY_2, ~(_BIT7 | _BIT6 | _BIT5 | _BIT4 | _BIT3), _BIT7 | _BIT5);
}

static BYTE DpTxAudioGetSecDataParity(BYTE ucHeader)
{
	BYTE ucParity = 0;

	ucParity |= ((BYTE)(GET_DATA_BIT1(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT6(ucHeader))) << 7;
	ucParity |= ((BYTE)(GET_DATA_BIT0(ucHeader) ^ GET_DATA_BIT1(ucHeader) ^ GET_DATA_BIT3(ucHeader) ^ GET_DATA_BIT5(ucHeader))) << 6;
	ucParity |= ((BYTE)(GET_DATA_BIT0(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT4(ucHeader) ^ GET_DATA_BIT7(ucHeader))) << 5;
	ucParity |= ((BYTE)(GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT3(ucHeader) ^ GET_DATA_BIT7(ucHeader))) << 4;
	ucParity |= ((BYTE)(GET_DATA_BIT1(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT3(ucHeader) ^ GET_DATA_BIT6(ucHeader) ^ GET_DATA_BIT7(ucHeader))) << 3;
	ucParity |= ((BYTE)(GET_DATA_BIT0(ucHeader) ^ GET_DATA_BIT1(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT3(ucHeader) ^ GET_DATA_BIT5(ucHeader) ^ GET_DATA_BIT6(ucHeader))) << 2;
	ucParity |= ((BYTE)(GET_DATA_BIT0(ucHeader) ^ GET_DATA_BIT1(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT4(ucHeader) ^ GET_DATA_BIT5(ucHeader) ^ GET_DATA_BIT7(ucHeader))) << 1;
	ucParity |= (GET_DATA_BIT0(ucHeader) ^ GET_DATA_BIT2(ucHeader) ^ GET_DATA_BIT3(ucHeader) ^ GET_DATA_BIT4(ucHeader) ^ GET_DATA_BIT7(ucHeader));

	return ucParity;
}

static void DpTxSetSecDataHeader(struct rtk_dptx_hwinfo *hwinfo, EnumDPTxSecDataPacketType enumPackettype)
{
	void __iomem *base = hwinfo->reg_base;
	BYTE pucData[4];

	memset(pucData, 0, sizeof(pucData));
	pucData[0] = 0x00;
	pucData[1] = enumPackettype;
	SetBit(base, PBC_7B_SEC_PH_PACKET_TYPE, ~(_BIT3 | _BIT2 | _BIT1 | _BIT0), pucData[1]);
	switch(enumPackettype)
	{
		case _SEC_PACKET_TYPE_AUDIO_TIMESTAMP:
			pucData[2] = 0x17;
			pucData[3] = 0x44; //(_DP_VERSION_1_1 << 2);
			break;
		case _SEC_PACKET_TYPE_VSC:
			pucData[2] = 0x01;
			pucData[3] = 0x01;
			break;
		case _SEC_PACKET_TYPE_INFOFRAME_AVI:
		case _SEC_PACKET_TYPE_INFOFRAME_AUDIO:
		case _SEC_PACKET_TYPE_INFOFRAME_MPEG:
			pucData[1] = 0x84;
			pucData[2] = 0x1B;
			pucData[3] = 0x44;


			SetReg(base, PBC_26_SEC_INFO_AUD_DB0, 0x11);
			SetReg(base, PBC_27_SEC_INFO_AUD_DB1, 0x0F);


			break;
		case _SEC_PACKET_TYPE_AUDIO_STREAM:
			pucData[2] = 0x00;
			pucData[3] = 0x01;
			break;
		case _SEC_PACKET_TYPE_INFOFRAME_RSV:
			pucData[1] = 0x83;
			pucData[2] = 0x01;
			pucData[3] = 0x08;
			break;
		default:
			break;
	}
	// Set Header Byte
	SetReg(base, PBC_73_SEC_PH_HB0, pucData[0]);
	SetReg(base, PBC_74_SEC_PH_HB1, pucData[1]);
	SetReg(base, PBC_75_SEC_PH_HB2, pucData[2]);
	SetReg(base, PBC_76_SEC_PH_HB3, pucData[3]);
	// Set Parity Byte
	SetReg(base, PBC_77_SEC_PH_PB0, DpTxAudioGetSecDataParity(pucData[0]));
	SetReg(base, PBC_78_SEC_PH_PB1, DpTxAudioGetSecDataParity(pucData[1]));
	SetReg(base, PBC_79_SEC_PH_PB2, DpTxAudioGetSecDataParity(pucData[2]));
	SetReg(base, PBC_7A_SEC_PH_PB3, DpTxAudioGetSecDataParity(pucData[3]));
	// Set Double Buffer
	SetBit(base, PBC_02_SEC_DBUF_CTRL, ~(_BIT1 | _BIT0), (_BIT1 | _BIT0));
}

static void DpTxAudioSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	BYTE pucData[6];

	// Set 2 channel layout and audio source from I2S
	SetBit(base, PBC_18_AUD_FUNCTION_CTRL1, ~(_BIT7 | _BIT6), 0x00);
	pucData[0] = 20;

	((WORD *)pucData)[0] = 2 + ((8 + pucData[0]) / GET_DP_TX_DOWNSTREAM_LANE_NUM(hwinfo));
//	((WORD *)pucData)[1] = (DWORD)g_stDPTxInputTimingInfo.usHTotal * 270 * GET_DP_TX_DOWNSTREAM_LINK_RATE() / GET_DP_TX_INPUT_PIXEL_CLK();
	((WORD *)pucData)[1] = (DWORD)(((WORD *)pucData)[1] - GET_DP_TX_HORIZONTAL_DE_PERIOD(hwinfo)) * 7 / 10;
	if(((WORD *)pucData)[1] > ((WORD *)pucData)[0])
		((WORD *)pucData)[2] = ((WORD *)pucData)[1] - ((WORD *)pucData)[0];
	else
		((WORD *)pucData)[2] = 0;
	
//	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, pucData[4]);
//	SetReg(base, PBB_D5_ARBITER_SEC_END_CNT_LB, pucData[5]);
	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, 0x00);
	SetReg(base, PBB_D5_ARBITER_SEC_END_CNT_LB, 0x10);
}

void DpTxSSTAudioPlaySetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;

	SetBit(base, PBC_00_SEC_FUNCTION_CTRL, ~_BIT0, 0x00);


	SetReg(base, PBC_47_AUD_FUNCTION_CTRL2, 0x4F);
	SetReg(base, PBC_48_AUD_FUNCTION_CTRL3, 0x04);
	SetReg(base, PBC_19_AUD_PAYLOAD_B3, 0x80);


	DpTxAudioSetting(hwinfo);

	// Set Maud	
	SetReg(base, PBC_20_AUD_TS_MAUD_H, 0x0);
	SetReg(base, PBC_21_AUD_TS_MAUD_M, 0x2);
	SetReg(base, PBC_22_AUD_TS_MAUD_L, 0x0);
	// Sed Naud
	SetReg(base, PBC_23_AUD_TS_NAUD_H, 0x0);
	SetReg(base, PBC_24_AUD_TS_NAUD_M, 0x15);
	SetReg(base, PBC_25_AUD_TS_NAUD_L, 0xf9);

	// Set Header for Audio Timestamp
	DpTxSetSecDataHeader(hwinfo, _SEC_PACKET_TYPE_AUDIO_TIMESTAMP);
	// Set Header for Audio Info-frame
	DpTxSetSecDataHeader(hwinfo, _SEC_PACKET_TYPE_INFOFRAME_AUDIO);
	// Set Header for Audio Stream
	DpTxSetSecDataHeader(hwinfo, _SEC_PACKET_TYPE_AUDIO_STREAM);

	SetBit(base, PBC_00_SEC_FUNCTION_CTRL, ~(_BIT3 | _BIT1 | _BIT0), (_BIT3 | _BIT1 | _BIT0));
	SetBit(base, PBC_8F_DPTX_I2S_CTRL, ~(_BIT7 | _BIT6 | _BIT5 | _BIT4 | _BIT1 | _BIT0), (_BIT7 | _BIT1 | _BIT0));

	SetReg(base, PBC_85_CH_STATUS_0, 0x04);
	SetReg(base, PBC_86_CH_STATUS_1, 0x00);
	SetReg(base, PBC_87_CH_STATUS_2, 0x00);
	SetReg(base, PBC_88_CH_STATUS_3, 0x02);
	SetReg(base, PBC_89_CH_STATUS_4, 0x0b);


	SetBit(base, PBB_C7_VBID, ~_BIT2, 0x00);
}

void DpTxSSTDisplayFormatSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	// Disable HW Auto Calculate H Delay
	SetBit(base, PBB_D3_LFIFO_WL_SET, ~_BIT7, 0x00);

	SetBit(base, PBB_A1_DP_RESET_CTRL, ~_BIT7, _BIT7);
	SetBit(base, PBB_A1_DP_RESET_CTRL, ~_BIT7, 0x00);

//	SetReg(base, PBB_CC_TU_SIZE, _DP_TX_SST_TU_SIZE);
	if(hwinfo->out_type == DP_FORMAT_2160P_60) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x3e);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x0);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0x7);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x80);
	} else if(hwinfo->out_type == DP_FORMAT_1024_768) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x2e);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x01);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0xc);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x00);
	} else if(hwinfo->out_type == DP_FORMAT_720P_60) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x35);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x0);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0xf);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x00);
	} else if(hwinfo->out_type == DP_FORMAT_1440_768) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x19);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x6);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0x8);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x70);
	} else if(hwinfo->out_type == DP_FORMAT_1440_900) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x1e);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x0);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0x8);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x70);
	} else if(hwinfo->out_type == DP_FORMAT_1280_800) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x17);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x8);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0x7);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x80);
	} else if(hwinfo->out_type == DP_FORMAT_960_544) {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x0c);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x4);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0x5);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0xa0);
	} else {
		SetReg(base, PBB_CD_TU_DATA_SIZE0, 0x35);
		SetReg(base, PBB_CE_TU_DATA_SIZE1, 0x0);
		SetReg(base, PBB_CA_V_DATA_PER_LINE0, 0xb);
		SetReg(base, PBB_CB_V_DATA_PER_LINE1, 0x40);
	}
//	SetBit(base, PBB_D3_LFIFO_WL_SET, ~_BIT7, _BIT7);
	SetReg(base, PBB_D3_LFIFO_WL_SET, 0xC0);
}

static void DpTxSSTMSASetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;

	SetBit(base, PBB_A1_DP_RESET_CTRL, ~_BIT6, _BIT6);
	SetBit(base, PBB_A1_DP_RESET_CTRL, ~_BIT6, 0x00);

	if(hwinfo->out_type == DP_FORMAT_2160P_30) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x13);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x30);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x01);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x81);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0xf);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x00);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x58);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x8);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0xca);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0x53);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x8);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x70);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0xa);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_2160P_60) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0xf);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x50);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x49);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0xf);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x00);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x20);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x8);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0xae);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0xf);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x8);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x70);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x8);
		SetReg(base, PBB_B5_MSA_MISC0, 0x3a);
	} else if(hwinfo->out_type == DP_FORMAT_1080P_60) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x8);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x98);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x00);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0xC1);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x7);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x80);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x2C);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x4);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x65);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0x2A);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x4);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x38);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x5);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_720P_60) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x6);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x72);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x01);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x33);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x5);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x00);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x28);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x2);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0xee);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0x1a);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x2);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0xd0);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x5);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_1024_768) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x5);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x40);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x1);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x29);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x4);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x00);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x80);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x88);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x3);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x26);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0x24);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x3);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x00);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x80);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x6);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_1440_768) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x5);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0xf0);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x0);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x49);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x5);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0xa0);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x20);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x3);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x16);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0xf);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x3);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x0);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x8);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_1440_900) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x5);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0xf0);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x0);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x49);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x5);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0xa0);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x20);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x3);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x9e);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0xf);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x3);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x84);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x8);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_1280_800) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x5);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x50);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x0);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x49);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x5);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0x00);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x20);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x3);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x37);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0xf);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x3);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x20);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x8);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	} else if(hwinfo->out_type == DP_FORMAT_960_544) {
		SetReg(base, PBB_B7_MN_STRM_ATTR_HTT_M, 0x4);
		SetReg(base, PBB_B8_MN_STRM_ATTR_HTT_L, 0x10);
		SetReg(base, PBB_B9_MN_STRM_ATTR_HST_M, 0x0);
		SetReg(base, PBB_BA_MN_STRM_ATTR_HST_L, 0x49);
		SetReg(base, PBB_BB_MN_STRM_ATTR_HWD_M, 0x3);
		SetReg(base, PBB_BC_MN_STRM_ATTR_HWD_L, 0xc0);
		SetReg(base, PBB_BD_MN_STRM_ATTR_HSW_M, 0x0);
		SetReg(base, PBB_BE_MN_STRM_ATTR_HSW_L, 0x20);
		SetReg(base, PBB_BF_MN_STRM_ATTR_VTTE_M, 0x2);
		SetReg(base, PBB_C0_MN_STRM_ATTR_VTTE_L, 0x30);
		SetReg(base, PBB_C2_MN_STRM_ATTR_VST_L, 0xf);
		SetReg(base, PBB_C3_MN_STRM_ATTR_VHT_M, 0x2);
		SetReg(base, PBB_C4_MN_STRM_ATTR_VHT_L, 0x20);
		SetReg(base, PBB_C5_MN_STRM_ATTR_VSW_M, 0x0);
		SetReg(base, PBB_C6_MN_STRM_ATTR_VSW_L, 0x8);
		SetReg(base, PBB_B5_MSA_MISC0, 0x20);
	}
	SetBit(base, PBB_B4_MSA_CTRL, ~_BIT7, _BIT7);
}

void DpTxSSTSetting(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	bool bMvidHWMode = true;
	BYTE ucCnt = 0;
	BYTE ucValidHwMvidMeaCnt = 0;
	WORD usLoopCnt = 0;
	WORD usMeasureTime = 0;
	DWORD ulHwMvidMin = 0xFFFFFFFF;
	DWORD ulHwMvidMax = 0x00000000;
	
	DWORD pucData[6];

//	SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~(_BIT7 | _BIT6), _BIT6 | _BIT7);
#if 1
	pucData[0] = 0;
	pucData[1] = 0x8000;
	pucData[2] = 0;
	pucData[3] = 0;
	SET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo, 1);
	pucData[0] = (GET_DP_TX_INPUT_PIXEL_CLK(hwinfo) * pucData[1]);
	pucData[0] /= ((GET_DP_TX_DOWNSTREAM_DOWN_SPREAD(hwinfo) == true) ?
					((DWORD)270 * GET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo) * 9975 / 10000) : ((DWORD)270 * GET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo)));
	printk(KERN_ERR "[DPTX] pucData[0] = 0x%x\n", pucData[0]);
	usMeasureTime = (WORD)(pucData[1] / ((GET_DP_TX_DOWNSTREAM_DOWN_SPREAD(hwinfo) == true) ? 
					((DWORD)27 * GET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo) * 9975 / 10000) : ((DWORD)27 * GET_DP_TX_DOWNSTREAM_LINK_RATE(hwinfo))) / 5);
	usMeasureTime *= 2;

	// Debounce 10 times
	ucCnt = 0x0A;
	do {
		--ucCnt;
		// Enable HW Mvid measure
		SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~_BIT7, _BIT7);
		// Delay time for one HW mvid measurement period time
		for(usLoopCnt = 0; usLoopCnt < usMeasureTime; usLoopCnt++)
			udelay(5);
		// Disable HW Mvid measure
		SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~_BIT7, 0x00);

		pucData[2] = (((DWORD)GetReg(base, PBB_AF_MVID_AUTO_H) << 16) | ((DWORD)GetReg(base, PBB_B0_MVID_AUTO_M) << 8) | GetReg(base, PBB_B1_MVID_AUTO_L));
		printk(KERN_ERR "[DPTX] pucData[2] = 0x%x\n", pucData[2]);
		if(pucData[2] == 0) {
			printk(KERN_ERR "[DPTX] bMvidHWMode = false\n");
			SetReg(base, PBB_A9_MN_M_VID_H, *((unsigned char *)pucData+2));
			SetReg(base, PBB_AA_MN_M_VID_M, *((unsigned char *)pucData+1));
			SetReg(base, PBB_AB_MN_M_VID_L, *((unsigned char *)pucData+0));

			bMvidHWMode = false;
			break;
		}
		if(abs(pucData[0] - pucData[2]) > pucData[0] >> 2)
			continue;

		if(pucData[3] == 0) {
			if(pucData[2] < ulHwMvidMin)
				ulHwMvidMin = pucData[2];
			if(pucData[2] > ulHwMvidMax)
				ulHwMvidMax = pucData[2];
			pucData[3] = pucData[2];

			ucValidHwMvidMeaCnt++;
			continue;
		}
		if(abs(((pucData[3] + (ucValidHwMvidMeaCnt / 2)) / ucValidHwMvidMeaCnt) - pucData[2]) < 0x50) {
			if(pucData[2] < ulHwMvidMin)
				ulHwMvidMin = pucData[2];
			if(pucData[2] > ulHwMvidMax)
				ulHwMvidMax = pucData[2];
			pucData[3] += pucData[2];

			ucValidHwMvidMeaCnt++;
		}
	} while(ucCnt > 0);

	if(bMvidHWMode == true) {
		if(ucValidHwMvidMeaCnt > 2) {
			pucData[3] -= ulHwMvidMax + ulHwMvidMin;
			ucValidHwMvidMeaCnt -= 2;
			pucData[3] = ((pucData[3] + (ucValidHwMvidMeaCnt / 2)) / ucValidHwMvidMeaCnt);
			if(abs(pucData[0] - pucData[3]) > (pucData[0] >> 1))
				pucData[3] = pucData[0];
		} else
			pucData[3] = pucData[0];

		SetReg(base, PBB_A9_MN_M_VID_H, *((unsigned char *)pucData+14));
		SetReg(base, PBB_AA_MN_M_VID_M, *((unsigned char *)pucData+13));
		SetReg(base, PBB_AB_MN_M_VID_L, *((unsigned char *)pucData+12));
		printk(KERN_ERR "[DPTX] 0x%x, 0x%x, 0x%x\n", pucData[3], *((unsigned char *)pucData+12), *((unsigned char *)pucData+13));
	}
	SetReg(base, PBB_AC_MN_N_VID_H, *((unsigned char *)pucData+6));
	SetReg(base, PBB_AD_MN_N_VID_M, *((unsigned char *)pucData+5));
	SetReg(base, PBB_AE_MN_N_VID_L, *((unsigned char *)pucData+4));
	printk(KERN_ERR "[DPTX] 0x%x, 0x%x\n", *((unsigned char *)pucData+4), *((unsigned char *)pucData+5));
	
	SetBit(base, PBB_B4_MSA_CTRL, ~_BIT6, 0x00);
	SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~(_BIT7 | _BIT6), _BIT6);
#else
	if(hwinfo->out_type == DP_FORMAT_1080P_60) {
		SetReg(base, PBB_AA_MN_M_VID_M, 0x46);
		SetReg(base, PBB_AB_MN_M_VID_L, 0x8E);
//		SetReg(base, PBB_AB_MN_M_VID_L, 0x67);
	} else if(hwinfo->out_type == DP_FORMAT_2160P_30) {
		SetReg(base, PBB_AA_MN_M_VID_M, 0x8D);
		SetReg(base, PBB_AB_MN_M_VID_L, 0x1C);
	} else {
		SetReg(base, PBB_AA_MN_M_VID_M, 0x1e);
		SetReg(base, PBB_AB_MN_M_VID_L, 0xd1);
	}
	SetReg(base, PBB_AD_MN_N_VID_M, 0x80);
	SetReg(base, PBB_AE_MN_N_VID_L, 0x00);
	SetReg(base, PBB_A8_MN_VID_AUTO_EN_1, 0x40);
	SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~(_BIT7 | _BIT6), _BIT6 | _BIT7);
#endif

	DpTxSSTMSASetting(hwinfo);
	DpTxSSTDisplayFormatSetting(hwinfo);
	SetReg(base, PBB_A4_PG_FIFO_CTRL, 0xFF);
#if AUDIO_ON
	DpTxSSTAudioPlaySetting(hwinfo);
#else
	SetReg(base, PBB_C7_VBID, 0x4);
	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, 0);
#endif
}

void DpTxIRQHandle(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	unsigned int reg;

	reg = GetReg(base, PBD_B1_AUX_IRQ_EVENT);

	if((reg & _BIT5)==_BIT5) {
		hwinfo->aux_status = 1;
		up(&hwinfo->sem);
	} else {
		printk(KERN_ERR "not expect interrupt 0x%x\n", reg);
		hwinfo->aux_status = 0;
		DpTxAuxReset(hwinfo);
		up(&hwinfo->sem);
	}
	
	SetReg(base, PBD_B1_AUX_IRQ_EVENT, 0x3F);
	SetBit(base, PBD_AB_AUX_FIFO_CTRL, ~(_BIT1 | _BIT0), (_BIT1 | _BIT0));
}

void vo_common_setting(void __iomem *pll_base, void __iomem *vo_base)
{
	writel(0x10C167, pll_base + 0x260);
	writel(0x7, pll_base + 0x264);
	writel(0x3, pll_base + 0x264);
	writel(0x10, vo_base + 0x8);
	writel(0x3, vo_base);
	writel(0x5, vo_base + 0x4);
	writel(0x1401, vo_base + 0x28);
	writel(0x2000000, vo_base + 0x38);
	writel(0x3000000, vo_base + 0x3c);

#ifndef TESTING
	writel(0x8000800, vo_base + 0xdfc);
	writel(0x800, vo_base + 0xe00);
	writel(0xed83e80, vo_base + 0xe04);
	writel(0x3c410c99, vo_base + 0xe08);
	writel(0x0, vo_base + 0xe0C);
	writel(0x150cda, vo_base + 0xe10);
	writel(0xc4a, vo_base + 0xe14);
#else
	writel(0x8000000, vo_base + 0xdfc);
	writel(0x0, vo_base + 0xe00);
	writel(0x8000000, vo_base + 0xe04);
	writel(0x800, vo_base + 0xe08);
	writel(0x0, vo_base + 0xe0C);
	writel(0x0, vo_base + 0xe10);
	writel(0x0, vo_base + 0xe14);
	writel(0x3, vo_base);
#endif
}

void Set_1024_768_1lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#endif

	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);

	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 65);

	writel(0x0, lvds_base+0x100);
	writel(0x88, lvds_base+0x404);
	writel(0x5400540, lvds_base+0x408);
	writel(0x11f051f, lvds_base+0x40C);
	writel(0x240324, lvds_base+0x410);
	writel(0x326, lvds_base+0x418);
	writel(0x10007, lvds_base+0x41C);
	writel(0x00000325, lvds_base+0x42C);

#if RTK_VO_SET
	writel(0x400, vo_base + 0x48);
	writel(0x400, vo_base + 0x4c);
	writel(0x400300, vo_base + 0x68);
	writel(0x4000300, vo_base + 0xd78);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x14);
	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x5);
	writel(0x1, lvds_base + 0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif

}

void Set_720p_1lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 74);

	writel(0x0, lvds_base+0x100);
	writel(0x28, lvds_base+0x404);
	writel(0x6720672, lvds_base+0x408);
	writel(0x0fb05fb, lvds_base+0x40C);
	writel(0x1a02ea, lvds_base+0x410);
	writel(0x2ee, lvds_base+0x418);
	writel(0x10006, lvds_base+0x41C);

#if RTK_VO_SET
	writel(0x500, vo_base + 0x48);
	writel(0x500, vo_base + 0x4c);
	writel(0x5002d0, vo_base + 0x68);
	writel(0x50002d0, vo_base + 0xd78);
	writel(0x800002eb, lvds_base+0x42C);
#else
	writel(0x000002eb, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x14);
	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x5);
	writel(0x1, lvds_base + 0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_1080p_2lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 148);

	writel(0x0, lvds_base+0x100);
	writel(0x2C, lvds_base+0x404);
	writel(0x8980898, lvds_base+0x408);
	writel(0xb70837, lvds_base+0x40C);
	writel(0x2A0462, lvds_base+0x410);
	writel(0x465, lvds_base+0x418);
	writel(0x10006, lvds_base+0x41C);
	
#if RTK_VO_SET
	writel(0x780, vo_base + 0x48);
	writel(0x780, vo_base + 0x4c);
	writel(0x780438, vo_base + 0x68);
	writel(0x7800438, vo_base + 0xd78);
	writel(0x80000463, lvds_base+0x42C);
#else
	writel(0x00000463, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x38);

	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x6);
	writel(0x3, lvds_base+0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
//	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, 0);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

//	SetBit(base, PBB_A8_MN_VID_AUTO_EN_1, ~(_BIT7 | _BIT6), _BIT6);
#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_1440_768_2lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 72);

	writel(0x0, lvds_base+0x100);
	writel(0x20, lvds_base+0x404);
	writel(0x5f005f0, lvds_base+0x408);
	writel(0x03f05df, lvds_base+0x40C);
	writel(0xf030f, lvds_base+0x410);
	writel(0x316, lvds_base+0x418);
	writel(0x10009, lvds_base+0x41C);

#if RTK_VO_SET
	writel(0x5a0, vo_base + 0x48);
	writel(0x5a0, vo_base + 0x4c);
	writel(0x5a0300, vo_base + 0x68);
	writel(0x5a00300, vo_base + 0xd78);
	writel(0x80000310, lvds_base+0x42C);
#else
	writel(0x00000310, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x38);

	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x6);
	writel(0x3, lvds_base+0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_1440_900_2lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 85);

	writel(0x0, lvds_base+0x100);
	writel(0x20, lvds_base+0x404);
	writel(0x5f005f0, lvds_base+0x408);
	writel(0x03f05df, lvds_base+0x40C);
	writel(0xf0393, lvds_base+0x410);
	writel(0x39e, lvds_base+0x418);
	writel(0x10009, lvds_base+0x41C);

#if RTK_VO_SET
	writel(0x5a0, vo_base + 0x48);
	writel(0x5a0, vo_base + 0x4c);
	writel(0x5a0384, vo_base + 0x68);
	writel(0x5a00384, vo_base + 0xd78);
	writel(0x80000394, lvds_base+0x42C);
#else
	writel(0x00000394, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x38);

	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x6);
	writel(0x3, lvds_base+0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_1280_800_2lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 67);

	writel(0x0, lvds_base+0x100);
	writel(0x20, lvds_base+0x404);
	writel(0x5500550, lvds_base+0x408);
	writel(0x03f053f, lvds_base+0x40C);
	writel(0xf032f, lvds_base+0x410);
	writel(0x337, lvds_base+0x418);
	writel(0x10009, lvds_base+0x41C);

#if RTK_VO_SET
	writel(0x500, vo_base + 0x48);
	writel(0x500, vo_base + 0x4c);
	writel(0x500320, vo_base + 0x68);
	writel(0x5000320, vo_base + 0xd78);
	writel(0x80000330, lvds_base+0x42C);
#else
	writel(0x00000330, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x38);

	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x6);
	writel(0x3, lvds_base+0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_960_544_2lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *base = hwinfo->reg_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#else
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);
#endif
	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 35);

	writel(0x0, lvds_base+0x100);
	writel(0x20, lvds_base+0x404);
	writel(0x4100410, lvds_base+0x408);
	writel(0x03f03ff, lvds_base+0x40C);
	writel(0xf022f, lvds_base+0x410);
	writel(0x230, lvds_base+0x418);
	writel(0x10009, lvds_base+0x41C);

#if RTK_VO_SET
	writel(0x3c0, vo_base + 0x48);
	writel(0x3c0, vo_base + 0x4c);
	writel(0x3c0220, vo_base + 0x68);
	writel(0x3c00220, vo_base + 0xd78);
	writel(0x80000330, lvds_base+0x42C);
#else
	writel(0x00000330, lvds_base+0x42C);
#endif

	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x38);

	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x6);
	writel(0x3, lvds_base+0x40);
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void Set_2160p30_4lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *vo_base = hwinfo->vo_base;
	
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);

	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 297);

	writel(0x0, lvds_base+0x100);
	writel(0x58, lvds_base+0x404);
	writel(0x11301130, lvds_base+0x408);
	writel(0x1771077, lvds_base+0x40C);
	writel(0x5308c3, lvds_base+0x410);
	writel(0x8ca, lvds_base+0x418);
	writel(0x1000b, lvds_base+0x41C);
	writel(0x000008c4, lvds_base+0x42C);
	
	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0xFC);
	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x7);
	writel(0xf, lvds_base + 0x40);
	
	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
//	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, 0);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);

	writel(0x3, vo_base + 0xe78);
}

void Set_2160p60_4lane(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	void __iomem *lvds_base = hwinfo->lvds_base;
	void __iomem *vo_base = hwinfo->vo_base;

#if RTK_VO_SET
	void __iomem *pll_base = hwinfo->pll_base;
	vo_common_setting(pll_base, vo_base);
#endif
	SetReg(base, PBB_C8_VBID_FW_CTL, 0x1);

	writel(0x1, lvds_base+0x100);
	writel(0x2000, lvds_base+0x108);
	writel(0x0, lvds_base+0x10c);
	writel(0x2000, lvds_base+0x110);
	writel(0x20000000, lvds_base+0x114);
	writel(0x0, lvds_base+0x118);
	writel(0x0, lvds_base+0x11c);

	SET_DP_TX_INPUT_PIXEL_CLK(hwinfo, 522);

	writel(0x20, lvds_base+0x404);
	writel(0xf500f50, lvds_base+0x408);
	writel(0x3f0f3f, lvds_base+0x40c);
	writel(0xf087f, lvds_base+0x410);
	writel(0x8ae, lvds_base+0x418);
	writel(0x10009, lvds_base+0x41C);
	writel(0x00000880, lvds_base+0x42C);

#if RTK_VO_SET
	writel(0xf00, vo_base + 0x48);
	writel(0xf00, vo_base + 0x4c);
	writel(0xf00870, vo_base + 0x68);
	writel(0xf000870, vo_base + 0xd78);
	writel(0xf00, vo_base + 0x48);
	writel(0xf00, vo_base + 0x4c);
#endif
	DpTxSSTSetting(hwinfo);

	SetReg(base, PBB_00_DP_PHY_CTRL, 0xFC);
	SetReg(base, PBB_A0_DP_MAC_CTRL, 0x7);
	writel(0xf, lvds_base + 0x40);

	SetReg(base, PBC_A7_DPTX_SFIFO_CTRL0, 0x80);
//	SetReg(base, PBB_D4_ARBITER_SEC_END_CNT_HB, 0);
	SetReg(base, PBB_0D_DPTX_PHY_CTRL, 0x15);
#if RTK_VO_SET
	writel(0x47, vo_base + 0xec4);
	writel(0x3, vo_base + 0x20);
	writel(0x3, vo_base + 0xe78);
#else
	writel(0x3, vo_base + 0xe78);
#endif
}

void dptx_lvdsint_en(struct rtk_dptx_hwinfo *hwinfo, int en)
{
	void __iomem *lvds_base = hwinfo->lvds_base;
	unsigned int reg;

	reg = readl(lvds_base + 0x42C);
	if(en)
		reg = reg | 0x80000000;
	else
		reg = reg & ~0x80000000;
	writel(reg, lvds_base + 0x42C);

	reg = readl(lvds_base + 0x42C);
}

void dptx_close_phy(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *base = hwinfo->reg_base;
	void __iomem *lvds_base = hwinfo->lvds_base;

	SetReg(base, PBB_00_DP_PHY_CTRL, 0x1);
	writel(0x0, lvds_base + 0x40);
}

void dptx_close_pll(struct rtk_dptx_hwinfo *hwinfo)
{
	void __iomem *pll_base = hwinfo->pll_base;
	unsigned int val;

	msleep(1);
	writel(0x0, pll_base + 0x24c);
	msleep(1);
	val = readl(pll_base + 0x248);
	val = val & ~(1<<31);
	writel(val, pll_base + 0x248);
	msleep(1);

	writel(0x8, pll_base + 0x600); //4k
	msleep(1);
	val = readl(pll_base + 0x254);
	val = val & ~0x1;
	writel(val, pll_base + 0x254);
	msleep(1);

	val = readl(pll_base + 0x250);
	val = val & ~(1<<31);
	writel(val, pll_base + 0x250);
	msleep(1);
}

int dptx_config_tv_system(struct rtk_dptx_hwinfo *hwinfo)
{
	int mode;
	int ret=0;
	int i;

	mode = hwinfo->out_type;

	switch(mode) {
	case DP_FORMAT_1080P_60:
		Set_1080p_2lane(hwinfo);
		break;
	case DP_FORMAT_2160P_30:
		Set_2160p30_4lane(hwinfo);
		break;
	case DP_FORMAT_2160P_60:
		Set_2160p60_4lane(hwinfo);
		break;
	case DP_FORMAT_1024_768:
		Set_1024_768_1lane(hwinfo);
		break;
	case DP_FORMAT_1440_768:
		Set_1440_768_2lane(hwinfo);
		break;
	case DP_FORMAT_1440_900:
		Set_1440_900_2lane(hwinfo);
		break;
	case DP_FORMAT_1280_800:
		Set_1280_800_2lane(hwinfo);
		break;
	case DP_FORMAT_960_544:
		Set_960_544_2lane(hwinfo);
		break;
	case DP_FORMAT_720P_60:
	default:
		Set_720p_1lane(hwinfo);
		break;
	}

	for(i=0; i<LINK_TRAIN_RETRY; i++) {
		ret = DpTxLinkTraining(hwinfo);
		if(ret < 0)
			printk(KERN_ERR "link training fail %d\n", i);
		else
			break;
		msleep(100);
	}
	if(ret < 0)
		return -1;
	return ret;
}
