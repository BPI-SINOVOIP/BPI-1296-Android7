#include <dev/udc.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"
#include "dwc3.h"

#undef log_level
#define log_level 3

/*=======================================================================*/
/*
 * EP0 routines
 */

/**
 * This routine starts the data stage of a 3-stage control command.
 * dwc3_dev->ep0state must be set to EP0_OUT_DATA_PHASE or EP0_IN_DATA_PHASE, and
 * dwc3_dev->ep0->dwc_ep.is_in must be set to 0 or 1 before calling this routine.
 * For IN, the data to be sent must be placed in dwc3_dev->ep0_status_buf before
 * the call.
 */
void dwc3_device_ep0_data_stage(dwc3_device_t *dwc3_dev, int length)
{
	dwc3_dev->ep0_req->dwc_req.buf[0] = (char *)dwc3_dev->ep0_status_buf;
	dwc3_dev->ep0_req->dwc_req.bufdma[0] = dwc3_dev->ep0_status_buf_dma;
	dwc3_dev->ep0_req->dwc_req.length = length;
	dwc3_dev->ep0_req->dwc_req.actual = 0;
	dwc3_dev->ep0_status_pending = 1;
	dwc3_dev->ep0->dwc_ep.send_zlp = 0;
	dwc3_device_ep0_start_transfer(dwc3_dev, dwc3_dev->ep0_req);
}

/**
 * This routine processes the SET_ADDRESS Setup Commands.
 */
static void do_set_address(dwc3_device_t *dwc3_dev)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;

	DBG("SET ADDRESS\n");

	if (ctrl.bmRequestType == UT_DEVICE) {
		DBG("SET_ADDRESS %d\n",
			   UGETW(ctrl.wValue));
		dwc3_set_address(dwc3_dev, UGETW(ctrl.wValue));
		dwc3_dev->ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
		if (UGETW(ctrl.wValue))
			dwc3_dev->state = DWC_STATE_ADDRESSED;
		else
			dwc3_dev->state = DWC_STATE_DEFAULT;
	}
}

/**
 * This routine stalls EP0.
 */
static void ep0_do_stall(dwc3_device_t *dwc3_dev, int err_val)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;

	INF("req %02x.%02x protocol STALL; err %d\n",
		   ctrl.bmRequestType, ctrl.bRequest, err_val);
	ep0->dwc_ep.is_in = 0;
	dwc3_device_ep_set_stall(dwc3_dev, ep0);
	ep0->dwc_ep.stopped = 1;
	dwc3_dev->ep0state = EP0_IDLE;
	dwc3_device_ep0_out_start(dwc3_dev);
}

/**
 * Clear the EP halt (STALL), and if there are pending requests start
 * the transfer.
 */
static void do_clear_halt(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("%s()\n", __func__);

	if (ep->dwc_ep.stall_clear_flag == 0) {
		if (ep != dwc3_dev->ep0)
			dwc3_device_ep_clear_stall(dwc3_dev, ep);

		if (ep->dwc_ep.stopped) {
			ep->dwc_ep.stopped = 0;

			/* If there is a request in the EP queue start it */
			if (ep != dwc3_dev->ep0 && ep->dwc_ep.is_in)
				dwc3_gadget_start_next_request(dwc3_dev, ep);
		}
	} else {
		dwc3_dis_usb2_suspend(dwc3_dev);

		/* Clear sequence number using DEPCFG */
		if (ep->dwc_ep.is_in) {
			ep_reg = ep->dwc_ep.in_ep_reg;
			dwc3_dep_cfg(dwc3_dev, ep_reg, ep->dwc_ep.param0in,
					 ep->dwc_ep.param1in, 0);
		} else {
			ep_reg = ep->dwc_ep.out_ep_reg;
			dwc3_dep_cfg(dwc3_dev, ep_reg, ep->dwc_ep.param0out,
					 ep->dwc_ep.param1out, 0);
		}

		dwc3_ena_usb2_suspend(dwc3_dev);
	}

	/* Start Control Status Phase */
	dwc3_dev->ep0->dwc_ep.is_in = 1;
	dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
}

/**
 * This routine handles the Get Descriptor request for the BOS descriptor
 * and the OTG descriptor, and passes all other requests to the Gadget driver.
 */
