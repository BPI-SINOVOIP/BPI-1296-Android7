
#include <string.h>
#include <stdlib.h>
#include <printf.h>
#include <assert.h>
#include <debug.h>
#include <reg.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <errno.h>

#include <dev/udc.h>
#include <dev/usb-phy.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"

#include "otg-rtk.h"

#define F_LL_INIT   1
#define F_UDC_INIT  2
#define EPT_TX(n) (1 << ((n) + 16))
#define EPT_RX(n) (1 << (n))

#undef log_level
#define log_level 3

/*
 *  EP0_MPS_LIMIT
 *
 *  Unfortunately there seems to be a limit of the amount of data that can
 *  be transferred by IN transactions on EP0. This is either 127 bytes or 3
 *  packets (which practically means 1 packet and 63 bytes of data) when the
 *  MPS is set to 64.
 *
 *  This means if we are wanting to move >127 bytes of data, we need to
 *  split the transactions up, but just doing one packet at a time does
 *  not work (this may be an implicit DATA0 PID on first packet of the
 *  transaction) and doing 2 packets is outside the controller's limits.
 *
 *  If we try to lower the MPS size for EP0, then no transfers work properly
 *  for EP0, and the system will fail basic enumeration. As no cause for this
 *  has currently been found, we cannot support any large IN transfers for
 *  EP0.
 */
#define EP0_MPS_LIMIT 64

struct usb_dev;

typedef struct usb_request {
	udc_request_t req; // must at first

	struct usb_request *next;

	unsigned 		stream_id:16;
	unsigned 		no_interrupt:1;
	unsigned 		zero:1;
	unsigned 		short_not_ok:1;

	int status;
	unsigned actual;
} usb_request_t;

#define udc_req_to_usb_req(req) 			\
	((usb_request_t *)((char *)(req) - 	\
		offsetof(struct usb_request, req)))

typedef struct usb_endpoint {
	udc_endpoint_t ept; //must at first

	struct usb_endpoint *next;
	struct usb_dev *usb;

	unsigned long 		total_data;
	unsigned int 		size_loaded;
	unsigned int 		last_load;
	unsigned int 		fifo_load;
	unsigned short 		fifo_size;

	unsigned char 		dir_in;
	unsigned char 		index;
	unsigned char 		mc;
	unsigned char 		interval;

	unsigned int 		halted:1;
	unsigned int 		periodic:1;
	unsigned int 		isochronous:1;
	unsigned int 		sent_zlp:1;

	char 				name[10];
} usb_endpoint_t;

#define udc_ep_to_usb_ep(ept) 			\
	((usb_endpoint_t *)((char *)(ept) - 	\
		offsetof(struct usb_endpoint, ept)))

typedef struct usb_dev {
	u32 reg_base;

	spin_lock_t lock;

	unsigned char num_of_eps;
	usb_endpoint_t *eps;
    uint32_t ept_alloc_table;

	/* EP0 */
	udc_endpoint_t *ep0in;
	udc_endpoint_t *ep0out;
	udc_request_t *ep0_tx_req; //reply, in
	udc_request_t *ep0_rx_req; //ctrl, out
	u8 txd[8];
	u8 rxd[8];

	struct usb2_phy *usb2phy;
	u32 phyif;
	bool dedicated_fifos;
	uint8_t online;
	uint8_t speed;
	uint8_t config_value;
	uint8_t flags;
	unsigned int setup;

	udc_device_t *device;
	udc_gadget_t *gadget;
	udc_driver_t *driver;
} usb_t;

static usb_t rtk_otg;

/*
 * dump_register - dump state of the udc
 * @param: The device state
 */
static void dump_register(usb_t *usb)
{
	ENTER();
	u32 regs = usb->reg_base;
	u32 val;
	int idx;

	VDBG("GOTGCTL=0x%08x, GOTGINT=0x%08x\n",
		readl(regs + GOTGCTL), readl(regs + GOTGINT));

	VDBG("GAHBCFG=0x%08x, GUSBCFG=0x%08x, GHWCFG1=0x%08x\n",
		readl(regs + GAHBCFG), readl(regs + GUSBCFG), readl(regs + 0x44));

	VDBG("GRSTCTL=0x%08x, GINTSTS=0x%08x, GINTMSK=0x%08x\n",
		readl(regs + GRSTCTL), readl(regs + GINTSTS), readl(regs + GINTMSK));

	VDBG("GRXSTSR=0x%08x, GRXFSIZ=0x%08x\n",
		readl(regs + GRXSTSR),
		readl(regs + GRXFSIZ));

	VDBG("GNPTXFSIZ=0x%08x, GNPTXSTS=0x%08x\n",
		readl(regs + GNPTXFSIZ), readl(regs + GNPTXSTS));

	VDBG("HPTXFSIZ=0x%08x\n", readl(regs + HPTXFSIZ));

	VDBG("show periodic fifo settings\n");
	/* show periodic fifo settings */
	for (idx = 1; idx <= 15; idx++) {
		val = readl(regs + DPTXFSIZn(idx));
		VDBG("DPTx[%d]=0x%08x, FSize=%d, StAddr=0x%08x\n", idx, val,
			val >> DPTXFSIZn_DPTxFSize_SHIFT,
			val & DPTXFSIZn_DPTxFStAddr_MASK);
	}

	VDBG("DCFG=0x%08x, DCTL=0x%08x, DSTS=%08x\n",
		readl(regs + DCFG), readl(regs + DCTL), readl(regs + DSTS));
	VDBG("DIEPMSK=%08x, DOEPMSK=0x%08x\n",
		readl(regs + DIEPMSK), readl(regs + DOEPMSK));

	VDBG("DAINT=0x%08x, DAINTMSK=0x%08x\n",
		readl(regs + DAINT), readl(regs + DAINTMSK));

	VDBG("DIEPEMPMSK=0x%08x, DEACHINT=0x%08x, DEACHINTMSK=0x%08x\n",
		readl(regs + 0x834), readl(regs + 0x838), readl(regs + 0x83c));

	for (idx = 0; idx < 15; idx++) {
		VDBG("ep%d-in: EPCTL=0x%08x, EPINT=0x%08x, SIZ=0x%08x, DMA=0x%08x, DTXFSTS=0x%08x\n", idx,
			readl(regs + DIEPCTL(idx)),
			readl(regs + DIEPINT(idx)),
			readl(regs + DIEPTSIZ(idx)),
			readl(regs + DIEPDMA(idx)),
			readl(regs + DTXFSTS(idx)));

		VDBG("ep%d-out: EPCTL=0x%08x, EPINT=0x%08x, SIZ=0x%08x, DMA=0x%08x\n",
			idx, readl(regs + DOEPCTL(idx)),
			readl(regs + DOEPINT(idx)),
			readl(regs + DOEPTSIZ(idx)),
			readl(regs + DOEPDMA(idx)));
	}

	VDBG("DVBUSDIS=0x%08x, DVBUSPULSE=%08x\n",
		readl(regs + DVBUSDIS), readl(regs + DVBUSPULSE));
	EXIT();
}

// ---- endpoint management
static void __init_ep(usb_t *usb, usb_endpoint_t *usb_ept,
		unsigned epnum)
{
	ENTER();
	udc_endpoint_t *ept;
	u32 ptxfifo;
	const char *dir;
	unsigned max_pkt;

	usb_ept->usb = usb;

	if (epnum == 0)
		dir = "";
	else if ((epnum % 2) == 0) {
		dir = "out";
		usb_ept->dir_in = 0;
	} else {
		dir = "in";
		usb_ept->dir_in = 1;
	}

	max_pkt = epnum ? 1024 : EP0_MPS_LIMIT;

	usb_ept->index = epnum;

	ept = &usb_ept->ept;
	ept->maxpkt = max_pkt;
	ept->num = epnum;
	ept->in = !!usb_ept->dir_in;
	ept->req = 0;
	ept->last = 0;
	snprintf(usb_ept->name, sizeof(usb_ept->name),
			"ep%d%s", epnum, dir);

	if (ept->in) {
		ept->bit = EPT_TX(ept->num);
	} else {
		ept->bit = EPT_RX(ept->num);
	}

	/*
	 * Read the FIFO size for the Periodic TX FIFO, even if we're
	 * an OUT endpoint, we may as well do this if in future the
	 * code is changed to make each endpoint's direction changeable.
	 */
	ptxfifo = readl(usb->reg_base + DPTXFSIZn(ept->num));
	usb_ept->fifo_size = DPTXFSIZn_DPTxFSize_GET(ptxfifo) * 4;

	DBG("init endpoint [ep%d %s] @%p max=%d bit=%x\n",
		epnum, dir, ept, max_pkt, ept->bit);
	EXIT();
}

static udc_endpoint_t *rtk_otg_udc_endpoint_alloc(unsigned type, unsigned maxpkt)
{
	ENTER();
	usb_t *usb = &rtk_otg;
	udc_endpoint_t *ept;
	unsigned n;
	unsigned in = !!(type & 0x80);

	if (!(usb->flags & F_UDC_INIT)) {
		panic("udc_init() must be called before udc_endpoint_alloc()\n");
	}

	DBG("alloc endpoint type=%x (%s) maxpkt=%d\n",
			type, in?"in":"out", maxpkt);
	for (n = 1; n < usb->num_of_eps; n++) {
		unsigned bit = in ? EPT_TX(n) : EPT_RX(n);
		if (usb->ept_alloc_table & (EPT_RX(n) | EPT_TX(n))) {
			continue;
		}
		ept = &usb->eps[n].ept;
		ept->maxpkt = maxpkt;
		if (ept) {
			usb->ept_alloc_table |= bit;
		}
		DBG("[ept%d(%s) %p] ept_alloc_table = 0x%x, bit=%x\n",
				n, in?"in":"out", ept, usb->ept_alloc_table, bit);
		EXIT();
		return ept;
	}
	EXIT();
    return NULL;
}

static void rtk_otg_udc_endpoint_free(struct udc_endpoint *ept)
{
	ENTER();
	// TODO
	EXIT();
}

// ---- request management
static udc_request_t *rtk_otg_udc_request_alloc(void)
{
	ENTER();
	//usb_t *usb = &rtk_otg;
	usb_request_t *usb_req;
	if ((usb_req = malloc(sizeof(*usb_req))) == NULL) {
		ERR("ERROR! malloc fail!\n");
		EXIT();
        return NULL;
	}
	memset(usb_req, 0, sizeof(*usb_req));

	DBG("alloc request %p\n", usb_req);
	EXIT();
    return &usb_req->req;
}

static void rtk_otg_udc_request_free(struct udc_request *req)
{
	ENTER();
	// TODO: check if active?
	usb_request_t *usb_req = (usb_request_t *)req;
	free(usb_req);
	EXIT();
}

static int ep_queue(usb_endpoint_t *usb_ep, usb_request_t *usb_req);

static int rtk_otg_udc_request_queue(udc_endpoint_t *ept, struct udc_request *req)
{
	ENTER();
	int ret = 0;
	spin_lock_saved_state_t state;
	usb_endpoint_t *usb_ept = udc_ep_to_usb_ep(ept);
	usb_request_t *usb_req = udc_req_to_usb_req(req);
	usb_t *usb = usb_ept->usb;

	DBG("[ep%d(%s) %p] queue req %p\n", ept->num, ept->in?"in":"out", ept, req);

	spin_lock_irqsave(&usb->lock, state);

	ret = ep_queue(usb_ept, usb_req);

	spin_unlock_irqrestore(&usb->lock, state);

	EXIT();
	return ret;
}

static void setup_ack(usb_t *usb)
{
	ENTER();
	usb->ep0_tx_req->complete = 0;
	usb->ep0_tx_req->length = 0;
	usb->ep0in->in = 1;
	rtk_otg_udc_request_queue(usb->ep0in, usb->ep0_tx_req);
	EXIT();
}

static void ep0in_complete(struct udc_request *req, unsigned actual, int status)
{
	ENTER();
	usb_t *usb = (usb_t *) req->context;
	DBG("ep0in_complete %p %d %d\n", req, actual, status);
	if (false && status == 0) {
		// TODO fixed
		req->length = 0;
		req->complete = 0;
		usb->ep0out->in = 0;
		rtk_otg_udc_request_queue(usb->ep0out, req);
	}
	EXIT();
}

static udc_request_t *ep_alloc_request(udc_endpoint_t *ep);

static void setup_tx(usb_t *usb, void *buf, unsigned len)
{
	ENTER();
	DBG("setup_tx %p %d\n", buf, len);

	usb->ep0_tx_req = ep_alloc_request(usb->ep0in);

	usb->ep0_tx_req->context = usb;

	usb->ep0_tx_req->buffer = buf;
	usb->ep0_tx_req->complete = ep0in_complete;
	usb->ep0_tx_req->length = len;
	DBG("ep0in %p, ep0_tx_req %p, buffer %p len %d\n",
			usb->ep0in, usb->ep0_tx_req,
			usb->ep0_tx_req->buffer,
			usb->ep0_tx_req->length);

	usb->ep0in->in = 1;
	rtk_otg_udc_request_queue(usb->ep0in, usb->ep0_tx_req);
	EXIT();
}

static int ep_enable(udc_endpoint_t *ep,
		const struct usb_endpoint_descriptor *desc);

static int ep_disable(udc_endpoint_t *ep);

static void stall_ep0(usb_t *usb);

#define SETUP(type,request) (((type) << 8) | (request))

