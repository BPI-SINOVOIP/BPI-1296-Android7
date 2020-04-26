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

#ifndef __DEV_UDC_H
#define __DEV_UDC_H

#include <stdint.h>
#include <kernel/event.h>

typedef struct udc_request udc_request_t;
typedef struct udc_gadget udc_gadget_t;
typedef struct udc_device udc_device_t;
typedef struct udc_driver udc_driver_t;

/* endpoints are opaque handles specific to the particular device controller */
typedef struct udc_endpoint udc_endpoint_t;

/* USB Device Controller Transfer Request */
struct udc_request {
    void *buffer;
    unsigned length;
    void (*complete)(udc_request_t *req, unsigned actual, int status);
    void *context;
};

udc_request_t *udc_request_alloc(void);
void udc_request_free(udc_request_t *req);
int udc_request_queue(udc_endpoint_t *ept, udc_request_t *req);
int udc_request_cancel(udc_endpoint_t *ept, udc_request_t *req);

#define UDC_BULK_IN    0x82
#define UDC_BULK_OUT   0x02

udc_endpoint_t *udc_endpoint_alloc(unsigned type, unsigned maxpkt);
void udc_endpoint_free(udc_endpoint_t *ept);

#define UDC_EVENT_ONLINE    1
#define UDC_EVENT_OFFLINE   2

struct udc_gadget {
    void (*notify)(udc_gadget_t *gadget, unsigned event);
    void *context;

    struct udc_gadget *next; // do not modify

    uint8_t ifc_class;
    uint8_t ifc_subclass;
    uint8_t ifc_protocol;
    uint8_t ifc_endpoints;
    const char *ifc_string;
    unsigned flags;

    udc_endpoint_t **ept;

    /* */
    event_t txn_done;
    unsigned char buffer[4096];
    struct udc_request *req;
    int txn_status;
    char *name;

    int (*usb_read)(udc_gadget_t *gadget, void *buf, unsigned len);
    int (*usb_write)(udc_gadget_t *gadget, void *buf, unsigned len);
};

struct udc_device {
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t version_id;

    const char *manufacturer;
    const char *product;
    const char *serialno;
};

struct udc_driver {
	const char *name;
	int (*udc_init)(struct udc_device* dev_info);
	int (*udc_register_gadget) (struct udc_gadget *gadget);
	int (*udc_start)(void);
	int (*udc_stop)(void);

	struct udc_endpoint *(*udc_endpoint_alloc)(unsigned type, unsigned maxpkt);
	void (*udc_endpoint_free)(struct udc_endpoint *ept);

	struct udc_request *(*udc_request_alloc)(void);
	void (*udc_request_free)(struct udc_request *req);
	int (*udc_request_queue)(udc_endpoint_t *ept, udc_request_t *req);
	int (*udc_request_cancel)(udc_endpoint_t *ept, udc_request_t *req);

	enum handler_return (*udc_irq)(void *arg);
};

int udc_init(udc_device_t *devinfo);
int udc_register_gadget(udc_gadget_t *gadget);
int udc_start(void);
int udc_stop(void);

int gadget_init(struct udc_gadget *gadget);

// calls this to build descriptors from device and gadgets
void udc_create_descriptors(udc_device_t *device, udc_gadget_t *gadget);

#endif
