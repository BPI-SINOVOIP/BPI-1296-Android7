#ifndef C_VIDEO_AGENT_H
#define C_VIDEO_AGENT_H

//Video Agent is for video call
#include <StreamClass/CFilter.h>
#include <StreamClass/CMemAllocator.h>

class CVideoAssistant{

};
class CVideoAssistantVp8: public CVideoAssistant{

 public:
  CVideoAssistantVp8();
  ~CVideoAssistantVp8();  
  HRESULT CreateVP8AudioAgent();
  HRESULT DestroyVP8AudioAgent();
  static void VP8_DecodeFrameThread(void *dec);
  void *m_optr;
  CThread *m_pVP8Thread;
  long m_VP8AudioAgent;

  
};
#endif
