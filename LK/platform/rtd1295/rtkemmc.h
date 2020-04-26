#ifndef __RTKEMMC_H__
#define __RTKEMMC_H__

/* 1295 emmc error */
#define CR_EMMC_EBE				0x8000
#define CR_EMMC_SBE				0x2000
#define CR_EMMC_HLE				0x1000
#define CR_EMMC_FRUN			0x0800
#define CR_EMMC_HTO				0x0400
#define CR_EMMC_DRTO			0x0200
#define CR_EMMC_RTO				0x0100
#define CR_EMMC_DCRC			0x0080
#define CR_EMMC_RCRC			0x0040
#define CR_EMMC_RE				0x0002

/* eMMC control register definition */
#define CR_BASE_ADDR             	0x98012000

#define CR_EMMC_CTRL 				( CR_BASE_ADDR)
#define CR_EMMC_PWREN 				( CR_BASE_ADDR + 0x004)
#define CR_EMMC_CLKDIV 				( CR_BASE_ADDR + 0x008)
#define CR_EMMC_CLKSRC 				( CR_BASE_ADDR + 0x00C)
#define CR_EMMC_CLKENA				( CR_BASE_ADDR + 0x010)
#define CR_EMMC_TMOUT				( CR_BASE_ADDR + 0x014)
#define CR_EMMC_CTYPE				( CR_BASE_ADDR + 0x018)
#define CR_EMMC_BLKSIZ              ( CR_BASE_ADDR + 0x01C)

#define CR_EMMC_BYTCNT				( CR_BASE_ADDR + 0x020)
#define CR_EMMC_INTMASK 			( CR_BASE_ADDR + 0x024)
#define CR_EMMC_CMDARG 				( CR_BASE_ADDR + 0x028)
#define CR_EMMC_CMD 				( CR_BASE_ADDR + 0x02C)
#define CR_EMMC_RESP0				( CR_BASE_ADDR + 0x030)
#define CR_EMMC_RESP1				( CR_BASE_ADDR + 0x034)
#define CR_EMMC_RESP2				( CR_BASE_ADDR + 0x038)
#define CR_EMMC_RESP3				( CR_BASE_ADDR + 0x03C)

#define CR_EMMC_RINTSTS				( CR_BASE_ADDR + 0x044)
#define CR_EMMC_STATUS				( CR_BASE_ADDR + 0x048)

#define CR_EMMC_FIFOTH				( CR_BASE_ADDR + 0x04C)
#define CR_EMMC_UHSREG				( CR_BASE_ADDR + 0x074)

#define CR_EMMC_BMOD				( CR_BASE_ADDR + 0x080)
#define CR_EMMC_DBADDR				( CR_BASE_ADDR + 0x088)
#define CR_EMMC_IDINTEN				( CR_BASE_ADDR + 0x090)
#define CR_EMMC_CARD_THR_CTL		( CR_BASE_ADDR + 0x100)
#define CR_EMMC_DDR_REG				( CR_BASE_ADDR + 0x10C)

#define CR_EMMC_CP					( CR_BASE_ADDR + 0x41C)
#define CR_EMMC_ISR                 ( CR_BASE_ADDR + 0x424)
#define CR_EMMC_DUMMY_SYS           ( CR_BASE_ADDR + 0x42C)
#define CR_EMMC_PAD_CTL				( CR_BASE_ADDR + 0x474)
#define CR_EMMC_CKGEN_CTL           ( CR_BASE_ADDR + 0x478)
#define CR_EMMC_DQS_CTRL1         	( CR_BASE_ADDR + 0x498 )
#define CR_EMMC_SWC_SEL				( CR_BASE_ADDR + 0x4D4)

#define CR_EMMC_muxpad0				(CR_BASE_ADDR + 0x600)	//mux for nand and emmc
#define CR_EMMC_muxpad1				(CR_BASE_ADDR + 0x604)	//mux for sdio and sd card
#define CR_EMMC_PFUNC_CR			(CR_BASE_ADDR + 0x610)	
#define CR_EMMC_muxpad2				(CR_BASE_ADDR + 0x61c)

#define CR_EMMC_PFUNC_NF1           ( CR_BASE_ADDR + 0x60c)
#define CR_EMMC_PDRIVE_NF0          ( CR_BASE_ADDR + 0x620)
#define CR_EMMC_PDRIVE_NF1          ( CR_BASE_ADDR + 0x624)
#define CR_EMMC_PDRIVE_NF2          ( CR_BASE_ADDR + 0x628)
#define CR_EMMC_PDRIVE_NF3          ( CR_BASE_ADDR + 0x62c)
#define CR_EMMC_PDRIVE_NF4          ( CR_BASE_ADDR + 0x630)

