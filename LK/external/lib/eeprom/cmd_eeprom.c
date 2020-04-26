#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <app.h>
#include <lib/console.h>
#include <rtk_i2c-lib.h>

/************************************************************************************************
*   EEPROM_SIZE:								 		*
*   24c00: 16, 24c01: 128, 24c02: 256, 24c04: 512, 24c08: 1024, 24c16: 2048	 		*
*   24c32: 4096, 24c64: 8192, 24c128: 16384, 24c256: 32768, 24c512: 65536, 	 		*
*												*
*   I2C_EEPROM_ADDR_LEN:									*
*   24c00, 24c01,24c02, 24c04, 24c08, 24c16 the address mode is 1 bytes,                        *
*   24c32, 24c64, 24c128, 24c256, 24c512, the address mode is 2 bytes                           *
*                                                                                               *
*   PAGE_WRITE_SIZE:										*
*   EEPROM supports page write mechanism, however, each eeprom type has different		*
*   page write size. User needs to read the eeprom spec and look for the page write 		*
*   size, or user will get trouble when write operation attempts to croos a page 		*
*   boundary. The following comment is from eeprom data sheet:					*
*  "Page write operations are limited to writing bytes within a single physical page,		*
*   regardless of the number of bytes actually being written. Physical page boundaries		*
*   start at addresses that are integer multiples of the page buffer size (or ‘page size’)	*
*   and end at addresses that are integer multiples of [page size - 1].				*
*   If a Page Write command attempts to write across a physical page boundary,			*
*   the result is that the data wraps around to the beginning of the current page		*
*   (overwriting data previously stored there), instead of being written to the next page,	*
*   as might be expected. It is, therefore, necessary for the application software to prevent	*
*   page write operations that would attempt to cross a page boundary."				*
*************************************************************************************************/
#define AT24C01		0
#define AT24C01A        1
#define AT24C02		2
#define AT24C04		3
#define AT24C08A	4
#define AT24C16A	5
#define AT24C32		6
#define AT24C64		7
#define AT24C128	8
#define AT24C256	9
#define AT24C512	10
/*new type eeprom type add here*/

#define EEPROM_SUPPORT_LIST 12 //when new eeprom type is added into eeprom_support, users need to modify this value

struct eeprom_data{
    unsigned int   EEPROM_SIZE;		//eeprom capacity
    unsigned short PAGE_WRITE_SIZE;	//write page mechanism, 0 if there is not such limitation
};

/*these data are from ATMEL EEPROM spec, it would need to be modified if any bugs*/
struct eeprom_data EEPROM[EEPROM_SUPPORT_LIST] ={
    {128,	4},		//AT24C01
    {128,       8},             //AT24C01A
    {256,	8},		//AT24C02
    {512,	16},		//AT24C04
    {1024,	16},		//AT24C08A
    {2048,	16},		//AT24C16A, AT24C164
    {4096,	32},		//AT24C32
    {8192,	32},		//AT24C64
    {16384,	64},		//AT24C128
    {32768,	64},		//AT24C256
    {65536,	128},		//AT24C512
    {0,0/*New support EEPROM will pe defined from here*/}
};

//***************************************   User Define Area   ************************************************
#define EEPROM_TYPE	AT24C128	//currently, we use AT24C128
#define I2C_CHANNEL	5		//In Realtek design, eeprom using i2c 5, eeprom device address is 0x50
#define EEPROM_ADDR	0x50
//*************************************************************************************************************

/*---------------If new support eeproms are added, the following if condition should be modified ----------------*/
#if EEPROM_TYPE >= AT24C32             //new 2 bytes address mode eeprom should be put behind AT24C32
	#define I2C_EEPROM_ADDR_LEN 2
#else
	#define I2C_EEPROM_ADDR_LEN 1
#endif

#if EEPROM_TYPE == AT24C01
	#define BUFFER_SIZE 4
#elif EEPROM_TYPE > AT24C01 && EEPROM_TYPE < AT24C04
	#define BUFFER_SIZE 8
#else
	#define BUFFER_SIZE 16
