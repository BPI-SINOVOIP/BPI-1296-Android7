#ifndef LIB_CGI_H
#define LIB_CGI_H
#include <stdio.h>

#undef CGI_DEBUG
#ifndef CGI_DEBUG
#define cgi_log
#endif

#define SIZEOF_ARRAY(x)   (sizeof(x)/sizeof(x[0]))
#define INSERT_PARAM(x,y)	{x, y}

typedef struct {
	const char *strCmd;
	int id;
} PARAMETER_TABLE;


/* Http request URL "id" parm definition */
typedef enum
{
	ID_REMOTE = 0,
	ID_QUERY_INFO = 1,
	ID_GETSET_SETTING_ITEM = 2,
	ID_FSDB_REQUEST = 3,
	ID_INPUT_STRING = 4,	
	ID_TOGO = 5,
	ID_WIFI = 6,
	ID_UNKNOWN,
} CGI_ID;

typedef enum
{
	METHOD_GET,
	METHOD_POST,
	METHOD_UNKNOWN,
} CGI_METHOD;

typedef enum
{
	/*application/x-www-form-urlencoded*/
	CONTENT_URLENC, 	
	/*multipart/form-data*/
	CONTENT_FORMDATA,
	/* application/json */
	CONTENT_JSON,
	CONTENT_UNKNOWN,
} CGI_CONTENT;

typedef enum
{
	CGI_OK=0,
	CGI_END=-1,
	CGI_FAIL=-2,
} CGI_RET;

typedef struct
{
	CGI_ID id;
	CGI_METHOD method;

	CGI_CONTENT 	content_type;
	int            	content_len;
	char*          	urlcontent;	
	char*          	boundary;
	char*          	http_cookie;
} CGI_t;

typedef enum{
	BUTTON_POWER,
	BUTTON_HOME,
	BUTTON_DISCMENU,
	BUTTON_POPUPMENU,
	BUTTON_TOPMENU,
	BUTTON_CURSOR,
	BUTTON_OPTION,
	BUTTON_BACK,
	BUTTON_STOP,
	BUTTON_PLAY,
	BUTTON_PAUSE,
	BUTTON_PREVIOUS,
	BUTTON_NEXT,
	BUTTON_REWIND,
	BUTTON_FORWARD,
	BUTTON_VOLUME,
	BUTTON_MUTE,
	BUTTON_AUDIO,
	BUTTON_SUBTITLE,
	BUTTON_EDIT,
	BUTTON_INFO,
	BUTTON_SCREENFIT,
	NUM_OF_BUTTON
} REMOTE_CONTROL_BUTTON_ID;

typedef struct
{
	REMOTE_CONTROL_BUTTON_ID id;
	char strId[16];
	bool isAvail;
} BUTTON_ITEM_T;

int cmd_string2id(const char *string, const PARAMETER_TABLE *table, unsigned int table_len );
char *cmd_id2string(int id, const PARAMETER_TABLE *table, unsigned int table_len);


int cgi_init(void);
void cgi_deinit(void);

CGI_METHOD cgi_get_method(void);
CGI_CONTENT cgi_get_content_type(void);
int cgi_get_content_len(void);
char *cgi_get_content(void);
char *cgi_get_boundary(void);

int cgi_get_id(void);
int cgi_get_ivalue(const char* name, int *retValue);
int cgi_get_svalue(const char* name, char **ret);
int cgi_get_json_svalue(char* name, char **ret);
int cgi_get_nextname(char **ret_name);
int cgi_save_nextvalue_buffer(char **ret_value, int *ret_len);
int cgi_save_nextvalue_file(FILE *fd, int *ret_len);
int cgi_find_nextvalue_start(void);
char *cgi_encode_url(char *);
char *cgi_decode_url(char *);
char *cgi_get_cookie();

#endif //LIB_CGI_H