/* eMMC clock div definition */
#define EMMC_CLOCK_DIV_NON      (0x00000000)
#define EMMC_CLOCK_DIV_2        (0x00000001)
#define EMMC_CLOCK_DIV_8        (0x00000004)
#define EMMC_CLOCK_DIV_128      (0x00000001<<6)
#define EMMC_CLOCK_DIV_256      (0x00000001<<7)

#define EMMC_INIT_CLOCK_DIV     EMMC_CLOCK_DIV_128


/* Standard MMC commands (4.1)              type    argument            response */
/* class 1 */
#define MMC_GO_IDLE_STATE           0       /* bc                           */
#define MMC_SEND_OP_COND            1       /* bcr  [31:0] OCR          R3  */
#define MMC_ALL_SEND_CID            2       /* bcr                      R2  */
#define MMC_SET_RELATIVE_ADDR       3       /* ac   [31:16] RCA         R1  */
#define MMC_SET_DSR                 4       /* bc   [31:16] RCA             */
#define MMC_SLEEP_AWAKE             5       /* ac   [31:16] RCA 15:flg  R1b */
#define MMC_SWITCH                  6       /* ac   [31:0] See below    R1b */
#define MMC_SELECT_CARD             7       /* ac   [31:16] RCA         R1  */
#define MMC_SEND_EXT_CSD            8       /* adtc                     R1  */
#define MMC_SEND_CSD                9       /* ac   [31:16] RCA         R2  */
#define MMC_SEND_CID                10      /* ac   [31:16] RCA         R2  */
#define MMC_READ_DAT_UNTIL_STOP     11      /* adtc [31:0] dadr         R1  */
#define MMC_STOP_TRANSMISSION       12      /* ac                       R1b */
#define MMC_SEND_STATUS             13      /* ac   [31:16] RCA         R1  */
#define MMC_GO_INACTIVE_STATE       15      /* ac   [31:16] RCA             */
#define MMC_SPI_READ_OCR            58      /* spi                      spi_R3 */
#define MMC_SPI_CRC_ON_OFF          59      /* spi  [0:0] flag          spi_R1 */

  /* class 2 */
#define MMC_SET_BLOCKLEN            16      /* ac   [31:0] block len    R1  */
#define MMC_READ_SINGLE_BLOCK       17      /* adtc [31:0] data addr    R1  */
#define MMC_READ_MULTIPLE_BLOCK     18      /* adtc [31:0] data addr    R1  */

  /* class 3 */
#define MMC_WRITE_DAT_UNTIL_STOP    20      /* adtc [31:0] data addr    R1  */

  /* class 4 */
#define MMC_SET_BLOCK_COUNT         23      /* adtc [31:0] data addr    R1  */
#define MMC_WRITE_BLOCK             24      /* adtc [31:0] data addr    R1  */
#define MMC_WRITE_MULTIPLE_BLOCK    25      /* adtc                     R1  */
#define MMC_PROGRAM_CID             26      /* adtc                     R1  */
#define MMC_PROGRAM_CSD             27      /* adtc                     R1  */

  /* class 6 */
#define MMC_SET_WRITE_PROT          28      /* ac   [31:0] data addr    R1b */
#define MMC_CLR_WRITE_PROT          29      /* ac   [31:0] data addr    R1b */
#define MMC_SEND_WRITE_PROT         30      /* adtc [31:0] wpdata addr  R1  */
#define MMC_SEND_WRITE_PROT_TYPE    31      /* adtc [31:0] wpdata addr  R1  */


  /* class 5 */
#define MMC_ERASE_GROUP_START       35      /* ac   [31:0] data addr    R1  */
#define MMC_ERASE_GROUP_END         36      /* ac   [31:0] data addr    R1  */
#define MMC_ERASE                   38      /* ac                       R1b */

  /* class 9 */
#define MMC_FAST_IO                 39      /* ac   <Complex>           R4  */
#define MMC_GO_IRQ_STATE            40      /* bcr                      R5  */

  /* class 7 */
#define MMC_LOCK_UNLOCK             42      /* adtc                     R1b */

  /* class 8 */
#define MMC_APP_CMD                 55      /* ac   [31:16] RCA         R1  */
#define MMC_GEN_CMD                 56      /* adtc [0] RD/WR           R1  */


#define R1_CURRENT_STATE(x)     ((x & 0x1E000000) >> 25) /* sx, b (3 bits, 9-11bits) */

