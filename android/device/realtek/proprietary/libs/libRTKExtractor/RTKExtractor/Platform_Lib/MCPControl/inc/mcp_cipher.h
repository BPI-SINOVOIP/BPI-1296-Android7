#ifndef __MCP_CIPHER_H__
#define __MCP_CIPHER_H__

#include "mcp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////
// IOCTL
/////////////////////////////////////////////////////////////////////

#define MCP_CIPHER_DEV_FILE             "/dev/mcp/cipher"
#define MCP_CIPHER_DEV_FILE2            "/dev/mcp_cipher"

#define MCP_CIPHER_IOCTL_CMD(x)         (0x12839000 + x)
#define MCP_CIPHER_IOCTL_INIT           MCP_CIPHER_IOCTL_CMD(0)
#define MCP_CIPHER_IOCTL_UPDATE         MCP_CIPHER_IOCTL_CMD(1)
#define MCP_CIPHER_IOCTL_FINAL          MCP_CIPHER_IOCTL_CMD(2)

#define MCP_CIPHER_DES_ECB              0x00
#define MCP_CIPHER_DES_CBC              0x01
#define MCP_CIPHER_TDES_ECB             0x10
#define MCP_CIPHER_TDES_CBC             0x11
#define MCP_CIPHER_AES_128_ECB          0x20
#define MCP_CIPHER_AES_128_CBC          0x21
#define MCP_CIPHER_AES_128_CTR          0x22

#ifdef MCP_OPENSSL_SUPPORT
#define MCP_CIPHER_USE_OPENSSL          0x80        // use Openssl if specified
#endif

typedef struct{
    unsigned char   type;                   
    unsigned char   enc;   
    unsigned char   key_len;
    unsigned char*  key;
    unsigned char   iv_len;
    unsigned char*  iv;
}MCP_CIPHER_PARAM;


typedef struct{
    unsigned char*  in;
    unsigned long   len;
    unsigned char*  out;    
}MCP_CIPHER_DATA;


/////////////////////////////////////////////////////////////////////
// APIs
/////////////////////////////////////////////////////////////////////

typedef struct 
{
    void*     cipher_data;

    struct 
    {
        int (*update)  (void* cipher, unsigned char* in, unsigned long len, unsigned char* out);
        int (*final)   (void* cipher, unsigned char* out);
        int (*cleanup) (void* cipher);
    }ops;

}MCP_CIPHER_CTX;

typedef enum {
    MCP_SBH_KEEP_CLEAR,
    MCP_SBH_CTS,
    MCP_SBH_WIDEVINE_CTS,
}MCP_SBH;

void MCP_CIPHER_CTX_init    (MCP_CIPHER_CTX *ctx); 
int  MCP_CIPHER_CTX_cleanup (MCP_CIPHER_CTX *ctx);
int  MCP_CipherInit         (MCP_CIPHER_CTX *ctx, unsigned char type, unsigned char* key, unsigned char key_len, unsigned char* iv, unsigned char iv_len, unsigned char enc);
int  MCP_CipherInitEx       (MCP_CIPHER_CTX *ctx, unsigned char type, unsigned char* key, unsigned char key_len, unsigned char* iv, unsigned char iv_len, unsigned char enc, unsigned char sbh);
int  MCP_CipherUpdate       (MCP_CIPHER_CTX *ctx, unsigned char* in, unsigned long len, unsigned char* out);
int  MCP_CipherFinal        (MCP_CIPHER_CTX *ctx, unsigned char* out);

#define MCP_PRINT(fmt, args...)                 do { printf(fmt, ## args); fflush(stdout); }while(0)
#define MCP_CIPHER_WARNING(fmt, args...)        MCP_PRINT("[MCP][CIPH] WARNING," fmt, ## args)
#define MCP_CIPHER_DBG(fmt, args...)            MCP_PRINT("[MCP][CIPH] DBG," fmt, ## args)


#ifdef __cplusplus
}
#endif

#endif // __MCP_CIPHER_H__