static void handle_setup(usb_t *usb, union setup_packet s)
{
	ENTER();

	// f. process packet
	// g. ensure setup status is 0

	DBG("setup 0x%02x 0x%02x %d %d %d\n",
			s.type, s.request, s.value, s.index, s.length);

	switch (SETUP(s.type, s.request)) {
		case SETUP(DEVICE_READ, GET_STATUS): {
			VDBG("setup DEVICE_READ GET_STATUS\n");
			static unsigned zero = 0;
			if (s.length == 2) {
				setup_tx(usb, &zero, 2);
				return;
			}
			break;
		}
		case SETUP(DEVICE_READ, GET_DESCRIPTOR): {
			VDBG("setup DEVICE_READ GET_DESCRIPTOR\n");
			struct udc_descriptor *desc = udc_descriptor_find(s.value);
			if (desc) {
				unsigned len = desc->len;
				if (len > s.length) len = s.length;
				setup_tx(usb, desc->data, len);
				return;
			}
			break;
		}
		case SETUP(DEVICE_READ, GET_CONFIGURATION):
			VDBG("setup DEVICE_READ GET_CONFIGURATION\n");
			if ((s.value == 0) && (s.index == 0) && (s.length == 1)) {
				setup_tx(usb, &usb->config_value, 1);
				return;
			}
			break;
		case SETUP(DEVICE_WRITE, SET_CONFIGURATION):
			VDBG("setup DEVICE_WRITE SET_CONFIGURATION\n");
			if (s.value == 1) {
				struct udc_descriptor *desc = udc_descriptor_find(TYPE_CONFIGURATION << 8);
				udc_endpoint_t *ept;
				unsigned char *data;
				struct usb_config_descriptor usb_cfg_desc;
				struct usb_endpoint_descriptor usb_ept_desc;
				struct usb_interface_descriptor usb_inf_desc;
				int i;

				data = desc->data;
				usb_cfg_desc.bLength = data[0];
				usb_cfg_desc.bDescriptorType = data[1];
				usb_cfg_desc.wTotalLength = data[2] | data[3] << 8;
				usb_cfg_desc.bNumInterfaces = data[4];
				usb_cfg_desc.bConfigurationValue = data[5];
				usb_cfg_desc.iConfiguration = data[6];
				usb_cfg_desc.bmAttributes = data[7];
				usb_cfg_desc.bMaxPower = data[8];

				VDBG("usb_cfg_desc bLength=%d bDescriptorType=%x, length %d\n",
					usb_cfg_desc.bLength, usb_cfg_desc.bDescriptorType, usb_cfg_desc.wTotalLength);

				data = data + 9;
				usb_inf_desc.bLength = data[0];
				usb_inf_desc.bDescriptorType = data[1];
				usb_inf_desc.bAlternateSetting = data[2];
				usb_inf_desc.bNumEndpoints = data[4];
				usb_inf_desc.bInterfaceClass = data[5];
				usb_inf_desc.bInterfaceSubClass = data[6];
				usb_inf_desc.bInterfaceProtocol = data[7];
				usb_inf_desc.iInterface = data[8];

				VDBG("usb_inf_desc bLength=%d bDescriptorType=%x, bNumEndpoints=%d\n",
					usb_inf_desc.bLength, usb_inf_desc.bDescriptorType, usb_inf_desc.bNumEndpoints);

				data = data + 9;
				/* enable endpoints */
				for (i = 1; i <= usb_inf_desc.bNumEndpoints; i++) {
					ept = &usb->eps[i].ept;
					usb_ept_desc.bLength = data[0];
					usb_ept_desc.bDescriptorType = data[1];
					usb_ept_desc.bEndpointAddress = data[2];
					usb_ept_desc.bmAttributes = data[3];
					usb_ept_desc.wMaxPacketSize = data[5] << 8 | data[4];
					usb_ept_desc.bInterval = data[6];
					usb_ept_desc.bRefresh = data[7];

					VDBG("[ep%d(%s) %p] bLength=%d, bDescriptorType=%x, addr 0x%02x, attr 0x%02x, mps 0x%04x, intr %d\n",
							ept->num, ept->in?"in":"out", ept,
							usb_ept_desc.bLength, usb_ept_desc.bDescriptorType,
							usb_ept_desc.bEndpointAddress, usb_ept_desc.bmAttributes,
							usb_ept_desc.wMaxPacketSize, usb_ept_desc.bInterval);

					DBG("To enable ep%d(%s) %p\n", ept->num, ept->in?"in":"out", ept);
					ep_enable(ept, &usb_ept_desc);

					data = data + 7;
				}

				usb->config_value = 1;
				gadget_connect(usb->driver, usb->gadget);
			} else {
				int i;
				usb->config_value = 0;
				gadget_disconnect(usb->driver, usb->gadget);

				for (i = 1; i < usb->num_of_eps; i++) {
					udc_endpoint_t *ept = &usb->eps[i].ept;
					DBG("To disable ep%d(%s) %p\n", ept->num, ept->in?"in":"out", ept);
					ep_disable(&usb->eps[i].ept);
				}
			}
			setup_ack(usb);
			usb->online = s.value ? 1 : 0;
			EXIT();
			return;
			case SETUP(DEVICE_WRITE, SET_ADDRESS):
			VDBG("setup DEVICE_WRITE SET_ADDRESS\n");
			// write address delayed (will take effect after the next IN txn)
			TODO_FOR("set_addr");
			setup_ack(usb);
			EXIT();
			return;
		case SETUP(INTERFACE_WRITE, SET_INTERFACE):
			DBG("setup INTERFACE_WRITE SET_INTERFACE\n");
			goto stall;
		case SETUP(ENDPOINT_WRITE, CLEAR_FEATURE): {
			DBG("setup ENDPOINT_WRITE CLEAR_FEATURE\n");
			//udc_endpoint_t *ept;
			unsigned num = s.index & 15;
			unsigned in = !!(s.index & 0x80);

			if ((s.value != 0) || (s.length != 0)) {
				break;
			}
			DBG("clr feat %d %d\n", num, in);
			TODO_FOR("endpoint clear feature.........");
			break;
		}
	}

	DBG("udc: stall %02x %02x %04x %04x %04x\n",
		s.type, s.request, s.value, s.index, s.length);

stall:
	stall_ep0(usb);
	EXIT();
}

static void handle_rx_data(usb_t *usb, int ep_idx, int size);

/**
 * read_frameno - read current frame number
 * @usb: The device instance
 *
 * Return the current frame number
 */
static u32 read_frameno(usb_t *usb)
{
	ENTER();
	u32 dsts;

	dsts = readl(usb->reg_base + DSTS);
	dsts &= DSTS_SOFFN_MASK;
	dsts >>= DSTS_SOFFN_SHIFT;

	EXIT();
	return dsts;
}

static void handle_rx_outdone(usb_t *usb, int epnum, bool was_setup);

/*
 * handle_irq_rx - RX FIFO has data
 *
 * The IRQ handler has detected that the RX FIFO has some data in it
 * that requires processing, so find out what is in there and do the
 * appropriate read.
 *
 * The RXFIFO is a true FIFO, the packets coming out are still in packet
 * chunks, so if you have x packets received on an endpoint you'll get x
 * FIFO events delivered, each with a packet's worth of data in it.
 *
 * When using DMA, we should not be processing events from the RXFIFO
 * as the actual data should be sent to the memory directly and we turn
 * on the completion interrupts to get notifications of transfer completion.
 */
static void handle_irq_rx(usb_t *usb) {
	ENTER();
	u32 grxstsr = readl(usb->reg_base + GRXSTSP);
	u32 epnum, status, size;

	epnum = grxstsr & GRXSTS_EPNum_MASK;
	status = grxstsr & GRXSTS_PktSts_MASK;

	size = grxstsr & GRXSTS_ByteCnt_MASK;
	size >>= GRXSTS_ByteCnt_SHIFT;

	VDBG("GRXSTS=0x%08x (size: %d@ep%d)\n",
		grxstsr, size, epnum);

#define __status(x) ((x) >> GRXSTS_PktSts_SHIFT)

	switch (status >> GRXSTS_PktSts_SHIFT) {
	case __status(GRXSTS_PktSts_GlobalOutNAK):
		VDBG("GlobalOutNAK\n");
		break;

	case __status(GRXSTS_PktSts_OutDone):
		VDBG("OutDone (Frame=0x%08x)\n",
			read_frameno(usb));

		handle_rx_outdone(usb, epnum, false);
		break;

	case __status(GRXSTS_PktSts_SetupDone):
		VDBG("SetupDone (Frame=0x%08x, DOPEPCTL=0x%08x)\n",
				read_frameno(usb),
				readl(usb->reg_base + DOEPCTL(0)));

		handle_rx_outdone(usb, epnum, true);
		break;

	case __status(GRXSTS_PktSts_OutRX):
		VDBG("OutRx (epnum %d, size %d)\n", epnum, size);
		handle_rx_data(usb, epnum, size);
		break;

	case __status(GRXSTS_PktSts_SetupRX):
		VDBG("SetupRX (Frame=0x%08x, DOPEPCTL=0x%08x)\n",
			read_frameno(usb),
			readl(usb->reg_base + DOEPCTL(0)));

			handle_rx_data(usb, epnum, size);
		break;

	default:
		ERR("unknown status %08x\n", grxstsr);

		dump_register(usb);
		break;
	}
	EXIT();
}

static void set_epint(usb_t *usb,
		unsigned int ep, unsigned int dir_in, unsigned int en);

static void enqueue_ep0_setup_pkt(usb_t *usb);

static void start_req(usb_t *usb, usb_endpoint_t *ep,
		usb_request_t *req, bool continuing);

static void process_control_req(usb_t *usb,
		struct usb_ctrlrequest *ctrl);

static void set_ep_maxpacket(usb_t *usb,
				       unsigned int ep, unsigned int mps);

static void kill_all_requests(usb_t *usb,
		usb_endpoint_t *usb_ep, int result, bool force);

static void complete_request(usb_t *usb, usb_endpoint_t *usb_ep,
				usb_request_t *usb_req, int result);

/*
 * complete_setup - completion of a setup transfer
 * @ep: The endpoint the request was on.
 * @req: The request completed.
 *
 * Called on completion of any requests the driver itself submitted for
 * EP0 setup packets
 */
static void complete_setup(struct udc_request *req, unsigned actual, int status)
{
	ENTER();
	usb_t *usb = (usb_t *)req->context;

	if (status < 0) {
		DBG("failed %d\n", status);
		return;
	}

	spin_lock(&usb->lock);
	if (actual == 0)
		enqueue_ep0_setup_pkt(usb);
	else {
		process_control_req(usb, req->buffer);
	}
	spin_unlock(&usb->lock);
	EXIT();
}

static int ep_queue(usb_endpoint_t *usb_ep,
		usb_request_t *usb_req)
{
	ENTER();
	udc_endpoint_t *ep = &usb_ep->ept;
	udc_request_t *req = &usb_req->req;
	usb_t *usb = (usb_t *)usb_ep->usb;
	bool first;

	DBG("[ep%d(%s) %p] ep_queue usb_req %p: %d@%p, noi=%d, zero=%d, snok=%d\n",
		ep->num, ep->in?"in":"out", usb_ep,
		usb_req, req->length, req->buffer, usb_req->no_interrupt,
		usb_req->zero, usb_req->short_not_ok);

	/* initialise status of the request */
	usb_req->next = 0;
	usb_req->actual = 0;
	usb_req->status = -EINPROGRESS;

	if (ep->req) {
		// already a transfer in flight, add us to the list
		// we'll get queue'd by the irq handler when it's our turn
		VDBG("[ep%d(%s) %p] Now have request, add to list\n",
				ep->num, ep->in?"in":"out", usb_ep);
		usb_request_t *last = (usb_request_t*)ep->last;
		last->next = usb_req;
	} else {
		VDBG("[ep%d(%s) %p] First request!\n",
				ep->num, ep->in?"in":"out", usb_ep);
		first = true;
	}
	ep->last = req;

	if (first)
		start_req(usb, usb_ep, usb_req, false);

	EXIT();
	return 0;
}

/**
 * ep_dequeue - dequeue given endpoint
 * @ep: The endpoint to dequeue.
 * @req: The request to be removed from a queue.
 */
static int ep_dequeue(usb_endpoint_t *usb_ep, usb_request_t *usb_req)
{
	udc_endpoint_t *ep = &usb_ep->ept;
	udc_request_t *req = &usb_req->req;
	usb_t *usb = (usb_t *)usb_ep->usb;

	//spin_lock_saved_state_t state;

	DBG("[ep%d(%s) %p] ep_dequeue usb_req %p: %d@%p, noi=%d, zero=%d, snok=%d\n",
		ep->num, ep->in?"in":"out", usb_ep,
		usb_req, req->length, req->buffer, usb_req->no_interrupt,
		usb_req->zero, usb_req->short_not_ok);

	//spin_lock_irqsave(&usb->lock, state);
	complete_request(usb, usb_ep, usb_req, -ECONNRESET);
	//spin_unlock_irqrestore(&usb->lock, state);

	return 0;
}

static int rtk_otg_udc_request_dequeue(udc_endpoint_t *ept, struct udc_request *req)
{
	ENTER();
	int ret = 0;
	usb_endpoint_t *usb_ept = udc_ep_to_usb_ep(ept);
	usb_request_t *usb_req = udc_req_to_usb_req(req);
	//usb_t *usb = usb_ept->usb;

	if (ept->req != req) {
		DBG("[ep%d(%s) %p] req %p not in endpoint req %p\n",
				ept->num, ept->in?"in":"out", ept, req, ept->req);
		return ret;
	}
	DBG("[ep%d(%s) %p] dequeue req %p\n", ept->num, ept->in?"in":"out", ept, req);

	ret = ep_dequeue(usb_ept, usb_req);

	EXIT();
	return ret;
}

/**
 * ep_enable - enable the given endpoint
 * @ep: The USB endpint to configure
 * @desc: The USB endpoint descriptor to configure with.
 *
 * This is called from the USB gadget code's usb_ep_enable().
 */
