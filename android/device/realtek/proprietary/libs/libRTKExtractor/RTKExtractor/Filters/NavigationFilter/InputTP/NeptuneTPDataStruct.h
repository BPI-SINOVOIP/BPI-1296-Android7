/////////////////////////////////////////////////////////////////////////////
////////////////////////////// data structure for Neptune TP ////////////////
/////////////////////////////////////////////////////////////////////////////


#ifndef __NEPTUNE_TP_DATA_STRUCTURE_H__
#define __NEPTUNE_TP_DATA_STRUCTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Util.h>

/* Note : if program element execeed 64, we will ignore those elements */
#define		NEPTUNETP_MAX_PROGRAM_ELEMENT	64
#define		NEPTUNETP_MAX_SECTION_LEN		1024

typedef struct _section_piece
{
	unsigned char		section[NEPTUNETP_MAX_SECTION_LEN];	// an internal memory to handle the case 
															// when one section is segmented into 2 ts packets
	int					size;				// if not zero, it means a portion of section is seen
	bool				bHasError;			// to indicate if any portion of this section carried in another ts encounters errors. 
	bool				bHeadFound;			// to indicate if section head has been seen
} NeptuneTP_section_piece;
inline bool combineSec(NeptuneTP_section_piece* pSecPiece, unsigned char* pData, int dataSize)
{
	if(pSecPiece->bHasError) return true;
	if(!pSecPiece->bHeadFound)	return true;
	
	if((pSecPiece->size+dataSize) > NEPTUNETP_MAX_SECTION_LEN)
	{
		printf("[%s %d] illegal section size \n", __FILE__, __LINE__);
		return false;
	}
	DASSERT((pSecPiece->size+dataSize) <= NEPTUNETP_MAX_SECTION_LEN);
	memcpy(pSecPiece->section+pSecPiece->size, pData, dataSize);
	pSecPiece->size += dataSize;
	
			
	return true;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////////////// data structure for PMT ///////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef struct _PMT_entry
{
	
	unsigned short  elementPID;		// program element PID (13 bit field)
	unsigned char 	streamType;
		
} NeptuneTP_PMT_entry_t;

typedef struct _PMT
{
	unsigned char		    versionNum;			// 5-bit field
	char					finalValidEntry;	/* the last used PMT entry;
												   -1 means PTM is empty */
	char					audioEntry;			// -1 means unknown
	char					videoEntry;			// -1 means unknown
	char					spuEntry;			// -1 means unknown
	
	unsigned long			audioStreamDescriptor;			// MPEG_PES_DESC_NA means unknown
	unsigned long			videoStreamDescriptor;			// MPEG_PES_DESC_NA means unknown
	unsigned long			spuStreamDescriptor;			// MPEG_PES_DESC_NA means unknown
	
	unsigned short			programNum;
	unsigned short			PCR_PID;			// 13-bit field
	NeptuneTP_PMT_entry_t   PMT[NEPTUNETP_MAX_PROGRAM_ELEMENT];
	NeptuneTP_section_piece secPiece;
} NeptuneTP_PMT_t;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// data structure for PAT ///////////////////////
/////////////////////////////////////////////////////////////////////////////

#define		NEPTUNETP_MAX_PROGRAM			64
typedef struct _PAT_entry
{
	unsigned short 	programNum;
	unsigned short  PMT_PID;			// program_map_PID (13-bit field)
	struct _PAT_entry* next;
	
} NeptuneTP_PAT_entry_t;

typedef struct _PAT
{
	NeptuneTP_PAT_entry_t* head;	
	NeptuneTP_PAT_entry_t* unUsed;
	NeptuneTP_PAT_entry_t  PAT[NEPTUNETP_MAX_PROGRAM];
	unsigned char		   versionNum;	// 5-bit field ( 0 ~ 31 )

	unsigned long			sectionBitMask[8];	/* PAT can be segmented into 256 sections at most.
												   If some bit is set, the corresponding section is duplicate.*/
	NeptuneTP_section_piece secPiece;
} NeptuneTP_PAT_t;

inline void initPAT(NeptuneTP_PAT_t* table)
{
	memset((unsigned char*)table, sizeof(NeptuneTP_PAT_t), 0);
	
	NeptuneTP_PAT_entry_t* curr;
	table->head = NULL;
	curr = table->unUsed = &table->PAT[0];
	
	for(int i = 1; i < NEPTUNETP_MAX_PROGRAM; i++)
	{
		curr->next = &table->PAT[i];
		curr = curr->next;
	}
	curr->next = NULL;
}

inline unsigned short queryPMT_PID(NeptuneTP_PAT_t* table, unsigned short program)
{
	
	NeptuneTP_PAT_entry_t* curr = table->head;
	while(curr)
	{
		if(program == curr->programNum)
			return (curr->PMT_PID & 0x1FFF);
		curr = curr->next;
	}		
	return 0xFFFF;
}


/* add to PAT by insertition sort. */
inline bool addPATEntry(NeptuneTP_PAT_t* table, unsigned short 	programNum, unsigned short  PMT_PID)
{
	// get one empty entry
	NeptuneTP_PAT_entry_t* entry = table->unUsed;
	if(entry == NULL)	return false;
	table->unUsed = entry->next;
	entry->next = NULL;
	entry->programNum = programNum;
	entry->PMT_PID = PMT_PID;
	
	// add to PAT
	if(table->head)
	{
		NeptuneTP_PAT_entry_t* prev = NULL;
		NeptuneTP_PAT_entry_t* curr = table->head;
		while(curr)
		{
			if(curr->programNum == programNum)
			{
				// push back to unUsed list
				entry->next = table->unUsed;
				table->unUsed = entry; 
				return false;
			}
			if(curr->programNum > programNum)
			{
				if(prev)
				{
					entry->next = curr;
					prev->next = entry;
				}
				else
				{
					entry->next = curr;
					table->head = entry;
				}
				return true;
			}
			prev = curr;
			curr = curr->next;
			if(curr == NULL)//ADDED AT THE TAIL
			{
				prev->next = entry;
				return true;
			
		}
		}
		
		return true;
	}
	else
	{
		table->head = entry;
		return true;
	}
}
#endif
