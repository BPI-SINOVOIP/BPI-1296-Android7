#ifndef __FACTORY_LIB_H__
#define __FACTORY_LIB_H__

#define FACTORY_HEADER_FILE_NAME		"tmp/factory/"

int factory_dump_info(void);
int factory_reset(void);
int factory_delete(const char *path);
int factory_read_by_index(int index, char *buffer, int *length, char *name);
int factory_read(const char *path, char**buffer, int *length);
int factory_write(const char *path, char *buffer, int length);
int factory_save(void);
int factory_init(void);

#endif /* __FACTORY_LIB_H__ */