#endif
/*-----------------------------------------------------------------------------------------------------------------*/

static int show_usage(void);
#define CMD_RET_USAGE show_usage()

static int string_check(const char *input);

static int IS_DIGIT(char input)
{
    int asci_num = 0;
    asci_num = input;
    if(asci_num >= 48 && asci_num <= 57) return 1;
    return 0;
}

static unsigned short string_to_integer(const char *str)
{
    unsigned short val = 0;
    unsigned short base = 0;

    if((str+1)!=NULL && str[1]== 'x') {
	str = str + 2;	//skip 0x
	base = 16;
    }
    else base =10;    

    while (*str != 0) {
	if(IS_DIGIT(*str)) val = val * base + (unsigned short)(*str - '0');
	else  val = val * base + (unsigned short)(*str-32-'A'+10);
        str ++;
    }
    return val;
}

int eeprom_read (unsigned short offset, unsigned short len)
{
    unsigned char buf[BUFFER_SIZE]={'\0'};
    int index = 0;
    unsigned short sub_len = 0;
    unsigned int count=0;

    printf("Read content from eeprom address %#x :\n",offset);
    while(len) {
	if(len > BUFFER_SIZE) {
                sub_len = BUFFER_SIZE;
        }
        else sub_len = len;

#if I2C_EEPROM_ADDR_LEN == 1
    	unsigned char offs;
    	offs = offset;
    	I2C_Write_EEPROM(I2C_CHANNEL, EEPROM_ADDR, I2C_EEPROM_ADDR_LEN, &offs, NON_STOP);
    	I2C_Read_EEPROM(I2C_CHANNEL, EEPROM_ADDR, I2C_EEPROM_ADDR_LEN, &offs, sub_len, buf, NON_STOP);

#else //I2C_EEPROM_ADDR_LEN = 2
    	unsigned char offs[2];
    	offs[0] = offset >> 8;
    	offs[1] = offset & 0xff;
    	I2C_Write_EEPROM(I2C_CHANNEL, EEPROM_ADDR, I2C_EEPROM_ADDR_LEN, offs, NON_STOP);
    	I2C_Read_EEPROM(I2C_CHANNEL, EEPROM_ADDR, I2C_EEPROM_ADDR_LEN, offs, sub_len, buf, NON_STOP);
#endif
	for (index=0; index<sub_len; index++) {
		printf("%c", buf[index]);
		if(count%10==9) printf("\n");
		count++;
	}

	len -= sub_len;
	offset += sub_len;
    }
    printf("\n");
    return 0;
}

int eeprom_write (unsigned short offset, const char *buffer, unsigned short len)
{
    unsigned char buf[BUFFER_SIZE+I2C_EEPROM_ADDR_LEN]={'\0'};
    unsigned short sub_len = 0;
    unsigned short cur = 0;
    unsigned short straddle = EEPROM[EEPROM_TYPE].PAGE_WRITE_SIZE;

    if(offset%BUFFER_SIZE != 0) {	//currently , each io is 16 bytes, if offset 1-15, 17-31...etc, it must cross another page
	while(straddle < offset)
	    straddle += EEPROM[EEPROM_TYPE].PAGE_WRITE_SIZE;
    }

    while(len) {
	if(len > BUFFER_SIZE) {
		sub_len = BUFFER_SIZE;
	}
	else sub_len = len;

	//avoid crossing another page in write operation
	if(offset < straddle && (offset+sub_len) > straddle)
		sub_len = straddle - offset;

#if I2C_EEPROM_ADDR_LEN  == 1
    	buf[0] = offset & 0xff;
    	memcpy(&buf[1], &buffer[cur], sub_len);
#else	//I2C_EEPROM_ADDR_LEN = 2
    	buf[0] = offset >> 8;
    	buf[1] = offset & 0xff;
    	memcpy(&buf[2], &buffer[cur], sub_len);
#endif

    	I2C_Write_EEPROM(I2C_CHANNEL, EEPROM_ADDR, sub_len + I2C_EEPROM_ADDR_LEN, buf, NO_READ);

	cur += sub_len;
	offset += sub_len;
	len -= sub_len;
    }
    return 0;
}

