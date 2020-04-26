/* ==========================================================================
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */
#include <dev/udc.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"

#include "dwc3.h"

#undef log_level
#define log_level 3

/**
 * This interrupt indicates that the USB link state has changed to L2, U3, or
 * (if L1 Hibernation is enabled) L1, and software intervention is required.
 */
static int handle_hiber_req_intr(dwc3_device_t *dwc3_dev, u32 event)
{
	int hird, is_superspeed;
	u32 state;

	INF("HIBERNATION REQUEST\n");

	is_superspeed = !!(event & DWC_DEVT_HIBER_SS_BIT);
	state = event >> DWC_DEVT_HIBER_STATE_SHIFT &
		DWC_DEVT_HIBER_STATE_BITS >> DWC_DEVT_HIBER_STATE_SHIFT;
	INF("state=0x%x, is_superspeed=%d\n",
		   state, is_superspeed);

	/* TODO: Workaround */
	if (!(state == DWC_LINK_STATE_U3 ||
	      //state == DWC_LINK_STATE_SS_DIS ||
	      (!is_superspeed && (state == DWC_LINK_STATE_SLEEP)))) {
		INF("HIBERNATION not handled\n");
		return 1;
	} /* End workaround */

	hird = event >> DWC_DEVT_HIBER_HIRD_SHIFT &
	       DWC_DEVT_HIBER_HIRD_BITS >> DWC_DEVT_HIBER_HIRD_SHIFT;
	INF("%s(%u), state=%d hird=%d ss=%d\n",
		   __func__, event, state, hird, is_superspeed);

	/* Enter hibernation if supported */
	if (dwc3_dev->dwc3->core_params->hibernate &&
	    (dwc3_dev->dwc3->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT) {
		/* Tell kernel thread to save state and enter hibernation */
		dwc3_dev->dwc3->hibernate = DWC_HIBER_ENTER_SAVE;
		return 1;
	}

	return 0;
}

/**
 * This interrupt indicates that the device has been disconnected.
 */
static int handle_disconnect_intr(dwc3_device_t *dwc3_dev)
{
	dwc3_core_t *dev = dwc3_dev->dwc3;
	u32 temp;

	INF("DISCONNECT\n");

	/* Must set DCTL[8:5] to 5, according to 2.40a and later databook.
	 * Assuming here this will be harmless on earlier cores.
	 */
	temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	temp &= ~DWC_DCTL_ULST_CHNG_REQ_BITS;
	temp |= DWC_LINK_STATE_REQ_RX_DETECT << DWC_DCTL_ULST_CHNG_REQ_SHIFT;
	dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);

	dwc3_clr_eps_enabled(dwc3_dev);
	dwc3_device_stop(dwc3_dev);

	/* Enter hibernation if supported */
	if (dev->core_params->hibernate && dev->core_params->hiberdisc &&
	    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT) {
		/* Tell kernel thread to enter hibernation */
		dev->hibernate = DWC_HIBER_ENTER_NOSAVE;
		return 1;
	}

	return 0;
}

/**
 * This interrupt occurs when a USB Reset is detected. When the USB Reset
 * Interrupt occurs, all transfers are stopped and the device state is set
 * to DEFAULT.
 */
static void handle_usb_reset_intr(dwc3_device_t *dwc3_dev)
{
	dwc3_device_ep_t *ep;
	int i;

	INF("USB RESET (dctl=0x%08x)\n", dwc3_readl(&dwc3_dev->dev_regs->dctl));

	/* If UsbReset comes without Disconnect first, fake it, because the
	 * gadget may need to see a disconnect first. The Synopsys UAS gadget
	 * needs this.
	 */
#if 1
	if (dwc3_dev->state != DWC_STATE_UNCONNECTED) {
		INF("fake DISCONNECT\n");
		dwc3_clr_eps_enabled(dwc3_dev);
		dwc3_device_stop(dwc3_dev);
	} else
#endif
	{
		/* Stop any active xfers on the non-EP0 endpoints */
		dwc3_stop_all_xfers(dwc3_dev);
		dwc3_clr_eps_enabled(dwc3_dev);
	}

	/* Clear stall on each EP */
	for (i = 0; i < dwc3_dev->num_in_eps; i++) {
		ep = dwc3_dev->in_ep[i];
		if (ep->dwc_ep.active && ep->dwc_ep.stopped)
			dwc3_device_ep_clear_stall(dwc3_dev, ep);
	}
	for (i = 0; i < dwc3_dev->num_out_eps; i++) {
		ep = dwc3_dev->out_ep[i];
		if (ep->dwc_ep.active && ep->dwc_ep.stopped)
			dwc3_device_ep_clear_stall(dwc3_dev, ep);
	}

	/* Set Device Address to 0 */
	dwc3_set_address(dwc3_dev, 0);

	dwc3_dev->remote_wakeup_enable = 0;
	dwc3_dev->ltm_enable = 0;
}

