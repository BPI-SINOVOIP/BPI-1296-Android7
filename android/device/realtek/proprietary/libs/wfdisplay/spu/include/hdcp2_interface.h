#ifndef _H2_INTERFACE_H_
#define _H2_INTERFACE_H_
#include "hdcp2_hal.h"
#include "hdcp2_messages.h"
#ifdef __cplusplus
	 extern "C" {
#endif

/////////////////////////////////////////////////////
// layout of secure flush (1KB : 0x3FF)
//---------------------------------------------------
// 0x000-0x00F : lc128     S (16) 
// 0x010-0x21F : cert_rx   N (522 + 6 padding)
// 0x220-0x35F : kpriv_rx  S (320)
// 0x360-0x36F : kh        S (16)
// 0x370-0x37F : km        S (16)
// 0x380-0x39F : kd        S (32)
// 0x3A0-0x3AF : dkey2     S (16)
// 0x3B0-0x3BF : Ks^lc128  S (16)   TS Descramble Key
// 0x3C0-0x3C7 : Riv       S (8)
// 0x3C8-0x3D7 : ks        S (16)
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define SRAM_ALIGN(d)           ((d + 7) & ~0x7)

#define SRAM_LC128_ENTRY        (0x40)
#define SRAM_LC128_SIZE         (SRAM_ALIGN(LC128_SIZE))

//#define SRAM_CERT_RX_ENTRY      (SRAM_LC128_ENTRY + (SRAM_LC128_SIZE >> 3))
#define SRAM_CERT_RX_SIZE       (SRAM_ALIGN(CERT_RX_SIZE))
unsigned char SRAM_CERT_RX_ENTRY[528];

//#define SRAM_KPRIV_RX_ENTRY     (SRAM_CERT_RX_ENTRY + (SRAM_CERT_RX_SIZE >> 3))
#define SRAM_KPRIV_RX_ENTRY     (SRAM_LC128_ENTRY + (SRAM_LC128_SIZE >> 3))
#define SRAM_KPRIV_RX_SIZE      (SRAM_ALIGN(KPRIVRX_SIZE))

#define SRAM_KH_ENTRY           (SRAM_KPRIV_RX_ENTRY + (SRAM_KPRIV_RX_SIZE >> 3))
#define SRAM_KH_SIZE            (SRAM_ALIGN(KH_SIZE))

#define SRAM_KM_ENTRY           (SRAM_KH_ENTRY + (SRAM_KH_SIZE >> 3))
#define SRAM_KM_SIZE            (SRAM_ALIGN(MASTERKEY_SIZE))

#define SRAM_KD_ENTRY           (SRAM_KM_ENTRY + (SRAM_KM_SIZE >> 3))
#define SRAM_KD_SIZE            (SRAM_ALIGN(KD_SIZE))

#define SRAM_DK2_ENTRY          (SRAM_KD_ENTRY + (SRAM_KD_SIZE >> 3))
#define SRAM_DK2_SIZE           (SRAM_ALIGN(DKEY_SIZE))

#define SRAM_KS_XOR_LC128_ENTRY (SRAM_DK2_ENTRY + (SRAM_DK2_SIZE >> 3))
#define SRAM_KS_XOR_LC128_SIZE  (SRAM_ALIGN(SESSIONKEY_SIZE))

#define SRAM_RIV_ENTRY          (SRAM_KS_XOR_LC128_ENTRY + (SRAM_KS_XOR_LC128_SIZE >> 3))
#define SRAM_RIV_SIZE           (SRAM_ALIGN(RIV_SIZE))

/////////////////////////////////////////////////////////////////////////////////

#define spu_SetLc128(v)          RCP_SET_CW(SRAM_LC128_ENTRY,  (unsigned char*) v, LC128_SIZE)
#define spu_GetLc128(v)          RCP_GET_CW(SRAM_LC128_ENTRY,  (unsigned char*) v, LC128_SIZE)

//#define spu_SetCertRx(v)         RCP_SET_CW(SRAM_CERT_RX_ENTRY, (unsigned char*) v, CERT_RX_SIZE)
#define spu_SetCertRx(v)         memcpy(SRAM_CERT_RX_ENTRY, (unsigned char*) v, CERT_RX_SIZE)
//#define spu_GetCertRx(v)         RCP_GET_CW(SRAM_CERT_RX_ENTRY, (unsigned char*) v, CERT_RX_SIZE)
#define spu_GetCertRx(v)         memcpy((unsigned char*) v, SRAM_CERT_RX_ENTRY, CERT_RX_SIZE)

#define spu_SetKPrivRx(v)        RCP_SET_CW(SRAM_KPRIV_RX_ENTRY, (unsigned char*) v, KPRIVRX_SIZE)
#define spu_GetKPrivRx(v)        RCP_GET_CW(SRAM_KPRIV_RX_ENTRY, (unsigned char*) v, KPRIVRX_SIZE)

#define spu_SetKH(v)             RCP_SET_CW(SRAM_KH_ENTRY, (unsigned char*) v, KH_SIZE)
#define spu_GetKH(v)             RCP_GET_CW(SRAM_KH_ENTRY, (unsigned char*) v, KH_SIZE)

#define spu_SetKM(v)             RCP_SET_CW(SRAM_KM_ENTRY, (unsigned char*) v, MASTERKEY_SIZE)
#define spu_GetKM(v)             RCP_GET_CW(SRAM_KM_ENTRY, (unsigned char*) v, MASTERKEY_SIZE)

#define spu_SetKD(v)             RCP_SET_CW(SRAM_KD_ENTRY, (unsigned char*) v, KD_SIZE)
#define spu_GetKD(v)             RCP_GET_CW(SRAM_KD_ENTRY, (unsigned char*) v, KD_SIZE)

#define spu_SetDKey2(v)          RCP_SET_CW(SRAM_DK2_ENTRY, (unsigned char*) v, DKEY_SIZE)
#define spu_GetDKey2(v)          RCP_GET_CW(SRAM_DK2_ENTRY, (unsigned char*) v, DKEY_SIZE)

#define spu_SetKsXorLc128(v)     RCP_SET_CW(SRAM_KS_XOR_LC128_ENTRY, (unsigned char*) v, SRAM_KS_XOR_LC128_SIZE)
#define spu_GetKsXorLc128(v)     RCP_GET_CW(SRAM_KS_XOR_LC128_ENTRY, (unsigned char*) v, SRAM_KS_XOR_LC128_SIZE)

#define spu_SetRiv(v)            RCP_SET_CW(SRAM_RIV_ENTRY, (unsigned char*) v, RIV_SIZE)
#define spu_GetRiv(v)            RCP_GET_CW(SRAM_RIV_ENTRY, (unsigned char*) v, RIV_SIZE)

/////////////////////////////////////////////////////////////////////////////////

/**
 * Secrets related to the session. Each new session
 * will reset these values.
 */

struct {
   unsigned char rtx[RTX_SIZE];
   unsigned char rn[RN_SIZE];
   unsigned char rRx[RRX_SIZE];

   unsigned char m[M_SIZE];
   unsigned char hPrime[H_SIZE];
   unsigned char lPrime[L_SIZE];
   unsigned char vPrime[V_SIZE/2];
} SessionSecrets;


/**
 * Definitions for various sizes
 */
#define SESSIONKEY_SIZE ( 128/8 )
#define MASTERKEY_SIZE  ( 128/8 )
#define KD_SIZE         ( 256/8 )
#define KH_SIZE         ( 128/8 )
#define CTR_SIZE        (  64/8 )
#define DKEY_SIZE       ( 128/8 )
#define STREAM_CTR_SIZE (  32/8 )
#define AES_BLK_SIZE    ( 256/8 )


typedef struct 
{
  unsigned char receiverId[RXID_SIZE];
  unsigned char kpubRx[KPUBRX_SIZE];
  unsigned char reserved[CERT_RESERVED_SIZE];
  unsigned char certRx[CERT_SIGN_SIZE];
} H2_ReceiverCert;

/**
 * Attached KSVs
 */


typedef struct {
   uint8_t DeviceCount;
   uint8_t Depth;
   uint8_t DevicesExceeded;
   uint8_t DepthExceeded;
   //hdcp2.1
   uint8_t HDCP20RepeaterDownStream;
   uint8_t HDCP1DeviceDownStream;
   uint8_t Ksvs[5*MAX_DEVICECOUNT];
} H2_gKsvInfo;

#define PAIRING_INIT 0
#define PAIRING_SEND_HPRIME 1
#define PAIRING_SEND_PAIRING_INFO 2

//int gPairingState = PAIRING_INIT;

/**
 * Store the secrets read from flash.
 * This data is constant once loaded and
 * should not ever change.
 *
 * This structure is here so that main.c can access it. When
 * the XTASK starts up, a global variable, SerializationSecrets, of
 * this type, will be initialized with the values read from flash.
 *
 */
typedef struct {
   unsigned char lc[LC128_SIZE];
   H2_ReceiverCert rxPubKeyCert;  
   unsigned char kPrivRx[KPRIVRX_SIZE];
} tSerializationSecrets;

enum
{
        RXID_200MS_TIMEOUT_UNDEF,
        RXID_200MS_TIMEOUT_INIT
};

/** More details of these functions are in hdcp2_interface.c */
H2status hdcp2_init( void );
H2status hdcp2_reset( void );
H2status hdcp2_GenrRx( void );
H2status hdcp2_Setrtx( const H2uint8 *rtx );
H2status hdcp2_Setrn( const H2uint8 *rn );
H2status hdcp2_SetRiv( const H2uint8 *riv );
H2status hdcp2_GetRiv( H2uint8 *riv );
H2status hdcp2_SetEKpubKm( const H2uint8 *EKpubKm );
H2status hdcp2_SetEKhKm( const H2uint8 *EKhKm, const H2uint8 *m );
H2status hdcp2_SetEdKeyKs( const H2uint8 *EdKeyKs );
H2status hdcp2_GetrRx( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_SetAesStreamCtr( H2uint8* pCtr, H2uint32 size ); 
H2status hdcp2_SetAesInputCtr( const H2uint8* pCtr, H2uint32 size ); 
H2status hdcp2_SetAesKey( const H2uint8* pIn, H2uint32 size ); 
H2status hdcp2_AesSetNonce( H2uint8* pOut, H2uint32 size );
H2status hdcp2_AesDecrypt( H2uint8* pOut, H2uint32 size );
H2status hdcp2_GetCertRx( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_GetEKhKm( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_GethPrime( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_GetlPrime( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_GetvPrime( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_SetKsvs( unsigned char *pMsg, unsigned int len );
H2status hdcp2_GetKsvInfo( H2uint8 *Devices, H2uint8 *Depth, H2uint8 *DevicesExceeded, H2uint8 *DepthExceeded, H2uint8 *pKSVs );

H2status hdcp2_decrypt(H2uint8 InputCtr[12], H2uint8* pData, H2uint32 Len);
H2status hdcp2_SetKsXorLc128(const H2uint8 *AESKey);
H2status hdcp2_load_from_flash(void); 

int hdcp22_GetProtocolDescriptorBit();
void hdcp2_SaveStringInFile(unsigned char *filename, unsigned char* str, int len);
H2status hdcp2_SetLPrime( const H2uint8 *lprime );
H2status hdcp2_JudgeLeastLPrime( H2_AkeRTTChallengePayLoad* payLoad , H2uint8 cnt );
H2status hdcp2_GetMostlPrime( H2uint8* pOut, H2uint32 ulSize );
H2status hdcp2_GetMPrime( H2_RepeaterAuthReady_PayLoad* pOut, unsigned char *message, int length );
H2status hdcp2_JudgeLeastVPrime( unsigned char * MVPrime , H2uint8 cnt );
H2status hdcp2_SetLeastVPrime( unsigned char * MVPrime );
int hdcp2_GetKSXorLc128_CWOffset();
int hdcp2_GetRiv_CWOffset( H2uint8 *riv );

#ifdef __cplusplus
}
#endif

#endif //_HDCP2_INTERFACE_H_
