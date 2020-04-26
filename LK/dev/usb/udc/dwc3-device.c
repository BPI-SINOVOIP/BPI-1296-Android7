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
#include <stdlib.h>
#include <dev/udc.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"

#include "dwc3.h"

#undef log_level
#define log_level 3

/**
 * This routine allocates the TRBs for an EP.
 *
 * @param ep            The EP for the allocation.
 * @param num_trbs      Number of TRBs to allocate.
 * @param trb_type      Type of the TRB.
 * @param iso_intvl     bInterval if this is an Isoc EP.
 * @param link          True if the TRBs should be linked in a circular chain
 *                      (only supported for Isoc EPs for now).
 * @param trbs_dma_ret  The DMA address of the allocation is returned through
 *                      this pointer.
 * @return              The address of the allocated memory, or NULL if the
 *                      allocation fails.
 */
dwc3_dma_desc_t *dwc3_device_trb_alloc(dwc3_device_ep_t *ep, int num_trbs,
		int trb_type, int iso_intvl, int link, dwc_dma_t *trbs_dma_ret)
{
	dwc3_dma_desc_t *trbs, *cur_trb;
	dwc_dma_t trbs_dma;
	int size, i;

	size = num_trbs * 16;
	if (link)
		size += 16;


	trbs = cur_trb = dwc3_gadget_alloc_dma(ep, size, &trbs_dma);

	DBG("trb_type=%d, num_trbs=%d, trbs@%p, cur_trb@%p\n", trb_type, num_trbs, trbs, cur_trb);
	if (!trbs)
		return NULL;

	/* Now initialize the TRBs */
	for (i = 0; i < num_trbs; i++, cur_trb++) {
		if (trb_type == USB_ENDPOINT_XFER_ISOC) {
				dwc3_fill_desc(cur_trb, 0, 0, 0,
						   DWC_DSCCTL_TRBCTL_ISOC_1ST,
						   DWC_DSCCTL_IOC_BIT |
						   DWC_DSCCTL_IMI_BIT |
						   DWC_DSCCTL_CSP_BIT, 0);
		}

		/* For types other than Isoc, the TRBs are initialized just
		 * before the transfer is started.
		 */
	}

	if (link) {
		dwc3_fill_desc(cur_trb, trbs_dma, 0, 0,
				   DWC_DSCCTL_TRBCTL_LINK, 0, 1);
	}

	/* Init the dev_ep structure */
	ep->dwc_ep.dma_desc = trbs;
	ep->dwc_ep.dma_desc_dma = trbs_dma;
	ep->dwc_ep.desc_size = size;
	ep->dwc_ep.desc_link = link;
	ep->dwc_ep.num_desc = num_trbs;
	ep->dwc_ep.desc_avail = num_trbs;
	ep->dwc_ep.desc_idx = 0;
	ep->dwc_ep.hiber_desc_idx = 0;

	if (trbs_dma_ret)
		*trbs_dma_ret = trbs_dma;
	return trbs;
}

/**
 * This routine frees the TRBs allocated by dwc3_device_trb_alloc().
 *
 * @param ep    The EP for the allocation.
 */
void dwc3_device_trb_free(dwc3_device_ep_t *ep)
{
	dwc3_dma_desc_t *trbs;
	dwc_dma_t trbs_dma;
	int size;

	if (ep->dwc_ep.dma_desc) {
		trbs = ep->dwc_ep.dma_desc;
		trbs_dma = ep->dwc_ep.dma_desc_dma;
		size = ep->dwc_ep.desc_size;
		ep->dwc_ep.dma_desc = NULL;
		ep->dwc_ep.dma_desc_dma = 0;

		dwc3_gadget_free_dma(ep, size, trbs, trbs_dma);
	}
}

/**
 * This routine assigns and fills in the TRBs for a request.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP for the transfer.
 * @param req   The request that needs the TRBs.
 */
void dwc3_device_fill_trbs(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			    dwc3_device_req_t *req)
{
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	u32 len, tlen, pkts, ctrl;
	int i;

	if (ep == dwc3_dev->ep0)
		return;

	DBG("%s() [ep%d-%s @0x%08lx] dev_reg@0x%08lx\n", __func__,
			ep->dwc_ep.num, ep->dwc_ep.is_in? "in":"out",
			(unsigned long)ep, (unsigned long)req);

	/* Get the next DMA Descriptor (TRB) for this EP */
	desc = ep->dwc_ep.dma_desc + ep->dwc_ep.desc_idx * req->dwc_req.numbuf;
	desc_dma = (dwc_dma_t)((unsigned long)ep->dwc_ep.dma_desc_dma +
		(unsigned long)ep->dwc_ep.desc_idx * req->dwc_req.numbuf * 16);

	if (++ep->dwc_ep.desc_idx >= ep->dwc_ep.num_desc)
		ep->dwc_ep.desc_idx = 0;
	ep->dwc_ep.desc_avail--;

	DBG("desc@0x%08lx\n", (unsigned long)desc);

	req->dwc_req.trb = desc;
	req->dwc_req.trbdma = desc_dma;

	pkts = 0;

	if (ep->dwc_ep.is_in) {
		/* For IN, TRB length is just xfer length */
		len = req->dwc_req.length;

		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC &&
				dwc3_dev->speed == USB_SPEED_HIGH) {
			pkts = (len + ep->dwc_ep.maxpacket - 1)
					/ ep->dwc_ep.maxpacket;
			if (pkts)
				pkts--;
		}
	} else {
		/* For OUT, TRB length must be multiple of maxpacket */
		if ((ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC ||
				ep->dwc_ep.type == USB_ENDPOINT_XFER_INT) &&
						ep->dwc_ep.maxpacket != 1024)
			/* Might not be power of 2, so use (expensive?)
			 * divide/multiply
			 */
			len = ((req->dwc_req.length + ep->dwc_ep.maxpacket - 1)
			       / ep->dwc_ep.maxpacket) * ep->dwc_ep.maxpacket;
		else
			/* Must be power of 2, use cheap AND */
			len = (req->dwc_req.length + ep->dwc_ep.maxpacket - 1)
			      & ~(ep->dwc_ep.maxpacket - 1);

		req->dwc_req.length = len;
	}

	/* DMA Descriptor Setup */
	for (i = 0; i < req->dwc_req.numbuf; i++, desc++) {
		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
			if (i != req->dwc_req.numbuf - 1) {
				tlen = req->dwc_req.buflen[i];
				len -= tlen;
			} else {
				tlen = len;
			}

			if (i == 0)
				tlen |= pkts << DWC_DSCSTS_PCM1_SHIFT;

			dwc3_fill_desc(desc, req->dwc_req.bufdma[i], tlen,
					   0, 0, 0, i != 0);
		} else {
			int type = DWC_DSCCTL_TRBCTL_NORMAL;
			if (i != req->dwc_req.numbuf - 1) {
				ctrl = 0;
				tlen = req->dwc_req.buflen[i];
				len -= tlen;
			} else {
				ctrl = DWC_DSCCTL_LST_BIT | DWC_DSCCTL_IOC_BIT;
				tlen = len;
			}

			/* If core is version 3.00a or later */
			if ((dwc3_dev->dwc3->snpsid & 0xffff) >= 0x300a) {
				/* The ZERO flag will be set if we are
				 * expected to do a ZLP after the end
				 * of the transfer if needed. This
				 * means if the transfer length is MPS
				 * aligned, do a ZLP. */
				if (ep->dwc_ep.is_in && (req->dwc_req.flags & DWC_PCD_REQ_ZERO) &&
					(tlen != 0) && ((tlen % ep->dwc_ep.maxpacket) == 0)) {
					type = DWC_DSCCTL_TRBCTL_NORMAL_ZLP;
				}
			}

			dwc3_fill_desc(desc, req->dwc_req.bufdma[i], tlen,
					   req->dwc_req.stream, type, ctrl,
					   i != 0);
		}
	}

	/* Must do this last! */
	desc = req->dwc_req.trb;
	dwc3_enable_desc(desc);
}

/**
 * This routine configures EP0 OUT to receive SETUP packets and configures
 * EP0 IN for transmitting packets.
 *
 * @param dwc3_dev           Programming view of DWC_usb3 peripheral controller.
 * @param restore       True if restoring endpoint state after hibernation.
 */
