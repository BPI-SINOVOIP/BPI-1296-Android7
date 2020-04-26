#ifndef _DECRYPT_H_
#define _DECRYPT_H_
#ifdef __cplusplus
extern "C" {
#endif
bool InitPrivKey();
bool decPrivPKeyData(unsigned char *in, unsigned char *out, unsigned long len);
bool encPrivPKeyData(unsigned char *in, unsigned char *out, unsigned long len);
#ifdef __cplusplus
}
#endif
#endif
