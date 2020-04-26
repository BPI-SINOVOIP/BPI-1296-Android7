
#include <dev/udc.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"
#include "dwc3.h"

#undef log_level
#define log_level 3


extern dwc3_device_req_t	g_dev_req[];
extern u32			g_dev_req_bm;

extern dwc3_dma_desc_t	g_out_trb_pool[][DWC_NUM_ISOC_TRBS + 1];
extern dwc3_dma_desc_t	g_in_trb_pool[][DWC_NUM_ISOC_TRBS + 1];

/******************************************************************************
 * Function Driver notification routines
 *
 * These routines receive notifications from the PCD when certain events occur
 * which the Function Driver may need to be aware of.
 *
 * These routines *must* have the exact names and parameters shown here,
 * because they are part of the API between the PCD and the Function Driver.
 ******************************************************************************/

int dwc3_gadget_connect(dwc3_device_t *dwc3_dev, int speed)
{
	DBG("%s()\n", __func__);
	dwc3_core_t *dwc3 = dwc3_dev->dwc3;
	usb_gadget_connect(dwc3);
	return 0;
}

int dwc3_gadget_disconnect(dwc3_device_t *dwc3_dev)
{
	DBG("%s()\n", __func__);
	dwc3_core_t *dwc3 = dwc3_dev->dwc3;
	usb_gadget_disconnect(dwc3);
	return 0;
}

/**
 * This routine receives Suspend notifications from the PCD
 *
 * @param pcd   Programming view of DWC_usb3 peripheral controller.
 * @return      0 for success, else negative error code.
 */
int dwc3_gadget_suspend(dwc3_device_t *dwc3_dev)
{
	DBG("%s()\n", __func__);
	return 0;
}

/**
 * This routine receives Resume notifications from the PCD
 *
 * @param pcd   Programming view of DWC_usb3 peripheral controller.
 * @return      0 for success, else negative error code.
 */
int dwc3_gadget_resume(dwc3_device_t *dwc3_dev)
{
	DBG("%s()\n", __func__);
	return 0;
}

usb_endpoint_t *dwc3_ep_enable(struct dwc3_core *dwc3, const void *epdesc, const void *epcomp);

/**
 * This routine receives Setup request notifications from the PCD
 *
 * @param pcd   Programming view of DWC_usb3 peripheral controller.
 * @param ctrl  Pointer to the Setup packet for the request.
 * @return      0 for success, else negative error code.
 */
