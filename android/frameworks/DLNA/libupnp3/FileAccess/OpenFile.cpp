#include <sys/syscall.h>
#include "OpenFile.h"

#define	SYS_open64 225
#define _open64(fn, flags, mode)	(syscall(SYS_open64, (fn), (flags), (mode)))

#ifdef SELF_OPEN64
int open64(const char *pathname, int flags, mode_t mode)
{
    return _open64(pathname, flags, mode);
}
#endif /* end of SELF_OPEN64 */
