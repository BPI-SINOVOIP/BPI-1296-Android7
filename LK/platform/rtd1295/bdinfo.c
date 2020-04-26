#include <stdio.h>
#include <string.h>
#include <lib/console.h>
#include <platform/setting.h>
#include <platform/rtd1295.h>
#include <platform/sb2.h>
#include <platform/ddr.h>
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/iso_reg.h>
#include <platform/io.h>

#define EFUSE_DATA_118_reg                        0x980171D8
#define get_EFUSE_DATA_118_reg                    (*((volatile unsigned int*) EFUSE_DATA_118_reg))

#define DC0_DPI_DLL_CRT_SSC3_reg                      0x9800E028
#define get_DC0_DPI_DLL_CRT_SSC3_reg                  (*((volatile unsigned int*) DC0_DPI_DLL_CRT_SSC3_reg))
#define DC1_DPI_DLL_CRT_SSC3_reg                      0x9800F028
#define get_DC1_DPI_DLL_CRT_SSC3_reg                  (*((volatile unsigned int*) DC1_DPI_DLL_CRT_SSC3_reg))

#define DCPHY_DPI_DLL_CRT_SSC3_get_DPI_N_CODE_T(data)   ((0x0000FF00&(data))>>8)

unsigned int dc_regs[] = {DC0_DPI_DLL_CRT_SSC3_reg, DC1_DPI_DLL_CRT_SSC3_reg};

static const char * get_ddr_speed_str(unsigned int * speed_setting, int dc_num)
{
	unsigned int dpi_n_code_t;
	const char *ret;

	//dpi_n_code_t = DCPHY_DPI_DLL_CRT_SSC3_get_DPI_N_CODE_T(*(volatile unsigned int*)dc_regs[dc_num]);
	dpi_n_code_t = DCPHY_DPI_DLL_CRT_SSC3_get_DPI_N_CODE_T(rtd_inl(dc_regs[dc_num]));

	switch (dpi_n_code_t) {
		case 0x2a ... 0xff:
			ret = "OVER 2400";
			break;
		case 0x25 ... 0x29:
			ret = "2400";
			break;
		case 0x20 ... 0x24:
			ret = "2133";
			break;
		case 0x1b ... 0x1f:
			ret = "1866";
			break;
		case 0x16 ... 0x1a:
			ret = "1600";
			break;
		case 0x15:
			ret = "1333";
			break;
		case 0x0 ... 0x14 :
			ret = "UNDER 1333";
			break;
		default:	
			ret = "";
			break;
	}

	if (speed_setting)
		*speed_setting = dpi_n_code_t;

	return ret;
}

static int print_bdinfo_ddr(void)
{
	unsigned int speed_setting;
	const char *name;
	const char *speed;
	unsigned int dc_sys_misc = rtd_inl(DC_SYS_MISC);
	int dc_enable[MAX_DC_COUNT] = {0};

	printf("---- DDR information ----\n");

	if (DC_SYS_MISC_TWO_DCSYS_EN_GET(dc_sys_misc)) {
		dc_enable[0] = 1;
		dc_enable[1] = 1;
	} else { // DC_SYS_MISC_TWO_DCSYS_EN not set, check DC0 or DC1 used
		if (DC_SYS_MISC_OUT_TO_PHY_SEL_GET(dc_sys_misc))
			dc_enable[1] = 1;
		else
			dc_enable[0] = 1;
	}

	for (int i = 0 ; i < MAX_DC_COUNT ; i++) {
		if (!dc_enable[i])
			continue;
		name = "DDR";
		speed = get_ddr_speed_str(&speed_setting, i);
		printf("%-12s = %s MHz (0x%02x) - DC%d\n", name, speed, speed_setting, i);
	}

	print_ddr_info();
	return 0;
}


#define NF_TO_FREQ(parent, n, f) \
    ((parent) * ((n) + 3) + ((parent) * (f)  / 2048))

#define PDIV_NF_TO_FREQ(parent, pdiv, n, f) \
    NF_TO_FREQ((parent) * (pdiv + 1), n, f)

#define FREQ_DIV_SEL(parent, div) \
    ((parent) / ((div) == 3 ? 4 : (div) == 2 ? 2 : 1))

