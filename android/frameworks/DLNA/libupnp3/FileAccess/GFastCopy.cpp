/*
 * fastcopy class. This class uses 2 threads to maximize copy speed
 * from one device to another. One thread is engaged in reading only,
 * and the other thread is engaged in writting only. Direct IO is 
 * used for read/write, and if possible, physically continuous memory
 * is used to make DMA faster.
 * Yuefeng Liu
 */

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif    
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <linux/sem.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/vfs.h>
// #include <Util.h>

// #include "OSAL/PLI.h"
#include "GFastCopy.h"

//#define jsprintf(format,arg...)  {DP(INFO_LEVEL,format,## arg);}
#define jsprintf(format,arg...)  {printf(format,## arg);}
//#define jsprintf(format,arg...)  {;}
#define GFILE_EXT ".mpg"

#define MAX_BUF_USED     2
#define MAX_BUF_SIZE     64*1024  
#define MAX_LOCKS        4

#define READ_LOCK        0
#define WRITE_LOCK       1
#define READ_RUN         2
#define WRITE_RUN        3

#ifdef _SEM_SEMUN_UNDEFINED
union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux specific) */
};
#endif

/*
 * define private struture/data to be passed between threads
 */

typedef enum t_state {
    THREAD_INIT,
    THREAD_RUN,
    THREAD_ABORT,   /* ask thread to abort, or thread run into err */
    THREAD_DONE,    /* thread is finished with task */
    THREAD_ERR,     /* thread encountered unrecoverable error */
    THREAD_DIE,     /* tell thread to die */
} THREAD_STATE;

typedef enum m_type {
    PLI_MEM,
    MAL_MEM,
} M_TYPE;

struct thread_buffer {
  off64_t size;
  char *buf;
  M_TYPE type;
};

struct thread_runner {
  char srcfile[PATH_MAX];
  char dstfile[PATH_MAX];
  FASTCOPY_FILETYPE srcfile_type;
  FASTCOPY_FILETYPE dstfile_type;
  int64_t m_startPos;
  int64_t m_endPos;
  
  int sem_id;
  /* use this sem to notify thread done (done, abort, die) */
  pthread_mutex_t * notify_sem_id;
  off64_t *written;
  THREAD_STATE volatile tstate;
  struct thread_buffer * buf_stat[MAX_BUF_USED];
};

struct sembuf GFastCopy::readrun_give = {READ_RUN, +1, 0};


/*
 * constructor, here we allocate memory to be used
 * by this class, and spawn the read and write thread.
 */
GFastCopy::GFastCopy(void) {
    struct thread_runner *thread;
    union semun arg;
    int i;

    state = FCOPY_NOT_RUN;
    written = 0;
    m_source_size=0;
    thread = (struct thread_runner *) malloc(sizeof(struct thread_runner));
    if (thread == NULL) {
        printf("malloc failure\n");
        return;
    }
    threadctx = thread;
    thread->tstate = THREAD_INIT;
    thread->written = &written;
    /* start initialize the semaphores */      
    thread->sem_id = semget(IPC_PRIVATE, MAX_LOCKS, 0600);
    if (thread->sem_id < 0) {
        printf("sem id alloc fail\n");
        free(threadctx);
        threadctx = NULL;
        state = FCOPY_ERR;
        return;
    }
    ushort initv[MAX_LOCKS] = {MAX_BUF_USED, 0, 0, 0};
    arg.array = initv;
    if (semctl(thread->sem_id, MAX_LOCKS, SETALL, arg) == -1) {
        printf("sem id setall fail\n");
        semctl(thread->sem_id, 0, IPC_RMID, arg);
        free(threadctx);
        threadctx = NULL;
        state = FCOPY_ERR;
        return;
    }

    /* start initialize all memory buffers */
    for (i=0; i<MAX_BUF_USED; i++) {
        thread->buf_stat[i] = (struct thread_buffer*) 
                              malloc(sizeof(struct thread_buffer));
        thread->buf_stat[i]->size = 0;
        thread->buf_stat[i]->buf = (char*) pli_allocContinuousMemory(MAX_BUF_SIZE, 0, 0);
        if (thread->buf_stat[i]->buf == NULL) {
            thread->buf_stat[i]->buf = (char*) memalign(getpagesize(), MAX_BUF_SIZE);
            if (thread->buf_stat[i]->buf == NULL) {
                printf("unable to alloc buf\n");
                clean_ctx();
                state = FCOPY_ERR;
                return;
            }
            thread->buf_stat[i]->type = MAL_MEM;
        } else {
            thread->buf_stat[i]->type = PLI_MEM;
        }
    }

    pthread_mutex_init(&notify_cond, NULL);
    thread->notify_sem_id = &notify_cond;
    if (pthread_create(&thread_id, NULL, thread_read, (void *)thread) != 0) {
        clean_ctx();
        pthread_mutex_destroy(&notify_cond);
        printf("thread read create fail\n");
        state = FCOPY_ERR;
    }
}

