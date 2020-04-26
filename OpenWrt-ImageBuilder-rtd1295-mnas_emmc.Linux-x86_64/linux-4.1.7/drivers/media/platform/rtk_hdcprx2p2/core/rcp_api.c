/***************************************************************************************************
  File        : rcp_api.cpp
  Description : Low Level API for RCP
  Author      : Kevin Wang
****************************************************************************************************
    Update List :
----------------------------------------------------------------------------------------------------
    20090605    | Create Phase
***************************************************************************************************/
#include <linux/kernel.h>
#include <linux/dma-mapping.h>

#include <hdcp2_hal.h>
#include <rcp_api.h>

#include <linux/platform_device.h>
extern struct platform_device* hdmirx_pdev;

extern int MCP_AES_Encryption(
    unsigned char           mode,
    unsigned char           key[16],
    unsigned char           iv[16],
    unsigned char*          p_in,
    unsigned char*          p_out,
    unsigned long           len
);

#ifdef USE_RTK_DEV_MEM
#define DEV_MEM     "/dev/rtkmem"
#else /*  */
#define DEV_MEM     "/dev/mem"
#endif /*  */

static int m_fd=0;

static void __iomem *p_reg_key;
static void __iomem *p_reg_data_in;
static void __iomem *p_reg_data_out;
static void __iomem *p_reg_iv;
static void __iomem *p_reg_set;
static void __iomem *p_tp_key_info;
static void __iomem *p_tp_key_ctrl;

static unsigned char rtp_cw[4][64];
static int rcp_in_use;
static int rcp_cw_valid;

#define rcp_wr_reg(x,y)		writel(x,(void __iomem*)(y))
#define rcp_rd_reg(x)		readl((void __iomem*)(x))

#define S_OK 0
#define S_FALSE -1
#define MAX_CW_ENTRY       128

#define RCP_INFO(fmt, args...)          pr_debug("[RCP] Info, " fmt, ## args)
#define RCP_WARNING(fmt, args...)       pr_debug("[RCP] Warning, " fmt, ## args)

void RCP_HDCP2_GenKd(int KmCw, unsigned char *Rtx, unsigned char *Rrx, unsigned char *Rn, int KdCW, int modeHDCP22)
{
	RCP_HDCP2_GenDKey(KmCw, Rtx, Rrx, Rn, 0, KdCW, modeHDCP22);
	RCP_HDCP2_GenDKey(KmCw, Rtx, Rrx, Rn, 1, KdCW + 2, modeHDCP22);
}

/*======================================================================
 * Func : RCP_Init
 *
 * Desc : Init RCP module
 *
 * Parm : N/A
 *
 * Retn : S_OK /  S_FALSE
 *======================================================================*/
 #define HDCP2_RCP_REG_BASE   0xB8014000
 #define HDCP2_RCP_REG_KEY_OFFSET 0x1034
 #define HDCP2_RCP_REG_DATA_IN_OFFSET 0x1044
 #define HDCP2_RCP_REG_DATA_OUT_OFFSET 0x1054
 #define HDCP2_RCP_REG_IV_OFFSET 0x10d0
 #define HDCP2_RCP_REG_SET_OFFSET 0x1074
 #define HDCP2_RCP_REG_TP_KEY_INFO_OFFSET 0x118
 #define HDCP2_RCP_REG_TP_KEY_CTRL_OFFSET 0x120
int RCP_Init()
{
	if (m_fd)
		return S_OK;

	p_reg_key = ioremap(0x98015834, 0x10);
	p_reg_data_in = ioremap(0x98015844, 0x10);
	p_reg_data_out = ioremap(0x98015854, 0x10);
	p_reg_iv = ioremap(0x98015864, 0x10);
	p_reg_set = ioremap(0x98015874, 0x10);
	p_tp_key_info = ioremap(0x98014058, 0x10);
	p_tp_key_ctrl = ioremap(0x98014060, 0x10);

	memset(rtp_cw, 0, sizeof(rtp_cw));
	rcp_in_use = 0;
	rcp_cw_valid = 0;
	m_fd = 1;
	RCP_INFO("Init RCP successfully\n");
	return S_OK;

}

/*======================================================================
 * Func : UnInit
 *
 * Desc : Uninit RCP module
 *
 * Parm : N/A
 *
 * Retn : S_OK /  S_FALSE
 *======================================================================*/
