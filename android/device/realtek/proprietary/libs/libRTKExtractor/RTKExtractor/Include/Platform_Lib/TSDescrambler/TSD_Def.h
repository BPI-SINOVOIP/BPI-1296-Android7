#ifndef  __TSD_DEF_H__
#define  __TSD_DEF_H__

typedef enum 
{    
    TSD_FUNC_OK = 0,        
    TSD_FUNC_FAIL = 0x80000000,            
}TSD_FUNC_RESULT;

typedef enum 
{
    // DES    
    DES_ECB,
    DES_CBC,
    DES_MARS_CBC,          
    
    TDES_ECB,
    TDES_CBC,
    TDES_MARS_CBC,
    
    // AES
    AES_ECB,
    AES_CBC,
    
    HDCP2_AES_CTR,
    TSD_DESCRAMBLE_ALGO_MAX,
}DESCRAMBLE_ALGO;


#define TSD_CAP_BIT(x)          (0x1UL <<x)


typedef enum 
{    
    NO_DESCRAMBLE,        
    TSP_LEVEL_DESCRAMBLE,
    PES_LEVEL_DESCRAMBLE,
    TSD_DESCRAMBLE_MODE_MAX,
}DESCRAMBLE_MODE;

#define MAX_KEY_LENGTH          32
#define MAX_IV_LENGTH           32

typedef enum 
{        
    DESCRAMBLE_KEY_SRC_MEM,        
    DESCRAMBLE_KEY_SRC_CW,    
    DESCRAMBLE_KEY_SRC_OTP,
    DESCRAMBLE_KEY_SRC_NONE = 0xFF,             
}DESCRAMBLE_KEY_SRC;

typedef enum 
{       
    DESCRAMBLE_IV_SRC_MEM,        
    DESCRAMBLE_IV_SRC_CW,    
    DESCRAMBLE_IV_SRC_NONE = 0xFF,  
}DESCRAMBLE_IV_SRC;

typedef struct 
{        
    DESCRAMBLE_KEY_SRC  KeySrc;
    DESCRAMBLE_IV_SRC   IvSrc;        
    unsigned char       Val[MAX_KEY_LENGTH];        
    unsigned char       IV[MAX_IV_LENGTH];
}DESCRAMBLE_KEY;

#endif   //__TSD_DEF_H__