int dwc3_gadget_setup(dwc3_device_t *dwc3_dev, usb_device_request_t *ctrl)
{
	//u8 desc_type = UGETW(ctrl->wValue) >> 8;
	//u8 desc_idx = UGETW(ctrl->wValue);
	u16 max = UGETW(ctrl->wLength);
	u16 len = 0;
	int ret = 0;

	DBG("%s()\n", __func__);

	if ((UT_GET_TYPE(ctrl->bmRequestType)) != UT_STANDARD) {
		/* handle non-standard (class/vendor) requests
		 * in the Function Driver
		 */
		TODO_FOR("handle non-standard (class/vendor) request\n");
		return -1;
	}

///////////////////////////////////////////
//// --- Standard Request handling --- ////

	switch (ctrl->bRequest) {
	case UR_SET_FEATURE:
		TODO_FOR("UR_SET_FEATURE\n");

	break;
	case UR_SET_INTERFACE:
		TODO_FOR("UR_SET_INTERFACE\n");

	break;
	case UR_GET_INTERFACE:
		TODO_FOR("UR_GET_INTERFACE\n");

	break;
	case UR_SET_CONFIG:
		if (UGETW(ctrl->wValue) == 1) {
			struct udc_descriptor *desc = udc_descriptor_find(TYPE_CONFIGURATION << 8);
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
				usb_ept_desc.bLength = data[0];
				usb_ept_desc.bDescriptorType = data[1];
				usb_ept_desc.bEndpointAddress = data[2];
				usb_ept_desc.bmAttributes = data[3];
				usb_ept_desc.wMaxPacketSize = data[5] << 8 | data[4];
				usb_ept_desc.bInterval = data[6];
				usb_ept_desc.bRefresh = data[7];

				dwc3_ep_enable(dwc3_dev->dwc3, &usb_ept_desc, NULL);

				data = data + 7;
			}
			dwc3_gadget_connect(dwc3_dev, 0);
		}
		dwc3_dev->ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
	break;
	case UR_GET_CONFIG:
		TODO_FOR("UR_GET_CONFIG\n");

	break;
	case UR_SYNCH_FRAME:
		TODO_FOR("UR_SYNCH_FRAME\n");

	break;
	case UR_GET_DESCRIPTOR:
		DBG("setup DEVICE_READ GET_DESCRIPTOR\n");
		struct udc_descriptor *desc = udc_descriptor_find(UGETW(ctrl->wValue));
		if (desc) {
			memcpy(dwc3_dev->ep0_status_buf, desc->data, desc->len);
			len = desc->len;
		}

		dwc3_dev->ep0state = EP0_IN_DATA_PHASE;
		dwc3_dev->ep0_req->dwc_req.length = len < max ? len : max;
		dwc3_dev->ep0_status_pending = 1;
		dwc3_dev->ep0_req->dwc_req.buf[0] = (void *)dwc3_dev->ep0_status_buf;
		dwc3_dev->ep0_req->dwc_req.bufdma[0] = dwc3_dev->ep0_status_buf_dma;
		dwc3_dev->ep0_req->dwc_req.actual = 0;
		dwc3_device_ep0_start_transfer(dwc3_dev, dwc3_dev->ep0_req);

	break;
	default:
		/* Call the Function Driver setup routines */
		TODO_FOR("Call Function Driver setup routines\n");
	}

	return ret;
}

/**
 * This routine receives Transfer Complete notifications from the PCD
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        PCD EP for the transfer.
 * @param pcd_req       PCD request for the transfer.
 * @param status        Transfer status, 0 for success else negative error code.
 * @return              0 for success, else negative error code.
 */
int dwc3_gadget_complete(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep,
			     dwc3_device_req_t *dev_req, int status)
{
	//usb_endpoint_t	*usb_ep = &dev_ep->usb_ep;
	usb_request_t	*usb_req = &dev_req->usb_req;
	u32		actual = dev_req->dwc_req.actual;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

	/* Remove the request from the queue */
	DWC_CIRCLEQ_REMOVE_INIT(&dev_ep->dwc_ep.queue, dev_req, entry);

	if (usb_req->complete) {
		usb_req->complete(usb_req, actual, status);
	}

	if (dwc3_dev->request_pending > 0)
		--dwc3_dev->request_pending;

	return 0;
}

/**
 * This routine allows overriding the standard Control transfer handling
 * (currently only done by the axs101 test gadget)
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_gadget_do_setup(dwc3_device_t *dwc3_dev)
{
	/* Call the standard Control transfer handler */
	dwc3_device_do_setup(dwc3_dev);
}

/******************************************************************************
 * Function Driver callback routines
 *
 * The PCD calls these routines when it needs something from the Function
 * Driver.
 *
 * These routines *must* have the exact names and parameters shown here,
 * because they are part of the API between the PCD and the Function Driver.
 ******************************************************************************/

/**
 * This routine allocates coherent DMA memory. It is used by the PCD to
 * allocate memory for TRBs. The block of memory returned must have a start
 * address aligned to a 16-byte boundary.
 *
 * This routine may be called from interrupt context, so it must be able to
 * allocate coherent DMA memory in that context. One strategy would be to
 * preallocate a block of memory at initialization time, and hand out chunks
 * from that block in this routine.
 *
 * @param pcd_ep        PCD EP that memory block will be associated with.
 * @param size          Size of memory block to allocate, in bytes.
 * @param mem_dma_ret   Physical address of allocated memory block is returned
 *                      through this pointer.
 * @return              Address of allocated memory block, or NULL if allocation
 *                      fails.
 */
