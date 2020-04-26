#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* for exit() */
#include <limits.h>
#include <errno.h>

#include "libcgi.h"

/* for open() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <dirent.h>

#define CGI_MAX_STRING 256

/*
 * public function used by all modules
 */
 
int cmd_string2id(const char *string, const PARAMETER_TABLE *table, unsigned int table_len )
{
	unsigned int i;

	if (string == NULL || strlen(string) == 0)
		return -1;

	for( i=0; i<table_len; i++ )
	{
		if( strncasecmp( table[i].strCmd, string, strlen(table[i].strCmd) ) == 0 )
		{
			return table[i].id;
		}
	}

	return -1;
}

char *cmd_id2string(int id, const PARAMETER_TABLE *table, unsigned int table_len)
{
	unsigned int i;

	for( i=0; i<table_len; i++ )
	{
		if( table[i].id == id)
		{
			return strdup(table[i].strCmd);
		}
	}

	return NULL;
}

/*
 * static structure used internally
 */
static CGI_t *handle;

static int cgi_check_number(char *p);
static int cgi_parse_method(void);
static int cgi_parse_contentheader(void);
static int cgi_parse_contenttype(void);
static int cgi_parse_contentlen(void);
static int cgi_parse_boundary(void);
static int cgi_check_end_boundary(char *buffer, int buffer_start, int *flush_cnt);


int cgi_init(void)
{
	handle = (CGI_t *)malloc(sizeof(CGI_t));
	handle->id = ID_UNKNOWN;
	handle->method = METHOD_UNKNOWN;
	handle->content_type = CONTENT_UNKNOWN;
	handle->content_len = -1;
	handle->boundary = NULL;
	handle->urlcontent = NULL;
	handle->http_cookie = NULL;

	if (cgi_parse_method() != CGI_OK)
		return CGI_FAIL;
	if (handle->method == METHOD_POST)
		if (cgi_parse_contentheader() != CGI_OK)
			return CGI_FAIL;

	return CGI_OK;
}

void cgi_deinit(void)
{
	if (handle->boundary)
	{
		free(handle->boundary);
		handle->boundary = NULL;
	}

	if (handle->urlcontent)
	{
		free(handle->urlcontent);
		handle->urlcontent = NULL;
	}

	if (handle->http_cookie)
	{
		free(handle->http_cookie);
		handle->http_cookie = NULL;
	}
	
	free(handle);
	handle = NULL;
}

CGI_METHOD cgi_get_method(void)
{
	return handle->method;
}

CGI_CONTENT cgi_get_content_type(void)
{
	return handle->content_type;
}

int cgi_get_content_len(void)
{
	return handle->content_len;
}

char *cgi_get_content(void)
{
    return handle->urlcontent;
}
char *cgi_get_boundary(void)
{
	return handle->boundary;
}

int cgi_get_id(void)
{
	int id = 0;
	if (cgi_get_ivalue("id", &id) != CGI_OK)
		return -1;
	else 
		return id;
}

/*
 * Get integer value according to the given name
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *         CGI_END on end.
 * In:  
 *		name: value name
 * Out:    
 *		retValue: returned value.
 */
int cgi_get_ivalue(const char* name, int *retValue)
{
	char *pvalue = NULL;

	if (strcmp(name, "id") && (handle->content_type == CONTENT_FORMDATA))
		return CGI_FAIL;


	if (cgi_get_svalue(name, &pvalue) != CGI_OK)
		return CGI_FAIL;

	if (cgi_check_number(pvalue) != CGI_OK)
		return CGI_FAIL;
	*retValue = atoi(pvalue);
	if (pvalue)
		free(pvalue);

	return CGI_OK;
}

/*
 * Get string value according to the given name
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *         CGI_END on end.
 * In:  
 *		name: value name
 * Out:    
 *		ret: returned value, it must be freed by 
 *			 caller if return status is CGI_OK.
 */
