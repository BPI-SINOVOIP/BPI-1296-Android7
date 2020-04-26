#ifndef __NRDGUID__
#define __NRDGUID__


#ifndef EXTERN_C
# ifdef __cplusplus
#  define EXTERN_C extern "C"
# else
#  define EXTERN_C extern
# endif
#endif

#define NRD_GUID_LEN	sizeof(NRD_GUID)

typedef struct _NRD_GUID {          // size is 16 bytes
	unsigned long   Data1;
	unsigned short	Data2;
	unsigned short	Data3;
	unsigned char   Data4[8];
} NRD_GUID;



#define DEFINE_NRD_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	const NRD_GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }


#define IsEqualNRDGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, NRD_GUID_LEN))

//==================================================================================
//						 Top-level NRD object GUIDS
//==================================================================================

//
// NRD Header Object == {75B22630-668E-11CF-A6D9-00AA0062CE6C}
//
EXTERN_C const NRD_GUID NRD_HEADER_OBJECT;
//DEFINE_NRD_GUID(NRD_HEADER_OBJECT, 0x75B22630,0x668E,0x11CF,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C);

//
// NRD Data Object == {75B22636-668E-11CF-A6D9-00AA0062CE6C}
//
EXTERN_C const NRD_GUID NRD_DATA_OBJECT;

//
// NRD Simple Index Object == {33000890-E5B1-11CF-89F4-00A0C90349CB}
//
EXTERN_C const NRD_GUID NRD_SIMPLE_INDEX_OBJECT;

//
// NRD Index Object == {D6E229D3-35DA-11D1-9034-00A0C90349BE}
//
EXTERN_C const NRD_GUID NRD_INDEX_OBJECT;


//==================================================================================
//						 Header object GUIDS
//==================================================================================

//
// NRD File Properties Object == {8CABDCA1-A947-11CF-8EE4-00C00C205365}
//
EXTERN_C const NRD_GUID NRD_FILE_PROPERITES_OBJECT;
//DEFINE_NRD_GUID(NRD_FILE_PROPERITES_OBJECT, 0x8CABDCA1, 0xA947, 0x11CF, 0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);

//
// NRD Stream Properties Object == {B7DC0791-A9B7-11CF-8EE6-00C00C205365}
//
EXTERN_C const NRD_GUID NRD_STREAM_PROPERITES_OBJECT;
//DEFINE_NRD_GUID(NRD_STREAM_PROPERITES_OBJECT, 0xB7DC0791, 0xA9B7, 0x11CF, 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);

//
// NRD Header Extension Object == {5FBF03B5-A92E-11CF-8EE3-00C00C205365}
//
EXTERN_C const NRD_GUID NRD_HEADER_EXTENSION_OBJECT;

//
// NRD Codec List Object == {86D15240-311D-11D0-A3A4-00A0C90348F6}
//
EXTERN_C const NRD_GUID NRD_CODEC_LIST_OBJECT;

//
// NRD Script Command Object == {1EFB1A30-0B62-11D0-A39B-00A0C90348F6}
//
EXTERN_C const NRD_GUID NRD_SCRIPT_COMMAND_OBJECT;


//
// NRD Marker Object == {F487CD01-A951-11CF-8EE6-00C00C205365}
//
EXTERN_C const NRD_GUID NRD_MARKER_OBJECT;

//
// NRD Bitrate Mutual Exclusion Object == {D6E229DC-35DA-11D1-9034-00A0C90349BE}
//
EXTERN_C const NRD_GUID NRD_BITRATE_MUTUAL_EXCLUSION_OBJECT;


//
// NRD Content Description Object == {75B22633-668E-11CF-A6D9-00AA0062CE6C}
//
EXTERN_C const NRD_GUID NRD_CONTENT_DESCRIPTION_OBJECT;


//
// NRD Extended Content Description Object == {D2D0A440-E307-11D2-97F0-00A0C95EA850}
//
EXTERN_C const NRD_GUID NRD_EXTENDED_CONTENT_DESCRIPTION_OBJECT;

//
// NRD Stream Bitrate Properties Object == {7BF875CE-468D-11D1-8D82-006097C9A2B2}
//
EXTERN_C const NRD_GUID NRD_STREAM_BITRATE_PROPERTIES_OBJECT;


//
// NRD Padding Object == {1806D474-CADF-4509-A4BA-9AABCB96AAE8}
//
EXTERN_C const NRD_GUID NRD_PADDING_OBJECT;

//==================================================================================
//						 Header Extension Object GUIDS
//==================================================================================

//
// NRD Extended Stream Porperties Object == {14E6A5CB-C672-4332-8399-A96952065B5A}
//
EXTERN_C const NRD_GUID NRD_EXTENDED_STREAM_PROPERTIES_OBJECT;

//
// NRD Language List Object == {7C4346A9-EFE0-4BFC-B229-393EDE415C85}
//
EXTERN_C const NRD_GUID NRD_LANGUAGE_LIST_OBJECT;

//
// NRD Metadata Object == {C5F8CBEA-5BAF-4877-8467-AA8C44FA4CCA}
//
EXTERN_C const NRD_GUID NRD_METADATA_OBJECT;

//
// NRD Index Parameters Object == {D6E229DF-35DA-11D1-9034-00A0C90349BE}
//
EXTERN_C const NRD_GUID NRD_INDEX_PARAMETERS_OBJECT;


//==================================================================================
//						 Stream Type GUIDS
//==================================================================================

//
// NRD Audio Media == {F8699E40-5B4D-11CF-A8FD-00805F5C442B}
//

EXTERN_C const NRD_GUID NRD_AUDIO_MEDIA;

//
// NRD Video Media == {BC19EFC0-5B4D-11CF-A8FD-00805F5C442B}
//
EXTERN_C const NRD_GUID NRD_VIDEO_MEDIA;

//==================================================================================
//						 DRM GUIDS
//==================================================================================
//
// DRM GUIDS == {2211B3FB-BD23-11D2-B4B7-00A0C955FC6E}
//
EXTERN_C const NRD_GUID NRD_DRM_GUID;

//==================================================================================
//						 Extended DRM GUIDS
//==================================================================================
//
// DRM GUIDS == {298AE614-2622-4C17-B935-DAE07EE9289C}
//
EXTERN_C const NRD_GUID NRD_EXTENDED_DRM_GUID;
#endif //__NRDGUID__
