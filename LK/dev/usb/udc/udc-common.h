/*
 * Copyright (c) 2015 Brian Swetland
 * Copyright (c) 2008 Google, Inc.
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

#ifndef __PRIVATE_UDC_H__
#define __PRIVATE_UDC_H__

#define GET_STATUS           0
#define CLEAR_FEATURE        1
#define SET_FEATURE          3
#define SET_ADDRESS          5
#define GET_DESCRIPTOR       6
#define SET_DESCRIPTOR       7
#define GET_CONFIGURATION    8
#define SET_CONFIGURATION    9
#define GET_INTERFACE        10
#define SET_INTERFACE        11
#define SYNCH_FRAME          12
#define SET_SEL              48

#define TYPE_DEVICE          1
#define TYPE_CONFIGURATION   2
#define TYPE_STRING          3
#define TYPE_INTERFACE       4
#define TYPE_ENDPOINT        5
#define TYPE_BOS             15
#define TYPE_DEVICE_CAP      16
#define TYPE_SS_EP_COMP      48

#define DEVICE_READ          0x80
#define DEVICE_WRITE         0x00
#define INTERFACE_READ       0x81
#define INTERFACE_WRITE      0x01
#define ENDPOINT_READ        0x82
#define ENDPOINT_WRITE       0x02

typedef struct udc_descriptor udc_descriptor_t;

union setup_packet {
	struct {
		uint8_t type;
		uint8_t request;
		uint16_t value;
		uint16_t index;
		uint16_t length;
	};
	struct {
		uint32_t w0;
		uint32_t w1;
	};
} __attribute__ ((packed));

struct udc_descriptor {
	udc_descriptor_t *next;
	uint16_t tag; /* ((TYPE << 8) | NUM) */
	uint16_t len; /* total length */
	uint8_t data[4];
};

// driver uses this to obtain descriptors
udc_descriptor_t *udc_descriptor_find(unsigned tag);

// driver provides this
void udc_ept_desc_fill(udc_endpoint_t *ept, unsigned char *data);

/**
 * struct usb_ctrlrequest - SETUP data for a USB device control request
 * @bRequestType: matches the USB bmRequestType field
 * @bRequest: matches the USB bRequest field
 * @wValue: matches the USB wValue field (le16 byte order)
 * @wIndex: matches the USB wIndex field (le16 byte order)
 * @wLength: matches the USB wLength field (le16 byte order)
 *
 * This structure is used to send control requests to a USB device.  It matches
 * the different fields of the USB 2.0 Spec section 9.3, table 9-2.  See the
 * USB spec for a fuller description of the different fields, and what they are
 * used for.
 *
 * Note that the driver for any interface can issue control requests.
 * For most devices, interfaces don't coordinate with each other, so
 * such requests may be made at any time.
 */
struct usb_ctrlrequest {
	u8 bRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed));

static inline struct usb_ctrlrequest buffer_to_ctrl(unsigned char *buffer) {
	struct usb_ctrlrequest ctrl;
	ctrl.bRequestType = buffer[0];
	ctrl.bRequest = buffer[1];
	ctrl.wValue = buffer[2] << 8 | buffer[3];
	ctrl.wIndex = buffer[4] << 8 | buffer[5];
	ctrl.wLength = buffer[6] << 8| buffer[7];
	return ctrl;
}

/*
 * USB directions
 *
 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
 * It's also one of three fields in control requests bRequestType.
 */
#define USB_DIR_OUT 		0x0 	/* to device */
#define USB_DIR_IN 			0x80 	/* to host */

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK 			(0x03 << 5)
#define USB_TYPE_STANDARD 		(0x00 << 5)
#define USB_TYPE_CLASS 			(0x01 << 5)
#define USB_TYPE_VENDOR 		(0x02 << 5)
#define USB_TYPE_RESERVED 		(0x03 << 5)

/*
 * USB recipients, the third of three bRequestType fields
 */
#define USB_RECIP_MASK 			0x1f
#define USB_RECIP_DEVICE 		0x00
#define USB_RECIP_INTERFACE 	0x01
#define USB_RECIP_ENDPOINT 		0x02
#define USB_RECIP_OTHER 		0x03


/*
 * Standard requests, for the bRequest field of a SETUP packet.
 *
 * These are qualified by the bRequestType field, so that for example
 * TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
 * by a GET_STATUS request.
 */
