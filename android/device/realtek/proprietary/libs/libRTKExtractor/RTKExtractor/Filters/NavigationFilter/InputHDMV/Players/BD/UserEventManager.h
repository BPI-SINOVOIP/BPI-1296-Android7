//-----------------------------------------------------------------------------
// UserEventManager.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __USEREVENTMANAGER_H__
#define __USEREVENTMANAGER_H__

#include "NotificationEventManager.h"
#include "BDROMCommonTypes.h"
#include "BDROM_NavigatorTypes_Internal.h"
#include "ModuleManager.h"
//#include "InteractiveGraphicsDecoder.h"
//#include "HDMVDecoder_Types.h"


// _________________________________________________________________________________________________________________________
// UOPSet
//
// Set of UOP components and the final UOPs
//

// UOPs built from Terminal Info
struct Title_UO_mask_table
{
	UInt8						mmenu_call_mask;
	UInt8						mtitle_search_mask;
};

struct UOPSet
{
	// object, title, playlist playback type
	BDROM_ObjectType			m_ObjectType;		///< Current FirstPlay/TopMenu/Title Object Type
	BDROM_TitleType				m_TitleType;		///< Current FirstPlay/TopMenu/Title Playback type

	// UOP tables
	Title_UO_mask_table			m_Title_UOPs;		///< UOP table from current Title's terminal info
	BDROM::UO_mask_table		m_PlayList_UOPs;	///< UOP table from current PlayList's AppInfo
	BDROM::UO_mask_table		m_PlayItem_UOPs;	///< UOP table from current PlayItem
	BDROM::UO_mask_table		m_Page_UOPs;		///< UOP table from current IG Page (if page is on display)

	BDROM::UO_mask_table		m_Player_UOPs;		///< UOP table created by combining the Title, PlayList, PlayItem and Page UOPs, and Object & Title type restrictions

public:
	UOPSet()					{ Clear(); }

	// Clears all UOPs. default is to allow all user operations
	void Clear(bool bAllowByDefault = true)
	{
		m_ObjectType = kBDROM_ObjectType__Unknown;
		m_TitleType  = kBDROM_TitleType__Unknown;

		UInt8 defaultUOPs = (bAllowByDefault ? BDROM_UOP_ALLOWED : BDROM_UOP_PROHIBITED);
		m_Title_UOPs.mmenu_call_mask = defaultUOPs;
		m_Title_UOPs.mtitle_search_mask = defaultUOPs;
		m_PlayList_UOPs.SetAll(defaultUOPs);
		m_PlayItem_UOPs.SetAll(defaultUOPs);
		m_Page_UOPs.SetAll(defaultUOPs);
		m_Player_UOPs.SetAll(defaultUOPs);
	}
};


// _________________________________________________________________________________________________________________________
// UserEventManager
//
// The User Event Manager maintains the UOPs for the system. It receives UOPs for the PlayList, PlayItem and Page and
// maintains an updated combined UOP table
//

class UserEventManager : public NotificationEventSink
{
public:
	UserEventManager(ref_ptr<ModuleManager> pModuleManager, InteractiveGraphicsDecoder * pIGDecoder);
	virtual ~UserEventManager();

	void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);

	// Event procesing
	//
public:
	// NotificationEventSink interface implementation
	SonicError NotifyEvent(BDROM_Event evt);

	// User Event Manager's Key Interest Table
