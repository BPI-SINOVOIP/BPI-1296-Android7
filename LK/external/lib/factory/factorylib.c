#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <factorylib.h>
#include <tar.h>
#include <lib/bio.h>
#include <lib/console.h>
#include <platform/setting.h>
#include <arch/ops.h>

#define ENABLE_FACTORY_CONSOLE_MODE
//#define ENABLE_FACTORY_DEBUG

#define FAC_PRINTF(fmt, args...)	printf(fmt, ##args)

#ifdef	ENABLE_FACTORY_DEBUG
#define FAC_DEBUG(fmt, args...)		printf(fmt, ##args)
#else
#define FAC_DEBUG(fmt, args...)
#endif

const char *bio_flash_name[] = {
	BIO_SPI_NAME,
	BIO_NAND_NAME,
	BIO_MMC_NAME,
	};

unsigned int bio_blk_size[] = {
	[BOOT_NOR_SERIAL] = SPI_BLOCK_SIZE,
	[BOOT_NAND] = 0,
	[BOOT_EMMC] = EMMC_BLOCK_SIZE,
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int factory_dirty = 0;
static uchar *factory_buffer = NULL;
static uchar *factory_buffer2 = NULL;
static int factory_tarsize = 0;
static int factory_current_pp = 0;
static int factory_seq_num = 0;
static ssize_t factory_size = 0;

static BOOT_FLASH_T boot_flash_type = FLASH_TYPE;
extern unsigned int tar_read_print_info;
extern unsigned char tar_read_print_prefix[32];

static bdev_t *system_flash = NULL;
static unsigned int flash_blk_size = 0;
static int flash_use_block_op = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int factory_get_start_address(addr_t *factory_addr)
{
	*factory_addr = 0;

	if (boot_flash_type == EMMC_FLASH) {
		*factory_addr = FACTORY_START;
	} else if (boot_flash_type == NAND_FLASH) {
		// We assume that reserve-size is always align to NAND block boundry so
		// there's no need to calculate IMG size <-> blocks mapping
		*factory_addr = NAND_TOTAL_RESERVE_BLOCKS * flash_blk_size + NAND_TOTAL_RESERVE_SIZE;
	} else {
		printf("%s: boot_flash_type %d not support yet!\n", __func__, boot_flash_type);
		return -1;
	}

	return 0;
}

static int factory_check_sanity(uchar *buffer, int length)
{
	uint factory_header_bytes = 512;
	unsigned long bdev_addr, bdev_size, factory_start;
	int rounding_offset = 0;
	uint n = 0;
	int ret = 0;
	posix_header *p_start = NULL;
	posix_header *p_end = NULL;
	char factory_header[] = FACTORY_HEADER_FILE_NAME;
	uint pp_ok = 0;
	unsigned int pp_seqnum[2], pp_tarsize[2];
	int i = 0;
	void *buf_contain_a_header;

 	if (buffer == NULL) {
		FAC_PRINTF("[FAC][ERR] factory buffer is NULL\n");
		return -1;
 	}

	if (factory_get_start_address(&factory_start)) {
		FAC_PRINTF("[FAC][ERR] Can't get factory start addr!\n");
		return -1;
	}

	/* reset factory data in ddr */
	memset(buffer, 0, length);

	/* extend buffer size since NAND use page operation */
	if (boot_flash_type == NAND_FLASH)
		factory_header_bytes = NAND_RW_ALIGNMENT;

	p_start = (posix_header *)malloc(factory_header_bytes);
	if (!p_start) {
		FAC_PRINTF("[FAC][ERR] can't allocate %u bytes for p_start!\n", factory_header_bytes);
		return -1;
	}

	buf_contain_a_header = (posix_header *)malloc(factory_size);
	if (!buf_contain_a_header) {
		FAC_PRINTF("[FAC][ERR] can't allocate %u bytes for buf_contain_a_header!\n", (unsigned)factory_size);
		return -1;
	}

	for (i = 0;i < 2;i++) {
		/* Read factory header */
		bdev_addr = factory_start + i * factory_size / 2;
		bdev_size = factory_header_bytes;
		bdev_addr_size(flash_use_block_op, flash_blk_size, bdev_addr, bdev_size, &bdev_addr, &bdev_size);
		n = BDEV_READ(flash_use_block_op, system_flash, p_start, bdev_addr, bdev_size);

		if (n < bdev_size) {
			/* Read error */
			FAC_PRINTF("[FAC][ERR] Get factory header from %s failed\n", bio_flash_name[boot_flash_type]);
			ret = -1;
		} else {
			/* Read success */
			FAC_DEBUG("[FAC] Get factory header [%d] from %s\n", i, bio_flash_name[boot_flash_type]);
		}

#ifdef ENABLE_FACTORY_DEBUG
		FAC_DEBUG("[FAC] ******** CHECK FACTORY HEADER [%d] ********\n", i);

		if (!ret) {
			FAC_DEBUG("[FAC] -- PASS: read %s\n", bio_flash_name[boot_flash_type]);
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: read %s\n", bio_flash_name[boot_flash_type]);
			continue;
		}

		if (!strcmp((char*)p_start, factory_header)) {
			FAC_DEBUG("[FAC] -- PASS: factory header\n");
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: factory header\n");
			continue;
		}

		if (tar_check_header((char*)p_start) == 0) {
			FAC_DEBUG("[FAC] -- PASS: tar header\n");
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: tar header\n");
			continue;
		}

		if (p_start->rtk_tarsize >= 256) {
			FAC_DEBUG("[FAC] -- PASS: tar size >= 256\n");
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: tar size < 256\n");
			continue;
		}

		if (!strcmp(p_start->rtk_signature, "RTK")) {
			FAC_DEBUG("[FAC] -- PASS: rtk signature\n");
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: rtk signature\n");
			continue;
		}

		if (p_start->rtk_tarsize < (factory_size / 2 - sizeof(posix_header))) {
			FAC_DEBUG("[FAC] -- PASS: tar size 0x%08x\n", p_start->rtk_tarsize);
		}
		else {
			FAC_DEBUG("[FAC] -- FAIL: tar size 0x%08x > 0x%08lx\n", p_start->rtk_tarsize, factory_size/2 - sizeof(posix_header));
			continue;
		}

		FAC_DEBUG("[FAC] ******** CHECK FACTORY HEADER [%d] ALL PASS\n", i);
		FAC_DEBUG("[FAC] Need to check the end header\n");
#endif

		if (!ret && !strcmp((const char *)p_start, factory_header) && (tar_check_header((char *)p_start) == 0) && (p_start->rtk_tarsize >= 256)
				&& !strcmp(p_start->rtk_signature, "RTK") && (p_start->rtk_tarsize < (factory_size / 2 - sizeof(posix_header)))) {

			pp_seqnum[i] = p_start->rtk_seqnum;
			pp_tarsize[i] = p_start->rtk_tarsize;

			// read the attched POSIX HEAD at the last of factory region
			// whcih should be the copy of head one (p_start).
			// Last POSIX-HEADER may not be aligned to flash-block-boundry so
			// offset calculation is required.
			bdev_addr = (factory_start + p_start->rtk_tarsize + i * factory_size / 2);
			bdev_size = 2 * flash_blk_size;
			// Round down to PAGE boundry if using NAND
			if (boot_flash_type == NAND_FLASH) {
				rounding_offset = bdev_addr - ROUNDDOWN(bdev_addr, NAND_RW_ALIGNMENT);
				bdev_addr = ROUNDDOWN(bdev_addr, NAND_RW_ALIGNMENT);
				bdev_size += rounding_offset;
			} else {
				rounding_offset = p_start->rtk_tarsize % flash_blk_size;
			}
			bdev_addr_size(flash_use_block_op, flash_blk_size, bdev_addr, bdev_size, &bdev_addr, &bdev_size);

			n = BDEV_READ(flash_use_block_op, system_flash, buf_contain_a_header, bdev_addr, bdev_size);

			p_end = (posix_header*)((void*)(buf_contain_a_header) + rounding_offset);

#ifdef ENABLE_FACTORY_DEBUG
			FAC_DEBUG("[FAC] **dump p_start at %p\n", p_start);
			tar_dump_posix_header(p_start);

			FAC_DEBUG("[FAC] **dump p_end at %p\n", p_end);
			tar_dump_posix_header(p_end);
#endif

			if (!memcmp(p_end, p_start, sizeof(posix_header))) {
				FAC_DEBUG("[FAC] pp %d ok\n", i);
				pp_ok |= (1 << i);
			}
		}
	}

	free(p_start);
	free(buf_contain_a_header);

	FAC_DEBUG("[FAC] result: pp_ok = 0x%x\n", pp_ok);
	switch (pp_ok) {
		case 1:
			factory_current_pp = 0;
			break;
		case 2:
			factory_current_pp = 1;
			break;
		case 3:
			if (pp_seqnum[1] > pp_seqnum[0]) {
				if ((pp_seqnum[1] - pp_seqnum[0]) > 0xFFFFFFF0) {
					factory_current_pp = 0;
				}
				else {
					factory_current_pp = 1;
				}
			}
			else {
				factory_current_pp = 0;
			}
			break;
		default:
		case 0:
			p_start = (posix_header *)buffer;
			tar_build_header((char *)p_start, FACTORY_HEADER_FILE_NAME, 0, '5');
			p_start->rtk_signature[0] = 'R';
			p_start->rtk_signature[1] = 'T';
			p_start->rtk_signature[2] = 'K';
			p_start->rtk_signature[3] = 0;
			p_start->rtk_tarsize = sizeof(posix_header);
			p_start->rtk_seqnum = factory_seq_num;
			tar_fill_checksum((char *)p_start);
			tar_check_header((char *)p_start);
			FAC_DEBUG("[FAC] pp:%d seq#:0x%x\n", factory_current_pp, factory_seq_num);
			FAC_PRINTF("[FAC] No factory data in %s\n", bio_flash_name[boot_flash_type]);
			return sizeof(posix_header);

	}

	bdev_addr = (factory_start + factory_current_pp * factory_size / 2);
	bdev_size = pp_tarsize[factory_current_pp];
	bdev_addr_size(flash_use_block_op, flash_blk_size, bdev_addr, bdev_size, &bdev_addr, &bdev_size);

	n = BDEV_READ(flash_use_block_op, system_flash, buffer, bdev_addr, bdev_size);

	factory_seq_num = pp_seqnum[factory_current_pp];

	// Clear tail copy to prevent from TAR path error
	p_start = (posix_header*)buffer;
	memset((void*)p_start + p_start->rtk_tarsize, 0, 2 * sizeof(posix_header));

	FAC_PRINTF("Factory: pp:%d, seq#:0x%x, size:0x%x\n",
		factory_current_pp, factory_seq_num, pp_tarsize[factory_current_pp]);

	return pp_tarsize[factory_current_pp];
}

static int factory_get_from_flash(uchar *buffer)
{
	int ret = 0;

	ret = factory_check_sanity(buffer, factory_size/2);

	return ret;
}

static int factory_save_to_flash(uchar* buffer, int len)
{
	posix_header *pp_start;
	int next_pp;
	unsigned int next_seqnum;
	uint n;
	unsigned long bdev_addr, bdev_size, factory_start;
	uint total_len;

 	if (buffer == NULL) {
		return -1;
 	}

	if (factory_get_start_address(&factory_start)) {
		FAC_PRINTF("[FAC][ERR] Can't get factory start addr!\n");
		return -1;
	}

	if (factory_current_pp < 0) {
		next_pp = 0;
		next_seqnum = 0;
	}
	else {
		next_pp = (factory_current_pp + 1) & 0x01;
		next_seqnum = factory_seq_num + 1;
	}

	/* first posix_header for data check while the other two is blank to indicate TAR end*/
	total_len = len + 3*sizeof(posix_header);

	/* the last block will be reserved to save serial no , and magic no */
	/* can't exceed 2M size - (1 block size)  */
	if (total_len > (unsigned)(factory_size / 2 - flash_blk_size) ) {
		FAC_DEBUG("[FAC][ERR] %s: too big\n", __func__);
		return 1;
	}

	/* fill the first header as RTK header */
	pp_start = (posix_header *)buffer;
	pp_start->rtk_signature[0] = 'R';
	pp_start->rtk_signature[1] = 'T';
	pp_start->rtk_signature[2] = 'K';
	pp_start->rtk_signature[3] = 0;
	pp_start->rtk_tarsize = (unsigned int)len;
	pp_start->rtk_seqnum = next_seqnum;
	tar_fill_checksum((char *)pp_start);

	/* copy this RTK header to the end of tar file for data check */
	memcpy((void*)pp_start + pp_start->rtk_tarsize, pp_start, sizeof(posix_header));
	/* Now create an blank TAR header at the end so user space program won't take garbage data */
	/* on storage as legal TAR data */
	memset((void*)pp_start + pp_start->rtk_tarsize + sizeof(posix_header), 0, 2*sizeof(posix_header));

	bdev_addr = factory_start + next_pp * factory_size / 2;
	bdev_size = ALIGN(total_len , flash_blk_size);
	bdev_addr_size(flash_use_block_op, flash_blk_size, bdev_addr, bdev_size, &bdev_addr, &bdev_size);

	n = BDEV_WRITE(flash_use_block_op, system_flash, buffer, bdev_addr, bdev_size);
	if (n < bdev_size)
	{
		/* Write error */
		FAC_PRINTF("[FAC][ERR] Save to %s FAILED\n", bio_flash_name[boot_flash_type]);
		return 1;
	}
	else
	{
		/* Write success */
		FAC_PRINTF("[FAC] Save to %s (blk#:0x%lx, buf:%8p, len:0x%x)\n",
			bio_flash_name[boot_flash_type], bdev_addr, buffer, total_len);
		FAC_PRINTF("[FAC] Save to %s (seq#:0x%x, pp:%d)\n",
			bio_flash_name[boot_flash_type], next_seqnum, next_pp);
	}

	/* remove the RTK header at the end of tar file to prevent side effect */
	memset((void *)pp_start + pp_start->rtk_tarsize, 0, 2 * sizeof(posix_header));

	factory_current_pp = next_pp;
	factory_seq_num = next_seqnum;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int factory_dump_info(void)
{

	FAC_PRINTF("****** dump factory info ******\n");
	FAC_PRINTF("boot_flash_type    = ");
	switch(boot_flash_type){
		case BOOT_NOR_SERIAL:
			FAC_PRINTF("NOR\n");
			break;

		case BOOT_NAND:
			FAC_PRINTF("NAND\n");
			break;

		case BOOT_EMMC:
			FAC_PRINTF("MMC\n");
			break;

		default:
			FAC_PRINTF("UNKNOWN\n");
			break;
	}

	FAC_PRINTF("factory_buffer     = %8p\n", factory_buffer);
	FAC_PRINTF("factory_buffer2    = %8p\n", factory_buffer2);
	FAC_PRINTF("factory_tarsize    = 0x%08x\n", factory_tarsize);
	FAC_PRINTF("factory_current_pp = 0x%08x\n", factory_current_pp);
	FAC_PRINTF("factory_seq_num    = 0x%08x\n", factory_seq_num);

	return 0;
}

int factory_dump_list(void)
{
	char *dst_addr;
	uint dst_length;

	tar_read_print_info = 1;

	memset(tar_read_print_prefix, 0, sizeof(tar_read_print_prefix));

	strcpy((char *)tar_read_print_prefix, FACTORY_HEADER_FILE_NAME);

	tar_read("DUMMY_NAME_FOR_DUMP", (char*)factory_buffer, factory_size/2, &dst_addr, &dst_length);

	tar_read_print_info = 0;

	return 0;
}

int factory_reset(void)
{
	int i = 0;
	posix_header *p_start = NULL;

 	if (factory_buffer == NULL) {
		FAC_PRINTF("[FAC] factory_reset: factory_buffer is NULL\n");
		return -1;
 	}

	/* reset sequence number */
	factory_seq_num = 0;

	/* reset tar size */
	factory_tarsize = sizeof(posix_header);

	memset(factory_buffer, 0, factory_size / 2);

	/* build RTK header */
	p_start = (posix_header *)factory_buffer;
	tar_build_header((char *)p_start, FACTORY_HEADER_FILE_NAME, 0, '5');
	p_start->rtk_signature[0] = 'R';
	p_start->rtk_signature[1] = 'T';
	p_start->rtk_signature[2] = 'K';
	p_start->rtk_signature[3] = 0;
	p_start->rtk_tarsize = sizeof(posix_header);
	p_start->rtk_seqnum = 0;
	tar_fill_checksum((char *)p_start);
	tar_check_header((char *)p_start);

	// save to pp factory data
	for (i = 0;i < 2;i++)
	{
		factory_dirty = 1;

		factory_save();
	}
	return 0;
}

int factory_delete(const char *path)
{
 	if (factory_buffer == NULL) {
		FAC_PRINTF("[FAC] factory_delete: factory_buffer is NULL\n");
		return -1;
 	}

	memset(factory_buffer2, 0, factory_size / 2);
	factory_tarsize = tar_add_or_delete((char *)factory_buffer, path, 0, 0, (char *)factory_buffer2, factory_size / 2, 0);
	factory_dirty = 1;

	return 0;
}

int factory_read_by_index(int index, char *buffer, int *length, char *name)
{
 	if (factory_buffer == NULL) {
		FAC_PRINTF("[FAC] factory_read_by_index: factory_buffer is NULL\n");
		return -1;
 	}

	return tar_read_by_index(index, (char *)factory_buffer, buffer, length, name);
}

/*
 * Return Value:
 *    0: success
 *    1: tar read failed
 *   -1: factory buffer error
 */
int factory_read(const char *path, char**buffer, int *length)
{
 	if (factory_buffer == NULL) {
		FAC_PRINTF("[FAC] factory_read: factory_buffer is NULL\n");
		return -1;
 	}

	return tar_read(path, (char *)factory_buffer, factory_size/2, buffer, (uint *)length) == 0;
}

int factory_write(const char *path, char *buffer, int length)
{
	int length_512 = ((length + 511)/512) * 512;

 	if (factory_buffer == NULL) {
		FAC_PRINTF("[FAC] factory_write: factory_buffer is NULL\n");
		return -1;
 	}

	if (length_512 + factory_tarsize + 512 > factory_size/2) {
		FAC_PRINTF("[FAC] factory_write: too big\n");
		return -1;
	}

	memset(factory_buffer2, 0 , factory_size/2);

	if ((factory_tarsize = tar_add_or_delete((char *)factory_buffer, path, buffer, length, (char *)factory_buffer2, factory_size/2, 1)) == 0)  {
		FAC_PRINTF("[FAC] factory_write: tar_add_or_delete fail\n");
		return -1;
	}
	FAC_DEBUG("factory_tarsize = 0x%x\n", factory_tarsize);

	factory_dirty = 1;

	return 0;
}

int factory_save(void)
{
	int ret = 0;

	FAC_PRINTF("[FAC] factory_save: ");

 	if (factory_buffer == NULL) {
		FAC_PRINTF("FAILED (factory_buffer is NULL)\n");
		return -1;
 	}

	if (factory_dirty && factory_tarsize > 0) {

		switch(boot_flash_type){
			case BOOT_NOR_SERIAL:
				//FAC_PRINTF("NOR\n");
				//ret = factory_save_to_SPI(factory_buffer, factory_tarsize);
				printf("%s, NOR not supported yet!!\n", __func__);
				ret = -1;
				break;

			case BOOT_NAND:
				FAC_PRINTF("NAND\n");
				ret = factory_save_to_flash(factory_buffer, factory_tarsize);
				break;

			case BOOT_EMMC:
				FAC_PRINTF("MMC\n");
				ret = factory_save_to_flash(factory_buffer, factory_tarsize);
				break;

			default:
				FAC_PRINTF("UNKNOWN\n");
				ret = -1;
				break;
		}
	}
	else {
		FAC_PRINTF("no change\n");
		FAC_DEBUG("[FAC] factory_dirty is %d\n", factory_dirty);
	}

	factory_dirty = 0;

	return ret;
}

int factory_init(void)
{
	int ret = 0;
	int factory_blocks = FACTORY_BLOCKS;

	system_flash = bio_open(bio_flash_name[boot_flash_type]);
	if (!system_flash) {
		printf("%s, can't open %s\n", __func__, bio_flash_name[boot_flash_type]);
		return -1;
	}
	//cloud add
	if (boot_flash_type ==BOOT_NAND)
		bio_blk_size[boot_flash_type] = bio_ioctl(system_flash, BIO_IOCTL_PUT_MEM_MAP, NULL);

	flash_blk_size = bio_blk_size[boot_flash_type];
	if (!flash_blk_size) {
		printf("%s, flash_blk_size 0\n", __func__);
	}

	// Get factory size, NAS with NAND flash use only 4 blocks as factory region
	// instead of static size.
	if (factory_blocks != 0)
		factory_size = flash_blk_size * factory_blocks;
	else
		factory_size = FACTORY_SIZE;

	printf("%s, factory size:0x%x\n", __func__, (unsigned)factory_size);

	factory_buffer = malloc(factory_size);
	factory_buffer2 = factory_buffer + (factory_size / 2);

	switch(boot_flash_type){
		case BOOT_NOR_SERIAL:
			//FAC_PRINTF("NOR\n");
			//factory_tarsize = factory_get_from_SPI(factory_buffer);
			printf("%s, FACTORY ON NOR NOT SUPPORTED YET!!", __func__);
			ret = -1;
			break;

		case BOOT_NAND:
			FAC_PRINTF("NAND\n");
			FAC_DEBUG("[NAND] factory_buffer  = 0x%p\n", factory_buffer);
			FAC_DEBUG("[NAND] factory_buffer2 = 0x%p\n", factory_buffer2);
			factory_tarsize = factory_get_from_flash(factory_buffer);
			FAC_DEBUG("[NAND] factory_tarsize = 0x%x\n", factory_tarsize);
			//printf("%s, FACTORY ON NAND NOT SUPPORTED YET!!", __func__);
			//ret = -1;
			break;

		case BOOT_EMMC:
			FAC_PRINTF("MMC\n");
			FAC_DEBUG("[FAC] factory_buffer  = 0x%p\n", factory_buffer);
			FAC_DEBUG("[FAC] factory_buffer2 = 0x%p\n", factory_buffer2);
			flash_use_block_op = 1;
			factory_tarsize = factory_get_from_flash(factory_buffer);
			FAC_DEBUG("[FAC] factory_tarsize = 0x%x\n", factory_tarsize);
			break;

		default:
			FAC_PRINTF("UNKNOWN\n");
			ret = -1;
			break;
	}

	return ret;
}

#ifdef ENABLE_FACTORY_CONSOLE_MODE
static void print_usage(void)
{
	printf(	"factory read path\n"
		"factory write path addr length\n"
		"factory delete path\n"
		"factory save\n"
		"factory reset\n"
		"factory info\n"
		"factory list\n" );
}

int do_factory(int argc, const cmd_args *argv)
{
	char *buffer, *dst_buffer;
	//unsigned long dst_buffer;//for rtice tool factory read
	int length;
	int rc;

	if (argc < 2) {
		print_usage();
		return -1;
	}
	else {
		if (strcmp(argv[1].str, "save") == 0) {
			factory_save();
		}
		else if (strcmp(argv[1].str, "reset") == 0) {
			factory_reset();
		}
		else if (strcmp(argv[1].str, "info") == 0) {
			factory_dump_info();
		}
		else if (strcmp(argv[1].str, "list") == 0) {
			factory_dump_list();
		}
		else if (strcmp(argv[1].str, "delete") == 0) {
			factory_delete(argv[2].str);
		}
		else if (strcmp(argv[1].str, "read") == 0) {
			rc = factory_read(argv[2].str, &buffer, &length);

			if (rc)
				FAC_PRINTF("%s: file not found\n", argv[2].str);
			else
				FAC_PRINTF("%s: %d bytes at %8p\n", argv[2].str, length, buffer);

			if (argc == 4) {
				dst_buffer = (char*)strtoul(argv[3].str, NULL, 16);
				memcpy((void *)dst_buffer, (void *)buffer, length);
				FAC_PRINTF("src[%8p] dst[%8p]\n", buffer, dst_buffer);
			}
		}
		else if (strcmp(argv[1].str, "write") == 0) {

			buffer = (char *)strtoul(argv[3].str, NULL, 16);
			length = (int)strtoul(argv[4].str, NULL, 16);

			factory_write(argv[2].str, buffer, length);
		}
		else {
			print_usage();
			return -1;
		}
	}

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("factory", "FACTORY sub system", (console_cmd)&do_factory)
STATIC_COMMAND_END(do_factory);

#endif

