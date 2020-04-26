#ifndef RTK_MTD_H
#define RTK_MTD_H
// flash meminfo api
// flash open api
// command api
#include <sys/types.h>
#include <stdlib.h>
#include <rtk_common.h>

#define MTD_ABSENT              0
#define MTD_RAM                 1
#define MTD_ROM                 2
#define MTD_NORFLASH            3
#define MTD_NANDFLASH           4
#define MTD_PEROM               5
#define MTD_DATAFLASH           6
#define MTD_OTHER               14
#define MTD_UNKNOWN             15

int rtk_open_mtd_char(char** p=NULL);
int rtk_open_mtd_block(char** p=NULL);
int get_mtd_block_name(char** ppstr);
int rtk_get_meminfo(struct mtd_info_user* meminfo);
uint32_t rtk_get_erasesize(void);
char* get_mtd_block_name_str(void);
char *get_mtd_char_name(void);
int rtk_open_mtd_block_with_offset(unsigned int offset);
int rtk_mtd_erase(int fd, int start, int len);
int rtk_mtd_char_erase(unsigned int address, unsigned int size);
int rtk_mtd_char_program(unsigned char *buf, unsigned int address, unsigned size);
int rtk_mtd_char_verify(unsigned char *buf, unsigned int address, unsigned size);
int rtk_open_mtd_char(void);
int modify_addr_signature(unsigned int startAddress, unsigned int reserved_boot_size);
int modify_signature(unsigned int reserved_boot_size, unsigned int flash_type);
int get_flsh_info(BOOTTYPE* flash_type, unsigned int* factory_start, unsigned int* factory_size, unsigned int* erasesize);
#ifdef RTK_VMX
int rtk_mtd_write_factory_install_flag(unsigned long long file_size);
#endif
#endif