static int ep_enable(udc_endpoint_t *ep,
			const struct usb_endpoint_descriptor *desc)
{
	ENTER();
	usb_endpoint_t *usb_ep = (usb_endpoint_t *)ep;
	usb_t *usb = usb_ep->usb;
	spin_lock_saved_state_t flags;
	int index = usb_ep->index;
	u32 epctrl_reg;
	u32 epctrl;
	u32 mps;
	int dir_in;
	int ret = 0;

	DBG("[ep%d(%s) %p] ep_addr 0x%02x, attr 0x%02x, mps 0x%04x, intr %d\n",
		ep->num, ep->in?"in":"out", ep,
		desc->bEndpointAddress, desc->bmAttributes,
		desc->wMaxPacketSize, desc->bInterval);

	/* not to be called for EP0 */
	WARN_ON(index == 0);

	dir_in = (desc->bEndpointAddress & USB_ENDPOINT_DIR_MASK) ? 1 : 0;
	if (dir_in != usb_ep->dir_in) {
		ERR("direction mismatch!\n");
		return -EINVAL;
	}

	mps = desc->wMaxPacketSize;//usb_endpoint_maxp(desc);

	/* note, we handle this here instead of rtk_hsotg_set_ep_maxpacket */

	epctrl_reg = dir_in ? DIEPCTL(index) : DOEPCTL(index);
	epctrl = readl(usb->reg_base + epctrl_reg);

	DBG("[ep%d(%s) %p] read DxEPCTL=0x%08x from 0x%08x\n",
		ep->num, ep->in?"in":"out", ep,
		epctrl, epctrl_reg);

	spin_lock_irqsave(&usb->lock, flags);

	epctrl &= ~(DxEPCTL_EPType_MASK | DxEPCTL_MPS_MASK);
	epctrl |= DxEPCTL_MPS(mps);

	/*
	 * mark the endpoint as active, otherwise the core may ignore
	 * transactions entirely for this endpoint
	 */
	epctrl |= DxEPCTL_USBActEp;

	/*
	 * set the NAK status on the endpoint, otherwise we might try and
	 * do something with data that we've yet got a request to process
	 * since the RXFIFO will take data for an endpoint even if the
	 * size register hasn't been set.
	 */

	epctrl |= DxEPCTL_SNAK;

	/* update the endpoint state */
	set_ep_maxpacket(usb, usb_ep->index, mps);

	/* default, set to non-periodic */
	usb_ep->isochronous = 0;
	usb_ep->periodic = 0;
	usb_ep->halted = 0;
	usb_ep->interval = desc->bInterval;

	if (usb_ep->interval > 1 && usb_ep->mc > 1)
		ERR("MC > 1 when interval is not 1\n");

	switch (desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) {
	case USB_ENDPOINT_XFER_ISOC:
		epctrl |= DxEPCTL_EPType_Iso;
		epctrl |= DxEPCTL_SetEvenFr;
		usb_ep->isochronous = 1;
		if (dir_in)
			usb_ep->periodic = 1;
		break;

	case USB_ENDPOINT_XFER_BULK:
		epctrl |= DxEPCTL_EPType_Bulk;
		break;

	case USB_ENDPOINT_XFER_INT:
		if (dir_in) {
			/*
			 * Allocate our TxFNum by simply using the index
			 * of the endpoint for the moment. We could do
			 * something better if the host indicates how
			 * many FIFOs we are expecting to use.
			 */

			usb_ep->periodic = 1;
			epctrl |= DxEPCTL_TxFNum(index);
		}

		epctrl |= DxEPCTL_EPType_Intterupt;
		break;

	case USB_ENDPOINT_XFER_CONTROL:
		epctrl |= DxEPCTL_EPType_Control;
		break;
	}

	/*
	 * if the hardware has dedicated fifos, we must give each IN EP
	 * a unique tx-fifo even if it is non-periodic.
	 */
	if (dir_in && usb->dedicated_fifos)
		epctrl |= DxEPCTL_TxFNum(index);

	/* for non control endpoints, set PID to D0 */
	if (index)
		epctrl |= DxEPCTL_SetD0PID;

	DBG("write DxEPCTL=0x%08x\n", epctrl);

	writel(epctrl, usb->reg_base + epctrl_reg);
	DBG("read DxEPCTL=0x%08x\n",
		readl(usb->reg_base + epctrl_reg));

	/* enable the endpoint interrupt */
	set_epint(usb, index, dir_in, 1);

	spin_unlock_irqrestore(&usb->lock, flags);

	EXIT();
	return ret;
}

/**
 * ep_disable - disable given endpoint
 * @ep: The endpoint to disable.
 */
static int ep_disable(udc_endpoint_t *ep)
{
	ENTER();
	usb_endpoint_t *usb_ep = (usb_endpoint_t *)ep;
	usb_t *usb = usb_ep->usb;
	int dir_in = usb_ep->dir_in;
	int index = usb_ep->index;
	spin_lock_saved_state_t flags;
	u32 epctrl_reg;
	u32 ctrl;

	DBG("[ep%d(%s) %p] ep_disable\n", ep->num, ep->in?"in":"out", ep);

	if (ep == &usb->eps[0].ept) {
		ERR("called for ep0\n");
		EXIT();
		return -EINVAL;
	}

	epctrl_reg = dir_in ? DIEPCTL(index) : DOEPCTL(index);

	spin_lock_irqsave(&usb->lock, flags);
	/* terminate all requests with shutdown */
	kill_all_requests(usb, usb_ep, -ESHUTDOWN, false);


	ctrl = readl(usb->reg_base + epctrl_reg);
	ctrl &= ~DxEPCTL_EPEna;
	ctrl &= ~DxEPCTL_USBActEp;
	ctrl |= DxEPCTL_SNAK;

	DBG("DxEPCTL=0x%08x\n", ctrl);
	writel(ctrl, usb->reg_base + epctrl_reg);

	/* disable endpoint interrupts */
	set_epint(usb, usb_ep->index, usb_ep->dir_in, 0);

	spin_unlock_irqrestore(&usb->lock, flags);
	EXIT();
	return 0;
}

/**
 * ep_sethalt - set halt on a given endpoint
 * @ep: The endpoint to set halt.
 * @value: Set or unset the halt.
 */
static int ep_sethalt(usb_endpoint_t *usb_ep, int value)
{
	ENTER();
	udc_endpoint_t *ep = &usb_ep->ept;
	usb_t *usb = (usb_t *)usb_ep->usb;
	int index = usb_ep->index;
	u32 epreg;
	u32 epctl;
	u32 xfertype;

	DBG("[ep%d(%s) %p] ep_sethalt value %d\n",
		ep->num, ep->in?"in":"out", usb_ep, value);

	if (index == 0) {
		if (value)
			stall_ep0(usb);
		else
			WARN("%s: can't clear halt on ep0\n", __func__);
		return 0;
	}

	/* write both IN and OUT control registers */

	epreg = DIEPCTL(index);
	epctl = readl(usb->reg_base + epreg);

	if (value) {
		epctl |= DxEPCTL_Stall + DxEPCTL_SNAK;
		if (epctl & DxEPCTL_EPEna)
			epctl |= DxEPCTL_EPDis;
	} else {
		epctl &= ~DxEPCTL_Stall;
		xfertype = epctl & DxEPCTL_EPType_MASK;
		if (xfertype == DxEPCTL_EPType_Bulk ||
			xfertype == DxEPCTL_EPType_Intterupt)
				epctl |= DxEPCTL_SetD0PID;
	}

	writel(epctl, usb->reg_base + epreg);

	epreg = DOEPCTL(index);
	epctl = readl(usb->reg_base + epreg);

	if (value)
		epctl |= DxEPCTL_Stall;
	else {
		epctl &= ~DxEPCTL_Stall;
		xfertype = epctl & DxEPCTL_EPType_MASK;
		if (xfertype == DxEPCTL_EPType_Bulk ||
			xfertype == DxEPCTL_EPType_Intterupt)
				epctl |= DxEPCTL_SetD0PID;
	}

	writel(epctl, usb->reg_base + epreg);

	usb_ep->halted = value;

	EXIT();
	return 0;
}

/**
 * enqueue_ep0_setup_pkt - start a request for EP0 packets
 * @usb: The device state.
 *
 * Enqueue a request on EP0 if necessary to received any SETUP packets
 * received from the host.
 */
static void enqueue_ep0_setup_pkt(usb_t *usb)
{
	ENTER();
	udc_request_t *req = usb->ep0_rx_req;
	udc_endpoint_t *ep0out = usb->ep0out;
	usb_request_t *usb_req = (usb_request_t *)req;
	usb_endpoint_t *usb_ep0out = (usb_endpoint_t *)ep0out;
	int ret;

	DBG("qeueing setup request\n");

	usb_req->zero = 0;
	req->length = 8;
	req->buffer = usb->rxd;
	req->complete = complete_setup;

	DBG("ep0out %p, REQ %p buf %p len %d complete %p noi=%d zp=%d snok=%d\n",
			ep0out, req,
			req->buffer, req->length, req->complete,
			usb_req->no_interrupt, usb_req->zero,
			usb_req->short_not_ok);

	if (ep0out->req) {
		DBG("already queued???\n");
		return;
	}

	usb->ep0out->in = 0;

	ret = ep_queue(usb_ep0out, usb_req);
	if (ret < 0) {
		ERR("failed queue (%d)\n", ret);
		/*
		 * Don't think there's much we can do other than watch the
		 * driver fail.
		 */
	}
	EXIT();
}

/**
 * en_gsint - enable one or more of the general interrupt
 * @usb: The device state
 * @ints: A bitmask of the interrupts to enable
 */
static void en_gsint(usb_t *usb, u32 ints)
{
	ENTER();
	u32 gsintmsk = readl(usb->reg_base + GINTMSK);
	u32 new_gsintmsk;

	new_gsintmsk = gsintmsk | ints;

	if (new_gsintmsk != gsintmsk) {
		DBG("gsintmsk now 0x%08x\n", new_gsintmsk);
		writel(new_gsintmsk, usb->reg_base + GINTMSK);
	}
	EXIT();
}

/**
 * disable_gsint - disable one or more of the general interrupt
 * @usb: The device state
 * @ints: A bitmask of the interrupts to enable
 */
static void disable_gsint(usb_t *usb, u32 ints)
{
	u32 gsintmsk = readl(usb->reg_base + GINTMSK);
	u32 new_gsintmsk;

	new_gsintmsk = gsintmsk & ~ints;

	if (new_gsintmsk != gsintmsk)
		writel(new_gsintmsk, usb->reg_base + GINTMSK);
}

/**
 * set_epint - enable/disable an endpoint irq
 * @usb: The device state
 * @ep: The endpoint index
 * @dir_in: True if direction is in.
 * @en: The enable value, true to enable
 *
 * Set or clear the mask for an individual endpoint's interrupt
 * request.
 */
static void set_epint(usb_t *usb,
	 unsigned int ep, unsigned int dir_in,
	 unsigned int en)
{
	ENTER();

	spin_lock_saved_state_t flags;
	u32 bit = 1 << ep;
	u32 daint;

	if (!dir_in)
		bit <<= 16;

	local_irq_save(flags);
	daint = readl(usb->reg_base + DAINTMSK);
	if (en)
		daint |= bit;
	else
		daint &= ~bit;
	writel(daint, usb->reg_base + DAINTMSK);
	local_irq_restore(flags);
	DBG("ctrl ep%d, dir %s, %s interrput (0x%08x)\n",
			ep, dir_in?"in":"out", en?"enable":"disable",
			readl(usb->reg_base + DAINTMSK));
	EXIT();
}

/*
 * init_fifo - initialise non-periodic FIFOs
 */
static void init_fifo(usb_t *usb)
{
	ENTER();
	unsigned int ep;
	unsigned int addr;
	unsigned int size;
	int timeout;
	u32 val;

#define FIFO_SZ 	512 //2048
#define FIFO_DEPTH	256 //1024
	/* set FIFO sizes to 512/256 2048/1024 */
	writel(FIFO_SZ, usb->reg_base + GRXFSIZ);
	writel(GNPTXFSIZ_NPTxFStAddr(FIFO_SZ) |
			GNPTXFSIZ_NPTxFDep(FIFO_DEPTH),
			usb->reg_base + GNPTXFSIZ);

	/*
	 * arange all the rest of the TX FIFOs, as some versions of this
	 * block have overlapping default addresses. This also ensures
	 * that if the settings have been changed, then they are set to
	 * known values.
	 */

	/* start at the end of the GNPTXFSIZ, rounded up */
	addr = FIFO_SZ + FIFO_DEPTH;
	size = 768;

	/*
	 * currently we allocate TX FIFOs for all possible endpoints,
	 * and assume that they are all the same size.
	 */

	for (ep = 1; ep <= 15; ep++) {
		val = addr;
		val |= size << DPTXFSIZn_DPTxFSize_SHIFT;
		addr += size;

		writel(val, usb->reg_base + DPTXFSIZn(ep));
		DBG("ep%d, write DPTXFSIZn by 0x%08x == 0x%08x\n",
			ep, val, readl(usb->reg_base + DPTXFSIZn(ep)));
	}

	/*
	 * according to p428 of the design guide, we need to ensure that
	 * all fifos are flushed before continuing
	 */
	writel(GRSTCTL_TxFNum(0x10) | GRSTCTL_TxFFlsh |
			GRSTCTL_RxFFlsh, usb->reg_base + GRSTCTL);

	/* wait until the fifos are both flushed */
	timeout = 100;
	while (1) {
		val = readl(usb->reg_base + GRSTCTL);

		if ((val & (GRSTCTL_TxFFlsh | GRSTCTL_RxFFlsh)) == 0)
			break;

		if (--timeout == 0) {
			ERR("timeout flushing fifos (GRSTCTL=%08x)\n",
				val);
		}

		udelay(1);
	}

	DBG("FIFOs reset, timeout count=%d\n", timeout);

	EXIT();
}