int cgi_get_svalue(const char* name, char **ret)
{
	char *querystr;
	char *p;

	if (strcmp(name, "id") && (handle->content_type == CONTENT_FORMDATA))
		return CGI_FAIL;

	if (handle->method == METHOD_GET || !strcmp(name, "id"))
	{
        char *tempStr = getenv("QUERY_STRING");

   		//querystr = strdup(getenv("QUERY_STRING"));
        if(tempStr)
        {
            querystr = strdup(tempStr);
        }
        else
        {
            return CGI_FAIL;
        }
	}
	else if (handle->method == METHOD_POST &&
		handle->content_type == CONTENT_URLENC)
	{
		/* For POST method, Only "application/x-www-form-urlencoded" is 
		 * supported in this function, to get value from 
		 * "multipart/form-data", use cgi_save_nextvalue_buffer or 
		 * cgi_save_nextvalue_file
		 */
		if (handle->urlcontent == NULL)
			return CGI_FAIL;
		querystr = strdup(handle->urlcontent);
	}
	else
		return CGI_FAIL;

	p = querystr;
	if (querystr == NULL) 
		return CGI_FAIL;

	while(1)
	{
		char *pname;

		pname = p;
		if (!strncmp(p, name, strlen(name)) &&
			(p[strlen(name)] == '='))
		{
			while(*p != '&' && *p != '\0')
				p++;
			if (*p == '&')
				*p = '\0';

			*ret = cgi_decode_url(pname + strlen(name) + 1);
			free(querystr);
			return CGI_OK;
		}

		while(*p != '&' && *p != '\0')
			p++;
		if (*p == '\0')
		{
			free(querystr);
			return CGI_FAIL;
		}
		p++;
	}
	return CGI_OK;
}

char *cgi_encode_url(char *url)
{
	char *retstr = NULL;
	unsigned int i;
	int pos = 0;

	if (url == NULL)
		return NULL;
	retstr = (char*)malloc(strlen(url) * 3 + 1);
	if (retstr == NULL)
		return NULL;
	
	for (i = 0; i < strlen(url); i++)
	{
		unsigned char ch = (unsigned char)url[i];
		if (((ch >= '0') && (ch <= '9')) ||
			((ch >= 'a') && (ch <= 'z')) ||
			((ch >= 'A') && (ch <= 'Z')))
			retstr[pos++] = ch;
		else if (ch == ' ')
			retstr[pos++] = '+';
		else
		{
			sprintf(&retstr[pos], "%%%02x", ch);
			pos += 3;
		}
	}
	retstr[pos] = '\0';
	return retstr;
}

char *cgi_decode_url(char *url)
{
	char *retstr = NULL;
	unsigned int i;
	int pos = 0;
	
	retstr = (char *)malloc(strlen(url) + 1);
	if (retstr == NULL)
		return NULL;
	
	for (i = 0; i < strlen(url); i++)
	{
		char hexstr[5];
		long hexch;
		char *endptr;

		if (url[i] == '+')
		{
			retstr[pos++] = ' ';
			continue;
		}
		if (url[i] != '%')
		{
			retstr[pos++] = url[i];
			continue;
		}
		if ((i + 2) > strlen(url))
			return NULL;
		hexstr[0] = '0';
		hexstr[1] = 'x';
		hexstr[2] = url[i+1];
		hexstr[3] = url[i+2];
		hexstr[4] = '\0';

		errno = 0;
		hexch = strtol(hexstr, &endptr, 16);
		if ((errno == ERANGE && 
			(hexch == LONG_MAX || hexch == LONG_MIN)) || 
			(errno != 0 && hexch == 0))
			return NULL;

		if (endptr == hexstr)
			return NULL;

		retstr[pos++] = (char)hexch;
		i += 2;	
	}
	retstr[pos] = '\0';
	return retstr;
}

char *cgi_get_cookie()
{
	if (handle->http_cookie == NULL)
		handle->http_cookie = strdup(getenv("HTTP_COOKIE"));
		return handle->http_cookie;
}

/*
 * The following functions are for multipart/form-data only.
 */

/*
 * Get next name from multipart/form-data
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *         CGI_END on end.
 * Out:    
 *		ret_name: returned name, it must be freed by 
 *					caller if return status is CGI_OK.
 */
