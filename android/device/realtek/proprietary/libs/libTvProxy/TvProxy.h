#ifndef _TV_PROXY_H_
#define _TV_PROXY_H_

#include <utils/StrongPointer.h>
#include <binder/IBinder.h>

class DvbChannelInfo;

using namespace android;

class TvProxy
{
public:
	TvProxy();
	~TvProxy();

	bool ConnectToServer();
	void SetBroadcastIntentAction(const char* action);
	int Init(int tunerId, int appType);
	int UnInit(int tunerId, int appType);
	bool Renew();
	void SetTvRegion(int tvRegion);
	bool TvAutoScanStart(int tunerId);
	void TvAutoScanStop(int tunerId);
	bool TvManualScanStart(int tunerId, int frequency, int bandwidth);
	void TvManualScanStop(int tunerId);
	int TvScanInfo(int tunerId, int infoId);
	bool PlayChannel(int tunerId, int chIdx, bool bShowVideo, bool bAudioFocus);
	bool PlayNextChannel(int tunerId, bool bShowVideo, bool bAudioFocus, int filterOutFlags);
	bool PlayPrevChannel(int tunerId, bool bShowVideo, bool bAudioFocus, int filterOutFlags);
	int GetChannelCount(int filterOutFlags);
	int GetCurChannel(DvbChannelInfo *pInfo, int tunerId);
	int GetNextChannel(DvbChannelInfo *pInfo, int tunerId, int filterOutFlags);
	int GetPrevChannel(DvbChannelInfo *pInfo, int tunerId, int filterOutFlags);
	int GetFirstChannel(DvbChannelInfo *pInfo, int filterOutFlags);
	int GetFirstChannelInFreq(DvbChannelInfo *pInfo, int frequency, int filterOutFlags);
	int GetChannelByChannelNumber(DvbChannelInfo *pInfo, int logicalChNum);
	int GetNextChannelByChNumber(DvbChannelInfo *pInfo, int logicalChNum, int filterOutFlags);
	int GetPrevChannelByChNumber(DvbChannelInfo *pInfo, int logicalChNum, int filterOutFlags);
	int GetChannelList(int filterOutFlags);
	void FreeChannelList();
	int GetChannelByChannelIndex(DvbChannelInfo *pInfo, int chIdx);
	int GetTunerState(int tunerId);
	int GetAvailableTunerId();
	void StartListen(int tunerId);
	void StopListen(int tunerId);

private:
	sp<IBinder> mTvBinder;

	void TransferReplyToDvdChannelInfo(Parcel *pReply, DvbChannelInfo *pInfo);

};

#endif	// #define _TV_PROXY_H_