/**
 * This interrupt occurs when a Connect Done is detected.
 * Read the device status register and set the device speed in the data
 * structure. Set up EP0 to receive SETUP packets.
 */
void dwc3_handle_connect_done_intr(dwc3_device_t *dwc3_dev)
{
	dwc3_core_t *dev = dwc3_dev->dwc3;
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	int speed;
	u32 temp;
	u32 diepcfg0, doepcfg0, diepcfg1, doepcfg1;
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	/* The timing on reconnect after hibernation is so tight that we
	 * cannot afford the overhead of printing this to the dmesg log!
	 */
	if (!dev->core_params->hibernate ||
	    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) !=
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT)
		INF("CONNECT\n");

	dwc3_print_ep0_state(dwc3_dev);

	ep0->dwc_ep.stopped = 0;
	speed = dwc3_get_device_speed(dwc3_dev);
	dwc3_dev->speed = speed;

	/* If LPM enable was requested */
	if (dev->core_params->lpmctl) {
		/* Set LPMCap bit */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		temp |= DWC_DCFG_LPM_CAP_BIT;
		dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);

		temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);

		/* HIRD threshold must be set to 0 when in SS */
		temp &= ~DWC_DCTL_HIRD_THR_BITS;

		/* If not SS */
		if (speed != USB_SPEED_SUPER) {
			/* If hibernation enabled and non-default threshold */
			if (dev->core_params->hibernate &&
			    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
			    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT &&
			    dev->hird_thresh) {
				/* Set requested threshold value */
				temp |= dev->hird_thresh << DWC_DCTL_HIRD_THR_SHIFT &
						DWC_DCTL_HIRD_THR_BITS;
			} else {
				/* Set default threshold value */
				temp |= 0x1c << DWC_DCTL_HIRD_THR_SHIFT;
			}
		}

		dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);
	}

	/* Set the MPS of EP0 based on the connection speed */
	switch (speed) {
	case USB_SPEED_SUPER:
		dwc3_dev->ep0->dwc_ep.maxpacket = 512;
		break;

	case USB_SPEED_HIGH:
	case USB_SPEED_FULL:
		dwc3_dev->ep0->dwc_ep.maxpacket = 64;
		break;

	case USB_SPEED_LOW:
		dwc3_dev->ep0->dwc_ep.maxpacket = 8;
		break;
	}

	diepcfg0 = DWC_USB3_EP_TYPE_CONTROL << DWC_EPCFG0_EPTYPE_SHIFT;
	diepcfg0 |= DWC_CFG_ACTION_MODIFY << DWC_EPCFG0_CFG_ACTION_SHIFT;
	diepcfg1 = DWC_EPCFG1_XFER_CMPL_BIT | //DWC_EPCFG1_XFER_IN_PROG_BIT |
		   DWC_EPCFG1_XFER_NRDY_BIT | DWC_EPCFG1_EP_DIR_BIT;

	doepcfg0 = DWC_USB3_EP_TYPE_CONTROL << DWC_EPCFG0_EPTYPE_SHIFT;
	doepcfg0 |= DWC_CFG_ACTION_MODIFY << DWC_EPCFG0_CFG_ACTION_SHIFT;
	doepcfg1 = DWC_EPCFG1_XFER_CMPL_BIT | //DWC_EPCFG1_XFER_IN_PROG_BIT |
		   DWC_EPCFG1_XFER_NRDY_BIT;

	diepcfg0 |= dwc3_dev->ep0->dwc_ep.maxpacket << DWC_EPCFG0_MPS_SHIFT;
	doepcfg0 |= dwc3_dev->ep0->dwc_ep.maxpacket << DWC_EPCFG0_MPS_SHIFT;

	diepcfg0 |= ep0->dwc_ep.tx_fifo_num << DWC_EPCFG0_TXFNUM_SHIFT;

	dwc3_dis_usb2_suspend(dwc3_dev);

	/* Issue "DEPCFG" command to EP0-OUT */
	ep_reg = &dwc3_dev->out_ep_regs[0];
	dwc3_dep_cfg(dwc3_dev, ep_reg, doepcfg0, doepcfg1, 0);

	/* Issue "DEPCFG" command to EP0-IN */
	ep_reg = &dwc3_dev->in_ep_regs[0];
	dwc3_dep_cfg(dwc3_dev, ep_reg, diepcfg0, diepcfg1, 0);

	dwc3_ena_usb2_suspend(dwc3_dev);

	if (dwc3_dev->state == DWC_STATE_UNCONNECTED)
		dwc3_dev->state = DWC_STATE_DEFAULT;

	/* Inform the gadget of the connection and the speed */
	//dwc3_gadget_connect(dwc3_dev, speed);

	if (dev->hiber_wait_connect) {
		/* Already did 'Perform the steps in Section 9.1.3
		 * "Initialization on Connect Done" in [DWSS]'.
		 */

		DBG("Hibernation wait connect\n");
		dev->hiber_wait_connect = 0;
		//dwc3_exit_hibernation_after_connect(dwc3_dev, 0);
	}
}