/*
 * is_ep_periodic - return true if the endpoint is in periodic mode.
 * @hs_ep: The endpoint to query.
 *
 * Returns true if the endpoint is in periodic mode, meaning it is being
 * used for an Interrupt or ISO transfer.
 */
static inline int is_ep_periodic(usb_endpoint_t *usb_ep)
{
	return usb_ep->periodic;
}

/**
 * @ep: USB endpoint to allocate request for.
 * @flags: Allocation flags
 *
 * Allocate a new USB request structure appropriate for the specified endpoint
 */
static udc_request_t *ep_alloc_request(udc_endpoint_t *ep)
{
	ENTER();
	usb_request_t *usb_req;
	if ((usb_req = malloc(sizeof(*usb_req))) == NULL) {
		ERR("ERROR! malloc fail!\n");
		EXIT();
        return NULL;
	}

	memset(usb_req, 0, sizeof(*usb_req));
	EXIT();
    return &usb_req->req;
}

/*
 * write_fifo - write packet Data to the TxFIFO
 * @usb: The controller state.
 * @ep: The endpoint we're going to write for.
 * @req: The request to write data for.
 *
 * This is called when the TxFIFO has some space in it to hold a new
 * transmission and we have something to give it. The actual setup of
 * the data size is done elsewhere, so all we have to do is to actually
 * write the data.
 *
 * The return value is zero if there is more space (or nothing was done)
 * otherwise -ENOSPC is returned if the FIFO space was used up.
 *
 * This routine is only needed for PIO
 */
static int write_fifo(usb_t *usb,
				usb_endpoint_t *usb_ep,
				usb_request_t *usb_req)
{
	ENTER();
	bool periodic = is_ep_periodic(usb_ep);
	u32 gnptxsts = readl(usb->reg_base + GNPTXSTS);
	int buf_pos = usb_req->actual;
	int to_write = usb_ep->size_loaded;
	void *data;
	int can_write;
	int pkt_round;
	int max_transfer;

	DBG("[ep%d(%s) %p], req %p, %s periodic, GNPTXSTS=0x%08x, ep-index=%d\n",
		usb_ep->ept.num, usb_ep->ept.in ? "in" : "out", usb_ep, usb_req,
		periodic?"is":"not", gnptxsts, usb_ep->index);

	to_write -= (buf_pos - usb_ep->last_load);

	/* if there's nothing to write, get out early */
	if (to_write == 0) {
		EXIT();
		return 0;
	}

	if (periodic && !usb->dedicated_fifos) {
		u32 epsize = readl(usb->reg_base + DIEPTSIZ(usb_ep->index));
		int size_left;
		int size_done;

		/*
		 * work out how much data was loaded so we can calculate
		 * how much data is left in the fifo.
		 */

		size_left = DxEPTSIZ_XferSize_GET(epsize);

		/*
		 * if shared fifo, we cannot write anything until the
		 * previous data has been completely sent.
		 */
		if (usb_ep->fifo_load != 0) {
			en_gsint(usb, GINTSTS_PTxFEmp);
			EXIT();
			return -ENOSPC;
		}

		DBG("left=%d, load=%d, fifo=%d, size %d\n",
			size_left,
			usb_ep->size_loaded, usb_ep->fifo_load, usb_ep->fifo_size);

		/* how much of the data has moved */
		size_done = usb_ep->size_loaded - size_left;

		/* how much data is left in the fifo */
		can_write = usb_ep->fifo_load - size_done;
		DBG("=> can_write1=%d\n",
			can_write);

		can_write = usb_ep->fifo_size - can_write;
		DBG("=> can_write2=%d\n",
			can_write);

		if (can_write <= 0) {
			en_gsint(usb, GINTSTS_PTxFEmp);
			return -ENOSPC;
		}
	} else if (usb->dedicated_fifos && usb_ep->index != 0) {
		can_write = readl(usb->reg_base + DTXFSTS(usb_ep->index));

		can_write &= 0xffff;
		can_write *= 4;
	} else {
		if (GNPTXSTS_NPTxQSpcAvail_GET(gnptxsts) == 0) {
			DBG("no queue slots available (0x%08x)\n",
				gnptxsts);

			en_gsint(usb, GINTSTS_NPTxFEmp);
			return -ENOSPC;
		}

		can_write = GNPTXSTS_NPTxFSpcAvail_GET(gnptxsts);
		can_write *= 4;	/* fifo size is in 32bit quantities. */
	}

	max_transfer = usb_ep->ept.maxpkt * usb_ep->mc;

	DBG("GNPTXSTS=%08x, can=%d, to=%d, max_transfer %d\n",
		gnptxsts, can_write, to_write, max_transfer);

	/*
	 * limit to 512 bytes of data, it seems at least on the non-periodic
	 * FIFO, requests of >512 cause the endpoint to get stuck with a
	 * fragment of the end of the transfer in it.
	 */
	if (can_write > 512 && !periodic)
		can_write = 512;

	/*
	 * limit the write to one max-packet size worth of data, but allow
	 * the transfer to return that it did not run out of fifo space
	 * doing it.
	 */
	if (to_write > max_transfer) {
		to_write = max_transfer;

		/* it's needed only when we do not use dedicated fifos */
		if (!usb->dedicated_fifos)
			en_gsint(usb, periodic ? GINTSTS_PTxFEmp :
					   GINTSTS_NPTxFEmp);
	}

	/* see if we can write data */

	if (to_write > can_write) {
		to_write = can_write;
		pkt_round = to_write % max_transfer;

		/*
		 * Round the write down to an
		 * exact number of packets.
		 *
		 * Note, we do not currently check to see if we can ever
		 * write a full packet or not to the FIFO.
		 */

		if (pkt_round)
			to_write -= pkt_round;

		/*
		 * enable correct FIFO interrupt to alert us when there
		 * is more room left.
		 */

		/* it's needed only when we do not use dedicated fifos */
		if (!usb->dedicated_fifos)
			en_gsint(usb, periodic ? GINTSTS_PTxFEmp :
					   GINTSTS_NPTxFEmp);
	}

	DBG("write %d/%d, can_write %d, done %d\n",
		to_write, usb_req->req.length, can_write, buf_pos);

	if (to_write <= 0) {
		EXIT();
		return -ENOSPC;
	}

	usb_req->actual = buf_pos + to_write;
	usb_ep->total_data += to_write;

	if (periodic)
		usb_ep->fifo_load += to_write;

	to_write = DIV_ROUND_UP(to_write, 4);
	data = usb_req->req.buffer + buf_pos;

	iowrite32_rep(usb->reg_base + EPFIFO(usb_ep->index), data, to_write);

	EXIT();
	return (to_write >= can_write) ? -ENOSPC : 0;
}

/**
 * get_ep_limit - get the maximum data legnth for this endpoint
 * @hs_ep: The endpoint
 *
 * Return the maximum data that can be queued in one go on a given endpoint
 * so that transfers that are too long can be split.
 */
static unsigned get_ep_limit(udc_endpoint_t *ep)
{
	usb_endpoint_t * usb_ep = (usb_endpoint_t *)ep;
	int index = usb_ep->index;
	unsigned maxsize;
	unsigned maxpkt;

	if (index != 0) {
		maxsize = DxEPTSIZ_XferSize_LIMIT + 1;
		maxpkt = DxEPTSIZ_PktCnt_LIMIT + 1;
	} else {
		maxsize = 64 + 64;
		if (usb_ep->dir_in)
			maxpkt = DIEPTSIZ0_PktCnt_LIMIT + 1;
		else
			maxpkt = 2;
	}

	/* we made the constant loading easier above by using +1 */
	maxpkt--;
	maxsize--;

	/*
	 * constrain by packet count if maxpkts*pktsize is greater
	 * than the length register size.
	 */

	if ((maxpkt * usb_ep->ept.maxpkt) < maxsize)
		maxsize = maxpkt * usb_ep->ept.maxpkt;

	VDBG("[ep%d(%s) %p] request maxsize %d\n",
			ep->num, ep->in?"in":"out", ep, maxsize);

	return maxsize;
}

/**
 * start_req - start a USB request from an endpoint's queue
 * @usb: The controller device.
 * @usb_ep: The endpoint to process a request for
 * @usb_req: The request to start.
 * @continuing: True if we are doing more for the current request.
 *
 * Start the given request running by setting the endpoint registers
 * appropriately, and writing any data to the FIFOs.
 */
static void start_req(usb_t *usb, usb_endpoint_t *usb_ep,
			usb_request_t *usb_req, bool continuing)
{
	ENTER();
	struct udc_endpoint *ep = &usb_ep->ept;
	struct udc_request *req = &usb_req->req;
	int index = ep->num;
	int dir_in = ep->in;
	u32 epctrl_reg;
	u32 epsize_reg;
	u32 epsize;
	u32 ctrl;
	unsigned length;
	unsigned packets;
	unsigned maxreq;

	if (index != 0) {
		if (ep->req && !continuing) {
			ERR("[ep%d(%s) %p] ep->req %p req %p, active request\n",
					ep->num, ep->in?"in":"out", ep, ep->req, req);
			WARN_ON(1);
			return;
		} else if (ep->req != req && continuing) {
			ERR("continue different req\n");
			WARN_ON(1);
			return;
		}
	}

	epctrl_reg = dir_in ? DIEPCTL(index) : DOEPCTL(index);
	epsize_reg = dir_in ? DIEPTSIZ(index) : DOEPTSIZ(index);

	/* If endpoint is stalled, we will restart request later */
	ctrl = readl(usb->reg_base + epctrl_reg);

	VDBG("[ep%d(%s) %p] DxEPCTL=0x%08x\n",
		index, ep->in?"in":"out", ep, ctrl);

	if (ctrl & DxEPCTL_Stall) {
		ERR("ERROR! ep%d is stalled\n", index);
		return;
	}

	length = req->length - usb_req->actual;
	VDBG("[ep%d(%s) %p] REQ %p buf %p len %d noi=%d zp=%d snok=%d\n",
			ep->num, ep->in?"in":"out", ep,
			req, req->buffer, length,
			usb_req->no_interrupt, usb_req->zero,
			usb_req->short_not_ok);

	maxreq = get_ep_limit(ep);
	if (length > maxreq) {
		int round = maxreq % usb_ep->ept.maxpkt;

		VDBG("[ep%d(%s) %p] length %d, max-req %d, round %d\n",
				ep->num, ep->in?"in":"out", ep,
				length, maxreq, round);

		/* round down to multiple of packets */
		if (round)
			maxreq -= round;

		length = maxreq;
	}

	if (length)
		packets = DIV_ROUND_UP(length, usb_ep->ept.maxpkt);
	else
		packets = 1; 	/* send one packet if length is zero. */

	if (usb_ep->isochronous && length > (usb_ep->mc * usb_ep->ept.maxpkt)) {
		ERR("Error! [ep%d(%s) %p] lreq length > maxpacket*mc\n",
				ep->num, ep->in?"in":"out", ep);
		return;
	}

	if (dir_in && index != 0)
		if (usb_ep->isochronous)
			epsize = DxEPTSIZ_MC(packets);
		else
			epsize = DxEPTSIZ_MC(1);
	else
		epsize = 0;

	if (index != 0 && usb_req->zero) {
		/*
		 * test for the packets being exactly right for the
		 * transfer
		 */

		if (length == (packets * usb_ep->ept.maxpkt))
			packets++;
	}

	epsize |= DxEPTSIZ_PktCnt(packets);
	epsize |= DxEPTSIZ_XferSize(length);

	VDBG("[ep%d(%s) %p] packets %d@%d/%d, write epsize 0x%08x to epsize_reg @0x%08x\n",
			ep->num, ep->in?"in":"out", ep,
			packets, length, req->length, epsize, epsize_reg);

	VDBG("[ep%d(%s) %p] set current request to %p\n",
			ep->num, ep->in?"in":"out", ep, req);
	/* store the request as the current one we're doing */
	ep->req = req;

	/* write size / packets */
	writel(epsize, usb->reg_base + epsize_reg);

	ctrl |= DxEPCTL_EPEna; /* ensure ep enabled */
	ctrl |= DxEPCTL_USBActEp;

	VDBG("[ep%d(%s) %p] setup req:%d\n",
			ep->num, ep->in?"in":"out", ep,
			usb->setup);

	/* For Setup request do not clear NAK */
#if 0 //barry
	if (usb->setup && index == 0)
		usb->setup = 0;
	else
#endif
		ctrl |= DxEPCTL_CNAK; /* clear NAK set by core */

	VDBG("[ep%d(%s) %p] write ctrl 0x%08x to DxEPCTL @0x%08x\n",
			ep->num, ep->in?"in":"out", ep,
			ctrl, epctrl_reg);
	writel(ctrl, usb->reg_base + epctrl_reg);

	/*
	 * set these, it seems that DMA support increments past the end
	 * of the packet buffer so we need to calculate the length from
	 * this information.
	 */
	usb_ep->size_loaded = length;
	usb_ep->last_load = usb_req->actual;

	if (dir_in) {
		/* set these anyway, we may need them for non-periodic in */
		usb_ep->fifo_load = 0;

		write_fifo(usb, usb_ep, usb_req);
	}

	/*
	 * clear the INTknTXFEmpMsk when we start request, more as a aide
	 * to debugging to see what is going on.
	 */
	if (dir_in)
		writel(DIEPMSK_INTknTXFEmpMsk,
			usb->reg_base + DIEPINT(index));

	/* check ep is enabled */
	if (!(readl(usb->reg_base + epctrl_reg) & DxEPCTL_EPEna))
		ERR("[ep%d(%s) %p] failed to become enabled (DxEPCTL=0x%08x)?\n",
				ep->num, ep->in?"in":"out", ep,
			 		readl(usb->reg_base + epctrl_reg));

	VDBG("[ep%d(%s) %p] read DxEPCTL=0x%08x @0x%08x\n",
			ep->num, ep->in?"in":"out", ep,
			readl(usb->reg_base + epctrl_reg), epctrl_reg);

	/* enable ep interrupts */
	set_epint(usb, ep->num, ep->in, 1);
	EXIT();
}

