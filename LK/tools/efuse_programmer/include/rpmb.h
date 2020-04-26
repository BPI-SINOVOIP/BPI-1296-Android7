#ifndef __RPMB_H__
#define __RPMB_H__

#include "hmac256.h"


#define cr_readl(addr)          	REG32(addr)
#define cr_writel(value,addr)   	(*(volatile unsigned int *)(addr) = (value))
#define ISB __asm__ volatile("isb" ::: "memory")

#define uswap_16(x) \
	( (((x) & 0xff00)>>8) | \
	  (((x) & 0x00ff)<<8)  )
	  
#define uswap_32(x) \
			( (((x) & 0xff000000)>>24) | \
			  (((x) & 0x00ff0000)>>8 ) | \
			  (((x) & 0x0000ff00)<<8 ) | \
			  (((x) & 0x000000ff)<<24) )

#define SWAP_32(x) \
    (((uint32_t)(x) << 24) | (((uint32_t)(x) & 0xff00) << 8) |(((uint32_t)(x) & 0x00ff0000) >> 8) | ((uint32_t)(x) >> 24))

#define SWAP_16(x)  ((((uint16_t)(x) & 0xff) << 8) | ((uint16_t)(x) >> 8))
#define PTR_U32(_p)   ((unsigned int)(unsigned long)(_p))
#define SWITCH_ERROR 		(1<<7)


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif



/* 
 * 1295 emmc error
 */
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

/*
 * EMMC commands
 */
#define MMC_SWITCH                  6       /* ac   [31:0] See below    R1b */
#define MMC_SELECT_CARD             7       /* ac   [31:16] RCA         R1  */
#define MMC_SEND_EXT_CSD            8       /* adtc                     R1  */
#define MMC_SEND_CID                10      /* ac   [31:16] RCA         R2  */
#define MMC_SEND_STATUS             13      /* ac   [31:16] RCA         R1  */
#define MMC_SET_BLOCK_COUNT         23      /* adtc [31:0] data addr    R1  */
#define MMC_READ_MULTIPLE_BLOCK     18      /* adtc [31:0] data addr    R1  */
#define MMC_WRITE_MULTIPLE_BLOCK    25      /* adtc                     R1  */



/*
 * MMC_SWITCH access modes
 */
#define MMC_SWITCH_MODE_CMD_SET     0x00    /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS    0x01    /* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS  0x02    /* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_WRITE_BYTE  0x03    /* Set target to value */

#define EXT_CSD_PART_CONF		179	/* R/W */
#define PART_ACCESS_MASK        0x7


/*emmc physic partition*/
#define MMC_PART_RPMB  0x3
#define MMC_PART_UDA   0x0

/* Sizes of RPMB data frame */
#define RPMB_SZ_STUFF		196
#define RPMB_SZ_MAC		    32
#define RPMB_SZ_DATA		256
#define RPMB_SZ_NONCE		16
#define HASH_BLOCK_SIZE     64

struct rpmb_packet { 		//512 bytes
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


/* Request codes */
#define RPMB_REQ_KEY		1
#define RPMB_REQ_WCOUNTER	2
#define RPMB_REQ_WRITE_DATA	3
#define RPMB_REQ_READ_DATA	4
#define RPMB_REQ_STATUS		5

/* Response code */
#define RPMB_RESP_KEY		    0x0100
#define RPMB_RESP_WCOUNTER	    0x0200
#define RPMB_RESP_WRITE_DATA	0x0300
#define RPMB_RESP_READ_DATA  	0x0400

#define CR_BASE_ADDR             	0x98012000
#define CR_EMMC_CMD 				( CR_BASE_ADDR + 0x02C)
#define CR_EMMC_RINTSTS				( CR_BASE_ADDR + 0x044)
#define CR_EMMC_INTMASK 			( CR_BASE_ADDR + 0x024)
#define CR_EMMC_CMDARG 				( CR_BASE_ADDR + 0x028)
#define CR_EMMC_STATUS				( CR_BASE_ADDR + 0x048)
#define CR_EMMC_CP					( CR_BASE_ADDR + 0x41C)
#define CR_EMMC_ISR                 ( CR_BASE_ADDR + 0x424)
#define CR_EMMC_DUMMY_SYS           ( CR_BASE_ADDR + 0x42C)
#define CR_EMMC_PAD_CTL				( CR_BASE_ADDR + 0x474)
#define CR_EMMC_CKGEN_CTL           ( CR_BASE_ADDR + 0x478)
#define CR_EMMC_DQS_CTRL1         	( CR_BASE_ADDR + 0x498 )
#define CR_EMMC_SWC_SEL				( CR_BASE_ADDR + 0x4D4)

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
#define CR_EMMC_DBADDR				( CR_BASE_ADDR + 0x088)


#define MAX_DESCRIPTOR_NUM    (8)
#define EMMC_MAX_SCRIPT_BLK   (8)
#define EMMC_MAX_MULTI_BLK    (EMMC_MAX_SCRIPT_BLK * MAX_DESCRIPTOR_NUM)
#define RESULT_ERROR_MASK 0x7


/* function declaration */

extern int emmc_get_cid(void);
extern int emmc_burn_key(void);
extern int emmc_write_rpmb(void);
extern void get_counter(void);



#endif   //__RPMB_H__
