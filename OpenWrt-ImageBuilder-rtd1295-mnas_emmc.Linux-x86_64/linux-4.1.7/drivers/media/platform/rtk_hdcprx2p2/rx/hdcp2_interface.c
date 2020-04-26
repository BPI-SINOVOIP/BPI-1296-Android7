#include <hdcp2_hal.h>
#include <bigdigits.h>
#include <crypto.h>
#include <hdcp2_messages.h>
#include <hdcp2_interface.h>
#include <hdcp2_session.h>
#include <rcp_api.h>
#include <hmac.h>
#include <hdmi.h>

H2_gKsvInfo gKsvInfo;

#ifdef CONFIG_RTK_HDMITX
extern int hdmitx_switch_get_state(void);
#endif
#ifdef TEST_HDCP2_2_RX_DRIVER
/**
 * Interface definition / prototypes
 */

#ifdef DEBUG_XTASK_INTERFACE
#define LOCALDBG ENABLE
#else
#define LOCALDBG DISABLE
#endif

unsigned char global_certRx[CERT_RX_SIZE] = {0};
unsigned char global_kprivRx[KPRIVRX_SIZE] = {0};
 
 //HDCP2.2 repeater  add
extern H2_RepeaterAuthSendRxIdList_PayLoad g_repeater_auth_rx_payload;
extern unsigned char bhdcp_repeater_get_state ;
extern unsigned char Repeater_clear_repeater_bit;
unsigned char global_vprime[V_SIZE] = {0};
/**
 * Attached KSVs
 */
 static struct {
	unsigned char rtx[RTX_SIZE];
	unsigned char rn[RN_SIZE];
	unsigned char rRx[RRX_SIZE];
	unsigned char m[M_SIZE];
	unsigned char hPrime[H_SIZE];
	unsigned char txcaps[TXCAPS_SIZE];
	unsigned char rxcaps[RXCAPS_SIZE];
	unsigned char ks[KS_SIZE];
	unsigned char vprime[32];
} SessionSecrets_Rx;

/**
 * @func spu_printBytes
 * @scope Static
 *
 * NOTE: This function does not do anything if LOCALDBG is set to DISABLE.
 *
 * @param H2uint8 *bytes - Data to print
 * @param H2uint32 len - How many bytes to print
 * @return void
 */
#ifdef DEBUG_XTASK_INTERFACE
static void spu_printBytes(const H2uint8 *bytes, H2uint32 len)
{
	unsigned int ii;
	for (ii = 0; ii < len; ii++) {
		pr_debug("%02x ", bytes[ii]);
		if (ii % 16 == 15) {
			pr_debug("\r\n");
		}
	}
	pr_debug("\r\n");
}

#define spu_printBytes_with_text(x, y, fmt, args...)    do {pr_debug(fmt, ## args); spu_printBytes(x, y); } while (0)

#else
#define spu_printBytes(x, y)
#define spu_printBytes_with_text(x, y, args...)
#endif
int read_binary_file(const char *path, unsigned char *buff, unsigned int len)
{
	struct file *file = NULL;
	int ret;
	file = filp_open(path, O_RDONLY, 0444);
	if (IS_ERR(file)) {
		pr_debug("create file error\n");
		return -1;
	}
	ret = kernel_read(file, 0, buff, len);
	filp_close(file, 0);
	return ret;
}

int write_binary_file(const char *path, unsigned char *buff, unsigned int len)
{
	struct file *file = NULL;
	mm_segment_t fs;
	loff_t pos;

	file = filp_open(path, O_RDWR | O_CREAT, 0444);
	if (IS_ERR(file)) {
		pr_debug("create file error\n");
		return -1;
	}
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = 0;
	vfs_write(file, buff, len, &pos);
	filp_close(file, 0);
	set_fs(fs);
	return 0;
}


/**
 * Zero out session secrets, called from CID_INIT
 */
H2status hdcp2_Rx_init(void)
{
	hdcp2_load_from_flash();
	memset((char *)&SessionSecrets_Rx, 0, sizeof(SessionSecrets_Rx));
	memset((char *)&gKsvInfo, 0, sizeof(gKsvInfo));

	    /* Disable the HDMI police */
	hdmi_policeEnable(FALSE);
	return H2_OK;
}

/**
 * Zero out session secrets
 */
H2status hdcp2_Rx_reset(void)
{
	memset((char *)&SessionSecrets_Rx, 0, sizeof(SessionSecrets_Rx));

	    /* Disable the HDMI police */
	hdmi_policeEnable(FALSE);
	return H2_OK;
}

#ifdef HDCP2_2_WIFI
H2status hdcp2_Rx_SetRepeater_wifi(char repeater)
{
	if (repeater & REPEATER_MASK)
		SessionSecrets_Rx.repeater = 1;
	else
		SessionSecrets_Rx.repeater = 0;
	pr_debug("repeater:%x\n", repeater);
	return H2_OK;
}

