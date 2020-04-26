#ifndef __OPEN_FILE_H__
#define __OPEN_FILE_H__

#include "stat.h"
//#define SELF_OPEN64
/*
struct statfs64 {
 __u32 f_type;
 __u32 f_bsize;
 __u64 f_blocks;
 __u64 f_bfree;
 __u64 f_bavail;
 __u64 f_files;
 __u64 f_ffree;
 __kernel_fsid_t f_fsid;
 __u32 f_namelen;
 __u32 f_frsize;
 __u32 f_spare[5];
} __attribute__ ((packed,aligned(4)));
*/
#ifdef SELF_OPEN64
int open64(const char *pathname, int flags, mode_t mode = S_IWRITE);
#endif /* end of SELF_OPEN64 */

#endif //!__OPEN_FILE__