void RCP_UnInit()
{
	m_fd = 0;
	p_reg_key = NULL;
	p_reg_data_in = NULL;
	p_reg_data_out = NULL;
	p_reg_iv = NULL;
	p_reg_set = NULL;
	p_tp_key_info = NULL;
	p_tp_key_ctrl = NULL;
	rcp_in_use = 0;
	rcp_cw_valid = 0;
}

unsigned int B4TL(unsigned char b1,unsigned char b2,unsigned char b3,unsigned char b4)
{
	unsigned int ret_val;
	unsigned int b1_tmp,b2_tmp,b3_tmp;
	b1_tmp = b1;
	b2_tmp = b2;
	b3_tmp = b3;
	ret_val = (b1_tmp<<24)+(b2_tmp<<16)+(b3_tmp<<8)+b4;
	return ret_val;
}

void LTB4(unsigned char *b1, unsigned char *b2, unsigned char *b3, unsigned char *b4, void __iomem *L)
{
	*b1 = ((rcp_rd_reg(L)>>24));
	*b2 = ((rcp_rd_reg(L)>>16));
	*b3 = ((rcp_rd_reg(L) >> 8));
	*b4 = (rcp_rd_reg(L));
}

void B16TL4(void __iomem *L, unsigned char *B)
{
	rcp_wr_reg(B4TL(B[0], B[1], B[2], B[3]), L);
	rcp_wr_reg(B4TL(B[4], B[5], B[6], B[7]), L+0x4);
	rcp_wr_reg(B4TL(B[8], B[9], B[10], B[11]), L+0x8);
	rcp_wr_reg(B4TL(B[12], B[13], B[14], B[15]), L+0xc);
}

void L4TB16(unsigned char *B,void __iomem *L)
{
	LTB4(&B[0], &B[1], &B[2], &B[3], L);
	LTB4(&B[4], &B[5], &B[6], &B[7], L+0x4);
	LTB4(&B[8], &B[9], &B[10], &B[11], L+0x8);
	LTB4(&B[12], &B[13], &B[14], &B[15], L+0xc);
}

void xor_array(unsigned char *in1, unsigned char *in2, unsigned char *out, unsigned long len)
{
	int i;
	for (i = 0; i < len; i++)
		out[i] = in1[i] ^ in2[i];
}

/*======================================================================
 * Func : RCP_AES_ECB_Cipher
 *
 * Desc :
 *
 * Parm : N/A
 *
 * Retn : S_OK /  S_FALSE
 *======================================================================*/
int RCP_AES_ECB_Cipher(unsigned char EnDe, unsigned char Key[16],
				unsigned char *pDataIn, unsigned char *pDataOut,
				unsigned long DataLen)
{
	unsigned long out_len = 0;
	RCP_Init();
	while (rcp_in_use)
		;
	rcp_in_use = 1;
	rcp_wr_reg((EnDe) ? RCP_AES_128_ECB_ENC : RCP_AES_128_ECB_DEC, p_reg_set);
	rcp_wr_reg(0, p_reg_set+0x4);
	rcp_wr_reg(0, p_reg_set+0x8);
	if (Key == NULL) {
		rcp_wr_reg(REG_SET1_KEY_MODE_OTP, p_reg_set+0x4);
	} else if ((unsigned long)Key <= MAX_CW_ENTRY) {
		rcp_wr_reg(REG_SET1_KEY_MODE_CW, p_reg_set+0x4);
		rcp_wr_reg(REG_SET2_KEY_ENTRY((unsigned long)(Key - 1)), p_reg_set+0x8);
	} else
		B16TL4(p_reg_key, Key);
	while (DataLen >= 16) {
		B16TL4(p_reg_data_in, pDataIn);
		L4TB16(pDataOut, p_reg_data_out);
		pDataIn += 16;
		pDataOut += 16;
		DataLen -= 16;
		out_len += 16;
		rcp_wr_reg(rcp_rd_reg(p_reg_set)&(~REG_SET_FIRST_128), p_reg_set);
	}
	rcp_in_use = 0;
	return out_len;
}

/*======================================================================
 * Func : RCP_AES_CBC_Cipher
 *
 * Desc :
 *
 * Parm : N/A
 *
 * Retn : S_OK /  S_FALSE
 *======================================================================*/
