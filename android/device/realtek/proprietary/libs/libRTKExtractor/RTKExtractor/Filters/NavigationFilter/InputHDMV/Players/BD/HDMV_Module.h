//-----------------------------------------------------------------------------
// HDMVModule.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMV_MODULE_H__
#define __HDMV_MODULE_H__

#include "BDROM_Module.h"

#include "BDROMIndex.h"
//#include "SonicThread.h"
#include "CanvasManager.h"
#include "PlaybackControlEngine.h"
#include "ModuleManager.h"
#include "BDROM_NavigatorTypes_Internal.h"

class InteractiveGraphicsDecoder;
struct IGButtonCommands;
class BDROM_NavigationTimer;
class BDROM_SoundPlayer;
class BDROM_TimerManager;
#ifndef REALTEK_MODIFY
class BDROM_Settings;
#endif

//__________________________________________________________________________________________________________________________________________________
// Defines for HDMV Module's internal use
//

const UInt32 kHDMV_Invalid_MObj_id				= 0xFFFF;

typedef enum tagNavCommandName
{
	kNC_None,
	kNC_LinkMK,
	kNC_LinkPI,
	kNC_TerminatePL,
	kNC_JumpObject,
	kNC_CallObject,
	kNC_JumpTitle,
	kNC_CallTitle,
	kNC_SetStream_IGFlag1,			// SetStream cmd with IG_flag=1
	kNC_SetButtonPage,
	kNC_PopUpOff,
	kNC_Resume,
	kNC_Break
} NavCommandName;


//__________________________________________________________________________________________________________________________________________________
/// @class	HDMVEnv
/// @brief	Environment for the HDMV Module. It is to be created and initiated by the Module Manager.
///
class HDMVEnv
{
public:
	HDMVEnv()
		: mbFirstPlayExists(false)
		, mbTopMenuExists(false)
		, mNumTitles(0)
		, mNumMObjs(0)
		, mbHDMVFirstPlay(false)
		, mbHDMVTopMenu(false)
		, mbHDMVTitle(0)
		, mbAllowJumpToNonHDMVTitle(true)
	{
	}

	virtual ~HDMVEnv()
	{
		if (mbHDMVTitle)
			delete [] mbHDMVTitle;
		mbHDMVTitle = 0;
	}

	// set functions
	//
	void	FirstPlaybackExists(bool bExists)		{ mbFirstPlayExists = bExists; }
	void	TopMenuExists(bool bExists)				{ mbTopMenuExists = bExists; }
	bool	SetNumberOfTitles(UInt16 uiNumTitles)
			{
				if (uiNumTitles)
				{
					mbHDMVTitle = new bool[uiNumTitles];
					if (mbHDMVTitle)
					{
						mNumTitles = uiNumTitles;
						for (UInt16 i = 0; i < uiNumTitles; i++)
							mbHDMVTitle[i] = false;
						return true;
					}
				}
				return false;
			}
	void	SetNumberOfMOBjects(UInt16 uiNumMObjs)	{ mNumMObjs = uiNumMObjs; }
	void	FirstPlaybackIsHDMV(bool bHDMV)			{ mbHDMVFirstPlay = bHDMV; }
	void	TopMenuIsHDMV(bool bHDMV)				{ mbHDMVTopMenu = bHDMV; }
	void	TitleIsHDMV(UInt16 uiTitleNum, bool bHDMV)
			{
				if (uiTitleNum <= mNumTitles)
					mbHDMVTitle[uiTitleNum-1] = bHDMV;
			}
	void	AllowJumpToNonHDMVTitle(bool bAllow)	{ mbAllowJumpToNonHDMVTitle = bAllow; }

	// get functions
	//
	bool	IsFirstPlayPresent(void)				{ return mbFirstPlayExists; }
	bool	IsTopMenuPresent(void)					{ return mbTopMenuExists; }
	UInt16	GetNumberOfTitles(void)					{ return mNumTitles; }
	UInt16	GetNumberOfMOBjects(void)				{ return mNumMObjs; }
	bool	IsFirstPlayHDMV(void)					{ return mbHDMVFirstPlay; }
	bool	IsTopMenuHDMV(void)						{ return mbHDMVTopMenu; }
	bool	IsTitleHDMV(UInt16 uiTitleNum)
			{
				if ((uiTitleNum > 0) && (uiTitleNum <= mNumTitles))
					return mbHDMVTitle[uiTitleNum-1];
				else if (uiTitleNum == 0)
					return mbHDMVTopMenu;
				else if (uiTitleNum == 0xFFFF)
					return mbHDMVFirstPlay;
				else
					return false;
			}
	bool	IsJumpToNonHDMVTitleAllowed(void)	{ return mbAllowJumpToNonHDMVTitle; }

private:
	// movie object, title information
	bool		mbFirstPlayExists;			///< Flag to indicate existance of a First Playback
	bool		mbTopMenuExists;			///< Flag to indicate existance of a Top Menu
	UInt16		mNumTitles;					///< Number of titles
	UInt16		mNumMObjs;					///< Number of Movie Objects
	bool		mbHDMVFirstPlay;			///< Flag to indicate if First Playback is a HDMV object
	bool		mbHDMVTopMenu;				///< Flag to indicate if Top Menu is a HDMV object
	bool *		mbHDMVTitle;				///< Flag to indicate which titles are HDMV titles

