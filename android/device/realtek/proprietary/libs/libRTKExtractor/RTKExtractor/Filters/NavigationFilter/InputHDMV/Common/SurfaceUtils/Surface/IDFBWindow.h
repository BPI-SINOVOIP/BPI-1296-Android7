#ifndef IDFBWINDOW_H
#define IDFBWINDOW_H

#include "IDFBBase.h"
#include "IDFBSurface.h"
#include "IDFBEventBuffer.h"
#include "CommonTypes.h"

class IDFBWindow :public IDFBBase<IDirectFBWindow>
{
public:
	IDFBWindow(IDirectFBWindow *dfbif = 0) : IDFBBase <IDirectFBWindow> (dfbif) {}

	DFBResult	GetID(DFBWindowID &windowID);
	DFBResult	GetPosition(int &x, int &y);
	DFBResult	GetSize(int &width, int &height);
	DFBResult	CreateEventBuffer(IDFBEventBuffer *&eventBuffer);
	DFBResult	AttachEventBuffer(IDFBEventBuffer &buffer);
	DFBResult	EnableEvents(DFBWindowEventType mask);
	DFBResult	DisableEvents(DFBWindowEventType mask);
	DFBResult	GetSurface(IDFBSurface *&surface);
	DFBResult	SetOptions(DFBWindowOptions options);
	DFBResult	GetOptions(DFBWindowOptions &options);
	DFBResult	SetColorKey(UInt8 r, UInt8 g, UInt8 b);
	DFBResult	SetColorKeyIndex(unsigned int index);
	DFBResult	SetOpacity(UInt8 opacity);
	DFBResult	SetOpaqueRegion(int x1, int y1, int x2, int y2);
	DFBResult	GetOpacity(UInt8 &opacity);
	DFBResult	SetCursorShape(IDFBSurface &shape, int hotX, int hotY);
	DFBResult	RequestFocus();
	DFBResult	GrabKeyboard();
	DFBResult	UngrabKeyboard();
	DFBResult	GrabPointer();
	DFBResult	UngrabPointer();
	DFBResult	GrabKey(DFBInputDeviceKeySymbol symbol, DFBInputDeviceModifierMask modifiers);
	DFBResult	UngrabKey(DFBInputDeviceKeySymbol symbol, DFBInputDeviceModifierMask modifiers);
	DFBResult	Move(int dx, int dy);
	DFBResult	MoveTo(int x, int y);
	DFBResult	Resize(unsigned int width, unsigned int height);
	DFBResult	SetStackingClass(DFBWindowStackingClass stackingClass);
	DFBResult	Raise();
	DFBResult	Lower();
	DFBResult	RaiseToTop();
	DFBResult	LowerToBottom();
	DFBResult	PutAtop(IDFBWindow &lower);
	DFBResult	PutBelow(IDFBWindow &upper);
	DFBResult	Close();
	DFBResult	Destroy();
};

#endif	//IDFBWINDOW_H
