//-----------------------------------------------------------------------------
// BDROM_PlaybackData.h
// Copyright (c) 2000 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_PlaybackData_H__
#define __BDROM_PlaybackData_H__

class BDROM_PlaybackInfo
{

public:

	typedef enum tagMessageCode
	{
		Unused = -1,
		Unspecified = 0,
		WaitingForDebugger = 1,
		LoadingXlet = 2,
		NoXletsToLoad = 3,
		InitXlet = 4,
		StartXlet = 5,
		PauseXlet = 6,
		DestroyXlet = 7,
		SystemOutPrintln = 8,
		Exception = 9,
		LoggerMessage = 10
	} MessageCode;

	struct BDROM_PlaybackData
	{
		void Clear()
		{
			mCode = Unused;
		}

		MessageCode mCode;
		unicode::string8	mMessage;
	};

	class PlaybackInfoCallback	// Derive from this class and override PlaybackInfoAvailable() to get callbacks when playback information is available
	{
	public:
		PlaybackInfoCallback() {}
		virtual ~PlaybackInfoCallback() {}	

		virtual void PlaybackInfoAvailable(BDROM_PlaybackData& playbackData) = 0;
	};

	// Setup optional callback to occur when playback information is to be delivered
	static void SetPlaybackInfoCallback(PlaybackInfoCallback* callback) { m_pPlaybackInfoCallback = callback; }

	static void PlaybackInfoAvailable(MessageCode code);

	static void PlaybackInfoAvailable(MessageCode code, unicode::string8 message);

private:
	static PlaybackInfoCallback*		m_pPlaybackInfoCallback;				///< Optional callback to send information when playback information is to be delivered
};

#endif //__BDROM_PlaybackData_H__
