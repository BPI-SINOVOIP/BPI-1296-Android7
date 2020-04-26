#ifndef __MCP_DGST_H__
#define __MCP_DGST_H__

#include "mcp_buff.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////
// IOCTL
/////////////////////////////////////////////////////////////////////////////

#define MCP_DGST_DEV_FILE           "/dev/mcp/dgst"
#define MCP_DGST_DEV_FILE2          "/dev/mcp_dgst"

//------- ioctl value --------
#define MCP_DGST_IOCTL_INIT           (0)
#define MCP_DGST_IOCTL_UPDATE         (1)
#define MCP_DGST_IOCTL_FINAL          (2)
#define MCP_DGST_IOCTL_PEEK           (3)


//------- md types --------
typedef enum {
    MCP_MD_AES_H,
    MCP_MD_MARS_AES_H,
    MCP_MD_SHA1,
    MCP_MD_MARS_SHA1
}MCP_MD_TYPES;



/////////////////////////////////////////////////////////////////////////////
// APIS
/////////////////////////////////////////////////////////////////////////////
#define MCP_BUFF            mcp_buff
#define MCP_MAX_DIGEST_SIZE 64

typedef struct 
{
    void*     digest;
    void*     md_data;
}MCP_MD_CTX;


void MCP_MD_CTX_init    (MCP_MD_CTX *ctx);
int  MCP_MD_CTX_cleanup (MCP_MD_CTX *ctx);
int  MCP_DigestInit     (MCP_MD_CTX* ctx, MCP_MD_TYPES type);
int  MCP_DigestUpdate   (MCP_MD_CTX* ctx, MCP_BUFF* mcpb);
int  MCP_DigestFinal    (MCP_MD_CTX* ctx, unsigned char* pHash,unsigned int* pHashLen);
int  MCP_DigestPeek     (MCP_MD_CTX* ctx, unsigned char* pHash,unsigned int* pHashLen);

#ifdef __cplusplus
}
#endif

#endif // __MCP_DGST_H__