void *dwc3_gadget_alloc_dma(dwc3_device_ep_t *dev_ep, int size, dwc_dma_t *mem_dma_ret)
{
	void		*mem;
	dwc_dma_t	mem_dma;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

	if (!dev_ep->dwc_ep.num) {
		ERR("Error! USB epnum is 0 for dev_ep\n");
		return NULL;
	}

	if (dev_ep->dwc_ep.is_in)
		mem = (void *)&g_in_trb_pool[dev_ep->dwc_ep.num - 1];
	else
		mem = (void *)&g_out_trb_pool[dev_ep->dwc_ep.num - 1];

	mem_dma = (dwc_dma_t)mem;

	*mem_dma_ret = mem_dma;

	return mem;
}

/**
 * This routine frees DMA memory allocated by dwc_usb3_gadget_alloc_dma().
 *
 * @param pcd_ep        PCD EP that memory block is associated with.
 * @param size          Size of memory block to free, in bytes.
 * @param mem           Address of memory block.
 * @param mem_dma       Physical address of memory block.
 */
void dwc3_gadget_free_dma(dwc3_device_ep_t *dev_ep, int size, void *mem, dwc_dma_t mem_dma)
{
	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

}

/**
 * This routine returns the PCD request corresponding to the current transfer
 * request for an endpoint. The current transfer request is the first request
 * submitted that has not been completed yet.
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        PCD EP to operate on.
 * @return              Pointer to PCD request, or NULL if no request available.
 */
dwc3_device_req_t *dwc3_gadget_get_request(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep)
{
	dwc3_device_req_t *dev_req;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

	/* Check for a pending request */
	if (DWC_CIRCLEQ_EMPTY(&dev_ep->dwc_ep.queue)) {
		DBG("ep->dwc_ep.queue empty\n");
		return NULL;
	}

	dev_req = DWC_CIRCLEQ_FIRST(&dev_ep->dwc_ep.queue);
	return dev_req;
}

/**
 * This routine checks to see if there is another transfer request waiting
 * on an endpoint that has not been started yet. If so then that transfer is
 * started.
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        PCD EP to operate on.
 */
void dwc3_gadget_start_next_request(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep)
{
	dwc3_device_req_t *dev_req = NULL;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

	if (DWC_CIRCLEQ_EMPTY(&dev_ep->dwc_ep.queue)) {
		DBG("start_next EP%d-%s: queue empty\n",
			   dev_ep->dwc_ep.num, dev_ep->dwc_ep.is_in ? "IN" : "OUT");
		return;
	}

	DWC_CIRCLEQ_FOREACH(dev_req, &dev_ep->dwc_ep.queue, entry) {
		if (!(dev_req->dwc_req.flags & DWC_PCD_REQ_STARTED)) {
			DBG("start_next EP%d-%s: OK\n",
				   dev_ep->dwc_ep.num, dev_ep->dwc_ep.is_in ? "IN" : "OUT");

			/* Setup and start the transfer */
			dwc3_device_fill_trbs(dwc3_dev, dev_ep, dev_req);
			dwc3_device_ep_start_transfer(dwc3_dev, dev_ep, dev_req, 0);
			return;
		}
	}

	DBG("start_next EP%d-%s: all req active\n",
		   dev_ep->dwc_ep.num, dev_ep->dwc_ep.is_in ? "IN" : "OUT");
}

/**
 * Start an Isoc EP running at the proper interval, after receiving the initial
 * XferNrdy event.
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        EP to operate on.
 * @param event         Event data containing the XferNrdy microframe.
 */
