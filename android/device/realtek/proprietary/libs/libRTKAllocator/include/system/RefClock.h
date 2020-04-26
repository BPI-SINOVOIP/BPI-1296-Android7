#ifndef RTK_LIBRTKALLOCATOR_REFCLOCK_H
#define RTK_LIBRTKALLOCATOR_REFCLOCK_H

#include <allocator/IonAllocator.h>

#ifdef __cplusplus
class RefClock {
public:
    void * const PHYADDR_ERROR = DEF_PHYADDR_ERROR;
    void * const VIRADDR_ERROR = DEF_VIRADDR_ERROR;

    RefClock();
    virtual ~RefClock();

    virtual int                 Alloc                       (void);
    virtual void                Free                        (void);
    virtual int                 Import                      (int fd);
    virtual int                 GetShareFd                  (void);

    virtual unsigned long       GetSize                     (void);
    virtual void *              GetVirAddr                  (void);
    virtual void *              GetPhyAddr                  (void);

    virtual void                SetEndianSwap               (bool enable = true) {mEndianSwap = enable;};
    virtual bool                GetEndianSwap               (void) {return mEndianSwap;};

    virtual long long           GetRCD                      (void);
    virtual unsigned int        GetRCDExt                   (void);
    virtual long long           GetMasterGPTS               (void);
    virtual long long           GetGPTSimeout               (void);
    virtual long long           GetVideoSystemPTS           (void);
    virtual long long           GetAudioSystemPTS           (void);
    virtual long long           GetVideoPTS                 (void);
    virtual long long           GetAudioPTS                 (void);
    virtual unsigned int        GetVideoContext             (void);
    virtual unsigned int        GetAudioContext             (void);
    virtual unsigned int        GetVideoFreeRunThreshold    (void);
    virtual unsigned int        GetAudioFreeRunThreshold    (void);
    virtual unsigned int        GetVoUnderflow              (void);
    virtual unsigned int        GetAoUnderflow              (void);
    virtual unsigned int        GetVideoEOS                 (void);
    virtual unsigned int        GetAudioEOS                 (void);

    virtual int                 SetRCD                      (long long      rcd                 = -1LL);
    virtual int                 SetRCDExt                   (unsigned int   rcd_ext             = 0);
    virtual int                 SetMasterGPTS               (long long      master_gpts         = -1LL);
    virtual int                 SetGPTSimeout               (long long      gpts_timeout        = 0);
    virtual int                 SetVideoSystemPTS           (long long      video_system_pts    = -1LL);
    virtual int                 SetAudioSystemPTS           (long long      audio_system_pts    = -1LL);
    virtual int                 SetVideoPTS                 (long long      video_pts           = -1LL);
    virtual int                 SetAudioPTS                 (long long      audio_pts           = -1LL);
    virtual int                 SetVideoContext             (unsigned int   video_context       = -1U);
    virtual int                 SetAudioContext             (unsigned int   audio_context       = -1U);
    virtual int                 SetVideoFreeRunThreshold    (unsigned int   video_free_run_threshold    = 0);
    virtual int                 SetAudioFreeRunThreshold    (unsigned int   audio_free_run_threshold    = 0);
    virtual int                 SetVoUnderflow              (unsigned int   video_underflow     = 0);
    virtual int                 SetAoUnderflow              (unsigned int   audio_underflow     = 0);
    virtual int                 SetVideoEOS                 (unsigned int   video_eos           = 0);
    virtual int                 SetAudioEOS                 (unsigned int   audio_eos           = 0);

