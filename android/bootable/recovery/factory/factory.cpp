#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rtk_factory.h>
#include <rtk_common.h>

#if 0
typedef struct _bootloader_message
{
    char command[32];
    char status[32];
    char recovery[1024];
} bootloader_message;

#define RECOVERY_FILE_IN_FACTORY  "/tmp/factory/recovery"
#endif

void usage(char *filename)
{

    printf("usage: %s [load | loadro | save ]\n", filename);


}
int main(int argc, char **argv)
{
    //printf("argc = %d\n", argc);
    //printf("argv = %s %s\n", argv[0], argv[1]);

#if 0
    bootloader_message boot;
    memset(&boot, 0, sizeof(bootloader_message));
    memset(&boot.command, '\0', sizeof(boot.command));
    memset(&boot.recovery, '\0', sizeof(boot.recovery));
    sprintf(boot.command, "boot-recovery");
    sprintf(boot.recovery, "recovery\n--update_package=CACHE:update.zip\n--locale=zh_CN");
    //sprintf(boot.recovery, "recovery\n--wipe_data\n--locale=zh_CN");

    FILE* fp = fopen(RECOVERY_FILE_IN_FACTORY, "wb+");
    if(fp == NULL)
    {
        printf("Can't open %s\n", RECOVERY_FILE_IN_FACTORY);
        return -1;
    }
    int count = fwrite(&boot, sizeof(boot), 1, fp);
    if(count != 1)
    {
        printf("Failed writing %s\n", RECOVERY_FILE_IN_FACTORY);
        return -1;
    }
    fflush(fp);
    if(fclose(fp) != 0)
    {
        printf("Failed closing %s\n", RECOVERY_FILE_IN_FACTORY);
        return -1;
    }
#endif

    if(argc < 2)
    {
        usage(argv[0]);
        return -1;
    }
    if(!strcmp(argv[1], "load"))
    {
        if(rtk_file_lock()) return -1;
        factory_load();
        system("chown 1000:1000 /tmp/factory");
        if(rtk_file_unlock()) return -1;
    }
    else if(!strcmp(argv[1], "save"))
    {
        if(rtk_file_lock()) return -1;
        factory_flush(0, 0);
        if(rtk_file_unlock()) return -1;
    }
#ifdef RTK_VMX
    else if(!strcmp(argv[1], "write"))
    {
        factory_write();
   
    }
#endif
    else if(!strcmp(argv[1], "recovery"))
    {
        if(rtk_file_lock()) return -1;
        factory_load();
        system("chown 1000:1000 /tmp/factory");
        sync();
        factory_flush(0, 0);
        if(rtk_file_unlock()) return -1;
    }
#ifdef FACTORY_RO_SUPPORT
    else if(!strcmp(argv[1], "loadro"))
    {
        factory_load_ro();
    }
#endif

    return 0;
}

