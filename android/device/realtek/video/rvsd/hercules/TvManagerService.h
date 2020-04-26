#ifndef _TV_SERVER_H_
#define _TV_SERVER_H_
namespace TvServer {
bool Connect();
int GetAvailableTunerId();
#ifdef ANDROID
int Init(int tunerId, int appType);
int UnInit(int tunerId, int appType);
#else
bool Init(int tunerId);
bool UnInit(int tunerId);
bool TvAutoScanStart(int tunerId);
bool TvAutoScanStop(int tunerId);
bool PlayNextChannel(int tunerId, bool bShowVideo, bool bAudioFocus, int filterOutFlags);
int GetChannelCount2(int tunerId,int filterOutFlags);
bool GetFirstChannel2(int tunerId, int filterOutFlags,void *pInfo);
#endif
bool SetTvRegion(int tvRegion);
bool TvManualScanStart(int tunerId, int frequency, int bandwidth);
int TvScanInfo(int tunerId, int infoId);
bool TvManualScanStop(int tunerId);
int GetChannelCount(int filterOutFlags);
bool GetFirstChannel(int filterOutFlags,void *pInfo);
bool PlayChannel(int tunerId, int chIdx, bool bShowVideo, bool bAudioFocus);
void Disconnect();
bool SetDescrambler(int tunerId, int ialgo, int rounds,int level);
bool SetSystemKey(int tunerId, char Key[64]);
bool SetEvenCW(int tunerId, char Key[64],char IV[32]);
bool SetOddCW(int tunerId, char Key[64],char IV[32]);
}
#endif