int GFastCopy::wait(unsigned int msec) {
    struct timespec timeout;
    struct timeval cur;
    int code;

    if (state == FCOPY_NOT_RUN || state == FCOPY_ERR) 
        return 0;

    if (msec == 0) {
        code = pthread_mutex_lock(&notify_cond);
        pthread_mutex_unlock(&notify_cond);
        return code;
    }

    gettimeofday(&cur, 0);
    
    timeout.tv_sec = cur.tv_sec + msec/1000;
    timeout.tv_nsec = cur.tv_usec*1000 + (msec % 1000) * 1000000;
    code = pthread_mutex_timedlock(&notify_cond, &timeout);
    if (code == 0) 
        pthread_mutex_unlock(&notify_cond);
    return code;
}
  
GFastCopy::~GFastCopy(void) {
    struct thread_runner *thread;
    void *token;
    printf("deconstructor called \n");

    thread = (struct thread_runner *) threadctx;

    if (thread) {
        this->abort();
        thread->tstate = THREAD_DIE;
        thread->notify_sem_id = &notify_cond;
        pthread_mutex_lock(&notify_cond);
        semop(thread->sem_id, &readrun_give, 1);
        pthread_join(thread_id, &token);
    
        clean_ctx();
        pthread_mutex_destroy(&notify_cond);
    }
    return;
}