int hdcp2_Rx_IsRepeater(void)
{
	if (SessionSecrets_Rx.repeater)
		return TRUE;
	else
		return FALSE;
}

BOOL hdcp2_Rx_IsProDesp(void)
{
	if (*(CertKey.Reserved) & 0xf)
		return TRUE;
	else
		return FALSE;
}

#endif

/**
 * Generate the Pseudo-Random 64 bit value rRx.
 *
 * Results are stored in SessionSecrets.rRx.
 *
 */
H2status hdcp2_Rx_GenrRx(void)
{
#if 0
	H2uint32 rRx[2];
	rRx[0] = crypto_random32();
	rRx[1] = crypto_random32();

#endif
	unsigned char rRx[] = {
		0xe1, 0x7a, 0xb0, 0xfd, 0x0f, 0x54, 0x40, 0x52
	};
	memcpy(SessionSecrets_Rx.rRx, rRx, sizeof(SessionSecrets_Rx.rRx));
	return H2_OK;
}

/**
 * Copy RTX to Session Secrets.
 */
H2status hdcp2_Rx_Setrtx(const H2uint8 *rtx)
{
	memcpy(SessionSecrets_Rx.rtx, rtx, sizeof(SessionSecrets_Rx.rtx));
	return H2_OK;
}

H2status hdcp2_Rx_SetTxcaps(unsigned char *message)
{
	if (message == NULL)
		return H2_ERROR;
	memcpy(SessionSecrets_Rx.txcaps, message, TXCAPS_SIZE);
		return H2_OK;
}

H2status hdcp2_Rx_GetTxcaps(H2uint8 *txcaps, H2uint32 ulSize)
{
	memcpy(txcaps, SessionSecrets_Rx.txcaps, sizeof(SessionSecrets_Rx.txcaps));
		return H2_OK;
}

H2status hdcp2_Rx_GenRxcaps(void)
{
	unsigned char rxcaps[3];
//bhdcp_repeater_get_state = 0x1;
if(bhdcp_repeater_get_state ==0x1)
{
		#ifdef CONFIG_RTK_HDMITX
		if(hdmitx_switch_get_state()==1)//Wait HDMI TX
		{
			rxcaps[2] = 1;
		}
		else
		#endif
		{
		       bhdcp_repeater_get_state =0;
			rxcaps[2] = 0;
    }
       }
else
{
    rxcaps[2] = 0;
		//pr_err("hdcp2_Rx_GenRxcaps repeater =0");
 }

//       rxcaps[2] = 1;
rxcaps[1] = 0;
rxcaps[0] = 2;
memcpy(SessionSecrets_Rx.rxcaps, rxcaps, sizeof(SessionSecrets_Rx.rxcaps));
return H2_OK;
}

H2status hdcp2_Rx_GetRxcaps(H2uint8 *rxcaps, H2uint32 ulSize)
{
	memcpy(rxcaps, SessionSecrets_Rx.rxcaps, sizeof(SessionSecrets_Rx.rxcaps));
		return H2_OK;
}

/**
 * Copy Rn to Session Secrets.
 */
H2status hdcp2_Rx_Setrn(const H2uint8 *rn)
{
	memcpy(SessionSecrets_Rx.rn, rn, sizeof(SessionSecrets_Rx.rn));
	return H2_OK;
}

H2status hdcp2_Rx_GetRn(H2uint8 *rn, H2uint32 ulSize)
{
	memcpy(rn, SessionSecrets_Rx.rn, sizeof(SessionSecrets_Rx.rn));
	return H2_OK;
}

/**
 * Set initialization vector RIV.
 */
H2status hdcp2_Rx_SetRiv(const H2uint8 *riv)
{
	spu_SetRiv(riv);
	return H2_OK;
}

/**
 * Receive EKpubKm from userspace and process it.
 *
 * Results are stored in SessionSecrets.
 * @todo Support 'repeater'=1.
 *
 */
