#ifndef GPIO_USERSPACE_ACCESS_LIBRARY
#define GPIO_USERSPACE_ACCESS_LIBRARY

#define MIS_PSELH	((volatile int)0xb801b000)
#define MIS_PSELL	((volatile int)0xb801b004)

#define MIS_GP0DIR	((volatile int)0xb801b100)
#define MIS_GP1DIR	((volatile int)0xb801b104)
#if IS_CHIP(VENUS) || IS_CHIP(NEPTUNE)
	#define MIS_GP0DATO	((volatile int)0xb801b108)
	#define MIS_GP1DATO	((volatile int)0xb801b10c)
	#define MIS_GP0DATI	((volatile int)0xb801b110)
	#define MIS_GP1DATI	((volatile int)0xb801b114)
#elif IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	#define MIS_GP2DIR	((volatile int)0xb801b108)
	#define MIS_GP3DIR	((volatile int)0xb801b10c)
	#define MIS_GP0DATO	((volatile int)0xb801b110)
	#define MIS_GP1DATO	((volatile int)0xb801b114)
	#define MIS_GP2DATO	((volatile int)0xb801b118)
	#define MIS_GP3DATO	((volatile int)0xb801b11c)
	#define MIS_GP0DATI	((volatile int)0xb801b120)
	#define MIS_GP1DATI	((volatile int)0xb801b124)
	#define MIS_GP2DATI	((volatile int)0xb801b128)
	#define MIS_GP3DATI	((volatile int)0xb801b12c)
#endif

#if IS_CHIP(JUPITER)
	#define ISO_GPDIR	((volatile int)0xB801BD70)
	#define ISO_GPDATO	((volatile int)0xB801BD74)
	#define ISO_GPDATI	((volatile int)0xB801BD78)
#elif IS_CHIP(SATURN)
	#define ISO_GPDIR	((volatile int)0xB8007100)
	#define ISO_GPDATO	((volatile int)0xB8007104)
	#define ISO_GPDATI	((volatile int)0xB8007108)
#endif 

#define VENUS_GPIO_IOC_MAGIC    'r'
#define VENUS_GPIO_ENABLE_INT		_IOW(VENUS_GPIO_IOC_MAGIC, 1, int)
#define VENUS_GPIO_DISABLE_INT		_IOW(VENUS_GPIO_IOC_MAGIC, 2, int)
#define VENUS_GPIO_SET_ASSERT		_IOW(VENUS_GPIO_IOC_MAGIC, 3, int)
#define VENUS_GPIO_SET_DEASSERT		_IOW(VENUS_GPIO_IOC_MAGIC, 4, int)
#define VENUS_GPIO_SET_SW_DEBOUNCE	_IOW(VENUS_GPIO_IOC_MAGIC, 5, int) 
#define VENUS_GPIO_CHOOSE_GPDEB1	_IOW(VENUS_GPIO_IOC_MAGIC, 6, int)
#define VENUS_GPIO_CHOOSE_GPDEB2	_IOW(VENUS_GPIO_IOC_MAGIC, 7, int)
#define VENUS_GPIO_CHOOSE_GPDEB3	_IOW(VENUS_GPIO_IOC_MAGIC, 8, int)
#define VENUS_GPIO_CHOOSE_GPDEB4	_IOW(VENUS_GPIO_IOC_MAGIC, 9, int)
#define VENUS_GPIO_RESET_INT_STATUS	_IO(VENUS_GPIO_IOC_MAGIC, 10)


#define VENUS_GPIO_IOC_MAXNR    9
#define VENUS_GPIO_DEVICE		"/dev/misc/venus_gpio"

int getGPIO(int GPIO_NUM);
int setGPIO(int GPIO_NUM, int value);
#if IS_CHIP(JUPITER) || IS_CHIP(SATURN)
int getISOGPIO(int ISOGPIO_NUM);
int setISOGPIO(int ISOGPIO_NUM, int value);
#endif 


#endif
