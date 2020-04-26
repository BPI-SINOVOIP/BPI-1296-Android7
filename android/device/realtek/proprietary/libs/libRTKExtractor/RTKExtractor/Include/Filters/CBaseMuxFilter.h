/********************************************************************************/
/*    File: CBaseMuxFilter.h                                                     */
/*    Desc: Header file for Base Mux Filter Class interface define               */
/*    Author: Yuantao Wu     04/25/2007                                         */
/*            Yi-Chian Yang  04/25/2007                                         */
/*    Copyright (c)RealTek.   All rights reserved.                              */
/********************************************************************************/

#ifndef __CBASE_MUXER_FILTER_H__
#define __CBASE_MUXER_FILTER_H__

#include <OSAL.h>
#include <CStream.h>

class CBaseMuxFilter : public CBaseFilter
{
public:
    CBaseMuxFilter():CBaseFilter(){};
    ~CBaseMuxFilter(){};

    virtual void       ProcessData() = 0;
    virtual int        InputDataEmpty()=0;
    virtual HRESULT    CheckOutputAvailable(int bufferNum) = 0;
};

#endif // __CBASE_MUXER_FILTER_H__

                    
