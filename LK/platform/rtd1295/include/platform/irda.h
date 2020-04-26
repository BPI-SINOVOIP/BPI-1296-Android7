#ifndef _RTK_IRDA_H_
#define _RTK_IRDA_H_

void IR_init(void);
unsigned int IR_Get_irdvf(void);
void IR_Set_irdvf(unsigned int data);
unsigned int IR_Parsing_key(void);
int IR_config_wakeup_keys(void *addr);

#endif
