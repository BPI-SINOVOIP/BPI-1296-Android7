#ifndef __MCP_CIPHER_CTS_ECB_H__
#define __MCP_CIPHER_CTS_ECB_H__

#include "mcp_cipher.h"

int MCP_CipherInit_CTS_ECB(MCP_CIPHER_CTX* ctx, unsigned char type, unsigned char* key, unsigned char key_len, int enc);

#endif // __MCP_CIPHER_CTS_ECB_H__