static void do_get_descriptor(dwc3_device_t *dwc3_dev)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	u8 desc_type = UGETW(ctrl.wValue) >> 8;
	int ret;

	DBG("GET_DESCRIPTOR %02x.%02x v%04x i%04x l%04x\n",
		   ctrl.bmRequestType, ctrl.bRequest, UGETW(ctrl.wValue),
		   UGETW(ctrl.wIndex), UGETW(ctrl.wLength));

	switch (desc_type) {

	case UDESC_BOS:
		DBG("\nGET_DESCRIPTOR(BOS)\n\n");
		if (dwc3_dev->speed != USB_SPEED_SUPER &&
		    dwc3_dev->dwc3->core_params->nobos) {
			ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
			return;
		}

	default:
		/* Call the Gadget driver's setup routine */
		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret < 0)
			ep0_do_stall(dwc3_dev, ret);
		break;
	}
}

/**
 * This routine processes the GET_STATUS Setup Commands.
 */
static void do_get_status(dwc3_device_t *dwc3_dev)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	u8 *status = dwc3_dev->ep0_status_buf;
	dwc3_device_ep_t *ep;
	int length;

	DBG("GET_STATUS %02x.%02x v%04x i%04x l%04x\n",
		   ctrl.bmRequestType, ctrl.bRequest, UGETW(ctrl.wValue),
		   UGETW(ctrl.wIndex), UGETW(ctrl.wLength));

	if (UGETW(ctrl.wLength) != 2) {
		ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
		return;
	}

	switch (UT_GET_RECIPIENT(ctrl.bmRequestType)) {
	case UT_DEVICE:
		{
			*status = 1; /* Self powered */

			if (dwc3_dev->speed == USB_SPEED_SUPER) {
				if (dwc3_dev->state == DWC_STATE_CONFIGURED) {
					if (dwc3_u1_enabled(dwc3_dev))
						*status |= 1 << 2;

					if (dwc3_u2_enabled(dwc3_dev))
						*status |= 1 << 3;

					*status |= dwc3_dev->ltm_enable << 4;
				}
			} else {
				*status |= dwc3_dev->remote_wakeup_enable << 1;
			}
		}

		DBG("GET_STATUS(Device)=%02x\n", *status);
		*(status + 1) = 0;
		break;

	case UT_INTERFACE:
		*status = 0;
		if (dwc3_dev->dwc3->core_params->wakeup)
			*status |= 1;
		*status |= dwc3_dev->remote_wakeup_enable << 1;
		DBG("GET_STATUS(Interface %d)=%02x\n",
				UGETW(ctrl.wIndex), *status);
		*(status + 1) = 0;
		break;

	case UT_ENDPOINT:
		ep = dwc3_device_get_ep_by_addr(dwc3_dev, UGETW(ctrl.wIndex));

		/* @todo check for EP stall */
		*status = ep->dwc_ep.stopped;
		DBG("GET_STATUS(Endpoint %d)=%02x\n",
				UGETW(ctrl.wIndex), *status);
		*(status + 1) = 0;
		break;

	default:
		ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
		return;
	}

	length = 2;
	dwc3_device_ep0_data_stage(dwc3_dev, length);
}

/**
 * This routine processes the SET_FEATURE Setup Commands.
 */