void dwc3_ep0_activate(dwc3_device_t *dwc3_dev, int restore)
{
	u32 diepcfg0, doepcfg0, diepcfg1, doepcfg1;
	u32 diepcfg2 = 0, doepcfg2 = 0;
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("%s()\n", __func__);

	diepcfg0 = DWC_USB3_EP_TYPE_CONTROL << DWC_EPCFG0_EPTYPE_SHIFT;
	diepcfg1 = DWC_EPCFG1_XFER_CMPL_BIT | //DWC_EPCFG1_XFER_IN_PROG_BIT |
		   DWC_EPCFG1_XFER_NRDY_BIT | DWC_EPCFG1_EP_DIR_BIT;

	doepcfg0 = DWC_USB3_EP_TYPE_CONTROL << DWC_EPCFG0_EPTYPE_SHIFT;
	doepcfg1 = DWC_EPCFG1_XFER_CMPL_BIT | //DWC_EPCFG1_XFER_IN_PROG_BIT |
		   DWC_EPCFG1_XFER_NRDY_BIT;

	/* Default to MPS of 512 (will reconfigure after ConnectDone event) */
	diepcfg0 |= 512 << DWC_EPCFG0_MPS_SHIFT;
	doepcfg0 |= 512 << DWC_EPCFG0_MPS_SHIFT;

	diepcfg0 |= dwc3_dev->ep0->dwc_ep.tx_fifo_num << DWC_EPCFG0_TXFNUM_SHIFT;

	if (restore) {
		diepcfg0 |= DWC_CFG_ACTION_RESTORE
					<< DWC_EPCFG0_CFG_ACTION_SHIFT;
		diepcfg2 = dwc3_dev->ep0_in_save_state;
		DBG("IN restore state=%08x\n", diepcfg2);
		doepcfg0 |= DWC_CFG_ACTION_RESTORE
					<< DWC_EPCFG0_CFG_ACTION_SHIFT;
		doepcfg2 = dwc3_dev->ep0_out_save_state;
		DBG("OUT restore state=%08x\n", doepcfg2);
	}

	/*
	 * Issue "DEPCFG" command to EP0-OUT
	 */

	ep_reg = &dwc3_dev->out_ep_regs[0];
	dwc3_dis_usb2_suspend(dwc3_dev);

	DBG("config EP0 %p\n", ep_reg);

	/* If core is version 1.09a or later */
	if ((dwc3_dev->dwc3->snpsid & 0xffff) >= 0x109a) {
		/* Must issue DEPSTRTNEWCFG command first */
		dwc3_dep_startnewcfg(dwc3_dev, ep_reg, 0);
	}

	dwc3_dep_cfg(dwc3_dev, ep_reg, doepcfg0, doepcfg1, doepcfg2);

	/*
	 * Issue "DEPSTRMCFG" command to EP0-OUT
	 */

	/* One stream */
	dwc3_dep_xfercfg(dwc3_dev, ep_reg, 1);

	/*
	 * Issue "DEPCFG" command to EP0-IN
	 */

	ep_reg = &dwc3_dev->in_ep_regs[0];
	dwc3_dep_cfg(dwc3_dev, ep_reg, diepcfg0, diepcfg1, diepcfg2);

	/*
	 * Issue "DEPSTRMCFG" command to EP0-IN
	 */

	/* One stream */
	dwc3_dep_xfercfg(dwc3_dev, ep_reg, 1);

	dwc3_ena_usb2_suspend(dwc3_dev);
	dwc3_dev->ep0->dwc_ep.active = 1;
}

/**
 * This routine activates an EP. The Device EP control registers for the EP
 * are configured as defined in the EP structure.
 *
 * @param dwc3_dev           Programming view of DWC_usb3 peripheral controller.
 * @param ep            The EP to activate.
 * @param restore       True if restoring endpoint state after hibernation.
 */
void dwc3_ep_activate(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			  int restore)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg, *ep0_reg;
	u32 depcfg0, depcfg1, depcfg2 = 0;

	DBG("%s() EP%d-%s dev_ep@0x%08lx\n", __func__, ep->dwc_ep.num,
		   (ep->dwc_ep.is_in ? "IN" : "OUT"), (unsigned long)ep);

	ep->dwc_ep.hiber_desc_idx = 0;

#ifdef DWC_STAR_9000463548_WORKAROUND
	if (dwc3_dev->configuring)
		goto skip;
#endif
	/*
	 * Get the appropriate EP registers
	 */
	if (ep->dwc_ep.is_in)
		ep_reg = ep->dwc_ep.in_ep_reg;
	else
		ep_reg = ep->dwc_ep.out_ep_reg;

	dwc3_dis_usb2_suspend(dwc3_dev);

	/* If this is first EP enable (ie. start of a new configuration) */
	if (!dwc3_dev->eps_enabled) {

#ifdef DWC_STAR_9000463548_WORKAROUND
		dwc3_dev_ep_regs_t __iomem *eptmp_reg;
		dwc3_device_ep_t *eptmp;
		int i;

		/* For the workaround, we must wait for all EndXfers on all EPs
		 * to complete before continuing
		 */
		for (i = 0; i < dwc3_dev->num_in_eps; i++) {
			eptmp = dwc3_dev->in_ep[i];
			DBG("DWC IN EP%d=%lx tri-in=%d\n",
				   i, (unsigned long)eptmp, eptmp->dwc_ep.tri_in);
			if (eptmp->dwc_ep.tri_in) {
				eptmp_reg = eptmp->dwc_ep.in_ep_reg;
				eptmp->dwc_ep.condition = 0;
				dwc3_dep_wait_endxfer(dwc3_dev, eptmp_reg,
						&eptmp->dwc_ep.condition);
				eptmp->dwc_ep.tri_in = 0;
			}
		}

		for (i = 0; i < dwc3_dev->num_out_eps; i++) {
			eptmp = dwc3_dev->out_ep[i];
			DBG("DWC OUT EP%d=%lx tri-out=%d\n",
				   i, (unsigned long)eptmp, eptmp->dwc_ep.tri_out);
			if (eptmp->dwc_ep.tri_out) {
				eptmp_reg = eptmp->dwc_ep.out_ep_reg;
				eptmp->dwc_ep.condition = 0;
				dwc3_dep_wait_endxfer(dwc3_dev, eptmp_reg,
						&eptmp->dwc_ep.condition);
				eptmp->dwc_ep.tri_out = 0;
			}
		}
#endif
		dwc3_dev->eps_enabled = 1;

		/* NOTE: When setting a new configuration, we must issue a
		 * "DEPCFG" command to physical EP1 (logical EP0-IN) first.
		 * This resets the core's Tx FIFO mapping table
		 */
		depcfg0 = DWC_USB3_EP_TYPE_CONTROL << DWC_EPCFG0_EPTYPE_SHIFT;
		depcfg0 |= DWC_CFG_ACTION_MODIFY << DWC_EPCFG0_CFG_ACTION_SHIFT;
		depcfg1 = DWC_EPCFG1_XFER_CMPL_BIT | DWC_EPCFG1_XFER_IN_PROG_BIT
			| DWC_EPCFG1_XFER_NRDY_BIT | DWC_EPCFG1_EP_DIR_BIT;

		switch (dwc3_dev->speed) {
		case USB_SPEED_SUPER:
			depcfg0 |= 512 << DWC_EPCFG0_MPS_SHIFT;
			break;

		case USB_SPEED_HIGH:
		case USB_SPEED_FULL:
			depcfg0 |= 64 << DWC_EPCFG0_MPS_SHIFT;
			break;

		case USB_SPEED_LOW:
			depcfg0 |= 8 << DWC_EPCFG0_MPS_SHIFT;
			break;
		}

		ep0_reg = &dwc3_dev->in_ep_regs[0];
		//dwc3_dep_cfg(dwc3_dev, ep0_reg, depcfg0, depcfg1, 0);

		/* If core is version 1.09a or later */
		if ((dwc3_dev->dwc3->snpsid & 0xffff) >= 0x109a) {
			/* Must issue DEPSTRTNEWCFG command first */
			ep0_reg = &dwc3_dev->out_ep_regs[0];
			dwc3_dep_startnewcfg(dwc3_dev, ep0_reg, 2);
		}
	}

	/*
	 * Issue "DEPCFG" command to EP
	 */
	depcfg0 = ep->dwc_ep.type << DWC_EPCFG0_EPTYPE_SHIFT;
	depcfg0 |= ep->dwc_ep.maxpacket << DWC_EPCFG0_MPS_SHIFT;

	if (ep->dwc_ep.is_in) {
		depcfg0 |= ep->dwc_ep.tx_fifo_num << DWC_EPCFG0_TXFNUM_SHIFT;
	}

	if (dwc3_dev->dwc3->core_params->burst) {
		DBG("Setting maxburst to %u\n",
			   ep->dwc_ep.maxburst);
		depcfg0 |= ep->dwc_ep.maxburst << DWC_EPCFG0_BRSTSIZ_SHIFT;
	}

	if (restore) {
		depcfg0 |= DWC_CFG_ACTION_RESTORE
					<< DWC_EPCFG0_CFG_ACTION_SHIFT;
		depcfg2 = ep->dwc_ep.save_state;
	}

	depcfg1 = ep->dwc_ep.num << DWC_EPCFG1_EP_NUM_SHIFT;
	if (ep->dwc_ep.is_in)
		depcfg1 |= DWC_EPCFG1_EP_DIR_BIT;

	depcfg1 |= DWC_EPCFG1_XFER_CMPL_BIT;
	depcfg1 |= DWC_EPCFG1_XFER_IN_PROG_BIT;
	depcfg1 |= DWC_EPCFG1_XFER_NRDY_BIT;
	DBG("Setting bInterval-1 to %u\n",
		     ep->dwc_ep.intvl);
	depcfg1 |= ep->dwc_ep.intvl << DWC_EPCFG1_BINTERVAL_SHIFT;

	if (ep->dwc_ep.num_streams) {
		DBG("Setting stream-capable bit\n");
		depcfg1 |= DWC_EPCFG1_STRM_CAP_BIT;
	}

	if (dwc3_dev->dwc3->core_params->ebc) {
		if (dwc3_dev->speed == USB_SPEED_SUPER ||
		    dwc3_dev->speed == USB_SPEED_HIGH) {
			DBG("Setting EBC enable bit\n");
			depcfg1 |= DWC_EPCFG1_EBC_MODE_BIT;
		}
	}

	/* Save the DEPCFG parameters for later */
	if (ep->dwc_ep.is_in) {
		ep->dwc_ep.param0in = depcfg0 & ~DWC_EPCFG0_CFG_ACTION_BITS;
		ep->dwc_ep.param1in = depcfg1;
	} else {
		ep->dwc_ep.param0out = depcfg0 & ~DWC_EPCFG0_CFG_ACTION_BITS;
		ep->dwc_ep.param1out = depcfg1;
	}

	dwc3_dep_cfg(dwc3_dev, ep_reg, depcfg0, depcfg1, depcfg2);

	/*
	 * Issue "DEPSTRMCFG" command to EP
	 */

	/* If this EP hasn't been enabled yet in this configuration */
	if (!ep->dwc_ep.ena_once) {
		ep->dwc_ep.ena_once = 1;

		/* One stream */
		DBG("Setting 1 stream resource\n");
		dwc3_dep_xfercfg(dwc3_dev, ep_reg, 1);
	}

	dwc3_ena_usb2_suspend(dwc3_dev);