    enum MastershipMode{
        MASTERSHIPMODE_AVSYNC_FORCED_SLAVE,
        MASTERSHIPMODE_AVSYNC_FORCED_MASTER,
        MASTERSHIPMODE_AVSYNC_AUTO_SLAVE,
        MASTERSHIPMODE_AVSYNC_AUTO_MASTER,
        MASTERSHIPMODE_AVSYNC_AUTO_MASTER_NO_SKIP,
        MASTERSHIPMODE_AVSYNC_AUTO_MASTER_CONSTANT_DELAY,   /* after getback mastership, it become force master */
        MASTERSHIPMODE_AVSYNC_EXTREME_LOW_DELAY,
        MASTERSHIPMODE_AVSYNC_AUTO_MASTER_NO_SKIP_TIMEOUT, /* hold on mastership if RCD = -1 and TIMEOUT is not expired */
        MASTERSHIPMODE_AVSYNC_ERROR = 0xff,
    };

    enum AutoMasterState {
        AUTOMASTERSTATE_NOT_MASTER,
        AUTOMASTERSTATE_IS_MASTER,
        AUTOMASTERSTATE_ERROR = 0xff,
    };

    virtual enum AutoMasterState    GetMastership_AutoMasterState   (void);
    virtual enum MastershipMode     GetMastership_SystemMode        (void);
    virtual enum MastershipMode     GetMastership_VideoMode         (void);
    virtual enum MastershipMode     GetMastership_AudioMode         (void);

    virtual int                     SetMastership_AutoMasterState   (enum AutoMasterState  state    = AUTOMASTERSTATE_NOT_MASTER);
    virtual int                     SetMastership_SystemMode        (enum MastershipMode   mode     = MASTERSHIPMODE_AVSYNC_FORCED_SLAVE);
    virtual int                     SetMastership_VideoMode         (enum MastershipMode   mode     = MASTERSHIPMODE_AVSYNC_FORCED_MASTER);
    virtual int                     SetMastership_AudioMode         (enum MastershipMode   mode     = MASTERSHIPMODE_AVSYNC_FORCED_MASTER);
private:
    void                        SetDefault(void);
    unsigned char               AutoMasterStateMapping              (enum AutoMasterState  state);
    unsigned char               MastershipModeMapping               (enum MastershipMode   mode);
    IonAllocator *              mIonAllocator;
    bool                        mEndianSwap;
};