int cgi_get_nextname(char **ret_name)
{
	char buffer[1024];
	char *pname;
	int  i = 0;

	while(1)
	{
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			return CGI_END;

		if (strstr(buffer, "Content-Disposition") == NULL)
			continue;
		if ((pname = strstr(buffer, "name=\"")) == NULL)
			return CGI_FAIL;

		i = 0;
		pname += strlen("name=\"");
		/* trim the ending '\"' */
		while ((*(pname + i) != '\"') && 
			   (*(pname + i) != '\0'))
			i++;
		if (*(pname + i) == '\"');
			*(pname + i) = '\0';
		*ret_name = strdup(pname);
		break;
	}
	return CGI_OK;
}

/*
 * Save value into buffer, this function must be called
 * after cgi_get_nextname().
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *         CGI_END on end.
 * Out:    
 *		ret_value: returned value buffer, it must be freed by 
 *					caller if return status is CGI_OK.
 *		ret_len:    total length of return value buffer.
 */
int cgi_save_nextvalue_buffer(char **ret_value, int *ret_len)
{
	char buffer[1024];
	int  total_read = 0;
	char *value_buffer = NULL;

	/* check buffer is a ring buffer to detect end boundary */
	char *check_buffer = NULL;
	int  check_start = 0;
	int  check_filled = 0;
	int  check_len = strlen(handle->boundary) + 2; /* 2 for "\r\n" */
	int  flush_cnt = 0;

	/* values start with a new line */
	while(1)
	{
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			return CGI_END;
		if (!strcmp(buffer, "\r\n"))
			break;
	}
	
	check_buffer = (char*)malloc(check_len);	
	if (check_buffer == NULL)
		return CGI_FAIL;

	while(1)
	{
		/* read check_len bytes from stdin */
		while(check_filled < check_len)
		{
			int ch;
			ch = fgetc(stdin);
			if (ch == EOF)
			{
				free(check_buffer);
				if (value_buffer)
					free(value_buffer);
				*ret_value = NULL;
				*ret_len = 0;
				return CGI_FAIL; /* unexpected EOF */
			}
			check_buffer[(check_start + check_filled) % check_len] = (char)ch;
			check_filled++;
		}

		/* Final exit */
		if (cgi_check_end_boundary(check_buffer, check_start, &flush_cnt) == CGI_OK)
		{
			/* value_buffer should be freed by caller */
			if (!(total_read % 256))
			{
				/* realloc for the terminating byte */
				if ((value_buffer = (char*)realloc(value_buffer, total_read + 1)) == NULL)
				{
					*ret_value = NULL;
					*ret_len = 0;
					return CGI_FAIL;
				}
			}
			value_buffer[total_read] = '\0';
			free(check_buffer);
			*ret_value = value_buffer;
			*ret_len = total_read;
			return CGI_OK;
		}

		/* Flush unmatched bytes into value_buffer */
		while(flush_cnt--)
		{
			if (!(total_read % 256))
			{
				if ((value_buffer = (char*)realloc(value_buffer, total_read + 256)) == NULL)
				{
					free(check_buffer);
					*ret_value = NULL;
					*ret_len = 0;
					return CGI_FAIL;
				}
			}
			value_buffer[total_read] = check_buffer[check_start];
			total_read++;
			check_start = (check_start + 1) % check_len;
			check_filled--;
		}
	}

	/* Never get here */
	return CGI_FAIL;
}

/*
 * Save value into file, this function must be called
 * after cgi_get_nextname().
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *         CGI_END on end.
 *
 * In:
 *		fd:file descriptor
 * Out:    
 *		ret_len: total bytes written into fd.
 */