#ifdef DWC_STAR_9000463548_WORKAROUND
skip:
#endif
	/* Enable EP in DALEPENA reg */
	dwc3_enable_ep(dwc3_dev, ep);

	ep->dwc_ep.active = 1;
	ep->dwc_ep.stall_clear_flag = 0;
}

/**
 * This routine deactivates an EP.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to deactivate.
 */
static void ep_deactivate(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	u8 tri;

	DBG(" ep_deactivate EP%d-%s @0x%08lx\n", ep->dwc_ep.num,
		   (ep->dwc_ep.is_in ? "IN" : "OUT"), (unsigned long)ep);
	/*
	 * Get the appropriate EP registers
	 */
	if (ep->dwc_ep.is_in) {
		ep_reg = ep->dwc_ep.in_ep_reg;
		tri = ep->dwc_ep.tri_in;
		ep->dwc_ep.tri_in = 0;
	} else {
		ep_reg = ep->dwc_ep.out_ep_reg;
		tri = ep->dwc_ep.tri_out;
		ep->dwc_ep.tri_out = 0;
	}

	DBG("end: DWC EP@0x%08lx tri=%d\n",
		   (unsigned long)ep, tri);
	dwc3_dis_usb2_suspend(dwc3_dev);

	/* Execute clear stall command */
	dwc3_dep_cstall(dwc3_dev, ep_reg, 0);

	if (tri) {
#ifdef DWC_STAR_9000463548_WORKAROUND
		/* For the workaround, we wait until the EP is re-enabled
		 * before waiting for the end transfer to complete
		 */
		dwc3_dep_endxfer_nowait(dwc3_dev, ep_reg, tri - 1,
					    DWC_ENDXFER_FORCE);
		if (ep->dwc_ep.is_in)
			ep->dwc_ep.tri_in = tri;
		else
			ep->dwc_ep.tri_out = tri;
#else
		/* Execute end transfer command */
		ep->dwc_ep.condition = 0;
		dwc3_dep_endxfer(dwc3_dev, ep_reg, tri - 1, DWC_ENDXFER_FORCE,
				     &ep->dwc_ep.condition);
#endif
	}

	dwc3_ena_usb2_suspend(dwc3_dev);
	ep->dwc_ep.xfer_started = 0;

	/* Disable EP in DALEPENA reg */
	dwc3_disable_ep(dwc3_dev, ep);

	ep->dwc_ep.active = 0;
}

/**
 * This routine sets up a SETUP stage transfer for EP0 and starts the transfer.
 *
 * @param dwc3_dev   Programming view of the PCD.
 */
void dwc3_device_ep0_out_start(dwc3_device_t *dwc3_dev)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	u8 tri;

	/* Get the SETUP packet DMA Descriptor (TRB) */
	desc = dwc3_ep0_setup_desc(dwc3_dev);
	desc_dma = dwc3_ep0_setup_desc_dma(dwc3_dev);

	/* DMA Descriptor setup */
	dwc3_fill_desc(desc, dwc3_ep0_setup_pkt_dma(dwc3_dev),
			   8, 0,
			   DWC_DSCCTL_TRBCTL_SETUP,
			   DWC_DSCCTL_LST_BIT | DWC_DSCCTL_ISP_BIT | DWC_DSCCTL_IOC_BIT, 1);

	DBG("desc@0x%08lx (DMA 0x%08lx) fercnt=%u bptr=0x%08x:%08x\n",
		   (unsigned long)desc, desc_dma, dwc3_get_xfercnt(desc),
		   desc->bpth, desc->bptl);
	VDBG("0x%08x 0x%08x 0x%08x 0x%08x\n",
		   *((unsigned *)desc), *((unsigned *)desc + 1),
		   *((unsigned *)desc + 2), *((unsigned *)desc + 3));

	ep_reg = &dwc3_dev->out_ep_regs[0];
	dwc3_dis_usb2_suspend(dwc3_dev);

	/* Issue "DEPSTRTXFER" command to EP0-OUT */
	wmb();
	tri = dwc3_dep_startxfer(dwc3_dev, ep_reg, desc_dma, 0);
	dwc3_dev->ep0->dwc_ep.tri_out = tri + 1;

	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * This routine sets up a data/status stage transfer for EP0 and starts the
 * transfer. If dwc3_dev->ep0->dwc_ep.is_in is 0 it will be an OUT transfer,
 * otherwise it will be an IN transfer.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param req   The request to start.
 */
void dwc3_device_ep0_start_transfer(dwc3_device_t *dwc3_dev,
				     dwc3_device_req_t *req)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	u32 desc_type, len;
	u8 tri;

	DBG("[ep%d-%s @0x%08lx] req@%08lx xfer_len=%d xfer_cnt=%d xfer_buf@%08lx\n",
		ep0->dwc_ep.num, (ep0->dwc_ep.is_in ? "IN" : "OUT"), (unsigned long)ep0,
		(unsigned long)req, req->dwc_req.length, req->dwc_req.actual,
		(unsigned long)req->dwc_req.buf[0]);

	/* Get the DMA Descriptor (TRB) for this request */
	if (ep0->dwc_ep.is_in) {
		req->dwc_req.trb = dwc3_ep0_in_desc(dwc3_dev);
		req->dwc_req.trbdma = dwc3_ep0_in_desc_dma(dwc3_dev);
	} else {
		req->dwc_req.trb = dwc3_ep0_out_desc(dwc3_dev);
		req->dwc_req.trbdma = dwc3_ep0_out_desc_dma(dwc3_dev);
	}

	desc = req->dwc_req.trb;
	desc_dma = req->dwc_req.trbdma;
	dwc3_dis_usb2_suspend(dwc3_dev);

	if (ep0->dwc_ep.is_in) {
		/*
		 * Start DMA on EP0-IN
		 */
		ep_reg = ep0->dwc_ep.in_ep_reg;

		/* DMA Descriptor (TRB) setup */
		len = req->dwc_req.length;

		DBG("IN EP0STATE=%d\n", dwc3_dev->ep0state);
		dwc3_print_ep0_state(dwc3_dev);

		if (dwc3_dev->ep0state == EP0_IN_STATUS_PHASE) {
			if (ep0->dwc_ep.three_stage)
				desc_type = DWC_DSCCTL_TRBCTL_STATUS_3;
			else
				desc_type = DWC_DSCCTL_TRBCTL_STATUS_2;
		} else {
			desc_type = DWC_DSCCTL_TRBCTL_CTLDATA_1ST;
		}

		dwc3_fill_desc(desc, req->dwc_req.bufdma[0],
				   len, 0, desc_type, DWC_DSCCTL_LST_BIT, 1);
		DBG("IN desc@0x%08lx xferlen=%u bptr=0x%08x:%08x\n",
			   (unsigned long)desc, dwc3_get_xfercnt(desc),
			   desc->bpth, desc->bptl);
		VDBG("0x%08x 0x%08x 0x%08x 0x%08x\n",
			   *((unsigned *)desc), *((unsigned *)desc + 1),
			   *((unsigned *)desc + 2), *((unsigned *)desc + 3));

		/* Issue "DEPSTRTXFER" command to EP0-IN */
		wmb();
		tri = dwc3_dep_startxfer(dwc3_dev, ep_reg, desc_dma, 0);
		ep0->dwc_ep.tri_in = tri + 1;
	} else {
		/*
		 * Start DMA on EP0-OUT
		 */
		ep_reg = ep0->dwc_ep.out_ep_reg;

		/* DMA Descriptor (TRB) setup */
		len = (req->dwc_req.length + ep0->dwc_ep.maxpacket - 1) &
			~(ep0->dwc_ep.maxpacket - 1);

		DBG("OUT EP0STATE=%d\n", dwc3_dev->ep0state);
		dwc3_print_ep0_state(dwc3_dev);

		if (dwc3_dev->ep0state == EP0_OUT_STATUS_PHASE) {
			if (ep0->dwc_ep.three_stage)
				desc_type = DWC_DSCCTL_TRBCTL_STATUS_3;
			else
				desc_type = DWC_DSCCTL_TRBCTL_STATUS_2;
		} else {
			desc_type = DWC_DSCCTL_TRBCTL_CTLDATA_1ST;
		}

		dwc3_fill_desc(desc, req->dwc_req.bufdma[0],
				   len, 0, desc_type, DWC_DSCCTL_LST_BIT, 1);
		DBG("OUT desc@0x%08lx xferlen=%u bptr=0x%08x:%08x\n",
			   (unsigned long)desc, dwc3_get_xfercnt(desc),
			   desc->bpth, desc->bptl);
		VDBG("0x%08x 0x%08x 0x%08x 0x%08x\n",
			   *((unsigned *)desc), *((unsigned *)desc + 1),
			   *((unsigned *)desc + 2), *((unsigned *)desc + 3));

		/* Issue "DEPSTRTXFER" command to EP0-OUT */
		wmb();
		tri = dwc3_dep_startxfer(dwc3_dev, ep_reg, desc_dma, 0);
		ep0->dwc_ep.tri_out = tri + 1;
	}

	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * This routine continues control IN transfers started by ep0_start_transfer,
 * when the transfer does not fit in a single request.
 *
 * @param dwc3_dev Programming view of DWC_usb3 peripheral controller.
 * @param req The request to continue.
 */