int RCP_AES_CBC_Cipher(unsigned char EnDe, unsigned char Key[16],
				unsigned char IV[16], unsigned char *pDataIn,
				unsigned char *pDataOut, unsigned long DataLen)
{
	unsigned long out_len = 0;
	if (IV == NULL) {
		RCP_WARNING("Run RCP_AES_CBC_Cipher failed, IV should not be NULL \n");
		return 0;
	}
	RCP_Init();
	while (rcp_in_use)
		msleep(1);
	rcp_in_use = 1;
	rcp_wr_reg((EnDe) ? RCP_AES_128_CBC_ENC : RCP_AES_128_CBC_DEC, p_reg_set);
	rcp_wr_reg(0, p_reg_set+0x4);
	rcp_wr_reg(0, p_reg_set+0x8);
	if (Key == NULL) {
		rcp_wr_reg(REG_SET1_KEY_MODE_OTP, p_reg_set+0x4);
	} else if ((unsigned long)Key <= MAX_CW_ENTRY) {
		rcp_wr_reg(REG_SET1_KEY_MODE_CW, p_reg_set+0x4);
		rcp_wr_reg(REG_SET2_KEY_ENTRY((unsigned long)(Key - 1)), p_reg_set+0x8);
	} else
		B16TL4(p_reg_key, Key);
	B16TL4(p_reg_iv, IV);
	while (DataLen >= 16) {
		B16TL4(p_reg_data_in, pDataIn);
		L4TB16(pDataOut, p_reg_data_out);
		pDataIn += 16;
		pDataOut += 16;
		DataLen -= 16;
		out_len += 16;
		rcp_wr_reg(rcp_rd_reg(p_reg_set)&(~REG_SET_FIRST_128), p_reg_set);
	}
	rcp_in_use = 0;
	return out_len;
}

void aes_ctr_cnt_add(unsigned char cnt[16])
{
	unsigned char ov = 0;
	int i = 15;
	do {
		cnt[i]++;
		ov = (cnt[i]) ? 0 : 1;
	} while (i-- >= 0 && ov);
}

/*======================================================================
 * Func : RCP_AES_CTR_Cipher
 *
 * Desc :
 *
 * Parm : N/A
 *
 * Retn : 0 : success, others fail
 *======================================================================*/
int RCP_AES_CTR_Cipher(unsigned char Key[16], unsigned char IV[16],
				unsigned char *pDataIn, unsigned char *pDataOut,
				unsigned long DataLen)
{
	int ret_val;
	unsigned long data_dma_vaddr;
	dma_addr_t data_dma_phyaddr;
	unsigned char *pData = NULL;

	data_dma_vaddr = (unsigned long)dma_alloc_coherent(&hdmirx_pdev->dev, DataLen, &data_dma_phyaddr, GFP_ATOMIC);
	pData = (unsigned char *)data_dma_vaddr;
	memcpy(pData, pDataIn, DataLen);
	ret_val = MCP_AES_Encryption( 2, Key, IV, (unsigned char *)data_dma_phyaddr, (unsigned char *)data_dma_phyaddr, DataLen);
	memcpy(pDataOut, pData, DataLen);
	dma_free_coherent(&hdmirx_pdev->dev, DataLen, (void *)data_dma_vaddr, data_dma_phyaddr);
	return ret_val;
}

void RCP_WRITE_SRAM(unsigned int id, unsigned char data[8])
{
	RCP_Init();
	rcp_wr_reg(B4TL(data[0], data[1], data[2], data[3]), p_tp_key_info);
	rcp_wr_reg(B4TL(data[4], data[5], data[6], data[7]), p_tp_key_info+0x4);
	rcp_wr_reg(id|0x80, p_tp_key_ctrl);
	rcp_wr_reg(0, p_tp_key_info);
	rcp_wr_reg(0, p_tp_key_info+0x4);
}

