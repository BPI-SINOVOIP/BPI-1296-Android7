
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include <platform/usb.h>

#include "udc-utils.h"

#undef log_level
#define log_level 3

static struct rtk_usb *rtk_usb = NULL;

extern void rtk_otg_init(u32 reg_base, struct usb2_phy *usb2phy);
extern void rtk_otg_stop(void);

extern void rtk_dwc3_init(u32 reg_base, struct usb2_phy *usb2phy,
	    struct usb3_phy *usb3phy);
extern void rtk_dwc3_stop(void);

void udc_usb_dev_init(void) {
	ENTER();
	int count = 0;
	int i;

	rtk_usb_clock_init();

	count = rtk_usb_device_init(&rtk_usb);

	DBG("rtk_usb %p device count %d\n", rtk_usb, count);
	for (i = 0; i < count; i++) {
		struct rtk_usb *rtkusb = &rtk_usb[i];
		DBG("%d rtk_usb %p\n", i, rtkusb);
		if (rtkusb->class_type == OTG_EHCI) {
			DBG("call rtk_otg_init\n");
			rtk_otg_init(rtkusb->reg_base, rtkusb->usb2phy);
		} else if (rtkusb->class_type == DWC3_XHCI) {
			DBG("call rtk_dwc3_init\n");
			rtk_dwc3_init(rtkusb->reg_base, rtkusb->usb2phy, rtkusb->usb3phy);
		}
	}
	EXIT();
}

void udc_usb_dev_stop(void) {
	ENTER();
	struct rtk_usb *rtkusb = &rtk_usb[0];
	int count = rtkusb->ctrl_count;
	int i;

	for (i = 0; i < count; i++) {
		struct rtk_usb *rtkusb = &rtk_usb[i];
		if (rtkusb->class_type == OTG_EHCI) {
			rtk_otg_stop();
		} else if (rtkusb->class_type == DWC3_XHCI) {
			rtk_dwc3_stop();
		}
	}

	rtk_usb_device_stop(&rtk_usb);

	rtk_usb_clock_stop();

	EXIT();
}
