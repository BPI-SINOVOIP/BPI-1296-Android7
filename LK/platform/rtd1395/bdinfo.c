#include <stdio.h>
#include <string.h>
#include <lib/console.h>
#include <platform/setting.h>
#include <platform/rtd1395.h>
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
	
	/* If the ddr information is protect, not showing ddr information. */
	if (dc_sys_misc == 0xdeaddead)
		return 0;

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
		printf("%-12s = %s MT/s (0x%02x) - DC%d\n", name, speed, speed_setting, i);
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

#define set_SYS_PLL_reg(reg,data)   (*((volatile unsigned int*) reg)=data)
#define get_SYS_PLL_reg(reg)   (*((volatile unsigned int*) reg))
#define PLLBUS 		0x98000424
#define PLLDCSB		0x98000428
#define PLLACPU		0x9800042C
#define PLLDDSA		0x98000430
#define PLLDDSB		0x98000434
#define PLLGPU 		0x98000438
#define PLLVCPU 	0x9800043C
#define PLLVCPU_2 	0x98000440
#define PLLSCPU		0x9801D700

unsigned int get_pll_frequency(unsigned int reg)
{
	int wait_count = 0; /*For getting pll frequency*/
	unsigned int val, freq_pll = 0;
	set_SYS_PLL_reg((uintptr_t)reg,0x0); //reset all	 
	set_SYS_PLL_reg((uintptr_t)reg,0x1); //reg_rstn=1
	set_SYS_PLL_reg((uintptr_t)reg,0x3); //reg_rstn_count_en=1
	while(wait_count < 10000){
		val = get_SYS_PLL_reg((uintptr_t)reg);
		/*Wait reg[30] =1*/
		if ( (val & 0x40000000) == 0x40000000){
			freq_pll = ( (val >> 13) & 0x0001FFFF ) / 10;
			/*Read reg[29:13] value and divide 10*/
			wait_count = 0;
			break;
		}
		wait_count++;
	}
	return freq_pll;
}

unsigned int get_scpu_pll_frequency(unsigned int reg)
{
	int wait_count = 0; /*For getting pll frequency*/
	unsigned int val, freq_pll = 0;
	set_SYS_PLL_reg((uintptr_t)reg,0x0); //reset all	 
	set_SYS_PLL_reg((uintptr_t)reg,0x20000); //reg_rstn_b=1
	set_SYS_PLL_reg((uintptr_t)reg,0x30000); //reg_rstn_counter_b=1
	while(wait_count < 10000){
		val = get_SYS_PLL_reg((uintptr_t)(reg+0x8));
		/*Wait (reg+8)[0] =1*/
		if ( (val & 0x1) == 0x1){
			freq_pll = ( (val >> 1) & 0x0001FFFF ) / 10;
			/*Read (reg+8)[17:1] value and divide 10*/
			wait_count = 0;
			break;
		}
		wait_count++;
	}
	return freq_pll;
}

int get_divide_freq(unsigned int pll_div)
{
	switch(pll_div)
	{
		case 3:
			return 4;
		case 2:
			return 2;
		case 1:
		case 0:
			return 1;
		default:
			return 1;
	}
}

int get_acpu_divide_freq(unsigned int pll_div)
{
	switch(pll_div)
	{
		case 0x1F:
			return 32;
		case 0xF:
			return 16;
		default:
			if((pll_div & 0xF) == 0x7)
				return 8;
			else if((pll_div & 0x7) == 0x3)
				return 4;
			else if((pll_div & 0x3) == 0x1)
				return 2;
			else if((pll_div & 0x1) == 0x0)
				return 1;
			else
				return 1;
	}
}

int get_scpu_divide_freq(unsigned int pll_div)
{
	unsigned int tmp_pll_div = (pll_div & 0xFC);
	switch(tmp_pll_div)
	{
		case 0xB4:
			return 13;
		case 0xA8:
			return 10;
		case 0xA0:
			return 8;
		case 0x9C:
			return 7;
		case 0x98:
			return 6;
		case 0x94:
			return 5;
		case 0x8C:
			return 3;
		default:
			tmp_pll_div = (pll_div & 0x83);
			if (tmp_pll_div == 0x3)
				return 4;
			else if (tmp_pll_div == 0x2)
				return 2;
			else if (tmp_pll_div == 0)
				return 1;
			else
				return 1;
	}
}

