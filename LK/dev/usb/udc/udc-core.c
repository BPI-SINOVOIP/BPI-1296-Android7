
#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include <assert.h>
#include <debug.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <kernel/event.h>
#include <platform/interrupts.h>

#include <dev/udc.h>

#include "udc-core.h"
#include "udc-utils.h"

#undef log_level
#define log_level 3

#define INT_USB_NUM 53

#define MAX_UDC 2

static udc_if_t usb_udc[MAX_UDC];

int gadget_disconnect(struct udc_driver *driver, udc_gadget_t *gadget);

static bool __set_offline_usb(struct udc_driver* driver) {
	int i = 0;
	udc_if_t *udc = NULL;
	for (i = 0; i < MAX_UDC; i++) {
		udc_if_t *_udc = &usb_udc[i];
		if (driver == _udc->driver) {
			udc = _udc;
		}
	}

	if (udc == NULL) {
		ERR("udc is NULL\n");
		return false;
	}

	DBG("set udc %s to offline\n", udc->name);
	udc->is_online = false;

	return true;
}

static bool __set_online_usb(struct udc_driver *driver) {
	int i = 0;
	udc_if_t *udc = NULL;
	for (i = 0; i < MAX_UDC; i++) {
		udc_if_t *_udc = &usb_udc[i];
		if (driver != _udc->driver && _udc->is_online) {
			DBG("udc %s driver %p is online, set driver to offline\n",
					_udc->name, _udc->driver);
			gadget_disconnect(_udc->driver, _udc->gadget);
			__set_offline_usb(driver);
		} else if (driver == _udc->driver) {
			udc = _udc;
		}
	}

	if (udc == NULL) {
		ERR("udc is NULL\n");
		return false;
	} else if (udc->driver == NULL) {
		ERR("driver is NULL\n");
		return false;
	}

	INF("set udc %s to online\n", udc->name);
	udc->is_online = true;
	return true;
}

static udc_if_t *__get_online_usb(void) {
	udc_if_t *_udc = NULL;
	struct udc_driver *driver = NULL;
	int i = 0;
	for (i = 0; i < 2; i++) {
		udc_if_t *udc = &usb_udc[i];
		driver = udc->driver;
		if (driver && udc->is_online) {
			DBG("udc %s driver %p is online\n", udc->name, driver);
			_udc = udc;
			break;
		}
	}

	return _udc;
}

udc_endpoint_t *udc_endpoint_alloc(unsigned type, unsigned maxpkt)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;
	udc_endpoint_t *ept = NULL;

	if (driver)
		ept = driver->udc_endpoint_alloc(type, maxpkt);
	else
		ERR("driver is NULL\n");

	if (ept)
		ept->udc = udc;

	DBG("udc %s driver %p alloc ept %p\n", udc?udc->name:"NULL", driver, ept);
	EXIT();
	return ept;
}

void udc_endpoint_free(struct udc_endpoint *ept)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;

	DBG("udc %s driver %p free ept %p\n", udc?udc->name:"NULL", driver, ept);
	if (driver)
		driver->udc_endpoint_free(ept);
	else
		ERR("driver is NULL\n");

	EXIT();
}

// ---- request management
udc_request_t *udc_request_alloc(void)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;
	udc_request_t *req = NULL;
	if (driver)
		req = driver->udc_request_alloc();
	else
		ERR("driver is NULL\n");

	DBG("udc %s driver %p alloc req %p\n", udc?udc->name:"NULL", driver, req);
	EXIT();
	return req;
}

void udc_request_free(struct udc_request *req)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;

	DBG("udc %s driver %p free req %p\n", udc?udc->name:"NULL", driver, req);
	if (driver)
		driver->udc_request_free(req);
	else
		ERR("driver is NULL\n");

	EXIT();
}

int udc_request_queue(udc_endpoint_t *ept, struct udc_request *req)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;
	int ret = 0;

	DBG("udc %s driver %p req %p queue to ept %p\n",
			udc?udc->name:"NULL", driver, req, ept);

	if (driver)
		ret = driver->udc_request_queue(ept, req);
	else
		ERR("driver is NULL\n");

	EXIT();
    return ret;
}

