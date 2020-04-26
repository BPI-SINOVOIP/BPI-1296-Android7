#ifndef __UART_WRITE_H
#define __UART_WRITE_H

static int do_uart_w( int argc, const cmd_args *argv);
void cmd_usage(void);
int do_uart_w_sn(char* buf ,int len);
int do_uart_w_mac(const char *mac_value);
#endif