static void do_set_feature(dwc3_device_t *dwc3_dev)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	dwc3_device_ep_t *ep;
	int ret;

	DBG("SET_FEATURE %02x.%02x v%04x i%04x l%04x\n",
		   ctrl.bmRequestType, ctrl.bRequest, UGETW(ctrl.wValue),
		   UGETW(ctrl.wIndex), UGETW(ctrl.wLength));

	switch (UT_GET_RECIPIENT(ctrl.bmRequestType)) {
	case UT_DEVICE:
		switch (UGETW(ctrl.wValue)) {
		case UF_DEVICE_REMOTE_WAKEUP:
			dwc3_dev->remote_wakeup_enable = 1;
			break;

		case UF_TEST_MODE:
			/* Setup the Test Mode tasklet to do the Test
			 * Packet generation after the SETUP Status
			 * phase has completed. */
			dwc3_dev->test_mode = UGETW(ctrl.wIndex) >> 8;
			//dwc3_task_schedule(&dwc3_dev->test_mode_tasklet);
			break;

		case UF_DEVICE_B_HNP_ENABLE:
			DBG("SET_FEATURE: USB_DEVICE_B_HNP_ENABLE\n");
			break;

		case UOTG_NTF_HOST_REL:
			DBG("SET_FEATURE: USB_NTF_HOST_REL\n");
			break;

		case UOTG_B3_RSP_ENABLE:
			DBG("SET_FEATURE: USB_B3_RSP_ENABLE\n");
			break;

		case UF_DEVICE_A_HNP_SUPPORT:
			/* RH port supports HNP */
			DBG("SET_FEATURE: USB_DEVICE_A_HNP_SUPPORT\n");
			break;

		case UF_DEVICE_A_ALT_HNP_SUPPORT:
			/* other RH port does */
			DBG("SET_FEATURE: USB_DEVICE_A_ALT_HNP_SUPPORT\n");
			break;

		case UF_U1_ENABLE:
			DBG("SET_FEATURE: UF_U1_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			if (dwc3_dev->dwc3->core_params->pwrctl & 1)
				dwc3_enable_u1(dwc3_dev);
			break;

		case UF_U2_ENABLE:
			DBG("SET_FEATURE: UF_U2_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			if (dwc3_dev->dwc3->core_params->pwrctl & 2)
				dwc3_enable_u2(dwc3_dev);
			break;

		case UF_LTM_ENABLE:
			DBG("SET_FEATURE: UF_LTM_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED ||
			    UGETW(ctrl.wIndex) != 0) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			dwc3_dev->ltm_enable = 1;
			dwc3_dev->send_lpm = 1;
			break;

		default:
			ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
			return;
		}

		break;

	case UT_INTERFACE:
		/* if FUNCTION_SUSPEND ... */
		if (UGETW(ctrl.wValue) == 0) {
			/* if Function Remote Wake Enabled ... */
			if (UGETW(ctrl.wIndex) >> 8 & 2)
				dwc3_dev->remote_wakeup_enable = 1;
			else
				dwc3_dev->remote_wakeup_enable = 0;

			/* if Function Low Power Suspend ... */
			// TODO

			break;
		}

		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret < 0)
			ep0_do_stall(dwc3_dev, ret);
		return;

	case UT_ENDPOINT:
		ep = dwc3_device_get_ep_by_addr(dwc3_dev, UGETW(ctrl.wIndex));
		if (UGETW(ctrl.wValue) != UF_ENDPOINT_HALT) {
			ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
			return;
		}

		ep->dwc_ep.stopped = 1;
		dwc3_device_ep_set_stall(dwc3_dev, ep);
		break;
	}

	dwc3_dev->ep0->dwc_ep.is_in = 1;
	dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
}

/**
 * This routine processes the CLEAR_FEATURE Setup Commands.
 */
