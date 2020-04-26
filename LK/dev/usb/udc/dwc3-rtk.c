
#include <stdlib.h>
#include <dev/udc.h>
#include <dev/usb-phy.h>

#include "udc-core.h"
#include "udc-common.h"
#include "udc-utils.h"

#include "dwc3.h"

#undef log_level
#define log_level 3

#define F_LL_INIT   1
#define F_UDC_INIT  2
#define EPT_TX(n) (1 << ((n) + 16))
#define EPT_RX(n) (1 << (n))

typedef struct usb_dev {
	u32 reg_base;
	spin_lock_t lock;

	unsigned char num_of_eps;
    uint32_t ept_alloc_table;

	uint8_t flags;

	struct dwc3_core dwc3;

	udc_device_t *device;
	udc_gadget_t *gadget;
	udc_driver_t *driver;
} usb_t;

#define dwc3_get_usb_t(dwc3) 			\
	((usb_t *)((char *)(dwc3) - 	\
		offsetof(struct usb_dev, dwc3)))

static usb_t rtk_dwc3;

// ---- endpoint management
static udc_endpoint_t *rtk_dwc3_udc_endpoint_alloc(unsigned type, unsigned maxpkt)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	dwc3_core_t *dwc3 = &usb->dwc3;
	dwc3_device_t *dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t *dev_ep;
	udc_endpoint_t *ept;
	unsigned n;
	unsigned in = !!(type & 0x80);

	if (!(usb->flags & F_UDC_INIT)) {
		panic("udc_init() must be called before udc_endpoint_alloc()\n");
	}

	usb->num_of_eps = dwc3_dev->num_out_eps + dwc3_dev->num_in_eps;

	DBG("alloc endpoint type=%x (%s) maxpkt=%d\n",
			type, in?"in":"out", maxpkt);

	for (n = 1; n < usb->num_of_eps; n++) {
		unsigned bit = in ? EPT_TX(n) : EPT_RX(n);
		if (usb->ept_alloc_table & (EPT_RX(n) | EPT_TX(n))) {
			continue;
		}
		if (in)
			dev_ep = dwc3_dev->in_ep[n - 1];
		else
			dev_ep = dwc3_dev->out_ep[n - 1];

		if (dev_ep) {
			usb->ept_alloc_table |= bit;
		}
		ept = &dev_ep->usb_ep;

		DBG("[ept%d(%s) @%p] dev_ep@%p ept_alloc_table = 0x%x, bit=%x, n=%d, in=%s\n",
				ept->num, ept->in?"in":"out", ept, dev_ep,
				usb->ept_alloc_table, bit, n, in?"in":"out");

		EXIT();
		return ept;
	}
	EXIT();
	return NULL;
}

static void rtk_dwc3_udc_endpoint_free(struct udc_endpoint *ept)
{
	ENTER();
	// TODO
	EXIT();
}

// ---- request management
static udc_request_t *rtk_dwc3_udc_request_alloc(void)
{
	ENTER();
	udc_request_t *req = dwc3_alloc_request(NULL, NULL);
	DBG("alloc request %p\n", req);
	EXIT();
    return req;
}

static void rtk_dwc3_udc_request_free(struct udc_request *req)
{
	ENTER();
	dwc3_free_request(NULL, NULL, req);
	EXIT();
}

static int rtk_dwc3_udc_request_queue(udc_endpoint_t *ept, struct udc_request *_req)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	int ret = 0;

	DBG("[ep%d(%s) %p] queue req %p\n", ept->num, ept->in?"in":"out", ept, _req);

	ret = dwc3_ep_queue(&usb->dwc3, ept, _req);

	EXIT();
	return ret;
}

static int rtk_dwc3_udc_request_dequeue(udc_endpoint_t *ept, struct udc_request *_req)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	int ret = 0;

	DBG("[ep%d(%s) %p] dequeue req %p\n", ept->num, ept->in?"in":"out", ept, _req);

	ret = dwc3_ep_dequeue(&usb->dwc3, ept, _req);

	EXIT();
	return ret;
}

int usb_gadget_connect(dwc3_core_t *dwc3)
{
	ENTER();
	usb_t *usb = dwc3_get_usb_t(dwc3);
	gadget_connect(usb->driver, usb->gadget);
	EXIT();
	return 0;
}

int usb_gadget_disconnect(dwc3_core_t *dwc3)
{
	ENTER();
	DBG("%s()\n", __func__);
	usb_t *usb = dwc3_get_usb_t(dwc3);
	gadget_disconnect(usb->driver, usb->gadget);
	usb->ept_alloc_table = 0;
	EXIT();
	return 0;
}

