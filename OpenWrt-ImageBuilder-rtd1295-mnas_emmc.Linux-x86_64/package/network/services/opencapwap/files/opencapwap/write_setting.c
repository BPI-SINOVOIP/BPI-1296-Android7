#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../boa/apmib/apmib.h"
#include "../boa/src/version.c"
#include "CWCommon.h"


static inline char *get_sw_version()
{
/*
	static char sw_ver[128];
	sprintf(sw_ver, "%d.%d.%s", DRV_VERSION_H, DRV_VERSION_L, DRV_VERSION_L_SUB);
	return sw_ver;
*/
	return (char *)fwVersion; // in ../boa/src/version.c
}

void write_config_ac()
{
	FILE *fp;
	fp = fopen("config.ac", "w+");
	if (!fp) {
		printf("%s error!\n", __FUNCTION__);
		_exit(1);
	}
	fprintf(fp, "</AC_HW_VERSION> 1235656\n");
//	fprintf(fp, "</AC_SW_VERSION> %u\n", (unsigned int)time(NULL));
	fprintf(fp, "</AC_SW_VERSION> %s\n", get_sw_version());
	fprintf(fp, "</AC_MAX_STATIONS> %d\n", AC_MAX_STA_NUM);
	fprintf(fp, "</AC_MAX_WTPS> %d\n", AC_MAX_WTP_NUM);
	fprintf(fp, "</AC_SECURITY> PRESHARED\n");
	fprintf(fp, "</AC_LEV3_PROTOCOL> IPv4\n");
	fprintf(fp, "</AC_FORCE_MTU> 1420\n");
	fprintf(fp, "</AC_NAME> My AC\n");
	fprintf(fp, "<AC_MCAST_GROUPS>\n");
	fprintf(fp, "</AC_MCAST_GROUPS>\n");
	fprintf(fp, "</AC_LOG_FILE_ENABLE> 1\n");
	fprintf(fp, "</AC_LOG_FILE_SIZE> %d\n", LOG_FILE_SIZE);
	fclose(fp);
}
void write_config_wtp()
{
}
void write_setting_ac()
{
}
void write_setting_wtp()
{
}

int main()
{
	write_config_ac();
	write_config_wtp();
	write_setting_ac();
	write_setting_wtp();
	return 0;
}
