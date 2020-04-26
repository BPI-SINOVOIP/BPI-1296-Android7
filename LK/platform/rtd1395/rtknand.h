#ifndef __RTKNAND_H__
#define __RTKNAND_H__

#define NF_REG_BASE		0x98010000	// for SWC usage.
#define NF_REG1_BASE	0x98010000  

#define	NF_PAGE_ADR0	(NF_REG_BASE + 0)
#define	NF_PAGE_ADR1	(NF_REG_BASE + 0x4)
#define	NF_PAGE_ADR2	(NF_REG_BASE + 0x8)		//ADDR[20:16] = [4:0]
#define	NF_PAGE_ADR3	(NF_REG_BASE + 0x2c)
#define	NF_COL_ADR0		(NF_REG_BASE + 0xc)
#define	NF_COL_ADR1		(NF_REG_BASE + 0x3c)

#define	NF_ADR_MODE		(NF_REG_BASE + 0x8)		//[7:5]
#define	NF_DATA			(NF_REG_BASE + 0x14)
#define	NF_ND_CTL		(NF_REG_BASE + 0x18)

#define	NF_DATA_TL0		(NF_REG_BASE + 0x100)
#define	NF_DATA_TL1		(NF_REG_BASE + 0x104)

#define	NF_CMD1			(NF_REG_BASE + 0x10)
#define	NF_CMD2			(NF_REG_BASE + 0x274)
#define	NF_CMD3			(NF_REG_BASE + 0x28)

#define NF_ECC_STATE	(NF_REG_BASE + 0x38)
#define NF_RSECC_NUM	(NF_REG_BASE + 0x204)
#define	NF_RND_CMD1		(NF_REG_BASE + 0x208)
#define	NF_RND_CMD2		(NF_REG_BASE + 0x20c)
#define	NF_RND_DATASTR_COL_H	(NF_REG_BASE + 0x210)
#define	NF_RND_SPR_STR_COL_H	(NF_REG_BASE + 0x214)
#define	NF_RND_SPR_STR_COL_L	(NF_REG_BASE + 0x218)
#define	NF_RND_RW_EN	(NF_REG_BASE + 0x21c)

#define NF_RMZ_CTRL		(NF_REG_BASE + 0x240)

#define	NF_ECC_STOP		(NF_REG_BASE + 0x264)
#define	NF_ECC_PAGE		(NF_REG_BASE + 0x268)
#define	NF_MULTI_CH_MODE	(NF_REG_BASE + 0x27c)

#define	NF_PAGE_CNT		(NF_REG_BASE + 0x26c)
#define	NF_PAGE_LEN		(NF_REG_BASE + 0x270)

#define NF_MAX_ECC_NUM	(NF_REG_BASE + 0x22c)

#define NF_T3			(NF_REG_BASE + 0x234)
#define NF_T2			(NF_REG_BASE + 0x238)
#define NF_T1			(NF_REG_BASE + 0x23c)

#define NF_PP_CTL1		(NF_REG_BASE + 0x12c)	//ADDR[7:0]
#define NF_PP_CTL0		(NF_REG_BASE + 0x110)	//ADDR[9:8] = [5:4]
#define NF_PP_RDY		(NF_REG_BASE + 0x228)

#define NF_LOW_PWR		(NF_REG_BASE + 0x13c)

#define NF_AUTO_TRIG	(NF_REG_BASE + 0x200)
#define NF_POLL_STATUS	(NF_REG_BASE + 0x30)

#define NF_SRAM_CTL		(NF_REG_BASE + 0x300)
#define NF_DMA1_CTL		(NF_REG_BASE + 0x10c)
#define	NF_DMA_ADR		(NF_REG_BASE + 0x304) //? DMA_CTRL1
#define	NF_DMA_LEN		(NF_REG_BASE + 0x308) //? DMA_CTRL2
#define	NF_DMA_CONF		(NF_REG_BASE + 0x30c) //? DMA_CTRL3
#define	NF_SYS_LOW_PWR	(NF_REG_BASE + 0x310) //? SYS_LOW_PWR

#if 0
#define NF_DES_CMD		(NF_REG_BASE + 0x314)
#define NF_DES_MODE		(NF_REG_BASE + 0x318)
#define NF_DES_USER		(NF_REG_BASE + 0x31c)
#define	NF_DES_BASE		(NF_REG_BASE + 0x330)
#define	NF_DES_LIMIT	(NF_REG_BASE + 0x334)
#define	NF_DES_WP		(NF_REG_BASE + 0x338)
#define	NF_DES_RP		(NF_REG_BASE + 0x33c)
#define	NF_DES_CNT		(NF_REG_BASE + 0x340)
#endif
#define	NF_DBU			(NF_REG_BASE + 0x344)
#define	NF_SPR_DDR_CTL	(NF_REG_BASE + 0x348)

#define	NF_CHIP_EN		(NF_REG_BASE + 0x130) //?

#define	NF_USER_MASK	(NF_REG_BASE + 0x320) //?

