#ifndef IDFBINPUTDEVICE_H
#define IDFBINPUTDEVICE_H

#include "IDFBBase.h"
#include "IDFBEventBuffer.h"

class IDFBInputDevice : public IDFBBase<IDirectFBInputDevice>
{
public:
	IDFBInputDevice(IDirectFBInputDevice* dfbif = 0) : IDFBBase <IDirectFBInputDevice> (dfbif){}

	DFBResult	GetID(DFBInputDeviceID &deviceID);
	DFBResult	GetDescription(DFBInputDeviceDescription &desc);
	DFBResult	GetKeymapEntry(int code, DFBInputDeviceKeymapEntry &entry);
	DFBResult	CreateEventBuffer(IDFBEventBuffer *&eventBuffer);
	DFBResult	AttachEventBuffer(IDFBEventBuffer &eventBuffer);
	DFBResult	GetKeyState(DFBInputDeviceKeyIdentifier keyID, DFBInputDeviceKeyState &state);
	DFBResult	GetModifiers(DFBInputDeviceModifierMask &mask);
	DFBResult	GetLockState(DFBInputDeviceLockState &state);
	DFBResult	GetButtons(DFBInputDeviceButtonMask &mask);
	DFBResult	GetButtonState(DFBInputDeviceButtonIdentifier button, DFBInputDeviceButtonState &state);
	DFBResult	GetAxis(DFBInputDeviceAxisIdentifier axisID, int &axis);
	DFBResult	GetXY(int &x, int &y);
};

#endif
