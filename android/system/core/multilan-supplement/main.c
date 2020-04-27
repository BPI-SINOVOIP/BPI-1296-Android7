#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "xml_util.h"

#define	BUFSZ			4096
#define MAXLANGUAGE		20
#define LANGUAGEIDSIZE	128

#define STRINGIDLEN		128
#define	XMLNODELEN		2048
#define FILEPATHLEN		256
#define SYSCMDLEN		128

#define ENDSTRING	"STRING_NUM"
#define	INPUTFILE	"MKStringTable.utf8"

//#define DEBUG

#define PRINT(x)	//printf x

typedef struct{
	char language_id[LANGUAGEIDSIZE];
	//FILE *fh_xml;
	char file_path[FILEPATHLEN];
	// and more...
}MULTILAN_STR;

void handle_header(char *data);
void handle_row(char *data);

int reach_end(char *data);
void open_file(MULTILAN_STR *str);
void create_new_xml_file(const char *file);
void prepare_values_folder(const char *dir);

FILE *fh=NULL;

char readBuf[BUFSZ];
char delim[1];

MULTILAN_STR language[MAXLANGUAGE];
int nums_of_language;

int main(void)
{
	printf("Realtek kiwi multi-language supplement tool\n");
	// TAB char
	delim[0]=(char)0x9;

	// load txt file
	fh = fopen(INPUTFILE,"r");

	if(fh!=NULL)
	{
		memset(readBuf,0x0,BUFSZ);
		if(fgets(readBuf,BUFSZ,fh)!=NULL)
		{
			// 1. parse header.
			handle_header(readBuf);
			memset(readBuf,0x0,BUFSZ);

			// 2. parse each row
			while(fgets(readBuf,BUFSZ,fh)!=NULL)
			{
				// EOS
				if(reach_end(readBuf)==1)
					break;
				handle_row(readBuf);
				memset(readBuf,0x0,BUFSZ);
			}
		}
		fclose(fh);
	}

	return 0;
}

int reach_end(char *data)
{
	if(strncmp(data,ENDSTRING,strlen(ENDSTRING))==0)
		return 1;
	else
		return -1;
}

void handle_header(char *data)
{
	//printf("[HEADER] %s\n",data);

	// reset related parameters
	nums_of_language=0;
	int i=0;
	for(i=0;i<MAXLANGUAGE;i++)
	{
		memset(&language[i],0x0,sizeof(MULTILAN_STR));
	}

	char *tok=strtok(data,delim);
	while(tok!=NULL)
	{
		snprintf(language[nums_of_language].language_id,LANGUAGEIDSIZE,"%s",tok);
		remove_tailing_newline(language[nums_of_language].language_id);
		PRINT(("[HEADER] -- %s\n",language[nums_of_language].language_id));
		// open xml file handle of this language.
		open_file(&language[nums_of_language]);

		nums_of_language++;
		tok=strtok(NULL,delim);
	}
	PRINT(("[HEADER] nums_of_language: %d\n",nums_of_language));
}

void handle_row(char *data)
{
	char *tok=strtok(data,delim);
	int i=0;
	char id[STRINGIDLEN];
	char xml_node[XMLNODELEN];
	int gotID=0;

	while(tok!=NULL)
	{
		// always remove new line of each token
		remove_tailing_newline(tok);
		if(i==0)
		{
			gotID=1;
			memset(id,0x0,STRINGIDLEN);
			snprintf(id,STRINGIDLEN,"%s",tok);
			PRINT(("[ROW] ID=%s\n",id));
		}
		else
		{
			int j=i-1;
			if(gotID>0)
			{
				int token_len = strlen(tok);
				PRINT(("[TOKEN] %d\n",token_len));
				if(token_len>0)
				{
					gen_xml_node(id,tok,xml_node,XMLNODELEN);
					PRINT(("[NODE] %s\n",xml_node));
					insert_xml_node(xml_node,language[j].file_path);
				}
			}
			//printf("[DATA] value is %s [%s]\n",tok,language[j].language_id);
		}
		i++;
		tok=strtok(NULL,delim);
	}
}