int cgi_save_nextvalue_file(FILE *fd, int *ret_len)
{
	char buffer[1024];
	int  total_read = 0;

	/* check buffer is a ring buffer to detect end boundary */
	char *check_buffer = NULL;
	int  check_start = 0;
	int  check_filled = 0;
	int  check_len = strlen(handle->boundary) + 2; /* 2 for "\r\n" */
	int  flush_cnt = 0;

	/* values start with a new line */
	while(1)
	{
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			return CGI_END;
		if (!strcmp(buffer, "\r\n"))
			break;
	}
	
	check_buffer = (char*)malloc(check_len);	
	if (check_buffer == NULL)
		return CGI_FAIL;

	while(1)
	{
		/* read check_len bytes from stdin */
		while(check_filled < check_len)
		{
			int ch;
			ch = fgetc(stdin);
			if (ch == EOF)
			{
				free(check_buffer);
				*ret_len = 0;
				return CGI_FAIL; /* unexpected EOF */
			}
			check_buffer[(check_start + check_filled) % check_len] = (char)ch;
			check_filled++;
		}

		/* Detect end boundary */
		if (cgi_check_end_boundary(check_buffer, check_start, &flush_cnt) == CGI_OK)
		{
			/* Final exit */
			free(check_buffer);
			*ret_len = total_read;
			return CGI_OK;
		}

		/* Flush unmatched bytes into file */
		while(flush_cnt--)
		{
			if (fputc(check_buffer[check_start], fd) == EOF)
			{
				free(check_buffer);
				*ret_len = 0;
				return CGI_FAIL;
			}
			total_read++;
			check_start = (check_start + 1) % check_len;
			check_filled--;
		}
	}

	/* Never get here */
	return CGI_FAIL;
}

int cgi_find_nextvalue_start(void)
{
	char buffer[1024];

	while(1)
	{
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			return CGI_END;
		if (!strcmp(buffer, "\r\n"))
			break;
	}
	return CGI_OK;
}

/*
 * Internal functions
 */

static int cgi_check_number(char *p)
{
	if ((*p != '-') && (*p > '9' || *p < '0'))
		return CGI_FAIL;
	p++;
	while (*p != '\0')
	{
		if (*p > '9' || *p < '0')
			return CGI_FAIL;
		p++;
	}
	return CGI_OK;
}

/* Get request method and save it to handle->method */
static int cgi_parse_method(void)
{
	char *method;

	method = getenv("REQUEST_METHOD");

	if (method == NULL || strncmp(method, "GET", 3) == 0)
		handle->method = METHOD_GET;
	else if (strncmp(method, "POST", 4) == 0)
		handle->method = METHOD_POST;
	else 
		return CGI_FAIL;

	return CGI_OK;
}

/* Get content type and content length*/
static int cgi_parse_contentheader(void)
{
	if ((cgi_parse_contenttype() != CGI_OK) ||
		(cgi_parse_contentlen() != CGI_OK))
		return CGI_FAIL;
	
	if (handle->content_type == CONTENT_FORMDATA)
		if (cgi_parse_boundary() != CGI_OK)
			return CGI_FAIL;
			
	if ((handle->content_type == CONTENT_URLENC)
		|| (handle->content_type == CONTENT_JSON)
		)
	{
		handle->urlcontent = (char*)malloc(handle->content_len + 1);
		if (handle->urlcontent == NULL)
			return CGI_FAIL;

		if (fread(handle->urlcontent, 1, handle->content_len, stdin) < 
			(unsigned int)handle->content_len)
		{	
			free(handle->urlcontent);
			return CGI_FAIL;
		}
		handle->urlcontent[handle->content_len] = '\0';
	}

	return CGI_OK;
}

static int cgi_parse_contenttype(void)
{
	char *content_type;

	if (handle->method != METHOD_POST)
		return CGI_FAIL;

	content_type = getenv("CONTENT_TYPE");
	if (content_type == NULL || 
		(strlen(content_type) > CGI_MAX_STRING))
		return CGI_FAIL;

	if (!strncmp(content_type, "application/x-www-form-urlencoded", 
		strlen("application/x-www-form-urlencoded")))
		handle->content_type = CONTENT_URLENC;
	else if (!strncmp(content_type, "multipart/form-data",
		strlen("multipart/form-data")))
		handle->content_type = CONTENT_FORMDATA;
    else if (!strncmp(content_type, "application/json",
        strlen("application/json")))
        handle->content_type = CONTENT_JSON;
	else
		/* If not indicated, suppose it's url encoded */
		handle->content_type = CONTENT_URLENC;

	return CGI_OK;
}