#define	NF_ISR			(NF_REG_BASE + 0x324)
#define	NF_ISREN		(NF_REG_BASE + 0x328)
#define NF_NWC_DUMMY_SYS (NF_REG1_BASE + 0x32c)
#define NF_SWC_DUMMY_SYS (NF_REG1_BASE + 0xF32c)

#define NF_DELAY_CNT	(NF_REG_BASE + 0x260)

#define NF_BLANK_CHK	(NF_REG_BASE + 0x34)
#define NF_ECC_SEL		(NF_REG_BASE + 0x128)
#define NF_CP_LEN		(NF_REG_BASE + 0x34c)

#define REG_RMZ_CTRL	(NF_REG_BASE + 0x240)
#define REG_RMZ_SEED_L	(NF_REG_BASE + 0x244)
#define REG_RMZ_SEED_H	(NF_REG_BASE + 0x248)
#define REG_RMZ_SEED_CTRL	(NF_REG_BASE + 0x24c)

#define CP_NF_SET		(NF_REG_BASE + 0x50a4)

#define CP_NF_INI_KEY_0	(NF_REG_BASE + 0x5094)
#define CP_NF_INI_KEY_1	(NF_REG_BASE + 0x5098)
#define CP_NF_INI_KEY_2	(NF_REG_BASE + 0x509c)
#define CP_NF_INI_KEY_3	(NF_REG_BASE + 0x50a0)

#define CP_NF_KEY_0		(NF_REG_BASE + 0x5084)
#define CP_NF_KEY_1		(NF_REG_BASE + 0x5088)
#define CP_NF_KEY_2		(NF_REG_BASE + 0x508c)
#define CP_NF_KEY_3		(NF_REG_BASE + 0x5090)
#define CP_NF_KEY_L0	(NF_REG_BASE + 0x50e0)
#define CP_NF_KEY_L1	(NF_REG_BASE + 0x50e4)
#define CP_NF_KEY_L2	(NF_REG_BASE + 0x50e8)
#define CP_NF_KEY_L3	(NF_REG_BASE + 0x50ec)
#define CP_NF_KEY_L4	(NF_REG_BASE + 0x50f0)
#define CP_NF_KEY_L5	(NF_REG_BASE + 0x50f4)
#define CP_NF_KEY_L6	(NF_REG_BASE + 0x50f8)
#define CP_NF_KEY_L7	(NF_REG_BASE + 0x50fc)

#define SYS_REG_BASE			0x98000000         
#define SYS_muxpad0				(SYS_REG_BASE + 0x360)	//mux for nf

/* NAND flash ECC type */
typedef enum nandecctype
{
	NAND_ECC_TYPE_6BIT = 0,
	NAND_ECC_TYPE_12BIT,
	NAND_ECC_TYPE_40BIT,
	NAND_ECC_TYPE_43BIT = 4,
	NAND_ECC_TYPE_65BIT = 6,
	NAND_ECC_TYPE_72BIT = 8,
	NAND_ECC_TYPE_24BIT = 0xa,
	NAND_ECC_TYPE_16BIT = 0xe,
	NAND_ECC_TOTAL_TYPE
}nandecctype_t;

typedef struct /*__attribute__ ((__packed__))*/{
    u16 BB_die;
    u16 bad_block;
    u16 RB_die;
    u16 remap_block;
}BB_t;

//nf
#define NAND_CHIP_SELECT		0x1e				// NAND flash chip select = 0
#define NAND_HW_SETTING_RETRY	0x100				/* search HW setting in NAND flash retry count (blocks) */
#define NAND_BOOTCODE_RETRY		0X100				/* search bootcode in NAND flash retry count (blocks) */
#define NAND_PROFILE_LIMIT		0x100				/* limit of search profile in NAND flash (pages) */
#define NAND_PROFILE_INCREMENT	0x40				/* increment count when search profile in NAND flash retry count (pages) */
#define HW_SETTING_CUR_FLAG_MAGIC_HI	0x6259		/* magic number for NAND flash HW setting current page flag */
#define HW_SETTING_CUR_FLAG_ADDR		0xb8060104	/* address of NAND flash HW setting current page flag, original value is 0xb801a608 */


#define ERR_WAKE_FROM_SUSPEND		0x8f
#define ERR_INVALID_ADDRESS		0x90
#define ERR_TIMEOUT			0x91
#define ERR_UNKNOWN_TYPE		0x92
#define ERR_INVALID_PARAM		0x93
#define ERR_ALLOC_FAILED		0x94
#define ERR_FUNC_NOT_AVAILABLE      0x95
#define ERR_AES_MODE_NOT_SUPPORT	0x96
#define ERR_WRITE_SRAM_FAIL			0x97
#define ERR_UART_BUFFER_FULL		0x9e
#define ERR_UNEXPECTED_RETURN		0x9f

#define ERR_NF_RESET_FAIL           0x40
#define ERR_NF_READ_ID_FAIL         0x41
#define ERR_NF_IDENTIFY_FAIL        0x42
#define ERR_NF_READ_ECC_FAIL        0x43
#define ERR_NF_ECC_IDENTIFY_FAIL        0x44

extern int nf_initial(void);

#endif /* __RTKEMMC_H__ */