static void ep_free_request(udc_endpoint_t *ep,
				      udc_request_t *req)
{
	ENTER();
	usb_request_t *usb_req = (usb_request_t *)req;

	free(usb_req);
	EXIT();
}

/**
 * complete_oursetup - setup completion callback
 * @ep: The endpoint the request was on.
 * @req: The request completed.
 *
 * Called on completion of any requests the driver itself
 * submitted that need cleaning up.
 */
static void complete_oursetup(udc_request_t *req, unsigned actual, int status)
{
	ENTER();
	TODO_FOR("complete_oursetup\n");
//	DBG("[ep%d(%s) %p] complete_oursetup req %p\n",
//			ep->num, ep->in?"in":"out", ep, req);

//	ep_free_request(ep, req);
	EXIT();
}

/**
 * ep_from_windex - convert control wIndex value to endpoint
 * @usb: The driver state.
 * @windex: The control request wIndex field (in host order).
 *
 * Convert the given wIndex into a pointer to an driver endpoint
 * structure, or return NULL if it is not a valid endpoint.
 */
static usb_endpoint_t *ep_from_windex(usb_t *usb,
					   u32 windex)
{
	usb_endpoint_t *ep = &usb->eps[windex & 0x7F];
	int dir = (windex & USB_DIR_IN) ? 1 : 0;
	int idx = windex & 0x7F;

	if (windex >= 0x100)
		return NULL;

	if (idx > usb->num_of_eps)
		return NULL;

	if (idx && ep->dir_in != dir)
		return NULL;

	return ep;
}

/**
 * send_reply - send reply to control request
 * @usb: The device state
 * @ep: Endpoint 0
 * @buff: Buffer for request
 * @length: Length of reply.
 *
 * Create a request and queue it on the given endpoint. This is useful as
 * an internal method of sending replies to certain control requests, etc.
 */
static int send_reply(usb_t *usb, usb_endpoint_t *usb_ep,
				void *buff, int length)
{
	ENTER();
	udc_request_t *req;
	usb_request_t *usb_req;
	int ret;

	DBG("[ep%d(%s) %p] send_reply buff %p, len %d\n",
			usb_ep->ept.num, usb_ep->ept.in?"in":"out", usb_ep,
			buff, length);

	req = ep_alloc_request(&usb_ep->ept);
	usb_req = (usb_request_t *)req;

	req->context = usb;
	usb->ep0_tx_req = req;
	if (!req) {
		ERR("cannot alloc req\n");
		EXIT();
		return -ENOMEM;
	}

	req->buffer = usb->txd;
	req->length = length;
	usb_req->zero = 1; /* always do zero-length final transfer */
	req->complete = 0;
	req->complete = complete_oursetup;

	if (length)
		memcpy(req->buffer, buff, length);
	else
		usb_ep->sent_zlp = 1;

	usb->ep0in->in = 1;
	ret = ep_queue(usb_ep, usb_req);
	if (ret) {
		ERR("cannot queue req\n");
		EXIT();
		return ret;
	}

	EXIT();
	return 0;
}

/*
 * process_req_status - process request GET_STATUS
 * @usb: The device state
 * @ctrl: USB control request
 */
static int process_req_status(usb_t *usb,
					struct usb_ctrlrequest *ctrl)
{
	usb_endpoint_t *ep0 = &usb->eps[0];
	usb_endpoint_t *ep;
	u16 reply;
	int ret;

	if (!ep0->dir_in) {
		ERR("Error! direction out?\n");
		return -EINVAL;
	}

	switch (ctrl->bRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		reply = cpu_to_le16(0); /* bit 0 => self powered,
					 * bit 1 => remote wakeup */
		break;

	case USB_RECIP_INTERFACE:
		/* currently, the data result should be zero */
		reply = cpu_to_le16(0);
		break;

	case USB_RECIP_ENDPOINT:
		ep = ep_from_windex(usb, le16_to_cpu(ctrl->wIndex));
		if (!ep)
			return -ENOENT;

		reply = cpu_to_le16(ep->halted ? 1 : 0);
		break;

	default:
		return 0;
	}

	if (le16_to_cpu(ctrl->wLength) != 2)
		return -EINVAL;

	ret = send_reply(usb, ep0, &reply, 2);
	if (ret) {
		ERR("failed to send reply\n");
		return ret;
	}

	return 1;
}

static int ep_sethalt(usb_endpoint_t *usb_ep, int value);

/*
 * process_req_featire - process request {SET,CLEAR}_FEATURE
 * @hsotg: The device state
 * @ctrl: USB control request
 */
static int process_req_feature(usb_t * usb,
					 struct usb_ctrlrequest *ctrl)
{
	ENTER();
	usb_endpoint_t *ep0 = &usb->eps[0];
	usb_request_t *usb_req = NULL;
	bool restart = false;
	bool set = (ctrl->bRequest == USB_REQ_SET_FEATURE);
	usb_endpoint_t *usb_ep;
	udc_endpoint_t *ep;
	int ret;
	bool halted;

	DBG("%s_FEATURE\n", set ? "SET" : "CLEAR");

	if (ctrl->bRequestType == USB_RECIP_ENDPOINT) {
		usb_ep = ep_from_windex(usb, le16_to_cpu(ctrl->wIndex));
		ep = &usb_ep->ept;
		if (!ep) {
			DBG("no endpoint for 0x%04x\n",
				le16_to_cpu(ctrl->wIndex));
			EXIT();
			return -ENOENT;
		}

		switch (le16_to_cpu(ctrl->wValue)) {
		case USB_ENDPOINT_HALT:
			halted = usb_ep->halted;

			ep_sethalt(usb_ep, set);

			ret = send_reply(usb, ep0, NULL, 0);
			if (ret) {
				DBG("failed to send reply\n");
				EXIT();
				return ret;
			}

			/*
			 * we have to complete all requests for ep if it was
			 * halted, and the halt was cleared by CLEAR_FEATURE
			 */

			if (!set && halted) {
				/*
				 * If we have request in progress,
				 * then complete it
				 */
				if (ep->req) {
					usb_req = (usb_request_t *)ep->req;
					ep->req = NULL;
					usb_req->req.complete(&usb_req->req, usb_req->actual, usb_req->status);
				}

				/* If we have pending request, then start it */
				TODO_FOR("restart");
				if (false && usb_req && usb_req->next) {
					usb_request_t *next = usb_req->next;
					usb_ep->ept.req = &next->req;
					restart = true;
				}
				if (restart) {
					DBG("restart start_req\n");
					usb_req = (usb_request_t *)usb_ep->ept.req;
					start_req(usb, ep0,
							    usb_req, false);
				}
			}

			break;

		default:
			EXIT();
			return -ENOENT;
		}
	} else {
		EXIT();
		return -ENOENT;  /* currently only deal with endpoint */
	}

	EXIT();
	return 1;
}

static void enqueue_ep0_setup_pkt(usb_t *usb);
static void disconnect(usb_t *usb);

/*
 * stall_ep0 - stall ep0
 * @hsotg: The device state
 *
 * Set stall for ep0 as response for setup request.
 */
static void stall_ep0(usb_t *usb) {
	usb_endpoint_t *ep0 = &usb->eps[0];
	u32 reg;
	u32 ctrl;

	DBG("ep0 stall (dir=%d)\n", ep0->dir_in);
	reg = (ep0->dir_in) ? DIEPCTL0 : DOEPCTL0;

	/*
	 * DxEPCTL_Stall will be cleared by EP once it has
	 * taken effect, so no need to clear later.
	 */

	ctrl = readl(usb->reg_base + reg);
	ctrl |= DxEPCTL_Stall;
	ctrl |= DxEPCTL_CNAK;
	writel(ctrl, usb->reg_base + reg);

	DBG("written DxEPCTL=0x%08x to %08x (DxEPCTL=0x%08x)\n",
		ctrl, reg, readl(usb->reg_base + reg));

	 /*
	  * complete won't be called, so we enqueue
	  * setup request here
	  */
	 enqueue_ep0_setup_pkt(usb);
}

/*
 * process_control_req - process a control request
 * @usb: The device state
 * @ctrl: The control request received
 *
 * The controller has received the SETUP phase of a control request, and
 * needs to work out what to do next (and whether to pass it on to the
 * gadget driver).
 */
static void process_control_req(usb_t *usb,
				      struct usb_ctrlrequest *ctrl)
{
	ENTER();
	usb_endpoint_t *ep0 = &usb->eps[0];
	int ret = 0;
	u32 dcfg;

	ep0->sent_zlp = 0;

	DBG("ctrl Req=0x%02x, Type=0x%02x, V=0x%04x, L=0x%04x\n",
		 ctrl->bRequest, ctrl->bRequestType,
		 ctrl->wValue, ctrl->wLength);

	/*
	 * record the direction of the request, for later use when enquing
	 * packets onto EP0.
	 */

	ep0->dir_in = (ctrl->bRequestType & USB_DIR_IN) ? 1 : 0;
	DBG("ctrl ep0 %p dir %s\n", ep0, ep0->dir_in?"in":"out");

	/*
	 * if we've no data with this request, then the last part of the
	 * transaction is going to implicitly be IN.
	 */
	if (ctrl->wLength == 0)
		ep0->dir_in = 1;

	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		switch (ctrl->bRequest) {
		case USB_REQ_SET_ADDRESS:
			disconnect(usb);
			dcfg = readl(usb->reg_base + DCFG);
			dcfg &= ~DCFG_DevAddr_MASK;
			dcfg |= ctrl->wValue << DCFG_DevAddr_SHIFT;
			writel(dcfg, usb->reg_base + DCFG);

			DBG("new address %d\n", ctrl->wValue);

			ret = send_reply(usb, ep0, NULL, 0);
			return;

		case USB_REQ_GET_STATUS:
			DBG("USB_REQ_GET_STATUS\n");
			ret = process_req_status(usb, ctrl);
			break;

		case USB_REQ_CLEAR_FEATURE:
		case USB_REQ_SET_FEATURE:
			ret = process_req_feature(usb, ctrl);
			break;
		}
	}

	// flush any pending io from previous setup transactions
	usb->ep0in->req = 0;
	usb->ep0out->req = 0;

	/* as a fallback, try delivering it to the driver to deal with */
	if (ret == 0) {
		spin_unlock(&usb->lock);
		union setup_packet s;
		s.request = ctrl->bRequest;
		s.type = ctrl->bRequestType;
		s.value = ctrl->wValue;
		s.index = ctrl->wIndex;
		s.length = ctrl->wLength;
		handle_setup(usb, s);
		spin_lock(&usb->lock);
	}

	/*
	 * the request is either unhandlable, or is not formatted correctly
	 * so respond with a STALL for the status stage to indicate failure.
	 */

	if (ret < 0)
		stall_ep0(usb);
	EXIT();
}

/**
 * complete_request - complete a request given to us
 * @usb: The device state.
 * @usb_ep: The endpoint the request was on.
 * @usb_req: The request to complete.
 * @result: The result code (0 => Ok, otherwise errno)
 *
 * The given request has finished, so call the necessary completion
 * if it has one and then look to see if we can start a new request
 * on the endpoint.
 *
 * Note, expects the ep to already be locked as appropriate.
 */
static void complete_request(usb_t *usb, usb_endpoint_t *usb_ep,
				usb_request_t *usb_req, int result)
{
	ENTER();
	bool restart = false;

	if (!usb_req) {
		DBG("nothing to complete?\n");
		return;
	}

	VDBG("complete: ep%d(%s) %p, req %p, result %d to complete %p\n",
		usb_ep->ept.num, usb_ep->ept.in?"in":"out",
		usb_ep, usb_req, result, usb_req->req.complete);

	/*
	 * only replace the status if we've not already set an error
	 * from a previous transaction
	 */

	if (usb_req->status == -EINPROGRESS)
		usb_req->status = result;

	usb_ep->ept.req = NULL;

	/*
	 * call the complete request with the locks off, just in case the
	 * request tries to queue more work for this endpoint.
	 */

	if (usb_req->req.complete) {
		spin_unlock(&usb->lock);
		usb_req->req.complete(&usb_req->req, usb_req->actual, usb_req->status);
		spin_lock(&usb->lock);
	}

	/*
	 * Look to see if there is anything else to do. Note, the completion
	 * of the previous request may have caused a new request to be started
	 * so be careful when doing this.
	 */
	if (!usb_ep->ept.req && result >= 0) {
		VDBG("[ep%d(%s) %p] Start check next request: req %p, result %d\n",
			usb_ep->ept.num, usb_ep->ept.in?"in":"out",
			usb_ep, usb_req, result);

		if (usb_req->next) {
			usb_request_t *next = usb_req->next;
			VDBG("orig request %p next request %p", usb_req, next);
			usb_ep->ept.req = &next->req;
			restart = true;
			VDBG("new request %p", &usb_ep->ept.req);
		} else {
			usb_ep->ept.last = NULL;
		}

		if (restart) {
			VDBG("[ep%d(%s) %p] Restart start_req\n",
				usb_ep->ept.num, usb_ep->ept.in?"in":"out", usb_ep);
			usb_req = (usb_request_t *)usb_ep->ept.req;
			VDBG("new usb_req = %p\n", usb_req);
			start_req(usb, usb_ep, usb_req, false);
		}
	}

	EXIT();
}

/**
 * handle_rx_data - receive data from the FIFO for an endpoint
 * @usb: The device state.
 * @ep_idx: The endpoint index for the data
 * @size: The size of data in the fifo, in bytes
 *
 * The FIFO status shows there is data to read from the FIFO for a given
 * endpoint, so sort out whether we need to read the data into a request
 * that has been made for that endpoint.
 */