void dwc3_device_ep0_continue_transfer(dwc3_device_t *dwc3_dev,
					dwc3_device_req_t *req)
{
	dwc3_device_ep_t *ep0 = dwc3_dev->ep0;
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	u8 tri;

	/* Currently the EP0 buffer size in the gadget is at least 256 bytes,
	 * and all control transfers are smaller than that, so this routine is
	 * never called to continue a transfer. However it can be called to
	 * send a 0-length packet after the end of a transfer, so the code here
	 * only supports that case.
	 */

	DBG("%s()\n", __func__);

	if (ep0->dwc_ep.is_in) {
		desc = dwc3_ep0_in_desc(dwc3_dev);
		desc_dma = dwc3_ep0_in_desc_dma(dwc3_dev);
		ep_reg = ep0->dwc_ep.in_ep_reg;

		/* DMA Descriptor Setup */
		dwc3_fill_desc(desc, req->dwc_req.bufdma[0], 0, 0,
				   DWC_DSCCTL_TRBCTL_NORMAL, DWC_DSCCTL_LST_BIT,
				   1);

		/* Make sure all writes to TRB have completed */
		wmb();

		dwc3_dis_usb2_suspend(dwc3_dev);
		tri = dwc3_dep_startxfer(dwc3_dev, ep_reg, desc_dma, 0);
		ep0->dwc_ep.tri_in = tri + 1;
		dwc3_ena_usb2_suspend(dwc3_dev);
	}
}

/**
 * This routine does the setup for a data transfer for an EP and starts
 * the transfer.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to start the transfer on.
 * @param req   The request to start.
 * @param event If non-zero, this is the first transfer for an Isoc EP, so we
 *              must calculate the starting uFrame and do a startxfer instead
 *              of an updatexfer.
 */
void dwc3_device_ep_start_transfer(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
				    dwc3_device_req_t *req, u32 event)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	u32 dsts;
	u16 current_uf, intvl, mask, now, target_uf = 0;
	u8 tri;
	u32 dcfg;

	DBG("[ep%d-%s (%d phys) @0x%08lx] max_pkt=%d req@0x%08lx"
		    " xfer_len=%d xfer_cnt=%d xfer_buf@0x%08lx\n",
		    ep->dwc_ep.num, (ep->dwc_ep.is_in ? "IN" : "OUT"),
		    ep->dwc_ep.phys, (unsigned long)ep, ep->dwc_ep.maxpacket,
		    (unsigned long)req, req->dwc_req.length,
		    req->dwc_req.actual, (unsigned long)req->dwc_req.buf[0]);

	ep->dwc_ep.hiber_desc_idx = 0;

	/* If first transfer for Isoc */
	if (event) {
		/* Get the uFrame of the host request */
		current_uf = event >> DWC_DEPEVT_ISOC_UFRAME_NUM_SHIFT &
			     DWC_DEPEVT_ISOC_UFRAME_NUM_BITS >>
					DWC_DEPEVT_ISOC_UFRAME_NUM_SHIFT;

		/* Get the EP's interval */
		intvl = 1 << ep->dwc_ep.intvl;

		/* Get the EP's interval mask */
		mask = ~(intvl - 1);

		dsts = dwc3_readl(&dwc3_dev->dev_regs->dsts);
		now = dsts >> DWC_DSTS_SOF_FN_SHIFT &
		      DWC_DSTS_SOF_FN_BITS >> DWC_DSTS_SOF_FN_SHIFT;
		if (now < (current_uf & 0x3fff))
			now += 0x4000;
		now += current_uf & 0xc000;

		/* Calculate a start time which is 2 or 4 intervals in the
		 * future
		 */
		target_uf = current_uf & mask;
again:
		if (intvl <= 8)
			target_uf += intvl << 2;
		else
			target_uf += intvl << 1;

		DBG("tgt:%1x now:%1x tgt-now:%1x\n",
			     target_uf, now, target_uf - now);
		if ((u16)(target_uf - now) >= 0x8000U)
			goto again;

		dcfg = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		DBG("dcfg:0x%1x dsts:0x%1x uf:0x%1x"
			   " intvl:0x%1x cur_uf:0x%1x tgt_uf:0x%1x\n",
			   dcfg, dsts, dsts >> 3 & 0x3fff, intvl, current_uf,
			   target_uf);
		DBG("now:%1x bIvl:%1x ivl:%1x(u)f tgt:%1x\n",
			     now, ep->dwc_ep.intvl + 1, intvl, target_uf);

		/* Make sure 'target_uf' is non-zero so the code below knows
		 * that this is the first Isoc xfer. It will decrement the
		 * value by 1 before using it
		 */
		target_uf++;
	}

	ep->dwc_ep.send_zlp = 0;
	req->dwc_req.flags |= DWC_PCD_REQ_STARTED;
	desc = req->dwc_req.trb;
	desc_dma = req->dwc_req.trbdma;
	dwc3_dis_usb2_suspend(dwc3_dev);
	wmb();

	if (ep->dwc_ep.is_in) {
		/*
		 * Start DMA on EPn-IN
		 */
		ep_reg = ep->dwc_ep.in_ep_reg;
		DBG("IN desc@0x%08lx xferlen=%u bptr=0x%08x:%08x\n",
			   (unsigned long)desc, dwc3_get_xfercnt(desc),
			   desc->bpth, desc->bptl);
		VDBG("0x%08x 0x%08x 0x%08x 0x%08x (@0x%08x)\n",
			   *((unsigned *)desc), *((unsigned *)desc + 1),
			   *((unsigned *)desc + 2), *((unsigned *)desc + 3),
			   (unsigned)desc_dma);

		/* If Isoc */
		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
			if (ep->dwc_ep.xfer_started) {
				/* Issue "DEPUPDTXFER" command to EP */
				dwc3_dep_updatexfer(dwc3_dev, ep_reg,
							ep->dwc_ep.tri_in - 1);
			} else if (target_uf) {
				/* Issue "DEPSTRTXFER" command to EP */
				tri = dwc3_dep_startxfer(dwc3_dev, ep_reg,
							     desc_dma,
							     target_uf - 1);
				ep->dwc_ep.tri_in = tri + 1;
				ep->dwc_ep.xfer_started = 1;
			} else {
				INF("Not starting isoc IN!\n");
			}
		} else {
			if (ep->dwc_ep.xfer_started) {
				/* Issue "DEPUPDTXFER" command to EP */
				dwc3_dep_updatexfer(dwc3_dev, ep_reg,
							ep->dwc_ep.tri_in - 1);
			} else {
				/* Issue "DEPSTRTXFER" command to EP */
				tri = dwc3_dep_startxfer(dwc3_dev, ep_reg,
							desc_dma,
							req->dwc_req.stream);
				ep->dwc_ep.tri_in = tri + 1;
				ep->dwc_ep.xfer_started = 1;
			}
		}
	} else {
		/*
		 * Start DMA on EPn-OUT
		 */
		ep_reg = ep->dwc_ep.out_ep_reg;
		DBG("OUT desc@0x%08lx xferlen=%u bptr=0x%08x:%08x\n",
			   (unsigned long)desc, dwc3_get_xfercnt(desc),
			   desc->bpth, desc->bptl);
		VDBG("0x%08x 0x%08x 0x%08x 0x%08x (@0x%08x)\n",
			   *((unsigned *)desc), *((unsigned *)desc + 1),
			   *((unsigned *)desc + 2), *((unsigned *)desc + 3),
			   (unsigned)desc_dma);

		/* If Isoc */
		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
			if (ep->dwc_ep.xfer_started) {
				/* Issue "DEPUPDTXFER" command to EP */
				dwc3_dep_updatexfer(dwc3_dev, ep_reg,
							ep->dwc_ep.tri_out - 1);
			} else if (target_uf) {
				/* Issue "DEPSTRTXFER" command to EP */
				tri = dwc3_dep_startxfer(dwc3_dev, ep_reg,
							     desc_dma,
							     target_uf - 1);
				ep->dwc_ep.tri_out = tri + 1;
				ep->dwc_ep.xfer_started = 1;
			} else {
				//dwc_print0(dwc3_dev->dwc3,
				//	   "Not starting isoc OUT!\n");
			}
		} else {
			if (ep->dwc_ep.xfer_started) {
				/* Issue "DEPUPDTXFER" command to EP */
				dwc3_dep_updatexfer(dwc3_dev, ep_reg,
							ep->dwc_ep.tri_out - 1);
			} else {
				/* Issue "DEPSTRTXFER" command to EP */
				tri = dwc3_dep_startxfer(dwc3_dev, ep_reg,
							desc_dma,
							req->dwc_req.stream);
				ep->dwc_ep.tri_out = tri + 1;
				ep->dwc_ep.xfer_started = 1;
			}
		}
	}

	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * For restart after hibernation, we need to restart the transfer with the
 * address of the TRB that was last active before the hibernation. That address
 * was saved in the <em>hiber_desc_idx</em> field of struct dwc_ep by the
 * hibernation wakeup code.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to restart the transfer on.
 * @return      1 if a transfer was restarted, 0 if not.
 */
