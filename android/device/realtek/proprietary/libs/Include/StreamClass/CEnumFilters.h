#ifndef __C_ENUM_FILTERS_H__
#define __C_ENUM_FILTERS_H__

#ifndef __I_STREAM_H__
#    include "IStream.h"
#endif

#include "CObject.h"

class CFlowManager;

class CEnumFilters: public IEnumFilters, public CObject{
friend class                    CFlowManager;
public:
                                
    HRESULT                     Next(
                                    IBaseFilter**           ppIFilter
                                );
    HRESULT                     Reset(void);

                                ~CEnumFilters(void) {}
protected:
                                CEnumFilters(
                                    CFlowManager*           pFilter
				);
protected:
    CFlowManager*               m_pFlowManager;
    int                         m_nCurrent;
    int                         m_nFilters;
};

#endif
