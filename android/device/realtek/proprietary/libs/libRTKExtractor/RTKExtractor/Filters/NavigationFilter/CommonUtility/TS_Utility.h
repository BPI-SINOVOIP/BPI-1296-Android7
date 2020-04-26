#ifndef __INPUT_PLUGIN_COMMON_UTILITY__TS_UTILITY_H__
#define __INPUT_PLUGIN_COMMON_UTILITY__TS_UTILITY_H__


#ifdef __cplusplus
extern "C" {
#endif

	int ResyncTSStream(const unsigned char* pData, unsigned int bytes, int& packetSize);
	
#ifdef __cplusplus
}
#endif

#endif