void dwc3_gadget_isoc_ep_start(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep, u32 event)
{
	dwc3_device_req_t *dev_req = NULL;

	DBG("%s(%lx,%x)\n", __func__, (unsigned long)dev_ep, event);

	if (DWC_CIRCLEQ_EMPTY(&dev_ep->dwc_ep.queue)) {
		DBG("%s(%lx), ep->dwc_ep.queue empty\n",
			   __func__, (unsigned long)dev_ep);
		return;
	}

	if (dev_ep->dwc_ep.desc_avail <= 0) {
		DBG("EP%d-%s: no TRB avail\n",
			   dev_ep->dwc_ep.num, dev_ep->dwc_ep.is_in ? "IN" : "OUT");
		return;
	}

	/*
	 * Start the next queued transfer at the target uFrame
	 */

	DWC_CIRCLEQ_FOREACH(dev_req, &dev_ep->dwc_ep.queue, entry) {
		if (dev_req->dwc_req.flags & DWC_PCD_REQ_STARTED)
			dev_req = NULL;
		else
			break;
	}

	DBG("req@0x%08lx\n", (unsigned long)dev_req);
	if (!dev_req) {
		DBG("EP%d-%s: no requests to start\n",
			   dev_ep->dwc_ep.num, dev_ep->dwc_ep.is_in ? "IN" : "OUT");
		return;
	}

	dwc3_device_fill_trbs(dwc3_dev, dev_ep, dev_req);
	dwc3_device_ep_start_transfer(dwc3_dev, dev_ep, dev_req, event);

	/*
	 * Now start any remaining queued transfers
	 */

	while (dev_req != DWC_CIRCLEQ_LAST(&dev_ep->dwc_ep.queue)) {
		dev_req = DWC_CIRCLEQ_NEXT(dev_req, entry);
		if (!(dev_req->dwc_req.flags & DWC_PCD_REQ_STARTED)) {
			dwc3_device_fill_trbs(dwc3_dev, dev_ep, dev_req);
			dwc3_device_ep_start_transfer(dwc3_dev, dev_ep, dev_req, 0);
		}
	}
}

/**
 * This routine terminates all requests which are pending on an endpoint.
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        EP to operate on.
 */
void dwc3_gadget_request_nuke(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep)
{
	dwc3_device_req_t *dev_req = NULL;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);
	dev_ep->dwc_ep.stopped = 1;

	while (!DWC_CIRCLEQ_EMPTY(&dev_ep->dwc_ep.queue)) {
		dev_req = DWC_CIRCLEQ_FIRST(&dev_ep->dwc_ep.queue);
		if (dev_req)
			dwc3_device_request_done(dwc3_dev, dev_ep, dev_req, -DWC_E_SHUTDOWN);
	}

	if (dev_ep != dwc3_dev->ep0)
		dev_ep->dwc_ep.hiber_desc_idx = 0;
}

/**
 * This routine marks all pending requests for an EP as not started.
 *
 * @param pcd           Programming view of DWC_usb3 peripheral controller.
 * @param pcd_ep        EP to operate on.
 */
void dwc3_gadget_set_ep_not_started(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep)
{
	dwc3_device_req_t *dev_req;

	DBG("%s(%lx)\n", __func__, (unsigned long)dev_ep);

	DWC_CIRCLEQ_FOREACH(dev_req, &dev_ep->dwc_ep.queue, entry) {
		if (dev_req->dwc_req.flags & DWC_PCD_REQ_STARTED)
			dev_req->dwc_req.flags &= ~DWC_PCD_REQ_STARTED;
	}
}

/******************************************************************************
 * Function Driver user-defined routines
 *
 * These routines are completely user-defined, and can implement whatever is
 * required for the application's Function Driver.
 *
 * The example code here implements a simple interface which allows external
 * code to implement the application's Function Driver by calling these
 * routines.
 ******************************************************************************/

/**
 * This routine enables a USB endpoint
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param epdesc        USB endpoint descriptor for the EP.
 * @param epcomp        USB SS endpoint companion descriptor for the EP.
 * @return              Pointer to USB EP context, or NULL on failure.
 */
