
#ifndef _SI_PVR_H
#define _SI_PVR_H


#include <system/si4/SITypes.h>


//first 'e' is for guard
//ver 1: size 520
#define SI_PVR_IFO_VER			0xe0000001

//#define SI_PVR_IFO_ATSC			
#define SI_PVR_IFO_TAG_DVB		0xab000000

//store si channel data into 
typedef struct _SI_PVR_IFO
{
	//unsigned int 				siType;		//atsc or dvb
	unsigned int 				version;		//not used currently(SI_PVR_IFO_VER)
	SI_CHANNEL_VERSION 		chVer;		//si channel version
	
	SI_CHANNEL_EXT	ch;
} SI_PVR_IFO;





#endif//_SI_PVR_H
