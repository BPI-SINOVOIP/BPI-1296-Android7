#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "camera_def.h"

class Camera
{
public:

    Camera();
    
    virtual ~Camera();    
    
protected:    
    unsigned char       m_id;      
    CAMERA_VIDEO_FORMAT m_format;
    void*               m_lock;
public:
    virtual int Init();
    virtual int GetModelID(){ return m_id; }    
    virtual int SetFormat(CAMERA_VIDEO_FORMAT* pFmt);
    virtual int GetFormat(CAMERA_VIDEO_FORMAT* pFmt);
    virtual int GetFrameBufferSize(unsigned long* p_size);    
    virtual int EnumFormat(CAMERA_VFMT_CAP* p_cap);
    virtual int EnumFrameSize(CAMERA_VFMT fmt, CAMERA_FS_CAP* p_cap);
    virtual int EnumFrameRate(CAMERA_VIDEO_FORMAT* pFmt, CAMERA_FR_CAP* p_cap);        
    virtual int StreamingControl(unsigned char OnOff);
    virtual int ReadFrame(unsigned char** ppBuff, unsigned long* pLen);
    virtual int ReadFrameEx(unsigned char** ppBuff, unsigned long* pLen, CAMERA_FRAME_INFO *pInfo);
    virtual int ReleaseFrame(unsigned char* pBuff, unsigned long Len);  
    
protected:
    virtual int CheckFrameData(unsigned char* pData, unsigned long Len);
    void Lock();    
    void Unlock();    
};


#endif  // __CAMERA_H__


