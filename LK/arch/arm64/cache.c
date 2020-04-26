/*
 * Copyright (c) 2014-2016 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <debug.h>
#include <stdlib.h>
#include <arch/ops.h>
#include <trace.h>
#include <string.h>

#define LOCAL_TRACE 0

#if WITH_LIB_CONSOLE

#include <lib/console.h>

void show_cache_op_usage(void)
{
	printf("usage :\n");
	printf("\tcache flush <addr> <size>\n");
	printf("\tcache invalidate <addr> <size>\n");
}

int cache_op(int argc, const cmd_args *argv)
{
	addr_t addr;
	size_t length;

	if (argc != 4) {
		show_cache_op_usage();
		return -1;
	}

	addr = (addr_t)argv[2].u;
	length = (size_t)argv[3].u;

	if (strcmp(argv[1].str, "flush") == 0) {
		printf("flushing cache 0x%08lx~0x%08lx\n", addr, (addr + length));
		arch_clean_cache_range(addr, length);
	} else if (strcmp(argv[1].str, "invalidate") == 0) {
		printf("invalidate cache 0x%08lx~0x%08lx\n", addr, (addr + length));
		arch_invalidate_cache_range(addr, length);
	} else {
		printf("error command %s\n", argv[1].str);
		show_cache_op_usage();
		return -1;
	}

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("cache", "cache operation", (console_cmd)&cache_op)
STATIC_COMMAND_END(cache);
#endif //WITH_LIB_CONSOLE
