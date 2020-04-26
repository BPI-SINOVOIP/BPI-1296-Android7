#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <rtk_common.h>
uint32_t get_checksum(uint8_t *p, uint32_t len)
{
    uint32_t checksum = 0;
    uint32_t i;

    for(i = 0; i < len; i++)
    {
        checksum += *(p+i);
    }
    return checksum;
}
int isready(int fd)
{
    int rc;
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    rc = select(fd+1, &fds, NULL, NULL, &tv);
    if (rc < 0)
        return -1;

    return FD_ISSET(fd, &fds) ? 1 : 0;
}
#define STAR_COUNT 30.0
static inline void progressbar(int len, int length, const char* filename)
{
    int i, starcount;
    if(len == 0) printf("\n");
    printf("\r");
    if(filename != NULL) printf("%s", filename);
    printf("%d |", (int) (100.0*len/length));
    starcount = (int) (STAR_COUNT*len/length);
    for(i=0; i<starcount; i++) printf("*");
    for(i=0; i<STAR_COUNT-starcount; i++) printf(" ");
    printf("| %d KB", len>>10);
    if(len == length) printf("\n");
}


int fd_to_fd(int sfd, int dfd, unsigned int length, unsigned int* pchecksum, FILE* ffd)
{
    int ret, rret, wret;
    unsigned int rlen;
    unsigned int len;
    unsigned int progress_len, interval;
    char data_buf[2048*8];
    time_t stime, etime;
    int restart;
    time_t start_time, end_time;
    stime = time(NULL);
    interval = length/10;
    progress_len = 0;
    len = 0;
    if(pchecksum != NULL)
        *pchecksum = 0;
#define TIME_OUT 3.0
    restart = 1;
    progressbar(0, length, NULL);
    fflush(stdout);
    while(len < length)
    {
        rlen = ((length - len) < sizeof(data_buf)) \
               ? (length - len): sizeof(data_buf);
        ret = isready(sfd);
        // select error
        if(ret < 0)
        {
            install_debug("select error\r\n");
            goto dd_end;
        }
        if(ret == 0)
        {
            if(restart == 1)
            {
                install_debug("first timeout\r\n");
                start_time = time(NULL);
                restart = 0;
            }
            else
            {
                install_debug("secand timeout\r\n");
                end_time = time(NULL);
                if(difftime(end_time, start_time) > TIME_OUT)
                    goto dd_end;
            }
            continue;
        }

        restart = 1;

        rret = read(sfd, (void*) data_buf, rlen);
        if(rret < 0)
        {
            install_debug("read source fd fail\r\n");
            goto dd_end;
        }
        else if(rret == 0)
        {
            install_debug("read source fd close\r\n");
            goto dd_end;
        }
        if(ffd == NULL)
            wret = write(dfd, (void*) data_buf, rret);
        else
            wret = fwrite(data_buf, 1, rret, ffd);
        if(wret < 0)
        {
            install_debug("write fail\r\n");
            goto dd_end;
        }
        if(rret!=wret)
        {
            install_debug("rret = %d, wret = %d\r\n", rret, wret);
            goto dd_end;
        }
        if(pchecksum != NULL)
        {
            *pchecksum += get_checksum((uint8_t *)data_buf, (uint32_t) wret);
        }
        len = len + rret;
        if(len >= progress_len)
        {
            progressbar(len, length, NULL);
            fflush(stdout);
            progress_len = progress_len + interval;
        }
    }
dd_end:
    etime = time(NULL);
    if(pchecksum != NULL)
        printf("checksum:0x%08X\r\n", *pchecksum);

    //printf("\r\ntotal_time:%.0lf secands\r\n", difftime(etime,stime));
    return len;
}

unsigned int octalStringToInt(const char* str, unsigned int lenstr)
{
    unsigned int size;
    unsigned int i;
    size = 0;
    for(i=0; i<lenstr; i++)
    {
        if(str[i]<='9'&&str[i]>='0')
            size = size*8 + (str[i]-0x30);
        else
            size = size*8;
    }
    return size;
}

