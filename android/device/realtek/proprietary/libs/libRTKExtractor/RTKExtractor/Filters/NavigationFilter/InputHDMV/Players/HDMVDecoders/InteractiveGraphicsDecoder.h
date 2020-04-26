//-----------------------------------------------------------------------------
// InteractiveGraphicsDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __INTERACTIVEGRAPHICSDECODER_H__
#define __INTERACTIVEGRAPHICSDECODER_H__

#include "HDMVDecoder_Types.h"

class NotificationEventManager;

//__________________________________________________________________________________________________________________________________________________
// InteractiveGraphicsDecoder Inteface
//


//--------------------------------------------------------------------------
// default implementation choices (mutually exclusive) for the IGDecoder interface
//
#define IGDECODER_IFACE_IMPL_INTERFACE		= 0;
#define IGDECODER_IFACE_IMPL_ASSERT			{ assert(false); }
#define IGDECODER_IFACE_IMPL_DEFAULT		{ return 0; }
#define IGDECODER_IFACE_IMPL				IGDECODER_IFACE_IMPL_INTERFACE


//--------------------------------------------------------------------------
// InteractiveGraphicsDecoder
/// @brief	IG Decoder interface
///
class InteractiveGraphicsDecoder
{
public:
	InteractiveGraphicsDecoder()										{};
	virtual ~InteractiveGraphicsDecoder()								{};

	// setup event notification channel
	//
	virtual void SetNotificationEventManager(ref_ptr<NotificationEventManager>	pNEManager)							IGDECODER_IFACE_IMPL

	// media control interface
	//
	virtual void Play(void)																							IGDECODER_IFACE_IMPL
	virtual void Pause(void)																						IGDECODER_IFACE_IMPL
	virtual void Stop(void)																							IGDECODER_IFACE_IMPL

	// Interactive User Operations
	//
	virtual SonicError MoveUpSelectedButton(void)																	IGDECODER_IFACE_IMPL
	virtual SonicError MoveDownSelectedButton(void)																	IGDECODER_IFACE_IMPL
	virtual SonicError MoveLeftSelectedButton(void)																	IGDECODER_IFACE_IMPL
	virtual SonicError MoveRightSelectedButton(void)																IGDECODER_IFACE_IMPL
	virtual SonicError SelectButton(UInt32 /*uiButtonNumber*/)														IGDECODER_IFACE_IMPL
	virtual SonicError ActivateButton(void)																			IGDECODER_IFACE_IMPL
	virtual SonicError SelectButtonAndActivate(UInt32 /*uiButtonNum*/)												IGDECODER_IFACE_IMPL
	virtual bool       IsPopUpAvailable(void)																		IGDECODER_IFACE_IMPL
	virtual bool       IsPopUpOn(void)																				IGDECODER_IFACE_IMPL
	virtual SonicError PopUpMenu(void)																				IGDECODER_IFACE_IMPL
	virtual SonicError PopUpOn(void)																				IGDECODER_IFACE_IMPL
	virtual SonicError PopUpOff(void)																				IGDECODER_IFACE_IMPL

	// Mouse input functions
	//
	virtual SonicError MousePosition(SInt32 /*x*/, SInt32 /*y*/, bool & /*bOnClickableButton*/)						IGDECODER_IFACE_IMPL
	virtual SonicError MouseClick(SInt32 /*x*/, SInt32 /*y*/)														IGDECODER_IFACE_IMPL

	// Button commands related functions (called by Module Manager)
	//
	virtual SonicError SetButtonPage(bool /*bSetPage*/, UInt8 /*uiPageId*/, bool /*bCurrPageOutEffect*/,
									 bool /*bSetButton*/, UInt16 /*uiButtonId*/)									IGDECODER_IFACE_IMPL
	virtual SonicError EnableButton(UInt32 /*uiButtonId*/)															IGDECODER_IFACE_IMPL
	virtual SonicError DisableButton(UInt32 /*uiButtonId*/)															IGDECODER_IFACE_IMPL
	virtual void       BtnCmdsExecuted(ButtonCmdsExecutionInfo & /*btnCmdsExecInfo*/)								IGDECODER_IFACE_IMPL

private:
	InteractiveGraphicsDecoder(const InteractiveGraphicsDecoder&); // no copying
	const InteractiveGraphicsDecoder& operator=(const InteractiveGraphicsDecoder&); // no copying
};

#endif // __INTERACTIVEGRAPHICSDECODER_H__
