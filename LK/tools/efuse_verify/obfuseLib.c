#include "mcp.h"
#include "dvrboot_inc/util.h"

extern unsigned char dec_array[32*2];
extern unsigned char v_array[1024];
extern unsigned char otp_retry_cnt;

unsigned int R0(unsigned char* pkey, unsigned int ofset, unsigned int len);
unsigned char *R1(unsigned char mode, unsigned char * input, unsigned int len);
#define TEST_MODE
#define RELEASE_MODE
//#define DEBUG_PRT
#define MAX_RETRY_CNT   5
#define OTP_BIT_SECUREBOOT              3494
//********************************************************************
//
// For Obfuscation APIs
//
//
//
//
//
//********************************************************************
int obfuse_getset(otp_method_enum_t type, unsigned char* enc_array, unsigned char* enc_array1, unsigned char* enc_array2, unsigned char* enc_array3, unsigned int len)
{
        int ret=1;
        unsigned char kk[512]={ \
				0x00,0x02,0x01,0x21,0x4a,0x2c,0x85,0x82,0x7a,0x21,0x34,0x3d,0x14,0x00,0x14,0x99,	\
				0x20,0xa2,0x02,0x22,0xb5,0x1b,0x21,0x1a,0x71,0x22,0xa0,0x2d,0x31,0x0a,0x34,0x29,	\
				0x52,0x82,0x03,0xa8,0xc5,0x8a,0x12,0xd8,0x7a,0x23,0x84,0x32,0x24,0x0b,0x12,0x92,	\
				0x80,0x01,0x0a,0x11,0xab,0x87,0x31,0x88,0x88,0x24,0x94,0x1d,0x92,0xc0,0x11,0x39,	\
				0x08,0x01,0x05,0xa8,0x85,0x67,0xb0,0xc8,0x77,0x52,0x3a,0xaa,0x14,0x0d,0x1a,0x91,	\
				0x11,0x02,0xc8,0x28,0x4a,0x63,0xa1,0xca,0x62,0x21,0x3b,0x9d,0x44,0x90,0x12,0x21,	\
				0xff,0xd0,0x00,0x81,0x41,0x53,0xa5,0x88,0x12,0x28,0xc4,0x1a,0x38,0x01,0x11,0x89,	\
				0xf2,0x72,0x04,0x81,0x5f,0x61,0x85,0x82,0x18,0x79,0x04,0x0d,0x84,0x11,0x3b,0x98,	\
				0x19,0x60,0x00,0x79,0xa2,0x62,0x25,0x1a,0x78,0x3a,0x00,0x31,0x94,0xa0,0x1f,0x09,	\
				0x28,0x88,0x8d,0x81,0x11,0x61,0x18,0x8a,0x72,0x21,0x14,0x1d,0x2a,0x0a,0x2d,0x08,        \
				0x18,0x01,0x00,0xa8,0x85,0x67,0xb0,0xc8,0x77,0x52,0x3a,0xaa,0x14,0x0d,0x1b,0x98,	\
				0x31,0x02,0xc8,0x28,0x4a,0x63,0xa1,0xca,0x62,0x21,0x3b,0x9d,0x44,0x90,0x52,0x31,	\
				0xef,0xd0,0x00,0x81,0x41,0x53,0xa5,0x88,0x12,0x28,0xc4,0x1a,0x38,0x01,0x32,0x49,	\
				0xfe,0x72,0x04,0x82,0x5f,0x61,0x85,0x82,0x18,0x79,0x04,0x0d,0x84,0x11,0x31,0x95,	\
				0x1b,0x66,0x0f,0x71,0x22,0x32,0x25,0x1a,0x78,0x3a,0x00,0x31,0x94,0xa0,0x1e,0x99,	\
				0x17,0x78,0x7d,0x61,0x51,0x41,0x18,0x8a,0x72,0x21,0x14,0x1d,0x2a,0x0a,0x2a,0x0a,	\
        };
	__align__ unsigned char r10[16] = {
                0x08, 0x20, 0x00, 0x30,
                0x0f, 0x50, 0x01, 0xc0,
                0x02, 0x60, 0x0d, 0x70,
                0x0b, 0xa0, 0x08, 0xd0
        };
	__align__ unsigned char r11[512];
	__align__ unsigned char r12[16];
	__align__ unsigned char r13[16]; //out1
	__align__ unsigned char r14[16]; //out2
	__align__ unsigned char r15[16]; //out3
	__align__ unsigned char r16[16]; //out4
        unsigned int i=0;
        volatile unsigned int key_rem = 0;
        volatile unsigned char* pkey=0x00;
	int ofset=0;

        key_rem = dec_array;
        key_rem &= 0xfffffff0;
        key_rem += 0x10;
        pkey = key_rem;

        r10[0] = kk[0x38];
        r10[1] = kk[0x29];
        r10[2] = kk[0x1a];
        r10[3] = kk[0x0b]+1;
        r10[4] = kk[0x74];
        r10[5] = kk[0x65];
        r10[6] = kk[0x56];
	r10[7] = kk[0x47]+1;
	r10[8] = kk[0x70];
	r10[9] = kk[0x81];
	r10[10] = kk[0x92];
	r10[11] = kk[0x83];
	r10[12] = kk[0x34];
	r10[13] = kk[0x43];
	r10[14] = kk[0x52];
	r10[15] = kk[0x61];

	memset(r11,0x00,512);

	switch(type)
	{ 
	  case ENUM_TYPE_AES_KEY:
		memset(r12,0x00,16);
        	r12[0] = kk[0x28];
        	r12[1] = kk[0x19];
        	r12[2] = kk[0x3a];
        	r12[3] = kk[0x5b]+1;
        	r12[4] = kk[0x64];
        	r12[5] = kk[0x67];
        	r12[6] = kk[0x58];
		r12[7] = kk[0x49]+1;
		r12[8] = kk[0x7a];
		r12[9] = kk[0xb1];
		r12[10] = kk[0xc2];
		r12[11] = kk[0xd3];
		r12[12] = kk[0xe4];
		r12[13] = kk[0xf3];
		r12[14] = kk[0x50];
		r12[15] = kk[0x11];
		#ifdef DEBUG_PRT
	  	hexdump("r12 :", r12, 16);
		#endif
	  	len &= ~0xf;
		#ifdef DEBUG_PRT
	  	hexdump("enc data :", enc_array, len);
	  	hexdump("key :", r10, len);
		#endif
		//1. get ans
	  	memcpy(r11, enc_array, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r13, pkey, len);
		flush_cache(r13, len);
		memset(pkey,0x00,len);
		#ifdef DEBUG_PRT
	  	hexdump("clr seed : ", r13, len);
		#endif
		//2. get ans
	  	memcpy(r11, enc_array1, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r14, pkey, len);
		flush_cache(r14, len);
		memset(pkey,0x00,len);
		#ifdef DEBUG_PRT
	  	hexdump("clr key :", r14, len);
		#endif
		//3. get ans
	  	ret=AES_ECB_encrypt( r12,len,r11,r13 );
	  	flush_cache(r11, len);
		memset(r12,0x00,len);
		#ifdef DEBUG_PRT
	  	hexdump("enc cid :", r11, len);
		#endif
		//4. get ans
	  	ret=AES_ECB_encrypt( r14,len,pkey,r11 );
	  	flush_cache(pkey, len);
		memset(r14,0x00,len);
		memset(r11,0x00,len);
	  	flush_cache(r14, len);
	  	flush_cache(r11, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc key :", pkey, len);
		#endif
		//w key
		otp_retry_cnt=0;
		ret = R0(pkey, 2304, len);
		memset(pkey,0x00,len);
	  	sync();
	  	break;
	  case ENUM_TYPE_AES_SEED:
	  	len &= ~0xf;
		#ifdef DEBUG_PRT
	  	hexdump("enc data :", enc_array, len);
	  	hexdump("dec key :", r10, len);
		#endif
		//1. get ans
	  	memcpy(r11, enc_array, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r13, pkey, len);
		flush_cache(r13, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr seed : ", r13, len);
		#endif
		#ifdef DEBUG_PRT
	  	hexdump("enc data :", enc_array1, len);
	  	hexdump("dec key :", r10, len);
		#endif
		//2.1 get ans
		flush_cache(enc_array1, len);
	  	memcpy(r11, enc_array1, len);
		flush_cache(r11, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r14, pkey, len);
		flush_cache(r14, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr key1 :", r14, len);
		#endif
		#ifdef DEBUG_PRT
	  	hexdump("enc data :", enc_array2, len);
	  	hexdump("dec key :", r10, len);
		#endif
		//2.2 get ans
		flush_cache(enc_array2, len);
	  	memcpy(r11, enc_array2, len);
		flush_cache(r11, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r15, pkey, len);
		flush_cache(r15, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr key2 :", r15, len);
		#endif
		//2.3 get ans
		flush_cache(enc_array3, len);
	  	memcpy(r11, enc_array3, len);
		flush_cache(r11, len);
	  	ret=AES_ECB_decrypt( r11,len,pkey,r10 );
		flush_cache(pkey, len);
	  	memcpy(r16, pkey, len);
		flush_cache(r16, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr key3 :", r16, len);
		#endif
		//TBD : get r12
		memset(r12,0x00,len);
		OTP_Get_Byte(4096, r12, len);
		flush_cache(r12, len);
		sync();
		//TBD : check the validation of cid field
		//3. get ans
	  	ret=AES_ECB_encrypt( r12,len,r11,r13 );
	  	flush_cache(r11, len);
		memset(r12, 0x00, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc cid :", r11, len);
		#endif
		//4. get ans
	  	ret=AES_ECB_encrypt( r14,len,pkey,r11 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc key1 :", pkey, len);
		#endif
	  	flush_cache(r14, len);
		//w key
		otp_retry_cnt=0;
		ret = R0(pkey, 2432, len);
	  	sync();
		if (!ret) return ret;
		//5. get ans
	  	ret=AES_ECB_encrypt( r15,len,pkey,r11 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc key2 :", pkey, len);
		#endif
	  	flush_cache(r15, len);
		//w key
		otp_retry_cnt=0;
		ret = R0(pkey, 4480, len);
	  	sync();
		if (!ret) return ret;
		//6. get ans
	  	ret=AES_ECB_encrypt( r16,len,pkey,r11 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc key3 :", pkey, len);
		#endif
	  	flush_cache(r16, len);
		//w key
		otp_retry_cnt=0;
		ret = R0(pkey, 4224, len);
	  	sync();
		if (!ret) return ret;
		otp_retry_cnt=0;
		//TBD : obfuscated
	  	flush_cache(r13, len);
		memcpy(r11,R1(1,r13,len),len);
	  	flush_cache(r11, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc seed :", r11, len);
		#endif
		ret = R0(r11, 4352, len);
	  	sync();
		if (!ret) return ret;
		break;
#ifndef RELEASE_MODE
	  case ENUM_TYPE_AES_DEC_KEY:
		memset(r12,0x00,16);
        	r12[0] = kk[0x28];
        	r12[1] = kk[0x19];
        	r12[2] = kk[0x3a];
        	r12[3] = kk[0x5b]+1;
        	r12[4] = kk[0x64];
        	r12[5] = kk[0x67];
        	r12[6] = kk[0x58];
		r12[7] = kk[0x49]+1;
		r12[8] = kk[0x7a];
		r12[9] = kk[0xb1];
		r12[10] = kk[0xc2];
		r12[11] = kk[0xd3];
		r12[12] = kk[0xe4];
		r12[13] = kk[0xf3];
		r12[14] = kk[0x50];
		r12[15] = kk[0x11];
		memset(r13,0x00,len);
		OTP_Get_Byte(4352, r13, len);
		flush_cache(r13, len);
		sync();
		memset(r14,0x00,len);
		OTP_Get_Byte(2304, r14, len);
		flush_cache(r14, len);
		sync();
#ifdef TEST_MODE
		r13[0]=0xf9;
		r13[1]=0xc8;
		r13[2]=0x63;
		r13[3]=0x3d;
		r13[4]=0x87;
		r13[5]=0xfe;
		r13[6]=0x1e;
		r13[7]=0x5e;
		r13[8]=0x49;
		r13[9]=0xb2;
		r13[10]=0x85;
		r13[11]=0xb0;
		r13[12]=0xaf;
		r13[13]=0x90;
		r13[14]=0x3b;
		r13[15]=0x81;
		r14[0]=0x50;
		r14[1]=0xc8;
		r14[2]=0x7d;
		r14[3]=0x14;
		r14[4]=0x41;
		r14[5]=0x21;
		r14[6]=0x95;
		r14[7]=0x74;
		r14[8]=0xfb;
		r14[9]=0x3b;
		r14[10]=0xf7;
		r14[11]=0xf7;
		r14[12]=0x1b;
		r14[13]=0xe3;
		r14[14]=0xa5;
		r14[15]=0xd5;
#endif
		//get
		memcpy(r11,R1(0,r13,len),len);
		#ifdef DEBUG_PRT
	  	hexdump("clr ob key :", r11, len);
	  	hexdump("enc key :", r14, len);
	  	hexdump("r12 key :", r12, len);
		#endif
	  	ret=AES_ECB_encrypt( r12,len,r15,r11 );
	  	flush_cache(r15, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc r15 :", r15, len);
		#endif
		memset(r11,0x00,len);
		memset(r12,0x00,len);
	  	ret=AES_ECB_decrypt( r14,len,r16,r15 );
	  	flush_cache(r16, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr key :", r16, len);
		#endif
		break;
	  case ENUM_TYPE_AES_DEC_SEED:
		//get efuse data
		memset(r12,0x00,len);
		OTP_Get_Byte(4096, r12, len);
		flush_cache(r12, len);
		sync();
		memset(r13,0x00,len);
		OTP_Get_Byte(4352, r13, len);
		flush_cache(r13, len);
		sync();
		memset(r14,0x00,len);
		OTP_Get_Byte(2432, r14, len);
		flush_cache(r14, len);
		sync();
		memset(r15,0x00,len);
		OTP_Get_Byte(4480, r15, len);
		flush_cache(r15, len);
		sync();
		memset(r16,0x00,len);
		OTP_Get_Byte(4224, r16, len);
		flush_cache(r16, len);
		sync();
#ifdef TEST_MODE
		r13[0]=0xf9;
		r13[1]=0xc8;
		r13[2]=0x63;
		r13[3]=0x3d;
		r13[4]=0x87;
		r13[5]=0xfe;
		r13[6]=0x1e;
		r13[7]=0x5e;
		r13[8]=0x49;
		r13[9]=0xb2;
		r13[10]=0x85;
		r13[11]=0xb0;
		r13[12]=0xaf;
		r13[13]=0x90;
		r13[14]=0x3b;
		r13[15]=0x81;
		r14[0]=0x0d;
		r14[1]=0xe7;
		r14[2]=0xb9;
		r14[3]=0x69;
		r14[4]=0xae;
		r14[5]=0xb9;
		r14[6]=0x94;
		r14[7]=0xe5;
		r14[8]=0x66;
		r14[9]=0x40;
		r14[10]=0xa5;
		r14[11]=0x35;
		r14[12]=0x54;
		r14[13]=0xe5;
		r14[14]=0x74;
		r14[15]=0x44;
		r15[0]=0xcb;
		r15[1]=0xfb;
		r15[2]=0xcc;
		r15[3]=0xc9;
		r15[4]=0x7d;
		r15[5]=0x08;
		r15[6]=0xc0;
		r15[7]=0xa4;
		r15[8]=0x5d;
		r15[9]=0xcf;
		r15[10]=0x22;
		r15[11]=0xb3;
		r15[12]=0x8a;
		r15[13]=0x9d;
		r15[14]=0x80;
		r15[15]=0x5f;
		r16[0]=0x74;
		r16[1]=0x5f;
		r16[2]=0x7b;
		r16[3]=0x76;
		r16[4]=0xf2;
		r16[5]=0x9d;
		r16[6]=0x50;
		r16[7]=0x76;
		r16[8]=0x44;
		r16[9]=0x49;
		r16[10]=0x06;
		r16[11]=0x0e;
		r16[12]=0x9f;
		r16[13]=0x67;
		r16[14]=0x34;
		r16[15]=0x37;
#endif
		#ifdef DEBUG_PRT
	  	hexdump("enc r12 key :", r12, len);
	  	hexdump("enc r13 key :", r13, len);
	  	hexdump("enc r14 key :", r14, len);
	  	hexdump("enc r15 key :", r15, len);
	  	hexdump("enc r16 key :", r16, len);
		#endif
		//get 
		memcpy(r11,R1(0,r13,len),len);
		flush_cache(r11, len);
		memset(r13,0x00,len);
	  	ret=AES_ECB_encrypt( r12,len,r13,r11 );
	  	flush_cache(r13, len);
		#ifdef DEBUG_PRT
	  	hexdump("enc r13 :", r13, len);
		#endif
		flush_cache(r14, len);
	  	ret=AES_ECB_decrypt( r14,len,pkey,r13 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr r14 :", pkey, len);
		#endif
		flush_cache(r15, len);
	  	ret=AES_ECB_decrypt( r15,len,pkey,r13 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr r15 :", pkey, len);
		#endif
		flush_cache(r16, len);
	  	ret=AES_ECB_decrypt( r16,len,pkey,r13 );
	  	flush_cache(pkey, len);
		#ifdef DEBUG_PRT
	  	hexdump("clr r16 :", pkey, len);
		#endif
		break;
#endif
	  case ENUM_TYPE_UNKNOWN:
		#ifdef DEBUG_PRT
	  	prints("data dec:\n");
		#endif
	  	pkey = enc_array;
	  	break;
          default:
		#ifdef DEBUG_PRT
	  	prints("unknown enum type\n");
		#endif
		break;
	}

	return ret;
}

unsigned char *R1(unsigned char mode, unsigned char * input, unsigned int len)
{
	int ret=0;
	unsigned int i=0;

        unsigned char kk[512]={ \
				0x00,0x02,0x01,0x21,0x4a,0x2c,0x85,0x82,0x7a,0x21,0x34,0x3d,0x14,0x00,0x14,0x99,	\
				0x20,0xa2,0x02,0x22,0xb5,0x1b,0x21,0x1a,0x71,0x22,0xa0,0x2d,0x31,0x0a,0x34,0x29,	\
				0x52,0x82,0x03,0xa8,0xc5,0x8a,0x12,0xd8,0x7a,0x23,0x84,0x32,0x24,0x0b,0x12,0x92,	\
				0x80,0x01,0x0a,0x11,0xab,0x87,0x31,0x88,0x88,0x24,0x94,0x1d,0x92,0xc0,0x11,0x39,	\
				0x08,0x01,0x05,0xa8,0x85,0x67,0xb0,0xc8,0x77,0x52,0x3a,0xaa,0x14,0x0d,0x1a,0x91,	\
				0x11,0x02,0xc8,0x28,0x4a,0x63,0xa1,0xca,0x62,0x21,0x3b,0x9d,0x44,0x90,0x12,0x21,	\
				0xff,0xd0,0x00,0x81,0x41,0x53,0xa5,0x88,0x12,0x28,0xc4,0x1a,0x38,0x01,0x11,0x89,	\
				0xf2,0x72,0x04,0x81,0x5f,0x61,0x85,0x82,0x18,0x79,0x04,0x0d,0x84,0x11,0x3b,0x98,	\
				0x19,0x60,0x00,0x79,0xa2,0x62,0x25,0x1a,0x78,0x3a,0x00,0x31,0x94,0xa0,0x1f,0x09,	\
				0x28,0x88,0x8d,0x81,0x11,0x61,0x18,0x8a,0x72,0x21,0x14,0x1d,0x2a,0x0a,0x2d,0x08,	\
				0x18,0x01,0x00,0xa8,0x85,0x67,0xb0,0xc8,0x77,0x52,0x3a,0xaa,0x14,0x0d,0x1b,0x98,	\
				0x31,0x02,0xc8,0x28,0x4a,0x63,0xa1,0xca,0x62,0x21,0x3b,0x9d,0x44,0x90,0x52,0x31,	\
				0xef,0xd0,0x00,0x81,0x41,0x53,0xa5,0x88,0x12,0x28,0xc4,0x1a,0x38,0x01,0x32,0x49,	\
				0xfe,0x72,0x04,0x82,0x5f,0x61,0x85,0x82,0x18,0x79,0x04,0x0d,0x84,0x11,0x31,0x95,	\
				0x1b,0x66,0x0f,0x71,0x22,0x32,0x25,0x1a,0x78,0x3a,0x00,0x31,0x94,0xa0,0x1e,0x99,	\
				0x17,0x78,0x7d,0x61,0x51,0x41,0x18,0x8a,0x72,0x21,0x14,0x1d,0x2a,0x0a,0x2a,0x0a,	\
        };
	__align__ unsigned char r16[16]; //out4

	memset(r16,0x00,len);	
        r16[0] = ~(kk[0x30]+1)&0xff;
        r16[1] = ~(kk[0x20]+3)&0xff;
        r16[2] = (kk[0x10]+5)&0xff;
        r16[3] = (kk[0x0b]+0xa)&0xff;
        r16[4] = (kk[0x71]+2)&0xff;
        r16[5] = (kk[0x62]+5)&0xff;
        r16[6] = (kk[0x53]+8)&0xff;
	r16[7] = ~(kk[0x44]+3)&0xff;
	r16[8] = ~(kk[0x78]+3)&0xff;
	r16[9] = ~(kk[0x91]+0xf)&0xff;
	r16[10] = (kk[0x2]+0xa)&0xff;
	r16[11] = (kk[0x13]+0xb)&0xff;
	r16[12] = (kk[0x24]+0xc)&0xff;
	r16[13] = ~(kk[0x53]+0xd)&0xff;
	r16[14] = ~(kk[0xa2]+0xe)&0xff;
	r16[15] = (kk[0x81]+0x0)&0xff;

	memset(v_array,0x00,1024);
	#ifdef DEBUG_PRT
	hexdump("obfuscator key : ", r16, len);
	#endif
	if (mode)
	{
		#ifdef DEBUG_PRT
  		prints("enc ");
		#endif
		input[1] ^= r16[1];
		input[4] ^= r16[4];
		input[8] ^= r16[8];
		input[0xa] ^= r16[0xa];
		ret=AES_ECB_encrypt( input,len,v_array,r16 );
		flush_cache(v_array, len);
	}
	else
	{
		#ifdef DEBUG_PRT
  		prints("dec ");
		#endif
		ret=AES_ECB_decrypt( input,len,v_array,r16 );
		flush_cache(v_array, len);
		v_array[1] ^= r16[1];
		v_array[4] ^= r16[4];
		v_array[8] ^= r16[8];
		v_array[0xa] ^= r16[0xa];
		flush_cache(v_array, len);
	}
	#ifdef DEBUG_PRT
	hexdump("out : ", v_array, len);
	#endif
	memset(r16,0x00,len);	
	flush_cache(r16,len);
		
	return v_array;
}

unsigned int R0(unsigned char* pkey, unsigned int ofset, unsigned int len)
{
	unsigned int i=0;
RETRY_START:
	//write
	sync();
        start_program(ofset, len, pkey);
	sync();
	//read
#ifdef DEBUG_PRT
	prints("write cnt : ");
	print_hex(otp_retry_cnt);
	prints(", ofset : ");
	print_hex(ofset);
	prints("\n");
#endif
	memset(v_array,0x00,len);
	OTP_Get_Byte(ofset, v_array, len);
	flush_cache(v_array, len);
        sync();
	#ifdef DEBUG_PRT
	hexdump("read otp :", v_array, len);
	#endif
        sync();
	//verify
	for(i=0;i<len;i++)
	{
		if (pkey[i] != v_array[i])
		{
			prints("array compare fail\n");
			otp_retry_cnt = otp_retry_cnt + 1;
			print_hex(otp_retry_cnt);
			if (otp_retry_cnt < MAX_RETRY_CNT)
			{
				prints("retry ...\n");
				goto RETRY_START;
			}
			else
			{
				prints(" --> stop !!\n");
				#ifdef TEST_MODE
				return 0;
				#else
				return 1;
				#endif
			}
		}
	}
	prints("array compare pass\n");
	return 0;
}

int obfuse_judge()
{
	return OTP_JUDGE_BIT(OTP_BIT_SECUREBOOT);
}