int rtk_get_size_of_file(const char* file_path)
{
    struct stat stat_buf;
    int ret;
    ret = stat(file_path, &stat_buf);
    if(ret < 0)
    {
        install_fail("Can't find %s\r\n", file_path);
        return -1;
    }
    return stat_buf.st_size;
}

int rtk_command(const char* command, int line, const char* file, int show)
{
    int ret;
    ret = system(command);
    //if(1 == show )
    //printf("command:%s, (line=%d) [file=%s], ret(%d)\r\n", command, line, file, ret);
    fflush(stdout);
    fflush(stderr);
    if(ret != 0)
    {
        install_fail("Can't exec command:%s, (line=%d) (file=%s) (ret=%d)\r\n", command, line, file, ret);
        return -1;
    }
    return 0;
}
int rtk_file_to_string(const char* path, char* str)
{

    FILE* filep;
    char cmd[128];
    int ret;
    sprintf(cmd, "dd if=%s of=/tmp/str.txt", path);
    ret = rtk_command(cmd, __LINE__, __FILE__);
    if(ret < 0)
    {
        install_fail("cmd fail\r\n");
        return -1;
    }
    filep = fopen("/tmp/str.txt", "r");
    if(filep == NULL)
    {
        install_fail("Can't open (%s)\r\n", path);
        return -1;
    }

    if(NULL != fgets(str, 512, filep))
    {
        install_info("Got String(%s)\r\n", str);
    }
    fclose(filep);
    return 0;
}

int rtk_file_to_flash(const char* filepath, unsigned int soffset, const char* devpath, unsigned int doffset, unsigned int len, unsigned int* checksum)
{
    int sfd, dfd;
    int ret;
    sfd = open(filepath, O_RDONLY);
    if(sfd<0)
    {
        install_fail("Can't open %s\r\n", filepath);
        return -1;
    }
    dfd = open(devpath, O_RDWR|O_SYNC);
    if(dfd<0)
    {
        install_fail("Can't open %s\r\n", devpath);
        return -1;
    }
    ret = lseek(sfd, soffset, SEEK_SET);
    if(ret < 0)
    {
        install_fail("lseek (%s) fail\r\n", filepath);
        close(sfd);
        close(dfd);
        return -1;
    }
    ret = lseek(dfd, doffset, SEEK_SET);
    if(ret < 0)
    {
        install_fail("lseek (%s) fail\r\n", devpath);
        close(sfd);
        close(dfd);
        return -1;
    }

    ret = fd_to_fd(sfd, dfd, len, checksum, NULL);
    close(sfd);
    close(dfd);

    if(ret != len)
    {
        install_fail("fd_to_fd fail(%d)\r\n", ret);
        return -1;
    }
    return ret;
}
int rtk_file_to_ptr(const char* filepath, unsigned int soffset, void* ptr, unsigned int len)
{
    int sfd;
    int ret;
    sfd = open(filepath, O_RDONLY);
    if(sfd<0)
    {
        install_fail("Can't open %s\r\n", filepath);
        return -1;
    }
    ret = lseek(sfd, soffset, SEEK_SET);
    if(ret < 0)
    {
        install_fail("lseek (%s) fail\r\n", filepath);
        close(sfd);
        return -1;
    }

    ret = read(sfd, ptr, len);
    close(sfd);

    if(ret != len)
    {
        install_fail("read fd fail(%d)\r\n", ret);
        return -1;
    }
    return 0;
}

int rtk_ptr_to_file(const char* filepath, unsigned int soffset, void* ptr, unsigned int len)
{
    int sfd;
    int ret;
    sfd = open(filepath, O_WRONLY );
    if(sfd<0)
    {
        install_fail("Can't open %s\r\n", filepath);
        return -1;
    }
    ret = lseek(sfd, soffset, SEEK_SET);
    if(ret < 0)
    {
        install_fail("lseek (%s) fail\r\n", filepath);
        close(sfd);
        return -1;
    }

    ret = write(sfd, ptr, len);
    printf("ret = %d\n", ret);
    close(sfd);

    if(ret != len)
    {
        install_fail("read fd fail(%d)\r\n", ret);
        return -1;
    }
    return 0;
}

