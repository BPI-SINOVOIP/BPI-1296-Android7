#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fastcopy_wrapper.h"
// #include "GFastCopy.h"

#define BUF_SIZE 1024

void do_fastcopy(char *srcFileName, char *dstFileName, int64_t start, int64_t end) {
    if (end < start) return;
    
    FILE *fSrc, *fDst;
    int iTotalSize = end - start + 1;
    char buf[BUF_SIZE];
    size_t iReadSize = 0;
    
    if (!(fSrc = fopen(srcFileName, "r"))) return;
    
    if (!(fDst = fopen(dstFileName, "a+"))) return;
    
    if (start > 0)
    {
        fseek(fSrc, start, SEEK_SET);
        fseek(fDst, start, SEEK_SET);
    }
    
    while(1)
    {
        iReadSize = fread(buf, 1, sizeof(buf), fSrc);
        iReadSize = iTotalSize < sizeof(buf) ? iTotalSize : iReadSize;
        fwrite(buf, iReadSize, 1, fDst);
        
        if (iReadSize < sizeof(buf))
            break;
            
        iTotalSize -= sizeof(buf);
    }
    
    fclose(fSrc);
    fclose(fDst);
}