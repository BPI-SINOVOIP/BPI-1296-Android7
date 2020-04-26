#ifndef __OPENSSL_CIPHER_NORMAL_H__
#define __OPENSSL_CIPHER_NORMAL_H__

#include "mcp_cipher.h"

int OPENSSL_CipherInit_Normal(MCP_CIPHER_CTX* ctx, unsigned char type, unsigned char* key, unsigned char key_len, 
                         unsigned char* iv, unsigned char iv_len, int enc);

#endif // __OPENSSL_CIPHER_NORMAL_H__
