#ifndef __HWM_API_H__
#define __HWM_API_H__

//#ifdef __cpluscplus
//extern "C" {
//#endif

int otp_read(unsigned char* buf, int length, int start_addr);
int otp_write(unsigned char* buf, int length, int start_addr);

int Binder_CwKey_ECB_Cipher(unsigned char* pDataIn, unsigned char EnDe, int CwIn, unsigned char* pDataOut, unsigned long DataLen);

int Binder_AES_ECB_128_Cipher(    unsigned char           EnDe,
    unsigned char           Key[16],     
    unsigned char*          pDataIn, 
    unsigned char*          pDataOut, 
    unsigned long           DataLen);

int dpaWrite(char *cmd, unsigned long param1, unsigned long param2, unsigned long param3);

//#ifdef __cpluscplus
//}
//#endif

#endif
