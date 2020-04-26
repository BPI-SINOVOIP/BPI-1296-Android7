package com.realtek.server;

import android.util.Log;
import com.realtek.app.TvManager;
import com.realtek.app.ITvManager;
import com.realtek.app.tv.*;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class TvManagerService extends ITvManager.Stub {
	static {
                System.loadLibrary("realtek_runtime");
	}

    private static final String TAG = "TvManagerService";
    private final Context mContext;
	private String strBroadcastIntentAction = null;

    public TvManagerService(Context context) {
        mContext = context;
        Log.v(TAG, "TvManagerService is constructed!");
    }

    protected void finalize() throws Throwable {
        try {
        } finally {
            close();
            super.finalize();
        }
    }

	// Callbacks for DtvService on AndroidDvdPlayer -- start --
	private void SignalStautsCallback(int tunerId, int status) {
		Log.v(TAG, "SignalStautsCallback:"+tunerId+","+status);
		if (strBroadcastIntentAction != null) {
			Intent intent = new Intent(strBroadcastIntentAction);
			intent.putExtra(TvManager.DTV_EVENT_TYPE, TvManager.TYPE_SIGNAL_STATUS);
			intent.putExtra(TvManager.DTV_EVENT_VALUE, status);
			mContext.sendBroadcast(intent);
		}
	}

	private void SiMessageCallback(int tunerId, int message, int data) {
		Log.v(TAG, "SiMessageCallback:"+tunerId+","+message+","+data);
	}

	private void RecordEventCallback(int tunerId, int event) {
		Log.v(TAG, "RecordEventCallback:"+tunerId+","+event);
	}

	// Callbacks for DtvService on AndroidDvdPlayer -- end --

	// APIs to DtvService on AndroidDvdPlayer -- start --
	public boolean ConnectToServer()	// 0
	{
		return connectToServer();
	}

	public void SetBroadcastIntentAction(String action)	// 1
	{
		strBroadcastIntentAction = action;
	}
	
	public int Init(int tunerId, int appType)	// 2
	{
		boolean result;		
		Log.v(TAG, "Init:"+tunerId+".appType="+appType);
		pushInteger(tunerId);
		pushInteger(appType);
		result = invoke("Init");
		if (result == true) {
			return getInteger();
		}
		return -1;
	}

	public int UnInit(int tunerId, int appType)	// 3
	{
		boolean result;		
		Log.v(TAG, "UnInit:"+tunerId+".appType="+appType);
		pushInteger(tunerId);
		pushInteger(appType);
		result = invoke("UnInit");
		if (result == true) {
			return getInteger();
		}
		return -1;
	}

	public boolean Renew()	// 4
	{
		boolean result;		
		Log.v(TAG, "Renew");
		result = invoke("Renew");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public void SetTvRegion(int tvRegion)	// 5
	{
		Log.v(TAG, "SetTvRegion:"+tvRegion);
		pushInteger(tvRegion);
		invoke("SetTvRegion");
	}

	public boolean TvAutoScanStart(int tunerId)	// 6
	{
		boolean result;		
		Log.v(TAG, "TvAutoScanStart:"+tunerId);
		pushInteger(tunerId);
		result = invoke("TvAutoScanStart");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public void TvAutoScanStop(int tunerId)	// 7
	{
		Log.v(TAG, "TvAutoScanStop:"+tunerId);
		pushInteger(tunerId);
		invoke("TvAutoScanStop");
	}

	public boolean TvManualScanStart(int tunerId, int frequency, int bandwidth)	// 8
	{
		boolean result;		
		Log.v(TAG, "TvManualScanStart:"+tunerId+","+frequency+","+bandwidth);
		pushInteger(tunerId);
		pushInteger(frequency);
		pushInteger(bandwidth);
		result = invoke("TvManualScanStart");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public void TvManualScanStop(int tunerId)	// 9
	{
		boolean result;		
		Log.v(TAG, "TvManualScanStop:"+tunerId);
		pushInteger(tunerId);
		invoke("TvManualScanStop");
	}

	public int TvScanInfo(int tunerId, int infoId)	// 10
	{
		boolean result;		
		int ret = 0;
		Log.v(TAG, "TvScanInfo:"+tunerId+","+infoId);
		pushInteger(tunerId);
		pushInteger(infoId);
		result = invoke("TvScanInfo");
		if (result == true) {
			ret = getInteger();
			Log.v(TAG, "TvScanInfo.getInteger="+ret);
		}
		return ret;
	}

	public boolean PlayChannel(int tunerId, int chIdx, boolean bShowVideo, boolean bAudioFocus)	// 11
	{
		boolean result;
		Log.v(TAG, "PlayChannel:"+tunerId+","+chIdx+","+bShowVideo+","+bAudioFocus);
		pushInteger(tunerId);
		pushInteger(chIdx);
		pushBoolean(bShowVideo);
		pushBoolean(bAudioFocus);
		result = invoke("PlayChannel");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public boolean PlayNextChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags)	// 12
	{
		boolean result;
		Log.v(TAG, "PlayNextChannel:"+tunerId+","+bShowVideo+","+bAudioFocus+","+filterOutFlags);
		pushInteger(tunerId);		
		pushBoolean(bShowVideo);
		pushBoolean(bAudioFocus);
		pushInteger(filterOutFlags);
		result = invoke("PlayNextChannel");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public boolean PlayPrevChannel(int tunerId, boolean bShowVideo, boolean bAudioFocus, int filterOutFlags)	// 13
	{
		boolean result;
		Log.v(TAG, "PlayPrevChannel:"+tunerId+","+bShowVideo+","+bAudioFocus+","+filterOutFlags);
		pushInteger(tunerId);		
		pushBoolean(bShowVideo);
		pushBoolean(bAudioFocus);
		pushInteger(filterOutFlags);
		result = invoke("PlayPrevChannel");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public int GetChannelCount(int filterOutFlags)	// 14
	{
		boolean result;
		Log.v(TAG, "GetChannelCount:"+filterOutFlags);
		pushInteger(filterOutFlags);
		result = invoke("GetChannelCount");
		if (result == true) {
			return getInteger();
		}
		return 0;
	}

	public DvbChannelInfo GetCurChannel(int tunerId)	// 15
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetCurChannel:"+tunerId);
		pushInteger(tunerId);
		result = invoke("GetCurChannel");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetNextChannel(int tunerId, int filterOutFlags)	// 16
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetNextChannel:"+tunerId+","+filterOutFlags);
		pushInteger(tunerId);
		pushInteger(filterOutFlags);
		result = invoke("GetNextChannel");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetPrevChannel(int tunerId, int filterOutFlags)	// 17
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetPrevChannel:"+tunerId+","+filterOutFlags);
		pushInteger(tunerId);
		pushInteger(filterOutFlags);
		result = invoke("GetPrevChannel");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetFirstChannel(int filterOutFlags)	// 18
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetFirstChannel:"+filterOutFlags);
		pushInteger(filterOutFlags);
		result = invoke("GetFirstChannel");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetFirstChannelInFreq(int frequency, int filterOutFlags)	// 19
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetFirstChannelInFreq:"+frequency+","+filterOutFlags);
		pushInteger(frequency);
		pushInteger(filterOutFlags);
		result = invoke("GetFirstChannelInFreq");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetChannelByChannelNumber(int logicalChNum)	// 20
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetChannelByChannelNumber:"+logicalChNum);
		pushInteger(logicalChNum);
		result = invoke("GetChannelByChannelNumber");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetNextChannelByChNumber(int logicalChNum, int filterOutFlags)	// 21
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetNextChannelByChNumber:"+logicalChNum+","+filterOutFlags);
		pushInteger(logicalChNum);
		pushInteger(filterOutFlags);
		result = invoke("GetNextChannelByChNumber");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	public DvbChannelInfo GetPrevChannelByChNumber(int logicalChNum, int filterOutFlags)	// 22
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetPrevChannelByChNumber:"+logicalChNum+","+filterOutFlags);
		pushInteger(logicalChNum);
		pushInteger(filterOutFlags);
		result = invoke("GetPrevChannelByChNumber");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	// the meaning of filterOutFlags refers to CHANNEL_FLAGS
	public int GetChannelList(int filterOutFlags)	// 23
	{
		boolean result;
		Log.v(TAG, "GetChannelList:"+filterOutFlags);
		pushInteger(filterOutFlags);
		result = invoke("GetChannelList");
		if (result == true) {
			return getInteger();
		}
		return 0;
	}

	public void FreeChannelList()	// 24
	{
		boolean result;		
		Log.v(TAG, "FreeChannelList");
		invoke("FreeChannelList");
	}

	public DvbChannelInfo GetChannelByChannelIndex(int chIdx)	// 25
	{
		DvbChannelInfo chInfo = null;
		boolean result;
		Log.v(TAG, "GetChannelByChannelIndex:"+chIdx);
		pushInteger(chIdx);
		result = invoke("GetChannelByChannelIndex");
		if (result == true) {
			chInfo = getChannelInfo();
			return chInfo;
		}
		return chInfo;
	}

	// the meaning of the return value refers to RECORD_ERR_CODE
	public int StartRecordTs(int tunerId, String filePath, boolean bWithPreview)	// 26
	{
		boolean result;
		Log.v(TAG, "StartRecordTs:"+tunerId+","+filePath+","+bWithPreview);
		pushInteger(tunerId);
		pushString(filePath);
		pushBoolean(bWithPreview);
		result = invoke("StartRecordTs");
		if (result == true) {
			return getInteger();	// return RECORD_ERR_CODE
		}
		return (-1);
	}

	public boolean StopRecordTs(int tunerId)	// 27
	{
		boolean result;
		Log.v(TAG, "StopRecordTs:"+tunerId);
		pushInteger(tunerId);
		result = invoke("StopRecordTs");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean PauseRecordTs(int tunerId)	// 28
	{
		boolean result;
		Log.v(TAG, "PauseRecordTs:"+tunerId);
		pushInteger(tunerId);
		result = invoke("PauseRecordTs");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean ResumeRecordTs(int tunerId)	// 29
	{
		boolean result;
		Log.v(TAG, "ResumeRecordTs:"+tunerId);
		pushInteger(tunerId);
		result = invoke("ResumeRecordTs");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean EnterBackgroundRecording(int tunerId)	// 30
	{
		boolean result;
		Log.v(TAG, "EnterBackgroundRecording:"+tunerId);
		pushInteger(tunerId);
		result = invoke("EnterBackgroundRecording");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean LeaveBackgroundRecording(int tunerId)	// 31
	{
		boolean result;
		Log.v(TAG, "LeaveBackgroundRecording:"+tunerId);
		pushInteger(tunerId);
		result = invoke("LeaveBackgroundRecording");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public DvbSubInfo[] GetDvbSubList(int tunerId) {	// 32
		DvbSubInfo[] list;
		Log.v(TAG, "GetDvbSubList:"+tunerId);
		pushInteger(tunerId);
		if (invoke("GetDvbSubList") == true) {
			list = getDvbSubInfo();
			if ((list != null) && (list.length > 0)) {
				for (int i=0;i<list.length;i++) {
					Log.v(TAG, i+".pid="+list[i].pid+".langCode="+list[i].langCode+".shortLangStr="+list[i].shortLangStr+".isHardOfHearing="+list[i].isHardOfHearing);
				}
				return list;
			}
		}
		return null;
	}

	public TtxSubInfo[] GetTtxSubList(int tunerId) {	// 33
		TtxSubInfo[] list;
		Log.v(TAG, "GetTtxSubList:"+tunerId);
		pushInteger(tunerId);
		if (invoke("GetTtxSubList") == true) {
			list = getTtxSubInfo();
			if ((list != null) && (list.length > 0)) {
				for (int i=0;i<list.length;i++) {
					Log.v(TAG, i+".pid="+list[i].pid+".langCode="+list[i].langCode+".shortLangStr="+list[i].shortLangStr+".magazineNum="+list[i].magazineNum+".pageNum="+list[i].pageNum);
				}
				return list;
			}
		}
		return null;
	}

	public TtxSubInfo[] GetTeletextList(int tunerId) {	// 34
		TtxSubInfo[] list;
		Log.v(TAG, "GetTeletextList:"+tunerId);
		pushInteger(tunerId);
		if (invoke("GetTeletextList") == true) {
			list = getTeletextInfo();
			if ((list != null) && (list.length > 0)) {
				for (int i=0;i<list.length;i++) {
					Log.v(TAG, i+".pid="+list[i].pid+".langCode="+list[i].langCode+".shortLangStr="+list[i].shortLangStr+".magazineNum="+list[i].magazineNum+".pageNum="+list[i].pageNum);
				}
				return list;
			}
		}
		return null;
	}

	public boolean EnableDvbSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow)	// 35
	{
		boolean result;
		Log.v(TAG, "EnableDvbSubtitle:"+tunerId+","+pid+","+langCode+","+index+","+bShow);
		pushInteger(tunerId);
		pushInteger(pid);
		pushInteger(langCode);
		pushInteger(index);
		pushBoolean(bShow);
		result = invoke("EnableDvbSubtitle");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean DisableDvbSubtitle(int tunerId)	// 36
	{
		boolean result;
		Log.v(TAG, "DisableDvbSubtitle:"+tunerId);
		pushInteger(tunerId);
		result = invoke("DisableDvbSubtitle");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetDvbSubtitleVisible(int tunerId, boolean isVisible)	// 37
	{
		boolean result;
		Log.v(TAG, "SetDvbSubtitleVisible:"+tunerId+","+isVisible);
		pushInteger(tunerId);
		pushBoolean(isVisible);
		result = invoke("SetDvbSubtitleVisible");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsDvbSubEnable(int tunerId)	// 38
	{
		boolean result;
		Log.v(TAG, "IsDvbSubEnable:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsDvbSubEnable");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsDvbSubVisible(int tunerId)	// 39
	{
		boolean result;
		Log.v(TAG, "IsDvbSubVisible:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsDvbSubVisible");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean EnableTtSubtitle(int tunerId, int pid, int langCode, int index, boolean bShow)	// 40
	{
		boolean result;
		Log.v(TAG, "EnableTtSubtitle:"+tunerId+","+pid+","+langCode+","+index+","+bShow);
		pushInteger(tunerId);
		pushInteger(pid);
		pushInteger(langCode);
		pushInteger(index);
		pushBoolean(bShow);
		result = invoke("EnableTtSubtitle");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean DisableTtSubtitle(int tunerId)	// 41
	{
		boolean result;
		Log.v(TAG, "DisableTtSubtitle:"+tunerId);
		pushInteger(tunerId);
		result = invoke("DisableTtSubtitle");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetTtSubtitleVisible(int tunerId, boolean isVisible)	// 42
	{
		boolean result;
		Log.v(TAG, "SetTtSubtitleVisible:"+tunerId+","+isVisible);
		pushInteger(tunerId);
		pushBoolean(isVisible);
		result = invoke("SetTtSubtitleVisible");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsTtSubEnable(int tunerId)	// 43
	{
		boolean result;
		Log.v(TAG, "IsTtSubEnable:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsTtSubEnable");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsTtSubVisible(int tunerId)	// 44
	{
		boolean result;
		Log.v(TAG, "IsTtSubVisible:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsTtSubVisible");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean EnableTeletext(int tunerId, int pid, int langCode, int index, int magazineNum, int pageNum)	// 45
	{
		boolean result;
		Log.v(TAG, "EnableTeletext:"+tunerId+","+pid+","+langCode+","+index+","+magazineNum+","+pageNum);
		pushInteger(tunerId);
		pushInteger(pid);
		pushInteger(langCode);
		pushInteger(index);
		pushInteger(magazineNum);
		pushInteger(pageNum);
		result = invoke("EnableTeletext");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean DisableTeletext(int tunerId)	// 46
	{
		boolean result;
		Log.v(TAG, "DisableTeletext:"+tunerId);
		pushInteger(tunerId);
		result = invoke("DisableTeletext");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetTeletextUserLabel(int tunerId, int userLabel)	// 47
	{
		boolean result;
		Log.v(TAG, "SetTeletextUserLabel:"+tunerId+","+userLabel);
		pushInteger(tunerId);
		pushInteger(userLabel);
		result = invoke("SetTeletextUserLabel");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	// the meaning of action refers to TELETEXT_USER_ACTION
	public boolean SetTeletextUserAction(int tunerId, int action)	// 48
	{
		boolean result;
		Log.v(TAG, "SetTeletextUserAction:"+tunerId+","+action);
		pushInteger(tunerId);
		pushInteger(action);
		result = invoke("SetTeletextUserAction");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsTeletextEnable(int tunerId)	// 49
	{
		boolean result;
		Log.v(TAG, "IsTeletextEnable:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsTeletextEnable");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public AudioInfo[] GetAudioList(int tunerId) {	// 50
		AudioInfo[] list;
		Log.v(TAG, "GetAudioList:"+tunerId);
		pushInteger(tunerId);
		if (invoke("GetAudioList") == true) {
			list = getAudioInfo();
			if ((list != null) && (list.length > 0)) {
				for (int i=0;i<list.length;i++) {
					Log.v(TAG, i+".pid="+list[i].pid+".streamType="+list[i].streamType+".type="+list[i].type+".langCount="+list[i].langCount);
					Log.v(TAG, i+".langCode[0]="+list[i].langCode[0]+".langCode[1]="+list[i].langCode[1]+".shortLangStr[0]="+list[i].shortLangStr[0]+".shortLangStr[1]="+list[i].shortLangStr[1]); 			
				}
				return list;
			}
		}
		return null;
	}

	public boolean PlayAudioStreamByPID(int tunerId, int pid)	// 51
	{
		boolean result;
		Log.v(TAG, "PlayAudioStreamByPID:"+tunerId+","+pid);
		pushInteger(tunerId);
		pushInteger(pid);
		result = invoke("PlayAudioStreamByPID");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsBilingual(int tunerId)	// 52
	{
		boolean result;
		Log.v(TAG, "IsBilingual:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsBilingual");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetBilingual(int tunerId, int lingual)	// 53
	{
		boolean result;
		Log.v(TAG, "SetBilingual:"+tunerId+","+lingual);
		pushInteger(tunerId);
		pushInteger(lingual);
		result = invoke("SetBilingual");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetChannelLocked(int lcn, int locked)	// 54
	{
		boolean result;
		Log.v(TAG, "SetChannelLocked:"+lcn+","+locked);
		pushInteger(lcn);
		pushInteger(locked);
		result = invoke("SetChannelLocked");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetChannelFavorited(int lcn, int favorited)	// 55
	{
		boolean result;
		Log.v(TAG, "SetChannelFavorited:"+lcn+","+favorited);
		pushInteger(lcn);
		pushInteger(favorited);
		result = invoke("SetChannelFavorited");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetChannelSkiped(int lcn, int skiped)	// 56
	{
		boolean result;
		Log.v(TAG, "SetChannelSkiped:"+lcn+","+skiped);
		pushInteger(lcn);
		pushInteger(skiped);
		result = invoke("SetChannelSkiped");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetChannelDeleted(int lcn, int deleted)	// 57
	{
		boolean result;
		Log.v(TAG, "SetChannelDeleted:"+lcn+","+deleted);
		pushInteger(lcn);
		pushInteger(deleted);
		result = invoke("SetChannelDeleted");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SetChannelName(int lcn, int nameStrLen, String nameStr)	// 58
	{
		boolean result;
		Log.v(TAG, "SetChannelName:"+lcn+","+nameStrLen+","+nameStr);
		pushInteger(lcn);
		pushInteger(nameStrLen);
		pushString(nameStr);
		result = invoke("SetChannelName");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean SwapChannel(int srcLcn, int dstLcn)	// 59
	{
		boolean result;
		Log.v(TAG, "SwapChannel:"+srcLcn+","+dstLcn);
		pushInteger(srcLcn);
		pushInteger(dstLcn);
		result = invoke("SetChannelDeleted");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean GetEpgData(int tunerId, int lcn, int iDayOffset, int iDayCount)	// 60
	{
		boolean result;
		Log.v(TAG, "GetEpgData:"+tunerId+","+lcn+","+iDayOffset+","+iDayCount);
		pushInteger(tunerId);
		pushInteger(lcn);
		pushInteger(iDayOffset);
		pushInteger(iDayCount);
		result = invoke("GetEpgData");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean DestroyEpgData(int tunerId)	// 61
	{
		boolean result;
		Log.v(TAG, "DestroyEpgData:"+tunerId);
		pushInteger(tunerId);
		result = invoke("DestroyEpgData");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public int GetEpgListEpgCount(int tunerId)	// 62
	{
		boolean result;
		Log.v(TAG, "GetEpgListEpgCount:"+tunerId);
		pushInteger(tunerId);
		result = invoke("GetEpgListEpgCount");
		if (result == true) {
			return getInteger();
		}
		return 0;
	}

	public EpgProgram GetEpgDataList(int tunerId, int indexOfList)	// 63
	{
		EpgProgram epgProgram = null;
		boolean result;
		Log.v(TAG, "GetEpgDataList:"+tunerId+","+indexOfList);
		pushInteger(tunerId);
		pushInteger(indexOfList);
		result = invoke("GetEpgDataList");
		if (result == true) {
			epgProgram = getEpgProgram();

			if (null != epgProgram) {
				Log.v(TAG, "EpgProgram:eventID="+epgProgram.eventID+".beginDateTime="+epgProgram.beginDateTime+".endDateTime="+epgProgram.endDateTime+".iso639LanguageCode="+epgProgram.iso639LanguageCode);
				Log.v(TAG, "EpgProgram:titleLen="+epgProgram.titleLen+".title="+epgProgram.title+".detailDescrLen="+epgProgram.detailDescrLen+".detailDescr="+epgProgram.detailDescr);
				Log.v(TAG, "EpgProgram:rating="+epgProgram.rating+".audioNum="+epgProgram.audioNum+".hasSubtitle="+epgProgram.hasSubtitle+".hasTeletext"+epgProgram.hasTeletext+",hasTTXSubtitle="+epgProgram.hasTTXSubtitle+",isScrambled="+epgProgram.isScrambled);
			}
			return epgProgram;
		}
		return epgProgram;
	}

	public int GetTunerState(int tunerId)	// 64
	{
		boolean result;
		Log.v(TAG, "GetTunerState:"+tunerId);
		pushInteger(tunerId);
		result = invoke("GetTunerState");
		if (result == true) {
			return getInteger();
		}
		return 0;
	}

	public int GetAvailableTunerId()	// 65
	{
		boolean result;
		Log.v(TAG, "GetAvailableTunerId");
		result = invoke("GetAvailableTunerId");
		if (result == true) {
			return getInteger();
		}
		return -1;
	}

	public boolean StartTimeshift(int tunerId, String livepauseDir, int bufSizeInMb)	// 66
	{
		boolean result;
		Log.v(TAG, "StartTimeshift:"+tunerId+","+livepauseDir+","+bufSizeInMb);
		pushInteger(tunerId);
		pushString(livepauseDir);
		pushInteger(bufSizeInMb);
		result = invoke("StartTimeshift");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean StopTimeshift(int tunerId, String livepauseDir)	// 67
	{
		boolean result;
		Log.v(TAG, "StopTimeshift:"+tunerId+","+livepauseDir);
		pushInteger(tunerId);
		pushString(livepauseDir);
		result = invoke("StopTimeshift");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public void StartListen(int tunerId)	// 68
	{
		Log.v(TAG, "StartListen:"+tunerId);
		pushInteger(tunerId);
		invoke("StartListen");
	}

	public void StopListen(int tunerId)	// 69
	{
		Log.v(TAG, "StopListen:"+tunerId);
		pushInteger(tunerId);
		invoke("StopListen");
	}

	public boolean CreateDecodeFlow(int tunerId)	// 70
	{
		boolean result;		
		Log.v(TAG, "CreateDecodeFlow:"+tunerId);
		pushInteger(tunerId);
		result = invoke("CreateDecodeFlow");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean DestroyDecodeFlow(int tunerId)	// 71
	{
		boolean result;		
		Log.v(TAG, "DestroyDecodeFlow:"+tunerId);
		pushInteger(tunerId);
		result = invoke("DestroyDecodeFlow");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}

	public boolean IsDecoding(int tunerId)	// 72
	{
		boolean result;		
		Log.v(TAG, "IsDecoding:"+tunerId);
		pushInteger(tunerId);
		result = invoke("IsDecoding");
		if (result == true) {
			return getBoolean();
		}
		return false;
	}
	// APIs to DtvService on AndroidDvdPlayer -- end --

	/**
	 * Below declaration are implemented by c native code 
	 * that implemented by device/realtek/frameworks/services/jni/com_realtek_server_TvManagerService.cpp
	 */
    private native int init();
	private native boolean connectToServer();
    private native void close();
    private native String getServerName();
    private native boolean invoke(String funcName);

    private native void pushBoolean(boolean value);
    private native void pushInteger(int value);
    private native void pushLong(long value);
    private native void pushDouble(double value);
    private native void pushFloat(float value);
    private native void pushChar(char value);
    private native void pushShort(short value);
	private native void pushString(String value);

    private native boolean getBoolean();
    private native int getInteger();
    private native long getLong();
    private native double getDouble();
    private native float getFloat();
    private native char getChar();
    private native short getShort();
	private native String getString();
	private native DvbChannelInfo getChannelInfo();
	//private native DvbSubInfo getDvbSubInfo();
	private native DvbSubInfo[] getDvbSubInfo();
	private native TtxSubInfo[] getTtxSubInfo();
	private native TtxSubInfo[] getTeletextInfo();
	private native AudioInfo[] getAudioInfo();
	private native EpgProgram getEpgProgram();
}
