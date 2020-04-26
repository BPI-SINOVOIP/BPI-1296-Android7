#ifndef __RTKSPI_H__
#define __RTKSPI_H__

#define SPI_FLASH_BASE_ADDR     (0x88100000)

// Serial Flash Controller Registers
#define SB2_SFC_BASE            (0x9801A800)
#define SB2_SFC_OPCODE			(SB2_SFC_BASE + 0x0)
#define SB2_SFC_CTL			    (SB2_SFC_BASE + 0x4)
#define SB2_SFC_SCK			    (SB2_SFC_BASE + 0x8)
#define SB2_SFC_CE			    (SB2_SFC_BASE + 0xC)
#define SB2_SFC_WP			    (SB2_SFC_BASE + 0x10)
#define SB2_SFC_POS_LATCH		(SB2_SFC_BASE + 0x14)
#define SB2_SFC_WAIT_WR			(SB2_SFC_BASE + 0x18)
#define SB2_SFC_EN_WR			(SB2_SFC_BASE + 0x1C)
#define SB2_SFC_FAST_RD			(SB2_SFC_BASE + 0x20)
#define SB2_SFC_SCK_TAP			(SB2_SFC_BASE + 0x24)

#define mdelay(_msec)      spin((_msec) * 1000)
#define udelay(_usec)      spin(_usec)

// Register Operation
#define PTR_U32(_p)   ((unsigned int)(unsigned long)(_p))
#define PTR_U64(_p)   ((unsigned long)(_p))
#define U64_PTR(_v)   ((void *)(unsigned long)(_v))
#define U32_PTR(_v)   U64_PTR(_v)

int rtk_spi_identify(void);
int rtk_spi_init(void);
unsigned int rtk_spi_erase(unsigned int nor_addr, unsigned int byte_size);
unsigned int rtk_spi_write(unsigned int nor_addr, unsigned int byte_size, const unsigned char *buffer);
unsigned int rtk_spi_read(unsigned int nor_addr, unsigned int byte_size, unsigned char *buffer);

#endif /* __RTKSPI_H__ */