H2status hdcp2_Rx_SetEKpubKm(const H2uint8 *EKpubKm)
{
	H2uint8 Km_tmp[MASTERKEY_SIZE];
	H2uint8 Kd_tmp[KD_SIZE];
	H2uint8 KPriv_tmp[KPRIVRX_SIZE];
	H2status rc;
	H2uint8 rtx[RTX_SIZE];
	H2uint8 rrx[RRX_SIZE];
	H2uint8 rn[RN_SIZE] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char rxcaps[RXCAPS_SIZE];
	unsigned char txcaps[TXCAPS_SIZE];

	    /* Decrypt Km */
	spu_GetKPrivRx(KPriv_tmp);
	rc = Decrypt_EKpubKm_kPrivRx(KPriv_tmp, Km_tmp, EKpubKm);
	if (rc != H2_OK) {
		H2DBGLOG((LOCALDBG, "Error decrypting master key!\r\n"));
		goto end_proc;
	}
	spu_SetKM(Km_tmp);
	//printk("TEST km:\n");
	//spu_print(Km_tmp, KM_SIZE);
	hdcp2_Rx_GetrTx(rtx, RTX_SIZE);
	hdcp2_Rx_GetrRx(rrx, RRX_SIZE);

    /** Generate Kd = dKey0 || dKey1 */
	    /*RCP_HDCP2_GenKd(SRAM_KM_ENTRY, SessionSecrets.rtx, SessionSecrets.rn, SRAM_KD_ENTRY);*/
	RCP_HDCP2_GenKd(SRAM_KM_ENTRY, rtx, rrx, rn, SRAM_KD_ENTRY, 1);
	//printk("Test KD\n");
	spu_GetKD(Kd_tmp);
	//spu_print(Kd_tmp, KD_SIZE);
	hdcp2_Rx_GetRxcaps(rxcaps, RXCAPS_SIZE);
	hdcp2_Rx_GetTxcaps(txcaps, TXCAPS_SIZE);

    /** Compute HPrime from Kd, RTX, and Repeater */
	    /*Compute_Hprime_Rx(Kd_tmp, rtx, 1, SessionSecrets_Rx.hPrime);*/
	Compute_Hprime_22(Kd_tmp, rtx, txcaps, rxcaps, SessionSecrets_Rx.hPrime);
	//printk("Test hPrime\n");
	//spu_print(SessionSecrets_Rx.hPrime, H_SIZE);
end_proc:
	memset(Kd_tmp, 0, sizeof(Kd_tmp));
	memset(Km_tmp, 0, sizeof(Km_tmp));
	memset(KPriv_tmp, 0, sizeof(KPriv_tmp));
	return rc;
}


/**
 * Set EKhKm and m. m should be stored after EKhKm.
 */
H2status hdcp2_Rx_SetEKhKm(H2uint8 *EKhKm, H2uint8 *m)
{
	H2uint8 Kh_temp[H_SIZE];
	H2uint8 Km_temp[MASTERKEY_SIZE];
	H2uint8 Kd_temp[KD_SIZE];
	H2uint8 KPriv_tmp[KPRIVRX_SIZE];
	H2uint8 rtx[RTX_SIZE];
	H2uint8 rrx[RRX_SIZE];
	H2uint8 rn[RN_SIZE] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char rxcaps[RXCAPS_SIZE];
	unsigned char txcaps[TXCAPS_SIZE];

	/* Compute Kh */
	spu_GetKPrivRx(KPriv_tmp);
	Compute_Kh(KPriv_tmp, KPRIVRX_SIZE, Kh_temp);

	memcpy(SessionSecrets_Rx.m, m, M_SIZE);
	spu_GetKH(Kh_temp);

    /** Compute Km */
	RCP_AES_CTR_Cipher(Kh_temp, SessionSecrets_Rx.m, EKhKm, Km_temp, EKHKM_SIZE);
	spu_SetKM(Km_temp);
	//printk("TEST km:\n");
	//spu_print(Km_temp, 16);

    /** Compute Kd */
	    /*RCP_HDCP2_GenKd(SRAM_KM_ENTRY, SessionSecrets.rtx, SessionSecrets.rn, SRAM_KD_ENTRY);
	     RCP_HDCP2_GenKd(SRAM_KM_ENTRY, rtx, rrx, rn, SRAM_KD_ENTRY,0);*/
	hdcp2_Rx_GetrTx(rtx, RTX_SIZE);
	hdcp2_Rx_GetrRx(rrx, RRX_SIZE);
	RCP_HDCP2_GenKd(SRAM_KM_ENTRY, rtx, rrx, rn, SRAM_KD_ENTRY, 1);
	spu_GetKD(Kd_temp);
	//printk("Test KD\n");
	//spu_print(Kd_temp, KD_SIZE);

    /** Compute HPrime */
	    /*Compute_Hprime_Rx(Kd_temp, SessionSecrets.rtx, HDCP_REPEATER, SessionSecrets.hPrime);*/
	hdcp2_Rx_GetRxcaps(rxcaps, RXCAPS_SIZE);
	hdcp2_Rx_GetTxcaps(txcaps, TXCAPS_SIZE);

	    /*Compute_Hprime_Rx(Kd_temp, rtx, 1, SessionSecrets_Rx.hPrime);*/
	Compute_Hprime_22(Kd_temp, rtx, txcaps, rxcaps, SessionSecrets_Rx.hPrime);
	//printk("Test1 hPrime\n");
	//spu_print(SessionSecrets_Rx.hPrime, H_SIZE);
	memset(Kh_temp, 0, sizeof(Kh_temp));
	memset(Km_temp, 0, sizeof(Km_temp));
	memset(Kd_temp, 0, sizeof(Kd_temp));
	return H2_OK;
}

H2status hdcp2_Rx_ParseEKhKm(H2uint8 *EKhKm, H2uint8 *m)
{
	H2uint8 Kh[KH_SIZE];
	H2uint8 Km[MASTERKEY_SIZE];
	spu_GetKH(Kh);

#if 0
	HDCP2DBGLOG("Kh: ", Kh, KH_SIZE);
	HDCP2DBGLOG("m: ", m, M_SIZE);
	HDCP2DBGLOG("EKhKm: ", EKhKm, EKHKM_SIZE);

#endif
    /** Compute Km */
	RCP_AES_CTR_Cipher(Kh, m, EKhKm, Km, EKHKM_SIZE);
	spu_SetKM(Km);
	return H2_OK;
}

