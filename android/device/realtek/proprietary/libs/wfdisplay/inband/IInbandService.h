#ifndef ANDROID_ITESTSERVICE_H
#define ANDROID_ITESTSERVICE_H
#endif
#include <utils/String8.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IInterface.h>
#include <utils/RefBase.h>

namespace android {

	class IInbandService: public IInterface {
		public:
			DECLARE_META_INTERFACE (InbandService);
			//virtual status_t inbandFunc(const char *cmd, const char *param1, const char *param2, char *value) = 0;
			virtual void SetParameters(int left,int top, int width, int height,int show) = 0;
			virtual int GetReady() = 0;
			virtual void SetUIBC(int on) = 0;
	};

	class BnInbandService: public BnInterface<IInbandService> {
		public:
			virtual status_t onTransact(uint32_t code, const Parcel& data,
					Parcel* reply, uint32_t flags = 0);
	};

}

