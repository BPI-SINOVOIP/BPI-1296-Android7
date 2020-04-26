#ifndef __PTHREAD_MACRO_H__
#define __PTHREAD_MACRO_H__

#include <sys/time.h>
#include <pthread.h>

static inline int pwait_event(pthread_cond_t *cond, pthread_mutex_t *mtx)
{
    int ret;

    pthread_mutex_lock(mtx);
    ret = pthread_cond_wait(cond, mtx);
    pthread_mutex_unlock(mtx);

    return ret;
}

static inline int pwait_event_timeout(pthread_cond_t *cond, pthread_mutex_t *mtx, unsigned int sec)
{
    int ret;
    struct timeval tp;
    struct timespec ts;

    pthread_mutex_lock(mtx);
    gettimeofday(&tp, NULL);
    ts.tv_sec  = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;
    ts.tv_sec += sec;
    ret = pthread_cond_timedwait(cond, mtx, &ts);
    pthread_mutex_unlock(mtx);

    return ret;
}

static inline void pwake_up(pthread_cond_t *cond, pthread_mutex_t *mtx)
{
    pthread_mutex_lock(mtx);
    pthread_cond_signal(cond);
    pthread_mutex_unlock(mtx);
}

#endif //__PTHREAD_MACRO_H__

