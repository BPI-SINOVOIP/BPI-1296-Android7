#ifndef SILK_DEC_OUTPUT_PIN_H
#define SILK_DEC_OUTPUT_PIN_H

#include <Filters/CDataOutput.h>

class SilkDecOutputPin: public CBaseOutputPin
{
public:
    SilkDecOutputPin();
    ~SilkDecOutputPin();
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
//    HRESULT EndOfStream(unsigned int eventID);
};

#endif	// SILK_DEC_OUTPUT_PIN_H