H2status hdcp2_Rx_SetKs(const H2uint8 *ks)
{
	memcpy(SessionSecrets_Rx.ks, ks, sizeof(SessionSecrets_Rx.ks));
	return H2_OK;
}

H2status hdcp2_Rx_GetKs(H2uint8 *ks, H2uint32 ulSize)
{
	memcpy(ks, SessionSecrets_Rx.ks, sizeof(SessionSecrets_Rx.ks));
	return H2_OK;
}

/**
 * Process EdKeyKs message. Compute sessionKey based on
 * EdKeyKs and dKey2
 */
H2status hdcp2_Rx_SetEdKeyKs(H2uint8 *EdKeyKs)
{
	unsigned char dkey2[16];
	H2uint8 rtx[RTX_SIZE];
	H2uint8 rrx[RRX_SIZE];
	H2uint8 rn[RN_SIZE];
	unsigned char ks[16];
	hdcp2_Rx_GetrTx(rtx, RTX_SIZE);
	hdcp2_Rx_GetrRx(rrx, RRX_SIZE);
	hdcp2_Rx_GetRn(rn, RN_SIZE);

    /** Compute dKey2 */
	RCP_HDCP2_GenDKey(SRAM_KM_ENTRY, rtx, rrx, rn, 2, SRAM_DK2_ENTRY, 1);

	    /*RCP_HDCP2_GenKd(SRAM_KM_ENTRY, rtx, rrx, rn, SRAM_KD_ENTRY,0);*/
	spu_GetDKey2(dkey2);
//	printk("TEST dkey2:\n");
//	spu_print(dkey2, 16);

#if 0
	unsigned char rrx_temp[16];
	unsigned char temp[16];
	memset(rrx_temp, 0, sizeof(rrx_temp));
	memcpy(rrx_temp + DKEY_SIZE - RRX_SIZE, rrx, RRX_SIZE);
	crypt_xor(EdKeyKs, dkey2, temp, DKEY_SIZE);
	crypt_xor(temp, rrx_temp, ks, DKEY_SIZE);

#endif

    /** Compute Session Key */
	RCP_HDCP2_GenKs(SRAM_DK2_ENTRY, EdKeyKs, rrx, SRAM_KS_XOR_LC128_ENTRY);
	spu_GetKsXorLc128(ks);
	hdcp2_Rx_SetKs(ks);
//	printk("TEST ks:\n");
//	spu_print(ks, 16);

	#if ENABLE_DDC_WRITE
	drvif_KS_Setting(ks);
	#endif
    /** Compute AES Key */
	RCP_HDCP2_GenKsXorLc128(SRAM_LC128_ENTRY, SRAM_KS_XOR_LC128_ENTRY, SRAM_KS_XOR_LC128_ENTRY);

	    /*
	     * Enable the HDMI police
	     */
	hdmi_policeEnable(TRUE);
	return H2_OK;
}

H2status hdcp2_Rx_Compute_EdKeyKs(H2uint8 *rtx, H2uint8 *rrx, H2uint8 *rn, H2uint8 *EdKeyKs, int modeHDCP22)
{
	unsigned char dkey2[DKEY_SIZE];
	unsigned char ks[KS_SIZE];
	RCP_HDCP2_GenDKey(SRAM_KM_ENTRY, rtx, rrx, rn, 2, SRAM_DK2_ENTRY, modeHDCP22);
	spu_GetDKey2(dkey2);

	/** Compute Session Key */
	RCP_HDCP2_GenKs(SRAM_DK2_ENTRY, EdKeyKs, rrx, SRAM_KS_XOR_LC128_ENTRY);
	spu_GetKsXorLc128(ks);
	hdcp2_Rx_SetKs(ks);
	return H2_OK;
}

H2status hdcp2_Rx_GetrTx(H2uint8 *pOut, H2uint32 ulSize)
{

	memcpy(pOut, SessionSecrets_Rx.rtx, RTX_SIZE);
	return H2_OK;
}

/**
 * Fetch the generated rRx value to send to the TX.
 *
 */
H2status hdcp2_Rx_GetrRx(H2uint8 *pOut, H2uint32 ulSize)
{
	memcpy(pOut, SessionSecrets_Rx.rRx, sizeof(SessionSecrets_Rx.rRx));
	return H2_OK;
}

/**
 * Read the public key certificate from Serialization Secrets.
 */
H2status hdcp2_Rx_GetCertRx(H2uint8 *pOut, H2uint32 ulSize)
{
	spu_GetCertRx(pOut);
	return H2_OK;
}

/**
 * Get the computed EKhKm value to be sent to the TX
 */