protected:
	class KeyIntrTable
	{
	public:
		KeyIntrTable()								{ m_KeyInterestTable = 0; }
		KeyIntrTable(UInt32 table)					{ m_KeyInterestTable = table; }
		~KeyIntrTable()								{ }

		void Set(const UInt32 table)				{ m_KeyInterestTable = table; }
		void Get(UInt32 & table)					{ table = m_KeyInterestTable; }

		bool HasPlay(void)							{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_PLAY) > 0;						}
		bool HasStop(void)							{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_STOP) > 0;						}
		bool HasFastFwd(void)						{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_FAST_FWD) > 0;					}
		bool HasRewind(void)						{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_REWIND) > 0;					}
		bool HasTrackNext(void)						{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_TRACK_NEXT) > 0;				}
		bool HasTrackPrev(void)						{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_TRACK_PREV) > 0;				}
		bool HasPause(void)							{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_PAUSE) > 0;					}
		bool HasStillOff(void)						{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_STILL_OFF) > 0;				}
		bool HasSecondaryAudioEnableDisable(void)	{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_SEC_AUDIO_ENABLE_DISABLE) > 0;	}
		bool HasSecondaryVideoEnableDisable(void)	{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_SEC_VIDEO_ENABLE_DISABLE) > 0;	}
		bool HasPGTextSTEnableDisable(void)			{ return (m_KeyInterestTable & KeyIntrTable::KITF_VK_PG_TEXTST_ENABLE_DISABLE) > 0;	}

	private:
		UInt32 m_KeyInterestTable;

		enum KIT_Flags
		{
			KITF_VK_PLAY						= 0x80000000,
			KITF_VK_STOP						= 0x40000000,
			KITF_VK_FAST_FWD					= 0x20000000,
			KITF_VK_REWIND						= 0x10000000,
			KITF_VK_TRACK_NEXT					= 0x08000000,
			KITF_VK_TRACK_PREV					= 0x04000000,
			KITF_VK_PAUSE						= 0x02000000,
			KITF_VK_STILL_OFF					= 0x01000000,
			KITF_VK_SEC_AUDIO_ENABLE_DISABLE	= 0x00800000,
			KITF_VK_SEC_VIDEO_ENABLE_DISABLE	= 0x00400000,
			KITF_VK_PG_TEXTST_ENABLE_DISABLE	= 0x00200000
		};
	};

protected:
	SonicError ProcessTitleStart(BDROM_Event & evt);
	SonicError ProcessMOBJorBDJOStart(BDROM_Event & evt);
	SonicError ProcessPlaylistEvent(BDROM_Event & evt);
	SonicError ProcessUOPEvent(BDROM_Event & evt);
	void       UpdateUserEventResponseTable(BDROM_TitleType titleType, UOPSet & uops, KeyIntrTable & kit, BDROM_UserEventResponseTable & uert);

	typedef enum tagDefaultUERTableAndUOPType
	{
		kDefUERT_UOP__StopDomain,						// Stop domain, with no disc-unbound apps running.
		kDefUERT_UOP__StopDomainDiscUnboundApps,		// Stop domain, with 1 or more disc-unbound apps running.
		kDefUERT_UOP__TitleStart,						// At start of a new Title, with no title-unbound apps.
		kDefUERT_UOP__TitleStartTitleUnboundApps		// At start of a new Title, with 1 or more title-unbound apps.
	} DefaultUERTableAndUOPType;


	// User Operations processing
	//