void* GFastCopy::thread_write(void* arg) {
    struct thread_runner *thread = (struct thread_runner *) arg;
    int i, fd = -1, ret, flags, remove=0;
    gfile_t *gfile=0;
    off64_t size, towrite;
    static struct sembuf readgive = {READ_LOCK, +1, 0};
    static struct sembuf writewait = {WRITE_LOCK, -1, 0};
    static struct sembuf writerun_wait = {WRITE_RUN, -1, 0};

    setpriority(PRIO_PROCESS, 0, 3);

    while (1) {
        ret = semop(thread->sem_id, &writerun_wait, 1);

        printf("writer start copying job\n");
        remove = 0;

        if (ret == -1) {
            printf("op wait on write run fail\n");
            continue;
        }
      
        if (thread->tstate == THREAD_ABORT) 
            goto abort;

        if (thread->tstate == THREAD_DIE)
            goto die;

        jsprintf("Destination File : %s \n",thread->dstfile);
        if(thread->dstfile_type == NORMAL_FILE)
        {
            flags = O_DIRECT|O_CREAT|O_TRUNC|O_RDWR;
            fd = open(thread->dstfile, flags);
            if (fd < 0) {
                printf("open dst file O_DIRECT for write failed, use buffer io\n");
                flags = O_CREAT|O_TRUNC|O_RDWR;
                fd = open(thread->dstfile, flags);
                if (fd < 0) {
                    printf("dst file unwritable, ABORT!\n");
                    thread->tstate = THREAD_ABORT;
                    goto abort;
                }
            }
        }
        else
        {
            flags = O_DIRECT|O_CREAT|O_TRUNC|O_RDWR;
            if((gfile=GFileOpen(thread->dstfile,flags,(ENUM_GFILE_TYPE)thread->dstfile_type))==NULL)
            {
                printf("open dst gfile O_DIRECT for write failed, use buffer io\n");
                flags = O_CREAT|O_TRUNC|O_RDWR;
                if((gfile=GFileOpen(thread->dstfile,flags,(ENUM_GFILE_TYPE)thread->dstfile_type))==NULL)
                {
                    printf("dst gfile unwritable, ABORT!\n");
                    thread->tstate = THREAD_ABORT;
                    goto abort;
                }
            }
        }
        
        remove = 1;
        i = 0;
        while (1) {
            ret = semop(thread->sem_id, &writewait, 1); /* write lock */

            if (ret < 0) 
                continue;

            if (thread->tstate == THREAD_ABORT) {
                printf("copy write aborted\n");
                goto abort;
            }
            if (thread->tstate == THREAD_DIE) {
                printf("thread writer die\n");
                goto die;
            }

            if (thread->buf_stat[i]->size <= 0) {
                printf("write complete\n");
                thread->tstate = THREAD_DONE;
                pthread_mutex_unlock(thread->notify_sem_id);
                if(thread->dstfile_type == NORMAL_FILE)
                {
                    close(fd);
                    fd=-1;
                }
                else
                {
                    GFileClose(gfile);
                    gfile=0;
                }
                break;
            }
            if (thread->buf_stat[i]->size == 
                ((thread->buf_stat[i]->size >> 10) << 10)) {
                towrite = 0;
                while (towrite < thread->buf_stat[i]->size) {
                     /* 1k aligned size, write directly */
                    if(thread->dstfile_type == NORMAL_FILE)
                    {
                         size = write(fd, thread->buf_stat[i]->buf + towrite, 
                                  thread->buf_stat[i]->size - towrite);
                    }
                    else
                    {
                         size = GFileWriteBytes(gfile, thread->buf_stat[i]->buf + towrite, 
                                  thread->buf_stat[i]->size - towrite);
                    }
                     //printf("%dW\n", W++);
                     if (size <= 0) {
                         printf("nothing written (disk full?) abort!!!\n");
                         goto abort;
                     }
                     towrite += size;
                }
            } else {
                /* non-aligned size, are we using O_DIRECT? */
                if (flags & O_DIRECT) {
                    /* yes, O_DIRECT flag set for write*/
                    if(thread->dstfile_type == NORMAL_FILE)
                    {
                        if(fd>=0)
                        {
                            close(fd);
                            fd=-1;
                        }
                        flags = O_APPEND|O_RDWR;
                        fd = open(thread->dstfile, flags);
                        towrite = 0;
                        while (towrite < thread->buf_stat[i]->size) {
                             size = write(fd, thread->buf_stat[i]->buf + towrite, 
                                      thread->buf_stat[i]->size - towrite);
                             if (size <= 0) {
                                 printf("write size fail\n");
                                 goto abort;
                             }
                             towrite += size;
                        }
                    }
                    else
                    {
#if 0                 
// this is for gfile support open write againg funciton
                        if(gfile)
                        {
                            GFileClose(gfile);
                            gfile=0;
                        }
                        flags = O_APPEND|O_RDWR;
                        gfile=GFileOpen(thread->dstfile,flags,(ENUM_GFILE_TYPE)thread->dstfile_type);
                        towrite = 0;
                        while (towrite < thread->buf_stat[i]->size) {
                                 size = GFileWriteBytes(gfile, thread->buf_stat[i]->buf + towrite, 
                                          thread->buf_stat[i]->size - towrite);
                             if (size <= 0) {
                                 printf("write size fail\n");
                                 goto abort;
                             }
                             towrite += size;
                        }
#else
// this is for gfile unsupport open write againg funciton
                        towrite = 0;
                        off64_t write_size=((thread->buf_stat[i]->size >> 10) << 10)+1024;
                        while (towrite < thread->buf_stat[i]->size) {
                                 size = GFileWriteBytes(gfile, thread->buf_stat[i]->buf + towrite, 
                                          write_size - towrite);
                                 if (size <= 0) {
                                     printf("write error, abort\n");
                                     goto abort;
                                 }
                                 towrite += size;
                        }
#endif
                    }
                } else {
                    /* not using O_DIRECT, just write */
                    towrite = 0;
                    while (towrite < thread->buf_stat[i]->size) {
                        if(thread->dstfile_type == NORMAL_FILE)
                        {
                             size = write(fd, thread->buf_stat[i]->buf + towrite, 
                                      thread->buf_stat[i]->size - towrite);
                        }
                        else
                        {
                             size = GFileWriteBytes(gfile, thread->buf_stat[i]->buf + towrite, 
                                      thread->buf_stat[i]->size - towrite);
                        }
                         if (size <= 0) {
                             printf("write error, abort\n");
                             goto abort;
                         }
                         towrite += size;
                    }
               }
            }
            *(thread->written) += thread->buf_stat[i]->size;
            i ++; i = i % MAX_BUF_USED;
            ret = semop(thread->sem_id, &readgive, 1);
        }
        continue;

    abort:        
        printf("writer has aborted\n");
        thread->tstate = THREAD_ABORT;
        if(thread->dstfile_type == NORMAL_FILE)
        {
            if (fd >= 0) {
                close(fd); fd = -1;
            }
            if (remove) 
                unlink(thread->dstfile);
        }
        else
        {
            if (gfile > 0) {
                GFileClose(gfile); gfile = 0;
            }
            if (remove) 
                GFileDelete(thread->dstfile);
        }
        
        /* write initiate abort, notify that this thread has halted */
        semop(thread->sem_id, &readgive, 1); /* reader to wake up and abort */
        pthread_mutex_unlock(thread->notify_sem_id);
    }
 die:
    thread->tstate = THREAD_DIE;
    if(thread->dstfile_type == NORMAL_FILE)
    {
        if (fd >= 0) {
            close(fd); fd = -1;
        }
        if (remove) 
            unlink(thread->dstfile);
    }
    else
    {
        if (gfile > 0) {
            GFileClose(gfile); gfile = 0;
        }
        if (remove) 
            GFileDelete(thread->dstfile);
    }
   
    /* notify this thread is halted */
    pthread_mutex_unlock(thread->notify_sem_id);
    printf("writer thread quits!\n");
    return NULL;
}