static void handle_rx_data(usb_t *usb, int ep_idx, int size)
{
	ENTER();
	usb_endpoint_t *usb_ep = &usb->eps[ep_idx];
	udc_endpoint_t *ep = &usb_ep->ept;
	udc_request_t *req = ep->req;
	usb_request_t *usb_req = (usb_request_t *)req;
	u32 fifo = usb->reg_base + EPFIFO(ep_idx);
	int to_read;
	int max_req;
	int read_ptr;

	if (!usb_req) {
		u32 epctl = readl(usb->reg_base + DOEPCTL(ep_idx));
		int ptr;

		ERR("FIFO %d bytes on ep%d but no req (DxEPCTl=0x%08x)\n",
			size, ep_idx, epctl);

		/* dump the data from the FIFO, we've nothing we can do */
		for (ptr = 0; ptr < size; ptr += 4)
			(void)readl(fifo);

		EXIT();
		return;
	}

	to_read = size;
	read_ptr = usb_req->actual;
	max_req = usb_req->req.length - read_ptr;

	VDBG("[ep%d FIFO %x] To read %d/%d, done %d/%d at buffer %p\n",
		ep_idx, fifo,
		to_read, max_req, read_ptr, usb_req->req.length,
		usb_req->req.buffer);

	if (to_read > max_req) {
		/*
		 * more data appeared than we where willing
		 * to deal with in this request.
		 */

		/* currently we don't deal this */
		ERR("ERROR! to_read (%d) > max_req (%d)",
				to_read, max_req);
		EXIT();
		return;
	}

	usb_ep->total_data += to_read;
	usb_req->actual += to_read;
	to_read = DIV_ROUND_UP(to_read, 4);

	/*
	 * note, we might over-write the buffer end by 3 bytes depending on
	 * alignment of the data.
	 */
	ioread32_rep(fifo, usb_req->req.buffer + read_ptr, to_read);

	EXIT();
}

/*
 * send_zlp - send zero-length packet on control endpoint
 * @usb: The device instance
 * @req: The request currently on this endpoint
 *
 * Generate a zero-length IN packet request for terminating a SETUP
 * transaction.
 *
 * Note, since we don't write any data to the TxFIFO, then it is
 * currently believed that we do not need to wait for any space in
 * the TxFIFO.
 */
static void send_zlp(usb_t *usb,
			       usb_request_t *usb_req)
{
	ENTER();
	u32 ctrl;

	if (!usb_req) {
		DBG("no request?\n");
		EXIT();
		return;
	}

	if (usb_req->req.length == 0) {
		usb->eps[0].sent_zlp = 1;
		enqueue_ep0_setup_pkt(usb);
		EXIT();
		return;
	}

	usb->eps[0].dir_in = 1;
	usb->eps[0].sent_zlp = 1;

	DBG("sending zero-length packet\n");

	/* issue a zero-sized packet to terminate this */
	writel(DxEPTSIZ_MC(1) | DxEPTSIZ_PktCnt(1) |
			DxEPTSIZ_XferSize(0), usb->reg_base + DIEPTSIZ(0));

	ctrl = readl(usb->reg_base + DIEPCTL0);
	ctrl |= DxEPCTL_CNAK;  /* clear NAK set by core */
	ctrl |= DxEPCTL_EPEna; /* ensure ep enabled */
	ctrl |= DxEPCTL_USBActEp;
	writel(ctrl, usb->reg_base + DIEPCTL0);

	EXIT();
}

/**
 * handle_rx_outdone - handle receiving OutDone/SetupDone from RXFIFO
 * @usb: The device instance
 * @epnum: The endpoint received from
 * @was_setup: Set if processing a SetupDone event.
 *
 * The RXFIFO has delivered an OutDone event, which means that the data
 * transfer for an OUT endpoint	DBG("[ep%d(%s) %p] usb_req %p, actual %d, length %d, size_left %d\n",
			ep->num, ep->in?"in":"out", ep,
			usb_req->actual, req->length, size_left);

 has been completed, either by a short
 * packet or by the finish of a transfer.
 */
static void handle_rx_outdone(usb_t *usb,
				     int epnum, bool was_setup)
{
	ENTER();
	u32 epsize = readl(usb->reg_base + DOEPTSIZ(epnum));
	usb_endpoint_t *usb_ep = &usb->eps[epnum];
	udc_endpoint_t *ep = &usb_ep->ept;
	udc_request_t *req = ep->req;
	usb_request_t *usb_req = (usb_request_t *)req;
	unsigned size_left = DxEPTSIZ_XferSize_GET(epsize);
	int result = 0;

	if (!usb_req) {
		DBG("no request active\n");
		EXIT();
		return;
	}

	VDBG("[ep%d(%s) %p] usb_req %p, actual %d, length %d, size_left %d\n",
			ep->num, ep->in?"in":"out", ep,
			usb_req, usb_req->actual, req->length, size_left);

	/* if there is more request to do, schedule new transfer */
	if (usb_req->actual < req->length && size_left == 0) {
		DBG("[ep%d(%s) %p] start_req %p\n",
				ep->num, ep->in?"in":"out", ep,
				usb_req);
		start_req(usb, usb_ep, usb_req, true);
		EXIT();
		return;
	} else if (epnum == 0) {
		/*
		 * After was_setup = 1 =>
		 * set CNAK for non Setup requests
		 */
		usb->setup = was_setup ? 0 : 1;
	}

	if (usb_req->actual < req->length && usb_req->short_not_ok) {
		DBG("got %d/%d (short not ok) => error\n",
			usb_req->actual, req->length);

		/*
		 * todo - what should we return here? there's no one else
		 * even bothering to check the status.
		 */
	}

	if (epnum == 0) {
		/*
		 * Condition req->complete != complete_setup says:
		 * send ZLP when we have an asynchronous request from gadget
		 */
		if (!was_setup && req->complete != complete_setup)
			send_zlp(usb, usb_req);
	}
	complete_request(usb, usb_ep, usb_req, result);

	EXIT();
}

/**
 * turn_ep0_mps - turn max packet size into register setting
 * @mps: The maximum packet size in bytes.
 */
static u32 turn_ep0_mps(unsigned int mps)
{
	switch (mps) {
	case 64:
		return D0EPCTL_MPS_64;
	case 32:
		return D0EPCTL_MPS_32;
	case 16:
		return D0EPCTL_MPS_16;
	case 8:
		return D0EPCTL_MPS_8;
	default:
		/* bad max packet size, warn and return invalid result */
		WARN_ON(1);
		ERR("error maximum packet size %d", mps);
		return (u32)-1;
	}
}

/**
 * set_ep_maxpacket - set endpoint's max-packet field
 * @usb: The driver state.
 * @ep: The index number of the endpoint
 * @mps: The maximum packet size in bytes
 *
 * Configure the maximum packet size for the given endpoint, updating
 * the hardware control registers to reflect this.
 */
static void set_ep_maxpacket(usb_t *usb,
				       unsigned int ep, unsigned int mps)
{
	ENTER();
	usb_endpoint_t *usb_ep = &usb->eps[ep];
	u32 regs = usb->reg_base;
	u32 mpsval;
	u32 mcval;
	u32 reg;

	if (ep == 0) {
		/* EP0 is a special case */
		mpsval = turn_ep0_mps(mps);
		if (mpsval > 3)
			goto bad_mps;
		usb_ep->ept.maxpkt = mps;
		usb_ep->mc = 1;
	} else {
		mpsval = mps & DxEPCTL_MPS_MASK;
		if (mpsval > 1024)
			goto bad_mps;
		mcval = ((mps >> 11) & 0x3) + 1;
		usb_ep->mc = mcval;
		if (mcval > 3)
			goto bad_mps;
		usb_ep->ept.maxpkt = mpsval;
	}

	/*
	 * update both the in and out endpoint controldir_ registers, even
	 * if one of the directions may not be in use.
	 */

	reg = readl(regs + DIEPCTL(ep));
	reg &= ~DxEPCTL_MPS_MASK;
	reg |= mpsval;
	writel(reg, regs + DIEPCTL(ep));

	if (ep) {
		reg = readl(regs + DOEPCTL(ep));
		reg &= ~DxEPCTL_MPS_MASK;
		reg |= mpsval;
		writel(reg, regs + DOEPCTL(ep));
	}

	EXIT();
	return;

bad_mps:
	ERR("ERROR! ep%d: bad mps of %d\n", ep, mps);
}

/**
 * txfifo_flush - flush Tx FIFO
 * @usb: The driver state
 * @idx: The index for the endpoint (0..15)
 */
static void txfifo_flush(usb_t *usb, unsigned int idx)
{
	ENTER();
	int timeout;
	int val;

	writel(GRSTCTL_TxFNum(idx) | GRSTCTL_TxFFlsh,
		usb->reg_base + GRSTCTL);

	/* wait until the fifo is flushed */
	timeout = 100;

	while (1) {
		val = readl(usb->reg_base + GRSTCTL);

		if ((val & (GRSTCTL_TxFFlsh)) == 0)
			break;

		if (--timeout == 0) {
			ERR("timeout flushing fifo (GRSTCTL=%08x)\n",
				val);
		}

		udelay(1);
	}
	EXIT();
}

/*
 * trytx - check to see if anything needs transmitting
 * @usb: The driver state
 * @hs_ep: The driver endpoint to check.
 *
 * Check to see if there is a request that has data to send, and if so
 * make an attempt to write data into the FIFO.
 */
static int trytx(usb_t *usb, usb_endpoint_t *usb_ep)
{
	udc_endpoint_t *ep = &usb_ep->ept;
	usb_request_t *usb_req = (usb_request_t *)ep->req;

	if (!usb_ep->dir_in || !usb_req) {
		/**
		 * if request is not enqueued, we disable interrupts
		 * for endpoints, excepting ep0
		 */
		if (usb_ep->index != 0)
			set_epint(usb, usb_ep->index,
					     usb_ep->dir_in, 0);
		return 0;
	}

	if (usb_req->actual < usb_req->req.length) {
		DBG("trying to write more for ep%d\n",
			usb_ep->index);
		return write_fifo(usb, usb_ep, usb_req);
	}

	return 0;
}

/**
 * complete_in - complete IN transfer
 * @usb: The device state.
 * @hs_ep: The endpoint that has just completed.
 *
 * An IN transfer has been completed, update the transfer's state and then
 * call the relevant completion routines.
 */
static void complete_in(usb_t *usb,
				  usb_endpoint_t *usb_ep)
{
	ENTER();
	udc_endpoint_t *ep = &usb_ep->ept;
	usb_request_t *usb_req = (usb_request_t *)ep->req;
	u32 epsize = readl(usb->reg_base + DIEPTSIZ(usb_ep->index));
	uint size_left, size_done;

	if (!usb_req) {
		DBG("XferCompl but no req\n");
		EXIT();
		return;
	}

	/* Finish ZLP handling for IN EP0 transactions */
	if (usb->eps[0].sent_zlp) {
		DBG("zlp packet received in EP0\n");
		complete_request(usb, usb_ep, usb_req, 0);
		EXIT();
		return;
	}

	/*
	 * Calculate the size of the transfer by checking how much is left
	 * in the endpoint size register and then working it out from
	 * the amount we loaded for the transfer.
	 *
	 * We do this even for DMA, as the transfer may have incremented
	 * past the end of the buffer (DMA transfers are always 32bit
	 * aligned).
	 */

	size_left = DxEPTSIZ_XferSize_GET(epsize);

	size_done = usb_ep->size_loaded - size_left;
	size_done += usb_ep->last_load;

	if (usb_req->actual != size_done)
		DBG("adjusting size done %d => %d\n",
			usb_req->actual, size_done);

	usb_req->actual = size_done;
	DBG("req->length:%d req->actual:%d req->zero:%d\n",
		usb_req->req.length, usb_req->actual, usb_req->zero);

	/*
	 * Check if dealing with Maximum Packet Size(MPS) IN transfer at EP0
	 * When sent data is a multiple MPS size (e.g. 64B ,128B ,192B
	 * ,256B ... ), after last MPS sized packet send IN ZLP packet to
	 * inform the host that no more data is available.
	 * The state of req.zero member is checked to be sure that the value to
	 * send is smaller than wValue expected from host.
	 * Check req.length to NOT send another ZLP when the current one is
	 * under completion (the one for which this completion has been called).
	 */
	if (usb_req->req.length && usb_ep->index == 0 && usb_req->zero &&
	    usb_req->req.length == usb_req->actual &&
	    !(usb_req->req.length % usb_ep->ept.maxpkt)) {

		DBG("ep0 zlp IN packet sent\n");
		send_zlp(usb, usb_req);

		EXIT();
		return;
	}

	if (!size_left && usb_req->actual < usb_req->req.length) {
		DBG("trying more for req...\n");
		start_req(usb, usb_ep, usb_req, true);
	} else
		complete_request(usb, usb_ep, usb_req, 0);

	EXIT();
}

/**
 * handle_irq_epint - handle an in/out endpoint interrupt
 * @usb: The driver state
 * @idx: The index for the endpoint (0..15)
 * @dir_in: Set if this is an IN endpoint
 *
 * Process and clear any interrupt pending for an individual endpoint
 */