usb_endpoint_t *dwc3_ep_enable(struct dwc3_core *dwc3, const void *epdesc, const void *epcomp)
{
	const usb_endpoint_descriptor_t			*ep_desc = epdesc;
	const ss_endpoint_companion_descriptor_t	*ep_comp = epcomp;
	dwc3_device_t					*dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t				*dev_ep;
	dwc3_dma_desc_t				*desc;
	dwc_dma_t					desc_dma;
	usb_endpoint_t					*usb_ep;
	int						ep_num, ep_dir, ep_type;
	int						link, num_trbs, retval;

	DBG("%s()\n", __func__);

	ep_num = UE_GET_ADDR(ep_desc->bEndpointAddress);
	ep_dir = UE_GET_DIR(ep_desc->bEndpointAddress);
	ep_type = UE_GET_XFERTYPE(ep_desc->bmAttributes);
	link = 0;

	if (ep_num < 1 || ep_num >= DWC_MAX_EPS)
		return NULL;

	if (ep_dir == UE_DIR_IN)
		dev_ep = dwc3_dev->in_ep[ep_num - 1];
	else
		dev_ep = dwc3_dev->out_ep[ep_num - 1];
	usb_ep = &dev_ep->usb_ep;

	/* Allocate the number of TRBs based on EP type */
	switch (ep_type) {
	case UE_INTERRUPT:
		num_trbs = DWC_NUM_INTR_TRBS;
		break;
	case USB_ENDPOINT_XFER_ISOC:
		num_trbs = DWC_NUM_ISOC_TRBS;
		link = 1;
		break;
	default:
		num_trbs = DWC_NUM_BULK_TRBS;
		break;
	}

	DBG("ep%d-%s@0x%08lx phys=%d dev_ep@0x%08lx\n", ep_num,
		   ep_dir == UE_DIR_IN ? "IN" : "OUT", (unsigned long)usb_ep,
		   dev_ep->dwc_ep.phys, (unsigned long)dev_ep);

	/* Set the TRB allocation for this EP */
	desc = dwc3_device_trb_alloc(dev_ep, num_trbs, ep_type, ep_desc->bInterval - 1,
				      link, &desc_dma);
	if (!desc)
		return NULL;

	/* Call the PCD API routine to enable the endpoint */
	retval = dwc3_device_ep_enable(dwc3_dev, dev_ep, ep_desc, ep_comp);
	if (retval) {
		dwc3_device_trb_free(dev_ep);
		return NULL;
	}

	usb_ep->maxpkt = UGETW(ep_desc->wMaxPacketSize);
	usb_ep->num = ep_num;
	usb_ep->in = !!ep_dir;
	usb_ep->req = 0;
	usb_ep->last = 0;

	INF("Enable ep%d-%s@0x%08lx dev_ep@0x%08lx\n", usb_ep->num,
		   usb_ep->in? "IN" : "OUT", (unsigned long)usb_ep,
		   (unsigned long)dev_ep);

	return usb_ep;
}

/**
 * This routine disables a USB endpoint
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param usb_ep        USB EP to disable.
 * @return              0 for success, else negative error code.
 */
int dwc3_ep_disable(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep)
{
	dwc3_device_t		*dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t	*dev_ep;
	int			retval;

	DBG("%s(%lx)\n", __func__, (unsigned long)usb_ep);
	dev_ep = dwc3_get_device_ep(usb_ep);

	/* Call the PCD API routine to disable the endpoint */
	retval = dwc3_device_ep_disable(dwc3_dev, dev_ep);

	dwc3_device_trb_free(dev_ep);

	return retval;
}

/**
 * This routine allocates a USB request object to use with the specified USB
 * endpoint. The contents of a USB request are defined by the Function Driver,
 * and are opaque to the PCD. The USB request is embedded inside of an enclosing
 * PCD request object (see the definition of struct dwc_usb3_pcd_req in pcd.h).
 * One request object is needed for each transfer that is submitted to the PCD.
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param usb_ep        USB EP for the request.
 */
usb_request_t *dwc3_alloc_request(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep)
{
	dwc3_device_req_t	*dev_req;
	unsigned int		req_idx;

	DBG("%s(%lx)\n", __func__, (unsigned long)usb_ep);

	for (req_idx = 0; req_idx < 32; req_idx++) {
		if (g_dev_req_bm & (1 << req_idx)) {
			g_dev_req_bm &= ~(1 << req_idx);
			dev_req = &g_dev_req[req_idx];
			memset(dev_req, 0, sizeof(*dev_req));
			//dev_req->usb_req.dma = DWC_DMA_ADDR_INVALID;
			DBG("%s(): dev_req@0x%08lx, usb_req@0x%08lx\n", __func__,
					(unsigned long)dev_req, (unsigned long)&dev_req->usb_req);
			return &dev_req->usb_req;
		}
	}

	return NULL;
}

