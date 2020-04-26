/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 *
 *
 * Author: 	Ryan Chou
 */

package com.realtek.app;

import com.realtek.app.tv.*;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;


public class TvManager
{  
    // TV_SCAN_INFO maps to it in DtvServiceDefs.h
    public final static int SCAN_INFO_STATE = 0;
    public final static int SCAN_INFO_PROGRESS = 1;
    public final static int SCAN_INFO_CHANNEL_COUNT = 2;
    public final static int SCAN_INFO_CHANNEL_STRENGTH = 3;
    public final static int SCAN_INFO_CHANNEL_QUALITY = 4;
    public final static int SCAN_INFO_CHANNEL_FREQUENCY = 5;
    public final static int SCAN_INFO_CHANNEL_BANDWIDTH = 6;
    public final static int SCAN_INFO_CURRENT_CHANNEL_NUMBER = 7;

	// Values to match the enum or define in DtvService on AndroidDvdPlayer -- start --
	public final static int DTV_TUNER_STATUS_INVALID = 0;
	public final static int DTV_TUNER_STATUS_IDLE = 1;
	public final static int DTV_TUNER_STATUS_PREVIEW = 2;
	public final static int DTV_TUNER_STATUS_RECORD = 3;
	public final static int DTV_TUNER_STATUS_TIMESHIFT = 6;
	
	// DTV_APP maps to it in DtvServiceDefs.h
	public final static int NO_APP = -1;
	public final static int APP_DTV = 0x0001;
	public final static int APP_TOGO = 0x0010;
	
	// CHANNEL_FLAGS maps to it in DtvServiceDefs.h
	public final static int CHANNEL_FLAGS_IGNORE = 0x00000000;
	public final static int CHANNEL_FLAGS_IS_DELETED = 0x00000001;
	public final static int CHANNEL_FLAGS_IS_LOCKED = 0x00000002;
	public final static int CHANNEL_FLAGS_IS_FAVORITE = 0x00000004;
	public final static int CHANNEL_FLAGS_IS_HIDDEN = 0x00000008;
	public final static int CHANNEL_FLAGS_DISAPPEARED = 0x00000200;
	public final static int CHANNEL_FLAG_IS_DTV = 0x00100000;
	public final static int CHANNEL_FLAG_IS_RADIO = 0x00200000;

	// ENUM_TV_REGION maps to it in DtvServiceDefs.h
	public final static int TV_REGION_NONE = 0;
	public final static int TV_REGION_ARGENTINA = 1;
	public final static int TV_REGION_AUSTRALIA = 2;
	public final static int TV_REGION_AUSTRIA = 3;
	public final static int TV_REGION_BELGIUM = 4;
	public final static int TV_REGION_BRAZIL = 5;
	public final static int TV_REGION_CHINA = 6;
	public final static int TV_REGION_CZECH = 7;
	public final static int TV_REGION_DENMARK = 8;
	public final static int TV_REGION_FINLAND = 9;
	public final static int TV_REGION_FRANCE = 10;
	public final static int TV_REGION_GERMANY = 11;
	public final static int TV_REGION_GREECE = 12;
	public final static int TV_REGION_HUNGARY = 13;
	public final static int TV_REGION_IRELAND = 14;
	public final static int TV_REGION_ITALY = 15;
	public final static int TV_REGION_JAPAN = 16;
	public final static int TV_REGION_KOREA = 17;
	public final static int TV_REGION_LUXEMBOURG = 18;
	public final static int TV_REGION_NETHERLANDS = 19;
	public final static int TV_REGION_NORWAY = 20;
	public final static int TV_REGION_POLAND = 21;
	public final static int TV_REGION_PORTUGAL = 22;
	public final static int TV_REGION_RUSSIA = 23;
	public final static int TV_REGION_SLOVAKIA = 24;
	public final static int TV_REGION_SLOVENIA = 25;
	public final static int TV_REGION_SPAIN_PENINSULA_Y_BALEARES = 26;
	public final static int TV_REGION_SPAIN_CANARIAS = 27;
	public final static int TV_REGION_SWEDEN = 28;
	public final static int TV_REGION_SWITZERLAND = 29;
	public final static int TV_REGION_TAIWAN = 30;
	public final static int TV_REGION_TURKEY = 31;
	public final static int TV_REGION_UNITED_KINGDOM = 32;
	public final static int TV_REGION_USA = 33;