#define MNO_TO_FREQ(parent, m, n, o) \
    ((parent) * ((m) + 2) * ((n) + 1) * ((o) + 1))


static int print_bdinfo_pll(void)
{
#define __print_pll() \
    printf("%-12s = %4d MHz\n", name, freq);

    const char *name;
    unsigned int val;
    unsigned int freq;

    printf("---- PLL information ----\n");

    name = "SCPU PLL";
    val  = get_SYS_PLL_SSC_DIG_SCPU1_reg;
    freq = NF_TO_FREQ(27, 
        SYS_PLL_SSC_DIG_SCPU1_get_NCODE_T_SCPU(val), 
        SYS_PLL_SSC_DIG_SCPU1_get_FCODE_T_SCPU(val));
    __print_pll();

    name = "SCPU";
    val  = get_SYS_PLL_DIV_reg;
    freq = FREQ_DIV_SEL(freq, SYS_PLL_DIV_get_scpu_freq_sel(val));
    __print_pll();

    name = "ACPU PLL";
	val  = get_SYS_PLL_SSC_DIG_ACPU1_reg;
    freq = NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_ACPU1_get_NCODE_T_ACPU(val),
        SYS_PLL_SSC_DIG_ACPU1_get_FCODE_T_ACPU(val));
    __print_pll();

    name = "ACPU";
    val  = get_SYS_PLL_DIV_reg;
    freq = FREQ_DIV_SEL(freq, SYS_PLL_DIV_get_acpu_freq_sel(val));
    __print_pll();

    name = "VE1 PLL";
    val  = get_SYS_PLL_VE1_1_reg;
    freq = MNO_TO_FREQ(27,
        SYS_PLL_VE1_1_get_REG_PLLVCPU_M(val),
        SYS_PLL_VE1_1_get_REG_PLLVCPU_N(val),
        SYS_PLL_VE1_1_get_REG_PLLVCPU_O(val));
    __print_pll();

    name = "VE2 PLL";
    val  = get_SYS_PLL_VE2_1_reg;
    freq = MNO_TO_FREQ(27,
        SYS_PLL_VE2_1_get_REG_PLLVCPU_M_2(val),
        SYS_PLL_VE2_1_get_REG_PLLVCPU_N_2(val),
        SYS_PLL_VE2_1_get_REG_PLLVCPU_O_2(val));
    __print_pll();

    name = "DDSA PLL";
    val  = get_SYS_PLL_SSC_DIG_DDSA1_reg;
    freq = PDIV_NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_DDSA1_get_REG_PLLDDSA_ICO_PDIV2(val),
        SYS_PLL_SSC_DIG_DDSA1_get_NCODE_T_DDSA(val),
        SYS_PLL_SSC_DIG_DDSA1_get_FCODE_T_DDSA(val));
    __print_pll();

    name = "DDSB PLL";
    val  = get_SYS_PLL_SSC_DIG_DDSB1_reg;
    freq = PDIV_NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_DDSB1_get_REG_PLLDDSB_ICO_PDIV2(val),
        SYS_PLL_SSC_DIG_DDSB1_get_NCODE_T_DDSB(val),
        SYS_PLL_SSC_DIG_DDSB1_get_FCODE_T_DDSB(val));
    __print_pll();

    name = "BUS PLL";
    val  = get_SYS_PLL_SSC_DIG_BUS1_reg;
    freq = NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_BUS1_get_NCODE_T_BUS(val),
        SYS_PLL_SSC_DIG_BUS1_get_FCODE_T_BUS(val));
    __print_pll();

    name = "BUS_H PLL";
    val  = get_SYS_PLL_SSC_DIG_BUSH1_reg;
    freq = NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_BUSH1_get_NCODE_T_BUS_H(val),
        SYS_PLL_SSC_DIG_BUSH1_get_FCODE_T_BUS_H(val));
    __print_pll();

    name = "GPU PLL";
    val  = get_SYS_PLL_SSC_DIG_GPU1_reg;
    freq = NF_TO_FREQ(27,
        SYS_PLL_SSC_DIG_GPU1_get_NCODE_T_GPU(val),
        SYS_PLL_SSC_DIG_GPU1_get_FCODE_T_GPU(val));
    __print_pll();

    name = "VODMA PLL";
    val  = get_SYS_PLL_VODMA1_reg;
    freq = MNO_TO_FREQ(27,
        SYS_PLL_VODMA1_get_REG_PLLVODMA_M(val),
        SYS_PLL_VODMA1_get_REG_PLLVODMA_N(val),
        SYS_PLL_VODMA1_get_REG_PLLVODMA_O(val));
    __print_pll();

	return 0;
}

