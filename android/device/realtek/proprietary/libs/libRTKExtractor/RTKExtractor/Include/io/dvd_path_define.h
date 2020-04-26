#ifndef __DVD_PATH_DEFINE__
#define __DVD_PATH_DEFINE__

#define PLAYBACK_ODD_PATH		"/dev/dvd"
#define RWAUTHOR_ODD_PATH		"/dev/cdrom0"
#define VR_HDD_PATH 			"/usr/local/etc/hdd/dvdvr"
#define VR_HDD_FAT32_PATH 		"/usr/local/etc/hdd/fat32"
#define VR_HDD_ROOT_PATH 		"/usr/local/etc/hdd/root"
#define VR_HDD_LIVEPAUSE_PATH 		"/usr/local/etc/hdd/livepause"
#define VR_HDD_MMAP_PATH		"/usr/local/etc/hdd/root/recovery"
#define VR_HDD_LIVEPAUSE_MMAP_PATH		"/tmp/ramfs" //"/tmp"
//#define VR_HDD_LIVEPAUSE_PATH_LEN	28
#define DVD_MOUNT_DIR 			"./DVD"
#define PROC_CD_TOC			"/proc/cd_toc"

#define FILE_IODEVICE_USB1      "/tmp/usbmounts/sda"
#define FILE_IODEVICE_USB2      "/tmp/usbmounts/sdb"
#define FILE_IODEVICE_USB3      "/tmp/usbmounts/sdc"
#define FILE_IODEVICE_USB4      "/tmp/usbmounts/sdd"
#define FILE_IODEVICE_PATH_LENGTH    18

#define IDE_HDD_MOUNT_POINT	"/usr/local/etc/hdd/volumes"


#endif /* __DVD_PATH_DEFINE__ */