int dwc3_device_isoc_ep_hiber_restart(dwc3_device_t *dwc3_dev,
				       dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	dwc3_dma_desc_t *desc;
	dwc_dma_t desc_dma;
	int owned;
	u8 *tri;

	/* Need to restart after hibernation? */
	owned = ep->dwc_ep.hiber_desc_idx - 1;
	if (owned < 0)
		return 0;

	if (ep->dwc_ep.is_in) {
		ep_reg = ep->dwc_ep.in_ep_reg;
		tri = &ep->dwc_ep.tri_in;
	} else {
		ep_reg = ep->dwc_ep.out_ep_reg;
		tri = &ep->dwc_ep.tri_out;
	}

	DBG("Restarting Isoc xfer\n");
	desc = ep->dwc_ep.dma_desc + owned;
	desc_dma = (dwc_dma_t)
		((unsigned long)ep->dwc_ep.dma_desc_dma + owned * 16);
	DBG("desc@0x%08lx\n", (unsigned long)desc);

	VDBG("0x%08x 0x%08x 0x%08x 0x%08x (@0x%08x)\n",
		   *((unsigned *)desc), *((unsigned *)desc + 1),
		   *((unsigned *)desc + 2), *((unsigned *)desc + 3),
		   (unsigned)desc_dma);

	dwc3_dis_usb2_suspend(dwc3_dev);
	wmb();
	*tri = dwc3_dep_startxfer(dwc3_dev, ep_reg, desc_dma, 0) + 1;
	dwc3_ena_usb2_suspend(dwc3_dev);

	return 1;
}

/**
 * Stop any active xfer on a non-EP0 endpoint.
 */
static void dwc3_stop_xfer(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("%s()\n", __func__);

	if (ep->dwc_ep.is_in) {
		if (ep->dwc_ep.active && ep->dwc_ep.tri_in) {
			ep_reg = ep->dwc_ep.in_ep_reg;
			ep->dwc_ep.condition = 0;
			dwc3_dep_endxfer(dwc3_dev, ep_reg, ep->dwc_ep.tri_in - 1,
					     DWC_ENDXFER_FORCE,
					     &ep->dwc_ep.condition);
			ep->dwc_ep.tri_in = 0;
		}
	} else {
		if (ep->dwc_ep.active && ep->dwc_ep.tri_out) {
			ep_reg = ep->dwc_ep.out_ep_reg;
			ep->dwc_ep.condition = 0;
			dwc3_dep_endxfer(dwc3_dev, ep_reg, ep->dwc_ep.tri_out - 1,
					     DWC_ENDXFER_FORCE,
					     &ep->dwc_ep.condition);
			ep->dwc_ep.tri_out = 0;
		}
	}
}

/**
 * Stop any active xfers on the non-EP0 endpoints.
 */
void dwc3_stop_all_xfers(dwc3_device_t *dwc3_dev)
{
	int i;
	dwc3_device_ep_t *ep;

	DBG("%s()\n", __func__);
	dwc3_dis_usb2_suspend(dwc3_dev);

	/* Stop any active xfers on the non-EP0 IN endpoints */
	for (i = dwc3_dev->num_in_eps; i > 0; i--) {
		ep = dwc3_dev->in_ep[i - 1];
		DBG("DWC IN EP%d@0x%08lx tri-in=%d\n",
			   i, (unsigned long)ep, ep->dwc_ep.tri_in);
		dwc3_stop_xfer(dwc3_dev, ep);
		dwc3_gadget_request_nuke(dwc3_dev, ep);
		ep->dwc_ep.xfer_started = 0;
	}

	/* Stop any active xfers on the non-EP0 OUT endpoints */
	for (i = dwc3_dev->num_out_eps; i > 0; i--) {
		ep = dwc3_dev->out_ep[i - 1];
		DBG("DWC OUT EP%d@0x%08lx tri-out=%d\n",
			   i, (unsigned long)ep, ep->dwc_ep.tri_out);
		dwc3_stop_xfer(dwc3_dev, ep);
		dwc3_gadget_request_nuke(dwc3_dev, ep);
		ep->dwc_ep.xfer_started = 0;
	}

	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * This routine completes the request for the EP. If there are additional
 * requests for the EP in the queue they will be started.
 */
static int dwc3_ep_complete_request(dwc3_device_t *dwc3_dev,
					dwc3_device_ep_t *ep,
					dwc3_device_req_t *req, u32 event)
{
	int is_last = 0, ret = 0;
	int now_uf, evt_uf, status, diff, intvl, tri, i;
	dwc3_dma_desc_t *desc;
	dwc3_dev_ep_regs_t __iomem *ep_reg;
	u32 byte_count;
	dwc3_core_t *dev = dwc3_dev->dwc3;
	u32 bmudbg;
	static u32 old0, old1, old2, old3, old4;

	DBG("%s()\n", __func__);

	ep->dwc_ep.send_zlp = 0;
	desc = req->dwc_req.trb;
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
	DBG("req@0x%08lx desc@0x%08lx\n", (unsigned long)req,
		   (unsigned long)desc);

	if (!desc) {
		DBG("### %s, EP%d-%s request TRB is NULL! ###\n",
			     __func__, ep->dwc_ep.num, ep->dwc_ep.is_in ?
			     "IN" : "OUT");
		return ret;
	}

	if (!(req->dwc_req.flags & DWC_PCD_REQ_STARTED)) {
		DBG("### %s, EP%d-%s request not started! ###\n",
			     __func__, ep->dwc_ep.num, ep->dwc_ep.is_in ?
			     "IN" : "OUT");
		if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC &&
		    (event & DWC_DEPEVT_INTTYPE_BITS) ==
		    DWC_DEPEVT_XFER_IN_PROG << DWC_DEPEVT_INTTYPE_SHIFT &&
		    ep->dwc_ep.xfer_started == 0) {
			is_last = 1;
			goto done;
		}
		return ret;
	}

	if (dwc3_is_hwo(desc)) {
		DBG("### %s, EP%d-%s HWO bit set! ###\n",
			     __func__, ep->dwc_ep.num, ep->dwc_ep.is_in ?
			     "IN" : "OUT");
		return ret;
	}

	if (ep->dwc_ep.type == USB_ENDPOINT_XFER_ISOC) {
		now_uf = dwc3_get_frame(dwc3_dev);
		evt_uf = dwc3_get_eventsofn(event);
		dwc3_inval_cache((uintptr_t)desc, 2 * sizeof(dwc3_dma_desc_t));
		DBG("ep:0x%1x evt:0x%08x frm:0x%04x now:0x%04x trb:0x%1lx len:0x%04x\n",
			ep->dwc_ep.num, event, evt_uf, now_uf,
			((unsigned long)req->dwc_req.trbdma -
			 (unsigned long)ep->dwc_ep.dma_desc_dma) /
			(req->dwc_req.numbuf * 16), dwc3_get_xfercnt(desc));
		VDBG("this trb: 0x%08x 0x%08x 0x%08x 0x%08x\n",
			*((unsigned *)desc), *((unsigned *)desc + 1),
			*((unsigned *)desc + 2), *((unsigned *)desc + 3));
		VDBG("next trb: 0x%08x 0x%08x 0x%08x 0x%08x\n",
			*((unsigned *)desc + 4), *((unsigned *)desc + 5),
			*((unsigned *)desc + 6), *((unsigned *)desc + 7));

		status = dwc3_get_xfersts(desc);
		if (status & DWC_TRBRSP_MISSED_ISOC_IN) {
			desc->status &= ~DWC_DSCSTS_TRBRSP_BITS;

			/* Undocumented BMU debug register */
			bmudbg = dwc3_readl((volatile u32 __iomem *) (intptr_t)
						(dev->base + 0xc16c));
			DBG("(0x%01x) Missed Isoc! (cmpl as 0-len)\n",
				     status);
			DBG("0x%08x 0x%08x 0x%08x 0x%08x (@0x%08x)\n",
			       *((unsigned *)desc), *((unsigned *)desc + 1),
			       *((unsigned *)desc + 2), *((unsigned *)desc + 3),
			       bmudbg);
			DBG("prev: 0x%08x 0x%08x 0x%08x 0x%08x (@ox%08x)\n",
				     old0, old1, old2, old3, old4);

			/* Only valid to check uFrame on TRBs with IOC set */
			if (!dwc3_is_ioc(desc))
				goto no_restart;

			/* Check uFrame from event to see if we are too far off
			 * and need to restart the EP
			 */
			evt_uf &= 0x3fff;
			diff = now_uf - evt_uf;
			if (diff < 0)
				diff += 0x4000;
			intvl = 1 << ep->dwc_ep.intvl;
			DBG("diff %1x intvl %1x\n", diff, intvl);
#if 0
			dwc3_dump_dbgregs(dwc3_dev->dwc3);
#endif
			if (diff < intvl * 2)
				goto no_restart;

			/* Something has gone wrong, shut down the EP and then
			 * restart it
			 */
			DBG("Restart!\n");
			if (ep->dwc_ep.is_in) {
				ep_reg = ep->dwc_ep.in_ep_reg;
				tri = ep->dwc_ep.tri_in;
				ep->dwc_ep.tri_in = 0;
				DBG("IN tri=%1x\n", tri - 1);
			} else {
				ep_reg = ep->dwc_ep.out_ep_reg;
				tri = ep->dwc_ep.tri_out;
				ep->dwc_ep.tri_out = 0;
				DBG("OUT tri=%1x\n", tri - 1);
			}

			if (ep->dwc_ep.active && tri) {
				ep->dwc_ep.condition = 0;
				dwc3_dis_usb2_suspend(dwc3_dev);
				dwc3_dep_endxfer(dwc3_dev, ep_reg, tri - 1,
						     DWC_ENDXFER_FORCE |
						     DWC_ENDXFER_NODELAY,
						     &ep->dwc_ep.condition);
				dwc3_ena_usb2_suspend(dwc3_dev);
			}

			ep->dwc_ep.xfer_started = 0;

			/* Tell caller to mark all queued requests as
			 * not started
			 */
			ret = -1;
no_restart:
			req->dwc_req.actual = 0;
			is_last = 1;
			goto done;

		} else {
			old0 = *((unsigned *)desc);
			old1 = *((unsigned *)desc + 1);
			old2 = *((unsigned *)desc + 2);
			old3 = *((unsigned *)desc + 3);

			/* Undocumented BMU debug register */
			old4 = dwc3_readl((volatile u32 __iomem *) (intptr_t)
						(dev->base + 0xc16c));
		}
	}

	if (ep->dwc_ep.is_in) {	/* IN endpoint */
		for (i = 0; i < req->dwc_req.numbuf; i++, desc++) {
			if (dwc3_get_xfercnt(desc) == 0) {
				req->dwc_req.actual += req->dwc_req.length;
				if (i == req->dwc_req.numbuf - 1)
					is_last = 1;
			} else {
				DBG("Incomplete transfer!\n");
			}
		}

		if (i)
			desc--;
		DBG("IN len=%d cnt=%d rem=%d\n",
			   req->dwc_req.length, req->dwc_req.actual,
			   dwc3_get_xfercnt(desc));

	} else {		/* OUT endpoint */
		for (i = 0; i < req->dwc_req.numbuf; i++, desc++) {
			byte_count = req->dwc_req.length -
						dwc3_get_xfercnt(desc);
			req->dwc_req.actual += byte_count;
		}

		if (i)
			desc--;
		DBG("OUT len=%d cnt=%d rem=%d\n",
			   req->dwc_req.length, req->dwc_req.actual,
			   dwc3_get_xfercnt(desc));
		is_last = 1;
	}
done:
	if ((event & DWC_DEPEVT_INTTYPE_BITS) ==
	    DWC_DEPEVT_XFER_CMPL << DWC_DEPEVT_INTTYPE_SHIFT) {
		if (ep->dwc_ep.is_in)
			ep->dwc_ep.tri_in = 0;
		else
			ep->dwc_ep.tri_out = 0;
	}

	/* Complete the request */
	if (is_last) {
		dwc3_device_request_done(dwc3_dev, ep, req, 0);
		//dwc_info2(dev, "ep_complete_request - start req %d-%s\n",
		//	  ep->dwc_ep.num, ep->dwc_ep.is_in ? "IN" : "OUT");
		if (ep->dwc_ep.type != USB_ENDPOINT_XFER_ISOC ||
						ep->dwc_ep.xfer_started)
			/* If there is a request in the queue start it. */
			dwc3_gadget_start_next_request(dwc3_dev, ep);

	} else {
		DBG("### EP%d-%s is_last not set! ###\n",
			   ep->dwc_ep.num, ep->dwc_ep.is_in ? "IN" : "OUT");
	}

	return ret;
}

