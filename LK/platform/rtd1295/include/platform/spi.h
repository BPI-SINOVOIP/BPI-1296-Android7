#pragma  once

int rtk_spi_init(void);
int rtk_spi_write(unsigned int blk_addr, unsigned int data_size, unsigned char *buffer);
int rtk_spi_read(unsigned int blk_addr, unsigned int data_size, unsigned char *buffer);