static int do_eeprom(int argc, cmd_args *argv)
{
    int err1=0, err2=0;
    unsigned short len=0, off=0;
    unsigned short content_size=0;

#if I2C_EEPROM_ADDR_LEN != 1 && I2C_EEPROM_ADDR_LEN != 2
    printf("Address mode must be 1 or 2\n");
    printf("please set the correct address mode in I2C_EEPROM_ADDR_LEN!!!\n");
    return 1;
#endif

    if (argc != 4)
	return CMD_RET_USAGE;

    if(strcmp(argv[1].str,"read")==0) {
	err1 = string_check(argv[2].str);
       	err2 = string_check(argv[3].str);

        if(!err1 && !err2) {
	    off = string_to_integer(argv[2].str);
	    len = string_to_integer(argv[3].str);

            if(off > EEPROM[EEPROM_TYPE].EEPROM_SIZE || (off+len) > EEPROM[EEPROM_TYPE].EEPROM_SIZE) {
		printf("Invalid address or size...\n");
                return CMD_RET_USAGE;
            }
	    I2CN_Init(I2C_CHANNEL);
	    eeprom_read (off, len);
	    I2CN_UnInit(I2C_CHANNEL);
	}
    }
    else if(strcmp(argv[1].str,"write")==0) {
	err1 = string_check(argv[2].str);
        content_size = (unsigned short)strlen(argv[3].str);

        if(!err1) {
	    off = string_to_integer(argv[2].str);
            if(off > EEPROM[EEPROM_TYPE].EEPROM_SIZE || (off + content_size) > EEPROM[EEPROM_TYPE].EEPROM_SIZE) {
		printf("Invalid address or content size...\n");
                return CMD_RET_USAGE;
            }
	    I2CN_Init(I2C_CHANNEL);
	    eeprom_write(off, argv[3].str,content_size);
	    I2CN_UnInit(I2C_CHANNEL);
        }
    }
    else
	return CMD_RET_USAGE;

    return 0;
}

static int string_check(const char *input)
{
    int  asci_num=0;
    int index=0;
    int hex=0;

    if(strstr(input,"0x")) //hex format
	hex = 1;

    if(hex == 0) {
	for(index=0;input[index]!='\0';index++) {
            asci_num = input[index];
            //only accept 0-9
            if(asci_num >= 48 && asci_num <= 57);
            else {
                printf("argument format must be 0-9\n");
                return 1;
            }
        }	
    }
    else {
        for(index=2;input[index]!='\0';index++) {
            asci_num = input[index];
	    //only accept 0-9 a-f
            if((asci_num >= 48 && asci_num <= 57) || (asci_num >= 97 && asci_num <= 102));
            else {
                printf("argument format must be 0-9, a-f\n");
                return 1;
            }
        }
    }
    return 0;
}

static int show_usage(void)
{
    printf("EEPROM commands:\n"
        "eeprom read <ofs> <len>                - read offset ofs and length len from eeprom\n"
        "eeprom write <ofs> <content>		- write the <content> to ofsset ofs to eeprom\n"
    );
    return 0;
}

static void eeprom_init(const struct app_descriptor *app)
{
    printf("EEPROM: SIZE=%d, ADRESS_MODE=%d, PAGE_WRITE_SIZE=%d, BUFFER_SIZE=%d\n",
	EEPROM[EEPROM_TYPE].EEPROM_SIZE, I2C_EEPROM_ADDR_LEN, EEPROM[EEPROM_TYPE].PAGE_WRITE_SIZE, BUFFER_SIZE);
}

static void eeprom_entry(const struct app_descriptor *app, void *args)
{
}

STATIC_COMMAND_START
STATIC_COMMAND("eeprom", "EEPROM commands", (console_cmd)&do_eeprom)
STATIC_COMMAND_END(eeprom);

APP_START(eeprom)
    .init = eeprom_init,
    .entry = eeprom_entry,
    .flags = 0
APP_END
