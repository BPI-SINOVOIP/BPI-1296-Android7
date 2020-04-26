#ifndef __FW_INFO_H__
#define __FW_INFO_H__

#include <platform/setting.h>

#define BYTE(d,n)       (((d) >> ((n) << 3)) & 0xFF)
#define SWAPEND32(d)    ((BYTE(d,0) << 24) | (BYTE(d,1) << 16) | (BYTE(d,2) << 8) | (BYTE(d,3) << 0))
#define BE32_TO_CPU(value)      (value)
#define UINT32_PTR(_addr)     ((void *)(unsigned long)(_addr))
#define PTR_UINT32(_addr)     ((unsigned int)(unsigned long)(_addr))

#define FIRMWARE_DESCRIPTION_TABLE_SIGNATURE "VERONA__"

#define BOOT_AV_INFO_MAGICNO BOOT_AV_INFO_MAGICNO_RTK

#if PLATFORM_RTD1295
#define BOOT_AV_INFO_MAGICNO_RTK    0x2452544B  // $RTK
#elif PLATFORM_RTD1395
#define BOOT_AV_INFO_MAGICNO_RTK    0x2452544D  // $RTK
#endif

#define BOOT_AV_INFO_MAGICNO_STD3   0x53544433  // STD3 <= support dynamic decode buffer

#define BOOT_LOGO_ADDR		0x30000000ULL
#define BOOT_LOGO_SIZE		0x00900000ULL
#define VO_SECURE_ADDR		BOOT_LOGO_ADDR + BOOT_LOGO_SIZE - 0x100000
#define VO_SECURE_SIZE		0x00005a00ULL

typedef enum {
	RTK_PLAT_ERR_OK = 0,
	RTK_PLAT_ERR_PARSE_FW_DESC,
	RTK_PLAT_ERR_READ_FW_IMG,
	RTK_PLAT_ERR_READ_KERNEL_IMG,
	RTK_PLAT_ERR_READ_RESCUE_IMG,
	RTK_PLAT_ERR_BOOT,
} rtk_plat_err_t;

 /* copy from darwin boot audio/video data info */
typedef struct {
    uint    dwMagicNumber;          //identificatin String "$RTK" or "STD3"

    uint    dwVideoStreamAddress;   //The Location of Video ES Stream
    uint    dwVideoStreamLength;    //Video ES Stream Length

    uint    dwAudioStreamAddress;   //The Location of Audio ES Stream
    uint    dwAudioStreamLength;    //Audio ES Stream Length

    uchar   bVideoDone;
    uchar   bAudioDone;

    uchar   bHDMImode;              //monitor device mode (DVI/HDMI)

    char    dwAudioStreamVolume;    //Audio ES Stream Volume -60 ~ 40

    char    dwAudioStreamRepeat;    //0 : no repeat ; 1 :repeat

    uchar   bPowerOnImage;          // Alternative of power on image or video
    uchar   bRotate;                // enable v &h flip
    uint    dwVideoStreamType;      // Video Stream Type

    uint    audio_buffer_addr;      // Audio decode buffer
    uint    audio_buffer_size;
    uint    video_buffer_addr;      // Video decode buffer
    uint    video_buffer_size;
    uint    last_image_addr;        // Buffer used to keep last image
    uint    last_image_size;

    uchar   logo_enable;
    uint    logo_addr;
    u_int   src_width;              //logo_src_width
    u_int   src_height;             //logo_src_height
    u_int   dst_width;              //logo_dst_width
    u_int   dst_height;             //logo_dst_height
    uint    logo_addr_2;
	
	uint	vo_secure_addr;			//Address for secure vo
} boot_av_info_t;

typedef struct {
    uchar   signature[8];
    uint    checksum;
    uchar   version;
#ifdef NAS_DUAL
	uchar	seqnum;
	uchar	reserved[6];
#else
    uchar   reserved[7];
#endif
    uint    paddings;
    uint    part_list_len;
    uint    fw_list_len;
} __attribute__((packed)) fw_desc_table_v1_t;

typedef struct {
    uchar type;
#ifdef LITTLE_ENDIAN
    uchar reserved:7,
    ro:1;
#else
    uchar ro:1,
    reserved:7;
#endif
    u64  length;
    uchar   fw_count;
    uchar   fw_type;
#ifdef CONFIG_SYS_RTK_EMMC_FLASH
    uchar   emmc_partIdx;
    uchar   reserved_1[3];
#else
    uchar   reserved_1[4];
#endif
    uchar   mount_point[32];
} __attribute__((packed)) part_desc_entry_v1_t;

#define FW_DESC_TABLE_V1_T_VERSION_1            0x1
#define FW_DESC_TABLE_V1_T_VERSION_11           0x11
#define FW_DESC_TABLE_V1_T_VERSION_21           0x21

#define FW_DESC_TABLE_V2_T_VERSION_2            0x2
#define FW_DESC_TABLE_V2_T_VERSION_12           0x12
#define FW_DESC_TABLE_V2_T_VERSION_22           0x22

#define FW_DESC_BASE_VERSION(v)			((v) & 0xf)
#define FW_DESC_EXT_VERSION(v)			((v) >> 4)

typedef struct {
	uchar   type;
#ifdef LITTLE_ENDIAN
	uchar   reserved:6,
		lzma:1,
		ro:1;
#else
	uchar   ro:1,
		lzma:1,
		reserved:6;
#endif
	uint    version;
	uint    target_addr;
	uint    offset;
	uint    length;
	uint    paddings;
	uint    checksum;
	uchar   reserved_1[6];
} __attribute__((packed)) fw_desc_entry_v1_t;

