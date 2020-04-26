
#ifndef _SI_AGENT_TYPES_H_
#define _SI_AGENT_TYPES_H_

#include <SITypes_local.h>


struct SIAGENT_OBJECT_ ;
typedef struct SIAGENT_OBJECT_       SIAGENT_OBJECT_T, *SIAGENT_OBJECT_P ;

struct SI_DATABASE_TS_NODE ;
typedef	struct SI_DATABASE_TS_NODE *TABLE_TREE_TS_LAYER_P;


//from siFunctionWrapper.h
typedef struct SI_DATABASE_PROGRAM_INFO       PROGRAM_INFO_T, *PROGRAM_INFO_P ;
typedef struct SI_DATABASE_PROGRAM_INFO_COLLECTION       PROGRAMS_INFO_T, *PROGRAMS_INFO_P ;



#ifndef bool
typedef int bool;
#endif//bool ??

#endif//_SI_AGENT_TYPES_H_
