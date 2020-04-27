/*
 * xml_util.c
 *
 *  Created on: 2014/3/4
 *      Author: bruce_huang
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "xml_util.h"

#define	NODELEN		2048
#define	SYSCMDLEN	128
#define METAXMLFILE	".meta.xml"

#define PRINT(x)	//printf x

void dump_xml(char *file_path)
{
	FILE *fp=fopen(file_path,"r");
	if(fp!=NULL)
	{
		printf("[XML][DEBUG] dump file:\n\n");
		char buf[NODELEN];
		memset(buf,0x0,NODELEN);
		while(fgets(buf,NODELEN,fp)!=NULL)
		{
			printf("%s",buf);
			memset(buf,0x0,NODELEN);
		}
		fclose(fp);
	}
}

void gen_xml_node(char *id, char *value, char *out, int out_buf_len)
{
	memset(out,0x0,out_buf_len);
	snprintf(out,out_buf_len,"    <string name=\"%s\">%s</string>",id,value);
}

// 1. find if node already exist, if does, remove it.
// 2. remove tailing </resources> line
// 3. insert node into end of file.
// 4. insert </resources> into end of file.
// 5. write back to file
void insert_xml_node(char *node, char *file_path)
{
	FILE *fh=fopen(file_path,"r");
	int opt_success=-1;

	if(fh!=NULL)
	{
		FILE *ftmp = fopen(METAXMLFILE,"w");
		if(ftmp!=NULL)
		{
			char line[NODELEN];
			memset(line,0x0,NODELEN);
			while(fgets(line,NODELEN,fh)!=NULL)
			{
				remove_tailing_newline(line);
				if(is_target_node(line,node)>0 || is_end_tag(line)>0)
				{
					PRINT(("[TARGET] %s\n",line));
					continue;
				}
				else
				{
					// write to new.xml file:
					fprintf(ftmp,"%s\n",line);
				}
			}

			fprintf(ftmp,"%s\n</resources>\n",node);
			fclose(ftmp);
			opt_success=1;
		}

		fclose(fh);

		// finally, write back to original xml file.
		char syscmd[SYSCMDLEN];
		memset(syscmd,0x0,SYSCMDLEN);
		snprintf(syscmd,SYSCMDLEN,"mv %s %s",METAXMLFILE,file_path);
		system(syscmd);
		usleep(1000);
	}
}

void remove_space_tab_newline(char *src, char *dst)
{
	int src_len = strlen(src);
	int i=0;
	int j=0;
	for(i=0;i<src_len;i++)
	{
		if(src[i]==' '||src[i]=='\t'||src[i]=='\n'||src[i]=='\r')
			continue;
		else
		{
			dst[j]=src[i];
			j++;
		}
	}
	//printf("[REMOVE CHAR] src: %s\n[REMOVE] dst: %s\n",src,dst);
}

int is_end_tag(char *line)
{
	char line2[NODELEN];
	memset(line2,0x0,NODELEN);
	remove_space_tab_newline(line,line2);

	if(strcmp(line2,"</resources>")==0)
		return 1;
	else
		return -1;
}

int is_target_node(char *line, char *node)
{
	char line2[NODELEN];
	char node2[NODELEN];

	memset(line2,0x0,NODELEN);
	memset(node2,0x0,NODELEN);

	remove_space_tab_newline(line,line2);
	remove_space_tab_newline(node,node2);

	// get end char position of node, find first '>'
	int i=0;
	while(node2[i]!='>')
	{
		i++;
	}
	int cmp_len = i+1;
	if(strncmp(line2,node2,cmp_len)==0)
		return 1;
	else
		return -1;

}

void remove_tailing_newline(char *data)
{
	int pos = strlen(data)-1;
	if(data[pos]=='\n')
		data[pos]='\0';
}