H2status hdcp2_Rx_GetEKhKm(H2uint8 *pOut, H2uint32 ulSize)
{
	unsigned char kh[KH_SIZE];
	unsigned char km[16];
	unsigned char m[16];
	memset(m, 0, 16);
	memcpy(m, SessionSecrets_Rx.rtx, 8);
	memcpy(m + 8, SessionSecrets_Rx.rRx, 8);
	spu_GetKH(kh);
	spu_GetKM(km);

	    /*RCP_HDCP2_EkhKm(SRAM_KH_ENTRY, SRAM_KM_ENTRY, SessionSecrets.rtx, pOut);   */
	RCP_AES_CTR_Cipher(kh, m, km, pOut, 16);
	return H2_OK;
}

H2status hdcp2_Rx_Compute_EKhKm(H2uint8 *m, H2uint8 *Ekhkm)
{
	unsigned char kh[KH_SIZE];
	unsigned char km[KM_SIZE];
	spu_GetKH(kh);
	spu_GetKM(km);

	    /*RCP_HDCP2_EkhKm(SRAM_KH_ENTRY, SRAM_KM_ENTRY, SessionSecrets.rtx, pOut);    */
	RCP_AES_CTR_Cipher(kh, m, km, Ekhkm, 16);
	return H2_OK;
}

/**
 * Get HPrime.
 */
H2status hdcp2_Rx_GethPrime(H2uint8 *pOut, H2uint32 ulSize)
{
	memcpy(pOut, &SessionSecrets_Rx.hPrime, sizeof(SessionSecrets_Rx.hPrime));
		return H2_OK;
}

/**
 * Get L prime.
 */
H2status hdcp2_Rx_GetlPrime(H2uint8 *pOut, H2uint32 ulSize)
{
	H2uint8 Kd_tmp[KD_SIZE];
	H2uint8 rrx[RRX_SIZE];
	H2uint8 rn[RN_SIZE];

	spu_GetKD(Kd_tmp);
	hdcp2_Rx_GetrRx(rrx, RRX_SIZE);
	hdcp2_Rx_GetRn(rn, RN_SIZE);
	Compute_Lprime_Rx(Kd_tmp, rrx, rn, pOut);
	memset(Kd_tmp, 0, sizeof(Kd_tmp));
	//printk("Test  Lprime:\n");
	//spu_print(pOut, L_SIZE);
	return H2_OK;
}

/**
 * The KSVs message contains:
 * Device Count - 1 byte
 * Depth ( 1 byte )
 * Max Devs Exceeded ( 1 byte )
 * Max Cascade Exceeded ( 1 byte )
 * if ( Max Devs Exceeded == 0 && MaxCascadeExceeded == 0 )
 * KSVs ( 5 bytes each )
 */
H2status hdcp2_Rx_SetKsvs(unsigned char *pMsg, unsigned int len)
{
	memset(&gKsvInfo, 0, sizeof(gKsvInfo));
	gKsvInfo.DeviceCount = *pMsg;
	if (gKsvInfo.DeviceCount == 0)
		goto end_proc;
	gKsvInfo.Depth = *(pMsg + 1) + 1;
	gKsvInfo.DevicesExceeded = *(pMsg + 2);
	gKsvInfo.DepthExceeded = *(pMsg + 3);
	H2DBGLOG((LOCALDBG, "KSVINFO: %u devices, %u depth, %u dev exceed, %u depth exceed\r\n",
		gKsvInfo.DeviceCount, gKsvInfo.Depth,
		gKsvInfo.DevicesExceeded, gKsvInfo.DepthExceeded));
	if ((gKsvInfo.DeviceCount < MAX_DEVICECOUNT) && (gKsvInfo.DevicesExceeded == 0) && (gKsvInfo.DepthExceeded == 0)) {
		H2DBGLOG((LOCALDBG, "KSVINFO: Copying %u bytes\r\n", 5 * gKsvInfo.DeviceCount));
		memcpy(gKsvInfo.Ksvs, pMsg + 4, 5 * gKsvInfo.DeviceCount);
	} else
		gKsvInfo.DevicesExceeded = 1;
end_proc:
	return H2_OK;
}

H2status hdcp2_Rx_CalMPrime(unsigned char *bstream_id_type, unsigned char *bseq_num_M, unsigned char *Mprime)
{

    #if 0
	H2uint8 Kd_tmp[KD_SIZE] = {0x4f,0x14,0x8d,0x11,0xdd,0x49,0x18,0x10,0x6f,0xab,0x16,0x6f,
							0xf6,0xfd,0xa6,0xed,0xb5,0x02,0x0c,0x0d,0xf2,0x81,0xba,0xdf,
							0xe4,0x19,0x77,0xfa,0xd0,0xac,0x61,0x17 };

	H2uint8 bstream_id_T[2] = {0,0x1};
	H2uint8 bseq_num_M_T[3] = {0,0,0};
	#else

	H2uint8 Kd_tmp[KD_SIZE] = {0};
	spu_GetKD(Kd_tmp);
	#endif
    Compute_Mprime_22(Kd_tmp, bstream_id_type, bseq_num_M, Mprime);
	memset(Kd_tmp, 0, sizeof(Kd_tmp));
}
/**
 * Compute and return vPrime.
 */