static void do_clear_feature(dwc3_device_t *dwc3_dev)
{
	usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	dwc3_device_ep_t *ep;

	DBG("CLEAR_FEATURE %02x.%02x v%04x i%04x l%04x\n",
		   ctrl.bmRequestType, ctrl.bRequest, UGETW(ctrl.wValue),
		   UGETW(ctrl.wIndex), UGETW(ctrl.wLength));

	switch (UT_GET_RECIPIENT(ctrl.bmRequestType)) {
	case UT_DEVICE:
		switch (UGETW(ctrl.wValue)) {
		case UF_DEVICE_REMOTE_WAKEUP:
			dwc3_dev->remote_wakeup_enable = 0;
			break;

		case UF_TEST_MODE:
			/* @todo Add CLEAR_FEATURE for TEST modes. */
			break;

		case UF_U1_ENABLE:
			DBG("CLEAR_FEATURE: UF_U1_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			dwc3_disable_u1(dwc3_dev);
			break;

		case UF_U2_ENABLE:
			DBG("CLEAR_FEATURE: UF_U2_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			dwc3_disable_u2(dwc3_dev);
			break;

		case UF_LTM_ENABLE:
			DBG("CLEAR_FEATURE: UF_LTM_ENABLE\n");
			if (dwc3_dev->speed != USB_SPEED_SUPER ||
			    dwc3_dev->state != DWC_STATE_CONFIGURED ||
			    UGETW(ctrl.wIndex) != 0) {
				ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
				return;
			}

			dwc3_dev->ltm_enable = 0;
			dwc3_dev->send_lpm = 1;
			break;

		default:
			ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
			return;
		}

		break;

	case UT_INTERFACE:
		/* if FUNCTION_SUSPEND ... */
		if (UGETW(ctrl.wValue) == 0) {
			/* if Function Remote Wake Enabled ... */
			if (UGETW(ctrl.wIndex) >> 8 & 2) {
				dwc3_dev->remote_wakeup_enable = 0;
			}

			/* if Function Low Power Suspend ... */
			// TODO

			break;
		}

		ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
		return;

	case UT_ENDPOINT:
		ep = dwc3_device_get_ep_by_addr(dwc3_dev, UGETW(ctrl.wIndex));
		if (UGETW(ctrl.wValue) != UF_ENDPOINT_HALT) {
			ep0_do_stall(dwc3_dev, -DWC_E_NOT_SUPPORTED);
			return;
		}

		do_clear_halt(dwc3_dev, ep);
		return;
	}

	dwc3_dev->ep0->dwc_ep.is_in = 1;
	dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
}

/**
 * This routine processes SETUP commands. The USB Command processing is
 * done in two places - the first being the PCD and the second being the
 * Gadget driver (for example, the File-Backed Storage Gadget driver).
 *
 * <table>
 * <tr><td> Command </td><td> Driver </td><td> Description </td></tr>
 *
 * <tr><td> GET_STATUS </td><td> PCD </td><td> Command is processed
 * as defined in chapter 9 of the USB 2.0 Specification. </td></tr>
 *
 * <tr><td> SET_FEATURE </td><td> PCD / Gadget driver </td><td> Device
 * and Endpoint requests are processed by the PCD. Interface requests
 * are passed to the Gadget driver. </td></tr>
 *
 * <tr><td> CLEAR_FEATURE </td><td> PCD </td><td> Device and Endpoint
 * requests are processed by the PCD. Interface requests are ignored.
 * The only Endpoint feature handled is ENDPOINT_HALT. </td></tr>
 *
 * <tr><td> SET_ADDRESS </td><td> PCD </td><td> Program the DCFG register
 * with device address received. </td></tr>
 *
 * <tr><td> GET_DESCRIPTOR </td><td> Gadget driver </td><td> Return the
 * requested descriptor. </td></tr>
 *
 * <tr><td> SET_DESCRIPTOR </td><td> Gadget driver </td><td> Optional -
 * not implemented by any of the existing Gadget drivers. </td></tr>
 *
 * <tr><td> GET_CONFIGURATION </td><td> Gadget driver </td><td> Return
 * the current configuration. </td></tr>
 *
 * <tr><td> SET_CONFIGURATION </td><td> Gadget driver </td><td> Disable
 * all EPs and enable EPs for new configuration. </td></tr>
 *
 * <tr><td> GET_INTERFACE </td><td> Gadget driver </td><td> Return the
 * current interface. </td></tr>
 *
 * <tr><td> SET_INTERFACE </td><td> Gadget driver </td><td> Disable all
 * EPs and enable EPs for new interface. </td></tr>
 * </table>
 *
 * When the SETUP Phase Done interrupt occurs, the generic SETUP commands
 * are processed by this routine. Calling the Gadget driver's
 * dwc3_gadget_setup() routine from here processes the gadget-specific
 * SETUP commands.
 */
void dwc3_device_do_setup(dwc3_device_t *dwc3_dev)
{
	//usb_device_request_t ctrl = dwc3_dev->ep0_setup_pkt->req;
	usb_device_request_t ctrl = dwc3_ep0_setup_pkt(dwc3_dev)->req;
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	u16 wvalue, wlength;
	int ret;

	dwc3_dma_desc_t *desc;

	DBG("%s(%lx)\n", __func__, (unsigned long)dwc3_dev);
	dwc3_inval_cache((uintptr_t)dwc3_dev->ep0_setup_desc, sizeof(dwc3_dma_desc_t));
	dwc3_inval_cache((uintptr_t)dwc3_dev->ep0_setup_pkt, 8);
	wvalue = UGETW(ctrl.wValue);
	wlength = UGETW(ctrl.wLength);

	DBG("setup_pkt[0]=0x%08x\n",
		   dwc3_dev->ep0_setup_pkt->d32[0]);
	DBG("setup_pkt[1]=0x%08x\n",
		   dwc3_dev->ep0_setup_pkt->d32[1]);
	DBG("SETUP %02x.%02x v%04x i%04x l%04x\n",
		   ctrl.bmRequestType, ctrl.bRequest, wvalue,
		   UGETW(ctrl.wIndex), wlength);

	/* Get the SETUP packet DMA Descriptor (TRB) */
	desc = dwc3_ep0_setup_desc(dwc3_dev);
	DBG("desc 0x%08x 0x%08x 0x%08x 0x%08x\n",
		   *((unsigned *)desc), *((unsigned *)desc + 1),
		   *((unsigned *)desc + 2), *((unsigned *)desc + 3));




	/* Clean up the request queue */
	dwc3_gadget_request_nuke(dwc3_dev, ep0);
	ep0->dwc_ep.stopped = 0;
	ep0->dwc_ep.three_stage = 1;

	if (ctrl.bmRequestType & UE_DIR_IN) {
		ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_DATA_PHASE;
	} else {
		ep0->dwc_ep.is_in = 0;
		dwc3_dev->ep0state = EP0_OUT_DATA_PHASE;
	}

	if (wlength == 0) {
		ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_WAIT_GADGET;
		ep0->dwc_ep.three_stage = 0;
	}

	if ((UT_GET_TYPE(ctrl.bmRequestType)) != UT_STANDARD) {
		/* handle non-standard (class/vendor) requests
		 * in the gadget driver
		 */
		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret < 0)
			ep0_do_stall(dwc3_dev, ret);
		return;
	}

	/* @todo NGS: Handle bad setup packet? */

///////////////////////////////////////////
//// --- Standard Request handling --- ////

	switch (ctrl.bRequest) {
	case UR_GET_STATUS:
		do_get_status(dwc3_dev);
		break;

	case UR_CLEAR_FEATURE:
		do_clear_feature(dwc3_dev);
		break;

	case UR_SET_FEATURE:
		do_set_feature(dwc3_dev);
		break;

	case UR_SET_ADDRESS:
		do_set_address(dwc3_dev);
		break;

	case UR_SET_INTERFACE:
		DBG("USB_REQ_SET_INTERFACE\n");
		dwc3_clr_eps_enabled(dwc3_dev);

#ifdef DWC_STAR_9000463548_WORKAROUND
		dwc3_dev->configuring = 1;
#endif
		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret < 0) {
#ifdef DWC_STAR_9000463548_WORKAROUND
			dwc3_dev->configuring = 0;
#endif
			ep0_do_stall(dwc3_dev, ret);
			return;
		}

		break;

	case UR_SET_CONFIG:
		DBG("USB_REQ_SET_CONFIGURATION\n");
		dwc3_clr_eps_enabled(dwc3_dev);

#ifdef DWC_STAR_9000463548_WORKAROUND
		dwc3_dev->configuring = 1;
#endif
		if (dwc3_dev->ltm_enable)
			dwc3_dev->send_lpm = 1;

		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret >= 0) {
			if (wvalue != 0)
				dwc3_dev->state = DWC_STATE_CONFIGURED;
			else
				dwc3_dev->state = DWC_STATE_ADDRESSED;
		} else {
			ep0_do_stall(dwc3_dev, ret);
#ifdef DWC_STAR_9000463548_WORKAROUND
			dwc3_dev->configuring = 0;
#endif
			return;
		}

		/* Must wait until SetConfig before accepting U1/U2 link
		 * control, otherwise we have problems with VIA hubs
		 */
		if (dwc3_dev->dwc3->core_params->pwrctl & 1)
			dwc3_accept_u1(dwc3_dev);
		if (dwc3_dev->dwc3->core_params->pwrctl & 2)
			dwc3_accept_u2(dwc3_dev);

		dwc3_dev->ltm_enable = 0;
		break;

	case UR_GET_DESCRIPTOR:
		do_get_descriptor(dwc3_dev);
		break;

	case UR_SET_SEL:
		DBG("USB_REQ_SET_SEL\n");

		/* For now this is a no-op */
		dwc3_device_ep0_data_stage(dwc3_dev, DWC_STATUS_BUF_SIZE < wlength ?
					    DWC_STATUS_BUF_SIZE : wlength);
		break;

	case UR_SET_ISOC_DELAY:
		DBG("USB_REQ_SET_ISOC_DELAY\n");

		/* For now this is a no-op */
		dwc3_dev->ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
		break;

	default:
		/* Call the Gadget driver's setup routine */
		ret = dwc3_gadget_setup(dwc3_dev, &ctrl);
		if (ret < 0)
			ep0_do_stall(dwc3_dev, ret);
		break;
	}
}

