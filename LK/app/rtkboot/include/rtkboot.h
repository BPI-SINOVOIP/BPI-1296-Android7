#ifndef __RTKBOOT_H__
#define __RTKBOOT_H__

#include <rtk_ipc_shm.h>

#define ADDR_BUF_LEN	32

#define _TAB		0x09
#define _ENTER		0x0D
#define _ESC		0x1B
#define _SPACE		0x20

#define TEE_MEM_START (0x10100000)
#define TEE_MEM_SIZE  (0xF00000)

#define VIDEO_POOL_START	(0x32800000)
#define VIDEO_POOL_SIZE		(0x0c800000)

#ifdef NAS_ENABLE
#define AFW_MEM_START (0x01B00000)
#else
#define AFW_MEM_START (0x0F900000)
#endif

#define AFW_MEM_SIZE  (0x500000)



typedef enum {
	BOOT_CONSOLE_MODE = 0,
	BOOT_RESCUE_MODE,
	BOOT_NORMAL_MODE,
	BOOT_MANUAL_MODE,
	BOOT_KERNEL_ONLY_MODE,
	BOOT_GOLD_MODE,
	BOOT_RECOVERY_MODE,
	BOOT_FASTBOOT_MODE,
	#ifdef FT_TOOL
	BOOT_FT_MODE,
	#endif //FT_TOOL
	BOOT_S5_MODE,
	BOOT_MODE_MAX, // Make sure BOOT_MODE_MAX is at the end
	// Deprecated modes, add back if needed
	//BOOT_QC_VERIFY_MODE,
	//BOOT_FLASH_WRITER_MODE,
	//BOOT_MODE_NUM,
	//BOOT_ANDROID_MODE,
} BOOT_MODE;

extern BOOT_MODE boot_mode;
extern struct rtk_ipc_shm ipc_shm;

int rtk_boot_kernel(void);
int acpu_bootcode_prepare(void);
void acpu_setup_boot_av(addr_t logo_addr);
void acpu_boot(void);
void set_boot_flash_type(void);

int rtk_plat_prepare_fw_image(void);
int check_customized_boot_mode(void);
int is_boot_from_suspend(void);
int is_boot_from_kernel(void);

#ifdef ANDROID_REBOOT_CMD
#define REBOOT_ACTION_ADDR	(PLATFORM_REBOOT_ACTION_ADDR)
#define REBOOT_ACTION_MASK	(0xff)
#define REBOOT_MAGIC		0xaabbcc00
#define REBOOT_MAGIC_SHIFT	8
#define REBOOT_ACTION_VALID(v)	!((REBOOT_MAGIC ^ (v)) >> REBOOT_MAGIC_SHIFT)

typedef enum{
	RESET_ACTION_NO_ACTION = 0,
	RESET_ACTION_FASTBOOT,
	RESET_ACTION_RECOVERY,
	RESET_ACTION_GOLDEN,
	RESET_ACTION_RESCUE,
}RESET_ACTION;
#endif // ANDROID_REBOOT_CMD

/* Data used for customized ops */
typedef struct {
	/* Boot Logo */
	addr_t logo_addr_1;
	ssize_t logo_size_1;
} customize_data_t;

extern customize_data_t customize_data;
#endif // __RTKBOOT_H__
