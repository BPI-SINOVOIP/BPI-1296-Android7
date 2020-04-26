#ifndef __MCP_UTIL_H__
#define __MCP_UTIL_H__

#include "mcp_dgst.h"
#include "mcp_cipher.h"
#include "mcp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

////////// File Hash ///////////
int MCP_FileHashWithRegion(unsigned char algo, const char* fname, unsigned char hash[MCP_MAX_DIGEST_SIZE], unsigned long block_size, unsigned long start, unsigned long end);    
#define MCP_FileHash(algo, fname, hash, bsize)   MCP_FileHashWithRegion(algo, fname, hash, bsize, FILE_START, FILE_END)     


////////// File Cipher ///////////
int MCP_FileCipher(unsigned char algo, unsigned char enc, unsigned char* key, unsigned char key_len,unsigned char* iv, unsigned char iv_len, unsigned char sbh, const char* sfile, const char* dfile);
#define MCP_FileEncryption(algo, key, key_len, iv, iv_len, sbh, sfile, dfile)  MCP_FileCipher(algo, 1, key, key_len, iv, iv_len, sbh, sfile, dfile)
#define MCP_FileDecryption(algo, key, key_len, iv, iv_len, sbh, sfile, dfile)  MCP_FileCipher(algo, 0, key, key_len, iv, iv_len, sbh, sfile, dfile)

// DES
#define MCP_DES_ECB_FileEncryptionEx(key, key_len, sbh, sfile, dfile)                           MCP_FileEncryption(MCP_CIPHER_DES_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_DES_ECB_FileDecryptionEx(key, key_len, sbh, sfile, dfile)                           MCP_FileDecryption(MCP_CIPHER_DES_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_DES_CBC_FileEncryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)               MCP_FileEncryption(MCP_CIPHER_DES_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
#define MCP_DES_CBC_FileDecryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)               MCP_FileDecryption(MCP_CIPHER_DES_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
                                                                                                
#define MCP_DES_ECB_FileEncryption(key, key_len, sfile, dfile)                                  MCP_FileEncryption(MCP_CIPHER_DES_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_DES_ECB_FileDecryption(key, key_len, sfile, dfile)                                  MCP_FileDecryption(MCP_CIPHER_DES_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_DES_CBC_FileEncryption(key, key_len, iv, iv_len, sfile, dfile)                      MCP_FileEncryption(MCP_CIPHER_DES_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_DES_CBC_FileDecryption(key, key_len, iv, iv_len, sfile, dfile)                      MCP_FileDecryption(MCP_CIPHER_DES_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)
                                                                                                
// TDES                                                                                         
#define MCP_TDES_ECB_FileEncryptionEx(key, key_len, sbh, sfile, dfile)                          MCP_FileEncryption(MCP_CIPHER_TDES_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_TDES_ECB_FileDecryptionEx(key, key_len, sbh, sfile, dfile)                          MCP_FileDecryption(MCP_CIPHER_TDES_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_TDES_CBC_FileEncryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)              MCP_FileEncryption(MCP_CIPHER_TDES_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
#define MCP_TDES_CBC_FileDecryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)              MCP_FileDecryption(MCP_CIPHER_TDES_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
                                                                                                
#define MCP_TDES_ECB_FileEncryption(key, key_len, sfile, dfile)                                 MCP_FileEncryption(MCP_CIPHER_TDES_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_TDES_ECB_FileDecryption(key, key_len, sfile, dfile)                                 MCP_FileDecryption(MCP_CIPHER_TDES_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_TDES_CBC_FileEncryption(key, key_len, iv, iv_len, sfile, dfile)                     MCP_FileEncryption(MCP_CIPHER_TDES_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_TDES_CBC_FileDecryption(key, key_len, iv, iv_len, sfile, dfile)                     MCP_FileDecryption(MCP_CIPHER_TDES_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)
                                                                                                
// AES                                                                                          
#define MCP_AES_128_ECB_FileEncryptionEx(key, key_len, sbh, sfile, dfile)                       MCP_FileEncryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_AES_128_ECB_FileDecryptionEx(key, key_len, sbh, sfile, dfile)                       MCP_FileDecryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL, 0, sbh, sfile, dfile)
#define MCP_AES_128_CBC_FileEncryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)           MCP_FileEncryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
#define MCP_AES_128_CBC_FileDecryptionEx(key, key_len, iv, iv_len, sbh, sfile, dfile)           MCP_FileDecryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, sbh, sfile, dfile)
                                                                                                