/**
 * This routine starts the Zero-Length Packet for the IN status phase of a
 * control write transfer.
 */
static void setup_in_status_phase(dwc3_device_t *dwc3_dev, void *buf,
				  dwc_dma_t dma)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;

	DBG("%s()\n", __func__);

	if (dwc3_dev->ep0state == EP0_STALL) {
		DBG("EP0 STALLED\n");
		return;
	}

	ep0->dwc_ep.is_in = 1;
	dwc3_dev->ep0state = EP0_IN_STATUS_PHASE;

	DBG("EP0 IN ZLP\n");

	dwc3_dev->ep0_req->dwc_req.buf[0] = buf;
	dwc3_dev->ep0_req->dwc_req.bufdma[0] = dma;
	dwc3_dev->ep0_req->dwc_req.length = 0;
	dwc3_dev->ep0_req->dwc_req.actual = 0;
	dwc3_device_ep0_start_transfer(dwc3_dev, dwc3_dev->ep0_req);
}

/**
 * This routine starts the Zero-Length Packet for the OUT status phase of a
 * control read transfer.
 */
static void setup_out_status_phase(dwc3_device_t *dwc3_dev, void *buf,
				   dwc_dma_t dma)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;

	DBG("%s()\n", __func__);

	if (dwc3_dev->ep0state == EP0_STALL) {
		DBG("EP0 STALLED\n");
		return;
	}

	ep0->dwc_ep.is_in = 0;
	dwc3_dev->ep0state = EP0_OUT_STATUS_PHASE;

	DBG("EP0 OUT ZLP\n");

	dwc3_dev->ep0_req->dwc_req.buf[0] = buf;
	dwc3_dev->ep0_req->dwc_req.bufdma[0] = dma;
	dwc3_dev->ep0_req->dwc_req.length = 0;
	dwc3_dev->ep0_req->dwc_req.actual = 0;
	dwc3_device_ep0_start_transfer(dwc3_dev, dwc3_dev->ep0_req);
}