int rtk_ptr_to_flash(void* ptr, unsigned int len, const char* devpath, unsigned int doffset)
{
    int dfd, ret;
    dfd = open(devpath, O_RDWR|O_SYNC);
    if(dfd<0)
    {
        install_fail("Can't open %s\r\n", devpath);
    }
    ret = lseek(dfd, doffset, SEEK_SET);
    if(ret < 0)
    {
        install_fail("lseek (%s) fail\r\n", devpath);
        close(dfd);
        return -1;
    }

    ret = write(dfd, ptr, len);
    close(dfd);
    if(ret != len)
    {
        install_fail("ret(%d)!=len(%u)\r\n", ret, len);
        return -1;
    }
    return 0;
}

int rtk_flash_erase(unsigned int offset, unsigned int len)
{
    return 0;
}


int rtk_file_lock(void)
{


    int fd;
    int timeout = 2000;
    do
    {
        fd = open("/tmp/factory.lock", O_RDWR | O_CREAT | O_EXCL, 0755);
        if (fd >= 0) return 0;
        usleep(1000);
        if (fd < 0) install_info("wait factory.lock");
    }
    while (--timeout && fd<0);

    if (timeout == 0 && fd < 0)
    {
        install_fail("get factory lock fail\n");
        return -1;
    }
    return 0;
}

int rtk_file_unlock(void)
{

    int ret;
    int timeout = 100;
    ret = access("/tmp/factory.lock", F_OK);

    if (ret)
    {
        install_info("try to unlock non-exist factory lock\n");
        return -1;
    }
    do
    {
        if (remove("/tmp/factory.lock"))
        {
            install_fail("remove factory lock fail\n");
        }
        else
        {
            //install_info("remove factory lock success\n");
            return 0;
        }
    }
    while(timeout--);
    return -1;


}

int rtk_file_compare(const char *file1, const char *file2)
{


    int len1, len2;
    unsigned char data1, data2;
    int fd1, fd2;
    int i;
    len1 = rtk_get_size_of_file(file1);

    if (len1 != (len2 = rtk_get_size_of_file(file2)) && (len1 !=0))
    {
        printf("%s file length not equal to %s  (%d %d bytes)\n", file1, file2, len1, len2);
        return -1;
    }

    fd1 = open(file1, O_RDONLY);

    if (fd1 < 0)
    {
        printf("%s open fail\n", file1);
        return -1;
    }
    fd2= open(file2, O_RDONLY);
    if (fd2 < 0)
    {
        printf("%s open fail\n", file2);
        return -1;
    }


    for (i=0; i<len1; i++)
    {

        read(fd1, &data1, 1);
        read(fd2, &data2, 1);
        if (data1 != data2)
        {
            close(fd1);
            close(fd2);
            return -1;
        }

    }


    close(fd1);
    close(fd2);

    return 0;


}

static uint32_t crc_table[256];

static uint32_t Reflect(uint32_t ref, char ch)
{
    uint32_t value = 0;
    int i;
    // Swap bit 0 for bit 7
    // bit 1 for bit 6, etc.
    for (i = 1; i < (ch + 1); i++)
    {
        if (ref & 1)
            value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value;
}

/* Make the table for a fast CRC. */

static void make_crc_table(void)
{

    uint32_t polynomial = 0x04C11DB7;
    int i, j;

    for (i = 0; i <= 0xFF; i++)
    {
        crc_table[i] = Reflect(i, 8) << 24;
        for (j = 0; j < 8; j++)
            crc_table[i] = (crc_table[i] << 1) ^ (crc_table[i] & (1 << 31) ? polynomial : 0);
        crc_table[i] = Reflect(crc_table[i],  32);
    }

    //crc_table_computed = 1;
}

uint32_t crc32(uint8_t *p, int len, uint32_t *crc)
{
    int cnt = len;
    uint32_t value;

    make_crc_table();

    value = 0xFFFFFFFF;
    while (cnt--)
    {
        value = (value >> 8) ^ crc_table[(value & 0xFF) ^ *p++];
    }

    *crc = value ^ 0xFFFFFFFF;

    return 1;
}

