#ifndef ANDROID_ITESTSERVICE_H
#define ANDROID_ITESTSERVICE_H
#endif
#include <utils/String8.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <binder/IInterface.h>
#include <utils/RefBase.h>

namespace android {

	class IRtkHWMBinderService: public IInterface {
		public:
			DECLARE_META_INTERFACE (RtkHWMBinderService);
			virtual status_t otpWriteFunc(char *cmd, int param1, int param2, char *value) = 0;
			virtual status_t otpReadFunc(char *cmd, int param1, int param2, char *value) = 0;
			virtual status_t cwKeyFunc(char *cmd, int param1, int param2, int param3, char *value) = 0;
			virtual status_t aesEcbEnFunc(char *cmd, char *key, int param2, int param3, char *value) = 0;
			virtual status_t dpaWriteFunc(char *cmd, int param1, int param2, int param3) = 0;
	};

	class BnRtkHWMBinderService: public BnInterface<IRtkHWMBinderService> {
		public:
			virtual status_t onTransact(uint32_t code, const Parcel& data,
					Parcel* reply, uint32_t flags = 0);
	};

}

