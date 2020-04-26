#include <app.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/console.h>
#include <rtk_i2c-lib.h>

static unsigned int current_bus = 0;

int rtk_i2c_get_bus(void)
{
	printf("Current bus: %u\n", current_bus);

	return 0;
}

int rtk_i2c_set_bus(int argc, const cmd_args *argv)
{
	unsigned int change_bus;

	if (argc < 3 || (int)argv[2].i < 0){
		printf("invalid format\n");
		return -1;
	}

	change_bus = (unsigned int)argv[2].u;

	if (change_bus >= I2C_GetBusCount()) {
		printf("Bad bus: %d\n", change_bus);
	} else { 
		current_bus = change_bus;
		printf("Switch to i2c bus %u\n", change_bus);
	}
		
	return 0;
}

int rtk_i2c_find(int argc, const cmd_args *argv)
{
	int i, ret, tmp = 0;
	uchar buffer[2];
	unsigned short chip;

	if (argc > 2) {
		I2CN_Init(current_bus);
		printf("device address: ");
		for (i = 2; i < argc; i++) {
			chip = strtoul(argv[i].str, NULL, 16);
			ret = I2C_Read_EX(current_bus, chip, 1, (unsigned char *)&tmp,
					1, buffer, NON_STOP);
			if (!ret)
				printf("0x%02x ", (unsigned int)argv[i].u);
		}
		printf("\n");
		I2CN_UnInit(current_bus);
		return 0;
	}
	printf("device address: ");
	I2CN_Init(current_bus);
	i = 0;
	while (i < 128) {
		ret = I2C_Read_EX(current_bus, i, 1, (unsigned char *)&tmp,
					1, buffer, NON_STOP);
		if (!ret)
			printf("0x%02x ", i);
		i++;
	}
	printf("\n");
	I2CN_UnInit(current_bus);
	return 0;
}


int rtk_i2c_get_speed(void)
{
	unsigned int current_speed;

	current_speed = I2C_GetSpeed(current_bus);
	printf("current speed: %u\n", current_speed);
	return 0;
}

int rtk_i2c_set_speed(int argc, const cmd_args *argv)
{
	unsigned int change_speed;
	int ret;
	if (argc < 3 || (int)argv[2].i < 0) {
		printf("invalid format\n");
		return 0;
	}
	change_speed = (unsigned int)argv[2].u;
	I2CN_Init(current_bus);
	ret = I2C_SetSpeed(current_bus, change_speed);
	I2CN_UnInit(current_bus);
	if (ret)
		printf("failed to change speed\n");
	else
		printf("change speed to %u KHz\n", change_speed);
	return 0;
}

int _rtk_i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int ret = 0;
	unsigned char tmp[2];

	if (alen >= 4)
		return -1;

	memcpy(tmp, (char *)&addr, alen);
	ret = I2C_Read_EX(current_bus, chip, alen, tmp, len, buffer, NON_STOP);

	return ret;
}


int rtk_i2c_read(int argc, const cmd_args *argv)
{
	unsigned int chip_addr, reg_addr, len;
	int i, j, k, ret;
	unsigned char tmp[2];

	if (argc < 4 ||argv > 6) {
		printf("invalid format\n");
		return 0;
	}
	chip_addr = strtoul(argv[2].str, NULL, 16);
	if (chip_addr >= 128) {
		printf("chip_addr is out of range!\n");
		return 0;
	}
	reg_addr = strtoul(argv[3].str, NULL, 16);
	if (reg_addr >=256) {
		printf("reg_addr out of range!\n");
		return 0;
	}
	len = strtoul(argv[4].str, NULL, 16);
	if (len > 256) {
		printf("length should be less than 0x100\n");
		return 0;
	}
	I2CN_Init(current_bus);
	if (argc == 4) {
		unsigned char buffer[16];
		memcpy(tmp, (char *)&reg_addr, 1);
		ret = I2C_Read_EX(current_bus, chip_addr, 1, tmp, 16, buffer, NON_STOP);
		if(ret != 0) {
			printf("Xfer Failed!!\n");
			return 0;
		}
		printf("chip address: 0x%02x\n", chip_addr);
		printf("%04x:",reg_addr);
		for (i = 0; i < 16; i++) {
			printf("%02x ",buffer[i]);
		}
		printf("\n");
		I2CN_UnInit(current_bus);
		return 0;
	}
	printf("chip address: 0x%02x\n", chip_addr);
	for (j = len, k = reg_addr; j > 0; j = j-16, k = k+16) {
		unsigned char buffer[16];
		printf("%04x:", k);
		if (j < 16) {
			memcpy(tmp, (char *)&k, 1);
			ret = I2C_Read_EX(current_bus, chip_addr, 1, tmp, j, buffer, NON_STOP);
			if(ret != 0) {
				printf("Xfer Failed!!\n");
				return 0;
			}
			for (i = 0; i < j; i++) {
				printf("%02x ",buffer[i]);
			}
			printf("\n");
		} else {
			memcpy(tmp, (char *)&k, 1);
			ret = I2C_Read_EX(current_bus, chip_addr, 1, tmp, 16, buffer, NON_STOP);
			if(ret != 0) {
				printf("Xfer Failed!!\n");
				return 0;
			}
			for (i = 0; i < 16; i++) {
				printf("%02x ",buffer[i]);
			}
			printf("\n");
		}
	}
	I2CN_UnInit(current_bus);
	return 0;
}