void* GFastCopy::thread_read(void* arg) {
    struct thread_runner *thread = (struct thread_runner *) arg;
    struct thread_runner *writer;
    pthread_t  thread_wr;
    static struct sembuf readwait = {READ_LOCK, -1, 0};
    static struct sembuf writegive = {WRITE_LOCK, +1, 0}; 
    static struct sembuf readrun_wait = {READ_RUN, -1, 0};
    static struct sembuf writerun_give = {WRITE_RUN, +1, 0};
    int i, fd = -1, ret;
    off64_t pos;
    gfile_t *gfile=0;
    void *token;
    pthread_mutex_t writer_cond;

    writer = (struct thread_runner *) malloc(sizeof(struct thread_runner));
    if (writer == NULL) {
        thread->tstate = THREAD_ERR;
        return NULL;
    }

    pthread_mutex_init(&writer_cond, NULL);
    memcpy(writer, thread, sizeof(struct thread_runner));
    writer->notify_sem_id = &writer_cond;
    if (pthread_create(&thread_wr, NULL, thread_write, 
                       (void *)writer) != 0) {
        thread->tstate = THREAD_ERR;
        return NULL;
    }

    while (1) {
        /* forever loop, waiting for a copy task to be assigned */
        ret = semop(thread->sem_id, &readrun_wait, 1);

        printf("reader begin copying task\n");
        if (ret == -1) {
            printf("op wait on readrun fail\n");
            continue;
        }
        
        if (thread->tstate == THREAD_DIE) {
            printf("thread die!\n");
            goto die;
        }

        if (thread->tstate == THREAD_ABORT) {
            printf("abort!, state\n");
            goto abort;
        }

        jsprintf("Source File : %s \n",thread->srcfile);

        if(thread->srcfile_type == NORMAL_FILE)
        {
            fd = open(thread->srcfile, O_DIRECT|O_RDONLY);
            if (fd < 0) {
                printf("src file O_DIRECT open fail, try buffer!\n");
                fd = open(thread->srcfile, O_RDONLY);
                if (fd < 0) {
                    printf("src file cannot open for read, ABORT!!\n");
                    thread->tstate = THREAD_ABORT;
                    goto abort;
                }
            }
            if ((thread->m_startPos != 0) && (lseek(fd, thread->m_startPos, SEEK_SET) != thread->m_startPos)) {
                printf("src file seek fail\n");
                thread->tstate = THREAD_ABORT;
                goto abort;
            }
        }
        else if (thread->srcfile_type == GFILE_ARRAY_RING) 
        {
            //@FIXME: if fail, not change to buffer I/O here..
            if ((gfile = GFileOpenRING(thread->srcfile, O_DIRECT|O_RDONLY, 0, GFILE_TYPE_ARRAY)) == NULL) {
                printf("GFileOpenRING fail\n");
                thread->tstate = THREAD_ABORT;
                goto abort;
            }
            if (GFileSeek(gfile, thread->m_startPos, SEEK_SET) != thread->m_startPos) {
                printf("src FileSeek to %lld fail\n", thread->m_startPos);
                thread->tstate = THREAD_ABORT;
                goto abort;
            }
        }
        else
        {
            if((gfile=GFileOpen(thread->srcfile,O_DIRECT|O_RDONLY,(ENUM_GFILE_TYPE)thread->srcfile_type))==NULL)
            {
                printf("src gfile O_DIRECT open fail, try buffer!\n");
                if((gfile=GFileOpen(thread->srcfile,O_RDONLY,(ENUM_GFILE_TYPE)thread->srcfile_type))==NULL)
                {
                    printf("src gfile cannot open for read, ABORT!!\n");
                    thread->tstate = THREAD_ABORT;
                    goto abort;
                }
            }            
            if ((thread->m_startPos != 0) && (GFileSeek(gfile, thread->m_startPos, SEEK_SET) != thread->m_startPos)) {
                printf("src file seek fail\n");
                thread->tstate = THREAD_ABORT;
                goto abort;
            }
        }
        memcpy(writer->dstfile, thread->dstfile, 
               PATH_MAX);
        writer->dstfile_type=thread->dstfile_type;
        
        pthread_mutex_lock(writer->notify_sem_id);
        writer->tstate = THREAD_RUN;
        ret = semop(thread->sem_id, &writerun_give, 1); 
        if (ret < 0) {
            printf("sem op on writerun fail\n");
            goto abort;
        }
        i = 0;
        while (1) {
            /* take a read lock */
            ret = semop(thread->sem_id, &readwait, 1);
            if (ret < 0) 
                continue;
            if (thread->tstate == THREAD_ABORT || 
                writer->tstate == THREAD_ABORT) {
                printf("copy aborted\n");
                goto abort;
            }

            if (thread->tstate == THREAD_DIE) {
                printf("copy thread die\n");
                goto die;
            }
            if(thread->srcfile_type == NORMAL_FILE)
            {
                thread->buf_stat[i]->size = read(fd, thread->buf_stat[i]->buf, 
                                             MAX_BUF_SIZE);
                pos = lseek64(fd, 0, SEEK_CUR);                                              
            }
            else
            {
                if (pos >= thread->m_endPos)
                    thread->buf_stat[i]->size = 0;
                else {   
                    if ((pos + MAX_BUF_SIZE) > thread->m_endPos)
                        thread->buf_stat[i]->size = GFileReadBytes(gfile, thread->buf_stat[i]->buf, 
                                                    (thread->m_endPos - pos));
                    else    
                        thread->buf_stat[i]->size = GFileReadBytes(gfile, thread->buf_stat[i]->buf, 
                                                    MAX_BUF_SIZE);
                }
                pos = GFileSeek(gfile, 0, SEEK_CUR);                              
            }
            //printf("%dR\n", R++);
            if (thread->buf_stat[i]->size <= 0) {
                /* we are done reading this file already */
                printf("done reading!\n");

                semop(thread->sem_id, &writegive, 1);
                pthread_mutex_lock(writer->notify_sem_id);
                pthread_mutex_unlock(writer->notify_sem_id);
                thread->tstate = writer->tstate;
                if(thread->srcfile_type == NORMAL_FILE)
                {
                    close(fd);
                    fd=-1;
                }
                else
                {
                    GFileClose(gfile);
                    gfile=0;
                }
                pthread_mutex_unlock(thread->notify_sem_id);
                break;
            }
            i++; i = i % MAX_BUF_USED;

            semop(thread->sem_id, &writegive, 1);
        }
        continue;

    abort:
        if (writer->tstate != THREAD_ABORT) {
            writer->tstate = THREAD_ABORT;

            semop(thread->sem_id, &writegive, 1);
        }
        /* wait for writer to abort */
        pthread_mutex_lock(writer->notify_sem_id);
        pthread_mutex_unlock(writer->notify_sem_id);
        thread->tstate = THREAD_ABORT;
        if(thread->srcfile_type == NORMAL_FILE)
        {
            if (fd >= 0) {
                close(fd); fd = -1;
            }
        }
        else
        {
            if (gfile > 0) {
                GFileClose(gfile); gfile = 0;
            }
        }
        /* notify any thread waiting on me to abort */
        pthread_mutex_unlock(thread->notify_sem_id);
    }

 die:
    writer->tstate = THREAD_DIE;
    semop(thread->sem_id, &writerun_give, 1); 

    semop(thread->sem_id, &writegive, 1);

    /* wait for writer to die */
    pthread_join(thread_wr, &token);
    if(thread->srcfile_type == NORMAL_FILE)
    {
        if (fd >= 0) {
            close(fd); fd = -1;
        }
    }
    else
    {
        if (gfile > 0) {
            GFileClose(gfile); gfile = 0;
        }
    }
    pthread_mutex_destroy(writer->notify_sem_id);
    free(writer);
    /* notify this thread is done */
    pthread_mutex_unlock(thread->notify_sem_id);
    printf("read thread quit!\n");
    return NULL;
}