	// Feature control
	bool		mbAllowJumpToNonHDMVTitle;	///< Flag to allow/prohibit nav commands that cause a jump to a non-HDMV title
};


//__________________________________________________________________________________________________________________________________________________
/// @struct	ScenarioPresentationState
/// @brief	HDMV Module's command execution state
///
typedef enum tagScenarioPresentationState
{
	kSPS_Idle,									///< Executing Movie Object Nav Cmds.
	kSPS_PlaybackExecution_Play,				///< In playback state. Not executing any Cmds.
	kSPS_PlaybackExecution_ButtonCommands,		///< Executing Button Cmds.
	kSPS_ScenarioTransition_JumpTitle,			///< In a Jump Title type branch.
	kSPS_ScenarioTransition_CallTitle,			///< In a Call Title type branch.
	kSPS_ScenarioTransition_JumpObject,			///< In a Jump Object type branch.
	kSPS_ScenarioTransition_CallObject,			///< In a Call Object type branch.
	kSPS_ScenarioTransition_Resume,				///< In a Resume type branch.
	kSPS_ScenarioTransition_Suspend,			///< In a Suspend type branch.
	kSPS_Terminated								///< Getting out of processing loop.
} ScenarioPresentationState;


//__________________________________________________________________________________________________________________________________________________
/// @struct	HDMVMod_MObjState
/// @brief	Stores a Movie Object's state info
///
struct HDMVMod_MObjState
{
	UInt32						m_uiMObjId;			///< Movie Object id
	UInt32						m_uiNavCmdNum;		///< Nav Command
	ScenarioPresentationState	m_State;			///< Movie object command execution state

	HDMVMod_MObjState()		{ m_uiMObjId = 0; m_uiNavCmdNum = 1; m_State = kSPS_Terminated; }
};


//__________________________________________________________________________________________________________________________________________________
/// @struct	HDMVMod_ResumeInfo
/// @brief	Stores Module Manager's resume information
///
struct HDMVMod_ResumeInfo
{
	bool						m_bValid;			///< True if resume info is valid
	HDMVMod_MObjState			m_MObj;				///< Movie object to resume at

	HDMVMod_ResumeInfo()								{ Clear(); }

	void   Clear(void)									{ m_bValid = false; }
	void   Store(HDMVMod_MObjState & mobj)				{ m_MObj = mobj; m_bValid = true; }

	bool   IsValid(void)								{ return m_bValid; }
	UInt32 MObjId(void)									{ return m_MObj.m_uiMObjId; }
	UInt32 NavCmdNum(void)								{ return m_MObj.m_uiNavCmdNum; }
	ScenarioPresentationState State(void)				{ return m_MObj.m_State; }
};


//__________________________________________________________________________________________________________________________________________________
/// @class	HDMV_Module
/// @brief	HDMV Module. It must be initialized with a HDMV Environment before it can process any commands.
///
class HDMV_Module : public BDROM_Module
{
#ifdef REALTEK_MODIFY_RESUMEPLAY	
	friend class ModuleManager;	
#endif
	
public:
	HDMV_Module(ref_ptr<CanvasManager> pCM, ref_ptr<PlaybackControlEngine> pPBCE, InteractiveGraphicsDecoder * pIGDecoder, 
#ifndef REALTEK_MODIFY		
	ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
#endif	
		ref_ptr<BDROM_TimerManager> pTimerManager, 
#ifdef REALTEK_MODIFY		
		void* pSettings = 0);
#else
		ref_ptr<BDROM_Settings> pSettings = 0);
