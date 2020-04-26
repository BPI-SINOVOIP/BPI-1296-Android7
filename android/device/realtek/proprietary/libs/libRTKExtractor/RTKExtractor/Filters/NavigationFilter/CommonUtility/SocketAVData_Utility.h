#ifndef __INPUT_PLUGIN_COMMON_UTILITY__SOCKETAVDATA_UTILITY_H__
#define __INPUT_PLUGIN_COMMON_UTILITY__SOCKETAVDATA_UTILITY_H__

#include <Platform_Lib/SocketAVData/SocketAVData.h>
#include <StreamClass/EType.h>
#include <stddef.h>
#include <stdbool.h>

#define VIDEO_ATTR_FIELD_OFFSET(_field)	(offsetof(SOCKET_MEDIA_ATTRIBUTE, _field))
#define AUDIO_ATTR_FIELD_OFFSET(_field)	(offsetof(SOCKET_MEDIA_ATTRIBUTE, _field)-offsetof(SOCKET_MEDIA_ATTRIBUTE, audioAttrSize))
#define SPU_ATTR_FIELD_OFFSET(_field)	(offsetof(SOCKET_MEDIA_ATTRIBUTE, _field)-offsetof(SOCKET_MEDIA_ATTRIBUTE, spicAttrSize))
#define CONTAINER_ATTR_FIELD_OFFSET(_field)	(offsetof(SOCKET_MEDIA_ATTRIBUTE, _field)-offsetof(SOCKET_MEDIA_ATTRIBUTE, contentAttrSize))

#define GET_VIDEO_ATTR_FIELD(_attr, _field) 	(*((long*)((_attr)+VIDEO_ATTR_FIELD_OFFSET(_field))))
#define GET_AUDIO_ATTR_FIELD(_attr, _field) 	(*((long*)((_attr)+AUDIO_ATTR_FIELD_OFFSET(_field))))
#define GET_SPU_ATTR_FIELD(_attr, _field) 		(*((long*)((_attr)+SPU_ATTR_FIELD_OFFSET(_field))))
#define GET_CONTAINER_ATTR_FIELD(_attr, _field) 	(*((long*)((_attr)+CONTAINER_ATTR_FIELD_OFFSET(_field))))
	
#ifdef __cplusplus
extern "C" {
#endif

	ENUM_MEDIA_TYPE LookupSocketMediaTypeMap(SOCKET_MEDIA_TYPE socket_media_type);
	const char* GetPropertyAtURL(const char* url, const char* property);
	bool GetSocketFileNameAtURL(const char* url, char* pFileName, int size);
	ENUM_MEDIA_TYPE LookupSocketContainerTypeMap(SOCKET_CONTAINER_TYPE container_type);
	
#ifdef __cplusplus
}
#endif

#endif
