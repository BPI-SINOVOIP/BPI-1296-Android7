
#ifndef __DEV_UDC_CORE_H
#define __DEV_UDC_CORE_H

struct udc_device;
struct udc_gadget;

struct udc_endpoint;
struct udc_request;
struct udc_driver;

typedef struct udc_if {
	char *name;
	struct udc_gadget *gadget;
	struct udc_device *device;
	struct udc_driver *driver;

	struct udc_desc *desc_list;

	struct udc_if *next;

	bool is_online;
} udc_if_t;

struct udc_endpoint {
	udc_if_t *udc;

	udc_request_t *req;
	udc_request_t *last;

	uint32_t bit;
	uint16_t maxpkt;
	uint8_t num;
	uint8_t in;
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

enum usb_speed {
	USB_SPEED_UNKNOWN = 0, 			/* enumerating */
	USB_SPEED_LOW, USB_SPEED_FULL, 	/* usb 1.1 */
	USB_SPEED_HIGH, 				/* usb 2.0 */
	USB_SPEED_WIRELESS, 			/* wireless (usb 2.5) */
	USB_SPEED_SUPER, 				/* usb 3.0 */
	USB_SPEED_SUPER_PLUS,
};

static const char *const speed_names[] = {
	[USB_SPEED_UNKNOWN] = "UNKNOWN",
	[USB_SPEED_LOW] = "low-speed",
	[USB_SPEED_FULL] = "full-speed",
	[USB_SPEED_HIGH] = "high-speed",
	[USB_SPEED_WIRELESS] = "wireless",
	[USB_SPEED_SUPER] = "super-speed",
	[USB_SPEED_SUPER_PLUS] = "super-speed plus",
};

static inline const char *usb_speed_string(enum usb_speed speed)
{
	if (speed < 0 || speed >= ARRAY_SIZE(speed_names))
		speed = USB_SPEED_UNKNOWN;
	return speed_names[speed];
}

int udc_core_add_udc_driver(struct udc_driver *driver);

int gadget_connect(struct udc_driver *driver, udc_gadget_t *gadget);

int gadget_disconnect(struct udc_driver *driver, udc_gadget_t *gadget);

int gadget_start(udc_gadget_t *gadget);

int gadget_stop(udc_gadget_t *gadget);

/* define by vender to init usb */
void udc_usb_dev_init(void);

void udc_usb_dev_stop(void);

#endif //__DEV_UDC_CORE_H
