#ifndef ANDROID_INBANDSERVICE_H
#define ANDROID_INBANDSERVICE_H

#include "IInbandService.h"

namespace android {
	typedef void (*f_callback_SetParameters)(int left,int top, int width, int height,int show);
	typedef int (*f_callback_GetReady)();
	typedef void (*f_callback_SetUIBC)(int on);
	class InbandService : public BnInbandService{
		private:
			static f_callback_SetParameters callback_SetParameters;
			static f_callback_GetReady callback_GetReady;
			static f_callback_SetUIBC callback_SetUIBC;
		public:
			static InbandService *thisService;
			static void instantiate();
			static void deinstantiate();
			//status_t inbandFunc(const char *cmd, const char *param1, const char *param2, char *value);
			void SetParameters(int left,int top, int width, int height,int show);
			int GetReady();
			void SetUIBC(int on);
			static void SetCallback_SetParameters(f_callback_SetParameters lfunc);
			static void SetCallback_GetReady(f_callback_GetReady lfunc);
			static void SetCallback_SetUIBC(f_callback_SetUIBC lfunc);
		private:
			InbandService();
			~InbandService();

	};

}

#endif
