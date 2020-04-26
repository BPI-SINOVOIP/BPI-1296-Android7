#ifndef __EFUSE_H__
#define __EFUSE_H__

typedef unsigned int UINT32; 
typedef unsigned char UINT8;


/***************************
 *Function Declaration
 ***************************/
UINT32 genTRNG( UINT32*val);
int GET_KF_IN_OTP(unsigned char * kf);
int start_program_KF(unsigned char * gen_kf);

#endif