// for fw_desc_table_v1_t.version = FW_DESC_TABLE_V1_T_VERSION_11
typedef struct {
	fw_desc_entry_v1_t v1;
	uint    act_size;
	uchar   hash[32];
	uchar   part_num;
	uchar   reserved[27];
} __attribute__((packed)) fw_desc_entry_v11_t;

// for fw_desc_table_v1_t.version = FW_DESC_TABLE_V1_T_VERSION_21
typedef struct {
	fw_desc_entry_v1_t v1;
	uint    act_size;
	uchar   part_num;
	uchar   RSA_sign[256];
	uchar   reserved[27];
} __attribute__((packed)) fw_desc_entry_v21_t;

// Version 2, offset->u64 / checksum->sha256
typedef struct {
	uchar	type;
#ifdef LITTLE_ENDIAN
	uchar	reserved:6,
		lzma:1,
		ro:1;
#else
	uchar	ro:1,
		lzma:1,
		reserved:6;
#endif
	uint    version;
	uint    target_addr;
	u64     offset;
	uint    length;
	uint    paddings;
	uchar   sha_hash[32];;
	uchar   reserved_1[6];
} __attribute__((packed)) fw_desc_entry_v2_t;

// for fw_desc_table_v1_t.version = FW_DESC_TABLE_V1_T_VERSION_11
typedef struct {
	fw_desc_entry_v2_t v2;
	uint    act_size;
	uchar   hash[32];
	uchar   part_num;
	uchar   reserved[27];
} __attribute__((packed)) fw_desc_entry_v12_t;


// for fw_desc_table_v1_t.version = FW_DESC_TABLE_V1_T_VERSION_21
typedef struct {
	fw_desc_entry_v2_t v2;
	uint    act_size;
	uchar   part_num;
	uchar   RSA_sign[256];
	uchar   reserved[27];
} __attribute__((packed)) fw_desc_entry_v22_t;

#define FW_ENTRY_MEMBER_SET(val, fw_entry, member, version) \
{ \
	fw_desc_entry_v1_t* entry_v1 = (fw_desc_entry_v1_t*)fw_entry; \
	fw_desc_entry_v2_t* entry_v2 = (fw_desc_entry_v2_t*)fw_entry; \
	if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V1_T_VERSION_1) \
		entry_v1->member = val; \
	else if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V2_T_VERSION_2) \
		entry_v2->member = val; \
} \

#define FW_ENTRY_MEMBER_GET(data, fw_entry, member, version) \
{ \
	fw_desc_entry_v1_t* entry_v1 = (fw_desc_entry_v1_t*)fw_entry; \
	fw_desc_entry_v2_t* entry_v2 = (fw_desc_entry_v2_t*)fw_entry; \
	if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V1_T_VERSION_1) \
		data = entry_v1->member; \
	else if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V2_T_VERSION_2) \
		data = entry_v2->member; \
} \

typedef enum {
	FW_TYPE_RESERVED = 0,
	FW_TYPE_BOOTCODE,
	FW_TYPE_KERNEL,
	FW_TYPE_RESCUE_DT,
	FW_TYPE_KERNEL_DT,
	FW_TYPE_RESCUE_ROOTFS,   //5
	FW_TYPE_KERNEL_ROOTFS,
	FW_TYPE_AUDIO,
	FW_TYPE_AUDIO_FILE,
	FW_TYPE_VIDEO_FILE,
	FW_TYPE_EXT4,        //10
	FW_TYPE_UBIFS,
	FW_TYPE_SQUASH,
	FW_TYPE_EXT3,
	FW_TYPE_ODD,
	FW_TYPE_YAFFS2,      //15
	FW_TYPE_ISO,
	FW_TYPE_SWAP,
	FW_TYPE_NTFS,
	FW_TYPE_JFFS2,
	FW_TYPE_IMAGE_FILE,      //20
	FW_TYPE_IMAGE_FILE1,
	FW_TYPE_IMAGE_FILE2,
	FW_TYPE_AUDIO_FILE1,
	FW_TYPE_AUDIO_FILE2,
	FW_TYPE_VIDEO_FILE1,     //25
	FW_TYPE_VIDEO_FILE2,
	FW_TYPE_VIDEO,
	FW_TYPE_VIDEO2,
	FW_TYPE_ECPU,
	FW_TYPE_TEE,         //30
	FW_TYPE_GOLD_KERNEL,
	FW_TYPE_GOLD_RESCUE_DT,
	FW_TYPE_GOLD_RESCUE_ROOTFS,
	FW_TYPE_GOLD_AUDIO,
	FW_TYPE_GOLD_TEE,        //35
	FW_TYPE_CONFIG,
	FW_TYPE_UBOOT,
	FW_TYPE_BL31,
	FW_TYPE_HYP,
	FW_TYPE_GOLD_BL31,
	FW_TYPE_RSA_KEY_FW,
	FW_TYPE_RSA_KEY_TEE,
	FW_TYPE_LK, //43
	FW_TYPE_RESCUE_LK,
	FW_TYPE_RESCUE_AUDIO,
	FW_TYPE_RESCUE_TEE,
	FW_TYPE_RESCUE_BL31,
	FW_TYPE_RESCUE_KERNEL,
	FW_TYPE_RESCUE_AREA,
	FW_TYPE_NORMAL_AREA, //50
	FW_TYPE_UNKNOWN
} fw_type_code_t;

typedef enum {
	FS_TYPE_JFFS2 = 0,
	FS_TYPE_YAFFS2,
	FS_TYPE_SQUASH,
	FS_TYPE_RAWFILE,
	FS_TYPE_EXT4,
	FS_TYPE_UBIFS,
	FS_TYPE_NONE,
	FS_TYPE_UNKOWN
} rtk_part_fw_type_t;

extern BOOT_FLASH_T boot_flash_type;
#endif // __FW_INFO_H__