/**
 * This interrupt indicates that the USB link state has changed.
 */
static void handle_link_status_change_intr(dwc3_device_t *dwc3_dev)
{
	int state;
	int speed;

	if (dwc3_dev->dwc3->snpsid >= 0x5533230a)
		return;

	state = dwc3_device_get_link_state(dwc3_dev);
	speed = dwc3_get_device_speed(dwc3_dev);
	dwc3_dev->speed = speed;
	DBG("LINK state=%d speed=%d\n", state, speed);

	switch (state) {
	case DWC_LINK_STATE_U0:
		if (dwc3_dev->dwc3->hiber_wait_u0) {
			dwc3_dev->speed = speed;
			if (dwc3_dev->remote_wakeup_enable)
				dwc3_device_remote_wake(dwc3_dev, 0);
			dwc3_dev->dwc3->hiber_wait_u0 = 0;
		}

		/* If transitioning from 3->0 */
		if (dwc3_dev->link_state == DWC_LINK_STATE_U3) {
			DBG("Enabling function remote wake\n");
			dwc3_dev->wkup_rdy = 1;
		} else {
			dwc3_dev->wkup_rdy = 0;
		}

		dwc3_dev->link_state = state;
		break;

	case DWC_LINK_STATE_U3:
		/* If transitioning to 3 */
		if (dwc3_dev->link_state != DWC_LINK_STATE_U3)
			dwc3_gadget_suspend(dwc3_dev);
		/* FALL-THRU */

	default:
		dwc3_dev->link_state = state;
		dwc3_dev->wkup_rdy = 0;
		break;
	}
}

/**
 * This interrupt indicates that the DWC_usb3 controller has detected a
 * resume or remote wakeup sequence.
 */
static void handle_wakeup_detected_intr(dwc3_device_t *dwc3_dev)
{
	int state;

	DBG("++Resume or Remote Wakeup Detected Interrupt++\n");
	DBG("DSTS=0x%01x\n", dwc3_readl(&dwc3_dev->dev_regs->dsts));
	state = dwc3_device_get_link_state(dwc3_dev);
	dwc3_dev->link_state = state;

	if (state == DWC_LINK_STATE_U0)
		dwc3_gadget_resume(dwc3_dev);
}

/**
 * This interrupt indicates that a U3/L2-L1 Suspend event has occurred.
 */
