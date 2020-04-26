#ifndef __RTK_POWER_HAL_POWER_BASE_H__
#define __RTK_POWER_HAL_POWER_BASE_H__

class PowerBase {
public:
    PowerBase();
    virtual ~PowerBase();
    virtual int         setPerformance(bool enable);
    virtual int         setPerformanceTimeout(int64_t timeout_ms);

private:
    virtual int         Performance_cb(bool enable) = 0;

    int                 updateState();
    static void *       Loop (void * data);
    bool                mPerformanceForce;
    bool                mPerformanceStore;
    int64_t             mPerformanceTimeout;
    bool                mEOS;
    pthread_t           mThread;
    pthread_mutex_t     mLock;
};

#endif /* End of __RTK_POWER_HAL_POWER_BASE_H__ */
