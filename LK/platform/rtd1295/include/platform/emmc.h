#pragma  once

int rtk_emmc_init(void);
int rtk_emmc_write(unsigned int blk_addr, unsigned int data_size, unsigned char *buffer);
int rtk_emmc_read(unsigned int blk_addr, unsigned int data_size, unsigned char *buffer);

