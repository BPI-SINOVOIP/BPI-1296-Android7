#ifndef __UVC_CAMERA_H__
#define __UVC_CAMERA_H__

#include "camera.h"

#define MAX_MMAP_BUFF_COUNT        1


typedef struct {
    unsigned char*  addr;
    unsigned long   length;    
    unsigned long   phy_addr;
}UVC_BUFF;


class UVCCamera : public Camera
{
public:

    UVCCamera(unsigned char id);
    
    UVCCamera(unsigned char id, int blocking);
    
    virtual ~UVCCamera();
    
private:    
    int             m_fd;
    int             m_dev_id;
    int             m_flags;
    unsigned char   m_status;     
    
#define UVC_STREAMING_ON        0x80    

    unsigned char   m_state;
#define UVC_STATE_IDEL              0        
#define UVC_STATE_REQUEST_BUFFER    1
#define UVC_STATE_QUEUE_BUFFER      2
#define UVC_STATE_START_STREAMING   3
#define UVC_STATE_RUNING            4

    int             m_read_retry_time;
    UVC_BUFF        m_buff[MAX_MMAP_BUFF_COUNT];        
    int             m_frame_count;    

    // frame rate control...   
    int             m_frame_interval; 
    long long       m_last_frame_pts;
    
public:
    virtual int Init();           
    virtual int SetFormat(CAMERA_VIDEO_FORMAT* pFmt); 
    virtual int GetFormat(CAMERA_VIDEO_FORMAT* pFmt); 
    virtual int EnumFormat(CAMERA_VFMT_CAP* p_cap);
    virtual int EnumFrameSize(CAMERA_VFMT fmt, CAMERA_FS_CAP* p_cap);
    virtual int EnumFrameRate(CAMERA_VIDEO_FORMAT* pFmt, CAMERA_FR_CAP* p_cap);
    virtual int GetFrameBufferSize(unsigned long* p_size);   
    virtual int StreamingControl(unsigned char OnOff);                 
    virtual int ReadFrameEx(unsigned char** ppBuff, unsigned long* pLen, CAMERA_FRAME_INFO* pInfo);
    virtual int ReleaseFrame(unsigned char* pBuff, unsigned long Len);      
    
private:
    int InitMMAP();    
    int UninitMMAP();    
    int StartStreaming();    
    int StopStreaming();    
};


#define UVC_WARNING(fmt, args...)       printf("[UVC] WARNING, " fmt, ## args)
#ifdef UVC_DBG_EN
#define UVC_DBG(fmt, args...)           printf("[UVC] DBG, " fmt, ## args)
#else
#define UVC_DBG(fmt, args...)   
#endif

#define UVC_INFO(fmt, args...)          printf("[UVC] INFO, " fmt, ## args)


#endif  // __UVC_CAMERA_H__
