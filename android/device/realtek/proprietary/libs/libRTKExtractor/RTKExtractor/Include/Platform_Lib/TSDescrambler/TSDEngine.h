#ifndef  __TSD_ENGINE_H__
#define  __TSD_ENGINE_H__

#include "TSD_Def.h"
#include "TSParser.h"


#define MAX_CIPHER_COUNT        2
#define MAX_TARGET_COUNT        16

typedef enum
{
    TSD_ENGINE_GENERIC,
    TSD_ENGINE_HDCP2,
    TSD_ENGINE_MAX,
}TSD_ENGINE_ID;

typedef enum
{
    TSD_TARGET_AUTO,
    TSD_TARGET_SELECT_BY_PID,     
}TSD_TARGET_SELECT;



typedef struct 
{
    TSD_TARGET_SELECT   mode;     
    unsigned char       n_target;       // number if targets
        
    union 
    {
        struct 
        {
            unsigned short pid;         // target pid
            unsigned char  key_set;     // which keyset to use
        }pid_targets[MAX_TARGET_COUNT];
    };
}TSD_TARGET;



class TSDEngine : public TSParser
{
public:

    TSDEngine() 
    {
        m_Capability = 0;
        strncpy(m_Name, "Base TSD", sizeof(m_Name));
    }
    
    virtual ~TSDEngine() {}
    
protected:
    
    char            m_Name[32];
    unsigned long   m_Capability;    
        
public:
    
    // System Information
    virtual const char* GetName()
    {
        return m_Name;
    }
    
    virtual int TestAlgorithm(DESCRAMBLE_ALGO Algorithm)
    {
        return (m_Capability & TSD_CAP_BIT(Algorithm)) ? TSD_FUNC_OK : TSD_FUNC_FAIL;
    }        

public:
            
    virtual int SetTarget(TSD_TARGET* pTarget) = 0;   
    
    virtual int SetDescrambleAlgorithm(DESCRAMBLE_ALGO Algorithm, DESCRAMBLE_MODE Mode) = 0;    

    virtual int SetCW(unsigned char KeySet, unsigned char KeySel, DESCRAMBLE_KEY* pKey) = 0;    
    
    virtual int DescrambleTS(unsigned char* pData, unsigned long Len, unsigned long UserKernelOffset) = 0;    
};

TSDEngine* CreateTSDEngine(TSD_ENGINE_ID id);
void ReleaseTSDEngine(TSDEngine* pTSD);

#endif  //  __TSD_ENGINE_H__