public:

	// UOP related functions implementation
	//
	void GetUserEventResponseTable(BDROM_UserEventResponseTable & userEventResponseTable)
												{ SonicAutoCriticalSection Lock(&m_csLock); userEventResponseTable = m_UserEventResponseTable; }

	// Title Control User Operations
	//
	bool IsResumeAllowed(void)					{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mresume_mask == 0); }
	bool IsStopAllowed(void)					{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mstop_mask == 0); }

	// Playback Control User Operations
	//
	bool IsChapterSearchAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mchapter_search_mask == 0); }
	bool IsTimeSearchAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mtime_search_mask == 0); }
	bool IsSkipToNextPointAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mskip_to_next_point_mask == 0); }
	bool IsSkipBackToPrevPointAllowed(void)		{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mskip_back_to_previous_point_mask == 0); }
	bool IsPauseOnAllowed(void)					{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mpause_on_mask == 0); }
	bool IsPauseOffAllowed(void)				{ return true; /* no UOP defined in spec (yet) */ }
	bool IsStillOffAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mstill_off_mask == 0); }
	bool IsForwardPlayAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mforward_play_mask == 0); }
	bool IsBackwardPlayAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mbackward_play_mask == 0); }
	bool IsPrimaryAudioChangeAllowed(void)		{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mprimary_audio_stream_number_change_mask == 0); }
	bool IsAngleChangeAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mangle_number_change_mask == 0); }
	bool IsPGTextSTEnableDisableAllowed(void)	{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mPG_textST_enable_disable_mask == 0); }
	bool IsPGTextSTChangeAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mPG_textST_stream_number_change_mask == 0); }
	bool IsSecVideoEnableDisableAllowed(void)	{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.msecondary_video_enable_disable_mask == 0); }
	bool IsSecVideoChangeAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.msecondary_video_stream_number_change_mask == 0); }
	bool IsSecAudioEnableDisableAllowed(void)	{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.msecondary_audio_enable_disable_mask == 0); }
	bool IsSecAudioChangeAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.msecondary_audio_stream_number_change_mask == 0); }
	bool IsPiPPGTextSTChangeAllowed(void)		{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mPiP_PG_textST_stream_number_change_mask == 0); }
	bool IsPlayAllowed(void)					{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_TitleType != kBDROM_TitleType__BDJ_Movie); }
	bool IsTitleSearchAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return m_pModuleManager->IsTitleSearchAllowed(); }
	bool IsMenuCallAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return m_pModuleManager->IsMenuCallAllowed(); }

	// Interactive User Operations
	//
	bool IsMoveUpSelBtnAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mmove_up_selected_button_mask == 0); }
	bool IsMoveDownSelBtnAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mmove_down_selected_button_mask == 0); }
	bool IsMoveLeftSelBtnAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mmove_left_selected_button_mask == 0); }
	bool IsMoveRightSelBtnAllowed(void)			{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mmove_right_selected_button_mask == 0); }
	bool IsSelectBtnAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mselect_button_mask == 0); }
	bool IsActivateBtnAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mactivate_button_mask == 0); }
	bool IsSelectAndActivateBtnAllowed(void)	{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mselect_and_activate_mask == 0); }
	bool IsPopUpOnAllowed(void)					{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mpopup_on_mask == 0); }
	bool IsPopUpOffAllowed(void)				{ SonicAutoCriticalSection Lock(&m_csLock); return (m_CurrUOPs.m_Player_UOPs.mpopup_off_mask == 0); }

	// User Op vs. Key Event
	//
	bool IsUserOperation(BDROM_UserEventCode u)	{ SonicAutoCriticalSection Lock(&m_csLock); return (m_UserEventResponseTable.GetResponseType(u) == kBDROM_ResponseType_UserOperation); }
	bool IsKeyEvent(BDROM_UserEventCode u)		{ SonicAutoCriticalSection Lock(&m_csLock); return (m_UserEventResponseTable.GetResponseType(u) == kBDROM_ResponseType_KeyEvent); }

protected:
	SonicCriticalSection 				m_csLock;				///< lock used to access UOPs
	ref_ptr<ModuleManager>				m_pModuleManager;		///< Module Manager
	InteractiveGraphicsDecoder *		m_pIGDecoder;			///< IG Decoder
	ref_ptr<NotificationEventManager>	m_pNEManager;			///< Event Manager

	UOPSet								m_CurrUOPs;					///< Current UOPs
	UOPSet								m_CurrOrNewerUOPs;			///< Current or Newer UOPs (e.g., used to store new UOPs at title/playlist start)
	bool								m_bHaveNewerUOPs;			///< true if m_CurrOrNewerUOPs is newer than m_CurrUOPs
	KeyIntrTable						m_CurrKeyIntrTable;			///< Current Key Interest Table
	BDROM_UserEventResponseTable		m_UserEventResponseTable;	///< The lastest User Event Reponse Table.
};

#endif