int udc_request_cancel(udc_endpoint_t *ept, struct udc_request *req)
{
	ENTER();
	udc_if_t *udc = __get_online_usb();
	struct udc_driver *driver = udc? udc->driver:NULL;
	int ret = 0;

	DBG("udc %s driver %p req %p cancel from ept %p\n",
			udc?udc->name:"NULL", driver, req, ept);

	if (driver)
		ret = driver->udc_request_cancel(ept, req);
	else
		ERR("driver is NULL\n");

	EXIT();
    return ret;
}

enum handler_return udc_interrupt(void *arg)
{
	ENTER();
	int i;
	struct udc_driver *driver = NULL;
	enum handler_return ret = 0;
	for (i = 0; i < 2; i++) {
		driver = usb_udc[i].driver;
		if (driver) {
			ret |= driver->udc_irq(arg);
		}
	}
	EXIT();
	return ret;
}

int udc_init(struct udc_device *dev)
{
	ENTER();
	udc_if_t *udc;
	struct udc_driver *driver = NULL;
	int i, ret = 0;

	udc_usb_dev_init();

	for (i = 0; i < 2; i++) {
		udc = &usb_udc[i];
		driver = udc->driver;
		if (driver) {
			INF("#%d call driver %p udc_init\n", i, driver);
			ret = driver->udc_init(dev);
		}
	}

	EXIT();
	return ret;
}

int udc_start(void)
{
	ENTER();
	udc_if_t *udc;
	struct udc_driver *driver = NULL;
	int i, ret = 0;

	for (i = 0; i < 2; i++) {
		udc = &usb_udc[i];
		driver = udc->driver;
		if (driver) {
			INF("#%d call driver %p udc_start\n", i, driver);
			ret = driver->udc_start();
		}
	}

	DBG("%s(): register_int_handler INT_USB_NUM=%d\n", __func__, INT_USB_NUM);
	register_int_handler(INT_USB_NUM, udc_interrupt, (void *)0);
	unmask_interrupt(INT_USB_NUM);

	EXIT();
	return ret;
}

int udc_stop(void)
{
	ENTER();
	struct udc_driver *driver = NULL;
	int i, ret = 0;

	for (i = 0; i < 2; i++) {
		driver = usb_udc[i].driver;
		if (driver) {
			INF("#%d call driver %p udc_stop\n", i, driver);
			ret = driver->udc_stop();
		}
	}
	mask_interrupt(INT_USB_NUM);

	udc_usb_dev_stop();

	EXIT();
    return ret;
}

// ---- UDC API
int udc_core_add_udc_driver(struct udc_driver *driver) {
	ENTER();
	int i;
	//udc_if_t *udc
	for (i = 0; i < 2; i++) {
		if (usb_udc[i].driver == NULL) {
			INF("#%d add udc %s driver %p\n", i, driver->name, driver);
			usb_udc[i].driver = driver;
			usb_udc[i].name = (char *)driver->name;
			break;
		}
	}

	EXIT();
	return 0;
}

int udc_register_gadget(udc_gadget_t *gadget)
{
	ENTER();
	struct udc_if *udc;
	int i;
	for (i = 0; i < 2; i++) {
		udc = &usb_udc[i];
		//if (!strcmp(udc->name, gadget->name))
		//	break;
		if (udc->driver == NULL)
			continue;

		if (udc->gadget) {
			udc_gadget_t *last = udc->gadget;
			while (last->next) {
				last = last->next;
			}
			last->next = gadget;
		} else {
			udc->gadget = gadget;
			udc->driver->udc_register_gadget(gadget);
		}
		gadget->next = NULL;
	}
	EXIT();
	return 0;
}

/* ---------------------------------- */
/* For gadget */
static void notify_gadgets(udc_gadget_t *gadget, unsigned event)
{
	ENTER();
	DBG("notify_gadgets gadget %p\n", gadget);
	while (gadget) {
		if (gadget->notify) {
			INF("gadget %p notify event %d\n",gadget, event);
			gadget->notify(gadget, event);
		}
		gadget = gadget->next;
	}
	EXIT();
}

int gadget_connect(struct udc_driver *driver, udc_gadget_t *gadget)
{
	DBG("%s()\n", __func__);

	__set_online_usb(driver);

	gadget_start(gadget);

	notify_gadgets(gadget, UDC_EVENT_ONLINE);
	return 0;
}

int gadget_disconnect(struct udc_driver *driver, udc_gadget_t *gadget)
{
	DBG("%s()\n", __func__);
	notify_gadgets(gadget, UDC_EVENT_OFFLINE);

	gadget_stop(gadget);

	return 0;
}