static void handle_u3_l2l1_susp_intr(dwc3_device_t *dwc3_dev)
{
	int state;

	if (dwc3_dev->dwc3->snpsid < 0x5533230a)
		return;

	state = dwc3_device_get_link_state(dwc3_dev);
	DBG("LINK state=%d\n", state);

	switch (state) {
	case DWC_LINK_STATE_U0:
		/* If transitioning from 3->0 */
		if (dwc3_dev->link_state == DWC_LINK_STATE_U3) {
			DBG("Enabling function remote wake\n");
			dwc3_dev->wkup_rdy = 1;
		} else {
			dwc3_dev->wkup_rdy = 0;
		}

		dwc3_dev->link_state = state;
		break;

	case DWC_LINK_STATE_U3:
		/* If transitioning to 3 */
		if (dwc3_dev->link_state != DWC_LINK_STATE_U3)
			dwc3_gadget_suspend(dwc3_dev);
		/* FALL-THRU */

	default:
		dwc3_dev->link_state = state;
		dwc3_dev->wkup_rdy = 0;
		break;
	}
}

/**
 * This routine handles the SOF Interrupts. At this time the SOF Interrupt
 * is disabled.
 */
static void handle_sof_intr(dwc3_device_t *dwc3_dev)
{
	DBG("SOF\n");
}

/**
 * This interrupt indicates that an EP has a pending interrupt.
 */
void dwc3_handle_ep_intr(dwc3_device_t *dwc3_dev, int physep, u32 event)
{
	dwc3_device_ep_t *ep;
	int epnum, is_in, temp;
	const char *dir;

	DBG("%s(%lx,%d,0x%08x)\n", __func__,
		   (unsigned long)dwc3_dev, physep, event);

	/* Physical Out EPs are even, physical In EPs are odd */
	is_in = physep & 1;
	epnum = physep >> 1 & 0xf;

	/* Get EP pointer */
	if (is_in) {
		ep = dwc3_get_in_ep(dwc3_dev, epnum);
		dir = "IN";
	} else {
		ep = dwc3_get_out_ep(dwc3_dev, epnum);
		dir = "OUT";
	}

	DBG("%s EP intr\n", dir);

	VDBG("EP%d-%s: type=%d mps=%d\n",
		   ep->dwc_ep.num, (ep->dwc_ep.is_in ? "IN" : "OUT"),
		   ep->dwc_ep.type, ep->dwc_ep.maxpacket);

	temp = dwc3_dev->dwc3->hibernate;
	if (temp >= DWC_HIBER_SLEEPING && temp != DWC_HIBER_WAIT_U0 &&
	    temp != DWC_HIBER_SS_DIS_QUIRK) {
		INF("EP%d-%s: got event 0x%08x while hibernating\n",
			  ep->dwc_ep.num, (ep->dwc_ep.is_in ? "IN" : "OUT"),
			  event);
		return;
	}

	switch (event & DWC_DEPEVT_INTTYPE_BITS) {
	case DWC_DEPEVT_XFER_CMPL << DWC_DEPEVT_INTTYPE_SHIFT:
		VDBG("[EP%d] %s xfer complete\n",
			   epnum, dir);
		ep->dwc_ep.xfer_started = 0;

		if (ep->dwc_ep.type != USB_ENDPOINT_XFER_ISOC) {
			/* Complete the transfer */
			if (epnum == 0)
				dwc3_handle_ep0_xfer(dwc3_dev, event);
			else
				dwc3_complete_request(dwc3_dev, ep, event);
		} else {
			DBG("[EP%d] %s xfer complete for ISOC EP!\n",
				   epnum, dir);
		}

		break;

	case DWC_DEPEVT_XFER_IN_PROG << DWC_DEPEVT_INTTYPE_SHIFT:
		VDBG("[EP%d] %s xfer in progress\n",
			   epnum, dir);

		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
#if 0
			u32 now_uf, evt_uf;

			/* Don't complete transfer if it's way in the past */
			now_uf = dwc_usb3_get_frame(dwc3_dev);
			evt_uf = dwc_usb3_get_eventsofn(event);
			now_uf &= DWC_DEPEVT_ISOC_UFRAME_NUM_BITS >>
				  DWC_DEPEVT_ISOC_UFRAME_NUM_SHIFT;
			if (now_uf - evt_uf < 0x8000)
#endif
				/* Complete the transfer */
				dwc3_complete_request(dwc3_dev, ep, event);
		} else {
//			DBG("[EP%d] %s xfer in progress for non-ISOC EP!\n",
//				 epnum, dir);

			/* Complete the transfer */
			if (epnum == 0)
				dwc3_handle_ep0_xfer(dwc3_dev, event);
			else
				dwc3_complete_request(dwc3_dev, ep, event);
		}

		break;

	case DWC_DEPEVT_XFER_NRDY << DWC_DEPEVT_INTTYPE_SHIFT:
		DBG("[EP%d] %s xfer not ready\n",
			   epnum, dir);

		if (epnum == 0) {
			switch (dwc3_dev->ep0state) {
			case EP0_IN_WAIT_GADGET:
			case EP0_IN_WAIT_NRDY:
				if (is_in)
					dwc3_handle_ep0_xfer(dwc3_dev, event);
				break;
			case EP0_OUT_WAIT_GADGET:
			case EP0_OUT_WAIT_NRDY:
				if (!is_in)
					dwc3_handle_ep0_xfer(dwc3_dev, event);
				break;
			default:
				break;
			}
		} else if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
			DBG("[EP%d] %s xfer not ready for ISOC EP!\n",
				     epnum, dir);
			if (!ep->dwc_ep.xfer_started)
				dwc3_gadget_isoc_ep_start(dwc3_dev, ep, event);
		}

		break;

	case DWC_DEPEVT_FIFOXRUN << DWC_DEPEVT_INTTYPE_SHIFT:
		ERR("Error! [EP%d] %s FIFO Underrun Error!\n",
			   epnum, dir);
		break;

	case DWC_DEPEVT_EPCMD_CMPL << DWC_DEPEVT_INTTYPE_SHIFT:
		ERR("Error! [EP%d] %s Command Complete!\n",
			   epnum, dir);
		break;

	default:
		ERR("Error! [EP%d] %s Unknown event!\n",
			   epnum, dir);
		break;
	}
}

