#ifndef _MPEG2_IFRAME_ENC_
#define _MPEG2_IFRAME_ENC_

#ifdef __cplusplus
extern "C" {
#endif

int MpegIFrameEncode(unsigned char *pStream,
		     int maxSize,
		     unsigned char *pLuma,
		     unsigned char *pChroma,
		     int pitch,
		     int width,
		     int height,
		     int quantScale);

#ifdef __cplusplus
};
#endif

#endif // _MPEG2_IFRAME_ENC_