//ext csd 
#define EXT_CSD_ERASE_GROUP_DEF		175	/* R/W */
#define EXT_CSD_HC_ERASE_GRP_SIZE	224	/* RO */


/* send status event */
#define STATE_IDLE  0
#define STATE_READY 1
#define STATE_IDENT 2
#define STATE_STBY  3
#define STATE_TRAN  4
#define STATE_DATA  5
#define STATE_RCV   6
#define STATE_PRG   7
#define STATE_DIS   8

/* Sizes of RPMB data frame */
#define RPMB_SZ_STUFF		196
#define RPMB_SZ_MAC		    32
#define RPMB_SZ_DATA		256
#define RPMB_SZ_NONCE		16
#define HASH_BLOCK_SIZE     64

/*register operation*/
#define PTR_U32(_p)   ((unsigned int)(unsigned long)(_p))
#define PTR_U64(_p)   ((unsigned long)(_p))
#define U64_PTR(_v)   ((void *)(unsigned long)(_v))
#define U32_PTR(_v)   U64_PTR(_v)

#define cr_writel(_v, _a)  __writel(_v, U64_PTR(_a))
#define cr_readl(_a)       __readl(U64_PTR(_a))


#define SWITCH_ERROR 		(1<<7)


#define mdelay(_msec)      spin((_msec) * 1000)
#define udelay(_usec)      spin(_usec)

/* Request codes */
#define RPMB_REQ_KEY		1
#define RPMB_REQ_WCOUNTER	2
#define RPMB_REQ_WRITE_DATA	3
#define RPMB_REQ_READ_DATA	4
#define RPMB_REQ_STATUS		5

/*
 * MMC_SWITCH access modes
 */
#define MMC_SWITCH_MODE_CMD_SET     0x00    /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS    0x01    /* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS  0x02    /* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_WRITE_BYTE  0x03    /* Set target to value */

#define EXT_CSD_BOOT_WP			173	/* R/W */
#define EXT_CSD_WP_STATUS  	    174  /* R */
#define EXT_CSD_PART_CONF		179	/* R/W */
#define PART_ACCESS_MASK        0x7

/* Response code */
#define RPMB_RESP_KEY		    0x0100
#define RPMB_RESP_WCOUNTER	    0x0200
#define RPMB_RESP_WRITE_DATA	0x0300
#define RPMB_RESP_READ_DATA  	0x0400


/*emmc physic partition*/
#define MMC_PART_UDA    0x0
#define MMC_PART_BOOT_1 0x1
#define MMC_PART_BOOT_2 0x2
#define MMC_PART_RPMB   0x3


#define RESULT_ERROR_MASK 0x7


struct backupRegs {
    unsigned int                 emmc_mux_pad0;
    unsigned int                 emmc_mux_pad1;
    unsigned int                 emmc_pfunc_nf1;
	unsigned int				 emmc_pfunc_cr;
    unsigned int                 emmc_pdrive_nf1;
    unsigned int                 emmc_pdrive_nf2;
    unsigned int                 emmc_pdrive_nf3;
    unsigned int                 emmc_ctype;
    unsigned int                 emmc_uhsreg;
    unsigned int                 emmc_ddr_reg;
    unsigned int                 emmc_card_thr_ctl;
    unsigned int                 emmc_clk_div;
    unsigned int                 emmc_ckgen_ctl;
    unsigned int                 emmc_dqs_ctrl1;
};



struct rpmb_packet { //512 bytes
unsigned char stuff[RPMB_SZ_STUFF];
unsigned char mac[RPMB_SZ_MAC];
unsigned char data[RPMB_SZ_DATA];
unsigned char nonce[RPMB_SZ_NONCE];
unsigned int write_counter;  // 4 bytes
unsigned short address;
unsigned short block_count;
unsigned short result;
unsigned short request;
};


/*extern for rpmb.c*/
//extern void make_ip_des(const unsigned char *dma_addr, unsigned int dma_length);
extern unsigned int emmc_send_cmd_get_rsp(unsigned int cmd_idx, unsigned int sd_arg, unsigned int rsp_con, unsigned int crc);
extern unsigned int rtk_emmc_write(unsigned int blk_addr, unsigned int blk_cnt, const unsigned char *buffer);
extern unsigned int rtk_emmc_read(unsigned int blk_addr, unsigned int blk_cnt, unsigned char *buffer);
extern int emmc_read_write_ip(unsigned int cmd_idx, unsigned int blk_addr,const unsigned char *dma_addr, unsigned int dma_length);


extern  unsigned int check_error(void);
extern  unsigned int  emmc_cid[4];
extern  unsigned int rpmb_size;



#endif /* __RTKEMMC_H__ */