/**
 * This routine prints the ep0 state for debug purposes.
 */
void dwc3_print_ep0_state(dwc3_device_t *dwc3_dev)
{
	const char *str;

	switch (dwc3_dev->ep0state) {
	case EP0_IDLE:
		str = "EP0_IDLE";
		break;
	case EP0_IN_DATA_PHASE:
		str = "EP0_IN_DATA_PHASE";
		break;
	case EP0_OUT_DATA_PHASE:
		str = "EP0_OUT_DATA_PHASE";
		break;
	case EP0_IN_WAIT_GADGET:
		str = "EP0_IN_WAIT_GADGET";
		break;
	case EP0_OUT_WAIT_GADGET:
		str = "EP0_OUT_WAIT_GADGET";
		break;
	case EP0_IN_WAIT_NRDY:
		str = "EP0_IN_WAIT_NRDY";
		break;
	case EP0_OUT_WAIT_NRDY:
		str = "EP0_OUT_WAIT_NRDY";
		break;
	case EP0_IN_STATUS_PHASE:
		str = "EP0_IN_STATUS_PHASE";
		break;
	case EP0_OUT_STATUS_PHASE:
		str = "EP0_OUT_STATUS_PHASE";
		break;
	case EP0_STALL:
		str = "EP0_STALL";
		break;
	default:
		str = "EP0_INVALID";
	}

	DBG("%s(%d)\n", str, dwc3_dev->ep0state);
}

/**
 * This routine completes the ep0 control transfer.
 */
static int ep0_complete_request(dwc3_device_t *dwc3_dev, dwc3_device_req_t *req,
				dwc3_dma_desc_t *desc, int status)
{
	dwc3_device_ep_t *ep = dwc3_dev->ep0;
	int is_last = 0;

	DBG("%s(%lx,%lx,%d)\n", __func__,
		   (unsigned long)req, (unsigned long)desc, status);

	if (dwc3_dev->ep0_status_pending && !req) {
		if (ep->dwc_ep.is_in) {
			DBG("Do setup OUT status phase\n");
			dwc3_dev->ep0->dwc_ep.is_in = 0;
			dwc3_dev->ep0state = EP0_OUT_WAIT_NRDY;
		} else {
			DBG("Do setup IN status phase\n");
			dwc3_dev->ep0->dwc_ep.is_in = 1;
			dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
		}

		dwc3_dev->ep0_status_pending = 0;
		return 1;
	}

	if (!req)
		return 0;

	DBG("req=%lx\n", (unsigned long)req);

	if (dwc3_dev->ep0state == EP0_OUT_STATUS_PHASE ||
	    dwc3_dev->ep0state == EP0_IN_STATUS_PHASE) {
		is_last = 1;

	} else if (ep->dwc_ep.is_in) {
		DBG("IN len=%d actual=%d xfrcnt=%d trbrsp=0x%02x\n",
			   req->dwc_req.length, req->dwc_req.actual,
			   dwc3_get_xfercnt(desc),
			   dwc3_get_xfersts(desc));
		if (dwc3_get_xfercnt(desc) == 0) {
			/* Is a Zero Len Packet needed? */
			if (req->dwc_req.flags & DWC_PCD_REQ_ZERO) {
				DBG("Setup Rx ZLP\n");
				req->dwc_req.flags &= ~DWC_PCD_REQ_ZERO;
			}

			dwc3_dev->ep0->dwc_ep.is_in = 0;
			dwc3_dev->ep0state = EP0_OUT_WAIT_NRDY;
		}
	} else {
		DBG("OUT len=%d actual=%d xfrcnt=%d trbrsp=0x%02x\n",
			   req->dwc_req.length, req->dwc_req.actual,
			   dwc3_get_xfercnt(desc),
			   dwc3_get_xfersts(desc));
		/* Is a Zero Len Packet needed? */
		if (req->dwc_req.flags & DWC_PCD_REQ_ZERO) {
			DBG("Setup Tx ZLP\n");
			req->dwc_req.flags &= ~DWC_PCD_REQ_ZERO;
		}

		dwc3_dev->ep0->dwc_ep.is_in = 1;
		dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
	}

	/* Complete the request */
	if (is_last) {
		DBG("is_last len=%d actual=%d\n",
			   req->dwc_req.length, req->dwc_req.actual);
		dwc3_device_request_done(dwc3_dev, ep, req, status);
		return 1;
	}

	return 0;
}

