#include <rtk_parameter.h>
#include <rtk_common.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define SYSTEM_PARAMETERS	"/sys/realtek_boards/system_parameters"
int get_parameter_string(const char* keystring, char *string, int str_maxlen)
{
    static int is_read_file = 0;
    static char sys_parameters[2048];
    char sys_para_tmp[256];
    int fd = 0;
    int ret;
    char *ptr, *ptr1, *ptr2;
    char *sep=": \t\n";
    int length = 0;
    if (string == NULL) return -1;
    if (str_maxlen == 0) return -1;

    if(is_read_file == 0)
    {
        memset(sys_parameters, 0 , sizeof(sys_parameters));
        ret = rtk_file_to_string(SYSTEM_PARAMETERS, sys_parameters);
        if(ret < 0)
        {
            install_debug("Can not read system parameters(%s).\n", sys_parameters);
            return -1;
        }
        is_read_file = 1;
    }

    // find keystring
    //install_info("system_para:[%s]\r\n", sys_parameters);
    ptr = strstr(sys_parameters, keystring);
    if(!ptr)
    {
        install_info("Can't find string(%s)\r\n", keystring);
        return -1;
    }

    sprintf(sys_para_tmp, "%s", sys_parameters);
    ptr = strtok(sys_para_tmp, sep);
    while(ptr)
    {
        ptr = strtok(NULL, sep);
        if(!ptr)
            continue;
        if(strstr(ptr, keystring))
        {
            ptr1 = strstr(ptr, "=");
            ptr1++;
            ptr2 = strtok(NULL, sep);
            if (ptr2)
            {
                length = ptr2 - ptr1;
            }
            else
            {
                length = strlen(ptr1);
            }
//			printf("ptr1 = %s\n", ptr1);
            string[0] = 0;
            if (length)
            {
                strcpy(string, ptr1);
            }
//			printf("string = %s\n", string);
            return 0;
        }
    }
    return -1;
}


int get_parameter_value(const char* keystring, unsigned int *keyval)
{
    char tmp_string[128];
    if (get_parameter_string(keystring, tmp_string, 128) == 0)
    {
        //printf("string = %s\n", tmp_string);
        *keyval = strtoul(tmp_string, 0, 16);
        install_log("Got system_parameter:%s 0x%x\r\n", keystring, *keyval);
        return 0;
    }
    return -1;

}