void RCP_READ_SRAM(unsigned int id, unsigned char data[8])
{
#if 0
	/* in security modem the cw is un-readable, so we have to find an otherway to fetch it data...*/
	RCP_Init();
	p_reg_set[0] = RCP_AES_128_ECB_ENC;
	p_reg_set[1] = REG_SET1_KEY_MODE_CW | REG_SET1_INPUT_MODE_CW;
	p_reg_set[2] = REG_SET2_KEY_ENTRY(id) | REG_SET2_INPUT_ENTRY(id);
	B16TL4(p_reg_data_in, tmp);
	L4TB16(tmp, p_reg_data_out);
	p_reg_set[0] = RCP_AES_128_ECB_DEC;
	p_reg_set[1] = REG_SET1_KEY_MODE_CW;
	p_reg_set[2] = REG_SET2_KEY_ENTRY(id);
	B16TL4(p_reg_data_in, tmp);
	L4TB16(tmp, p_reg_data_out);
	memcpy(data, tmp, 8);
#else /*  */
	RCP_Init();
	rcp_wr_reg(id, p_tp_key_ctrl);
	LTB4(&data[0], &data[1], &data[2], &data[3], p_tp_key_info);
	LTB4(&data[4], &data[5], &data[6], &data[7], p_tp_key_info+0x4);
	rcp_wr_reg(0, p_tp_key_info);
	rcp_wr_reg(0, p_tp_key_info+0x4);
#endif /*  */
}

void RCP_HDCP2_GenDKey(int KmCw, unsigned char *Rtx, unsigned char *Rrx,
				unsigned char *Rn, unsigned char ctr, int DKeyCW,
				int modeHDCP22)
{
	unsigned char aes_key[16],aes_iv[16],tmp8[8];
	unsigned char km_tmp[16];
	unsigned char dkey_tmp[16];
	unsigned long data_dma_vaddr;
	dma_addr_t data_dma_phyaddr;
	unsigned char *pData = NULL;

	/* AES KEY = Km^Rn */
	memset(aes_key, 0, 16);
	memcpy(&aes_key[8], Rn, 8);
	RCP_GET_CW(KmCw, km_tmp, 16);
	xor_array(km_tmp, aes_key, aes_key, 16);

	/* rrx^ctr */
	memset(tmp8, 0, 8);
	tmp8[7] = ctr;
	xor_array(Rrx, tmp8, tmp8, 8);

	/* AES IV = rtx||(rrx^ctr) */
	memset(aes_iv, 0, 16);
	memcpy(aes_iv, Rtx, 8);
	memcpy(aes_iv + 8, tmp8, 8);

	/* Generate dkey*/
	data_dma_vaddr = (unsigned long)dma_alloc_coherent(&hdmirx_pdev->dev, 16, &data_dma_phyaddr, GFP_ATOMIC);
	pData = (unsigned char *)data_dma_vaddr;
	memset(pData, 0, 16);// AES DataIn -> 0
	MCP_AES_Encryption( 2, aes_key, aes_iv, (unsigned char *)data_dma_phyaddr, (unsigned char *)data_dma_phyaddr, 16);

	memset(dkey_tmp, 0, 16);
	xor_array(pData, dkey_tmp, dkey_tmp, 16);// Dkey = 0 ^ AES DataOut
	dma_free_coherent(&hdmirx_pdev->dev, 16, (void *)data_dma_vaddr, data_dma_phyaddr);

	/* Set dkey*/
	RCP_SET_CW(DKeyCW, dkey_tmp, 16);

}

void RCP_HDCP2_DataDecrypt(int KsXorLs128CW,  int DatadecryptoCW, unsigned char *key, unsigned char *pcounter)
{
	RCP_SET_CW(DatadecryptoCW, key, 16);

	rcp_wr_reg(RCP_AES_128_ECB_ENC, p_reg_set);
	rcp_wr_reg(REG_SET1_KEY_MODE_CW | REG_SET1_OUTPUT_MODE_CW, p_reg_set+0x4);
	rcp_wr_reg(REG_SET2_KEY_ENTRY(DatadecryptoCW) | REG_SET2_OUTPUT_ENTRY(DatadecryptoCW), p_reg_set+0x8);
	B16TL4(p_reg_data_in, pcounter);
	L4TB16(pcounter, p_reg_data_out);
}

int RCP_SET_CW(unsigned int id, unsigned char *pCW, unsigned int len)
{
	while (len) {
		if (len < 8) {
			unsigned char sram_data[8];
			memcpy(sram_data, pCW, len);
			RCP_WRITE_SRAM(id++, sram_data);
			break;
		}
		RCP_WRITE_SRAM(id++, pCW);
		pCW += 8;
		len -= 8;
	}
	return 0;
}