/**
 * This routine frees a USB request object
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param usb_ep        USB EP for the request.
 * @param usb_req       USB request to be freed.
 */
void dwc3_free_request(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req)
{
	dwc3_device_req_t	*dev_req = dwc3_get_device_req(usb_req);
	unsigned int		req_idx = dev_req - &g_dev_req[0];

	DBG("%s(%lx)\n", __func__, (unsigned long)usb_ep);

	if (req_idx >= 32) {
		ERR("Error! Bad request index %u\n", req_idx);
		return;
	}

	g_dev_req_bm |= 1 << req_idx;
}

/**
 * This routine submits a transfer request for a USB endpoint. The example code
 * maintains a queue of requests for each endpoint, so that the application can
 * start another transfer on the same endpoint without having to wait for the
 * first transfer to complete.
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param usb_ep        USB EP for the transfer.
 * @param usb_req       USB request for the transfer.
 * @return              0 for success, else negative error code.
 */
int dwc3_ep_queue(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req)
{
	dwc3_device_t		*dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t	*dev_ep = dwc3_get_device_ep(usb_ep);
	dwc3_device_req_t	*dev_req = dwc3_get_device_req(usb_req);
	int			retval = 0, req_flags = 0;

	DBG("%s(%lx)\n", __func__, (unsigned long)usb_ep);

	//usb_req->status = -DWC_E_IN_PROGRESS;
	//usb_req->actual = 0;

	//if (usb_req->zero)
	//	req_flags |= DWC_PCD_REQ_ZERO;

	dev_req->dwc_req.buf[0] = usb_req->buffer;
	dev_req->dwc_req.bufdma[0] = (dwc_dma_t) usb_req->buffer;
	dev_req->dwc_req.buflen[0] = usb_req->length;
	dev_req->dwc_req.numbuf = 1;
	dev_req->dwc_req.length = usb_req->length;
	dev_req->dwc_req.actual = 0;
	dev_req->dwc_req.stream = 0/*usb_req->stream_id*/;
	dev_req->dwc_req.flags = req_flags;

	DBG("[ep%d-%s @0x%08lx] usb_req@0x%08lx, buffer@0x%08lx, length=%d,"
			" dev_req@0x%08lx, buf@0x%08lx (DMA 0x%08lx), length=%d\n",
		usb_ep->num, usb_ep->in?"in":"out", (unsigned long)usb_ep,
		(unsigned long)usb_req, (unsigned long)usb_req->buffer, usb_req->length,
		(unsigned long)dev_req, (unsigned long)dev_req->dwc_req.buf[0],
		(unsigned long)dev_req->dwc_req.bufdma[0], dev_req->dwc_req.length);

	DWC_CIRCLEQ_INIT_ENTRY(dev_req, entry);
#if 1
	if (!DWC_CIRCLEQ_EMPTY(&dev_ep->dwc_ep.queue)) {
		/* If queue is not empty, and this is not an Isoc transfer, then
		 * don't submit the transfer. But we still queue the request so
		 * it can be submitted later.
		 */
		DBG("q !empty, pend %d, stop %d, avail %d, start %d\n",
			   dwc3_dev->request_pending, dev_ep->dwc_ep.stopped,
			   dev_ep->dwc_ep.desc_avail, dev_ep->dwc_ep.xfer_started);

		if (!dev_ep->dwc_ep.stopped && dev_ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC &&
		    dev_ep->dwc_ep.desc_avail > 0 && dev_ep->dwc_ep.xfer_started)
			goto do_start;
	} else
#endif
	if (dev_ep->dwc_ep.stopped || (dev_ep != dwc3_dev->ep0 && dev_ep->dwc_ep.desc_avail <= 0) ||
	    (dev_ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC && !dev_ep->dwc_ep.xfer_started)) {
		/* If EP is stopped, or there is no TRB available, or this
		 * is an Isoc transfer and the EP is not started, then don't
		 * submit the transfer. But we still queue the request so it
		 * can be submitted later.
		 */
		DBG(" q empty, pend %d, stop %d, avail %d, start %d\n",
			   dwc3_dev->request_pending, dev_ep->dwc_ep.stopped,
			   dev_ep->dwc_ep.desc_avail, dev_ep->dwc_ep.xfer_started);
	} else {
do_start:
		/* Call the PCD API routine to set up the request TRBs */
		dwc3_device_fill_trbs(dwc3_dev, dev_ep, dev_req);

		/* Call the PCD API routine to submit the transfer request */
		retval = dwc3_device_ep_submit_req(dwc3_dev, dev_ep, dev_req, req_flags);
	}

	if (!retval) {
		DWC_CIRCLEQ_INSERT_TAIL(&dev_ep->dwc_ep.queue, dev_req, entry);
		++dwc3_dev->request_pending;
	}

	/* dwc_usb3_pcd_ep_submit_req() can return positive values for
	 * Control transfers, don't propagate those from this function
	 */
	return retval < 0 ? retval : 0;
}

