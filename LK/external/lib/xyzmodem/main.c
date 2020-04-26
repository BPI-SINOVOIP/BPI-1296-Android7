#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/console.h>
#include <dev/uart.h>
#include <target/debugconfig.h>
#include <xyzModem.h>
#include <arch/ops.h>
#include <kernel/thread.h>

static int getcuart(void)
{
	return uart_getc(DEBUG_UART, false);
}

int serial_load(int argc, const cmd_args *argv)
{
	addr_t target_addr = NULL;
	connection_info_t info;
	char ymodemBuf[1024];
	int err;
	int res;

	if (argc != 2) {
		printf("usage : loady <TARGET_ADDR>\n");
		return -1;
	}

	target_addr = (addr_t)atoull(argv[1].str);
	printf("Start loading to %8p\n", (void*)target_addr);

	thread_set_priority(HIGH_PRIORITY);
	printf("changing thread to HIGH_PRIORITY\n");

	info.mode = xyzModem_ymodem;
	res = xyzModem_stream_open(&info, &err);

	if (res){
		printf("%s\n", xyzModem_error(err));
		return -1;
	}

	while ((res = xyzModem_stream_read(ymodemBuf, 1024, &err)) > 0) {
		thread_yield();
		memcpy((void*)target_addr, ymodemBuf, res);
		target_addr += res;
	}

	thread_set_priority(DEFAULT_PRIORITY);
	printf("changing thread back to normal priority\n");

	xyzModem_stream_close(&err);
	xyzModem_stream_terminate(false, &getcuart);

	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("loady", "Y-MODEM through UART", (console_cmd)&serial_load)
STATIC_COMMAND_END(loady);
