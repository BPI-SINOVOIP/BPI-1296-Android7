#include "config.h"
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#define MIN_TIME_ELAPSE_SEC        2

static time_t m_last_stat_time = 0;
static unsigned int m_bytes_collected = 0;
static unsigned int m_downrate_bytes = 0;

#define HTTP_STATS_FILE    "/tmp/httpDownloadRate"

void ne_stats_countbytes(unsigned int bytes) 
{
    time_t now;
    m_bytes_collected += bytes;
    
    now = time(NULL);
    if ((now - m_last_stat_time) > MIN_TIME_ELAPSE_SEC) {
        FILE * file;
        char fstring[128];
        
        m_downrate_bytes = m_bytes_collected / (now - m_last_stat_time);
        m_last_stat_time = now;
        m_bytes_collected = 0;
        
        file = fopen(HTTP_STATS_FILE, "w+");
        if (file == NULL)
            return;
        snprintf(fstring, 128, "%d", m_downrate_bytes);
        fputs(fstring, file);
        fclose(file);
    }

}

unsigned int ne_stats_returnKB(void) 
{
    return m_downrate_bytes;
}
