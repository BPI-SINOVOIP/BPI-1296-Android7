/*
 *  dptx_hwapi.h - RTK display port driver header file
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DPTX_HWAPI_H__
#define __DPTX_HWAPI_H__

#include "dptx_reg.h"

typedef enum
{
	DP_FORMAT_720P_60 = 0,
	DP_FORMAT_1024_768,
	DP_FORMAT_1080P_60,
	DP_FORMAT_2160P_30,
	DP_FORMAT_2160P_60,
	DP_FORMAT_1440_768,
	DP_FORMAT_1280_800,
	DP_FORMAT_1440_900,
	DP_FORMAT_960_544,
} EnumDPTxOuputType;

typedef enum
{
	_DP_ONE_LANE = 1,
	_DP_TWO_LANE = 2,
	_DP_FOUR_LANE = 4,
} EnumDPTxLaneNumber;

typedef enum
{
    _SEC_PACKET_TYPE_AUDIO_TIMESTAMP = 0x01,
    _SEC_PACKET_TYPE_AUDIO_STREAM = 0x02,
    _SEC_PACKET_TYPE_VSC = 0x07,
    _SEC_PACKET_TYPE_INFOFRAME_AVI = 0x0A,
    _SEC_PACKET_TYPE_INFOFRAME_AUDIO = 0x0C,
    _SEC_PACKET_TYPE_INFOFRAME_MPEG = 0x0D,
    _SEC_PACKET_TYPE_INFOFRAME_RSV = 0x0F,
} EnumDPTxSecDataPacketType;

typedef enum
{
	_DP_TX_LEVEL_0 = 0x00,
	_DP_TX_LEVEL_1,
	_DP_TX_LEVEL_2,
	_DP_TX_LEVEL_3,
} EnumDpTxSignalLevel;

typedef enum
{
	_DP_TX_LANE_0 = 0x00,
	_DP_TX_LANE_1,
	_DP_TX_LANE_2,
	_DP_TX_LANE_3,
} EnumDpTxLane;

typedef enum
{
	_DP_TP_NONE = 0x00,
	_DP_TP_1,
	_DP_TP_2,
	_DP_TP_3,
} EnumDpTxTrainPattern;

typedef enum
{
	_DP_TX_TRAINING_PATTERN1 = 0x00,
	_DP_TX_TRAINING_PATTERN2,
	_DP_TX_TRAINING_PATTERN_END ,
} EnumDPTxLTStatus;

typedef enum
{
	_DP_TX_TRAINING_NO_RESULT = 0x00,
	_DP_TX_TRAINING_PATTERN1_FAIL,
	_DP_TX_TRAINING_PATTERN2_FAIL,
	_DP_TX_TRAINING_PATTERN1_ADJUST_FAIL,
	_DP_TX_TRAINING_PATTERN2_ADJUST_FAIL,
	_DP_TX_TRAINING_PATTERN1_PASS,
	_DP_TX_TRAINING_PATTERN2_PASS,
	_DP_TX_TRAINING_PASS,
	_DP_TX_TRAINING_FAIL,
} EnumDPTxLTResult;

typedef enum
{
	_DP_LANE0_MAX_POST_CURSOR2_REACHED = _BIT2,
	_DP_LANE1_MAX_POST_CURSOR2_REACHED = _BIT6,
	_DP_LANE2_MAX_POST_CURSOR2_REACHED = _BIT2,
	_DP_LANE3_MAX_POST_CURSOR2_REACHED = _BIT6,
	_DP_SCRAMBLING_DISABLE = _BIT5,
} EnumDpTxLinkTrainAdditionalInfo;

typedef struct
{
    unsigned char ucDPCDRev;
    unsigned char b3PeerDeviceType : 3;
    unsigned char ucLinkRate;
    unsigned char b3LaneNum : 3;
    unsigned char b1MSGCapStatus : 1;
    unsigned char b1DPPlugStatus : 1;
    unsigned char b1DPPlugChange : 1;
    unsigned char b1LegacyPlugStatus : 1;
    unsigned char b1EnhanceFraming : 1;
    unsigned char b1AlternateSrCapable : 1;
    unsigned char b1FramingChangeCapable : 1;
    unsigned char b1SSCSupport : 1;
    unsigned char b1TP3Support : 1;
    unsigned char b1NumberOfSDP : 1;
    unsigned char b1NumberOfSDPSink : 1;
    unsigned char ucMaxLinkRate;
    unsigned char b1DownStreamInfoReady : 1;
    unsigned char b1UpRequestCap : 1;
} StructDownStreamInfo;

typedef struct
{
    unsigned char b3ColorDepth : 3;
    unsigned char b3ColorSpace : 3;
    unsigned char b1AudioReadyToPlay : 1;
    unsigned char ucCurrnetAudioFrequency;
    unsigned char b3AudioChannelCount : 3;
    unsigned short usInputPixelClk;
    unsigned char b1InterlaceMode : 1;
    unsigned char b1InterlaceVTotalNumber : 1;
    unsigned short usHorizontalDataEnablePeriod;
} StructDPTxInputInfo;

typedef struct
{
	unsigned char b2LinkTrainingState : 2;
	unsigned char b4LinkTrainingResult : 4;
	unsigned char b3TP1Lane0SameVolCnt : 3;
	unsigned char b3TP1Lane1SameVolCnt : 3;
	unsigned char b3TP1Lane2SameVolCnt : 3;
	unsigned char b3TP1Lane3SameVolCnt : 3;
	unsigned char b3TP1count : 3;
	unsigned char b3TP2count : 3;
	unsigned char ucLinkPBNPerTimeSlot;
	unsigned char ucLane0Adjust;
	unsigned char ucLane1Adjust;
	unsigned char ucLane2Adjust;
	unsigned char ucLane3Adjust;
	unsigned char ucLane01LTStatus;
	unsigned char ucLane23LTStatus;
	unsigned char ucLaneAlignStatus;
	unsigned char ucAdjustReqLane01;
	unsigned char ucAdjustReqLane23;
} StructDPTxLTInfo;

struct rtk_dptx_hwinfo {
	void __iomem *reg_base;
	void __iomem *pll_base;
	void __iomem *lvds_base;
	void __iomem *vo_base;

	int out_type;
	int aux_status;
	struct semaphore sem;
	StructDownStreamInfo DownStreamInfo;
	StructDPTxInputInfo TxInputInfo;
	StructDPTxLTInfo TxLTInfo;
};

int link_training(struct rtk_dptx_hwinfo *hwinfo);
int DpTxLinkTraining(struct rtk_dptx_hwinfo *hwinfo);

void DpTxPixelPLLSetting(struct rtk_dptx_hwinfo *hwinfo);
void DpTxDpPLLSetting(struct rtk_dptx_hwinfo *hwinfo);
void DpTxInitial(struct rtk_dptx_hwinfo *hwinfo);

void DpTxIRQHandle(struct rtk_dptx_hwinfo *hwinfo);
void Set_1080p_2lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_2160p30_4lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_2160p60_4lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_720p_1lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_1280_800_2lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_1440_768_2lane(struct rtk_dptx_hwinfo *hwinfo);
void Set_1440_900_2lane(struct rtk_dptx_hwinfo *hwinfo);
int dptx_config_tv_system(struct rtk_dptx_hwinfo *hwinfo);

void dptx_lvdsint_en(struct rtk_dptx_hwinfo *hwinfo, int en);
void dptx_close_phy(struct rtk_dptx_hwinfo *hwinfo);
void dptx_close_pll(struct rtk_dptx_hwinfo *hwinfo);
int ReadEDID(struct rtk_dptx_hwinfo *hwinfo, unsigned char *ptr, int lentgh);
#endif  //__DPTX_HWAPI_H__