static void handle_irq_epint(usb_t *usb, unsigned int idx,
			    int dir_in)
{
	ENTER();
	usb_endpoint_t *usb_ep = &usb->eps[idx];
	udc_endpoint_t *ep = &usb_ep->ept;
	u32 epint_reg = dir_in ? DIEPINT(idx) : DOEPINT(idx);
	u32 epctl_reg = dir_in ? DIEPCTL(idx) : DOEPCTL(idx);
	u32 epsiz_reg = dir_in ? DIEPTSIZ(idx) : DOEPTSIZ(idx);
	u32 ints;
	u32 ctrl;

	ints = readl(usb->reg_base + epint_reg);
	ctrl = readl(usb->reg_base + epctl_reg);

	/* Clear endpoint interrupts */
	writel(ints, usb->reg_base + epint_reg);

	DBG("ep%d(%s) DxEPINT=0x%08x, DIEPMSK=0x%08x, DOEPMSK=0x%08x\n",
		idx, dir_in ? "in" : "out", ints,
		readl(usb->reg_base + DIEPMSK), readl(usb->reg_base + DOEPMSK));

	if (ints & DxEPINT_XferCompl) {
		if (usb_ep->isochronous && usb_ep->interval == 1) {
			if (ctrl & DxEPCTL_EOFrNum)
				ctrl |= DxEPCTL_SetEvenFr;
			else
				ctrl |= DxEPCTL_SetOddFr;
			writel(ctrl, usb->reg_base + epctl_reg);
		}

		DBG("XferCompl: DxEPCTL=0x%08x, DxEPTSIZ=%08x\n",
			readl(usb->reg_base + epctl_reg),
			readl(usb->reg_base + epsiz_reg));

		/*
		 * we get OutDone from the FIFO, so we only need to look
		 * at completing IN requests here
		 */
		if (dir_in) {
			complete_in(usb, usb_ep);

			if (idx == 0 && !ep->req)
				enqueue_ep0_setup_pkt(usb);
		}
	}

	if (ints & DxEPINT_EPDisbld) {
		DBG("EPDisbld\n");

		if (dir_in) {
			int epctl = readl(usb->reg_base + epctl_reg);

			txfifo_flush(usb, idx);

			if ((epctl & DxEPCTL_Stall) &&
				(epctl & DxEPCTL_EPType_Bulk)) {
				int dctl = readl(usb->reg_base + DCTL);

				dctl |= DCTL_CGNPInNAK;
				writel(dctl, usb->reg_base + DCTL);
			}
		}
	}

	if (ints & DxEPINT_AHBErr)
		DBG("AHBErr\n");

	if (ints & DxEPINT_Setup) {  /* Setup or Timeout */
		DBG("Setup/Timeout\n");
	}

	if (ints & DxEPINT_Back2BackSetup)
		DBG("B2BSetup/INEPNakEff\n");

	if (dir_in && !usb_ep->isochronous) {
		/* not sure if this is important, but we'll clear it anyway */
		if (ints & DIEPMSK_INTknTXFEmpMsk) {
			DBG("ep%d: INTknTXFEmpMsk\n", idx);
		}

		/* this probably means something bad is happening */
		if (ints & DIEPMSK_INTknEPMisMsk) {
			ERR("ep%d: INTknEP\n", idx);
		}

		/* FIFO has space or is empty (see GAHBCFG) */
		if (usb->dedicated_fifos &&
		    ints & DIEPMSK_TxFIFOEmpty) {
			DBG("ep%d: TxFIFOEmpty\n", idx);
			trytx(usb, usb_ep);
		}
	}
	EXIT();
}

/**
 * handle_irq_enumdone - Handle EnumDone interrupt (enumeration done)
 * @usb: The device state.
 *
 * Handle updating the device settings after the enumeration phase has
 * been completed.
 */
static void handle_irq_enumdone(usb_t *usb)
{
	ENTER();
	u32 dsts = readl(usb->reg_base + DSTS);
	int ep0_mps = 0, ep_mps;

	/*
	 * This should signal the finish of the enumeration phase
	 * of the USB handshaking, so we should now know what rate
	 * we connected at.
	 */

	DBG("EnumDone (DSTS=0x%08x)\n", dsts);

	/*
	 * note, since we're limited by the size of transfer on EP0, and
	 * it seems IN transfers must be a even number of packets we do
	 * not advertise a 64byte MPS on EP0.
	 */

	/* catch both EnumSpd_FS and EnumSpd_FS48 */
	switch (dsts & DSTS_EnumSpd_MASK) {
	case DSTS_EnumSpd_FS:
	case DSTS_EnumSpd_FS48:
		usb->speed = USB_SPEED_FULL;
		usb->speed = USB_SPEED_FULL;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 1023;
		//ep_mps = 64;
		break;

	case DSTS_EnumSpd_HS:
		usb->speed = USB_SPEED_HIGH;
		ep0_mps = EP0_MPS_LIMIT;
		ep_mps = 1024;
		//ep_mps = 512;
		break;

	case DSTS_EnumSpd_LS:
		usb->speed = USB_SPEED_LOW;
		/*
		 * note, we don't actually support LS in this driver at the
		 * moment, and the documentation seems to imply that it isn't
		 * supported by the PHYs on some of the devices.
		 */
		break;
	}
	INF("new device is %s\n",
		 usb_speed_string(usb->speed));

	/*
	 * we should now know the maximum packet size for an
	 * endpoint, so set the endpoints to a default value.
	 */

	if (ep0_mps) {
		int i;
		set_ep_maxpacket(usb, 0, ep0_mps);
		for (i = 1; i < usb->num_of_eps; i++)
			set_ep_maxpacket(usb, i, ep_mps);
	}

	/* ensure after enumeration our EP0 is active */
	enqueue_ep0_setup_pkt(usb);

	DBG("EP0: DIEPCTL0=0x%08x, DOEPCTL0=0x%08x\n",
		readl(usb->reg_base + DIEPCTL0),
		readl(usb->reg_base + DOEPCTL0));
	EXIT();
}

/**
 * kill_all_requests - remove all requests from the endpoint's queue
 * @usb: The device state.
 * @ep: The endpoint the requests may be on.
 * @result: The result code to use.
 * @force: Force removal of any current requests
 *
 * Go through the requests on the given endpoint and mark them
 * completed with the given result code.
 */
static void kill_all_requests(usb_t *usb,
			      usb_endpoint_t *usb_ep,
			      int result, bool force)
{
	ENTER();
	usb_request_t *usb_req = NULL;
	udc_request_t *req = NULL;
	if (usb_ep == NULL) {
		ERR("Error! usb_ep == NULL\n");
		return;
	}
	udc_endpoint_t *ep = &usb_ep->ept;

	if (ep == NULL) {
		ERR("Error! ep == NULL\n");
		return;
	}

	if (ep->req) {
		req = (udc_request_t *)ep->req;
		usb_req = (usb_request_t *)req;
	}
	DBG("[ep%d(%s) %p] usb_req %p\n",
		ep->num, ep->in?"in":"out", ep, usb_req);

	while (req) {
		/*
		 * currently, we can't do much about an already
		 * running request on an in endpoint
		 */
		//TODO fixed me
		if (ep->req == req && ep->in && !force)
			continue;

		complete_request(usb, usb_ep, usb_req,
					   result);

		req = (udc_request_t *)usb_req->next;
		usb_req = (usb_request_t *)req;

		DBG("[ep%d(%s) %p] usb_req %p\n",
			ep->num, ep->in?"in":"out", ep, usb_req);
	}
	if(usb->dedicated_fifos)
		if ((readl(usb->reg_base + DTXFSTS(usb_ep->index)) & 0xffff) * 4 < 3072)
			txfifo_flush(usb, usb_ep->index);
	EXIT();
}

/*
 * disconnect - disconnect service
 * @usb: The device state.
 *
 * The device has been disconnected. Remove all current
 * transactions and signal the gadget driver that this
 * has happened.
 */
static void disconnect(usb_t *usb)
{
	ENTER();
	unsigned ep;

	gadget_disconnect(usb->driver, usb->gadget);

	for (ep = 0; ep < (unsigned)usb->num_of_eps + 1; ep++) {
		if (&usb->eps[ep]) {
			kill_all_requests(usb, &usb->eps[ep], -ESHUTDOWN, true);
		}
	}
	usb->ept_alloc_table = 0;

	EXIT();
}

/**
 * handle_irq_fifoempty - TX FIFO empty interrupt handler
 * @usb: The device state:
 * @periodic: True if this is a periodic FIFO interrupt
 */
static void handle_irq_fifoempty(usb_t *usb, bool periodic)
{
	usb_endpoint_t *ep;
	int epno, ret;

	/* look through for any more data to transmit */

	for (epno = 0; epno < usb->num_of_eps; epno++) {
		ep = &usb->eps[epno];

		if (!ep->dir_in)
			continue;

		if ((periodic && !ep->periodic) ||
		    (!periodic && ep->periodic))
			continue;

		ret = trytx(usb, ep);
		if (ret < 0)
			break;
	}
}

static int usb2_core_reset(usb_t *usb);

static void usb_enable(usb_t *usb)
{
	ENTER();

	usb2_phy_switch_to_device(usb->usb2phy);

	usb2_core_reset(usb);

	/*
	 * we must now enable ep0 ready for host detection and then
	 * set configuration.
	 */

	/* set the PLL on, remove the HNP/SRP and set the PHY */
	writel(usb->phyif | GUSBCFG_TOutCal(7) |
			(0x5 << 10), usb->reg_base + GUSBCFG);
	mdelay(100);

	init_fifo(usb);

	write_enable(DCTL_SftDiscon, usb->reg_base + DCTL);

	writel(1 << 18 | DCFG_DevSpd_HS,  usb->reg_base + DCFG);

	/* Clear any pending OTG interrupts */
	writel(0xffffffff, usb->reg_base + GOTGINT);

	/* Clear any pending interrupts */
	writel(0xffffffff, usb->reg_base + GINTSTS);

	writel(GINTSTS_ErlySusp | GINTSTS_SessReqInt |
			GINTSTS_GOUTNakEff | GINTSTS_GINNakEff |
			GINTSTS_ConIDStsChng | GINTSTS_USBRst |
			GINTSTS_EnumDone | GINTSTS_OTGInt |
			GINTSTS_USBSusp | GINTSTS_WkUpInt,
			usb->reg_base + GINTMSK);

	writel(((usb->dedicated_fifos) ? (GAHBCFG_NPTxFEmpLvl |
			GAHBCFG_PTxFEmpLvl) : 0) |
			GAHBCFG_GlblIntrEn,
			usb->reg_base + GAHBCFG);

	/*
	 * If INTknTXFEmpMsk is enabled, it's important to disable ep interrupts
	 * when we have no data to transfer. Otherwise we get being flooded by
	 * interrupts.
	 */
	writel(((usb->dedicated_fifos) ? DIEPMSK_TxFIFOEmpty |
			DIEPMSK_INTknTXFEmpMsk : 0) |
			DIEPMSK_EPDisbldMsk | DIEPMSK_XferComplMsk |
			DIEPMSK_TimeOUTMsk | DIEPMSK_AHBErrMsk |
			DIEPMSK_INTknEPMisMsk,
			usb->reg_base + DIEPMSK);

	/*
	 * don't need XferCompl, we get that from RXFIFO in slave mode. In
	 * DMA mode we may need this.
	 */
	writel(DOEPMSK_EPDisbldMsk | DOEPMSK_AHBErrMsk |
			DOEPMSK_SetupMsk,
			usb->reg_base + DOEPMSK);

	writel(0, usb->reg_base + DAINTMSK);

	DBG("EP0 DIEPCTL0=0x%08x, DOEPCTL0=0x%08x\n",
		readl(usb->reg_base + DIEPCTL0),
		readl(usb->reg_base + DOEPCTL0));

	/* enable in and out endpoint interrupts */
	en_gsint(usb, GINTSTS_OEPInt | GINTSTS_IEPInt);

	/*
	 * Enable the RXFIFO when in slave mode, as this is how we collect
	 * the data. In DMA mode, we get events from the FIFO but also
	 * things we cannot process, so do not use it.
	 */
	en_gsint(usb, GINTSTS_RxFLvl);

	/* Enable interrupts for EP0 in and out */
	set_epint(usb, 0, 0, 1);
	set_epint(usb, 0, 1, 1);

	write_enable(DCTL_PWROnPrgDone, usb->reg_base + DCTL);
	udelay(10);  /* see openiboot */
	write_disable(DCTL_PWROnPrgDone, usb->reg_base + DCTL);

	DBG("DCTL=0x%08x\n", readl(usb->reg_base + DCTL));

	/*
	 * DxEPCTL_USBActEp says RO in manual, but seems to be set by
	 * writing to the EPCTL register..
	 */

	/* set to read 1 8byte packet */
	writel(DxEPTSIZ_MC(1) | DxEPTSIZ_PktCnt(1) |
			DxEPTSIZ_XferSize(8), usb->reg_base + DOEPTSIZ0);

	writel(turn_ep0_mps(usb->ep0out->maxpkt) |
			DxEPCTL_CNAK | DxEPCTL_EPEna |
			DxEPCTL_USBActEp,
			usb->reg_base + DOEPCTL0);

	/* enable, but don't activate EP0in */
	writel(turn_ep0_mps(usb->ep0in->maxpkt) |
			DxEPCTL_USBActEp, usb->reg_base + DIEPCTL0);

	enqueue_ep0_setup_pkt(usb);

	DBG("EP0: DIEPCTL0=0x%08x, DOEPCTL0=0x%08x\n",
		readl(usb->reg_base + DIEPCTL0),
		readl(usb->reg_base + DOEPCTL0));

	/* clear global NAKs */
	writel(DCTL_CGOUTNak | DCTL_CGNPInNAK,
	       usb->reg_base + DCTL);

	/* must be at-least 3ms to allow bus to see disconnect */
	mdelay(3);

	/* remove the soft-disconnect and let's go */
	write_disable(DCTL_SftDiscon, usb->reg_base + DCTL);

	DBG("DCTL=0x%08x, DSTS=0x%08x\n",
		readl(usb->reg_base + DCTL),
		readl(usb->reg_base + DSTS));
	EXIT();
}