/**
 * dwc3_dev interrupt handler.
 *
 * The dwc3_dev handles the device interrupts. Many conditions can cause a
 * device interrupt. When an interrupt occurs, the device interrupt
 * service routine determines the cause of the interrupt and
 * dispatches handling to the appropriate routine.
 */
int dwc3_handle_dev_intr(dwc3_device_t *dwc3_dev, u32 event)
{
	u32 dint = event >> DWC_DEVT_SHIFT & DWC_DEVT_BITS >> DWC_DEVT_SHIFT;
	int temp, ret = 0;

	VDBG("%s() event=%08x\n", __func__, event);

	temp = dwc3_dev->dwc3->hibernate;
	if (temp >= DWC_HIBER_SLEEPING && temp != DWC_HIBER_WAIT_U0 &&
	    temp != DWC_HIBER_SS_DIS_QUIRK) {
		INF("Device: got event 0x%08x while hibernating\n",
			  event);
		return 0;
	}

	switch (dint) {
	case DWC_DEVT_DISCONN:
		DBG("Disconnect\n");
		ret = handle_disconnect_intr(dwc3_dev);
		break;

	case DWC_DEVT_USBRESET:
		DBG("USB Reset\n");
		handle_usb_reset_intr(dwc3_dev);
		break;

	case DWC_DEVT_CONNDONE:
		DBG("Connect Done\n");
		dwc3_handle_connect_done_intr(dwc3_dev);
		break;

	case DWC_DEVT_ULST_CHNG:
		DBG("Link Status Change\n");
		handle_link_status_change_intr(dwc3_dev);
		break;

	case DWC_DEVT_WKUP:
		DBG("Wakeup\n");
		handle_wakeup_detected_intr(dwc3_dev);
		break;

	case DWC_DEVT_HIBER_REQ:
		DBG("Hibernation Request\n");
		ret = handle_hiber_req_intr(dwc3_dev, event);
		break;

	case DWC_DEVT_U3_L2L1_SUSP:
		DBG("U3/L2-L1 Suspend Event\n");
		handle_u3_l2l1_susp_intr(dwc3_dev);
		break;

	case DWC_DEVT_SOF:
		DBG("Start of Frame\n");
		handle_sof_intr(dwc3_dev);
		break;

	case DWC_DEVT_ERRATICERR:
		DBG("Erratic Error\n");
		break;

	case DWC_DEVT_CMD_CMPL:
		DBG("Command Complete\n");
		break;

	case DWC_DEVT_OVERFLOW:
		DBG("Overflow\n");
		break;

	default:
		DBG("Unknown event!\n");
		break;
	}

	return ret;
}
