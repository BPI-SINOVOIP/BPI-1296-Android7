//-----------------------------------------------------------------------------
// ModuleManager.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __MODULEMANAGER_H__
#define __MODULEMANAGER_H__

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	#include <OSAL.h>
#else
	#include "SonicThread.h"
#endif

#include "SonicSemaphore.h"


#include "HDMV_Module.h"
// additional headers needed for including BDJ_Module.h
#include "BDROMNavigationCache.h"
#include "BDROM_Module.h"

#include "PlaybackControlEngine.h"
//#include "VFSManager.h"
#include "BDJModuleTypes.h" 
#if defined(ENABLE_BDJ_TITLE) && !defined(REALTEK_MODIFY)
	#include <jni.h>
	
	#include "BDJ_Module.h"
	class BDJ_Module;
#endif
class BDROM_Module;

class HDMV_Module;
struct IGButtonCommands;
class BDROMAppEventSink;
class HDMVEnv;
class BDROM_Navigator;

#ifndef REALTEK_MODIFY
	class BDROM_Settings;
#endif

//__________________________________________________________________________________________________________________________________________________
/// @struct	ModuleMgr_ResumeInfo
/// @brief	Stores Module Manager's resume information
///
struct ModuleMgr_ResumeInfo
{
	bool		m_bValid;			///< True if resume info is valid
	UInt32		m_uiTitleNum;		///< Title Number to resume at

	ModuleMgr_ResumeInfo()				{ Clear(); }

	void   Clear(void)					{ m_bValid = false; }
	void   Store(UInt32 titleNum)		{ m_uiTitleNum = titleNum; m_bValid = true; }

	bool   IsValid(void)				{ return m_bValid; }
	UInt32 Title(void)					{ return m_uiTitleNum; }
};


//__________________________________________________________________________________________________________________________________________________
/// @class	ModuleManager
/// @brief	BDROM Module Manager. It also is the User Event Manager. All events generated within the BDROM Navigator are sent to the Module Manager
///
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class ModuleManager  : public NotificationEventSink
#else
class ModuleManager  : public SonicThread, public NotificationEventSink
#endif
{
public:
#if defined(ENABLE_BDJ_TITLE) && !defined(REALTEK_MODIFY)	
	ModuleManager(BDROM_Navigator* pNavigator, ref_ptr<BDROM::BDROM_NavigationCache> pNavCache, ref_ptr<HDMV_Module> pHDMV_Module, ref_ptr<BDJ_Module> pBDJ_Module,	ref_ptr<BDROM_Settings> pSettings = 0);
#else
	ModuleManager(BDROM_Navigator* pNavigator, ref_ptr<BDROM::BDROM_NavigationCache> pNavCache, ref_ptr<HDMV_Module> pHDMV_Module);
#endif	
	~ModuleManager();

	void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);

	// Main processing loop
	SonicError Run();
	SonicError RunHDMV(void);	// runs a HDMV title
	SonicError RunBDJ(void);	// runs a BDJ title

	SonicError Play(void);		// resumes thread to start playback from 'First Playback'
	SonicError Stop(void);		// stops all processing and suspends thread
	SonicError PauseOn(void);	// kill the navigator timer
	SonicError PauseOff(void);	// do nothing

	void ThreadStart();			// start the Module manager thread
	void ThreadStop();			// terminates all processing and the Module manager thread
	void Exit();				// terminates all processing and the Module manager thread
#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
#endif

	typedef enum tagApplicationEnvironmentState
	{
		kAES_Idle,				// Doing nothing (initial state)
		kAES_Starting,			// Playing Starting
		kAES_Running,			// Playing a title
		kAES_TitleTransition,	// Transition from one title to another
		kAES_Resume,			// Resume operation
		kAES_Stopping,			// Playback being Stopped
		kAES_Stopped,			// Playback Stopped
		kAES_Terminated,		// Module being terminated
	} ApplicationEnvironmentState;

	// user events and other commands
	SonicError KeyEvent(BDROM_KeyEventCode keyEvent, BDROM_KeyState keyState);		// handle only BDJ key events
	SonicError MouseEvent(UInt32 uiX, UInt32 uiY, bool bLeftButtonClicked, bool& bOnClickableButton);
	SonicError UserEvent(BDROM_InputKeyCodes bdKeyCode);							// handle BDJ key events and more.
	SonicError TitleSearch(UInt32 uiTitleNum, bool bBookMark = false);	// Processes TitleSearch User Op. stops current title and starts new title
	SonicError MenuCall(void);					// Processes MenuCall User Op. stops/suspends current title and jumps to top menu.
	SonicError ResumePlayList(void);			// Processes Resume User Op. stops current title and jumps to suspended title.

	SonicError ProcessStreamDone(void);

	// get information
	BDROM_PlayerState GetPlayerState(void);
	SonicError GetDiscId(BDROMDiscId & discId);

	SonicError GetNumberOfTitles(UInt32 & uiNumTitles);
	SonicError GetNumberOfTitles(UInt32 & uiNumTitles, bool & bHasFirstPlayback, bool & bHasTopMenu);
	SonicError GetTitleType(UInt32 uiTitleNum, BDROM_ObjectType & objType, BDROM_TitleType & titleType);
	SonicError GetTitleAccessType(UInt32 uiTitleNum, BDROM_TitleAccessType & accessType);
	bool       IsFirstPlaybackPresent(void);
	bool       IsTopMenuPresent(void);
    bool       IsInteractiveTitle(UInt32 uiTitleNum);
	bool	   IsTitleAccessAllowed(UInt32 uiTitleNum);	// check the title_access_type 
	bool	   IsTitleSearchAllowed();					// check the title_search_mask UO
	bool	   IsMenuCallAllowed();						// check the menu_call_mask	UO
	SonicError IsPopUpAvailable(bool & bAvailable);
	bool       IsPopUpOn(void);
	SonicError IsResumePossible(bool & bPossible);

	SonicError GetCurrentTitleDuration(DVDTimeStampType & titleDuration);
    SonicError GetCurrentTitleNum(UInt16 & uiTitleNum);
	SonicError GetCurrentTitleType(BDROM_TitleType & titleType);

	// Region Control
	SonicError SetRegion(UInt32 uiRgnCode);

	// Get Module Pointers
	BDROM_Navigator*  GetNavigator(void);
	ref_ptr<HDMV_Module> GetHDMVModule(void);