#define test_Vprime  0
H2status hdcp2_Rx_CalvPrime(unsigned char *receiver_id, unsigned char *temp_rxinfor, unsigned char *temp_seq_num, unsigned char *Vprime)
{
#if test_Vprime
H2uint8 Kd_tmp[KD_SIZE] = {0x4f,0x14,0x8d,0x11,0xdd,0x49,0x18,0x10,0x6f,0xab,0x16,0x6f,
							0xf6,0xfd,0xa6,0xed,0xb5,0x02,0x0c,0x0d,0xf2,0x81,0xba,0xdf,
							0xe4,0x19,0x77,0xfa,0xd0,0xac,0x61,0x17 };

#else
	H2uint8 Kd_tmp[KD_SIZE] = {0};
	H2uint8 rev_receive_ID[5]={0};
	spu_GetKD(Kd_tmp);
#endif

#if test_Vprime
	Compute_Vprime_22(Kd_tmp, receiver_ID_LIST, global_rxinfo, global_seq_num_V, global_Vprime);
	spu_print(global_Vprime, 32);
	pr_err("TEST \n");
#endif

	Compute_Vprime_22(Kd_tmp, receiver_id, temp_rxinfor, temp_seq_num, Vprime);
	//spu_print(Vprime, 32);
	memset(Kd_tmp, 0, sizeof(Kd_tmp));
}


H2status hdcp2_Rx_GetvPrime(H2uint8 *pOut, H2uint32 ulSize)
{
	H2status rc = H2_ERROR;
	memcpy(pOut, &SessionSecrets_Rx.vprime, sizeof(SessionSecrets_Rx.vprime));
	return rc;
}

H2status hdcp2_Rx_GetKsvInfo(H2uint8 *Devices, H2uint8 *Depth, H2uint8 *DevicesExceeded, H2uint8 *DepthExceeded, H2uint8 *pKSVs ,H2uint8 *repeater_down_stream,H2uint8 *repeater_hdcp1_down_stream)
{
       #if REAPETER_2p2
       gKsvInfo.DeviceCount = g_repeater_auth_rx_payload.deviceCount;
	gKsvInfo.Depth =   g_repeater_auth_rx_payload.depth ;
	gKsvInfo.DevicesExceeded = g_repeater_auth_rx_payload.maxDevsExceeded;
	gKsvInfo.DepthExceeded =g_repeater_auth_rx_payload.maxCascadeExceeded;
	gKsvInfo.HDCP20RepeaterDownStream=g_repeater_auth_rx_payload.HDCP2_0_repeater_downstream;
	gKsvInfo.HDCP1DeviceDownStream =g_repeater_auth_rx_payload.HDCP1_device_downstream;
       memcpy(pKSVs, g_repeater_auth_rx_payload.Receiver_ID_LIST,  5*gKsvInfo.DeviceCount);
//	pr_err("hdcp2_Rx_GetKsvInfo ID list 2  %x ,%x ,%x ,%x ,%x ",g_repeater_auth_rx_payload.Receiver_ID_LIST[5],g_repeater_auth_rx_payload.Receiver_ID_LIST[6],g_repeater_auth_rx_payload.Receiver_ID_LIST[7],g_repeater_auth_rx_payload.Receiver_ID_LIST[8],g_repeater_auth_rx_payload.Receiver_ID_LIST[9]);
	#else
	unsigned char Buff[5 * MAX_DEVICECOUNT + 16];
	int ret = read_binary_file("/tmp/ksv_list.bin", Buff, sizeof(Buff));
	H2DBGLOG((LOCALDBG, "ReadKSVData=%d\n", ret));
	if (ret < 6) {
		gKsvInfo.DeviceCount = 0;
		gKsvInfo.Depth = 0;
		gKsvInfo.DevicesExceeded = 0;
		gKsvInfo.DepthExceeded = 0;
		memset(gKsvInfo.Ksvs, 0, sizeof(gKsvInfo.Ksvs));
	} else {
		gKsvInfo.DeviceCount = ret / 5;
		gKsvInfo.Depth = Buff[0] + 1;
		gKsvInfo.DevicesExceeded = (gKsvInfo.DeviceCount > H2_MAX_DEVICECOUNT) ? 1 : 0;
		gKsvInfo.DepthExceeded = (gKsvInfo.Depth > 4) ? 1 : 0;
		memcpy(gKsvInfo.Ksvs, &Buff[1], gKsvInfo.DeviceCount * 5);
	}
		gKsvInfo.DeviceCount = 1; //ret / 5;
		gKsvInfo.Depth =   1 ; //Buff[0] + 1;
		gKsvInfo.DevicesExceeded = (gKsvInfo.DeviceCount > H2_MAX_DEVICECOUNT) ? 1 : 0;
		gKsvInfo.DepthExceeded = (gKsvInfo.Depth > 4) ? 1 : 0;
	//	memcpy(gKsvInfo.Ksvs, &Buff[1], gKsvInfo.DeviceCount * 5);

	#endif
	if (Devices) {
		H2DBGLOG((LOCALDBG, "Device Count: %u\r\n", gKsvInfo.DeviceCount));
		*Devices = gKsvInfo.DeviceCount;
		//pr_err("Devices =%x \n" , *Devices);
	}
	if (Depth) {
		H2DBGLOG((LOCALDBG, "Depth: %u\r\n", gKsvInfo.Depth));
		*Depth = gKsvInfo.Depth;

			//pr_err("Depth =%x \n" , *Depth);
	}
	if (DevicesExceeded) {
		H2DBGLOG((LOCALDBG, "Devices Exceeded: %u\r\n", gKsvInfo.DevicesExceeded));
		*DevicesExceeded = gKsvInfo.DevicesExceeded;
	}
	if (DepthExceeded) {
		H2DBGLOG((LOCALDBG, "Depth Exceeded: %u\r\n", gKsvInfo.DepthExceeded));
		*DepthExceeded = gKsvInfo.DepthExceeded;
	}
	if(repeater_down_stream){
	*repeater_down_stream = gKsvInfo.HDCP20RepeaterDownStream;
		
	}

	if(repeater_down_stream){
	*repeater_hdcp1_down_stream = gKsvInfo.HDCP1DeviceDownStream;
	}
	#if 0
	if (pKSVs && !gKsvInfo.DevicesExceeded && !gKsvInfo.DepthExceeded) {
		H2DBGLOG((LOCALDBG, "Copying %u KSVs\r\n", gKsvInfo.DeviceCount));
		memcpy(pKSVs, gKsvInfo.Ksvs, gKsvInfo.DeviceCount * 5);
	}

	#endif
	return H2_OK;
}

