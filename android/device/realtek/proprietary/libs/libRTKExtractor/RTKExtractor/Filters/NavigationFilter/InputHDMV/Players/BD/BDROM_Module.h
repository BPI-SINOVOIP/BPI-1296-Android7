//-----------------------------------------------------------------------------
// BDROM_Module.h
// Copyright (c) Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//__________________________________________________________________________________________________________________________________________________
/// @class	BDROM_Module
/// @brief	Base class for BDROM Modules (HDMV, BD-J)
///

#ifndef __BDROM_MODULE_H__
#define __BDROM_MODULE_H__

#include "BDJModuleTypes.h"
#include "ref_ptr.h"
#include "BDROM_NavigatorErrors.h"
#include "CanvasManager.h"
#include "DVDnUtilities.h"
#include "NotificationEventManager.h"

class HDMVEnv;
struct IGButtonCommands;
class PlaybackControlEngine;

class BDROM_Module
{
public:
	BDROM_Module(ref_ptr<CanvasManager> pCM, ref_ptr<PlaybackControlEngine> pPCE): m_pCM(pCM), m_pPBCE(pPCE), m_pNEManager(0)
	{ }

	virtual ~BDROM_Module()
	{
		m_pCM = 0;
		m_pPBCE = 0;
	}

	virtual SonicError Process(UInt32 & /*nextTitle*/)												{ return 0; }
	virtual void Stop(bool bDiscardResumeInfo = true)												{ /*bDiscardResumeInfo; */}

	virtual SonicError ProcessStreamDone(bool bRepeat = false/*REALTEK_MODIFY_REPEAT_TITLE*/)															{ return 0; }
	virtual SonicError UserEvent(BDROM_InputKeyCodes /*bdKeyCode*/, bool keyPressed = true)			{ /*keyPressed;*/ return 0; }
	virtual SonicError MouseEvent(UInt32 /*uiX*/, UInt32 /*uiY*/, bool /*bLeftButtonClicked*/, bool& /*bOnClickableButton*/) {return 0;}
	virtual SonicError SetButtonCommands(IGButtonCommands * /*pBtnCmds*/)							{ return 0; }

	virtual void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager)			{ m_pNEManager = pNEManager; }

	// only meant for HDMV Module. 
	virtual SonicError SetEnvironment(HDMVEnv * /*env*/)											{ return 0;	}
	
	virtual SonicError GetTerminalInfo(bool * /*pMenu_call_mask*/, bool * /*pTitle_search_mask*/)		{ return 0; }

	virtual SonicError IsPopUpAvailable(bool & /*bAvailable*/)										{ return -1; }
	virtual bool       IsPopUpOn(void)																{ return false; }

	CanvasManager * GetCM(void)																		{ return (m_pCM) ? m_pCM.get() : 0; }	//REALTEK_MODIFY
	PlaybackControlEngine * GetPBCE(void)															{ return m_pPBCE.get(); }

protected:
	ref_ptr<CanvasManager> m_pCM;
	ref_ptr<PlaybackControlEngine> m_pPBCE;
	ref_ptr<NotificationEventManager>	m_pNEManager;
};

#endif
