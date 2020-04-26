#ifndef _RVSD_DBG_MD5_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct MD5state_st {
    unsigned int A,B,C,D;
    unsigned int Nl,Nh;
    unsigned int data[16];
    unsigned int num;
} MD5_CTX;


unsigned char *MD5(
    const unsigned char* d,
    size_t n,
    unsigned char* md
);

#endif
