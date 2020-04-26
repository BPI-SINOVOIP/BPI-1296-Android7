#ifndef __AVI_INPUT_PLUGIN_INTERNAL_AVI2_SUPPORTING_HEADER__
#define __AVI_INPUT_PLUGIN_INTERNAL_AVI2_SUPPORTING_HEADER__

HRESULT InputAVI_AVI2_PreSeekToNextAudioChunk(void *pInstance);
HRESULT InputAVI_AVI2_PreSeekToNextVideoChunk(void *pInstance);
HRESULT InputAVI_AVI2_VideoNextChunk(void *pInstance);
HRESULT InputAVI_AVI2_VideoPrevChunk(void *pInstance);
HRESULT InputAVI_AVI2_AudioNextChunk(void *pInstance);
HRESULT InputAVI_AVI2_SpicNextChunk(void *pInstance);
bool InputAVI_AVI2_UpdateAudioReadPosition(void *pInstance, int64_t videoPTS);
bool InputAVI_AVI2_UpdateSpicReadPosition(void *pInstance, int64_t videoPTS);

HRESULT	InputAVI_AVI2_single_index_handleSeeking(void *pInstance,
				int trueFrameIdx,
				seekDirection direction);

HRESULT	InputAVI_AVI2_two_tiered_index_handleSeeking(void *pInstance, 
				int trueFrameIdx,
				seekDirection direction);

#endif
