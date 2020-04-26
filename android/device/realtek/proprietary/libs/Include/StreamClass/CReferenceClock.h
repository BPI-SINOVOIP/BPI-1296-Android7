#ifndef __C_REFERENCE_CLOCK_H__
#define __C_REFERENCE_CLOCK_H__

#include <IStream.h>
#include "SType.h"
#include "CObject.h"
#include "../../../../common/IPC/include/rpc_common.h"

class CReferenceClock: public IReferenceClock, public CObject
{
public:
           CReferenceClock();
  virtual ~CReferenceClock();

  // core section
  // when change the core section, go to corresponding header file under common/IPC/include/rpc_common.h 
  // see rep_common.h for latest version

  REFCLOCK *m_core;
  BYTE *m_virtualAddr;
  unsigned long m_phyAddr;

  HRESULT  GetGlobalSCR(int64_t *pSCR);  // 27MHz  
  HRESULT  GetGlobalPTS(int64_t *pPTS);  // 90KHz 
  HRESULT  GetLocalPTS (int64_t *pPTS);  // 90KHz 
  HRESULT  SetLocalPTS (int64_t PTS);    // 90KHz 
  HRESULT  GetRCD (int64_t *pRCD);       // 90KHz
  HRESULT  SetRCD (int64_t RCD);         // 90KHz
  HRESULT  SetTimeout(int64_t timeout);  // 90KHz
  HRESULT  SetFreeRunThreshold(unsigned long video, unsigned long audio);
  HRESULT  SetMastership(MASTERSHIP *mastership);
  HRESULT  GetMastership(MASTERSHIP *mastership);
  HRESULT  GetPresentationPositions(PRESENTATION_POSITIONS *pos);
  HRESULT  ResetPresentationPositions();
  HRESULT  GetUnderflowStates(long* video, long* audio, int64_t *RCD);
  HRESULT  ResetUnderflowStates();
  HRESULT  GetCoreAddress(long *phyAddr, BYTE **virtualAddr, BYTE **nonCachedAddr); // get the core structure that shared between CPUs
  HRESULT  SetAutoPauseEncFlags(long audioFullness, long audioPauseFlag);
  HRESULT  GetAutoPauseEncFlags(long *audioFullness, long *audioPauseFlag);


  // end of core section
};

#endif