void GFastCopy::clean_ctx(void) {
    struct thread_runner *thread = (struct thread_runner *) threadctx;
    int i;
    union semun arg;

    semctl(thread->sem_id, 0, IPC_RMID, arg);
    for (i=0; i<MAX_BUF_USED; i++) {
        if (thread->buf_stat[i]->buf) {
            if (thread->buf_stat[i]->type == PLI_MEM) 
                pli_freeContinuousMemory(thread->buf_stat[i]->buf);
            else 
                free(thread->buf_stat[i]->buf);
        }
        free(thread->buf_stat[i]);
        break;
    }

    free(threadctx);
    threadctx = NULL;
}

static int space_ok (struct stat64 *src, char *dst) {
    struct statfs64 dstfs;
    off_t dstsize;
    if (statfs64(dst, &dstfs) < 0) {
        /*
         * we return OK here, some fs does not support
         * statfs. if insufficient space is encoutered,
         * copy will abort
         */
        return 1;
    }
    dstsize = dstfs.f_bavail * dstfs.f_bsize;
    printf("src size %lld, dst avail %lld\n", src->st_size, dstsize); 
    if (src->st_size > dstsize) {
        return 0;
    }

    return 1;
}

static int space_ok2 (off64_t src_size, char *dst) {
    struct statfs64 dstfs;
    off_t dstsize;
    if (statfs64(dst, &dstfs) < 0) {
        /*
         * we return OK here, some fs does not support
         * statfs. if insufficient space is encoutered,
         * copy will abort
         */
        return 1;
    }
    dstsize = dstfs.f_bavail * dstfs.f_bsize;
    printf("src size %lld, dst avail %lld\n", src_size, dstsize); 
    if (src_size > dstsize) {
        return 0;
    }

    return 1;
}

