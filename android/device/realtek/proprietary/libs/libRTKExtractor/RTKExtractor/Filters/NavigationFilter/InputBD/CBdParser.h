#ifndef __C_BD_PARSER_H__
#define __C_BD_PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include "mpls_parse.h"
#include "clpi_parse.h"

typedef enum {PLAYLIST, CLIPINF, STREAM} BD_FILE_TYPE;

class CBdParser
{
public:
	CBdParser() : m_mediaPath(0) {}
	~CBdParser();
	
	inline bool init(char* stream_dir) {m_mediaPath = strdup(stream_dir);	return (m_mediaPath != 0); }
	
	FILE* fopenFile(BD_FILE_TYPE type, int fileId);
	int openLargeFile(BD_FILE_TYPE type, int fileId, bool bDirectIO);
	
	void loadFileIdList(int type, int &fileIdNum, int* fileIdList);
	bool bdt_mpls_parse(int fileId, bdt_pl_t *pl);
	void bdt_mpls_free(bdt_pl_t *pl);
	bool parse_playlist(FILE *stream, bdt_pl_t *pl);
	bool parse_playitem(FILE *stream, bdt_pi_t *pi);
	bool bdt_clpi_parse(int fileId, bdt_cl_t *cl);
	void bdt_clpi_free(bdt_cl_t *cl);
		
protected:

	char*			m_mediaPath;
};

#endif//__C_BD_PARSER_H__