int rtk_i2c_write(int argc, const cmd_args *argv)
{
	unsigned int chip_addr, reg_addr;
	unsigned char buffer[256], tmp[256], value;
	int i, len, ret;
	if (argc < 4) {
		printf("invalid format\n");
		return 0;
	}
	chip_addr = strtoul(argv[2].str, NULL, 16);
	if (chip_addr >= 128) {
		printf("chip_addr is out of range!\n");
		return 0;
	}
	reg_addr = strtoul(argv[3].str, NULL, 16);
	if (reg_addr >=256) {
		printf("reg_addr out of range!\n");	
		return 0;
	}

	len=0;

	for (i = 4; i < argc; i++) {
		value = strtoul(argv[i].str, NULL, 16);
		buffer[len] = value;
		len++;
	}
	if (len > 255) {
		printf("Too many value!");
		return 0;
	}
	I2CN_Init(current_bus);
	memcpy(tmp, (char *)&reg_addr, 1);
	memcpy(tmp + 1, buffer, len);
	ret = I2C_Write_EX(current_bus, chip_addr, 1 + len, tmp, NO_READ);
	I2CN_UnInit(current_bus);

	if(ret != 0) {
		printf("Xfer Failed!!\n");
		return 0;
	}

	return 0;
}



static void show_usage(void)
{
	printf("I2C commands\n"
		"i2c bus                                       - show current i2c bus number\n"
		"i2c setbus <number>                           - switch to i2c bus <number>\n"
		"i2c find <address1> <address2>...             - find and show devices on current I2C bus\n"
		"i2c speed                                     - show speed of current I2C bus\n"
		"i2c setspeed <KHz>                            - set speed of current I2C bus to <KHz>(freq range 10~800kHz)\n"
		"i2c read <chip addr> <reg addr> <# of value>  - read # of value of i2c device on address from reg address\n"
		"i2c write <chip addr> <reg addr> <value1> <value2>...<value256>\n"
		"                                              - write value from reg address of i2c device\n"
		"i2c help                                      - i2c command info\n"
		);
}

int rtk_i2c(int argc, const cmd_args *argv)
{
	int ret;
	if (argc < 2) {
		show_usage();
		return -1;
	}

	if (!strcmp(argv[1].str, "bus")) {
		rtk_i2c_get_bus();
	} else if (!strcmp(argv[1].str, "setbus")) {
		ret = rtk_i2c_set_bus(argc, argv);
		if (ret)
			printf("Failed to switch i2c\n");
	} else if (!strcmp(argv[1].str, "find")) {
		rtk_i2c_find(argc, argv);
	} else if (!strcmp(argv[1].str, "speed")) {
		rtk_i2c_get_speed();
	} else if (!strcmp(argv[1].str, "setspeed")) {
		rtk_i2c_set_speed(argc, argv);
	} else if (!strcmp(argv[1].str, "read")) {
		rtk_i2c_read(argc, argv);
	} else if (!strcmp(argv[1].str, "write")) {
		rtk_i2c_write(argc, argv);
	} else if (!strcmp(argv[1].str, "help")) {
		show_usage();
	}
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("i2c", "i2c commands", (console_cmd)&rtk_i2c)
STATIC_COMMAND_END(rtki2c);