void create_new_xml_file(const char *file)
{

	FILE *fh=fopen(file,"w");
	//printf("[XML] create new empty file %s\n",file);
	if(fh!=NULL)
	{
		fprintf(fh,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		fprintf(fh,"<resources>\n");
		fprintf(fh,"</resources>\n");
		fclose(fh);
	}
	else
	{
		printf("create new xml file %s failed\n",file);
		exit(-1);
	}
}

void open_file(MULTILAN_STR *str)
{
	//const char *file_path=NULL;
	const char *dir_path=NULL;

	if(strncmp(str->language_id,"ENGLISH",strlen("ENGLISH"))==0)
		dir_path="res/values/";
	else if(strncmp(str->language_id,"CHINESE",strlen("CHINESE"))==0)
		dir_path="res/values-zh-rTW/";
	else if(strncmp(str->language_id,"SCHINESE",strlen("SCHINESE"))==0)
		dir_path="res/values-zh-rCN/";
	else if(strncmp(str->language_id,"JAPANESE",strlen("JAPANESE"))==0)
		dir_path="res/values-ja/";
	else if(strncmp(str->language_id,"GERMAN",strlen("GERMAN"))==0)
		dir_path="res/values-de/";
	else if(strncmp(str->language_id,"SPANISH",strlen("SPANISH"))==0)
		dir_path="res/values-es/";
	else if(strncmp(str->language_id,"FRENCH",strlen("FRENCH"))==0)
		dir_path="res/values-fr/";
	else if(strncmp(str->language_id,"ITALIAN",strlen("ITALIAN"))==0)
		dir_path="res/values-it/";
	else if(strncmp(str->language_id,"KOREAN",strlen("KOREAN"))==0)
		dir_path="res/values-ko/";
	else if(strncmp(str->language_id,"DUTCH",strlen("DUTCH"))==0)
		dir_path="res/values-nl/";
	else if(strncmp(str->language_id,"RUSSIAN",strlen("RUSSIAN"))==0)
		dir_path="res/values-ru/";
	else if(strncmp(str->language_id,"THAI",strlen("THAI"))==0)
		dir_path="res/values-th/";
	else if(strncmp(str->language_id,"HEBREW",strlen("HEBREW"))==0)
		dir_path="res/values-iw/";
	else if(strncmp(str->language_id,"ARABIC",strlen("ARABIC"))==0)
		dir_path="res/values-ar/";

	// can not get mapped file path, should not reach here
	if(dir_path==NULL)
	{
		printf("LANGUAGE ENUM [%s] ERROR, return\n",str->language_id);
		exit(-1);
	}
	char syscmd[SYSCMDLEN];
	// check folder existence
	if (0 != access(dir_path, F_OK))
	{
		if (ENOENT == errno)
		{
			memset(syscmd,0x0,SYSCMDLEN);
			snprintf(syscmd,SYSCMDLEN,"mkdir -p %s",dir_path);
			system(syscmd);
			usleep(1000);
		}
	}

	memset(str->file_path,0x0,FILEPATHLEN);
	snprintf(str->file_path,FILEPATHLEN,"%sstrings.xml",dir_path);

	// check file existence
	if(access(str->file_path,W_OK)<0)
	{
		PRINT(("[HEADER] xml file %s not exist, create a new one.\n",str->file_path));
		create_new_xml_file(str->file_path);
	}

	memset(syscmd,0x0,SYSCMDLEN);
	snprintf(syscmd,SYSCMDLEN,"dos2unix %s",str->file_path);
	system(syscmd);
	usleep(1000);

	PRINT(("[HEADER] open file:%s\n",str->file_path));
#ifdef DEBUG
	dump_xml(str->file_path);
#endif
}


