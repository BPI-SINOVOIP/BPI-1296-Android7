#ifndef IDFBSCREEN_H
#define IDFBSCREEN_H

#include "IDFBBase.h"

class IDFBScreen : public IDFBBase<IDirectFBScreen>
{
public:
	IDFBScreen(IDirectFBScreen* dfbif = 0) : IDFBBase <IDirectFBScreen> (dfbif) {}

	DFBResult	GetID(DFBScreenID &screenID);
	DFBResult	GetDescription(DFBScreenDescription &descr);
	DFBResult	EnumDisplayLayers(DFBDisplayLayerCallback callback, void *callbackData);
	DFBResult	SetPowerMode(DFBScreenPowerMode mode);
	DFBResult	WaitForSync();
	DFBResult	GetMixerDescriptions(DFBScreenMixerDescription &desc);
	DFBResult	GetMixerConfiguration(int mixer, DFBScreenMixerConfig &config);
	DFBResult	TestMixerConfiguration(int mixer, const DFBScreenMixerConfig &config, DFBScreenMixerConfigFlags &flags);
	DFBResult	SetMixerConfiguration(int mixer, const DFBScreenMixerConfig &config);
	DFBResult	GetEncoderDescriptions(DFBScreenEncoderDescription &descs);
	DFBResult	GetEncoderConfiguration(int encoder, DFBScreenEncoderConfig &config);
	DFBResult	TestEncoderConfiguration(int encoder, const DFBScreenEncoderConfig &config, DFBScreenEncoderConfigFlags &flags);
	DFBResult	SetEncoderConfiguration(int encoder, const DFBScreenEncoderConfig &config);
	DFBResult	GetOutputDescriptions(DFBScreenOutputDescription &descs);
	DFBResult	GetOutputConfiguration(int output, DFBScreenOutputConfig &config);
	DFBResult	TestOutputConfiguration(int output, const DFBScreenOutputConfig &config, DFBScreenOutputConfigFlags &flags);
	DFBResult	SetOutputConfiguration(int output, const DFBScreenOutputConfig &config);
};

#endif