#define USB_REQ_GET_STATUS 			0x00
#define USB_REQ_CLEAR_FEATURE 		0x01
#define USB_REQ_SET_FEATURE 		0x03
#define USB_REQ_SET_ADDRESS 		0x05
#define USB_REQ_GET_DESCRIPTOR 		0x06
#define USB_REQ_SET_DESCRIPTOR 		0x07
#define USB_REQ_GET_CONFIGURATION 	0x08
#define USB_REQ_SET_CONFIGURATION 	0x09
#define USB_REQ_GET_INTERFACE 		0x0A
#define USB_REQ_SET_INTERFACE 		0x0B
#define USB_REQ_SYNCH_FRAME 		0x0C
#define USB_REQ_SET_SEL 			0x30
#define USB_REQ_SET_ISOCH_DELAY 	0x31

#define USB_ENDPOINT_HALT 			0 /* IN/OUT will STALL */

/* USB_DT_CONFIG: Configuration descriptor information.
 *  *
 *   * USB_DT_OTHER_SPEED_CONFIG is the same descriptor, except that the
 *    * descriptor type is different.  Highspeed-capable devices can look
 *     * different depending on what speed they're currently running.  Only
 *      * devices with a USB_DT_DEVICE_QUALIFIER have any OTHER_SPEED_CONFIG
 *       * descriptors.
 *        */
struct usb_config_descriptor {
	u8  bLength;
	u8  bDescriptorType;

	u16 wTotalLength;
	u8  bNumInterfaces;
	u8  bConfigurationValue;
	u8  iConfiguration;
	u8  bmAttributes;
	u8  bMaxPower;
} __attribute__ ((packed));

#define USB_DT_CONFIG_SIZE 		9

/* USB_DT_INTERFACE: Interface descriptor */
struct usb_interface_descriptor {
	u8  bLength;
	u8  bDescriptorType;

	u8  bInterfaceNumber;
	u8  bAlternateSetting;
	u8  bNumEndpoints;
	u8  bInterfaceClass;
	u8  bInterfaceSubClass;
	u8  bInterfaceProtocol;
	u8  iInterface;
} __attribute__ ((packed));

#define USB_DT_INTERFACE_SIZE 		9

/* USB_DT_ENDPOINT: Endpoint descriptor */
struct usb_endpoint_descriptor {
	u8  bLength;
	u8  bDescriptorType;

	u8  bEndpointAddress;
	u8  bmAttributes;
	u16 wMaxPacketSize;
	u8  bInterval;

	/* NOTE:  these two are _only_ in audio endpoints. */
	/* use USB_DT_ENDPOINT*_SIZE in bLength, not sizeof. */
	u8  bRefresh;
	u8  bSynchAddress;
} __attribute__ ((packed));

#define USB_DT_ENDPOINT_SIZE 		7

/*
 *  * Endpoints
 *   */
#define USB_ENDPOINT_NUMBER_MASK 	0x0f 	/* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK 		0x80

#define USB_ENDPOINT_ADDR 			0x0f
#define USB_ENDPOINT_GET_ADDR(a) 	((a) & USB_ENDPOINT_ADDR)

#define USB_ENDPOINT_GET_DIR(a) 	((a) & 0x80)
#define USB_ENDPOINT_SET_DIR(a,d)	((a) | (((d)&1) << 7))
#define USB_ENDPOINT_DIR_OUT 		USB_DIR_OUT
#define USB_ENDPOINT_DIR_IN 		USB_DIR_IN

#define USB_ENDPOINT_XFERTYPE_MASK 	0x03 	/* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL 	0
#define USB_ENDPOINT_XFER_ISOC 		1
#define USB_ENDPOINT_XFER_BULK 		2
#define USB_ENDPOINT_XFER_INT 		3
#define USB_ENDPOINT_MAX_ADJUSTABLE 0x80
#define USB_ENDPOINT_GET_XFERTYPE(a) ((a)&USB_ENDPOINT_XFERTYPE_MASK)

/* The USB 3.0 spec redefines bits 5:4 of bmAttributes as interrupt ep type. */
#define USB_ENDPOINT_INTRTYPE 		0x30
#define USB_ENDPOINT_INTR_PERIODIC 	(0 << 4)
#define USB_ENDPOINT_INTR_NOTIFICATION (1 << 4)

#define USB_ENDPOINT_SYNCTYPE 		0x0c
#define USB_ENDPOINT_SYNC_NONE 		(0 << 2)
#define USB_ENDPOINT_SYNC_ASYNC 	(1 << 2)
#define USB_ENDPOINT_SYNC_ADAPTIVE 	(2 << 2)
#define USB_ENDPOINT_SYNC_SYNC 		(3 << 2)

#define USB_ENDPOINT_USAGE_MASK 	0x30
#define USB_ENDPOINT_USAGE_DATA 	0x00
#define USB_ENDPOINT_USAGE_FEEDBACK 0x10
#define USB_ENDPOINT_USAGE_IMPLICIT_FB 0x20 	/* Implicit feedback Data endpoint */

#endif
