/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 * IMdService.h
 *
 *  Created on: June 25, 2015
 *      Author: calvinkuo
 */

#ifndef IMDSERVICE_H_
#define IMDSERVICE_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#include "RtkIpc/MdOp.h"
//#include "RtkIpc/MdClient.h"          //not necessary

namespace android {

class IMdService: public IInterface
{
public:
  DECLARE_META_INTERFACE(MdService);
  virtual unsigned int mdInit(sp<IBinder> &binder) = 0;
  virtual void mdUninit(sp<IBinder> &binder) = 0;
  virtual int mdMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned int hQueue) = 0;

  virtual int mdlibExecCmd(int byCommandQueue) = 0;
  virtual int mdlibWaitCmdDone(int byCommandQueue) = 0;
};

sp<IMdService> defaultMdService();

/* --- Server side --- */

class BnMdService: public BnInterface<IMdService>
{
public:
  virtual status_t    onTransact( uint32_t code,
                                  const Parcel& data,
                                  Parcel* reply,
                                  uint32_t flags = 0);
};

}; // namespace android

#endif /* IMDSERVICE_H_ */