int GFastCopy::Copy(char *src, char *dst ,FASTCOPY_FILETYPE src_ftype,FASTCOPY_FILETYPE dst_ftype,
                    int64_t startPos, int64_t endPos)
{

    struct stat64 buf;
    struct stat64 srcbuf;
    char *filename;
    int fd;
    struct thread_runner *thread;
    thread = (struct thread_runner *) threadctx;
    union semun arg;
    

    thread->srcfile_type=src_ftype;
    thread->dstfile_type=dst_ftype;

    if (thread->tstate == THREAD_RUN || state == FCOPY_ERR) 
        return FCOPY_ERR_INTERNAL;

    if ((strlen(src) > PATH_MAX) || (strlen(dst) > PATH_MAX)) 
        return FCOPY_ERR_PATH;

    /* src file sanity check */
    if(thread->srcfile_type==NORMAL_FILE)
    {
        if (stat64(src, &srcbuf) != 0) {
            /* src not exist! */
            return FCOPY_ERR_SRC_BAD;
        }
        if (S_ISDIR(srcbuf.st_mode) || S_ISFIFO(srcbuf.st_mode) || 
            S_ISSOCK(srcbuf.st_mode)) {
            return FCOPY_ERR_SRC_BAD;
        }
    }
    if (thread == NULL) {
        return FCOPY_ERR_INTERNAL;
    }

    snprintf(thread->srcfile, PATH_MAX, "%s", src);

    if (stat64(dst, &buf) == 0) {
        printf("dst file exists, %s\n", dst);
        /* dst file  exists, sanity check */
        if (S_ISDIR(buf.st_mode)) {
            /* dst is a directory, append filename*/
            filename = strrchr(src, '/');
            if (filename == NULL) {
                return FCOPY_ERR_SRC_BAD; /* no src file name */
            }
            snprintf(thread->dstfile, PATH_MAX, "%s/%s", dst, filename);
            printf("dst file dir, %s\n", thread->dstfile);
            fd = open(thread->dstfile, O_CREAT|O_RDWR|O_TRUNC);
            if (fd < 0) {
                return FCOPY_ERR_DST_BAD;
            }
            close(fd);
        } else {
            snprintf(thread->dstfile, PATH_MAX, "%s", dst);
            fd = open(thread->dstfile, O_CREAT|O_RDWR|O_TRUNC);
            if (fd < 0) {
                return FCOPY_ERR_DST_BAD;
            }
            close(fd);
        }
    } else {
        /* dst file does not exists, check if write-able */
        snprintf(thread->dstfile, PATH_MAX, "%s", dst);
        printf("dst file not exist, %s\n", thread->dstfile);
        fd = open(thread->dstfile, O_CREAT|O_RDWR|O_TRUNC);
        if (fd < 0) {
            return FCOPY_ERR_DST_BAD;
        }
        close(fd);
    }
    if(thread->dstfile_type!=NORMAL_FILE)
    {
        char *filename;
        unlink(thread->dstfile);
        filename = strrchr(thread->dstfile, '.');
        if(filename == NULL)
            strncat(thread->dstfile,GFILE_EXT,PATH_MAX);
    }
    
    // check if endPos and startPos is corrent
    if (((endPos != -1) && (endPos < startPos)) || (startPos < 0))
        return FCOPY_ERR_SRC_BAD;
    thread->m_endPos = endPos;
    thread->m_startPos = startPos;
        
    // if we reach this point, both src and dst are valid, 
    // check disk space 
    if(thread->srcfile_type==NORMAL_FILE)
    {
        m_source_size=(off64_t)srcbuf.st_size;
        if (thread->m_endPos == -1)
            thread->m_endPos = m_source_size;
        if (thread->m_endPos > m_source_size) 
            return FCOPY_ERR_SRC_BAD;
            
        if (!space_ok(&srcbuf, thread->dstfile)) {
            printf("insufficient space on dst\n");
            unlink(thread->dstfile);
            return FCOPY_ERR_SPACE;
        }
    }
    else if (thread->srcfile_type == GFILE_ARRAY_RING) {
        //@FIXME: not check file size now...
        m_source_size = (off64_t)endPos-startPos;
    }
    else
    { 
        gfile_t *gfile_src;
        if((gfile_src=GFileOpen(thread->srcfile,O_RDONLY,(ENUM_GFILE_TYPE)thread->srcfile_type))!=NULL)
        {
            off64_t src_size=0;            
            src_size=GgetFileSize(gfile_src);
            m_source_size=src_size;
            if (thread->m_endPos == -1)
                thread->m_endPos = m_source_size;
            if (thread->m_endPos > m_source_size)    
                return FCOPY_ERR_SRC_BAD;
                
            if (!space_ok2(src_size, thread->dstfile)) {
                printf("insufficient space on dst\n");
                unlink(thread->dstfile);
                return FCOPY_ERR_SPACE;
            }
        }
        else
        {
            unlink(thread->dstfile);
            return FCOPY_ERR_SRC_BAD;
        }
    }
    
    arg.val = MAX_BUF_USED;
    if (semctl(thread->sem_id, READ_LOCK, SETVAL, arg) == -1) {
        printf("sem id setval fail for read lock\n");
        return FCOPY_ERR_INTERNAL;
    }
    arg.val = 0;
    if (semctl(thread->sem_id, WRITE_LOCK, SETVAL, arg) == -1) {
        printf("sem id setval fail for write lock\n");
        return FCOPY_ERR_INTERNAL;
    }

    /* going into run state */
    written = 0;
    thread->tstate = THREAD_RUN;
    thread->written = &written;
    thread->notify_sem_id = &notify_cond;
    state = FCOPY_RUNNING;
    pthread_mutex_lock(&notify_cond);
    semop(thread->sem_id, &readrun_give, 1);
    return 0;
}

FASTCOPY_STATE GFastCopy::get_status(void) {
    struct thread_runner *thread = (struct thread_runner *)threadctx;
    if (!thread || state == FCOPY_ERR) {
        printf("status err!\n");
        return FCOPY_ERR;
    }

    switch(thread->tstate) {
    case THREAD_ERR:
        return FCOPY_ERR;
    case THREAD_DONE:
        return FCOPY_COMPLETE;
    case THREAD_ABORT:
        return FCOPY_ABORT;
    case THREAD_RUN:
        return FCOPY_RUNNING;
    default:
        return FCOPY_NOT_RUN;
    }
}

off64_t GFastCopy::get_written(void) {
    return written;
}

void GFastCopy::abort(void) {
    struct thread_runner *thread;
    if (state == FCOPY_NOT_RUN) 
        return;
    thread = (struct thread_runner *) threadctx;
    if (thread->tstate == THREAD_RUN) {
        printf("wait for thread to finish\n");
        thread->tstate = THREAD_ABORT;
        pthread_mutex_lock(&notify_cond);
        pthread_mutex_unlock(&notify_cond);
    }
    state = FCOPY_NOT_RUN;

    return;
}
