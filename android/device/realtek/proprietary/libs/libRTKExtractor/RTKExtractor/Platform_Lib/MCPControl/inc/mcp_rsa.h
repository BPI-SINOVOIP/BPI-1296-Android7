#ifndef __MCP_RSA_H__
#define __MCP_RSA_H__

#include "mcp_bi.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////
// IOCTL
/////////////////////////////////////////////////////////////////////

#define MCP_RSA_DEV_FILE                "/dev/mcp/rsa"
#define MCP_RSA_DEV_FILE2               "/dev/mcp_rsa"

#define MCP_RSA_IOCTL_CMD(x)            (0x10 + x)
#define MCP_RSA_IOCTL_VERIFY            MCP_RSA_IOCTL_CMD(0)

// RSA
typedef struct {
    BI* signature;
    BI* pub_exp;
    BI* mod;
    BI* dgst;
}RSA_VERIFY;

/////////////////////////////////////////////////////////////////////
// APIs
/////////////////////////////////////////////////////////////////////

int MCP_RSAVerify(BI *signature, BI* pub_exp, BI* mod, BI *dgst);

#ifdef __cplusplus
}
#endif

#endif // __MCP_RSA_H__
