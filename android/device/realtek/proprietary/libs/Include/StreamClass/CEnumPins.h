#ifndef __C_ENUM_PINS_H__
#define __C_ENUM_PINS_H__

#ifndef __I_STREAM_H__
#    include "IStream.h"
#endif

#include "CObject.h"

class   CBaseFilter;

class CEnumPins: public IEnumPins, public CObject{
friend class                    CBaseFilter;
public:
                                
    HRESULT                     Next(
                                    IPin**                  ppIPin
                                );
    HRESULT                     Reset(void);

                                ~CEnumPins(void) {}
protected:
                                CEnumPins(
                                    CBaseFilter*            pFilter
				);
protected:
    CBaseFilter*                m_pFilter;
    int                         m_nCurrent;
    int                         m_nPins;
};

/*
class CEnumPinsFromInPin: public IEnumPins, public CObject{
friend class                    CBaseInputPin;
public:
                                
    HRESULT                     Next(
                                    IPin**                  ppIPin
                                );
    HRESULT                     Reset(void);

                                ~CEnumPinsFromInPin(void) {}
protected:
                                CEnumPins(
                                    CBaseInputPin*          pInPin
				);
protected:
    CBaseInputPin*              m_pInputPin;
    int                         m_nCurrent;
};

class CEnumPinsFromOutPin: public IEnumPins, public CObject{
friend class                    CBaseOutputPin;
public:
                                
    HRESULT                     Next(
                                    IPin**                  ppIPin
                                );
    HRESULT                     Reset(void);

                                ~CEnumPinsFromOutPin(void) {}
protected:
                                EnumPinsFromOutPin(
                                    CBaseOutputPin*         pOutPin
				);
protected:
    CBaseOutputPin*             m_pOutputPin;
    int                         m_nCurrent;
};
*/

#endif
