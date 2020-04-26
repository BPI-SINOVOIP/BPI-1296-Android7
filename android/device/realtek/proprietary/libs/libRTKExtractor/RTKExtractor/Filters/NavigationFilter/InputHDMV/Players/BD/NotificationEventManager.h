//-----------------------------------------------------------------------------
// NotificationEventManager.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __NOTIFICATIONEVENTMANAGER_H__
#define __NOTIFICATIONEVENTMANAGER_H__

#include "ref_ptr.h"
#include "BDROM_NavigatorTypes.h"
#include "SonicCriticalSection.h"
#include "BDROM_ApplicationEventSink.h"


//--------------------------------------------------------------------------
// NotificationEventSink
/// @brief	BDROM Internal Event Sink. Event sinks subscribe to events (i.e., eventually, they will).
///
class NotificationEventSink
{
public:
	NotificationEventSink()				{;}
	virtual ~NotificationEventSink()	{;}

	virtual SonicError NotifyEvent(BDROM_Event) = 0;
};


//--------------------------------------------------------------------------
// NotificationEventSinkType
/// @brief	Sink Type
///
typedef enum tagNotificationEventSinkType
{
	kEventSinkType_ModuleManager,
	kEventSinkType_PBCE,
	kEventSinkType_AACSAuthListener,
	kEventSinkType_UserEventManager,
	kEventSinkType_Application
} NotificationEventSinkType;


//--------------------------------------------------------------------------
// NotificationEventManager
/// @brief	BDROM Internal Event Handler. All events within the BDROM Navigator are channeled to this module which then dispatches them to the whoeover needs them
///
class NotificationEventManager
{
public:
	NotificationEventManager();
	virtual ~NotificationEventManager();

	// set/remove event sinks. Notification event manager sends events to 1 or more sinks based on event type
	void SetEventSink(NotificationEventSinkType sinkType, NotificationEventSink * pSink);
	void SetEventSink(NotificationEventSinkType sinkType, BDROMAppEventSink * pAppSink);
	void RemoveEventSink(NotificationEventSinkType sinkType);

	// event dispatching
	SonicError SendEvent(BDROM_Event);

private:
	SonicCriticalSection 		m_csLock;

	NotificationEventSink *		m_pEventSinkModManager;
	NotificationEventSink *		m_pEventSinkPBCE;
	NotificationEventSink *		m_pEventSinkAACSAuth;
	NotificationEventSink *		m_pEventSinkUserEventManager;
	BDROMAppEventSink *			m_pEventSinkApplication;
};

#endif	//__NOTIFICATIONEVENTMANAGER_H__
