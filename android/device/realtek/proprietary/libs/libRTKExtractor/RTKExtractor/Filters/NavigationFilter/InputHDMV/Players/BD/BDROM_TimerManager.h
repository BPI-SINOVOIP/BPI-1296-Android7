//-----------------------------------------------------------------------------
// BDROM_TimerManager.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BDROM_TimerManager_h
#define __BDROM_TimerManager_h


#include "ref_ptr.h"
#include "SonicCriticalSection.h"
#include "TimeValue.h"
#include "BDROM_NavigatorTypes_Internal.h"

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	#include <OSAL.h>
#else
	#include "SonicThread.h"
#endif

class BaseRenderer;
class BDROM_Timer;
class NotificationEventManager;

#ifdef REALTEK_MODIFY
class BDROM_TimerManager
#else
class BDROM_TimerManager: public SonicThread
#endif	
{
public:
public:
	BDROM_TimerManager(ref_ptr<BaseRenderer> pRenderer, ref_ptr<NotificationEventManager> pNEManager);
	~BDROM_TimerManager();

	void AddTimer(ref_ptr<BDROM_Timer> pTimer);
	void RemoveTimer(ref_ptr<BDROM_Timer> pTimer);
	
	ref_ptr<NotificationEventManager> GetNotificaitonEventManager() { return m_pNEManager; }
    
    void PauseClock(EBDROMPrecisionClock clockId);
    void ResumeClock(EBDROMPrecisionClock clockId);

	// Thread control
	//
public:
	SonicError Exit(void);
	SonicError Start(void);
	SonicError Stop(void);
#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
#endif
protected:
	SonicError Run(void);
	void Process(void);
	void ProcessEvent(void);

protected:
	SonicCriticalSection 				m_csLock;					///< Lock to serialize threads accessing/modifying intenal data

	bool								m_bProcess;
	bool								m_bExit;

	ref_ptr<BaseRenderer>				m_pRenderer;
	ref_ptr<NotificationEventManager>	m_pNEManager;
	std::vector< ref_ptr<BDROM_Timer> >	m_timeList;
		
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 								m_pThread;
#endif		
};
#endif
