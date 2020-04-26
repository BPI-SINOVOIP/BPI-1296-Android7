#ifndef __GFASTCOPY_H__
#define __GFASTCOPY_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <stddef.h>     /* for NULL */
#include <string.h>
#include <stdlib.h>     /* for exit() */
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <linux/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include "file_access.h"

typedef enum fastcopy_filetype {
    GFILE_ARRAY = GFILE_TYPE_ARRAY,
    GFILE_LIST = GFILE_TYPE_LIST,
    GFILE_ARRAY_RING,
    NORMAL_FILE=10,
} FASTCOPY_FILETYPE;

typedef enum fastcopy_state {
    FCOPY_NOT_RUN,
    FCOPY_RUNNING,
    FCOPY_COMPLETE,
    FCOPY_ABORT,         /* copy aborted, by user or error */
    FCOPY_ERR,           /* err, cannot recover */
} FASTCOPY_STATE;

// define all the error codes
#define FCOPY_ERR_INTERNAL    -1 /* internal operation failure in Fastcopy */
#define FCOPY_ERR_SRC_BAD     -2 /* src file is bad (non-exist or non-regular)*/
#define FCOPY_ERR_SPACE       -3 /* insufficient space on disk */
#define FCOPY_ERR_DST_BAD     -4 /* dst is bad (non-regular file or no perm) */
#define FCOPY_ERR_PATH        -5 /* path name unreachable */

class GFastCopy {
   private:
      off64_t m_source_size;
      pthread_t  thread_id;
      void * threadctx;
      FASTCOPY_STATE state;
      off64_t written;
      static struct sembuf readrun_give;
      pthread_mutex_t notify_cond;

      static void* thread_write(void* arg);
      static void* thread_read(void *arg);

      void clean_ctx(void);

   public:
      GFastCopy(void);
      ~GFastCopy(void);
      
      // wait msec for completion of Copy task, return 0 if copy 
      // task is done during wait.  if msec=0, wait copy task is
      // finished.
      int wait(unsigned int msec);
		
      // async copy of src file to dst file.  returns 0 if
      // copy is successfully started.
      int Copy(char *src, char *dst ,FASTCOPY_FILETYPE src_ftype=NORMAL_FILE,FASTCOPY_FILETYPE dst_ftype=NORMAL_FILE, int64_t startPos=0, int64_t endPos=-1);

      // check the status of the current copy task.
      FASTCOPY_STATE get_status(void);

      // check how many bytes have been written
      off64_t get_written(void);
      off64_t get_SourceSize(void) { return m_source_size;}

      // abort the current copy task
      void abort(void);
};

#endif
