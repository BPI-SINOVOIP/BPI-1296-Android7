#ifndef _OGG_SUPPORT_HEADER_FILE_
#define _OGG_SUPPORT_HEADER_FILE_


#define MAX_SEG_NUM	255

#define OGG_CONTINUOUS_PACKET 	0x01
#define OGG_BEGIN_OF_STREAM		0x02
#define OGG_END_OF_STREAM		0x04

typedef struct _ogg_page_header {
	uint32_t	capture_pattern				__attribute__((packed));
	uint8_t 	stream_structure_version	__attribute__((packed));
	uint8_t		header_type_flag			__attribute__((packed));
	uint64_t	absolute_granule_position	__attribute__((packed));
	uint32_t	stream_serial_number		__attribute__((packed));
	uint32_t	page_sequence_no			__attribute__((packed));
	uint32_t	page_checksum				__attribute__((packed));
	uint8_t		page_segments				__attribute__((packed));
	uint8_t		segment_table[MAX_SEG_NUM]	__attribute__((packed));
} OGG_PAGE_HEADER;

typedef struct _ogg_status_descriptor {
	bool			valid;
	unsigned char* 	buffer;
	size_t			bufferSize;		// buffer capacity
	size_t			dataSize;		// valid data size
} OGG_STATUS_DESCRIPTOR;

#endif
