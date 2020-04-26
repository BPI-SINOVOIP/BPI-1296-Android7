#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define LOG_NDEBUG 1
#ifndef LOG_TAG
#define LOG_TAG "UPLOAD_CGI"
#endif

#include <cutils/log.h>
#define CLOGD(fmt,args...)  ALOGD(fmt, ##args)
#define CLOGE(fmt,args...)  ALOGE(fmt, ##args)
#define CLOGV(fmt,args...)  ALOGV(fmt, ##args)

#ifndef ANDROID
	#include <sys/statvfs.h>
	#include <sys/sem.h>
#else
	#include <linux/sem.h>
	#include <sys/vfs.h>
	#define statvfs statfs
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "ccgi.h"
#include <Application/AppClass/SetupClassAndroid.h>

/* recursive mkdir */
/* abort on ENOENT; ignore other errors like "directory already exists" */
/* return 1 if OK */
/*        0 on error */

int makedir (char *newdir)
{
  char *buffer = strdup(newdir);
  char *p;
  int  len = strlen(buffer);

  if (len <= 0) {
    free(buffer);
    return 0;
  }
  if (buffer[len-1] == '/') {
    buffer[len-1] = '\0';
  }
  if (mkdir(buffer, 0755) == 0)
    {
      free(buffer);
      return 1;
    }

  p = buffer+1;
  while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
        p++;
      hold = *p;
      *p = 0;
      if (mkdir(buffer, 0755) != 0)
        {
          CLOGE("Couldn't create directory %s, error:%d", buffer, errno);
          free(buffer);
          return 0;
        }
      if (hold == 0)
        break;
      *p++ = hold;
    }
  free(buffer);
  return 1;
}

int main(void)
{
	CLOGD("The uplocadCGI Module is loaded");
    int count = 0;
    while(FCGI_Accept() >= 0) {

		CLOGD("The uplocadCGI Module is accepted");
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }
        printf("Content-type: text/html\r\n\r\n"
               "<title>FastCGI Upload !</title>"
               "<h1>FastCGI Upload!</h1>"
               "Request number %d running on host <i>%s</i>\n",
                ++count, getenv("SERVER_NAME"));

        printf("<br />CONTENT_LENGTH = %d <br />\r\n", len);
        printf("<form enctype='multipart/form-data' method='post' action='?X-Progress-ID=d2034932771bf2cac7730f0eb010a036'><input type='text' name='text1' /><input type='file' name='uploadfile'/><input type='submit' /></form>");
        printf("<hr />");

        fflush(stdout);

        CLOGD("STDOUT is flished");

        if (len > 0)
        {
        	SetupClassAndroid setupClass;

        	const char *pRes = "/sdcard/UploadCGI";
			char UploadFolder[512];
			strcpy(UploadFolder, pRes);

			int result = makedir(UploadFolder);
			CLOGD("makedir [%s], result is %d", UploadFolder, result);

			char tmpPath[1024]; memset(tmpPath, 0 , sizeof(tmpPath));
			snprintf(tmpPath, sizeof(tmpPath), "%s/cgi-upload-XXXXXX", UploadFolder);
        	CGI_varlist *vl = CGI_get_post(0, tmpPath);
        	if (vl)
        	{
	        	CGI_value *value = CGI_lookup_all(vl, "uploadfile");
	        	if (value)
	        	{
	        		CLOGD("the value is %s:%s", value[0], value[1]);
	        		char newPath[1024]; memset(newPath, 0 , sizeof(newPath));
	        		snprintf(newPath, sizeof(newPath), "%s/%s", UploadFolder, value[1]);
	        		rename(value[0], newPath);

					char cmd[1024]; memset(cmd, 0, sizeof(cmd));
					sprintf(cmd,"am broadcast -a rtk.provider.media.scan -e \"path\" \"%s\"", newPath);
					CLOGD("Add the file (%d):%s", strlen(cmd), cmd);
					system(cmd);
	        	}
	        	else
	        	{
	        		CLOGE("The uploadfile is not exist!");
			        printf("<h1>Upload Failed !</h1>");
               	}
	        	CGI_free_varlist(vl);
        	}
        	else
        	{
        		CLOGE("Failed to get the POST data!");
        		printf("<h1>Upload Failed - No POST data !</h1>");
        	}
        }

        FCGI_Finish();
    }
}