#define MCP_AES_128_ECB_FileEncryption(key, key_len, sfile, dfile)                              MCP_FileEncryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_AES_128_ECB_FileDecryption(key, key_len, sfile, dfile)                              MCP_FileDecryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL, 0, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_AES_128_CBC_FileEncryption(key, key_len, iv, iv_len, sfile, dfile)                  MCP_FileEncryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)
#define MCP_AES_128_CBC_FileDecryption(key, key_len, iv, iv_len, sfile, dfile)                  MCP_FileDecryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, sfile, dfile)

////////// Data Cipher ///////////
int MCP_DataCipher(unsigned char algo, unsigned char enc, unsigned char* key, unsigned char key_len, unsigned char* iv, unsigned char iv_len, unsigned char sbh, unsigned char* data_in, unsigned char* data_out, unsigned long len);
#define MCP_DataEncryption(algo, key, key_len, iv, iv_len, sbh, data_in, data_out, len)         MCP_DataCipher(algo, 1, key, key_len, iv, iv_len, sbh, data_in, data_out, len)
#define MCP_DataDecryption(algo, key, key_len, iv, iv_len, sbh, data_in, data_out, len)         MCP_DataCipher(algo, 0, key, key_len, iv, iv_len, sbh, data_in, data_out, len)

#define MCP_AES_128_ECB_DataEncryptionEx(key, key_len, sbh, data_in, data_out, len)             MCP_DataEncryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL,    0, sbh, data_in, data_out, len)
#define MCP_AES_128_ECB_DataDecryptionEx(key, key_len, sbh, data_in, data_out, len)             MCP_DataDecryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL,    0, sbh, data_in, data_out, len)
#define MCP_AES_128_CBC_DataEncryptionEx(key, key_len, iv, iv_len, sbh, data_in, data_out, len) MCP_DataEncryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, sbh, data_in, data_out, len)
#define MCP_AES_128_CBC_DataDecryptionEx(key, key_len, iv, iv_len, sbh, data_in, data_out, len) MCP_DataDecryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, sbh, data_in, data_out, len)     
#define MCP_AES_128_CTR_DataEncryptionEx(key, key_len, iv, iv_len, sbh, data_in, data_out, len) MCP_DataEncryption(MCP_CIPHER_AES_128_CTR, key, key_len, iv, iv_len, sbh, data_in, data_out, len)
#define MCP_AES_128_CTR_DataDecryptionEx(key, key_len, iv, iv_len, sbh, data_in, data_out, len) MCP_DataDecryption(MCP_CIPHER_AES_128_CTR, key, key_len, iv, iv_len, sbh, data_in, data_out, len)     

#define MCP_AES_128_ECB_DataEncryption(key, key_len, data_in, data_out, len)                    MCP_DataEncryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL,    0, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)
#define MCP_AES_128_ECB_DataDecryption(key, key_len, data_in, data_out, len)                    MCP_DataDecryption(MCP_CIPHER_AES_128_ECB, key, key_len, NULL,    0, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)
#define MCP_AES_128_CBC_DataEncryption(key, key_len, iv, iv_len, data_in, data_out, len)        MCP_DataEncryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)
#define MCP_AES_128_CBC_DataDecryption(key, key_len, iv, iv_len, data_in, data_out, len)        MCP_DataDecryption(MCP_CIPHER_AES_128_CBC, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)
#define MCP_AES_128_CTR_DataEncryption(key, key_len, iv, iv_len, data_in, data_out, len)        MCP_DataEncryption(MCP_CIPHER_AES_128_CTR, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)
#define MCP_AES_128_CTR_DataDecryption(key, key_len, iv, iv_len, data_in, data_out, len)        MCP_DataDecryption(MCP_CIPHER_AES_128_CTR, key, key_len, iv, iv_len, MCP_SBH_KEEP_CLEAR, data_in, data_out, len)


      
#ifdef __cplusplus
}
#endif
        
#endif  // __MCP_UTIL_H__