int RCP_GET_CW(unsigned int id, unsigned char *pCW, unsigned int len)
{
	while (len) {
		if (len < 8) {
			unsigned char sram_data[8];
			RCP_READ_SRAM(id++, sram_data);
			memcpy(pCW, sram_data, len);
			break;
		}
		RCP_READ_SRAM(id++, pCW);
		pCW += 8;
		len -= 8;
	}
	return 0;
}

void RCP_CW_XOR(int CwIn1, int CwIn2, int CwOut)
{
	unsigned char tmp[16];

	/* CW XOR : ~AES_ECB(AES_CBC(In1, In2, K),K) = ~AES_ECB(AES_ECB(In1 ^ In2, K)) = In1 ^ In2*/
	rcp_wr_reg(RCP_AES_128_CBC_ENC, p_reg_set);
	rcp_wr_reg(REG_SET1_IV_MODE_CW | REG_SET1_INPUT_MODE_CW | REG_SET1_OUTPUT_MODE_CW, p_reg_set+0x4);
	rcp_wr_reg(REG_SET2_IV_ENTRY(CwIn2) | REG_SET2_INPUT_ENTRY(CwIn1) | REG_SET2_OUTPUT_ENTRY(CwOut), p_reg_set+0x8);

	rcp_wr_reg(0, p_reg_key);
	rcp_wr_reg(0, p_reg_key+0x4);
	rcp_wr_reg(0, p_reg_key+0x8);
	rcp_wr_reg(0, p_reg_key+0xc);
	rcp_wr_reg(0, p_reg_data_in+0xc);
	L4TB16(tmp, p_reg_data_out);
	rcp_wr_reg(RCP_AES_128_ECB_DEC, p_reg_set);
	rcp_wr_reg(REG_SET1_INPUT_MODE_CW | REG_SET1_OUTPUT_MODE_CW, p_reg_set+0x4);
	rcp_wr_reg(REG_SET2_INPUT_ENTRY(CwOut) | REG_SET2_OUTPUT_ENTRY(CwOut), p_reg_set+0x8);

	rcp_wr_reg(0, p_reg_data_in+0xc);
	L4TB16(tmp, p_reg_data_out);
}

void RCP_HDCP2_EkhKm(int KhCw, int KmCw, unsigned char *Rtx,
				unsigned char *pEkhKm)
{
	memset(pEkhKm, 0, 16);
	memcpy(pEkhKm, Rtx, 8);
	rcp_wr_reg(RCP_AES_128_ECB_ENC, p_reg_set);
	rcp_wr_reg(REG_SET1_KEY_MODE_CW, p_reg_set+0x4);
	rcp_wr_reg(REG_SET2_KEY_ENTRY(KhCw), p_reg_set+0x8);
	B16TL4(p_reg_data_in, pEkhKm);
	L4TB16(pEkhKm, p_reg_data_out);
	B16TL4(p_reg_data_in, pEkhKm);
	L4TB16(pEkhKm, p_reg_data_out);
	rcp_wr_reg(RCP_AES_128_CBC_DEC, p_reg_set);
	rcp_wr_reg(REG_SET1_KEY_MODE_CW | REG_SET1_IV_MODE_CW, p_reg_set+0x4);
	rcp_wr_reg(REG_SET2_KEY_ENTRY(KhCw) | REG_SET2_IV_ENTRY(KmCw), p_reg_set+0x8);
	B16TL4(p_reg_data_in, pEkhKm);
	L4TB16(pEkhKm, p_reg_data_out);
}

void RCP_HDCP2_GenKs(int dKey2CW, unsigned char *EdKeyKs,
				unsigned char *rRx, int KsCW)
{
	unsigned char tmp[16];
	unsigned char dkey2_tmp[16];
	unsigned char ks_tmp[16];
	memcpy(tmp, EdKeyKs, 16);
	xor_array(&tmp[8], rRx, &tmp[8], 8);
	RCP_SET_CW(KsCW, tmp, 16);
#if 1
	RCP_GET_CW(dKey2CW, dkey2_tmp, 16);
	RCP_GET_CW(KsCW, ks_tmp, 16);
	xor_array(dkey2_tmp, ks_tmp, ks_tmp, 16);
	RCP_SET_CW(KsCW, ks_tmp, 16);
#endif /*  */
}

void RCP_HDCP2_GenKsXorLc128(int Lc128Cw, int KsCW, int KsXorLc128CW)
{
	RCP_CW_XOR(Lc128Cw, KsCW, KsXorLc128CW);
}