#endif		
	~HDMV_Module();

	// Initialization
	SonicError SetEnvironment(HDMVEnv * env);
	SonicError Activate(ref_ptr<BDROM::MovieObject_bdmv> mobj);

	// MovieObject and Title playback
	SonicError JumpToMovieObject(UInt32 playback_type, UInt32 mobj_ID);
	SonicError JumpToMovieObject(UInt32 mobj_ID);
	SonicError Terminate();
	SonicError Suspend(bool bUseResumeIntentionFlag = false);		// suspends PBCE playback and stores movie object resume info or stops playback
	SonicError Resume(void);										// resumes movie using previously stored resume info
	void       DiscardResumeInfo(void);								// clears resume info
	void       Stop(bool bDiscardResumeInfo = true);

	// button command and user events
	SonicError SetButtonCommands(IGButtonCommands * pBtnCmds);
	SonicError UserEvent(BDROM_InputKeyCodes bdKeyCode, bool keyPressed = true);

	// main processing loop
	SonicError Process(UInt32 & nextTitle);
	SonicError ProcessStreamDone(bool bRepeat = false );	//REALTEK_MODIFY_REPEAT_TITLE						// notification that current PL playback is done

	// get information
	SonicError GetTerminalInfo(bool *pMenu_call_mask, bool *pTitle_search_mask);
	SonicError IsPopUpAvailable(bool & bAvailable);
	bool       IsPopUpOn(void);

	// Timer
	SonicError KillNVTimer();
	SonicError DecrementNVTimer();

	// Setup optional callback to occur when an HDMV Navi Command is executed
	void SetNaviCommandCallback(NaviCommandCallback* callback) { m_pNaviCommandCallback = callback; }

protected:
	HDMV_Module(const HDMV_Module&); // no copying
	const HDMV_Module& operator=(const HDMV_Module&); // no copying

	// command processing
	SonicError ProcessCommand(BDROM::navigation_command &nc);
	SonicError ProcessBranchCommand(BDROM::navigation_command &nc);
	SonicError ProcessCompareCommand(BDROM::navigation_command &nc);
	SonicError ProcessSetCommand(BDROM::navigation_command &nc);
	void GetOperands(BDROM::navigation_command& internalCommandInfo);
	void GetCommonOperands(BDROM::navigation_command& internalCommandInfo, UInt32 value1Mask = 0, UInt32 value2Mask = 0);
	void CommandInfoInternalToExternal(BDROM::navigation_command &internalCommandInfo);

	void DiscardButtonCommands(void);


protected:
	// HDMV Enviroment
	HDMVEnv *							m_pEnv;
#ifndef REALTEK_MODIFY		
	ref_ptr<BDROM_Settings>				m_pSettings
#endif
	// Navigation modules
	InteractiveGraphicsDecoder *		m_pIGDecoder;
#ifndef REALTEK_MODIFY		
	ref_ptr<BDROM_SoundPlayer>			m_pSoundPlayer;
#endif
	// current state
	ref_ptr<BDROM::MovieObject_bdmv>	m_pMobj;							///< Movie Object in the current disc
	HDMVMod_MObjState					m_currentMObjState;					///< Current Movie Object's execution state
	UInt32								m_current_Navigation_command;		///< Nav Cmd number of current MObj or Btn cmds being executed
	ScenarioPresentationState			m_State;							///< HDMV Module's execution State
	UInt32								m_NextTitleOrMObj;					///< Next title or movie object to call/jump to

	// resume information
	HDMVMod_ResumeInfo					m_ResumeInfo;						///< Resume Info that contains movie object state to resume to

	// Nav Timer processing
	ref_ptr<BDROM_NavigationTimer>		m_navTimer;
	ref_ptr<BDROM_TimerManager>			m_pTimerManager;

	// processing thread state
	bool								m_bExit;
	SonicCriticalSection 				m_csLock;							///< Lock to serialize state changes

	// button command processing
	std::list<IGButtonCommands *>		m_ButtonCommandsQueue;				///< Queue for next set of button commands (will be processed after current set)
	IGButtonCommands *					m_pButtonCommands;					///< Current set of button commands being processed
	bool								m_bEndButtonCmdProcessing;			///< set to true during cmd processing if button cmd processing is to be stopped.
	NavCommandName						m_LastButtonCmdProcessed;			///< the last button command that was processed

	NaviCommandCallback*				m_pNaviCommandCallback;				///< Optional callback to send information when a NaviCmd is executed
	HDMV_CommandInfo					m_NaviCommandCallbackInfo;			///< Command info in a format that can be sent back to a client via the optional callback mechanism

	BDROM_RandomGen						m_RndGen;							///< random number generator
	

#ifdef REALTEK_MODIFY_RESUMEPLAY
	bool 				m_bBookMark;
	DVDTimeStampType	m_markAtTime;
	#ifdef REALTEK_MODIFY_BUG_30406
	int					m_markAtPlNum;
	#endif
	#ifdef REALTEK_MODIFY_BUG_28722
	int                 m_markAtAudioNum;
	int                 m_markAtSpuNum;
	#endif
#endif	
};

#endif
