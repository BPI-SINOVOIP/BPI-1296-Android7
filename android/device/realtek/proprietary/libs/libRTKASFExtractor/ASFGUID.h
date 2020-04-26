#ifndef __ASFGUID__
#define __ASFGUID__

#define ASF_GUID_LEN	sizeof(ASF_GUID)

typedef struct _ASF_GUID {          // size is 16 bytes
    unsigned long   Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char   Data4[8];
} ASF_GUID;



#define DEFINE_ASF_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        const ASF_GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }


#define IsEqualASFGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, ASF_GUID_LEN))

#define PRINT_ASF_GUID(guid) printf("\{%X-%X-%X-%X%X-%X%X%X%X%X%X\}\n", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0] \
, guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
//==================================================================================
//						 Top-level ASF object GUIDS
//==================================================================================

//
// ASF Header Object == {75B22630-668E-11CF-A6D9-00AA0062CE6C}
//
extern const ASF_GUID ASF_HEADER_OBJECT;
//DEFINE_ASF_GUID(ASF_HEADER_OBJECT, 0x75B22630,0x668E,0x11CF,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C);

//
// ASF Data Object == {75B22636-668E-11CF-A6D9-00AA0062CE6C}
//
extern const ASF_GUID ASF_DATA_OBJECT;

//
// ASF Simple Index Object == {33000890-E5B1-11CF-89F4-00A0C90349CB}
//
extern const ASF_GUID ASF_SIMPLE_INDEX_OBJECT;

//
// ASF Index Object == {D6E229D3-35DA-11D1-9034-00A0C90349BE}
//
extern const ASF_GUID ASF_INDEX_OBJECT;


//==================================================================================
//						 Header object GUIDS
//==================================================================================

//
// ASF File Properties Object == {8CABDCA1-A947-11CF-8EE4-00C00C205365}
//
extern const ASF_GUID ASF_FILE_PROPERITES_OBJECT;
//DEFINE_ASF_GUID(ASF_FILE_PROPERITES_OBJECT, 0x8CABDCA1, 0xA947, 0x11CF, 0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);

//
// ASF Stream Properties Object == {B7DC0791-A9B7-11CF-8EE6-00C00C205365}
//
extern const ASF_GUID ASF_STREAM_PROPERITES_OBJECT;
//DEFINE_ASF_GUID(ASF_STREAM_PROPERITES_OBJECT, 0xB7DC0791, 0xA9B7, 0x11CF, 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);

//
// ASF Header Extension Object == {5FBF03B5-A92E-11CF-8EE3-00C00C205365}
//
extern const ASF_GUID ASF_HEADER_EXTENSION_OBJECT;

//
// ASF Codec List Object == {86D15240-311D-11D0-A3A4-00A0C90348F6}
//
extern const ASF_GUID ASF_CODEC_LIST_OBJECT;

//
// ASF Script Command Object == {1EFB1A30-0B62-11D0-A39B-00A0C90348F6}
//
extern const ASF_GUID ASF_SCRIPT_COMMAND_OBJECT;


//
// ASF Marker Object == {F487CD01-A951-11CF-8EE6-00C00C205365}
//
extern const ASF_GUID ASF_MARKER_OBJECT;

//
// ASF Bitrate Mutual Exclusion Object == {D6E229DC-35DA-11D1-9034-00A0C90349BE}
//
extern const ASF_GUID ASF_BITRATE_MUTUAL_EXCLUSION_OBJECT;


//
// ASF Content Description Object == {75B22633-668E-11CF-A6D9-00AA0062CE6C}
//
extern const ASF_GUID ASF_CONTENT_DESCRIPTION_OBJECT;


//
// ASF Extended Content Description Object == {D2D0A440-E307-11D2-97F0-00A0C95EA850}
//
extern const ASF_GUID ASF_EXTENDED_CONTENT_DESCRIPTION_OBJECT;

//
// ASF Stream Bitrate Properties Object == {7BF875CE-468D-11D1-8D82-006097C9A2B2}
//
extern const ASF_GUID ASF_STREAM_BITRATE_PROPERTIES_OBJECT;


//
// ASF Padding Object == {1806D474-CADF-4509-A4BA-9AABCB96AAE8}
//
extern const ASF_GUID ASF_PADDING_OBJECT;

//==================================================================================
//						 Header Extension Object GUIDS
//==================================================================================

//
// ASF Extended Stream Porperties Object == {14E6A5CB-C672-4332-8399-A96952065B5A}
//
extern const ASF_GUID ASF_EXTENDED_STREAM_PROPERTIES_OBJECT;

//
// ASF Language List Object == {7C4346A9-EFE0-4BFC-B229-393EDE415C85}
//
extern const ASF_GUID ASF_LANGUAGE_LIST_OBJECT;

//
// ASF Metadata Object == {C5F8CBEA-5BAF-4877-8467-AA8C44FA4CCA}
//
extern const ASF_GUID ASF_METADATA_OBJECT;

//
// ASF Index Parameters Object == {D6E229DF-35DA-11D1-9034-00A0C90349BE}
//
extern const ASF_GUID ASF_INDEX_PARAMETERS_OBJECT;


//==================================================================================
//						 Stream Type GUIDS
//==================================================================================

//
// ASF Audio Media == {F8699E40-5B4D-11CF-A8FD-00805F5C442B}
//

extern const ASF_GUID ASF_AUDIO_MEDIA;

//
// ASF Video Media == {BC19EFC0-5B4D-11CF-A8FD-00805F5C442B}
//
extern const ASF_GUID ASF_VIDEO_MEDIA;

//
// ASF Video Media == {3AFB65E2-47EF-40F2-AC2C-70A90D71D343}
//
extern const ASF_GUID ASF_BINARY_MEDIA;
//==================================================================================
//						 DRM GUIDS
//==================================================================================
//
// DRM GUIDS == {2211B3FB-BD23-11D2-B4B7-00A0C955FC6E}
//
extern const ASF_GUID ASF_DRM_GUID;

//==================================================================================
//						 Extended DRM GUIDS
//==================================================================================
//
// DRM GUIDS == {298AE614-2622-4C17-B935-DAE07EE9289C}
//
extern const ASF_GUID ASF_EXTENDED_DRM_GUID;

//for pyv
extern const ASF_GUID ASF_PROTECTION_SYSTEM_IDENTIFIER_OBJECT;
extern const ASF_GUID ASF_PAYLOAD_EXTENSION_ENCRYPTION_SAMPLEID;

//
// ASF Video Media == {3AFB65E2-47EF-40F2-AC2C-70A90D71D343}
//
extern const ASF_GUID ASF_MEDIASUBTYPE_DOLBY_AC3;

#endif //__ASFGUID__
