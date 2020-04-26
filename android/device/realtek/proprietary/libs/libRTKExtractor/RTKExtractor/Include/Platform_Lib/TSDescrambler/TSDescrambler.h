#ifndef  __TS_DESCRAMBLER_H__
#define  __TS_DESCRAMBLER_H__

#include "TSD_Def.h"
#include "TSDEngine.h"



typedef struct 
{        
    DESCRAMBLE_MODE         Mode;
    
    DESCRAMBLE_ALGO         Algorithm;                

    DESCRAMBLE_KEY          Key[2];    
    
}DESCRAMBLE_CONFIG;



class TS_DESCRAMBLER
{
public:

    TS_DESCRAMBLER();
    
    virtual ~TS_DESCRAMBLER();
    
protected:

    TSDEngine*          m_pTSDEngine;
    
public:
    
    virtual int SetAlgorithm(DESCRAMBLE_ALGO Algo, DESCRAMBLE_MODE Mode);           
    
    virtual int SetTarget(TSD_TARGET* pTarget);
    
    virtual int SetCW   (unsigned char           KeySet, 
                         unsigned char           KeySel, 
                         unsigned char           Val[MAX_KEY_LENGTH], 
                         unsigned char           IV[MAX_IV_LENGTH]);    
                         
    virtual int SetCW   (unsigned char           KeySet, 
                         unsigned char           KeySel, 
                         DESCRAMBLE_KEY*         pKey);                                                                                
                
    virtual int DescrambleTS(unsigned char* pData, unsigned long Len, unsigned long UserKernelOffset);
    
public:    
// old version API
    virtual int SetParam(DESCRAMBLE_CONFIG* pConfig); 
        
};



#endif   //__TS_DESCRAMBLER_H__
