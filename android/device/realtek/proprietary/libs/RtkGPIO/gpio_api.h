#ifndef __GPIO_API_H__
#define __GPIO_API_H__

//#ifdef __cpluscplus
//extern "C" {
//#endif

int test(void);
int gpio_read(int gpio_num, unsigned char* res);
int gpio_write(int gpio_num, int value, unsigned char* res);

//#ifdef __cpluscplus
//}
//#endif

#endif
