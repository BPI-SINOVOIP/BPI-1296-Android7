#ifndef _SI_BEAN_GEN_H_
#define _SI_BEAN_GEN_H_

#include <ss_typedef4integration.h>
#include <tpInterface/SI_pvr_tpInterface.h>

//*************************************************************
// DEFINE
//*************************************************************

#define BEANGEN_FILTERNUM				32

typedef struct _BEANGEN_FILTERINFO
{
	unsigned int						pid;
	unsigned char						tid;
	unsigned int						remainedSecLen;
} BEANGEN_FILTERINFO;


typedef	struct _BEAN_GENERATOR
{
	BEANGEN_FILTERINFO					filterInfo[BEANGEN_FILTERNUM];
} BEAN_GENERATOR;



//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
ErrCode SI_BeanGen_New(BEAN_GENERATOR **ppBeanGen);
ErrCode SI_BeanGen_Reset(BEAN_GENERATOR *pBeanGen);
ErrCode SI_BeanGen_Destroy(BEAN_GENERATOR **ppBeanGen);

ErrCode SI_BeanGen_EnablePid(BEAN_GENERATOR *pBeanGen, unsigned int pid);
ErrCode SI_BeanGen_DisablePid(BEAN_GENERATOR *pBeanGen, unsigned int pid);

ErrCode SI_BeanGen_Convert(BEAN_GENERATOR *pBeanGen, unsigned char **dstBuf, unsigned int *dstLen, unsigned char **srcbuf, unsigned int *srcLen);


#endif
