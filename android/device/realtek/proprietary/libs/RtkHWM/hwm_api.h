#ifndef __HWM_API_H__
#define __HWM_API_H__

//#ifdef __cpluscplus
//extern "C" {
//#endif

#define OTP_ERR_OPEN_DEV -1
#define OTP_ERR_MODULE -2
#define OTP_ERR_START_ADDR -3
#define OTP_ERR_LEN -4
#define OTP_ERR_WRITE_CHECK -5

int otp_read(unsigned char* buf, int length, int start_addr);
int otp_write(unsigned char* buf, int length, int start_addr);


//#ifdef __cpluscplus
//}
//#endif

#endif