/**
 * Set Ks^lc128. (for decryptor test only...)
 */
H2status hdcp2_Rx_SetKsXorLc128(const H2uint8 *AESKey)
{
	spu_SetKsXorLc128(AESKey);
	return H2_OK;
}

/**
 * Copy Rn to Session Secrets.
 */
H2status hdcp2_Rx_decrypt(H2uint8 InputCtr[12], H2uint8 *pData, H2uint32 Len)
{
	unsigned char IV[16];
	spu_GetRiv(IV);
	IV[4] ^= InputCtr[0];
	IV[5] ^= InputCtr[1];
	IV[6] ^= InputCtr[2];
	IV[7] ^= InputCtr[3];
	memcpy(&IV[8], &InputCtr[4], 8);
	RCP_AES_CTR_Decryption(KEY_CW(SRAM_KS_XOR_LC128_ENTRY), IV, pData, pData, Len);
	memcpy(&InputCtr[4], &IV[8], 8);
	return H2_OK;
}

H2status hdcp2_save_encrypted_keyset(unsigned char *pKeySet, unsigned char *dst_file_path, int keyLength)
{
	unsigned char *output_tmpBuff = NULL;
	unsigned char iv[16];
	unsigned int key[4];
	H2status rc;
	output_tmpBuff = kcalloc(1024, sizeof(unsigned char), GFP_KERNEL);
	if (output_tmpBuff == NULL) {
		return H2_ERROR;
	}
	memset(output_tmpBuff, 0, 1024);
	if (pKeySet == NULL || dst_file_path == NULL)
		return H2_ERROR;
	spu_SetLc128(pKeySet);
	spu_SetCertRx(&pKeySet[16]);
	spu_SetKPrivRx(&pKeySet[16 + 522]);
	key[0] = HDCP2X_AES_KEY_0;
	key[1] = HDCP2X_AES_KEY_1;
	key[2] = HDCP2X_AES_KEY_2;
	key[3] = HDCP2X_AES_KEY_3;
	memset(iv, 0, sizeof(iv));
	RCP_AES_CBC_Encryption((unsigned char *)key, (unsigned char *)iv, pKeySet, output_tmpBuff, keyLength);

	 rc = write_binary_file(dst_file_path, output_tmpBuff, keyLength);
	if (output_tmpBuff) {
		kfree(output_tmpBuff);
		output_tmpBuff = NULL;
	}
	return rc;
}

H2status hdcp2_load_encrypted_keyset(unsigned char *pKeySet, unsigned char *src_file_path, int keyLength)
{
	unsigned char *tmpBuff = NULL;
	unsigned char iv[16];
	unsigned int key[4];
	tmpBuff = kcalloc(1024, sizeof(unsigned char), GFP_KERNEL);
	if (tmpBuff == NULL) {
		return H2_ERROR;
	}
	memset(tmpBuff, 0, 1024);
	if (pKeySet == NULL || src_file_path == NULL)
		return H2_ERROR;
	key[0] = HDCP2X_AES_KEY_0;
	key[1] = HDCP2X_AES_KEY_1;
	key[2] = HDCP2X_AES_KEY_2;
	key[3] = HDCP2X_AES_KEY_3;
	memset(iv, 0, sizeof(iv));
	read_binary_file(src_file_path, tmpBuff, keyLength);

	RCP_AES_CBC_Decryption((unsigned char *)key, (unsigned char *)iv, tmpBuff, pKeySet, keyLength);
	if (tmpBuff) {
		kfree(tmpBuff);
		tmpBuff = NULL;
	}
	return H2_OK;
}