static void usb_disable(usb_t *usb) {
	ENTER();
	int i;
	INF("usb_disable EPs:%d\n", usb->num_of_eps);

	/* all endpoints should be shutdown */
	for (i = 1; i < usb->num_of_eps; i++) {
		ep_disable(&usb->eps[i].ept);
	}

	usb->speed = USB_SPEED_UNKNOWN;

	DBG("usb_disable disable irq\n");
	/* Clear any pending OTG interrupts */
	writel(0xffffffff, usb->reg_base + GOTGINT);

	/* Clear any pending interrupts */
	writel(0xffffffff, usb->reg_base + GINTSTS);

	writel(0, usb->reg_base + GINTMSK);

	EXIT();
}

/* IRQ flags which will trigger a retry around the IRQ loop */
#define IRQ_RETRY_MASK (GINTSTS_NPTxFEmp | \
						GINTSTS_PTxFEmp |  \
						GINTSTS_RxFLvl)

enum handler_return rtk_otg_udc_irq(void *arg) {
	ENTER();
	usb_t *usb = &rtk_otg;
	int retry_count = 8;
	u32 gintsts;
	u32 gintmsk;

	spin_lock(&usb->lock);

irq_retry:
	gintsts = readl(usb->reg_base + GINTSTS);
	gintmsk = readl(usb->reg_base + GINTMSK);

	if (gintmsk & gintsts)
		VDBG("GINTSTS: 0x%08x GINTMSK: 0x%08x --> 0x%08x retry %d\n",
			gintsts, gintmsk, gintsts & gintmsk, retry_count);

	gintsts &= gintmsk;

	if (!gintsts) {
		spin_unlock(&usb->lock);
		EXIT();
		return INT_NO_RESCHEDULE;
	}

	if (gintsts & GINTSTS_OTGInt) {
		u32 otgint = readl(usb->reg_base + GOTGINT);
		VDBG("OTGInt: %08x\n", otgint);
		writel(otgint, usb->reg_base + GOTGINT);
	}

	if (gintsts & GINTSTS_SessReqInt) {
		VDBG("SessReqInt\n");
		writel(GINTSTS_SessReqInt, usb->reg_base + GINTSTS);
	}

	if (gintsts & GINTSTS_EnumDone) {
		VDBG("EnumDone\n");
		writel(GINTSTS_EnumDone, usb->reg_base + GINTSTS);

		handle_irq_enumdone(usb);
	}

	if (gintsts & GINTSTS_ConIDStsChng) {
		VDBG("ConIDStsChg (DSTS=0x%08x, GOTCTL=%08x)\n",
			readl(usb->reg_base + DSTS),
			readl(usb->reg_base + GOTGCTL));

		writel(GINTSTS_ConIDStsChng, usb->reg_base + GINTSTS);
	}

	if (gintsts & (GINTSTS_OEPInt | GINTSTS_IEPInt)) {
		u32 daint = readl(usb->reg_base + DAINT);
		u32 daintmsk = readl(usb->reg_base + DAINTMSK);
		u32 daint_out, daint_in;
		int ep;

		daint &= daintmsk;
		daint_out = daint >> DAINT_OutEP_SHIFT;
		daint_in = daint & ~(daint_out << DAINT_OutEP_SHIFT);

		VDBG("DAINT=0x%08x, DAINTMSK=0x%08x\n", daint, daintmsk);

		for (ep = 0; ep < 15 && daint_out; ep++, daint_out >>= 1) {
			if (daint_out & 1)
				handle_irq_epint(usb, ep, 0);
		}

		for (ep = 0; ep < 15 && daint_in; ep++, daint_in >>= 1) {
			if (daint_in & 1)
				handle_irq_epint(usb, ep, 1);
		}
	}

	if (gintsts & GINTSTS_USBRst) {
		VDBG("USBRst\n");

		u32 usb_status = readl(usb->reg_base + GOTGCTL);

		writel(GINTSTS_USBRst, usb->reg_base + GINTSTS);

		if (usb_status & GOTGCTL_BSESVLD) {
			TODO_FOR("GOTGCTL_BSESVLD");
		}
	}

	/* check both FIFOs */

	if (gintsts & GINTSTS_NPTxFEmp) {
		VDBG("NPTxFEmp\n");

		/*
		 * Disable the interrupt to stop it happening again
		 * unless one of these endpoint routines decides that
		 * it needs re-enabling
		 */
		disable_gsint(usb, GINTSTS_NPTxFEmp);
		handle_irq_fifoempty(usb, false);
	}

	if (gintsts & GINTSTS_PTxFEmp) {
		VDBG("PTxFEmp\n");

		/* See note in GINTSTS_NPTxFEmp */

		disable_gsint(usb, GINTSTS_PTxFEmp);
		handle_irq_fifoempty(usb, true);
	}

	if (gintsts & GINTSTS_RxFLvl) {
		VDBG("GINTSTS_RxFLvl\n");
		/*
		 * note, since GINTSTS_RxFLvl doubles as FIFO-not-empty,
		 * we need to retry handle_irq_rx if this is still
		 * set.
		 */
		handle_irq_rx(usb);
	}

	if (gintsts & GINTSTS_ModeMis) {
		ERR("warning, mode mismatch triggered\n");
		writel(GINTSTS_ModeMis, usb->reg_base + GINTSTS);
	}

	if (gintsts & GINTSTS_USBSusp) {
		VDBG("GINTSTS_USBSusp\n");
		writel(GINTSTS_USBSusp, usb->reg_base + GINTSTS);
	}

	if (gintsts & GINTSTS_WkUpInt) {
		VDBG("GINTSTS_WkUpIn\n");
		writel(GINTSTS_WkUpInt, usb->reg_base + GINTSTS);
	}

	if (gintsts & GINTSTS_ErlySusp) {
		VDBG("GINTSTS_ErlySusp\n");
		writel(GINTSTS_ErlySusp, usb->reg_base + GINTSTS);
	}

	/*
	 * these next two seem to crop-up occasionally causing the core
	 * to shutdown the USB transfer, so try clearing them and logging
	 * the occurrence.
	 */
	if (gintsts & GINTSTS_GOUTNakEff) {
		VDBG("GOUTNakEff triggered\n");
		writel(DCTL_CGOUTNak, usb->reg_base + DCTL);
	}

	if (gintsts & GINTSTS_GINNakEff) {
		VDBG("GINNakEff triggered\n");
		writel(DCTL_CGNPInNAK, usb->reg_base + DCTL);
	}

	/*
	 * if we've had fifo events, we should try and go around the
	 * loop again to see if there's any point in returning yet.
	 */

	if (gintsts & IRQ_RETRY_MASK && --retry_count > 0)
		goto irq_retry;

	spin_unlock(&usb->lock);

	EXIT();
	return INT_RESCHEDULE;
}

static void rtk_usb2_phy_init(usb_t *usb) {
	ENTER();

	/* Set default UTMI width */
	usb->phyif = GUSBCFG_PHYIf16|GUSBCFG_FORCEDEVMODE;

	/* init usb2 phy */
	usb2_phy_init(usb->usb2phy);

	/* enable device mode on phy */
	usb2_phy_switch_to_device(usb->usb2phy);

	EXIT();
}

static int usb2_core_reset(usb_t *usb) {
	ENTER();
	int timeout;
	u32 grstctl;

	DBG("resetting core\n");

	/* issue soft reset */
	writel(GRSTCTL_CSftRst, usb->reg_base + GRSTCTL);

	timeout = 10000;
	do {
		grstctl = readl(usb->reg_base + GRSTCTL);
	} while ((grstctl & GRSTCTL_CSftRst) && timeout-- > 0);

	if (grstctl & GRSTCTL_CSftRst) {
		ERR("Failed to get CSftRst asserted\n");
		return -EINVAL;
	}

	timeout = 10000;

	while (1) {
		u32 grstctl = readl(usb->reg_base + GRSTCTL);

		if (timeout-- < 0) {
			ERR("reset failed, GRSTCTL=%08x\n",
				grstctl);
			return -ETIMEDOUT;
		}

		if (!(grstctl & GRSTCTL_AHBIdle))
			continue;

		break; /* reset done */
	}

	DBG("reset successful\n");

	EXIT();
	return 0;
}

static void usb2_device_init(usb_t *usb) {
	ENTER();

	/* unmask subset of endpoint interrupts */
	writel(DIEPMSK_TimeOUTMsk | DIEPMSK_AHBErrMsk |
			DIEPMSK_EPDisbldMsk | DIEPMSK_XferComplMsk,
			usb->reg_base + DIEPMSK);

	writel(DOEPMSK_SetupMsk | DOEPMSK_AHBErrMsk |
			DOEPMSK_EPDisbldMsk | DOEPMSK_XferComplMsk,
			usb->reg_base + DOEPMSK);

	writel(0, usb->reg_base + DAINTMSK);

	/* Be in disconnected state until gadget is registered */
	write_enable(DCTL_SftDiscon, usb->reg_base + DCTL);

	if (0) {
		/* post global nak until we're ready */
		writel(DCTL_SGNPInNAK | DCTL_SGOUTNak,
	       usb->reg_base + DCTL);
	}

	/* setup fifos */

	DBG("GRXFSIZ=0x%08x, GNPTXFSIZ=0x%08x\n",
		readl(usb->reg_base + GRXFSIZ),
		readl(usb->reg_base + GNPTXFSIZ));

	init_fifo(usb);

	/* set the PLL on, remove the HNP/SRP and set the PHY */
	writel(GUSBCFG_PHYIf16 | GUSBCFG_TOutCal(7) | (0x5 << 10),
		usb->reg_base + GUSBCFG);
	//writel(hsotg->phyif | GUSBCFG_TOutCal(7) | (0x5 << 10),>>---//barry
	//       hsotg->regs + GUSBCFG);
	mdelay(100);

	writel(0x0, usb->reg_base + GAHBCFG);
	EXIT();
}

static void usb2_hw_cfg(usb_t *usb) {
	ENTER();
	u32 cfg2, cfg4;
	/* check hardware configuration */
	cfg2 = readl(usb->reg_base + 0x48);
	usb->num_of_eps = (cfg2 >> 10) & 0xF;
	DBG("EPs:%d\n", usb->num_of_eps);

	cfg4 = readl(usb->reg_base + 0x50);
	usb->dedicated_fifos = (cfg4 >> 25) & 1;
	DBG("%s fifos\n",
		usb->dedicated_fifos ? "dedicated" : "shared");

	EXIT();
}
// ---- UDC API

/* udc_driver's udc_init callback*/
int rtk_otg_udc_init(struct udc_device *dev)
{
	ENTER();
	usb_t *usb = &rtk_otg;
	usb_endpoint_t *eps;
	int epnum, ret = 0;

	DBG("start low level HW init\n");
	rtk_usb2_phy_init(usb);
	usb2_core_reset(usb);
	usb2_device_init(usb);
	usb2_hw_cfg(usb);

	usb->flags |= F_LL_INIT; //finish low level init

	DBG("init endpoint\n");

	eps = malloc((usb->num_of_eps + 1) * sizeof(usb_endpoint_t));
	if (!eps) {
		ERR("cannot get memory\n");
		ret = -ENOMEM;
		return ret;
	}
	memset(eps, 0, (usb->num_of_eps + 1) * sizeof(usb_endpoint_t));

	usb->eps = eps;

	for (epnum = 0; epnum < usb->num_of_eps; epnum++)
		__init_ep(usb, &usb->eps[epnum], epnum);

	usb->device = dev;
	usb->ep0out = &usb->eps[0].ept;
	usb->ep0in = &usb->eps[0].ept;
	usb->ep0_rx_req = rtk_otg_udc_request_alloc();
	usb->ep0_rx_req->context = usb;
	usb->flags |= F_UDC_INIT; //finish udc init

	dump_register(usb);

	EXIT();
    return 0;
}

int rtk_otg_udc_register_gadget(struct udc_gadget *gadget) {
	ENTER();
	usb_t *usb = &rtk_otg;
	usb->gadget = gadget;

	DBG("gadget %p\n", gadget);
	EXIT();
	return 0;
}

int rtk_otg_udc_start(void)
{
	ENTER();
	usb_t *usb = &rtk_otg;

	if (!(usb->flags & F_LL_INIT)) {
		panic("udc cannot start before hw init\n");
	}
	if (!usb->device) {
		panic("udc cannot start before init\n");
	}
	if (!usb->gadget) {
		panic("udc has no gadget registered\n");
	}

	usb_enable(usb);

	dump_register(usb);

	EXIT();
    return 0;
}

int rtk_otg_udc_stop(void)
{
	ENTER();
	usb_t *usb = &rtk_otg;

	usb_disable(usb);
	thread_sleep(10);

	EXIT();
	return 0;
}

/* Add udc_driver to udc_core */
static struct udc_driver rtk_otg_udc_driver = {
	.name = "rtk-otg",
	.udc_init  = rtk_otg_udc_init,
	.udc_register_gadget = rtk_otg_udc_register_gadget,
    .udc_start = rtk_otg_udc_start,
	.udc_stop  = rtk_otg_udc_stop,

	.udc_endpoint_alloc = rtk_otg_udc_endpoint_alloc,
	.udc_endpoint_free  = rtk_otg_udc_endpoint_free,

	.udc_request_alloc  = rtk_otg_udc_request_alloc,
	.udc_request_free  = rtk_otg_udc_request_free,

	.udc_request_queue  = rtk_otg_udc_request_queue,
	.udc_request_cancel = rtk_otg_udc_request_dequeue,

	.udc_irq = rtk_otg_udc_irq

};

void rtk_otg_init(u32 reg_base, struct usb2_phy *usb2phy) {
	ENTER();
	usb_t *usb = &rtk_otg;

	usb->reg_base = reg_base;

	usb->usb2phy = usb2phy;

	usb->driver = &rtk_otg_udc_driver;

	udc_core_add_udc_driver(usb->driver);
	EXIT();
}

void rtk_otg_stop(void) {
	ENTER();

	EXIT();
}
