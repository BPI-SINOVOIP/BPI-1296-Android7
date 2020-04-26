#ifndef SILK_ENC_OUTPUT_PIN_H
#define SILK_ENC_OUTPUT_PIN_H

#include <Filters/CDataOutput.h>

class SilkEncOutputPin: public CBaseOutputPin
{
public:
    SilkEncOutputPin();
    ~SilkEncOutputPin();
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
};

#endif	// SILK_ENC_OUTPUT_PIN_H