void hdcp2_save_keyset(unsigned char *pKeySet)
{
	spu_SetLc128(pKeySet);
	spu_SetCertRx(&pKeySet[16]);
	spu_SetKPrivRx(&pKeySet[16 + 522]);
}

#define HDCP22_len	878
void hdcp2_load_from_flash()
{
	unsigned char hdcp22_buf[HDCP22_len];
	if(0)
	{
		pr_notice("hdcp22 get key form otp...............\n");
		spu_SetLc128(hdcp22_buf);
		spu_SetCertRx(&hdcp22_buf[16]);
		spu_SetKPrivRx(&hdcp22_buf[16+522]);
	}
}

H2status hdcp2_load_from_file(unsigned char *pKeySet, unsigned char *src_file_path, int keyLength)
{
	hdcp2_load_encrypted_keyset(pKeySet, src_file_path, keyLength);

	/* save data in SRAM....  */
	spu_SetLc128(pKeySet);
	spu_SetCertRx(&pKeySet[16]);
	spu_SetKPrivRx(&pKeySet[16 + 522]);
	return H2_OK;
}

H2status hdcp2_Verify_EdkeyKs(unsigned char *pEks, unsigned char *rrx, unsigned char *rtx, unsigned char *rn, int modeHDCP22)
{
	unsigned char Dkey2[DKEY_SIZE];
	unsigned char ks[KS_SIZE];
	unsigned char temp[DKEY_SIZE];
	unsigned char rrx_temp[DKEY_SIZE];
	hdcp2_Rx_GetKs(ks, KS_SIZE);
	if (rn == NULL || rtx == NULL || rrx == NULL || ks == NULL)
		return H2_ERROR;
	RCP_HDCP2_GenDKey(SRAM_KM_ENTRY, rtx, rrx, rn, 2, SRAM_DK2_ENTRY, modeHDCP22);
	spu_GetDKey2(Dkey2);

	memset(rrx_temp, 0, sizeof(rrx_temp));
	memcpy(rrx_temp + DKEY_SIZE - RRX_SIZE, rrx, RRX_SIZE);
	crypt_xor(rrx_temp, Dkey2, temp, DKEY_SIZE);
	crypt_xor(temp, ks, pEks, DKEY_SIZE);

	return H2_OK;
}

H2status hdcp2_GenKsXorLc128()
{
	unsigned char lc[LC128_SIZE];
	unsigned char ks[KS_SIZE];
	unsigned char ksxorLc[16];
	spu_GetLc128(lc);
	hdcp2_Rx_GetKs(ks, KS_SIZE);
	xor_array(lc, ks, ksxorLc, 16);
	spu_SetKsXorLc128(ksxorLc);
	return H2_OK;
}

H2status hdcp2_copy_paramter(HDCP2_PM_PARAMTER *hdcp2_data)
{
	if (hdcp2_data == NULL)
		return H2_ERROR;
	memcpy((unsigned char *)hdcp2_data, (unsigned char *)(&SessionSecrets_Rx), sizeof(SessionSecrets_Rx));
	spu_GetLc128(hdcp2_data->lc128);
	spu_GetCertRx(hdcp2_data->certRx);
	spu_GetKPrivRx(hdcp2_data->kPrivRx);
	spu_GetKH(hdcp2_data->kh);
	spu_GetKM(hdcp2_data->km);
	spu_GetKD(hdcp2_data->kd);
	spu_GetDKey2(hdcp2_data->dkey2);
	spu_GetKsXorLc128(hdcp2_data->ksXorLc128);
	spu_GetRiv(hdcp2_data->riv);
	return H2_OK;
}

H2status hdcp2_recover_paramter(HDCP2_PM_PARAMTER *hdcp2_data)
{
	if (hdcp2_data == NULL)
		return H2_ERROR;
	memcpy((unsigned char *)(&SessionSecrets_Rx), (unsigned char *)hdcp2_data, sizeof(SessionSecrets_Rx));
	spu_SetLc128(hdcp2_data->lc128);
	spu_SetCertRx(hdcp2_data->certRx);
	spu_SetKPrivRx(hdcp2_data->kPrivRx);
	spu_SetKH(hdcp2_data->kh);
	spu_SetKM(hdcp2_data->km);
	spu_SetKD(hdcp2_data->kd);
	spu_SetDKey2(hdcp2_data->dkey2);
	spu_SetKsXorLc128(hdcp2_data->ksXorLc128);
	spu_SetRiv(hdcp2_data->riv);
	return H2_OK;
}

#endif