#if defined(ENABLE_BDJ_TITLE) && !defined(REALTEK_MODIFY)	
	ref_ptr<BDJ_Module>  GetBDJModule(void);
#endif
	SonicError DiscUnboundFirstPlay();

	//---------------------------------------------------------------------------------
	// API for breaking and continuing playback - For Emulator use only

	// Break: Pauses playback.
	SonicError Break(void);

	// Continue: Resumes playback
	SonicError Continue(void);

#ifdef	REALTEK_MODIFY_REPEAT_TITLE
	void SetRepeatTitle(bool bEnable) { m_bRepeatTitle = bEnable; }
#endif

#ifdef REALTEK_MODIFY_RESUMEPLAY
	SonicError ResumePlay(int titleNum, int chpaterNum, int time, int plNum = -1, int audioStreamNum =-1, int spuStreamNum =-1);
#endif
	
protected:
	bool       OpenIndexFile(void);				// opens index.bdmv
	bool       OpenMovieObjectFile(void);		// opens MovieObject.bdmv

	SonicError OpenTitle(UInt32 uiTitleNum);
	SonicError OpenHDMVTitle(UInt32 uiTitleNum, BDROM_TitleType titleType);
	SonicError OpenBDJTitle(UInt32 uiTitleNum);

	BDROM_Domain GetDomain(UInt32 uiTitleNum);

	SonicError ProcessFatalError(void);
	void       ProcessResumeAvailabilityChange(bool bBefore, bool bAfter);
	SonicError SendEvent(BDROM_Event evt);

	SonicError NewTitleLoadBegin(UInt32 uiTitleNum, BDROM_ObjectType objType, BDROM_TitleType  titleType);
public:
	void       NewTitleLoadEnd(UInt32 uiTitleNum, BDROM_ObjectType objType, BDROM_TitleType  titleType);
protected:
	void       NewMOBJorBDJOLoaded(void);

	// NotificationEventSink interface implementation
public:
	virtual SonicError NotifyEvent(BDROM_Event event);

protected:
    BDROM_Navigator*                        m_pNavigator;		
	ref_ptr<HDMV_Module>					m_pHDMV_Module;
#if defined(ENABLE_BDJ_TITLE) && !defined(REALTEK_MODIFY)	
	ref_ptr<BDJ_Module>						m_pBDJ_Module;
	ref_ptr<BDROM_Settings>					m_pSettings;
#endif
	ref_ptr<BDROM_Module>					m_pCurrentModule;
	ref_ptr<BDROM::BDROM_NavigationCache>	m_pNavCache;
	ref_ptr<BDROM::index_bdmv>				m_pIndexBdmv;		///< Index.bdmv data
	ref_ptr<BDROM::MovieObject_bdmv>		m_pMovieObjectBdmv;	///< MovieObject.bdmv data
	ref_ptr<HDMVEnv>						m_pHDMVEnv;			///< Current HDMV Environment
	char									m_bdmv_orgId[9];	///< Org ID from id.bdmv
	char									m_bdmv_discId[33];	///< Disc ID from id.bdmv

	ref_ptr<NotificationEventManager>		m_pNEManager;

	bool									m_bExit;					///< Exit from main processing loop (will terminate the thread)
	bool									m_bExitCurrentModule;		///< Exit from current module's processing loop
	bool									m_bUserModuleExitRequest;	///< true if the current module was stopped by the Module Manager
	UInt32									m_FirstTitle;			///< Title to Play when switching from Stop to Run state
	UInt32									m_uiCurrentTitleNum;	///< Current Title number
	UInt32									m_NextTitle;			///< Next title to load and play

	SonicCriticalSection 					m_csLock;			///< Lock to serialize state changes
	SonicCriticalSection 					m_titleLock;		///< Lock to serialize title changes
	ApplicationEnvironmentState				m_CurrState;		///< current state of the Module Manager
	bool									m_bStateChange;		///< flag to indicate a state change is required
	ApplicationEnvironmentState				m_NextState;		///< next state to change to 

	UInt32									m_uiRgnCode;		///< Player Region Code

	ModuleMgr_ResumeInfo					m_ResumeInfo;		///< Information needed to perform a resume

	bool									m_bInBreak;			///< true if Break() has been called (and Continue() has not)

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 								m_pThread;
#endif

//#ifdef REALTEK_MODIFY_REPEAT_TITLE
	bool									m_bRepeatTitle;
//#endif
};

#endif
