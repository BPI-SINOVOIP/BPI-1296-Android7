#ifndef RTK_FACTORY_H
#define RTK_FACTORY_H
#include <stdio.h>

#define FACTORY_RO_SUPPORT	1

int factory_init(const char* dir=NULL);
int factory_load(const char *dir=NULL);
#ifdef RTK_VMX
int rtk_check_installPartition_name(const char* prefix, char* mtd_name, char* mmcblk_name);
int rtk_check_install_img_name_path(const char* path, char* install_name);
int rtk_get_partition_size(char *mtd_name);
int file_size(char *filename);
int factory_write(const char *dir=NULL);
#endif
#ifdef FACTORY_RO_SUPPORT
int factory_init_ro();
int factory_load_ro(const char *dir=NULL);
#endif
int factory_flush(unsigned int factory_start, unsigned int factory_size);
extern char factory_dir[32];
#endif
