#ifndef __AVI_INPUT_PLUGIN_INTERNAL_OGG_DECODE_HEADER__
#define __AVI_INPUT_PLUGIN_INTERNAL_OGG_DECODE_HEADER__

/* Ogg Decode - Header Version
 *
 * if multiple 'OggS' pages are available, send them together 
 * */
HRESULT	InputAVI_HeaderOggDecode(void *pInstance, 
				NAVBUF *pBuffer,
				int size, 
				unsigned char *data);


/* Ogg Decode - Bitstream Version
 *
 * if multiple 'OggS' pages are available, send it with one-by-one policy 
 * */
HRESULT	InputAVI_BSOggDecode(void *pInstance, NAVBUF *pBuffer);

/* Ogg Bitstream Append
 *
 * Append bitstream to this->oggVorbis
 * */
HRESULT InputAVI_BSOggAppend(void *pInstance);

/* invalidate this->oggVorbis */
HRESULT InputAVI_OggVorbisReset(void *pInstance);

#endif
