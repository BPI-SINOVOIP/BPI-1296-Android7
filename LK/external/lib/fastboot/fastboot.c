/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/thread.h>
#include <kernel/event.h>
#include <dev/udc.h>
#include <platform/interrupts.h>
#include <lib/console.h>
#include <rtkboot.h>
#include <lib/fastboot.h>

void boot_linux(void *bootimg, unsigned sz);

/* todo: give lk strtoul and nuke this */
static unsigned hex2unsigned(const char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

struct fastboot_cmd {
	struct fastboot_cmd *next;
	const char *prefix;
	unsigned prefix_len;
	void (*handle)(const char *arg, void *data, unsigned sz);
};

struct fastboot_var {
	struct fastboot_var *next;
	const char *name;
	const char *value;
};

static struct fastboot_cmd *cmdlist;

void fastboot_register(const char *prefix,
		       void (*handle)(const char *arg, void *data, unsigned sz))
{
	struct fastboot_cmd *cmd;
	cmd = malloc(sizeof(*cmd));
	if (cmd) {
		cmd->prefix = prefix;
		cmd->prefix_len = strlen(prefix);
		cmd->handle = handle;
		cmd->next = cmdlist;
		cmdlist = cmd;
	}
}

static struct fastboot_var *varlist;

void fastboot_publish(const char *name, const char *value)
{
	struct fastboot_var *var;
	var = malloc(sizeof(*var));
	if (var) {
		var->name = name;
		var->value = value;
		var->next = varlist;
		varlist = var;
	}
}

#ifdef FASTBOOT_BUFFER_ADDR
#define FASTBOOT_BUFFER_BASE FASTBOOT_BUFFER_ADDR
#else
#define FASTBOOT_BUFFER_BASE 0x28000000
#endif

#ifdef FASTBOOT_BUFFER_MAX_SIZE
#define FASTBOOT_BUFFER_SIZE_MAX FASTBOOT_BUFFER_MAX_SIZE
#else
#define FASTBOOT_BUFFER_SIZE_MAX 100*1024*1024 // 100MB
#endif

#define MAX_RSP_SIZE 64
static void *download_base;
static unsigned download_max;
static unsigned download_size;
char max_download_size[MAX_RSP_SIZE];

#define STATE_OFFLINE	0
#define STATE_COMMAND	1
#define STATE_COMPLETE	2
#define STATE_ERROR	3

static unsigned fastboot_state = STATE_OFFLINE;

void fastboot_ack(const char *code, const char *reason)
{
	char response[MAX_RSP_SIZE];
	dprintf(INFO,"fastboot_ack(): code %s, reason %s\n", code, reason);

	if (fastboot_state != STATE_COMMAND) {
		dprintf(INFO,"fastboot_ack(): fastboot_state != STATE_COMMAND\n");
		return;
	}
	if (reason == 0)
		reason = "";

	snprintf(response, MAX_RSP_SIZE, "%s%s", code, reason);
	fastboot_state = STATE_COMPLETE;

	if (usb_write(response, strlen(response)) < 0) {
		dprintf(INFO,"fastboot_ack: usb_write fail!\n");
		fastboot_state = STATE_ERROR;
	}

}

void fastboot_info(const char *reason)
{
	char response[MAX_RSP_SIZE];
	const char *code = "INFO";

	dprintf(INFO,"fastboot_info(): code %s, reason %s\n", code, reason);

	if (reason == 0)
		reason = "";

	snprintf(response, MAX_RSP_SIZE, "%s%s", code, reason);

	if (usb_write(response, strlen(response)) < 0) {
		dprintf(INFO,"fastboot_ack: usb_write fail!\n");
	}
}

void fastboot_fail(const char *reason)
{
	fastboot_ack("FAIL", reason);
}

void fastboot_okay(const char *info)
{
	fastboot_ack("OKAY", info);
}

static void cmd_getvar(const char *arg, void *data, unsigned sz)
{
	struct fastboot_var *var;
	dprintf(INFO,"fastboot: cmd_getvar\n");

	for (var = varlist; var; var = var->next) {
		if (!strcmp(var->name, arg)) {
			fastboot_okay(var->value);
			return;
		}
	}
	fastboot_okay("");
}

static void cmd_download(const char *arg, void *data, unsigned sz)
{
	char response[MAX_RSP_SIZE];
	unsigned len = hex2unsigned(arg);
	int r;
	dprintf(INFO,"fastboot: cmd_download\n");

	download_size = 0;
	if (len > download_max) {
		fastboot_fail("data too large");
		return;
	}

	sprintf(response,"DATA%08x", len);
	if (usb_write(response, strlen(response)) < 0)
		return;

	memset(download_base, 0, download_max);

	dprintf(INFO,"%s: usb_read to download_base@%p, len=0x%x\n",
		    __func__, download_base, len);
	r = usb_read(download_base, len);
	if ((r < 0) || ((unsigned)r != len)) {
		fastboot_state = STATE_ERROR;
		dprintf(INFO,"%s():%d fastboot: fastboot_state STATE_ERROR\n",
			    __func__, __LINE__);
		return;
	}
	download_size = len;
	fastboot_okay("");
}

static void fastboot_command_loop(void)
{
	struct fastboot_cmd *cmd;
	int r;
	dprintf(INFO,"fastboot: processing commands\n");

	uint8_t *buffer = (uint8_t *)memalign(CACHE_LINE, ROUNDUP(4096, CACHE_LINE));
	if (!buffer) {
		dprintf(CRITICAL, "Could not allocate memory for fastboot buffer\n.");
		// TODO DEBUG_ASSERT(0);
	}

again:
	while (fastboot_state != STATE_ERROR) {
		r = usb_read(buffer, MAX_RSP_SIZE);
		if (r < 0) break;
		buffer[r] = 0;
		dprintf(INFO,"fastboot: %s\n", buffer);

		for (cmd = cmdlist; cmd; cmd = cmd->next) {
			if (memcmp(buffer, cmd->prefix, cmd->prefix_len))
				continue;
			fastboot_state = STATE_COMMAND;
			cmd->handle((const char*) buffer + cmd->prefix_len,
				    (void*) download_base, download_size);
			if (fastboot_state == STATE_COMMAND)
				fastboot_fail("unknown reason");
			goto again;
		}

		fastboot_fail("unknown command");
	}
	fastboot_state = STATE_OFFLINE;
	dprintf(INFO,"fastboot: oops!\n");
}

static int fastboot_handler(void *arg)
{
	for (;;) {
		dprintf(INFO, "%s, start wait\n", __func__);
		fastboot_usb_wait();
		fastboot_command_loop();
	}
	return 0;
}

int fastboot_init(void *base, unsigned size)
{
	thread_t *thr;
	int ret = 0;

	dprintf(INFO, "fastboot_init: base %p, size: %d\n", base, size);

	download_base = base;
	download_max = size;

	ret = fastboot_usb_init();
	if (ret) {
		return -1;
	}

	fastboot_register("getvar:", cmd_getvar);
	fastboot_register("download:", cmd_download);
	fastboot_publish("version", "0.5");
	/* Max download size supported */
	snprintf(max_download_size, MAX_RSP_SIZE, "\t0x%x",
			FASTBOOT_BUFFER_SIZE_MAX);
	fastboot_publish("max-download-size", (const char *) max_download_size);

	dprintf(INFO, "fastboot_init fastboot_cmd_init\n");
	fastboot_cmd_init();

	dprintf(INFO, "fastboot_init thread_create\n");
	thr = thread_create("fastboot", fastboot_handler, 0, DEFAULT_PRIORITY, 4096);
	thread_resume(thr);

	dprintf(INFO, "fastboot_init fastboot_usb_start\n");
	fastboot_usb_start();
	return 0;
}

int do_fastboot(int argc, const cmd_args *argv)
{

	dprintf(INFO, "do_fastboot...\n");
	boot_mode = BOOT_FASTBOOT_MODE;
	void *buffer = (void *)FASTBOOT_BUFFER_BASE;
	//void *buffer = malloc(FASTBOOT_BUFFER_SIZE_MAX);
	dprintf(INFO, "fastboot: buffer@%p "
		    "(FASTBOOT_BUFFER_BASE=0x%x, FASTBOOT_BUFFER_SIZE_MAX=0x%x)\n",
		    buffer, FASTBOOT_BUFFER_BASE, FASTBOOT_BUFFER_SIZE_MAX);
	memset(buffer, 0, FASTBOOT_BUFFER_SIZE_MAX);
	fastboot_init(buffer, FASTBOOT_BUFFER_SIZE_MAX);

	while (1) {};

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("fastboot", "fastboot command", (console_cmd)&do_fastboot)
STATIC_COMMAND_END(fastboot);


