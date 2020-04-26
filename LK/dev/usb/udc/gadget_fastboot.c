
#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/thread.h>
#include <kernel/event.h>
#include <dev/udc.h>

#include "udc-utils.h"
#include "udc-core.h"

#undef log_level
#define log_level 3

static void req_complete(struct udc_request *req, unsigned actual, int status)
{
	ENTER();
	udc_gadget_t *gadget = (udc_gadget_t *)req->context;
	gadget->txn_status = status;
	req->length = actual;
	DBG("To send event_signal for txn_done to gadget %p\n", gadget);
	event_signal(&gadget->txn_done, true);
	EXIT();
}

static int usb_read(udc_gadget_t *gadget, void *_buf, unsigned len)
{
	ENTER();
	udc_request_t *req = gadget->req;
	udc_endpoint_t *out = gadget->ept[1];
	int r;
	unsigned xfer;
	unsigned char *buf = _buf;
	int count = 0;

	INF("usb_read: ep%d(%s) %p\n", out->num, out->in?"in":"out", out);

	while (len > 0) {
		xfer = (len > 4096) ? 4096 : len;
		req->buffer = buf;
		req->length = xfer;
		req->complete = req_complete;
		r = udc_request_queue(out, req);
		if (r < 0) {
			ERR("Error! queue failed\n");
			goto oops;
		}
		DBG("Wait txn_done\n");
		event_wait(&gadget->txn_done);
		DBG("Get txn_done\n");

		if (gadget->txn_status < 0) {
			ERR("Error! transaction failed\n");
			goto oops;
		}

		count += req->length;
		buf += req->length;
		len -= req->length;

		DBG("Get data count %d (remain %d, next buf@%p)\n",  count, len, buf);
		/* short transfer? */
		if (req->length != xfer) break;
	}
	INF("Get data count %d (remain %d, End buf@%p)\n",  count, len, buf);

	EXIT();
	return count;
oops:
	EXIT();
	return -1;
}

static int usb_write(udc_gadget_t *gadget, void *buf, unsigned len)
{
	ENTER();
	udc_request_t *req = gadget->req;
	udc_endpoint_t *in = gadget->ept[0];
	int r;

	INF("usb_write: ep%d(%s) %p\n", in->num, in->in?"in":"out", in);

	req->buffer = buf;
	req->length = len;
	req->complete = req_complete;
	r = udc_request_queue(in, req);
	if (r < 0) {
		ERR("Error! queue failed\n");
		goto oops;
	}
	event_wait(&gadget->txn_done);
	if (gadget->txn_status < 0) {
		ERR("Error! transaction failed\n");
		goto oops;
	}
	EXIT();
	return req->length;

oops:
	EXIT();
	return -1;
}

int gadget_start(struct udc_gadget *gadget)
{
	ENTER();

	gadget->ept[0] = udc_endpoint_alloc(UDC_BULK_IN, 512);
	if (!gadget->ept[0])
		goto fail_alloc_in;
	gadget->ept[1] = udc_endpoint_alloc(UDC_BULK_OUT, 512);
	if (!gadget->ept[1])
		goto fail_alloc_out;

	DBG("gadget ept[0]=[ep%d(%s) %p] , gadget ept[1]=[ep%d(%s) %p]\n",
			gadget->ept[0]->num, gadget->ept[0]->in?"in":"out", gadget->ept[0],
			gadget->ept[1]->num, gadget->ept[1]->in?"in":"out", gadget->ept[1]);

	gadget->req = udc_request_alloc();
	gadget->req->context = gadget;
	if (!gadget->req)
		goto fail_alloc_req;

	EXIT();
	return 0;

fail_alloc_req:
fail_alloc_out:
fail_alloc_in:
	return -1;
}

int gadget_stop(struct udc_gadget *gadget)
{
	ENTER();

	if (gadget->req)
		udc_request_cancel(gadget->ept[0], gadget->req);

	if (gadget->req)
		udc_request_cancel(gadget->ept[1], gadget->req);

	EXIT();
	return 0;
}

udc_endpoint_t g_default_ep_in = {
	.maxpkt = 512,
	.num = 1,
	.in = 1,
};

udc_endpoint_t g_default_ep_out = {
	.maxpkt = 512,
	.num = 2,
	.in = 0,
};

int gadget_init(struct udc_gadget *gadget)
{
	ENTER();
	gadget->usb_read = &usb_read;
	gadget->usb_write = &usb_write;

	event_init(&gadget->txn_done, 0, EVENT_FLAG_AUTOUNSIGNAL);

	gadget->ept = malloc((2) * sizeof(udc_endpoint_t *));

	gadget->ept[0] = &g_default_ep_in;
	gadget->ept[1] = &g_default_ep_out;

	if (udc_register_gadget(gadget))
		goto fail_udc_register;

	EXIT();
	return 0;

fail_udc_register:

	return -1;
}