static int print_bdinfo_flash(void)
{
    printf("---- Flash information ----\n");

    if (FLASH_TYPE == SPI_FLASH) {
        printf("%-12s = %s\n", "boot_flash", "spi");

    } else if (FLASH_TYPE == NAND_FLASH) {
        printf("%-12s = %s\n", "boot_flash", "nand");


    } else if (FLASH_TYPE == EMMC_FLASH) {
        printf("%-12s = %s\n", "boot_flash", "emmc");
        // TODO get info
    } else {
        printf("%-12s = %s\n", "boot flash", "error");
    }

	return 0;
}

static const char *get_cpu_id_str(void)
{
    if ((get_EFUSE_DATA_118_reg & 0x3) == 1) 
        return "giraffe 1294";
    else if (get_ISO_CHIP_INFO1_reg & 0x00000800) 
        return "saola";
    return "giraffe";
}

static const char *get_chip_rev_str(void)
{
    switch(get_chip_rev())
    {
        case KYLIN_REV_A00:
            return "A00";
        case KYLIN_REV_A01:
            return "A01";
        case KYLIN_REV_B00:
            return "B00";
        default:
            return "Unrecognized";
    }
}

static int get_uart1_baudrate(void)
{
    // FIXME: get from uart1
    return 115200;
}


static int print_bdinfo(void)
{
    const char *name;
    unsigned int val;
    
    printf("---- Kylin Rev. %s ----\n", get_chip_rev_str());
    printf("%-12s = %s\n", "cpu_id", get_cpu_id_str()); 


    name = "baudrate";
    val = get_uart1_baudrate();
    printf("%-12s = %u bps\n", name, val);

    // TODO: eth info

    // TODO: TLB info

    return 0;
}


int rtk_bdinfo(int argc, const cmd_args *argv)
{
    if (argc == 1) {
        print_bdinfo();
        print_bdinfo_pll();
        print_bdinfo_ddr();
        print_bdinfo_flash();
    } else if (argc == 2) {
    	if (strcmp(argv[1].str, "pll") == 0) {
            print_bdinfo_pll();
        } else if (strcmp(argv[1].str, "ddr") == 0) {
			print_bdinfo_ddr();
		} else if (strcmp(argv[1].str, "flash") == 0) {
            print_bdinfo_flash();
        } else {
            goto print_usage;
        }
	}

    return 0;

print_usage:

    printf("bdinfo - show board information\n");
    printf("Usage:\n");
    printf("  bdinfo       - show all information\n");
    printf("  bdinfo pll   - show PLL information\n");
    printf("  bdinfo ddr   - show DDR information\n");
    printf("  bdinfo flash - show DDR information\n");

	return 0;
}

int do_chip(int argc, const cmd_args *argv){

	if (get_chip_rev() == KYLIN_REV_A00)
		printf("Chip Version: A00\n");
	else if (get_chip_rev() == KYLIN_REV_A01)
		printf("Chip Version: A01\n");
	else if (get_chip_rev() == KYLIN_REV_B00)
		printf("Chip Version: B00\n");
	else
		printf("Chip Version: Unknown\n");
	/*Showing chip version information*/
	
	if (get_chip_id() == RTK1295_CPU_ID)
		printf("Chip      ID: RTD1295\n");
	else if (get_chip_id() == RTK1296_CPU_ID)
		printf("Chip      ID: RTD1296\n");
	else if (get_chip_id() == RTK1294_CPU_ID)
		printf("Chip      ID: RTD1294\n");
	else if (get_chip_id() == RTK1198_CPU_ID)
		printf("Chip      ID: RTD1198\n");
	else
		printf("Chip      ID: Unknown\n");
	/*Showing chip id information*/
	
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("bdinfo", "Board information", (console_cmd)&rtk_bdinfo)
STATIC_COMMAND("chip", "detect soc version",(console_cmd)&do_chip)
STATIC_COMMAND_END(rtk_bdinfo);
