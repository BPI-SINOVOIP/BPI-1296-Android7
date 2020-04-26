/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 *
 *
 * Author: 	Ryan Chou
 */

package com.realtek.app;

import com.realtek.app.tv.DvbChannelInfo;
import com.realtek.app.tv.DvbSubInfo;
import com.realtek.app.tv.TtxSubInfo;
import com.realtek.app.tv.AudioInfo;
import com.realtek.app.tv.EpgProgram;

/**
 * System private API for talking with the tv manager service.
 *
 * {@hide}
 */
interface ITvManager {
	// APIs to DtvService on AndroidDvdPlayer -- start --
	boolean ConnectToServer();																			// 0
	void SetBroadcastIntentAction(String action);														// 1
	int Init(int tunerId, int appType);																	// 2
	int UnInit(int tunerId, int appType);																// 3
	boolean Renew();																					// 4
	void SetTvRegion(int tvRegion);																		// 5
	boolean TvAutoScanStart(int tunerId);																// 6
	void TvAutoScanStop(int tunerId);																	// 7
	boolean TvManualScanStart(int tunerId, int frequency, int bandwidth);								// 8
	void TvManualScanStop(int tunerId);																	// 9
	int TvScanInfo(int tunerId, int infoId);															// 10
	boolean PlayChannel(int tunerId, int chIdx, boolean bShowVideo, boolean bAudioFocus);				// 11
	boolean PlayNextChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags);	// 12
	boolean PlayPrevChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags);	// 13
	int GetChannelCount(int filterOutFlags);															// 14
	DvbChannelInfo GetCurChannel(int tunerId);															// 15
	DvbChannelInfo GetNextChannel(int tunerId, int filterOutFlags);										// 16
	DvbChannelInfo GetPrevChannel(int tunerId, int filterOutFlags);										// 17
	DvbChannelInfo GetFirstChannel(int filterOutFlags);													// 18
	DvbChannelInfo GetFirstChannelInFreq(int frequency, int filterOutFlags);							// 19
	DvbChannelInfo GetChannelByChannelNumber(int logicalChNum);											// 20
	DvbChannelInfo GetNextChannelByChNumber(int logicalChNum, int filterOutFlags);						// 21
	DvbChannelInfo GetPrevChannelByChNumber(int logicalChNum, int filterOutFlags);						// 22
	int GetChannelList(int filterOutFlags);																// 23
	void FreeChannelList();																				// 24
	DvbChannelInfo GetChannelByChannelIndex(int chIdx);													// 25
	int StartRecordTs(int tunerId, String filePath, boolean bWithPreview);								// 26
	boolean StopRecordTs(int tunerId);																	// 27
	boolean PauseRecordTs(int tunerId);																	// 28							
	boolean ResumeRecordTs(int tunerId);																// 29
	boolean EnterBackgroundRecording(int tunerId);														// 30
	boolean LeaveBackgroundRecording(int tunerId);														// 31
	DvbSubInfo[] GetDvbSubList(int tunerId);															// 32
	TtxSubInfo[] GetTtxSubList(int tunerId);															// 33
	TtxSubInfo[] GetTeletextList(int tunerId);															// 34
	boolean EnableDvbSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow);			// 35
	boolean DisableDvbSubtitle(int tunerId);															// 36
	boolean SetDvbSubtitleVisible(int tunerId, boolean isVisible);										// 37
	boolean IsDvbSubEnable(int tunerId);																// 38
	boolean IsDvbSubVisible(int tunerId);																// 39
	boolean EnableTtSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow);				// 40
	boolean DisableTtSubtitle(int tunerId);																// 41
	boolean SetTtSubtitleVisible(int tunerId, boolean isVisible);										// 42
	boolean IsTtSubEnable(int tunerId);																	// 43
	boolean IsTtSubVisible(int tunerId);																// 44
	boolean EnableTeletext(int tunerId, int pid, int langCode, int index, int magazineNum, int pageNum);// 45
	boolean DisableTeletext(int tunerId);																// 46
	boolean SetTeletextUserLabel(int tunerId, int userLabel);											// 47
	boolean SetTeletextUserAction(int tunerId, int action);												// 48
	boolean IsTeletextEnable(int tunerId);																// 49
	AudioInfo[] GetAudioList(int tunerId);																// 50
	boolean PlayAudioStreamByPID(int tunerId, int pid);													// 51
	boolean IsBilingual(int tunerId);																	// 52
	boolean SetBilingual(int tunerId, int lingual);														// 53
	boolean SetChannelLocked(int lcn, int locked);														// 54
	boolean SetChannelFavorited(int lcn, int favorited);												// 55
	boolean SetChannelSkiped(int lcn, int skiped);														// 56
	boolean SetChannelDeleted(int lcn, int deleted);													// 57
	boolean SetChannelName(int lcn, int nameStrLen, String nameStr);									// 58
	boolean SwapChannel(int srcLcn, int dstLcn);														// 59
	boolean GetEpgData(int tunerId, int lcn, int iDayOffset, int iDayCount);							// 60
	boolean DestroyEpgData(int tunerId);																// 61
	int GetEpgListEpgCount(int tunerId);																// 62
	EpgProgram GetEpgDataList(int tunerId, int indexOfList);											// 63
	int GetTunerState(int tunerId);																		// 64
	int GetAvailableTunerId();																			// 65
	boolean StartTimeshift(int tunerId, String livepauseDir, int bufSizeInMb);							// 66
	boolean StopTimeshift(int tunerId, String livepauseDir);											// 67
	void StartListen(int tunerId);																		// 68
	void StopListen(int tunerId);																		// 69
	boolean CreateDecodeFlow(int tunerId);																// 70
	boolean DestroyDecodeFlow(int tunerId);																// 71
	boolean IsDecoding(int tunerId);																	// 72
	// APIs to DtvService on AndroidDvdPlayer -- end --
}