/**
 * This routine cancels a transfer request for a USB endpoint. This is only
 * needed in exceptional cases, a normal transfer completion does not require
 * this.
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @param usb_ep        USB EP for the transfer.
 * @param usb_req       USB request for the transfer.
 * @return              0 for success, else negative error code.
 */
int dwc3_ep_dequeue(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req)
{
	dwc3_device_t		*dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t	*dev_ep = dwc3_get_device_ep(usb_ep);
	dwc3_device_req_t	*dev_req = NULL;

	DBG("%s(%08lx,%08lx)\n", __func__, (unsigned long)usb_ep, (unsigned long)usb_req);

	if (dwc3_device_ep_num(dev_ep) != 0 && !dev_ep->dwc_ep.usb_ep_desc) {
		DBG("%s, bad dev_ep\n", __func__);
		return -DWC_E_INVALID;
	}

	/* Make sure it's actually queued on this EP */
	DWC_CIRCLEQ_FOREACH(dev_req, &dev_ep->dwc_ep.queue, entry) {
		DBG("Find dev_req@%p, usb_req@%p\n", dev_req, &dev_req->usb_req);
		if (&dev_req->usb_req == usb_req)
			break;
		else
			dev_req = NULL;
	}

	if (!dev_req || dev_req == DWC_CIRCLEQ_END(&dev_ep->dwc_ep.queue)) {
		WARN("%s, no request (usb_req@%p) in queue\n", __func__, usb_req);
		return 0;
	}
	DBG("TO cancel dev_req@%p, usb_req@%p\n", dev_req, &dev_req->usb_req);

	/* Call the PCD API routine to cancel the transfer request */
	dwc3_device_ep_cancel_req(dwc3_dev, dev_ep, dev_req, 0/*usb_req->stream_id*/);

	return 0;
}

/**
 * Function Driver initialization routine
 *
 * @param usb3_dev      Programming view of DWC_usb3 device.
 * @return              0 for success, else negative error code.
 */
int dwc3_gadget_init(dwc3_core_t *dwc3)
{
	dwc3_device_t		*dwc3_dev = &dwc3->dwc3_dev;
	dwc3_device_ep_t	*ep;
	int			i;

	DBG("%s()\n", __func__);

	ep = dwc3_dev->ep0;
	ep->dwc_ep.num = 0;
	DWC_CIRCLEQ_INIT(&ep->dwc_ep.queue);

	for (i = 0; i < dwc3_dev->num_out_eps; i++) {
		ep = dwc3_dev->out_ep[i];
		ep->dwc_ep.num = i + 1;
		DWC_CIRCLEQ_INIT(&ep->dwc_ep.queue);
	}

	for (i = 0; i < dwc3_dev->num_in_eps; i++) {
		ep = dwc3_dev->in_ep[i];
		ep->dwc_ep.num = i + 1;
		DWC_CIRCLEQ_INIT(&ep->dwc_ep.queue);
	}

	return 0;
}

/**
 * Function Driver removal routine
 *
				 * @param usb3_dev      programming view of dwc_usb3 device.
 */
void dwc3_gadget_remove(dwc3_core_t *dwc3)
{
	DBG("%s()\n", __func__);
}
