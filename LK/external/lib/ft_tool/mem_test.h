#ifndef _MEM_TEST_H_
#define _MEM_TEST_H_

/* Define Data type */
typedef unsigned int	UINT32;
typedef unsigned short	UINT16;
typedef unsigned char	UINT8;
typedef unsigned char	BOOLEAN;

/* macro definition */
#define Rreg32(Addr)				rtd_inl(Addr)
#define Wreg32(Addr, Value)			rtd_outl(Addr, Value)
#define Wreg32_M(Addr, Mask, Value)	rtd_maskl(Addr, Mask, Value)

/* CRT register */
#define REG_CRT							0x98000000
#define SOFT_RESET1						REG_CRT

/* DC_SYS register */
#define REG_DC_SYS						0x98008000
#define DC_MT_SADDR(x)					(REG_DC_SYS + 0x200 + 0x4 * x)
#define DC_MT_EADDR(x)					(REG_DC_SYS + 0x220 + 0x4 * x)
#define DC_MT_MODE(x)					(REG_DC_SYS + 0x240 + 0x4 * x)
#define DC_MT_TABLE(x)					(REG_DC_SYS + 0x260 + 0x4 * x)
#define DC_MT_ADDCMD_HI(x)				(REG_DC_SYS + 0x280 + 0x4 * x)	// burst length[7:0]
#define DC_MT_ADDCMD_LO_0(x)			(REG_DC_SYS + 0x2A0 + 0x4 * x)	// addr[31:2], rw[1]
#define DC_MT_ADDCMD_LO_1(x)			(REG_DC_SYS + 0x2C0 + 0x4 * x)
#define DC_mem_scramble_ctrl			(REG_DC_SYS + 0x404)
#define DC_mem_scramble_saddr(x)		(REG_DC_SYS + 0x478 * (!(x >> 3)) + 0x700 * (x >> 3) + 0x08 * (x & 7))
#define DC_mem_scramble_eaddr(x)		(REG_DC_SYS + 0x47c * (!(x >> 3)) + 0x704 * (x >> 3) + 0x08 * (x & 7))
#define DC_mem_scramble_swap_0			(REG_DC_SYS + 0x4E0)
#define DC_mem_scramble_swap_1			(REG_DC_SYS + 0x4E4)
#define DC0_mem_scramble_keyA(x, y)		(REG_DC_SYS + 0x600 + 0x10 * x + 0x04 * y) //region x, key y
#define DC0_mem_scramble_keyB(x, y)		(REG_DC_SYS + 0x770 + 0x08 * x + 0x04 * y) 
#define DC1_mem_scramble_keyA(x, y)		(REG_DC_SYS + 0x900 + 0x10 * x + 0x04 * y) //region x, key y
#define DC1_mem_scramble_keyB(x, y)		(REG_DC_SYS + 0x7f0 + 0x08 * x + 0x04 * y)

/* MD register */
#define REG_MD							0x9800B000
#define MD_SMQ_CNTL						REG_MD
#define MD_SMQ_INT_STATUS				(REG_MD+0x4)
#define MD_SMQBase						(REG_MD+0xC)
#define MD_SMQLimit						(REG_MD+0x10)
#define MD_SMQRdptr						(REG_MD+0x14)
#define MD_SMQWrptr						(REG_MD+0x18)

/* 256 bit SHA256 initial vector */
#define SHA256_H0						0x6A09E667
#define SHA256_H1						0xBB67AE85
#define SHA256_H2						0x3C6EF372
#define SHA256_H3						0xA54FF53A
#define SHA256_H4						0x510E527F
#define SHA256_H5						0x9B05688C
#define SHA256_H6						0x1F83D9AB
#define SHA256_H7						0x5BE0CD19

/* MCP General Registers */
#define  REG_CP							0x98015000
#define  K_MCP_CTRL						(REG_CP + 0x900)
#define  K_MCP_STATUS					(REG_CP + 0x904)
#define  K_MCP_EN						(REG_CP + 0x908)

/* MCP Ring-Buffer Registers */
#define  K_MCP_BASE						(REG_CP + 0x90c)
#define  K_MCP_LIMIT					(REG_CP + 0x910)
#define  K_MCP_RDPTR					(REG_CP + 0x914)
#define  K_MCP_WRPTR					(REG_CP + 0x918)
#define  K_MCP_DES_COUNT				(REG_CP + 0x934)
#define  K_MCP_DES_COMPARE				(REG_CP + 0x938)

typedef struct md_descriptor {
	UINT32 mode;
	UINT32 Daddr;
	UINT32 Saddr;
	UINT32 Length;
} __attribute__((aligned(4))) t_md_descriptor;

typedef struct mcp_descriptor
{
	UINT32 mode;
	UINT32 key[6];
	UINT32 ini_key[4];
	UINT32 src_addr;
	UINT32 dst_addr;
	UINT32 length;
} __attribute__((aligned(8))) t_mcp_descriptor;

/* DC_SYS Scramble Information */
#define DC_SCRAMBLE_IV0					(0x1fa0647c)
#define DC_SCRAMBLE_IV1					(0xaf128374) 
#define DC_SCRAMBLE_SWAP1				(0x9bfd0426)
#define DC_SCRAMBLE_SWAP0				(0xa8ce1573)
#define DC_SCRAMBLE_SWAP1_inv			(0xd4c5e7f6)   
#define DC_SCRAMBLE_SWAP0_inv			(0x182a093b)

enum {
	AES = 0x0,
	FAST_MODE = 0x1
};

/* DDR Test Information */
#define TST_ADDR			0x01500000
#define PAT_NUM				0x100
#define MD_DES_ADDR			0x1000000
#define MD_LENGTH			(0x100000 / PAT_NUM)
#define MD_START_ADDR		0x15000000
#define MD_DST_ADDR			0x16000000
#define CP_HASH_ADDR		0x2000000
#define CP_LENGTH			0xFFFFF

/* Golden Hash */
UINT32 Gcmd[] = { 0x148b0c9d, 0xc2da5e75, 0x2f53e2c1, 0x77fdb302, 0x1410eb36, 0x40a487f8, 0x61714769, 0x9d180c2 };
UINT32 Gdata[] = { 0x39e0189b, 0x2530a8e0, 0x80fea502, 0x5723d826, 0xbf9e4ad8, 0x95fe29c0, 0x7e8ababc, 0x17105dd9 };

#endif	/* _MEM_TEST_H_ */
