//-----------------------------------------------------------------------------
// TextSubtitleDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __TEXTSUBTITLEDECODER_H__
#define __TEXTSUBTITLEDECODER_H__

class NotificationEventManager;

//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder Inteface
//

//--------------------------------------------------------------------------
// default implementation choices (mutually exclusive) for the TextSTDecoder interface
//
#define TEXTSTDECODER_IFACE_IMPL_INTERFACE		= 0;
#define TEXTSTDECODER_IFACE_IMPL_ASSERT			{ assert(false); }
#define TEXTSTDECODER_IFACE_IMPL_DEFAULT		{ return 0; }
#define TEXTSTDECODER_IFACE_IMPL				TEXTSTDECODER_IFACE_IMPL_INTERFACE

//--------------------------------------------------------------------------
// TextSubtitleDecoder
/// @brief	Text Subtitle Decoder interface
///
class TextSubtitleDecoder
{
public:
	TextSubtitleDecoder()												{};
	virtual ~TextSubtitleDecoder()										{};

	// setup event notification channel
	virtual void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager)		TEXTSTDECODER_IFACE_IMPL

	// media control interface
	virtual void Play(void)												TEXTSTDECODER_IFACE_IMPL
	virtual void Pause(void)											TEXTSTDECODER_IFACE_IMPL
	virtual void Stop(void)												TEXTSTDECODER_IFACE_IMPL

	// Interactive User Operations
	virtual SonicError SelectUserStyleNumber(UInt32 styleNum)			TEXTSTDECODER_IFACE_IMPL
	virtual SonicError GetNumberOfUserStyles(UInt32 & numStyles)		TEXTSTDECODER_IFACE_IMPL
	virtual SonicError EnableDisplay(bool bOn)							TEXTSTDECODER_IFACE_IMPL

	// pass primary video size to text decoder
	virtual void SetVideoSize(UInt32 width, UInt32 height)				TEXTSTDECODER_IFACE_IMPL

private:
	TextSubtitleDecoder(const TextSubtitleDecoder&); // no copying
	const TextSubtitleDecoder& operator=(const TextSubtitleDecoder&); // no copying
};

#endif // __TEXTSUBTITLEDECODER_H__