	// Broadcast event, type/value, map to DTV events	
	public final static String DTV_EVENT_TYPE = "DTV_EVENT_TYPE";
	public final static String TYPE_SIGNAL_STATUS = "TYPE_SIGNAL_STATUS";
	public final static String TYPE_SI_MESSAGE = "TYPE_SI_MESSAGE";
	public final static String TYPE_RECORD_EVENT = "TYPE_RECORD_EVENT";
	
	public final static String DTV_EVENT_VALUE = "DTV_EVENT_VALUE";
	public final static int VALUE_NO_SIGNAL = 0;
	public final static int VALUE_ONLY_RF = 1;
	public final static int VALUE_HAS_SIGNAL_NORMAL = 2;
	public final static int VALUE_HAS_SIGNAL_SCRAMBLED = 3;
	
	// Values to match the enum or define in DtvService on AndroidDvdPlayer -- end --

    private final ITvManager mService;


    /**
     * package private on purpose
     */     
	public TvManager() {
		mService = ITvManager.Stub.asInterface(ServiceManager.getService("tv"));
		if(mService != null) {
			Log.i("TvManager", mService.toString());
		}
		else {
			Log.e("TvManager", "error! CANNOT get TvManagerService!");
		}
	}	     

	// APIs to DtvService on AndroidDvdPlayer -- start --
	public boolean ConnectToServer()
	{
		Log.d("TvManager", "ConnectToServer");
		try {
			return mService.ConnectToServer();
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public void SetBroadcastIntentAction(String action)
	{
		Log.d("TvManager", "SetBroadcastIntentAction");
		try {
			mService.SetBroadcastIntentAction(action);
		}
		catch (RemoteException ex) {
		}
	}
	
	public int Init(int tunerId, int appType)
	{
		Log.d("TvManager", "Init:"+tunerId+".appType="+appType);
		try {
			return mService.Init(tunerId, appType);
		}
		catch (RemoteException ex) {
			return -1;
		}
	}

	public int UnInit(int tunerId, int appType)
	{
		Log.d("TvManager", "UnInit:"+tunerId+".appType="+appType);
		try {
			return mService.UnInit(tunerId, appType);
		}
		catch (RemoteException ex) {
			return -1;
		}
	}

	public boolean Renew()
	{
		Log.d("TvManager", "Renew");
		try {
			return mService.Renew();
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public void SetTvRegion(int tvRegion)
	{
		Log.d("TvManager", "SetTvRegion:"+tvRegion);
		try {
			mService.SetTvRegion(tvRegion);
		}
		catch (RemoteException ex) {
		}		
	}

	public boolean TvAutoScanStart(int tunerId)
	{
		Log.d("TvManager", "TvAutoScanStart:"+tunerId);
		try {
			return mService.TvAutoScanStart(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public void TvAutoScanStop(int tunerId)
	{
		Log.d("TvManager", "TvAutoScanStop:"+tunerId);
		try {
			mService.TvAutoScanStop(tunerId);
		}
		catch (RemoteException ex) {
		}		
	}

	public boolean TvManualScanStart(int tunerId, int frequency, int bandwidth)
	{
		Log.d("TvManager", "TvManualScanStart:"+tunerId+","+frequency+","+bandwidth);
		try {
			return mService.TvManualScanStart(tunerId, frequency, bandwidth);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public void TvManualScanStop(int tunerId)
	{
		Log.d("TvManager", "TvManualScanStop:"+tunerId);
		try {
			mService.TvManualScanStop(tunerId);
		}
		catch (RemoteException ex) {
		}		
	}

	public int TvScanInfo(int tunerId, int infoId)
	{
		Log.d("TvManager", "TvScanInfo:"+tunerId+","+infoId);
		int ret = 0;
		try {
			ret = mService.TvScanInfo(tunerId, infoId);
		}
		catch (RemoteException ex) {
		}
		return ret;
	}

	public boolean PlayChannel(int tunerId, int chIdx, boolean bShowVideo, boolean bAudioFocus)
	{
		Log.d("TvManager", "PlayChannel:"+tunerId+","+chIdx+","+bShowVideo+","+bAudioFocus);
		try {
			return mService.PlayChannel(tunerId, chIdx, bShowVideo, bAudioFocus);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public boolean PlayNextChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags)
	{
		Log.d("TvManager", "PlayNextChannel:"+tunerId+","+bShowVideo+","+bAudioFocus+","+filterOutFlags);
		try {
			return mService.PlayNextChannel(tunerId, bShowVideo, bAudioFocus, filterOutFlags);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public boolean PlayPrevChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags)
	{
		Log.d("TvManager", "PlayPrevChannel:"+tunerId+","+bShowVideo+","+bAudioFocus+","+filterOutFlags);
		try {
			return mService.PlayPrevChannel(tunerId, bShowVideo, bAudioFocus, filterOutFlags);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public int GetChannelCount(int filterOutFlags)
	{
		Log.d("TvManager", "GetChannelCount:"+filterOutFlags);
		try {
			return mService.GetChannelCount(filterOutFlags);
		}
		catch (RemoteException ex) {
			return 0;
		}
	}

	public DvbChannelInfo GetCurChannel(int tunerId)
	{
		Log.d("TvManager", "GetCurChannel:"+tunerId);
		try {
			return mService.GetCurChannel(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetNextChannel(int tunerId, int filterOutFlags)
	{
		Log.d("TvManager", "GetNextChannel:"+tunerId+","+filterOutFlags);
		try {
			return mService.GetNextChannel(tunerId, filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetPrevChannel(int tunerId, int filterOutFlags)
	{
		Log.d("TvManager", "GetPrevChannel:"+tunerId+","+filterOutFlags);
		try {
			return mService.GetPrevChannel(tunerId, filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetFirstChannel(int filterOutFlags)
	{
		Log.d("TvManager", "GetFirstChannel:"+filterOutFlags);
		try {
			return mService.GetFirstChannel(filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetFirstChannelInFreq(int frequency, int filterOutFlags)
	{
		Log.d("TvManager", "GetFirstChannelInFreq:"+frequency+","+filterOutFlags);
		try {
			return mService.GetFirstChannelInFreq(frequency, filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetChannelByChannelNumber(int logicalChNum)
	{
		Log.d("TvManager", "GetChannelByChannelNumber:"+logicalChNum);
		try {
			return mService.GetChannelByChannelNumber(logicalChNum);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetNextChannelByChNumber(int logicalChNum, int filterOutFlags)
	{
		Log.d("TvManager", "GetNextChannelByChNumber:"+logicalChNum+","+filterOutFlags);
		try {
			return mService.GetNextChannelByChNumber(logicalChNum, filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public DvbChannelInfo GetPrevChannelByChNumber(int logicalChNum, int filterOutFlags)
	{
		Log.d("TvManager", "GetPrevChannelByChNumber:"+logicalChNum+","+filterOutFlags);
		try {
			return mService.GetPrevChannelByChNumber(logicalChNum, filterOutFlags);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public int GetChannelList(int filterOutFlags)
	{
		Log.d("TvManager", "GetChannelList:"+filterOutFlags);
		try {
			return mService.GetChannelList(filterOutFlags);
		}
		catch (RemoteException ex) {
			return 0;
		}
	}

	public void FreeChannelList()
	{
		Log.d("TvManager", "FreeChannelList");
		try {
			mService.FreeChannelList();
		}
		catch (RemoteException ex) {
		}		
	}

	public DvbChannelInfo GetChannelByChannelIndex(int chIdx)
	{
		Log.d("TvManager", "GetChannelByChannelIndex:"+chIdx);
		try {
			return mService.GetChannelByChannelIndex(chIdx);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	// the meaning of the return value refers to RECORD_ERR_CODE
	public int StartRecordTs(int tunerId, String filePath, boolean bWithPreview)
	{
		Log.d("TvManager", "StartRecordTs:"+tunerId+","+filePath+","+bWithPreview);
		try {
			return mService.StartRecordTs(tunerId, filePath, bWithPreview);	// return RECORD_ERR_CODE
		}
		catch (RemoteException ex) {
			return (-1);
		}
	}

	public boolean StopRecordTs(int tunerId)
	{
		Log.d("TvManager", "StopRecordTs:"+tunerId);
		try {
			return mService.StopRecordTs(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean PauseRecordTs(int tunerId)
	{
		Log.d("TvManager", "PauseRecordTs:"+tunerId);
		try {
			return mService.PauseRecordTs(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean ResumeRecordTs(int tunerId)
	{
		Log.d("TvManager", "ResumeRecordTs:"+tunerId);
		try {
			return mService.ResumeRecordTs(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean EnterBackgroundRecording(int tunerId)
	{
		Log.d("TvManager", "EnterBackgroundRecording:"+tunerId);
		try {
			return mService.EnterBackgroundRecording(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean LeaveBackgroundRecording(int tunerId)
	{
		Log.d("TvManager", "LeaveBackgroundRecording:"+tunerId);
		try {
			return mService.LeaveBackgroundRecording(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}
/*
	public DvbSubInfo GetDvbSubList(int tunerId)
	{
		Log.d("TvManager", "GetDvbSubList:"+tunerId);
		try {
			return mService.GetDvbSubList(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}		
	}
*/
	public DvbSubInfo[] GetDvbSubList(int tunerId)
	{
		Log.d("TvManager", "GetDvbSubList:"+tunerId);
		try {
			return mService.GetDvbSubList(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}		
	}

	public TtxSubInfo[] GetTtxSubList(int tunerId)
	{
		Log.d("TvManager", "GetTtxSubList:"+tunerId);
		try {
			return mService.GetTtxSubList(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}		
	}

	public TtxSubInfo[] GetTeletextList(int tunerId)
	{
		Log.d("TvManager", "GetTeletextList:"+tunerId);
		try {
			return mService.GetTeletextList(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}		
	}

	public boolean EnableDvbSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow)
	{
		Log.d("TvManager", "EnableDvbSubtitle:"+tunerId+","+pid+","+langCode+","+index+","+bShow);
		try {
			return mService.EnableDvbSubtitle(tunerId, pid, langCode, index, bShow);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean DisableDvbSubtitle(int tunerId)
	{
		Log.d("TvManager", "DisableDvbSubtitle:"+tunerId);
		try {
			return mService.DisableDvbSubtitle(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetDvbSubtitleVisible(int tunerId, boolean isVisible)
	{
		Log.d("TvManager", "SetDvbSubtitleVisible:"+tunerId+","+isVisible);
		try {
			return mService.SetDvbSubtitleVisible(tunerId, isVisible);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsDvbSubEnable(int tunerId)
	{
		Log.d("TvManager", "IsDvbSubEnable:"+tunerId);
		try {
			return mService.IsDvbSubEnable(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsDvbSubVisible(int tunerId)
	{
		Log.d("TvManager", "IsDvbSubVisible:"+tunerId);
		try {
			return mService.IsDvbSubVisible(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean EnableTtSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow)
	{
		Log.d("TvManager", "EnableTtSubtitle:"+tunerId+","+pid+","+langCode+","+index+","+bShow);
		try {
			return mService.EnableTtSubtitle(tunerId, pid, langCode, index, bShow);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean DisableTtSubtitle(int tunerId)
	{
		Log.d("TvManager", "DisableTtSubtitle:"+tunerId);
		try {
			return mService.DisableTtSubtitle(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetTtSubtitleVisible(int tunerId, boolean isVisible)
	{
		Log.d("TvManager", "SetTtSubtitleVisible:"+tunerId+","+isVisible);
		try {
			return mService.SetTtSubtitleVisible(tunerId, isVisible);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsTtSubEnable(int tunerId)
	{
		Log.d("TvManager", "IsTtSubEnable:"+tunerId);
		try {
			return mService.IsTtSubEnable(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsTtSubVisible(int tunerId)
	{
		Log.d("TvManager", "IsTtSubVisible:"+tunerId);
		try {
			return mService.IsTtSubVisible(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean EnableTeletext(int tunerId, int pid, int langCode, int index, int magazineNum, int pageNum)
	{
		Log.d("TvManager", "EnableTeletext:"+tunerId+","+pid+","+langCode+","+index+","+magazineNum+","+pageNum);
		try {
			return mService.EnableTeletext(tunerId, pid, langCode, index, magazineNum, pageNum);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean DisableTeletext(int tunerId)
	{
		Log.d("TvManager", "DisableTeletext:"+tunerId);
		try {
			return mService.DisableTeletext(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetTeletextUserLabel(int tunerId, int userLabel)
	{
		Log.d("TvManager", "SetTeletextUserLabel:"+tunerId+","+userLabel);
		try {
			return mService.SetTeletextUserLabel(tunerId, userLabel);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	// the meaning of action refers to TELETEXT_USER_ACTION
	public boolean SetTeletextUserAction(int tunerId, int action)
	{
		Log.d("TvManager", "SetTeletextUserAction:"+tunerId+","+action);
		try {
			return mService.SetTeletextUserAction(tunerId, action);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsTeletextEnable(int tunerId)
	{
		Log.d("TvManager", "IsTeletextEnable:"+tunerId);
		try {
			return mService.IsTeletextEnable(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public AudioInfo[] GetAudioList(int tunerId)
	{
		Log.d("TvManager", "GetAudioList:"+tunerId);
		try {
			return mService.GetAudioList(tunerId);
		}
		catch (RemoteException ex) {
			return null;
		}		
	}

	public boolean PlayAudioStreamByPID(int tunerId, int pid)
	{
		Log.d("TvManager", "PlayAudioStreamByPID:"+tunerId+","+pid);
		try {
			return mService.PlayAudioStreamByPID(tunerId, pid);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean IsBilingual(int tunerId)
	{
		Log.d("TvManager", "IsBilingual:"+tunerId);
		try {
			return mService.IsBilingual(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetBilingual(int tunerId, int lingual)
	{
		Log.d("TvManager", "SetBilingual:"+tunerId+","+lingual);
		try {
			return mService.SetBilingual(tunerId, lingual);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetChannelLocked(int lcn, int locked)
	{
		Log.d("TvManager", "SetChannelLocked:"+lcn+","+locked);
		try {
			return mService.SetChannelLocked(lcn, locked);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetChannelFavorited(int lcn, int favorited)
	{
		Log.d("TvManager", "SetChannelFavorited:"+lcn+","+favorited);
		try {
			return mService.SetChannelFavorited(lcn, favorited);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetChannelSkiped(int lcn, int skiped)
	{
		Log.d("TvManager", "SetChannelSkiped:"+lcn+","+skiped);
		try {
			return mService.SetChannelSkiped(lcn, skiped);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetChannelDeleted(int lcn, int deleted)
	{
		Log.d("TvManager", "SetChannelDeleted:"+lcn+","+deleted);
		try {
			return mService.SetChannelDeleted(lcn, deleted);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SetChannelName(int lcn, int nameStrLen, String nameStr)
	{
		Log.d("TvManager", "SetChannelName:"+lcn+","+nameStrLen+","+nameStr);
		try {
			return mService.SetChannelName(lcn, nameStrLen, nameStr);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean SwapChannel(int srcLcn, int dstLcn)
	{
		Log.d("TvManager", "SwapChannel:"+srcLcn+","+dstLcn);
		try {
			return mService.SwapChannel(srcLcn, dstLcn);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean GetEpgData(int tunerId, int lcn, int iDayOffset, int iDayCount)
	{
		Log.d("TvManager", "GetEpgData:"+tunerId+","+lcn+","+iDayOffset+","+iDayCount);
		try {
			return mService.GetEpgData(tunerId, lcn, iDayOffset, iDayCount);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public boolean DestroyEpgData(int tunerId)
	{
		Log.d("TvManager", "DestroyEpgData:"+tunerId);
		try {
			return mService.DestroyEpgData(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}		
	}

	public int GetEpgListEpgCount(int tunerId)
	{
		Log.d("TvManager", "GetEpgListEpgCount:"+tunerId);
		try {
			return mService.GetEpgListEpgCount(tunerId);
		}
		catch (RemoteException ex) {
			return 0;
		}
	}

	public EpgProgram GetEpgDataList(int tunerId, int indexOfList)
	{
		Log.d("TvManager", "GetEpgDataList:"+tunerId+","+indexOfList);
		try {
			return mService.GetEpgDataList(tunerId, indexOfList);
		}
		catch (RemoteException ex) {
			return null;
		}
	}

	public int GetTunerState(int tunerId)
	{
		Log.d("TvManager", "GetTunerState:"+tunerId);
		try {
			return mService.GetTunerState(tunerId);
		}
		catch (RemoteException ex) {
			return 0;
		}
	}

	public int GetAvailableTunerId()
	{
		Log.d("TvManager", "GetAvailableTunerId");
		try {
			return mService.GetAvailableTunerId();
		}
		catch (RemoteException ex) {
			return -1;
		}
	}

	public boolean StartTimeshift(int tunerId, String livepauseDir, int bufSizeInMb)
	{
		Log.d("TvManager", "StartTimeshift:"+tunerId+","+livepauseDir+","+bufSizeInMb);
		try {
			return mService.StartTimeshift(tunerId, livepauseDir, bufSizeInMb);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public boolean StopTimeshift(int tunerId, String livepauseDir)
	{
		Log.d("TvManager", "StopTimeshift:"+tunerId+","+livepauseDir);
		try {
			return mService.StopTimeshift(tunerId, livepauseDir);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public void StartListen(int tunerId)
	{
		Log.d("TvManager", "StartListen:"+tunerId);
		try {
			mService.StartListen(tunerId);
		}
		catch (RemoteException ex) {
		}		
	}

	public void StopListen(int tunerId)
	{
		Log.d("TvManager", "StopListen:"+tunerId);
		try {
			mService.StopListen(tunerId);
		}
		catch (RemoteException ex) {
		}		
	}

	public boolean CreateDecodeFlow(int tunerId)
	{
		Log.d("TvManager", "CreateDecodeFlow:"+tunerId);
		try {
			return mService.CreateDecodeFlow(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public boolean DestroyDecodeFlow(int tunerId)
	{
		Log.d("TvManager", "DestroyDecodeFlow:"+tunerId);
		try {
			return mService.DestroyDecodeFlow(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}
	}

	public boolean IsDecoding(int tunerId)
	{
		Log.d("TvManager", "IsDecoding:"+tunerId);
		try {
			return mService.IsDecoding(tunerId);
		}
		catch (RemoteException ex) {
			return false;
		}
	}
	// APIs to DtvService on AndroidDvdPlayer -- end --
}