static int print_bdinfo_pll(void)
{
#define __print_pll() \
    printf("%-12s = %4d MHz\n", name, freq);
#define __print_pll_div(freq_div) \
    printf("%-12s = %4d MHz (div %d)\n", name, freq, freq_div);

    const char *name;
	int divide_freg;
    unsigned int freq;

    printf("---- PLL information ----\n");

	name = "BUS PLL";
	freq = get_pll_frequency(PLLBUS);
	__print_pll();
	
	name = "BUS";
	divide_freg = get_divide_freq((get_SYS_PLL_reg(SYS_PLL_DIV_reg) & 0x3));
	freq = freq / divide_freg;
	__print_pll_div(divide_freg);
	
	name = "DCSB PLL";
	freq = get_pll_frequency(PLLDCSB);
	__print_pll();
	
	name = "DCSB";
	divide_freg = get_divide_freq(((get_SYS_PLL_reg(SYS_PLL_DIV_reg) >> 2) & 0x3));
	freq = freq / divide_freg;
	__print_pll_div(divide_freg);
	
	name = "ACPU PLL";
	freq = get_pll_frequency(PLLACPU);
	__print_pll();
	
	name = "ACPU";
	divide_freg = get_acpu_divide_freq(((get_SYS_PLL_reg(SYS_PLL_DIV_reg) >> 22) & 0x1F));
	freq = freq / divide_freg;
	__print_pll_div(divide_freg);
	
	name = "DDSA PLL";
	freq = get_pll_frequency(PLLDDSA);
	__print_pll();
	
	name = "DDSB PLL";
	freq = get_pll_frequency(PLLDDSB);
	__print_pll();
	
	name = "GPU PLL";
	freq = get_pll_frequency(PLLGPU);
	__print_pll();
	
	name = "VCPU PLL";
	freq = get_pll_frequency(PLLVCPU);
	__print_pll();
	
	name = "VCPU_2 PLL";
	freq = get_pll_frequency(PLLVCPU_2);
	__print_pll();
	
	name = "SCPU PLL";
	freq = get_scpu_pll_frequency(PLLSCPU);
	__print_pll();
	
	name = "SCPU";
	divide_freg = get_scpu_divide_freq(((get_SYS_PLL_reg(SYS_PLL_DIV_reg) >> 6) & 0xFF));
	freq = freq / divide_freg;
	__print_pll_div(divide_freg);

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
	if (get_chip_id() == RTK1395_CPU_ID)
		return "Hercules 1395";
	else if (get_chip_id() == RTK1355_CPU_ID)
		return "Hercules 1355";
	return "Hercules 1355";
}

static const char *get_chip_rev_str(void)
{
	switch(get_chip_rev())
    {
        case HERCULES_REV_A00:
            return "A00";
        case HERCULES_REV_A01:
            return "A01";
        case HERCULES_REV_B00:
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
    
    printf("---- Hercules Rev. %s ----\n", get_chip_rev_str());
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

	if (get_chip_rev() == HERCULES_REV_A00)
		printf("Chip Version: A00\n");
	else if (get_chip_rev() == HERCULES_REV_A01)
		printf("Chip Version: A01\n");
	else if (get_chip_rev() == HERCULES_REV_B00)
		printf("Chip Version: B00\n");
	else
		printf("Chip Version: Unknown\n");
	/*Showing chip version information*/
	
	if (get_chip_id() == RTK1395_CPU_ID)
		printf("Chip      ID: RTD1395\n");
	else if (get_chip_id() == RTK1355_CPU_ID)
		printf("Chip      ID: RTD1355\n");
	else
		printf("Chip      ID: Unknown\n");
	/*Showing chip id information*/
	
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("bdinfo", "Board information", (console_cmd)&rtk_bdinfo)
STATIC_COMMAND("chip", "detect soc version",(console_cmd)&do_chip)
STATIC_COMMAND_END(rtk_bdinfo);