static int cgi_parse_contentlen(void)
{
	char *content_len;

	if (handle->method != METHOD_POST)
		return CGI_FAIL;

	content_len = getenv("CONTENT_LENGTH");
	if (content_len == NULL || 
		(strlen(content_len) > CGI_MAX_STRING))
		return CGI_FAIL;
	
	handle->content_len = atoi(content_len);
	if (handle->content_len <= 0)
		return CGI_FAIL;

	return CGI_OK;
}

int cgi_get_json_svalue(char* name, char **ret)
{
    char *ptr = handle->urlcontent, *ptr_end;
    int size = 0;

    while(*ptr != '\0')
    {
        if (strncmp(ptr, name, strlen(name)) == 0)
        {
            //find the tag, try to get value
            ptr += strlen(name)+strlen("\":\"");
            ptr_end = ptr;

            while(ptr_end != NULL && *ptr_end != '\"') ptr_end++;

            size = ptr_end - ptr;
            *ret = (char*) malloc(size+1);
            strncpy(*ret, ptr, size);

            return CGI_OK;
        }

        ptr++;
    }

    return CGI_FAIL;
}

static int cgi_parse_boundary(void)
{
	char *content_type = getenv("CONTENT_TYPE");
	char *boundary = NULL;

	if (handle->method != METHOD_POST &&
		handle->content_type != CONTENT_FORMDATA)
		return CGI_FAIL;
	
	if ((boundary = strstr(content_type, "boundary=")) == NULL)
		return CGI_FAIL;

	handle->boundary = (char*)malloc(strlen(boundary + 9) + 3);
	if (handle->boundary == NULL)
		return CGI_FAIL;
	
	sprintf(handle->boundary, "--%s", boundary + 9);
	handle->boundary[strlen(boundary + 9) + 2] = '\0';
	return CGI_OK;
}

/*
 * Check if buffer contains boundary
 * 
 * Return: CGI_OK on success; CGI_FAIL on error; 
 *
 * In:
 *		buffer: ring buffer to check
 *		buffer_start: start position of buffer
 * Out:    
 *		flush_cnt: If return value is CGI_FAIL, flush_cnt is set to 
 *		indicate the number of bytes matching boundary.
 */
static int cgi_check_end_boundary(char *buffer, int buffer_start, int *flush_cnt)
{
	int match_cnt = 0;
	int total = strlen(handle->boundary) + 2;
	char *pboundary = handle->boundary;
	int i = 0;
	int pos = 0;

	/* check if this a full match */
	pos = buffer_start;
	if (buffer[pos] == 13  && 
		buffer[(pos + 1) % total] == 10)
	{
		pos = (pos + 2) % total;
		while(buffer[pos] && *pboundary &&
			  buffer[pos] == *(pboundary++))
		{
			pos = (pos + 1) % total;
			match_cnt++;
		}

		if ((unsigned int)match_cnt == strlen(handle->boundary))
			return CGI_OK;
	}
	/* count the number of bytes to be flushed */
	pos = buffer_start;
	for (i = 0; i < total; i++)
	{
		int j = 0;
		if (buffer[pos] != 13)
		{
			pos = (pos + 1) % total;
			continue;
		}
		pos = (pos + 1) % total;
		i++;
		if (buffer[pos] != 10)
		{
			pos = (pos + 1) % total;
			continue;
		}
		pos = (pos + 1) % total;
		i++;
		/* calculate the possiblly matched length */
		for (j = 0; (j < total - i) && (buffer[pos] == handle->boundary[j]); j++)
			pos = (pos + 1) % total;

		/* Got the length */
		if (j == total -i)
		{
			/* minus the "\r\n" */
			*flush_cnt = i - 2;
			return CGI_FAIL;
		}
		i += j;
	}
	
	/* Nothing valuable found in boundary */
	pos = buffer_start;
	if (buffer[(pos + total - 1) % total] == 13)
		*flush_cnt = total - 1;
	else if ((buffer[(pos + total - 1) % total] == 10) &&
			(buffer[(pos + total - 2) % total] == 13))
		*flush_cnt = total - 2;
	else
		*flush_cnt = total; 
	return CGI_FAIL;
}