/**
 * This routine handles non-EP0 transfers.
 *
 * This routine gets the request corresponding to the completed transfer
 * and then calls the core routine for handling the completion.
 */
void dwc3_complete_request(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			       u32 event)
{
	dwc3_device_req_t *req;
	dwc3_dma_desc_t *desc;
	int ret;

	DBG("%s()\n", __func__);
	DBG("Requests (request_pending=%d)\n", dwc3_dev->request_pending);

	req = dwc3_gadget_get_request(dwc3_dev, ep);
	if (!req) {
		DBG("%s(ep@0x%08lx), ep->dwc_ep.queue empty!\n",
			   __func__, (unsigned long)ep);
		return;
	}
next:
	ret = dwc3_ep_complete_request(dwc3_dev, ep, req, event);
	DBG("dwc3_ep_complete_request() returned %d\n", ret);
	if (!ret)
		return;

	req = dwc3_gadget_get_request(dwc3_dev, ep);
	if (!req)
		return;

	if (ret < 0) {
		/* Isoc restart - mark all requests in queue as not started */
		dwc3_gadget_set_ep_not_started(dwc3_dev, ep);
	} else {
		/* ep_complete_request() wants to process next TRB */
		DBG("Requests2 %d\n",
			   dwc3_dev->request_pending);
		desc = req->dwc_req.trb;
		dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
		if (desc && (req->dwc_req.flags & DWC_PCD_REQ_STARTED) &&
		    !dwc3_is_hwo(desc)) {
			DBG("Processing next TRB\n");
			goto next;
		}
	}
}

/**
 * Set the EP STALL.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to set the stall on.
 */