static enum handler_return rtk_dwc3_udc_irq(void *arg)
{
	usb_t *usb = &rtk_dwc3;
	dwc3_core_t *dwc3 = &usb->dwc3;
	spin_lock_saved_state_t state;
	int ret;
	spin_lock_irqsave(&usb->lock, state);

	ret = dwc3_irq(dwc3, 53);

	spin_unlock_irqrestore(&usb->lock, state);

	if (ret == 0)
		return INT_NO_RESCHEDULE;
	else
		return INT_RESCHEDULE;
}

#define RTK_REG_BASE_OFFSET 0x8000

/* udc_driver's udc_init callback*/
int rtk_dwc3_udc_init(struct udc_device *dev)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	int ret = 0;

	usb->dwc3.base = usb->reg_base + RTK_REG_BASE_OFFSET;
	DBG("start low level HW init (rtk reg_base 0x%x, "
		    "RTK_REG_BASE_OFFSET 0x%x)\n", usb->reg_base, RTK_REG_BASE_OFFSET);
	dwc3_core_init(&usb->dwc3);

	usb->dwc3.cmn_initialized = 1;

	usb->flags |= F_LL_INIT; //finish low level init

	ret = dwc3_gadget_init(&usb->dwc3);
	if (ret) {
		ERR("Error! dwc3 gadget initialization failed!\n");
		//goto fail;
	}
	usb->dwc3.gadget_initialized = 1;

	dwc3_device_init(&usb->dwc3);

	usb->dwc3.pcd_initialized = 1;

	usb->device = dev;
	usb->flags |= F_UDC_INIT; //finish udc init

	EXIT();
    return 0;
}

int rtk_dwc3_udc_register_gadget(struct udc_gadget *gadget) {
	ENTER();
	usb_t *usb = &rtk_dwc3;
	usb->gadget = gadget;

	DBG("gadget %p\n", gadget);
	EXIT();
	return 0;
}

int rtk_dwc3_udc_start(void)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	dwc3_core_t *dwc3 = &usb->dwc3;
	int ret = 0;

	if (!(usb->flags & F_LL_INIT)) {
		panic("udc cannot start before hw init\n");
	}
	if (!usb->device) {
		panic("udc cannot start before init\n");
	}
	if (!usb->gadget) {
		panic("udc has no gadget registered\n");
	}

	dwc3_core_device_start(&usb->dwc3, 0);

	dwc3->cmn_irq_installed = 1;

	EXIT();
	return ret;
}

int rtk_dwc3_udc_stop(void)
{
	ENTER();
	usb_t *usb = &rtk_dwc3;
	dwc3_core_t *dwc3 = &usb->dwc3;
	dwc3_device_stop(&dwc3->dwc3_dev);
	dwc3_core_device_remove(&usb->dwc3);

	EXIT();
	return 0;
}

/* Add udc_driver to udc_core */
static struct udc_driver rtk_dwc3_udc_driver = {
	.name = "rtk-dwc3",
	.udc_init  = rtk_dwc3_udc_init,
	.udc_register_gadget = rtk_dwc3_udc_register_gadget,
    .udc_start = rtk_dwc3_udc_start,
	.udc_stop  = rtk_dwc3_udc_stop,

	.udc_endpoint_alloc = rtk_dwc3_udc_endpoint_alloc,
	.udc_endpoint_free  = rtk_dwc3_udc_endpoint_free,

	.udc_request_alloc  = rtk_dwc3_udc_request_alloc,
	.udc_request_free  = rtk_dwc3_udc_request_free,

	.udc_request_queue  = rtk_dwc3_udc_request_queue,
	.udc_request_cancel  = rtk_dwc3_udc_request_dequeue,

	.udc_irq = rtk_dwc3_udc_irq

};

void rtk_dwc3_init(u32 reg_base, struct usb2_phy *usb2phy,
	    struct usb3_phy *usb3phy) {
	ENTER();
	usb_t *usb = &rtk_dwc3;

	usb->reg_base = reg_base;

	usb->dwc3.usb2phy = usb2phy;
	usb->dwc3.usb3phy = usb3phy;

	usb2_phy_switch_to_device(usb2phy);

	usb->driver = &rtk_dwc3_udc_driver;

	udc_core_add_udc_driver(usb->driver);
	EXIT();
}

void rtk_dwc3_stop(void) {
	ENTER();

	EXIT();
}