/**
 * This routine handles EP0 Control transfers.
 *
 * The state of the control tranfers are tracked in <code>ep0state</code>.
 */
static void dwc3_handle_ep0(dwc3_device_t *dwc3_dev, dwc3_device_req_t *req,
				u32 event)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	dwc3_dma_desc_t *desc;
	u32 byte_count, len;
	int status;

	DBG("%s()\n", __func__);
	dwc3_print_ep0_state(dwc3_dev);
	DBG("HANDLE EP0\n");

	switch (dwc3_dev->ep0state) {
	case EP0_IN_DATA_PHASE:
		if (!req)
			req = dwc3_dev->ep0_req;
		desc = dwc3_ep0_in_desc(dwc3_dev);
		DBG("ep0_req@0x%08lx\n", (unsigned long)req);
		DBG("DATA_IN EP%d-%s: type=%d mps=%d trb.status=0x%08x\n",
			ep0->dwc_ep.num, (ep0->dwc_ep.is_in ? "IN" : "OUT"),
			ep0->dwc_ep.type, ep0->dwc_ep.maxpacket, desc->status);
		if (dwc3_is_hwo(desc)) {
			DBG("### %s, EP%d-%s HWO bit set 1! ###\n",
				__func__, ep0->dwc_ep.num, ep0->dwc_ep.is_in ?
				"IN" : "OUT");
			goto out;
		}

		status = dwc3_get_xfersts(desc);
		if (status & DWC_TRBRSP_SETUP_PEND) {
			/* Start of a new Control transfer */
			DBG("IN SETUP PENDING\n");
			desc->status = 0;
		}

		byte_count = req->dwc_req.length - dwc3_get_xfercnt(desc);
		req->dwc_req.actual += byte_count;
		req->dwc_req.buf[0] += byte_count;
		req->dwc_req.bufdma[0] += byte_count;
		DBG("length=%d byte_count=%d actual=%d\n",
			req->dwc_req.length, byte_count, req->dwc_req.actual);

		if (req->dwc_req.actual < req->dwc_req.length) {
			//dwc3_dump_dbgregs(dwc3_dev->dwc3);
			DBG("IN CONTINUE\n");
			//dwc3_device_ep0_continue_transfer(dwc3_dev, req);
			//DBG("CONTINUE TRANSFER\n");
			DBG("Stall EP0\n");
			ep0->dwc_ep.is_in = 0;
			dwc3_device_ep_set_stall(dwc3_dev, ep0);
			ep0->dwc_ep.stopped = 1;
			dwc3_dev->ep0state = EP0_IDLE;
			dwc3_device_ep0_out_start(dwc3_dev);

		} else if (ep0->dwc_ep.send_zlp) {
			DBG("IN ZLP\n");
			dwc3_device_ep0_continue_transfer(dwc3_dev, req);
			ep0->dwc_ep.send_zlp = 0;
			DBG("CONTINUE TRANSFER\n");

		} else {
			DBG("IN COMPLETE\n");
			/* This sets ep0state = EP0_IN/OUT_WAIT_NRDY */
			ep0_complete_request(dwc3_dev, req, desc, 0);
			DBG("COMPLETE TRANSFER\n");
		}

		break;

	case EP0_OUT_DATA_PHASE:
		if (!req)
			req = dwc3_dev->ep0_req;
		desc = dwc3_ep0_out_desc(dwc3_dev);
		DBG("req@0x%08lx\n", (unsigned long)req);
		DBG("DATA_OUT EP%d-%s: type=%d mps=%d trb.status=0x%08x\n",
			ep0->dwc_ep.num, (ep0->dwc_ep.is_in ? "IN" : "OUT"),
			ep0->dwc_ep.type, ep0->dwc_ep.maxpacket, desc->status);
		if (dwc3_is_hwo(desc)) {
			DBG("### %s, EP%d-%s HWO bit set 2! ###\n",
				__func__, ep0->dwc_ep.num, ep0->dwc_ep.is_in ?
				"IN" : "OUT");
			goto out;
		}

		status = dwc3_get_xfersts(desc);
		if (status & DWC_TRBRSP_SETUP_PEND) {
			/* Start of a new Control transfer */
			DBG("OUT SETUP PENDING\n");
		}

		len = (req->dwc_req.length + ep0->dwc_ep.maxpacket - 1) &
			~(ep0->dwc_ep.maxpacket - 1);
		byte_count = len - dwc3_get_xfercnt(desc);
		req->dwc_req.actual += byte_count;
		req->dwc_req.buf[0] += byte_count;
		req->dwc_req.bufdma[0] += byte_count;
		DBG("length=%d byte_count=%d actual=%d\n",
			req->dwc_req.length, byte_count, req->dwc_req.actual);

		/*if (req->dwc_req.actual < req->dwc_req.length) {
			dwc_debug0(dwc3_dev->usb3_dev, "OUT CONTINUE\n");
			dwc3_device_ep0_continue_transfer(dwc3_dev, req);
			dwc_debug0(dwc3_dev->usb3_dev, "CONTINUE TRANSFER\n");

		} else*/ if (ep0->dwc_ep.send_zlp) {
			DBG("OUT ZLP\n");
			dwc3_device_ep0_continue_transfer(dwc3_dev, req);
			ep0->dwc_ep.send_zlp = 0;
			DBG("CONTINUE TRANSFER\n");

		} else {
			DBG("OUT COMPLETE\n");
			/* This sets ep0state = EP0_IN/OUT_WAIT_NRDY */
			ep0_complete_request(dwc3_dev, req, desc, 0);
			DBG("COMPLETE TRANSFER\n");
		}

		break;

	case EP0_IN_WAIT_GADGET:
		dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
		break;

	case EP0_OUT_WAIT_GADGET:
		dwc3_dev->ep0state = EP0_OUT_WAIT_NRDY;
		break;

	case EP0_IN_WAIT_NRDY:
	case EP0_OUT_WAIT_NRDY:
		if (ep0->dwc_ep.is_in)
			setup_in_status_phase(dwc3_dev, dwc3_ep0_setup_pkt(dwc3_dev),
					      dwc3_ep0_setup_pkt_dma(dwc3_dev));
		else
			setup_out_status_phase(dwc3_dev, dwc3_ep0_setup_pkt(dwc3_dev),
					       dwc3_ep0_setup_pkt_dma(dwc3_dev));
		break;

	case EP0_IN_STATUS_PHASE:
	case EP0_OUT_STATUS_PHASE:
		if (ep0->dwc_ep.is_in)
			desc = dwc3_ep0_in_desc(dwc3_dev);
		else
			desc = dwc3_ep0_out_desc(dwc3_dev);
		DBG("STATUS EP%d-%s\n", ep0->dwc_ep.num,
			   (ep0->dwc_ep.is_in ? "IN" : "OUT"));
		ep0_complete_request(dwc3_dev, req, desc, 0);
		dwc3_dev->ep0state = EP0_IDLE;
		ep0->dwc_ep.stopped = 1;
		ep0->dwc_ep.is_in = 0;	/* OUT for next SETUP */

		if (dwc3_dev->send_lpm) {
			dwc3_dev->send_lpm = 0;
		}

		DBG("Prepare for next SETUP Packet\n");
		/* Prepare for more SETUP Packets */
		dwc3_device_ep0_out_start(dwc3_dev);
		break;

	case EP0_STALL:
		DBG("EP0 STALLed, should not get here\n");
		break;

	case EP0_IDLE:
		DBG("EP0 IDLE, should not get here\n");
		break;
	}
out:
	dwc3_print_ep0_state(dwc3_dev);
	return;
}

/**
 * This routine handles EP0 transfers.
 *
 * This routine gets the request corresponding to the current EP0 transfer. If
 * EP0 is in IDLE state, it calls dwc3_gadget_do_setup() to begin processing
 * the next Setup request, otherwise it calls dwc3_handle_ep0() to handle
 * the next stage of the current transfer.
 */
void dwc3_handle_ep0_xfer(dwc3_device_t *dwc3_dev, u32 event)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	dwc3_device_req_t *req = NULL;

	DBG("%s()\n", __func__);
	req = dwc3_gadget_get_request(dwc3_dev, ep0);

	if (dwc3_dev->ep0state == EP0_IDLE) {
		dwc3_print_ep0_state(dwc3_dev);
		DBG("IDLE EP%d-%s\n", ep0->dwc_ep.num,
			   (ep0->dwc_ep.is_in ? "IN" : "OUT"));
		dwc3_dev->request_config = 0;
		dwc3_gadget_do_setup(dwc3_dev);
	} else {
		dwc3_handle_ep0(dwc3_dev, req, event);
	}
}