void dwc3_device_ep_set_stall(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("%s(ep@0x%08lx)\n", __func__, (unsigned long)ep);
	DBG("ep_num=%d is_in=%d\n",
		  ep->dwc_ep.num, ep->dwc_ep.is_in);

	if (ep->dwc_ep.is_in)
		ep_reg = ep->dwc_ep.in_ep_reg;
	else
		ep_reg = ep->dwc_ep.out_ep_reg;

	dwc3_dis_usb2_suspend(dwc3_dev);
	dwc3_dep_sstall(dwc3_dev, ep_reg);
	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * Clear the EP STALL.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to clear the stall on.
 */
void dwc3_device_ep_clear_stall(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("%s()\n", __func__);
	DBG("ep_num=%d is_in=%d\n",
		   ep->dwc_ep.num, ep->dwc_ep.is_in);

	if (ep->dwc_ep.is_in)
		ep_reg = ep->dwc_ep.in_ep_reg;
	else
		ep_reg = ep->dwc_ep.out_ep_reg;

	dwc3_dis_usb2_suspend(dwc3_dev);
	dwc3_dep_cstall(dwc3_dev, ep_reg, 0);
	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * This routine returns a pointer to Out EP struct with number ep_num.
 */
dwc3_device_ep_t *dwc3_get_out_ep(dwc3_device_t *dwc3_dev, u32 ep_num)
{
	//DBG("%s(%d)\n", __func__, ep_num);

	if (ep_num == 0)
		return dwc3_dev->ep0;
	else
		return dwc3_dev->out_ep[ep_num - 1];
}

/**
 * This routine returns a pointer to In EP struct with number ep_num.
 */
dwc3_device_ep_t *dwc3_get_in_ep(dwc3_device_t *dwc3_dev, u32 ep_num)
{
	//DBG("%s(%d)\n", __func__, ep_num);

	if (ep_num == 0)
		return dwc3_dev->ep0;
	else
		return dwc3_dev->in_ep[ep_num - 1];
}

/**
 * This routine gets a pointer to an EP from the wIndex address value of the
 * control request.
 */
dwc3_device_ep_t *dwc3_device_get_ep_by_addr(dwc3_device_t *dwc3_dev, u16 index)
{
	u32 ep_num = USB_ENDPOINT_GET_ADDR(index);

	//DBG("%s(%d)\n", __func__, index);

	if (USB_ENDPOINT_GET_DIR(index) == USB_ENDPOINT_DIR_IN)
		return dwc3_get_in_ep(dwc3_dev, ep_num);
	else
		return dwc3_get_out_ep(dwc3_dev, ep_num);
}

/* USB Endpoint Operations */
/*
 * The following sections briefly describe the behavior of the Gadget API
 * endpoint operations implemented in the DWC_usb3 driver software. Detailed
 * descriptions of the generic behavior of each of these routines can be
 * found in the Linux header file include/linux/usb_gadget.h.
 *
 * The Gadget API provides wrapper routines for each of the function
 * pointers defined in usb_ep_ops. The Gadget Driver calls the wrapper
 * routine, which then calls the underlying PCD routine. The following
 * sections are named according to the wrapper routines. Within each
 * section, the corresponding DWC_usb3 PCD routine name is specified.
 *
 */

/**
 * This routine is called by the Function Driver for each EP (except EP0) to
 * be configured for the current configuration (SET_CONFIGURATION).
 *
 * This routine initializes the dwc3_ep_t data structure, and then
 * calls dwc3_ep_activate.
 */
int dwc3_device_ep_enable(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			   const usb_endpoint_descriptor_t *ep_desc,
			   const ss_endpoint_companion_descriptor_t *ep_comp)
{
	int num, dir;
	u16 maxpacket;
	u8 type;

	DBG("%s(%lx,%lx,%lx,%lx)\n", __func__,
		   (unsigned long)dwc3_dev, (unsigned long)ep,
		   (unsigned long)ep_desc, (unsigned long)ep_comp);
	DBG("ep@0x%08lx is_in=%d\n", (unsigned long)ep,
		   ep->dwc_ep.is_in);

	if (ep->dwc_ep.usb_ep_desc) {
		INF("%s, bad ep or descriptor!\n",
			   __func__);
		return -DWC_E_INVALID;
	}

	ep->dwc_ep.usb_ep_desc = ep_desc;

	/*
	 * Activate the EP
	 */
	ep->dwc_ep.stopped = 0;

	num = USB_ENDPOINT_GET_ADDR(ep_desc->bEndpointAddress);
	if (ep->dwc_ep.num != num) {
		DBG("%s, EP num mismatch, is %d asked %d!\n",
			   __func__, ep->dwc_ep.num, num);
	}

	dir = USB_ENDPOINT_GET_DIR(ep_desc->bEndpointAddress);
	if (ep->dwc_ep.is_in != (dir == USB_ENDPOINT_DIR_IN)) {
		DBG("%s, EP dir mismatch, is %d asked %d!\n",
			   __func__, ep->dwc_ep.is_in, dir == USB_ENDPOINT_DIR_IN);
	}

	type = USB_ENDPOINT_GET_XFERTYPE(ep_desc->bmAttributes);
	maxpacket = UGETW(ep_desc->wMaxPacketSize);

	ep->dwc_ep.type = type;
	ep->dwc_ep.maxpacket = maxpacket & 0x7ff;
	ep->dwc_ep.intvl = 0;
	ep->dwc_ep.mult = 0;
	ep->dwc_ep.maxburst = 0;
	ep->dwc_ep.num_streams = 0;
	ep->dwc_ep.xfer_started = 0;

	if (dwc3_dev->speed == USB_SPEED_SUPER && ep_comp)
		ep->dwc_ep.maxburst = ep_comp->bMaxBurst;

	switch (type) {
	case USB_ENDPOINT_XFER_ISOC:
		if (dwc3_dev->speed == USB_SPEED_SUPER && ep_comp) {
			ep->dwc_ep.mult = USSE_GET_MAX_PACKET_NUM(
						ep_comp->bmAttributes) + 1;
			/* 3 packets at most */
			if (ep->dwc_ep.mult > 3)
				return -DWC_E_INVALID;
		}
		/* FALL THRU */
	case USB_ENDPOINT_XFER_INT:
		if (dwc3_dev->speed == USB_SPEED_SUPER) {
			ep->dwc_ep.intvl = ep_desc->bInterval - 1;
			break;
		}

		ep->dwc_ep.intvl = ep_desc->bInterval - 1;

		/*
		 * Bits 12:11 specify number of _additional_
		 * packets per microframe.
		 */
		ep->dwc_ep.mult = (maxpacket >> 11 & 3) + 1;

		/* 3 packets at most */
		if (ep->dwc_ep.mult > 3)
			return -DWC_E_INVALID;

		break;

	case USB_ENDPOINT_XFER_BULK:
		if (dwc3_dev->speed == USB_SPEED_SUPER && ep_comp)
			ep->dwc_ep.num_streams =
				USSE_GET_MAX_STREAMS(ep_comp->bmAttributes);

		/* Set initial data PID */
		ep->dwc_ep.data_pid_start = 0;
		break;
	}

	DBG("type=%u maxpkt=%u mult=%u maxbst=%u numstrm=%u\n",
		   type, maxpacket, ep->dwc_ep.mult, ep->dwc_ep.maxburst,
		   ep->dwc_ep.num_streams);

	dwc3_ep_activate(dwc3_dev, ep, 0);
	return 0;
}

/**
 * This routine is called when an EP (except EP0) is disabled due to
 * disconnect or change in configuration. Any pending requests will
 * terminate with a status of -ESHUTDOWN.
 *
 * This routine modifies the dwc3_ep_t data structure for this EP,
 * and then calls ep_deactivate.
 */
int dwc3_device_ep_disable(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	DBG("%s(%lx)\n", __func__, (unsigned long)ep);
	DBG("ep@0x%08lx is_in=%d\n", (unsigned long)ep,
		   ep->dwc_ep.is_in);

	if (!ep->dwc_ep.usb_ep_desc)
		return -DWC_E_INVALID;

	ep_deactivate(ep->dwc_ep.dwc3_dev, ep);
	dwc3_gadget_request_nuke(dwc3_dev, ep);
	ep->dwc_ep.usb_ep_desc = NULL;

	return 0;
}

/**
 * This routine submits an I/O Request to an EP.
 *
 * - When the request completes the request's completion callback is called
 *   to return the request to the driver.
 * - An EP, except control EPs, may have multiple requests pending.
 * - Once submitted the request cannot be examined or modified.
 * - Each request is turned into one or more packets.
 * - A BULK EP can queue any amount of data; the transfer is packetized.
 * - Zero-length packets are specified with the request 'zero' flag.
 */
int dwc3_device_ep_submit_req(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			       dwc3_device_req_t *req, int req_flags)
{
	DBG("%s()\n", __func__);
	DBG("[ep%d-%s (%d phys) @0x%08lx] req@%08lx req_flags=%x"
		    " len=%d buf@%08lx\n",
		    ep->dwc_ep.num, (ep->dwc_ep.is_in ? "IN" : "OUT"),
		    ep->dwc_ep.phys, (unsigned long)ep,
		    (unsigned long)req, req_flags, req->dwc_req.length,
		    (unsigned long)req->dwc_req.buf[0]);

	/* EP0 Transfer? */
	if (ep == dwc3_dev->ep0) {
		switch (dwc3_dev->ep0state) {
		case EP0_IN_DATA_PHASE:
			DBG("%s ep0: EP0_IN_DATA_PHASE\n",
				   __func__);
			break;

		case EP0_OUT_DATA_PHASE:
			DBG("%s ep0: EP0_OUT_DATA_PHASE\n", __func__);
			if (dwc3_dev->request_config) {
				/* Complete STATUS PHASE */
				ep->dwc_ep.is_in = 1;
				dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
				return 1;
			}

			break;

		case EP0_IN_WAIT_GADGET:
			DBG("%s ep0: EP0_IN_WAIT_GADGET\n", __func__);
			dwc3_dev->ep0state = EP0_IN_WAIT_NRDY;
			return 2;

		case EP0_OUT_WAIT_GADGET:
			DBG("%s ep0: EP0_OUT_WAIT_GADGET\n", __func__);
			dwc3_dev->ep0state = EP0_OUT_WAIT_NRDY;
			return 3;

		case EP0_IN_WAIT_NRDY:
			DBG("%s ep0: EP0_IN_WAIT_NRDY\n",
				   __func__);
			dwc3_dev->ep0state = EP0_IN_STATUS_PHASE;
			break;

		case EP0_OUT_WAIT_NRDY:
			DBG("%s ep0: EP0_OUT_WAIT_NRDY\n",
				   __func__);
			dwc3_dev->ep0state = EP0_OUT_STATUS_PHASE;
			break;

		default:
			DBG("%s ep0: odd state %d!\n",
				   __func__, dwc3_dev->ep0state);
			return -DWC_E_SHUTDOWN;
		}

		ep->dwc_ep.send_zlp = 0;

		if ((req_flags & DWC_PCD_REQ_ZERO) &&
				req->dwc_req.length != 0 &&
				(req->dwc_req.length &
					(ep->dwc_ep.maxpacket - 1)) == 0) {
			ep->dwc_ep.send_zlp = 1;
		}

		dwc3_device_ep0_start_transfer(dwc3_dev, req);

#ifdef DWC_STAR_9000463548_WORKAROUND
		if (dwc3_dev->configuring) {
			dwc3_device_ep_t *ept;
			int i;

			dwc3_dev->configuring = 0;

			for (i = 0; i < dwc3_dev->num_in_eps; i++) {
				ept = dwc3_dev->in_ep[i];
				if (ept->dwc_ep.active)
					dwc3_ep_activate(dwc3_dev, ept, 0);
			}

			for (i = 0; i < dwc3_dev->num_out_eps; i++) {
				ept = dwc3_dev->out_ep[i];
				if (ept->dwc_ep.active)
					dwc3_ep_activate(dwc3_dev, ept, 0);
			}
		}
#endif
	} else {
		/* Setup and start the Transfer */
		dwc3_device_ep_start_transfer(dwc3_dev, ep, req, 0);
	}

	return 0;
}

/**
 * This routine cancels an I/O request from an EP.
 */
void dwc3_device_ep_cancel_req(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
				dwc3_device_req_t *req, u32 stream)
{
	DBG("%s(%lx,%lx) stream %d\n", __func__,
		   (unsigned long)ep, (unsigned long)req, stream);

	dwc3_dis_usb2_suspend(dwc3_dev);
	dwc3_stop_xfer(dwc3_dev, ep);
	dwc3_ena_usb2_suspend(dwc3_dev);

	ep->dwc_ep.xfer_started = 0;
	dwc3_device_request_done(dwc3_dev, ep, req, -DWC_E_DISCONNECT);
}

/**
 * usb_ep_set_halt stalls an endpoint.
 *
 * usb_ep_clear_halt clears an endpoint stall and resets its data toggle.
 *
 * Both of these routines are implemented with the same underlying routine.
 * The behavior depends on the value argument.
 *
 * @param dwc3_dev   The PCD structure.
 * @param ep    The endpoint to set halt or clear halt.
 * @param value - 0 means clear_halt.
 *              - 1 means set_halt.
 *              - 2 means clear stall lock flag.
 *              - 3 means set stall lock flag.
 */
void dwc3_device_ep_set_halt(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			      int value)
{
	DBG("%s(%lx,%d)\n", __func__,
		   (unsigned long)ep, value);

	if (value == 0) {
		ep->dwc_ep.stall_clear_flag = 0;
		if (ep != dwc3_dev->ep0)
			dwc3_device_ep_clear_stall(dwc3_dev, ep);

		if (ep->dwc_ep.stopped) {
			ep->dwc_ep.stopped = 0;

			/* If there is a request in the EP queue start it */
			if (ep != dwc3_dev->ep0 && ep->dwc_ep.is_in)
				dwc3_gadget_start_next_request(dwc3_dev, ep);
		}
	} else if (value == 1) {
stall:
		if (ep == dwc3_dev->ep0) {
			ep->dwc_ep.is_in = 0;
			dwc3_device_ep_set_stall(dwc3_dev, ep);
			dwc3_dev->ep0state = EP0_STALL;
		} else {
			dwc3_device_ep_set_stall(dwc3_dev, ep);
		}

		ep->dwc_ep.stopped = 1;

	} else if (value == 2) {
		ep->dwc_ep.stall_clear_flag = 0;

	} else if (value == 3) {
		ep->dwc_ep.stall_clear_flag = 1;
		goto stall;
	}
}

/**
 * This routine completes a request. It calls the request callback.
 */
void dwc3_device_request_done(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep,
			       dwc3_device_req_t *req, int status)
{
	unsigned stopped = ep->dwc_ep.stopped;

	DBG("%s(%lx,%lx,%d)\n", __func__,
		   (unsigned long)ep, (unsigned long)req, status);

	if (ep != dwc3_dev->ep0) {
		req->dwc_req.flags &= ~DWC_PCD_REQ_STARTED;
		if (req->dwc_req.trb) {
			dwc3_disable_desc(req->dwc_req.trb);
			ep->dwc_ep.desc_avail++;
			req->dwc_req.trb = NULL;
		}
	}

	/* don't modify queue heads during completion callback */
	ep->dwc_ep.stopped = 1;

	dwc3_gadget_complete(dwc3_dev, ep, req, status);

	//req->dwc_req.actual = 0;
	ep->dwc_ep.stopped = stopped;
}

/**
 * This routine is called when the Device is disconnected. It stops any
 * active requests and informs the Function Driver of the disconnect.
 */
void dwc3_device_stop(dwc3_device_t *dwc3_dev)
{
	dwc3_device_ep_t *ep;
	int i;

	DBG("%s()\n", __func__);

	/* don't disconnect drivers more than once */
	if (dwc3_dev->state == DWC_STATE_UNCONNECTED) {
		DBG("%s() Already Disconnected\n",
			   __func__);
	} else {
		dwc3_dev->state = DWC_STATE_UNCONNECTED;

		/* report disconnect; the driver is already quiesced */
		dwc3_gadget_disconnect(dwc3_dev);
	}

	dwc3_dis_usb2_suspend(dwc3_dev);

	/* kill any outstanding requests, prevent new request submissions */
	for (i = 0; i < dwc3_dev->num_in_eps; i++) {
		ep = dwc3_dev->in_ep[i];
		dwc3_stop_xfer(dwc3_dev, ep);
		dwc3_gadget_request_nuke(dwc3_dev, ep);
		ep->dwc_ep.xfer_started = 0;
	}

	for (i = 0; i < dwc3_dev->num_out_eps; i++) {
		ep = dwc3_dev->out_ep[i];
		dwc3_stop_xfer(dwc3_dev, ep);
		dwc3_gadget_request_nuke(dwc3_dev, ep);
		ep->dwc_ep.xfer_started = 0;
	}

	dwc3_ena_usb2_suspend(dwc3_dev);
}

/**
 * Gets the current USB frame number.
 */
int dwc3_device_get_frame_number(dwc3_device_t *dwc3_dev)
{
	return dwc3_get_frame(dwc3_dev);
}

/**
 * Initialize the EP structures to their default state.
 *
 */
static void dwc3_device_epinit(dwc3_device_t *dwc3_dev)
{
	int num_out_eps = dwc3_dev->num_out_eps;
	int num_in_eps = dwc3_dev->num_in_eps;
	int ep_cntr, i;
	dwc3_device_ep_t *ep;

	DBG("%s(%lx)\n", __func__, (unsigned long)dwc3_dev);
	DBG("num_out_eps=%d\n", num_out_eps);
	DBG("num_in_eps=%d\n", num_in_eps);

	/*
	 * Initialize the EP0 structure
	 */
	ep = dwc3_dev->ep0;
	DBG("initializing EP0 (@%p)\n", ep);

	/* Init EP structure */
	ep->dwc_ep.dma_desc = NULL;
	ep->dwc_ep.dma_desc_dma = 0;
	ep->dwc_ep.usb_ep_desc = NULL;
	ep->dwc_ep.dwc3_dev = dwc3_dev;
	ep->dwc_ep.stopped = 1;
	ep->dwc_ep.is_in = 0;
	ep->dwc_ep.active = 0;
	ep->dwc_ep.phys = 0;
	ep->dwc_ep.num = 0;
	ep->dwc_ep.tx_fifo_num = 0;
	ep->dwc_ep.out_ep_reg = &dwc3_dev->out_ep_regs[0];
	ep->dwc_ep.in_ep_reg = &dwc3_dev->in_ep_regs[0];

	ep->dwc_ep.type = DWC_USB3_EP_TYPE_CONTROL;
	ep->dwc_ep.maxpacket = DWC_MAX_EP0_SIZE;
	ep->dwc_ep.send_zlp = 0;
	ep->dwc_ep.queue_sof = 0;

	dwc3_dev->ep0_req->dwc_req.buf[0] = NULL;
	dwc3_dev->ep0_req->dwc_req.bufdma[0] = 0;
	dwc3_dev->ep0_req->dwc_req.buflen[0] = 0;
	dwc3_dev->ep0_req->dwc_req.length = 0;
	dwc3_dev->ep0_req->dwc_req.actual = 0;

	/*
	 * Initialize the EP1-n structures
	 */
	ep_cntr = 0;

	for (i = 1; ep_cntr < num_out_eps; i++) {
		ep = dwc3_dev->out_ep[ep_cntr];
		DBG("initializing EP%d-OUT (out_ep[%d]@%p)\n", i, ep_cntr, ep);
		ep_cntr++;

		/* Init EP structure - but don't overwrite '.num', the gadget
		 * has already set that
		 */
		ep->dwc_ep.dma_desc = NULL;
		ep->dwc_ep.dma_desc_dma = 0;
		ep->dwc_ep.usb_ep_desc = NULL;
		ep->dwc_ep.dwc3_dev = dwc3_dev;
		ep->dwc_ep.stopped = 1;
		ep->dwc_ep.is_in = 0;
		ep->dwc_ep.active = 0;
		ep->dwc_ep.phys = ep_cntr << 1;
		ep->dwc_ep.tx_fifo_num = 0;
		ep->dwc_ep.out_ep_reg = &dwc3_dev->out_ep_regs[ep_cntr];

		/* Control until EP is activated */
		ep->dwc_ep.type = DWC_USB3_EP_TYPE_CONTROL;
		ep->dwc_ep.maxpacket = DWC_MAX_EP0_SIZE;
		ep->dwc_ep.send_zlp = 0;
		ep->dwc_ep.queue_sof = 0;
	}

	ep_cntr = 0;

	for (i = 1; ep_cntr < num_in_eps; i++) {
		ep = dwc3_dev->in_ep[ep_cntr];
		DBG("initializing EP%d-IN (in_ep[%d]@%p)\n", i, ep_cntr, ep);
		ep_cntr++;

		/* Init EP structure - but don't overwrite '.num', the gadget
		 * has already set that
		 */
		ep->dwc_ep.dma_desc = NULL;
		ep->dwc_ep.dma_desc_dma = 0;
		ep->dwc_ep.usb_ep_desc = NULL;
		ep->dwc_ep.dwc3_dev = dwc3_dev;
		ep->dwc_ep.stopped = 1;
		ep->dwc_ep.is_in = 1;
		ep->dwc_ep.active = 0;
		ep->dwc_ep.phys = ep_cntr << 1 | 1;
		ep->dwc_ep.tx_fifo_num = ep_cntr;
		ep->dwc_ep.in_ep_reg = &dwc3_dev->in_ep_regs[ep_cntr];

		/* Control until EP is activated */
		ep->dwc_ep.type = DWC_USB3_EP_TYPE_CONTROL;
		ep->dwc_ep.maxpacket = DWC_MAX_EP0_SIZE;
		ep->dwc_ep.send_zlp = 0;
		ep->dwc_ep.queue_sof = 0;
	}

	dwc3_dev->ep0state = EP0_IDLE;
	dwc3_dev->ep0->dwc_ep.maxpacket = DWC_MAX_EP0_SIZE;
	dwc3_dev->ep0->dwc_ep.type = DWC_USB3_EP_TYPE_CONTROL;
}

int dwc3_device_init(dwc3_core_t *dev)
{
	ENTER();

	dev->dwc3_dev.dwc3 = dev;
	dev->dwc3_dev.speed = USB_SPEED_UNKNOWN;

	/*
	 * Initialize EP structures
	 */
	dwc3_device_epinit(&dev->dwc3_dev);

	/*
	 * Initialize the Core (also enables interrupts and sets Run/Stop bit)
	 */
	// move to start
	DBG("%s() --> dwc3_core_device_init()\n", __func__);
	dwc3_core_device_init(dev, 1, 0);

	EXIT();
	return 0;
}

void dwc3_device_remove(dwc3_core_t *dev)
{
	ENTER();
	DBG("%s() --> dwc3_core_device_remove()\n", __func__);
	dwc3_core_device_remove(dev);
	EXIT();
}
