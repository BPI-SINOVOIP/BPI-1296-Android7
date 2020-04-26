#ifndef __INPUT_PLUGIN_COMMON_UTILITY__AAC_UTILITY_H__
#define __INPUT_PLUGIN_COMMON_UTILITY__AAC_UTILITY_H__




#ifdef __cplusplus
extern "C" {
#endif

	int detect_aac_sbr_present_flag(unsigned char *buffer,
				unsigned short bufferLength,
				unsigned char *channelsConfiguration,
				unsigned int *samplingFrequencyIndex,
				int *privateData1,
				int *privateData2) ;
				
#ifdef __cplusplus
}
#endif

#endif
