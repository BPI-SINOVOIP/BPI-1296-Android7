#ifndef __MCP_API_H__
#define __MCP_API_H__


#ifdef __cplusplus
extern "C" {
#endif

#define MARS_MCP_MODE(x)     (x & 0x1F)   

#define MCP_ALGO_DES         0x00
#define MCP_ALGO_3DES        0x01
#define MCP_ALGO_RC4         0x02
#define MCP_ALGO_MD5         0x03
#define MCP_ALGO_SHA_1       0x04
#define MCP_ALGO_AES         0x05
#define MCP_ALGO_AES_G       0x06
#define MCP_ALGO_AES_H       0x07
#define MCP_ALGO_CMAC        0x08

#define MCP_ALGO_AES_192         0x1d
#define MCP_ALGO_AES_256         0x15

#define MARS_MCP_BCM(x)     ((x & 0x3) << 6)  
#define MCP_BCM_ECB          0x0
#define MCP_BCM_CBC          0x1
#define MCP_BCM_CTR          0x2
#define MCP_RC4              0x3

#define MCP_SBH_LEAVE_CLEAR             0x0
#define MCP_SBH_CIPHER_BYTE_STEALING    0x1

#define MARS_MCP_ENC(x)     ((x & 0x1) << 5)      


#define DES_ECB_ENC     (MARS_MCP_MODE(MCP_ALGO_DES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(1))    
#define DES_ECB_DEC     (MARS_MCP_MODE(MCP_ALGO_DES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(0))    
#define DES_CBC_ENC     (MARS_MCP_MODE(MCP_ALGO_DES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(1))    
#define DES_CBC_DEC     (MARS_MCP_MODE(MCP_ALGO_DES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(0))    

#define TDES_ECB_ENC    (MARS_MCP_MODE(MCP_ALGO_3DES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(1))    
#define TDES_ECB_DEC    (MARS_MCP_MODE(MCP_ALGO_3DES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(0))    
#define TDES_CBC_ENC    (MARS_MCP_MODE(MCP_ALGO_3DES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(1))    
#define TDES_CBC_DEC    (MARS_MCP_MODE(MCP_ALGO_3DES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(0))    

#define AES_ECB_ENC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(1))    
#define AES_ECB_DEC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_ECB) | MARS_MCP_ENC(0))    
#define AES_CBC_ENC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(1))    
#define AES_CBC_DEC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_CBC) | MARS_MCP_ENC(0))    
#define AES_CTR_ENC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_CTR) | MARS_MCP_ENC(1))    
#define AES_CTR_DEC     (MARS_MCP_MODE(MCP_ALGO_AES) | MARS_MCP_BCM(MCP_BCM_CTR) | MARS_MCP_ENC(0))    

#define MARS_MCP_KEY_SEL(x) ((x & 0x3) << 12)  
#define MCP_KEY_SEL_CW      0x2   // Key form CW
#define MCP_KEY_SEL_OTP     0x1
#define MCP_KEY_SEL_DESC    0x0

#define MARS_MCP_IV_SEL(x)  ((x & 0x1) << 11)  
#define MCP_IV_SEL_REG      0x1
#define MCP_IV_SEL_DESC     0x0

#define GET_MCP_ALGO(x)      (x & 0x1F)    

#define MCP_CW_KEY(x)        (((x) & 0x7F)<<8)

#if defined(__LINARO_SDK__)
#define uint32_t unsigned long
#define uint64_t unsigned long long
#endif

typedef struct 
{
    uint32_t flags;
    uint32_t Key[6];
    uint32_t IV[4];
    uint32_t data_in;      // data in : physical address
    uint32_t data_out;     // data out : physical address
    uint32_t length;       // data length
}MCP_DESC;



typedef struct 
{
    union {
        MCP_DESC *pDesc;
	uint64_t padding_pDesc;
    };
    uint32_t nDesc;
    int32_t reserved;
}MCP_DESC_SET;

#define MCP_IOCTL_DO_COMMAND    0x70000001

extern void mars_mcp_des_enc_test();

int  MCP_Init();
void MCP_UnInit();
int  MCP_DoCommand(MCP_DESC* pDesc, int nDesc);

extern unsigned char NULL_IV[32];

#define MCP_BUFFER_SIZE     (512 * 1024)

/********************************************************************************
 DES 
 ********************************************************************************/
int MCP_DES_DESC_INIT(MCP_DESC* pDesc, unsigned char EnDe, unsigned char Mode, unsigned char Key[7], unsigned char IV[8]);
int MCP_DES_Decryption(unsigned char Mode, unsigned char Key[7], unsigned char IV[8], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_DES_Encryption(unsigned char Mode, unsigned char Key[7], unsigned char IV[8], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
#define MCP_DES_ECB_Decryption(Key, DataIn, DataOut, Len)       MCP_DES_Decryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_DES_ECB_Encryption(Key, DataIn, DataOut, Len)       MCP_DES_Encryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_DES_CBC_Decryption(Key, IV, DataIn, DataOut, Len)   MCP_DES_Decryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)
#define MCP_DES_CBC_Encryption(Key, IV, DataIn, DataOut, Len)   MCP_DES_Encryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)

/********************************************************************************
 TDES 
 ********************************************************************************/
int MCP_TDES_DESC_INIT(MCP_DESC* pDesc, unsigned char EnDe, unsigned char Mode, unsigned char Key[21], unsigned char IV[8]);
int MCP_TDES_Decryption(unsigned char Mode, unsigned char Key[21], unsigned char IV[8], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_TDES_Encryption(unsigned char Mode, unsigned char Key[21], unsigned char IV[8], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
#define MCP_TDES_ECB_Decryption(Key, DataIn, DataOut, Len)      MCP_DES_Decryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_TDES_ECB_Encryption(Key, DataIn, DataOut, Len)      MCP_DES_Encryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_TDES_CBC_Decryption(Key, IV, DataIn, DataOut, Len)  MCP_DES_Decryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)
#define MCP_TDES_CBC_Encryption(Key, IV, DataIn, DataOut, Len)  MCP_DES_Encryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)


/********************************************************************************
 AES 
 ********************************************************************************/ 
int MCP_AES_DESC_INIT(MCP_DESC* pDesc, unsigned char EnDe, unsigned char Mode, unsigned char Key[16], unsigned char IV[16]);

// Used for Data in Physical Address
int MCP_RC4_Decryption(unsigned char Key[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_RC4_Encryption(unsigned char Key[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_AES_Decryption(unsigned char Mode, unsigned char Key[16], unsigned char IV[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_AES_Encryption(unsigned char Mode, unsigned char Key[16], unsigned char IV[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len);
int MCP_AES_256_Decryption(unsigned char Mode, unsigned char Key[16], unsigned char IV[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len, unsigned char AesMode);
int MCP_AES_256_Encryption(unsigned char Mode, unsigned char Key[16], unsigned char IV[16], unsigned long DataIn, unsigned long DataOut, unsigned long Len, unsigned char AesMode);
#define MCP_AES_ECB_Decryption(Key, DataIn, DataOut, Len)       MCP_AES_Decryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_AES_ECB_Encryption(Key, DataIn, DataOut, Len)       MCP_AES_Encryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len)
#define MCP_AES_192_ECB_Decryption(Key, DataIn, DataOut, Len)       MCP_AES_256_Decryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len, MCP_ALGO_AES_192)
#define MCP_AES_192_ECB_Encryption(Key, DataIn, DataOut, Len)       MCP_AES_256_Encryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len, MCP_ALGO_AES_192)
#define MCP_AES_256_ECB_Decryption(Key, DataIn, DataOut, Len)       MCP_AES_256_Decryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len, MCP_ALGO_AES_256)
#define MCP_AES_256_ECB_Encryption(Key, DataIn, DataOut, Len)       MCP_AES_256_Encryption(MCP_BCM_ECB, Key, NULL, DataIn, DataOut, Len, MCP_ALGO_AES_256)
#define MCP_AES_CBC_Decryption(Key, IV, DataIn, DataOut, Len)   MCP_AES_Decryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)
#define MCP_AES_CBC_Encryption(Key, IV, DataIn, DataOut, Len)   MCP_AES_Encryption(MCP_BCM_CBC, Key, IV, DataIn, DataOut, Len)
#define MCP_AES_CTR_Decryption(Key, IV, DataIn, DataOut, Len)   MCP_AES_Decryption(MCP_BCM_CTR, Key, IV, DataIn, DataOut, Len)
#define MCP_AES_CTR_Encryption(Key, IV, DataIn, DataOut, Len)   MCP_AES_Encryption(MCP_BCM_CTR, Key, IV, DataIn, DataOut, Len)

// Used for Data in Virtual Address
int MCP_RC4_DataDecryption(unsigned char Key[16], unsigned char* pDataIn, unsigned char* pDataOut, unsigned long Len);
int MCP_RC4_DataEncryption(unsigned char Key[16], unsigned char* pDataIn, unsigned char* pDataOut, unsigned long Len);
int MCP_AES_DataEncryption(unsigned char EnDe, unsigned char Mode, unsigned char Key[16], unsigned char IV[16], unsigned char* pDataIn, unsigned char* pDataOut, unsigned long Len);
#define MCP_AES_ECB_DataDecryption(Key, pDataIn, pDataOut, Len)       MCP_AES_DataEncryption(0, MCP_BCM_ECB, Key, NULL, pDataIn, pDataOut, Len)
#define MCP_AES_ECB_DataEncryption(Key, pDataIn, pDataOut, Len)       MCP_AES_DataEncryption(1, MCP_BCM_ECB, Key, NULL, pDataIn, pDataOut, Len)
#define MCP_AES_CBC_DataDecryption(Key, IV, pDataIn, pDataOut, Len)   MCP_AES_DataEncryption(0, MCP_BCM_CBC, Key, IV, pDataIn, pDataOut, Len)
#define MCP_AES_CBC_DataEncryption(Key, IV, pDataIn, pDataOut, Len)   MCP_AES_DataEncryption(1, MCP_BCM_CBC, Key, IV, pDataIn, pDataOut, Len)
#define MCP_AES_CTR_DataDecryption(Key, IV, pDataIn, pDataOut, Len)   MCP_AES_DataEncryption(0, MCP_BCM_CTR, Key, IV, pDataIn, pDataOut, Len)
#define MCP_AES_CTR_DataEncryption(Key, IV, pDataIn, pDataOut, Len)   MCP_AES_DataEncryption(1, MCP_BCM_CTR, Key, IV, pDataIn, pDataOut, Len)

/********************************************************************************
 AES_H Hashing
 ********************************************************************************/
#define AES_H_FILE_HASHING_BUFFER_SIZE        (512 * 1024)
#define FILE_START  0
#define FILE_END    0xFFFFFFFF

int MCP_AES_H_HASH_INIT(MCP_DESC* pDesc);
int MCP_AES_H_IV_UPDATE(MCP_DESC* pDesc, unsigned char Hash[16]);
int MCP_AES_H_Hashing(MCP_DESC* pDesc, unsigned long DataIn, unsigned long Len, unsigned long DataOut);
int MCP_AES_H_DataHashEx(unsigned char* pData, unsigned long DataLen, unsigned char Hash[16], unsigned long BlockSize, unsigned char IsFirstBlock);
int MCP_AES_H_FileHash(const char* FName, unsigned char Hash[16], unsigned long BlockSize, unsigned long Start, unsigned long End);
#define MCP_AES_H_DataHash(args...)     MCP_AES_H_DataHashEx(args, 1)


#ifdef __cplusplus
}
#endif

#endif  // __MCP_API_H__
