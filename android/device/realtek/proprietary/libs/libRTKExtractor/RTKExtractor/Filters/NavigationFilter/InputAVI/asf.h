#ifndef INPUT_AVI_ASF_DESCRIPTOR_HEADER
#define INPUT_AVI_ASF_DESCRIPTOR_HEADER

typedef struct _ASF_GUID {          // size is 16 bytes
    unsigned long   Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char   Data4[8];
} ASF_GUID;

#define ASF_GUID_LEN	sizeof(ASF_GUID)
#define DEFINE_ASF_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	static const ASF_GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#endif
