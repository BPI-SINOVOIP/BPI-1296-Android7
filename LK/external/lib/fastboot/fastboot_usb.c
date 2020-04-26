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

struct fastboot_usb_t {
	event_t online;
	struct udc_gadget *gadget;
};

static struct fastboot_usb_t fastboot_usb;

int usb_read(void *_buf, unsigned len)
{
	if (fastboot_usb.gadget != NULL) {
		return fastboot_usb.gadget->usb_read(fastboot_usb.gadget, _buf, len);
	}
	dprintf(CRITICAL, "%s(), error gadget %p usb_read\n", __func__, fastboot_usb.gadget);
	return -1;
}

int usb_write(void *buf, unsigned len)
{
	if (fastboot_usb.gadget != NULL) {
		return fastboot_usb.gadget->usb_write(fastboot_usb.gadget, buf, len);
	}
	dprintf(CRITICAL, "%s(), error gadget %p usb_write\n", __func__, fastboot_usb.gadget);
	return -1;
}

int fastboot_usb_wait(void)
{
	dprintf(INFO, "%s() wait online signal\n", __func__);
	event_wait(&fastboot_usb.online);
	dprintf(INFO, "%s() get online signal\n", __func__);
	return 0;
}

static void fastboot_notify(struct udc_gadget *gadget, unsigned event)
{
	if (event == UDC_EVENT_ONLINE) {
		if (fastboot_usb.gadget == NULL) {
			dprintf(INFO, "%s() gadget %p event_signal --> online\n", __func__, gadget);
			fastboot_usb.gadget = gadget;
			event_signal(&fastboot_usb.online, true);
		} else if (fastboot_usb.gadget == gadget) {
			dprintf(INFO, "%s() gadget %p is online\n", __func__, gadget);
		} else {
			dprintf(CRITICAL, "%s() error gadget %p, fastboot_usb.gadget = %p\n", __func__,
				gadget, fastboot_usb.gadget);
		}
	} else if (event == UDC_EVENT_OFFLINE) {
		dprintf(INFO, "%s() gadget %p event_unsignal --> offline\n", __func__, gadget);
		fastboot_usb.gadget = NULL;
		event_unsignal(&fastboot_usb.online);
	} else
		dprintf(CRITICAL, "%s() error event %d\n", __func__, event);
}

static struct udc_device fastboot_udc_device = {
	.vendor_id    = 0x18d1,
	.product_id   = 0x4e40,
	.version_id   = 0x0100,
	.manufacturer = "Realtek",
	.product      = "Fastboot",
};

static struct udc_gadget fastboot_udc_gadget = {
	.notify		= fastboot_notify,
	.ifc_class	= 0xff,
	.ifc_subclass	= 0x42,
	.ifc_protocol	= 0x03,
	.ifc_endpoints	= 2,
	.ifc_string	= "fastboot",
};

int fastboot_usb_init(void)
{
	dprintf(INFO, "fastboot_usb_init()\n");

	event_init(&fastboot_usb.online, 0, EVENT_FLAG_AUTOUNSIGNAL);

    dprintf(INFO, "%s() --> udc_init()\n", __func__);
	udc_init(&fastboot_udc_device);

    dprintf(INFO, "%s() --> gadget_init()\n", __func__);
	gadget_init(&fastboot_udc_gadget);

	udc_create_descriptors(&fastboot_udc_device, &fastboot_udc_gadget);

	return 0;
}

int fastboot_usb_start(void) {
	dprintf(INFO, "%s() --> udc_start()\n", __func__);
	udc_start();
	return 0;
}

int fastboot_usb_stop(void) {
	dprintf(INFO, "%s() --> udc_stop()\n", __func__);
	udc_stop();

	return 0;
}