#else
#define REFCLOCK_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define REFCLOCK_VIRADDR_ERROR DEF_VIRADDR_ERROR;
#define REFCLOCK_AVSYNC_FORCED_SLAVE                        (0x00)
#define REFCLOCK_AVSYNC_FORCED_MASTER                       (0x01)
#define REFCLOCK_AVSYNC_AUTO_SLAVE                          (0x02)
#define REFCLOCK_AVSYNC_AUTO_MASTER                         (0x03)
#define REFCLOCK_AVSYNC_AUTO_MASTER_NO_SKIP                 (0x04)
#define REFCLOCK_AVSYNC_AUTO_MASTER_CONSTANT_DELAY          (0x05)
#define REFCLOCK_AVSYNC_EXTREME_LOW_DELAY                   (0x06)
#define REFCLOCK_AVSYNC_AUTO_MASTER_NO_SKIP_TIMEOUT         (0x07)
#define REFCLOCK_AVSYNC_ERROR                               (0xff)
#define REFCLOCK_AUTOMASTERSTATE_NOT_MASTER                 (0x00)
#define REFCLOCK_AUTOMASTERSTATE_IS_MASTER                  (0x01)
#define REFCLOCK_AUTOMASTERSTATE_ERROR                      (0xff)
typedef struct RefClock RefClock;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    RefClock *          RefClock_new();
    void                RefClock_delete                         (RefClock * pInstance);

    int                 RefClock_Alloc                          (RefClock * pInstance);
    void                RefClock_Free                           (RefClock * pInstance);
    int                 RefClock_Import                         (RefClock * pInstance, int fd);
    int                 RefClock_GetShareFd                     (RefClock * pInstance);
    unsigned long       RefClock_GetSize                        (RefClock * pInstance);
    void *              RefClock_GetVirAddr                     (RefClock * pInstance);
    void *              RefClock_GetPhyAddr                     (RefClock * pInstance);
    void                RefClock_SetEndianSwap                  (RefClock * pInstance, int enable);
    bool                RefClock_GetEndianSwap                  (RefClock * pInstance);
    long long           RefClock_GetRCD                         (RefClock * pInstance);
    unsigned int        RefClock_GetRCDExt                      (RefClock * pInstance);
    long long           RefClock_GetMasterGPTS                  (RefClock * pInstance);
    long long           RefClock_GetGPTSimeout                  (RefClock * pInstance);
    long long           RefClock_GetVideoSystemPTS              (RefClock * pInstance);
    long long           RefClock_GetAudioSystemPTS              (RefClock * pInstance);
    long long           RefClock_GetVideoPTS                    (RefClock * pInstance);
    long long           RefClock_GetAudioPTS                    (RefClock * pInstance);
    unsigned int        RefClock_GetVideoContext                (RefClock * pInstance);
    unsigned int        RefClock_GetAudioContext                (RefClock * pInstance);
    unsigned int        RefClock_GetVideoFreeRunThreshold       (RefClock * pInstance);
    unsigned int        RefClock_GetAudioFreeRunThreshold       (RefClock * pInstance);
    unsigned int        RefClock_GetVoUnderflow                 (RefClock * pInstance);
    unsigned int        RefClock_GetAoUnderflow                 (RefClock * pInstance);
    unsigned int        RefClock_GetVideoEOS                    (RefClock * pInstance);
    unsigned int        RefClock_GetAudioEOS                    (RefClock * pInstance);
    int                 RefClock_SetRCD                         (RefClock * pInstance, long long      rcd);
    int                 RefClock_SetRCDExt                      (RefClock * pInstance, unsigned int   rcd_ext);
    int                 RefClock_SetMasterGPTS                  (RefClock * pInstance, long long      master_gpts);
    int                 RefClock_SetGPTSimeout                  (RefClock * pInstance, long long      gpts_timeout);
    int                 RefClock_SetVideoSystemPTS              (RefClock * pInstance, long long      video_system_pts);
    int                 RefClock_SetAudioSystemPTS              (RefClock * pInstance, long long      audio_system_pts);
    int                 RefClock_SetVideoPTS                    (RefClock * pInstance, long long      video_pts);
    int                 RefClock_SetAudioPTS                    (RefClock * pInstance, long long      audio_pts);
    int                 RefClock_SetVideoContext                (RefClock * pInstance, unsigned int   video_context);
    int                 RefClock_SetAudioContext                (RefClock * pInstance, unsigned int   audio_context);
    int                 RefClock_SetVideoFreeRunThreshold       (RefClock * pInstance, unsigned int   video_free_run_threshold);
    int                 RefClock_SetAudioFreeRunThreshold       (RefClock * pInstance, unsigned int   audio_free_run_threshold);
    int                 RefClock_SetVoUnderflow                 (RefClock * pInstance, unsigned int   video_underflow);
    int                 RefClock_SetAoUnderflow                 (RefClock * pInstance, unsigned int   audio_underflow);
    int                 RefClock_SetVideoEOS                    (RefClock * pInstance, unsigned int   video_eos);
    int                 RefClock_SetAudioEOS                    (RefClock * pInstance, unsigned int   audio_eos);
    unsigned char       RefClock_GetMastership_AutoMasterState  (RefClock * pInstance);
    unsigned char       RefClock_GetMastership_SystemMode       (RefClock * pInstance);
    unsigned char       RefClock_GetMastership_VideoMode        (RefClock * pInstance);
    unsigned char       RefClock_GetMastership_AudioMode        (RefClock * pInstance);
    int                 RefClock_SetMastership_AutoMasterState  (RefClock * pInstance, unsigned char state);
    int                 RefClock_SetMastership_SystemMode       (RefClock * pInstance, unsigned char mode);
    int                 RefClock_SetMastership_VideoMode        (RefClock * pInstance, unsigned char mode);
    int                 RefClock_SetMastership_AudioMode        (RefClock * pInstance, unsigned char mode);
#ifdef __cplusplus
}
#endif

#endif /* End of RTK_LIBRTKALLOCATOR_REFCLOCK_H */
