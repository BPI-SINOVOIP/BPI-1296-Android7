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
#include <lib/heap.h>
#include <dev/udc.h>
#include <dev/usb-phy.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"

#include "dwc3.h"

#undef log_level
#define log_level 3

/** Endpoint context structs */
static dwc3_device_ep_t 	g_ep0;
static dwc3_device_ep_t 	g_out_ep[DWC_MAX_EPS - 1];
static dwc3_device_ep_t 	g_in_ep[DWC_MAX_EPS - 1];

/** EP0 request */
static dwc3_device_req_t 	g_ep0_req;

/** EP request pool */
dwc3_device_req_t 			g_dev_req[32];
u32 						g_dev_req_bm;

/** Driver options struct, default values are defined here */
dwc3_core_params_t core_params = {
	.burst = 1,
	.newcore = 0,
	.phy = 2,
	.wakeup = 0,
	.pwrctl = 3,
	.lpmctl = 1,
	.phyctl = 0,
	.usb2mode = 0,
	.hibernate = 0,
	.hiberdisc = 1,
	.clkgatingen = 0,
	.ssdisquirk = 0,
	.nobos = 0,
	.loop = 0,
	//.nump = 16,
	.nump = 0,
	.newcsr = 0,
	.rxfsz = 0,
	.txfsz = { 0, },
	.txfsz_cnt = 0,
	.imod = 0,
};

/*** The following data structures must be in coherent DMA capable memory.
 *** All memory in the simulation environment is such, and physical address
 *** == virtual address, so we can just allocate these statically.
 ***/

/** Event buffer */
//static u32		g_event_buf[DWC_EVENT_BUF_SIZE];

/** EP0 SETUP packet buffer */
static char 	g_ep0_setup_pkt[8];

/** Data packet buffer used to return data for GET_STATUS and
 *  GET_DESCRIPTOR requests, up to 512 bytes in length
 */
static u8 		g_ep0_status_buf[DWC_STATUS_BUF_SIZE];

/** DMA descriptor (TRB) for EP0 SETUP packets */
static UALIGNED16 dwc3_dma_desc_t 	g_ep0_setup_desc;

/** DMA descriptor (TRB) for EP0 Data Out and Status Out phases */
static UALIGNED16 dwc3_dma_desc_t 	g_ep0_out_desc;

/** DMA descriptor (TRB) for EP0 Data In and Status In phases */
static UALIGNED16 dwc3_dma_desc_t 	g_ep0_in_desc;

/** DMA descriptor (TRB) pools for non-EP0 EPs */
UALIGNED16 dwc3_dma_desc_t g_out_trb_pool[DWC_MAX_EPS - 1][DWC_NUM_ISOC_TRBS + 1];
UALIGNED16 dwc3_dma_desc_t g_in_trb_pool[DWC_MAX_EPS - 1][DWC_NUM_ISOC_TRBS + 1];

void dwc3_core_soft_reset(dwc3_core_t *dev)
{
	u32 temp;
 	dwc3_core_global_regs_t *global_regs =
						dev->core_global_regs;

	/* Before Resetting PHY, put Core in Reset */
	temp = dwc3_readl(&global_regs->gctl);
	temp |= DWC_GCTL_CORE_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gctl);

	/* Assert USB3 PHY reset */
	temp = dwc3_readl(&global_regs->gusb3pipectl[0]);
	temp |= DWC_PIPECTL_PHY_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gusb3pipectl[0]);

	/* Assert USB2 PHY reset */
	temp = dwc3_readl(&global_regs->gusb2phycfg[0]);
	temp |= DWC_USB2PHYCFG_PHY_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gusb2phycfg[0]);

	usb3_phy_init(dev->usb3phy);
	mdelay(100);

	/* Clear USB3 PHY reset */
	temp = dwc3_readl(&global_regs->gusb3pipectl[0]);
	temp &= ~DWC_PIPECTL_PHY_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gusb3pipectl[0]);


	/* Clear USB2 PHY reset */
	temp = dwc3_readl(&global_regs->gusb2phycfg[0]);
	temp &= ~DWC_USB2PHYCFG_PHY_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gusb2phycfg[0]);

	mdelay(100);

	/* After PHYs are stable we can take Core out of reset state */
	temp = dwc3_readl(&global_regs->gctl);
	temp &= ~DWC_GCTL_CORE_SOFT_RST_BIT;
	dwc3_writel(temp, &global_regs->gctl);

	usb2_phy_init(dev->usb2phy);
	mdelay(100);
}

/**
 * Fill in the four dwords of a DMA descriptor (aka a TRB).
 */
void dwc3_fill_desc(dwc3_dma_desc_t *desc, dwc_dma_t dma_addr,
			u32 dma_len, u32 stream, u32 type,
			u32 ctrlbits, int own)
{
	desc->bptl = (u32)(dma_addr & 0xffffffffU);
#ifdef DWC_64_BIT_ARCH
	desc->bpth = (u32)(dma_addr >> 32U & 0xffffffffU);
#else
	desc->bpth = 0;
#endif
	desc->status = dma_len << DWC_DSCSTS_XFRCNT_SHIFT;

	/* Note: If type is 0, leave original control bits intact (for isoc) */
	if (type)
		desc->control = type << DWC_DSCCTL_TRBCTL_SHIFT;

	desc->control |= stream << DWC_DSCCTL_STRMID_SOFN_SHIFT | ctrlbits;

	/* Must do this last! */
	if (own) {
		wmb();
		desc->control |= DWC_DSCCTL_HWO_BIT;
	}
	dwc3_flush_cache((uintptr_t)dma_addr, dma_len);
	dwc3_flush_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
}

/**
 * Make a DMA descriptor the start of a chain by setting its CHN bit and
 * clearing its IOC bit.
 *
 * @param desc  The DMA descriptor (TRB) to operate on.
 */
void dwc3_start_desc_chain(dwc3_dma_desc_t *desc)
{
	desc->control |= DWC_DSCCTL_CHN_BIT;
	desc->control &= ~DWC_DSCCTL_IOC_BIT;
	dwc3_flush_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
}

/**
 * Make a DMA descriptor the end of a chain by clearing its CHN bit and
 * setting its IOC bit.
 *
 * @param desc  The DMA descriptor (TRB) to operate on.
 */
void dwc3_end_desc_chain(dwc3_dma_desc_t *desc)
{
	desc->control &= ~DWC_DSCCTL_CHN_BIT;
	desc->control |= DWC_DSCCTL_IOC_BIT;
	dwc3_flush_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
}

/**
 * Enable a DMA descriptor by setting its HWO bit.
 *
 * @param desc  The DMA descriptor (TRB) to operate on.
 */
void dwc3_enable_desc(dwc3_dma_desc_t *desc)
{
	wmb();
	desc->control |= DWC_DSCCTL_HWO_BIT;
	dwc3_flush_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
}

/**
 * Disable a DMA descriptor by clearing its HWO bit.
 *
 * NOTE: This must only be called if it is known that the hardware has finished
 * with the DMA descriptor, but for some reason the hardware has not cleared
 * the HWO bit.
 *
 * @param desc  The DMA descriptor (TRB) to operate on.
 */
void dwc3_disable_desc(dwc3_dma_desc_t *desc)
{
	desc->control &= ~DWC_DSCCTL_HWO_BIT;
	wmb();
	dwc3_flush_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t));
}

/**
 * Spin on register bit until handshake completes or times out.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 * @param ptr   Address of register to read.
 * @param mask  Bit to look at in result of read.
 * @param done  Value of the bit when handshake succeeds.
 * @return      1 when the mask bit has the specified value (handshake done),
 *              0 when timeout has passed (handshake failed).
 */
static int _handshake(dwc3_core_t *dev, volatile u32 *ptr,
		      u32 mask, u32 done)
{
	ENTER();
	u32 usec = 100000;
	u32 result;

	DBG("%s() ptr %p mask %08x done %08x\n", __func__, ptr, mask, done);

	do {
		result = dwc3_readl(ptr);
		if ((result & mask) == done) {
			DBG("after DEPCMD=0x%08x\n", result);
			return 1;
		}

		udelay(1);
		usec -= 1;
	} while (usec > 0);

	EXIT();
	return 0;
}

/*
 * Routines for sending the various Device commands to the hardware.
 * See description of DGCMD register in "Control and Status Registers"
 * section of the USB3 controller databook.
 */

/**
 * Send TRANSMIT FUNCTION WAKE DEVICE NOTIFICATION command to Device
 */
int dwc3_xmit_fn_remote_wake(dwc3_device_t *dwc3_dev, u32 intf)
{
	ENTER();
	DBG("%s()\n", __func__);

	/* Use the generic Transmit Device Notification command if the core
	 * supports it
	 */
	if (dwc3_dev->dwc3->snpsid >= 0x5533210a) {
		/* Set param */
		dwc3_writel(intf << DWC_DGCMDPAR_DEV_NOTIF_PARAM_SHIFT |
				DWC_DGCMD_FUNCTION_WAKE_DEV_NOTIF,
				&dwc3_dev->dev_regs->dgcmdpar);
		DBG("DGCMDPAR=0x%08x\n",
			   intf << DWC_DGCMDPAR_DEV_NOTIF_PARAM_SHIFT |
			   DWC_DGCMD_FUNCTION_WAKE_DEV_NOTIF);

		/* Start the command */
		dwc3_writel(DWC_DGCMD_XMIT_DEV_NOTIF | DWC_DGCMD_ACT_BIT,
				&dwc3_dev->dev_regs->dgcmd);
		DBG("DGCMD=0x%08x\n",
			   DWC_DGCMD_XMIT_DEV_NOTIF | DWC_DGCMD_ACT_BIT);
	} else {
		/* Set param */
		dwc3_writel(intf, &dwc3_dev->dev_regs->dgcmdpar);
		DBG("DGCMDPAR=0x%08x\n", intf);

		/* Start the command */
		dwc3_writel(DWC_DGCMD_XMIT_FUNC_WAKE_DEV_NOTIF | DWC_DGCMD_ACT_BIT,
				&dwc3_dev->dev_regs->dgcmd);
		DBG("DGCMD=0x%08x\n",
			   DWC_DGCMD_XMIT_FUNC_WAKE_DEV_NOTIF | DWC_DGCMD_ACT_BIT);
	}

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &dwc3_dev->dev_regs->dgcmd,
		  DWC_DGCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send LATENCY TOLERANCE MESSAGE DEVICE NOTIFICATION command to Device
 */
int dwc3_xmit_ltm(dwc3_device_t *dwc3_dev, u32 value)
{
	ENTER();
	DBG("%s()\n", __func__);

	/* Use the generic Transmit Device Notification command if the core
	 * supports it. Otherwise this function is a no-op.
	 */
	if (dwc3_dev->dwc3->snpsid >= 0x5533210a) {
		/* Set param */
		dwc3_writel(value << DWC_DGCMDPAR_DEV_NOTIF_PARAM_SHIFT |
				DWC_DGCMD_LATENCY_TOL_DEV_NOTIF,
				&dwc3_dev->dev_regs->dgcmdpar);
		DBG("DGCMDPAR=0x%08x\n",
				value << DWC_DGCMDPAR_DEV_NOTIF_PARAM_SHIFT |
				DWC_DGCMD_LATENCY_TOL_DEV_NOTIF);

		/* Start the command */
		dwc3_writel(DWC_DGCMD_XMIT_DEV_NOTIF | DWC_DGCMD_ACT_BIT,
				&dwc3_dev->dev_regs->dgcmd);
		DBG("DGCMD=0x%08x\n",
			   DWC_DGCMD_XMIT_DEV_NOTIF | DWC_DGCMD_ACT_BIT);

		/* Wait for command completion */
		handshake(dwc3_dev->dwc3, &dwc3_dev->dev_regs->dgcmd,
			  DWC_DGCMD_ACT_BIT, 0);
	}

	EXIT();
	return 0;
}

/**
 * Send SET SCRATCHPAD BUFFER ARRAY command to Device
 */
int dwc3_set_scratchpad_buf_array(dwc3_device_t *dwc3_dev, dwc_dma_t dma_addr)
{
	ENTER();
	DBG("%s(%lx)\n", __func__,
		   (unsigned long)dma_addr);

	/* Set param */
	dwc3_writel(dma_addr & 0xffffffffU,
			&dwc3_dev->dev_regs->dgcmdpar);
	DBG("DGCMDPAR=0x%08lx\n",
		   (unsigned long)(dma_addr & 0xffffffffU));

	/* Start the command */
	dwc3_writel(DWC_DGCMD_SET_SCRATCHPAD_ARRAY_ADR_LO | DWC_DGCMD_ACT_BIT,
			&dwc3_dev->dev_regs->dgcmd);
	DBG("DGCMD=0x%08x\n",
		   DWC_DGCMD_SET_SCRATCHPAD_ARRAY_ADR_LO | DWC_DGCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &dwc3_dev->dev_regs->dgcmd,
		  DWC_DGCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send SELECTED FIFO FLUSH command to Device
 */
int dwc3_flush_fifo(dwc3_device_t *dwc3_dev, u32 fifo_sel)
{
	ENTER();
	DBG("%s()\n", __func__);

	/* Set param */
	dwc3_writel(fifo_sel, &dwc3_dev->dev_regs->dgcmdpar);
	DBG("DGCMDPAR=0x%08x\n", fifo_sel);

	/* Start the command */
	dwc3_writel(DWC_DGCMD_SELECTED_FIFO_FLUSH | DWC_DGCMD_ACT_BIT,
			&dwc3_dev->dev_regs->dgcmd);
	DBG("DGCMD=0x%08x\n",
		   DWC_DGCMD_SELECTED_FIFO_FLUSH | DWC_DGCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &dwc3_dev->dev_regs->dgcmd,
		  DWC_DGCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/*
 * Routines for sending the various Endpoint commands to the hardware.
 * See description of DEPCMDn register in "Control and Status Registers"
 * section of the USB3 controller databook.
 */

/**
 * Send DEPCFG command to EP
 */
int dwc3_dep_cfg(dwc3_device_t *dwc3_dev,
		     dwc3_dev_ep_regs_t *ep_reg,
		     u32 depcfg0, u32 depcfg1, u32 depcfg2)
{
	ENTER();
	DBG("dep_cfg, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Set param 2 */
	dwc3_writel(depcfg2, &ep_reg->depcmdpar2);
	DBG("DEPCFG2=0x%08x\n", depcfg2);

	/* Set param 1 */
	dwc3_writel(depcfg1, &ep_reg->depcmdpar1);
	DBG("DEPCFG1=0x%08x\n", depcfg1);

	/* Set param 0 */
	dwc3_writel(depcfg0, &ep_reg->depcmdpar0);
	DBG("DEPCFG0=0x%08x\n", depcfg0);

	wmb();
	/* Start the command */
	dwc3_writel(DWC_EPCMD_SET_EP_CFG | DWC_EPCMD_ACT_BIT,
			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   DWC_EPCMD_SET_EP_CFG | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send DEPXFERCFG command to EP
 */
int dwc3_dep_xfercfg(dwc3_device_t *dwc3_dev,
			 dwc3_dev_ep_regs_t *ep_reg,
			 u32 depstrmcfg)
{
	ENTER();
	DBG("dep_xfercfg, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Set param 0 */
	dwc3_writel(depstrmcfg, &ep_reg->depcmdpar0);
	DBG("DEPSTRMCFG=0x%08x\n", depstrmcfg);

	/* Start the command */
	dwc3_writel(DWC_EPCMD_SET_XFER_CFG | DWC_EPCMD_ACT_BIT,
 			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   DWC_EPCMD_SET_XFER_CFG | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send DEPGETEPSTATE command to EP
 */
u32 dwc3_dep_getepstate(dwc3_device_t *dwc3_dev,
				 dwc3_dev_ep_regs_t __iomem *ep_reg)
{
	ENTER();
	u32 retval;

	DBG("dep_getepstate, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Start the command */
	dwc3_writel(DWC_EPCMD_GET_EP_STATE | DWC_EPCMD_ACT_BIT,
			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   DWC_EPCMD_GET_EP_STATE | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	/* Get state returned in DEPCMDPAR2 */
	retval = dwc3_readl(&ep_reg->depcmdpar2);

	EXIT();
	return retval;
}

/**
 * Send DEPSSTALL command to EP
 */
int dwc3_dep_sstall(dwc3_device_t *dwc3_dev,
			dwc3_dev_ep_regs_t __iomem *ep_reg)
{
	ENTER();
	DBG("dep_sstall, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Start the command */
	dwc3_writel(DWC_EPCMD_SET_STALL | DWC_EPCMD_ACT_BIT,
			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   DWC_EPCMD_SET_STALL | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send DEPCSTALL command to EP
 */
int dwc3_dep_cstall(dwc3_device_t *dwc3_dev,
			dwc3_dev_ep_regs_t __iomem *ep_reg, int clr_pend)
{
	ENTER();
	u32 depcmd;

	DBG("dep_cstall, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Fill clear stall command */
	depcmd = DWC_EPCMD_CLR_STALL | DWC_EPCMD_ACT_BIT;
	if (clr_pend)
		depcmd |= DWC_EPCMD_HP_FRM_BIT;

	/* Start the command */
	dwc3_writel(depcmd, &ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n", depcmd);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send DEPSTRTXFER command to EP
 */
int dwc3_dep_startxfer(dwc3_device_t *dwc3_dev,
			   dwc3_dev_ep_regs_t __iomem *ep_reg,
			   dwc_dma_t dma_addr, u32 stream_or_uf)
{
	ENTER();
	u32 retval;

	DBG("dep_startxfer, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Set param 1 */
	dwc3_writel(dma_addr & 0xffffffffU,
			&ep_reg->depcmdpar1);
	DBG("TDADDRLO=0x%08lx\n",
		   (unsigned long)(dma_addr & 0xffffffffU));

	/* Set param 0 */
#ifdef DWC_64_BIT_ARCH
	dwc3_writel(dma_addr >> 32U & 0xffffffffU,
			&ep_reg->depcmdpar0);
	DBG("TDADDRHI=0x%08lx\n",
		   (unsigned long)(dma_addr >> 32U & 0xffffffffU));
#else
	dwc3_writel(0, &ep_reg->depcmdpar0);
	DBG("TDADDRHI=0x%08x\n", 0);
#endif
	/* Start the command */
	dwc3_writel(stream_or_uf << DWC_EPCMD_STR_NUM_OR_UF_SHIFT |
			DWC_EPCMD_START_XFER | DWC_EPCMD_ACT_BIT,
			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   stream_or_uf << DWC_EPCMD_STR_NUM_OR_UF_SHIFT |
		   DWC_EPCMD_START_XFER | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	retval = dwc3_readl(&ep_reg->depcmd);

	EXIT();
	return retval >> DWC_EPCMD_XFER_RSRC_IDX_SHIFT &
	       DWC_EPCMD_XFER_RSRC_IDX_BITS >> DWC_EPCMD_XFER_RSRC_IDX_SHIFT;
}

/**
 * Send DEPUPDTXFER command to EP
 */
int dwc3_dep_updatexfer(dwc3_device_t *dwc3_dev,
			    dwc3_dev_ep_regs_t __iomem *ep_reg,
			    u32 tri)
{
	ENTER();
	DBG("dep_updxfer, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Start the command */
	dwc3_writel(tri << DWC_EPCMD_XFER_RSRC_IDX_SHIFT |
			DWC_EPCMD_UPDATE_XFER | DWC_EPCMD_ACT_BIT,
			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   tri << DWC_EPCMD_XFER_RSRC_IDX_SHIFT |
		   DWC_EPCMD_UPDATE_XFER | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**
 * Send DEPENDXFER command to EP
 */
int dwc3_dep_endxfer(dwc3_device_t *dwc3_dev,
			 dwc3_dev_ep_regs_t __iomem *ep_reg,
			 u32 tri, int flags, void *condition)
{
	ENTER();
	u32 depcmd;

	DBG("dep_endxfer, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Fill end transfer command */
	depcmd = tri << DWC_EPCMD_XFER_RSRC_IDX_SHIFT | DWC_EPCMD_END_XFER;
	depcmd |= DWC_EPCMD_ACT_BIT;
	if (flags & DWC_ENDXFER_FORCE)
		depcmd |= DWC_EPCMD_HP_FRM_BIT;

	/* Start the command. */
	dwc3_writel(depcmd, &ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n", depcmd);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);
	if (!(flags & DWC_ENDXFER_NODELAY))
		udelay(100);

	EXIT();
	return 0;
}

/**
 * Send DEPSTRTNEWCFG command to EP
 */
int dwc3_dep_startnewcfg(dwc3_device_t *dwc3_dev,
			     dwc3_dev_ep_regs_t __iomem *ep_reg,
			     u32 rsrcidx)
{
	ENTER();
	DBG("dep_startnewcfg, ep_reg=0x%08lx\n",
		   (unsigned long)ep_reg);

	/* Start the command */
	dwc3_writel(rsrcidx << DWC_EPCMD_XFER_RSRC_IDX_SHIFT |
			DWC_EPCMD_START_NEW_CFG | DWC_EPCMD_ACT_BIT,
 			&ep_reg->depcmd);
	DBG("DEPCMD=0x%08x\n",
		   rsrcidx << DWC_EPCMD_XFER_RSRC_IDX_SHIFT |
		   DWC_EPCMD_START_NEW_CFG | DWC_EPCMD_ACT_BIT);

	/* Wait for command completion */
	handshake(dwc3_dev->dwc3, &ep_reg->depcmd, DWC_EPCMD_ACT_BIT, 0);

	EXIT();
	return 0;
}

/**********************/

/**
 * Enable an EP in the DALEPENA register.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to enable.
 * @return      0 if succesful, -DWC_E_BUSY if already enabled.
 */
int dwc3_enable_ep(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	ENTER();
	u32 ep_index_num, dalepena;

	ep_index_num = ep->dwc_ep.num * 2;

	if (ep->dwc_ep.is_in)
		ep_index_num += 1;

	dalepena = dwc3_readl(&dwc3_dev->dev_regs->dalepena);

	/* If we have already enabled this EP, leave it alone
	 * (shouldn't happen)
	 */
	if (dalepena & 1 << ep_index_num)
		return -DWC_E_BUSY;

	dalepena |= 1 << ep_index_num;
	dwc3_writel(dalepena, &dwc3_dev->dev_regs->dalepena);
	DBG("enable_ep: DALEPENA=0x%08x\n", dalepena);
	EXIT();
	return 0;
}

/**
 * Disable an EP in the DALEPENA register.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param ep    The EP to disable.
 * @return      0 if succesful, -DWC_E_INVALID if already disabled.
 */
int dwc3_disable_ep(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep)
{
	ENTER();
	u32 ep_index_num, dalepena;

	ep_index_num = ep->dwc_ep.num * 2;

	if (ep->dwc_ep.is_in)
		ep_index_num += 1;

	dalepena = dwc3_readl(&dwc3_dev->dev_regs->dalepena);

	/* If we have already disabled this EP, leave it alone
	 * (shouldn't happen)
	 */
	if (!(dalepena & 1 << ep_index_num))
		return -DWC_E_INVALID;

	dalepena &= ~(1 << ep_index_num);
	dwc3_writel(dalepena, &dwc3_dev->dev_regs->dalepena);
	DBG("disable_ep: DALEPENA=0x%08x\n", dalepena);
	EXIT();
	return 0;
}

/**
 * Get the device speed from the device status register and convert it
 * to USB speed constant.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @return      The device speed.
 */
int dwc3_get_device_speed(dwc3_device_t *dwc3_dev)
{
	u32 dsts;
	int speed = USB_SPEED_UNKNOWN;

	dsts = dwc3_readl(&dwc3_dev->dev_regs->dsts);

	switch (dsts >> DWC_DSTS_CONNSPD_SHIFT &
		DWC_DSTS_CONNSPD_BITS >> DWC_DSTS_CONNSPD_SHIFT) {
	case DWC_SPEED_HS_PHY_30MHZ_OR_60MHZ:
		DBG("HIGH SPEED\n");
		speed = USB_SPEED_HIGH;
		break;

	case DWC_SPEED_FS_PHY_30MHZ_OR_60MHZ:
	case DWC_SPEED_FS_PHY_48MHZ:
		DBG("FULL SPEED\n");
		speed = USB_SPEED_FULL;
		break;

	case DWC_SPEED_LS_PHY_6MHZ:
		DBG("LOW SPEED\n");
		speed = USB_SPEED_LOW;
		break;

	case DWC_SPEED_SS_PHY_125MHZ_OR_250MHZ:
		DBG("SUPER SPEED\n");
		speed = USB_SPEED_SUPER;
		break;

	case DWC_SPEED_SS_PHY_SSP:
		DBG("SUPER SPEED PLUS\n");
		speed = USB_SPEED_SUPER; /** @TODO Handle SSP  */
		break;
	}

	return speed;
}

/**
 * Get the current microframe number.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @return      The current microframe number.
 */
int dwc3_get_frame(dwc3_device_t *dwc3_dev)
{
	u32 dsts;

	/* read current frame/microframe number from DSTS register */
	dsts = dwc3_readl(&dwc3_dev->dev_regs->dsts);

	return dsts >> DWC_DSTS_SOF_FN_SHIFT &
	       DWC_DSTS_SOF_FN_BITS >> DWC_DSTS_SOF_FN_SHIFT;
}

/**
 * Get the current link state.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @return      The current link state.
 */
int dwc3_device_get_link_state(dwc3_device_t *dwc3_dev)
{
	u32 status, state;

	status = dwc3_readl(&dwc3_dev->dev_regs->dsts);
	state = status >> DWC_DSTS_USBLNK_STATE_SHIFT &
		DWC_DSTS_USBLNK_STATE_BITS >> DWC_DSTS_USBLNK_STATE_SHIFT;
	DBG("status=0x%08x state=%d\n", status, state);

	return state;
}

/**
 * Set state of USB link.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param state Link state to set.
 */
void dwc3_device_set_link_state(dwc3_device_t *dwc3_dev, int state)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl &= ~DWC_DCTL_ULST_CHNG_REQ_BITS;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl &= ~DWC_DCTL_ULST_CHNG_REQ_BITS;
	dctl |= state << DWC_DCTL_ULST_CHNG_REQ_SHIFT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Send a Remote Wakeup to the host.
 *
 * @param dwc3_dev           Programming view of DWC_usb3 peripheral controller.
 * @param function      Function that caused the remote wakeup.
 */
void dwc3_device_remote_wake(dwc3_device_t *dwc3_dev, int function)
{
	/* For USB 3.0, send function remote wake notification */
	if (dwc3_dev->speed == USB_SPEED_SUPER)
		dwc3_xmit_fn_remote_wake(dwc3_dev, function);
}

/**
 * Set the Device Address.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @param addr  The address to set.
 */
void dwc3_set_address(dwc3_device_t *dwc3_dev, int addr)
{
	u32 dcfg;

	dcfg = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
	dcfg &= ~DWC_DCFG_DEVADDR_BITS;
	dcfg |= addr << DWC_DCFG_DEVADDR_SHIFT;
	dwc3_writel(dcfg, &dwc3_dev->dev_regs->dcfg);
}

/**
 * Enable USB2 Phy suspend.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
static void dwc3_ena_usb2_phy_suspend(dwc3_device_t *dwc3_dev)
{
	dwc3_core_t *dev = dwc3_dev->dwc3;
	int hiber = dev->core_params->hibernate &&
		    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
		    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT;
	int writeit = 0;
	u32 usb2phycfg;

	if (dev->core_params->phyctl || hiber || dev->core_params->lpmctl) {
		usb2phycfg = dwc3_readl(&dev->core_global_regs->gusb2phycfg[0]);

		if ((dev->core_params->phyctl || hiber) &&
		    !(usb2phycfg & DWC_USB2PHYCFG_SUS_PHY_BIT)) {
			usb2phycfg |= DWC_USB2PHYCFG_SUS_PHY_BIT;
			writeit = 1;
		}

		if (dev->core_params->lpmctl &&
		    !(usb2phycfg & DWC_USB2PHYCFG_ENBL_SLP_M_BIT)) {
			//usb2phycfg |= DWC_USB2PHYCFG_ENBL_SLP_M_BIT;
			//writeit = 1;
		}

		if (writeit)
			dwc3_writel(usb2phycfg, &dev->core_global_regs->gusb2phycfg[0]);
	}
}

/**
 * Disable USB2 Phy suspend.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
static void dwc3_dis_usb2_phy_suspend(dwc3_device_t *dwc3_dev)
{
	u32 usb2phycfg;

	usb2phycfg = dwc3_readl(&dwc3_dev->dwc3->core_global_regs->gusb2phycfg[0]);

	//TODO_FOR("check DWC_USB2PHYCFG_ENBL_SLP_M_BIT");
	//return;

	if (usb2phycfg & (DWC_USB2PHYCFG_SUS_PHY_BIT |
			  DWC_USB2PHYCFG_ENBL_SLP_M_BIT)) {
		usb2phycfg &= ~DWC_USB2PHYCFG_SUS_PHY_BIT;
		usb2phycfg &= ~DWC_USB2PHYCFG_ENBL_SLP_M_BIT;
		dwc3_writel(usb2phycfg,
			 &dwc3_dev->dwc3->core_global_regs->gusb2phycfg[0]);
	}
}

/**
 * Enable USB3 Phy suspend.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
static void dwc3_ena_usb3_phy_suspend(dwc3_device_t *dwc3_dev)
{
	u32 pipectl;

	pipectl = dwc3_readl(&dwc3_dev->dwc3->core_global_regs->gusb3pipectl[0]);
	if (!(pipectl & DWC_PIPECTL_SUS_PHY_BIT)) {
		pipectl |= DWC_PIPECTL_SUS_PHY_BIT;
		dwc3_writel(pipectl, &dwc3_dev->dwc3->core_global_regs->gusb3pipectl[0]);
	}
}

/**
 * Disable USB3 Phy suspend.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
static void dwc3_dis_usb3_phy_suspend(dwc3_device_t *dwc3_dev)
{
	u32 pipectl;

	pipectl = dwc3_readl(&dwc3_dev->dwc3->core_global_regs->gusb3pipectl[0]);
	if (pipectl & DWC_PIPECTL_SUS_PHY_BIT) {
		pipectl &= ~DWC_PIPECTL_SUS_PHY_BIT;
		dwc3_writel(pipectl,
			 &dwc3_dev->dwc3->core_global_regs->gusb3pipectl[0]);
	}
}

/**
 * Enable USB2 Phy suspend if the device is connected at HS or FS.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_ena_usb2_suspend(dwc3_device_t *dwc3_dev)
{
	if (dwc3_dev->speed == USB_SPEED_SUPER)
		return;

	dwc3_ena_usb2_phy_suspend(dwc3_dev);
}

/**
 * Disable USB2 Phy suspend if the device is connected at HS or FS.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_dis_usb2_suspend(dwc3_device_t *dwc3_dev)
{
	if (dwc3_dev->speed == USB_SPEED_SUPER)
		return;

	dwc3_dis_usb2_phy_suspend(dwc3_dev);
}

/**
 * Enable the Device to accept U1 control commands from the Host.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_accept_u1(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl |= DWC_DCTL_ACCEPT_U1_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Enable the Device to accept U2 control commands from the Host.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_accept_u2(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl |= DWC_DCTL_ACCEPT_U2_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Enable U1 sleep.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_enable_u1(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl |= DWC_DCTL_INIT_U1_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Enable U2 sleep.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_enable_u2(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl |= DWC_DCTL_INIT_U2_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Disable U1 sleep.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_disable_u1(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl &= ~DWC_DCTL_INIT_U1_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Disable U2 sleep.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_disable_u2(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	dctl &= ~DWC_DCTL_INIT_U2_EN_BIT;
	dwc3_writel(dctl, &dwc3_dev->dev_regs->dctl);
}

/**
 * Test whether U1 sleep is enabled.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @return      1 if enabled, 0 if not.
 */
int dwc3_u1_enabled(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	return !!(dctl & DWC_DCTL_INIT_U1_EN_BIT);
}

/**
 * Test whether U2 sleep is enabled.
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 * @return      1 if enabled, 0 if not.
 */
int dwc3_u2_enabled(dwc3_device_t *dwc3_dev)
{
	u32 dctl;

	dctl = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	return !!(dctl & DWC_DCTL_INIT_U2_EN_BIT);
}

/**
 * Clear 'eps_enabled' flag and 'ena_once' flags for all EPs, so EPs will get
 * completely reconfigured by SetConfig and SetInterface.
 */
void dwc3_clr_eps_enabled(dwc3_device_t *dwc3_dev)
{
	dwc3_device_ep_t *ep;
	int i;

	dwc3_dev->eps_enabled = 0;

	for (i = 0; i < dwc3_dev->num_in_eps; i++) {
		ep = dwc3_dev->in_ep[i];
		if (ep) {
			ep->dwc_ep.ena_once = 0;
			ep->dwc_ep.usb_ep_desc = NULL;
		}
	}

	for (i = 0; i < dwc3_dev->num_out_eps; i++) {
		ep = dwc3_dev->out_ep[i];
		if (ep) {
			ep->dwc_ep.ena_once = 0;
			ep->dwc_ep.usb_ep_desc = NULL;
		}
	}
}

/**
 * This routine calculates the number of IN EPs (excluding EP0)
 * using GHWPARAMS3 register values
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
static int calc_num_in_eps(dwc3_core_t *dev)
{
	u32 num_in_eps = dev->hwparams3 >> DWC_HWP3_NUM_IN_EPS_SHIFT &
		DWC_HWP3_NUM_IN_EPS_BITS >> DWC_HWP3_NUM_IN_EPS_SHIFT;

	return num_in_eps - 1;
}

/**
 * This routine calculates the number of OUT EPs (excluding EP0)
 * using GHWPARAMS3 register values
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
static int calc_num_out_eps(dwc3_core_t *dev)
{
	u32 num_eps = dev->hwparams3 >> DWC_HWP3_NUM_EPS_SHIFT &
		DWC_HWP3_NUM_EPS_BITS >> DWC_HWP3_NUM_EPS_SHIFT;
	u32 num_in_eps = dev->hwparams3 >> DWC_HWP3_NUM_IN_EPS_SHIFT &
		DWC_HWP3_NUM_IN_EPS_BITS >> DWC_HWP3_NUM_IN_EPS_SHIFT;

	return num_eps - num_in_eps - 1;
}

/**
 * This routine ensures the device is really a DWC_usb3 controller, by reading
 * and verifying the SNPSID register contents. The value should be 0x5533XXXX,
 * which corresponds to "U3", as in "USB3 version X.XXX".
 *
 * The SNPSID value is also saved in <em>dev->snpsid</em> for later use in
 * determining if any version-specific operations need to be performed.
 *
 * This routine should be called before any other initialization routines, to
 * ensure that the <em>dev->snpsid</em> value is set in case any of the other
 * routines need it.
 *
 * @param dev           Programming view of DWC_usb3 controller.
 * @param addr_ofs      Offset to the Device registers in the CSR space. It is
 *                      needed because this routine is called early, before the
 *                      normal register access routines are functional.
 * @return              0 if the SNPSID value is valid, -DWC_E_INVALID if not.
 */
int dwc3_device_check_snpsid(dwc3_core_t *dev, u32 addr_ofs)
{
	dev->snpsid = dwc3_readl((volatile u32 __iomem *) (intptr_t)
			(dev->base + addr_ofs + 0x120));
	if ((dev->snpsid & 0xffff0000) != 0x55330000) {
		ERR("Error! bad value for SNPSID: 0x%08x!\n", dev->snpsid);
		return -DWC_E_INVALID;
	} else
		DBG("SNPSID = 0x%08x\n", dev->snpsid);

	return 0;
}

int dwc3_core_init(dwc3_core_t *dwc3) {
	ENTER();
	dwc3_device_t *dwc3_dev = &dwc3->dwc3_dev;
	int ret, i;
	u32 temp;

	dwc3->core_params = &core_params;
	dwc3->core_global_regs = (dwc3_core_global_regs_t *) (intptr_t)
					(dwc3->base + DWC_CORE_GLOBAL_REG_OFFSET);

	dwc3_dev = &dwc3->dwc3_dev;

	dwc3_dev->dev_regs = (dwc3_device_regs_t *) (intptr_t)
					(dwc3->base + DWC_DEVICE_REG_OFFSET);
	dwc3_dev->in_ep_regs = (dwc3_dev_ep_regs_t *) (intptr_t)
					(dwc3->base + DWC_DEV_IN_EP_REG_OFFSET);
	dwc3_dev->out_ep_regs = (dwc3_dev_ep_regs_t *) (intptr_t)
					(dwc3->base + DWC_DEV_OUT_EP_REG_OFFSET);

	DBG("Global regs @%p Device regs @%p, in_ep_regs @%p, out_ep_regs @%p\n",
			dwc3->core_global_regs, dwc3_dev->dev_regs,
			dwc3_dev->in_ep_regs, dwc3_dev->out_ep_regs);

	/*
	 * Checks that this device is really a DWC_usb3 controller. Also saves
	 * the SNPSID register value in usb3_dev->snpsid for later use by the
	 * PCD.
	 */
	ret = dwc3_device_check_snpsid(dwc3, 0x0);
	if (ret) {
		ERR("Error! bad value for SNPSID!\n");
		goto fail;
	}
	/*
	 * Up to 32 Event Buffers are supported by the hardware, but we only
	 * use 1
	 */
#if 1
	dwc3->event_buf[0] = memalign(CACHELINE_SIZE,
					DWC_EVENT_BUF_SIZE * sizeof(u32));
	memset(dwc3->event_buf[0], '\0', DWC_EVENT_BUF_SIZE * sizeof(u32));
	dwc3->event_buf_dma[0] = (dwc_dma_t) (dwc3->event_buf[0]);
	DBG("Event Buffer @%08lx (dma %08lx)\n",
			(long unsigned int) dwc3->event_buf[0],
			(long unsigned int) dwc3->event_buf_dma[0]);
#else
	dwc3->event_buf[0] = g_event_buf;
	dwc3->event_buf_dma[0] = (dwc_dma_t)g_event_buf;
	DBG("Event Buffer @%08lx (dma %08lx) g_event_buf @%p\n",
			(long unsigned int) dwc3->event_buf[0],
			(long unsigned int) dwc3->event_buf_dma[0], g_event_buf);
#endif

	/*
	 * "allocate" all the data structures that must be in DMA memory
	 */
	dwc3->dwc3_dev.ep0_setup_desc = &g_ep0_setup_desc;
	dwc3->dwc3_dev.ep0_setup_desc_dma = (dwc_dma_t)&g_ep0_setup_desc;
	dwc3->dwc3_dev.ep0_out_desc = &g_ep0_out_desc;
	dwc3->dwc3_dev.ep0_out_desc_dma = (dwc_dma_t)&g_ep0_out_desc;
	dwc3->dwc3_dev.ep0_in_desc = &g_ep0_in_desc;
	dwc3->dwc3_dev.ep0_in_desc_dma = (dwc_dma_t)&g_ep0_in_desc;
	dwc3->dwc3_dev.ep0_status_buf = g_ep0_status_buf;
	dwc3->dwc3_dev.ep0_status_buf_dma = (dwc_dma_t)g_ep0_status_buf;
	dwc3->dwc3_dev.ep0_setup_pkt = (union dwc_setup_pkt *)g_ep0_setup_pkt;
	dwc3->dwc3_dev.ep0_setup_pkt_dma = (dwc_dma_t)g_ep0_setup_pkt;
	DBG("ep0_setup_desc@%08lx (dma %08lx)\n",
			(unsigned long) dwc3->dwc3_dev.ep0_setup_desc,
			(unsigned long) dwc3->dwc3_dev.ep0_setup_desc_dma);
	DBG("ep0_out_desc@%08lx (dma %08lx)\n",
			(unsigned long) dwc3->dwc3_dev.ep0_out_desc,
			(unsigned long) dwc3->dwc3_dev.ep0_out_desc_dma);
	DBG("ep0_in_desc@%08lx (dma %08lx)\n",
			(unsigned long) dwc3->dwc3_dev.ep0_in_desc,
			(unsigned long) dwc3->dwc3_dev.ep0_in_desc_dma);
	DBG("ep0_status_buf@%08lx (dma %08lx)\n",
			(unsigned long) dwc3->dwc3_dev.ep0_status_buf,
			(unsigned long) dwc3->dwc3_dev.ep0_status_buf_dma);
	DBG("ep0_setup_pkt@%08lx (dma %08lx)\n",
			(unsigned long) dwc3->dwc3_dev.ep0_setup_pkt,
			(unsigned long) dwc3->dwc3_dev.ep0_setup_pkt_dma);

	/*
	 * Now "allocate" the remaining data structures
	 */
	dwc3->dwc3_dev.ep0_req = &g_ep0_req;
	dwc3->dwc3_dev.ep0 = &g_ep0;
	DBG("EP0@0x%08lx\n", (unsigned long) dwc3->dwc3_dev.ep0);
	for (i = 0; i < DWC_MAX_EPS - 1; i++) {
		dwc3->dwc3_dev.out_ep[i] = &g_out_ep[i];
		dwc3->dwc3_dev.in_ep[i] = &g_in_ep[i];
		DBG("EP%d in @0x%08lx out @0x%08lx\n", i + 1,
			(unsigned long) dwc3->dwc3_dev.in_ep[i],
			(unsigned long) dwc3->dwc3_dev.out_ep[i]);
	}

	g_dev_req_bm = 0xffffffff;

	/*
	 * Store the contents of the hardware configuration registers here for
	 * easy access later.
	 */
	dwc3->hwparams0 = dwc3_readl(&dwc3->core_global_regs->ghwparams0);
	dwc3->hwparams1 = dwc3_readl(&dwc3->core_global_regs->ghwparams1);
	dwc3->hwparams2 = dwc3_readl(&dwc3->core_global_regs->ghwparams2);
	dwc3->hwparams3 = dwc3_readl(&dwc3->core_global_regs->ghwparams3);
	dwc3->hwparams4 = dwc3_readl(&dwc3->core_global_regs->ghwparams4);
	dwc3->hwparams5 = dwc3_readl(&dwc3->core_global_regs->ghwparams5);
	dwc3->hwparams6 = dwc3_readl(&dwc3->core_global_regs->ghwparams6);
	dwc3->hwparams7 = dwc3_readl(&dwc3->core_global_regs->ghwparams7);
	dwc3->hwparams8 = dwc3_readl(&dwc3->core_global_regs->ghwparams8);

	if (dwc3->program_gsbuscfg) {
		DBG("Programming SBUSCFG0,1 to 0x%08x 0x%08x\n",
			   dwc3->gsbuscfg0, dwc3->gsbuscfg1);
		dwc3_writel(dwc3->gsbuscfg0, &dwc3->core_global_regs->gsbuscfg0);
		dwc3_writel(dwc3->gsbuscfg1, &dwc3->core_global_regs->gsbuscfg1);
	}


	temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
	DBG("dcfg=0x%08x\n", temp);

	DBG("mode=0x%0x\n",
		   dwc3->hwparams0 >> DWC_HWP0_MODE_SHIFT &
		   DWC_HWP0_MODE_BITS >> DWC_HWP0_MODE_SHIFT);
	DBG("num_ep=%d\n",
		   dwc3->hwparams3 >> DWC_HWP3_NUM_EPS_SHIFT &
		   DWC_HWP3_NUM_EPS_BITS >> DWC_HWP3_NUM_EPS_SHIFT);
	DBG("num_in_ep=%d\n",
		   dwc3->hwparams3 >> DWC_HWP3_NUM_IN_EPS_SHIFT &
		   DWC_HWP3_NUM_IN_EPS_BITS >> DWC_HWP3_NUM_IN_EPS_SHIFT);
	DBG("dfq_fifo_depth=%d\n",
	       dwc3->hwparams5 >> DWC_HWP5_DFQ_FIFO_DEPTH_SHIFT &
	       DWC_HWP5_DFQ_FIFO_DEPTH_BITS >> DWC_HWP5_DFQ_FIFO_DEPTH_SHIFT);
	DBG("dwq_fifo_depth=%d\n",
	       dwc3->hwparams5 >> DWC_HWP5_DWQ_FIFO_DEPTH_SHIFT &
	       DWC_HWP5_DWQ_FIFO_DEPTH_BITS >> DWC_HWP5_DWQ_FIFO_DEPTH_SHIFT);
	DBG("txq_fifo_depth=%d\n",
	       dwc3->hwparams5 >> DWC_HWP5_TXQ_FIFO_DEPTH_SHIFT &
	       DWC_HWP5_TXQ_FIFO_DEPTH_BITS >> DWC_HWP5_TXQ_FIFO_DEPTH_SHIFT);
	DBG("rxq_fifo_depth=%d\n",
	       dwc3->hwparams5 >> DWC_HWP5_RXQ_FIFO_DEPTH_SHIFT &
	       DWC_HWP5_RXQ_FIFO_DEPTH_BITS >> DWC_HWP5_RXQ_FIFO_DEPTH_SHIFT);

	/* Initialize parameters from Hardware configuration registers. */
	dwc3->dwc3_dev.num_in_eps = calc_num_in_eps(dwc3);
	if (dwc3->dwc3_dev.num_in_eps > 15) {
		DBG("Number of IN endpoints (%d) too large\n",
			   dwc3->dwc3_dev.num_in_eps);
		return -DWC_E_INVALID;
	}

	dwc3->dwc3_dev.num_out_eps = calc_num_out_eps(dwc3);
	if (dwc3->dwc3_dev.num_out_eps > 15) {
		DBG("Number of OUT endpoints (%d) too large\n",
			   dwc3->dwc3_dev.num_out_eps);
		return -DWC_E_INVALID;
	}

	/*
	 * TODO Workaround: PCD can't handle soft reset during HNP.
	 * RTL issue will be fixed. Skip the reset when called with
	 * soft_reset=0. When not configured for OTG do the reset
	 * unconditionally.
	 */
	/* Soft-reset the core */
	temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	temp &= ~DWC_DCTL_RUN_STOP_BIT;
	temp |= DWC_DCTL_CSFT_RST_BIT;
	dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);

	/* Wait for core to come out of reset */
	do {
		mdelay(1);
		temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	} while (temp & DWC_DCTL_CSFT_RST_BIT);

		/* Wait for at least 3 PHY clocks */
	mdelay(1);

	dwc3_core_soft_reset(dwc3);

	EXIT();
	return 0;

fail:
	ERR("Error! %s Fail", __func__);
	return -1;
}

void dwc3_core_remove(dwc3_core_t *dev)
{
	ENTER();
	EXIT();
}

/**
 * Set the size of the Tx FIFOs
 *
 * NOTE: The following code for setting the FIFO sizes only
 * works for cores configured with the 3 RAM option. Setting
 * FIFO sizes for the 2 RAM option is not implemented.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 * @param sz    New sizes for the FIFOs.
 */
void dwc3_set_tx_fifo_size(dwc3_core_t *dev, int *sz)
{
	dwc3_core_global_regs_t *global_regs =
						dev->core_global_regs;
	int i, ram_width, ram_depth, size, prev_start, txsz[DWC_MAX_TX_FIFOS];

	ram_width = (dev->hwparams0 >> DWC_HWP0_MDWIDTH_SHIFT &
		     DWC_HWP0_MDWIDTH_BITS >> DWC_HWP0_MDWIDTH_SHIFT)
		    / 8;
	ram_depth = (dev->hwparams7 >> DWC_HWP7_RAM1_DEPTH_SHIFT &
		     DWC_HWP7_RAM1_DEPTH_BITS >> DWC_HWP7_RAM1_DEPTH_SHIFT)
		    * ram_width;
	size = dwc3_readl(&global_regs->gtxfifosiz[0]);
	prev_start = size >> DWC_FIFOSZ_STARTADDR_SHIFT &
		DWC_FIFOSZ_STARTADDR_BITS >> DWC_FIFOSZ_STARTADDR_SHIFT;

	for (i = 0; i < dev->dwc3_dev.num_in_eps + 1; i++) {
		size = sz[i];

		if (i == 0 && size && size < 512 + 2 * ram_width) {
			ERR("Error! Requested Tx FIFO %d size too small\n",
				   i);
			ERR("Error! Not setting Tx FIFO sizes\n");
			goto txerr;
		}

		if (!size) {
			/* Default to 512 for EP0, 1K for others */
			size = i ? 1024 : 512;
			size += 2 * ram_width;
		}

		size = (size + ram_width - 1) & ~(ram_width - 1);
		DBG("Tx FIFO %d size = %d bytes out of %d available\n",
			   i, size, ram_depth);
		if (size > ram_depth) {
			ERR("Error! Requested Tx FIFO %d size too large\n",
				   i);
			ERR("Error! Not setting Tx FIFO sizes\n");
			goto txerr;
		}

		txsz[i] = size;
		ram_depth -= size;
	}

	for (i = 0; i < dev->dwc3_dev.num_in_eps + 1; i++) {
		size = txsz[i];
		DBG("Setting GTXFIFOSIZ%d = 0x%08x\n", i,
			   (size / ram_width) << DWC_FIFOSZ_DEPTH_SHIFT |
			   prev_start << DWC_FIFOSZ_STARTADDR_SHIFT);
		dwc3_writel((size / ram_width) << DWC_FIFOSZ_DEPTH_SHIFT |
				prev_start << DWC_FIFOSZ_STARTADDR_SHIFT,
				&global_regs->gtxfifosiz[i]);

		DBG("GTXFIFOSIZ%d = 0x%08x\n", i,
			   dwc3_readl(&global_regs->gtxfifosiz[i]));
		prev_start += size / ram_width;
	}
txerr:
	return;
}

/**
 * Set the size of the Rx FIFO
 *
 * NOTE: The following code for setting the FIFO sizes only
 * works for cores configured with the 3 RAM option. Setting
 * FIFO sizes for the 2 RAM option is not implemented.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 * @param size  New size for the FIFO.
 */
void dwc3_set_rx_fifo_size(dwc3_core_t *dev, int size)
{
	dwc3_core_global_regs_t *global_regs =
						dev->core_global_regs;
	u32 sz, rxthrcfg;
	int ram_width, ram_depth, prev_start, cnt, bst;

	ram_width = (dev->hwparams0 >> DWC_HWP0_MDWIDTH_SHIFT &
		     DWC_HWP0_MDWIDTH_BITS >> DWC_HWP0_MDWIDTH_SHIFT)
		    / 8;
	ram_depth = (dev->hwparams7 >> DWC_HWP7_RAM2_DEPTH_SHIFT &
		     DWC_HWP7_RAM2_DEPTH_BITS >> DWC_HWP7_RAM2_DEPTH_SHIFT)
		    * ram_width;
	sz = dwc3_readl(&global_regs->grxfifosiz[0]);
	prev_start = sz >> DWC_FIFOSZ_STARTADDR_SHIFT &
		     DWC_FIFOSZ_STARTADDR_BITS >> DWC_FIFOSZ_STARTADDR_SHIFT;

	if (size < 512 + 24 + 16 + (ram_width == 16 ? 24 : 0)) {
		ERR("Error! Requested Rx FIFO size too small\n");
		ERR("Error! Not setting Rx FIFO size\n");
		goto rxerr;
	}

	size = (size + ram_width - 1) & ~(ram_width - 1);
	if (size > ram_depth) {
		ERR("Error! Requested Rx FIFO size too large\n");
		ERR("Error! Not setting Rx FIFO size\n");
		goto rxerr;
	}

	DBG("Setting GRXFIFOSIZ0=0x%08x\n",
		   (size / ram_width) << DWC_FIFOSZ_DEPTH_SHIFT |
		   prev_start << DWC_FIFOSZ_STARTADDR_SHIFT);
	dwc3_writel((size / ram_width) << DWC_FIFOSZ_DEPTH_SHIFT |
			prev_start << DWC_FIFOSZ_STARTADDR_SHIFT,
			&global_regs->grxfifosiz[0]);
	DBG("GRXFIFOSIZ0=0x%08x\n",
		   dwc3_readl(&global_regs->grxfifosiz[0]));
	DBG("Rx FIFO size=%d bytes out of %d available\n",
		   size, ram_depth);

	/*
	 * If thresholding is enabled in GRXTHRCFG, update USBRxPktCnt according
	 * to the new FIFO size
	 */
	rxthrcfg = dwc3_readl(&global_regs->grxthrcfg);
	DBG("GRXTHRCFG=0x%08x\n", rxthrcfg);
	if (rxthrcfg & DWC_RXTHRCTL_USB_RX_PKT_CNT_EN_BIT) {
		cnt = (size - ram_width * 4) / 1024;
		if (cnt > 0) {
			if (cnt > DWC_RXTHRCTL_USB_RX_PKT_CNT_BITS >>
					DWC_RXTHRCTL_USB_RX_PKT_CNT_SHIFT)
				cnt = DWC_RXTHRCTL_USB_RX_PKT_CNT_BITS >>
					DWC_RXTHRCTL_USB_RX_PKT_CNT_SHIFT;
			bst = rxthrcfg >>
				DWC_RXTHRCTL_USB_MAX_RX_BURST_SIZE_SHIFT &
			     DWC_RXTHRCTL_USB_MAX_RX_BURST_SIZE_BITS >>
				DWC_RXTHRCTL_USB_MAX_RX_BURST_SIZE_SHIFT;
			if (cnt > bst)
				cnt = bst;
			if (cnt < 1)
				goto disable;
			rxthrcfg &= ~DWC_RXTHRCTL_USB_RX_PKT_CNT_BITS;
			rxthrcfg |= cnt << DWC_RXTHRCTL_USB_RX_PKT_CNT_SHIFT;
		} else {
disable:
			rxthrcfg &= ~DWC_RXTHRCTL_USB_RX_PKT_CNT_EN_BIT;
		}
		DBG("Setting GRXTHRCFG=0x%08x\n", rxthrcfg);
		dwc3_writel(rxthrcfg, &global_regs->grxthrcfg);
	}
rxerr:
	return;
}

/**
 * This routine initializes the DWC_usb3 controller registers.
 *
 * If the <strong><em>soft_reset</em></strong> parameter is
 * <strong>true</strong>, then this routine must be called in a context that
 * allows <em>dwc_msleep()</em> to be used, because that function is called
 * while waiting for the core to come out of reset.
 *
 * This routine is called by dwc3_device_init() when the driver is loaded,
 * so it normally does not need to be called separately, except in special
 * circumstances, such as when exiting from hibernation.
 *
 * @param dev           Programming view of DWC_usb3 controller.
 * @param soft_reset    True if doing a soft reset of the core.
 * @param restore       True if restoring register state after hibernation.
 */
void dwc3_core_device_init(dwc3_core_t *dev, int soft_reset,
			      int restore)
{
	ENTER();
	dwc3_core_global_regs_t *global_regs =
						dev->core_global_regs;
	dwc3_device_t *dwc3_dev = &dev->dwc3_dev;
	u32 temp;
	int i, ram_width, ram_depth, size;

	DBG("%s()\n", __func__);

	/* Soft reset clears the GSBUSCFG registers, so write them again */
	if (dev->program_gsbuscfg) {
		DBG("Programming SBUSCFG0,1 to 0x%08x 0x%08x\n",
			   dev->gsbuscfg0, dev->gsbuscfg1);
		dwc3_writel(dev->gsbuscfg0, &global_regs->gsbuscfg0);
		dwc3_writel(dev->gsbuscfg1, &global_regs->gsbuscfg1);
	}

	dwc3_dev->link_state = 0;
	dwc3_dev->wkup_rdy = 0;

	/*
	 * Set Tx FIFO sizes
	 */
	ram_width = (dev->hwparams0 >> DWC_HWP0_MDWIDTH_SHIFT &
		     DWC_HWP0_MDWIDTH_BITS >> DWC_HWP0_MDWIDTH_SHIFT)
		/ 8;
	ram_depth = (dev->hwparams7 >> DWC_HWP7_RAM1_DEPTH_SHIFT &
		     DWC_HWP7_RAM1_DEPTH_BITS >> DWC_HWP7_RAM1_DEPTH_SHIFT)
		* ram_width;
	DBG("RAM width:%d RAM1 depth:%d\n", ram_width, ram_depth);

	for (i = 0; i < dwc3_dev->num_in_eps + 1; i++) {
		size = dwc3_readl(&global_regs->gtxfifosiz[i]);
		DBG("Initial GTXFIFOSIZ%d = 0x%08x\n", i, size);
	}

	/* Only set if non-default Tx FIFO sizes were requested */
	if (dev->core_params->txfsz_cnt)
		dwc3_set_tx_fifo_size(dev, dev->core_params->txfsz);

	/*
	 * Set Rx FIFO size
	 */
	ram_depth = (dev->hwparams7 >> DWC_HWP7_RAM2_DEPTH_SHIFT &
		     DWC_HWP7_RAM2_DEPTH_BITS >> DWC_HWP7_RAM2_DEPTH_SHIFT)
		* ram_width;
	DBG("RAM2 depth:%d\n", ram_depth);
	size = dwc3_readl(&global_regs->grxfifosiz[0]);
	DBG("Initial GRXFIFOSIZ0 = 0x%08x\n", size);
	size = dev->core_params->rxfsz;

	/* Only set if non-default Rx FIFO size was requested */
	if (size)
		dwc3_set_rx_fifo_size(dev, size);

#if 1
#define RX_THRESHOLD_EN 		(1<<29)
#define RX_PKT_CNT(n) 			(n<<24)
#define RX_MAX_BURST_SZ(n) 		(n<<19)

	dwc3_writel(0x01010000, &global_regs->gtxthrcfg);
	dwc3_writel(RX_THRESHOLD_EN | RX_PKT_CNT(3) | RX_MAX_BURST_SZ(3),
				&global_regs->grxthrcfg);
	dwc3_writel(dwc3_readl(&global_regs->guctl) | (1<<14), &global_regs->guctl);   // enable auto retry
#endif

	/* Initialize the Event Buffer registers */
	dwc3_init_eventbuf(dev, 0, dev->event_buf[0], DWC_EVENT_BUF_SIZE,
			       dev->event_buf_dma[0]);
	dev->event_ptr[0] = dev->event_buf[0];

	/* Set device mode */
	temp = dwc3_readl(&global_regs->gctl);
	temp &= ~(DWC_GCTL_PRT_CAP_DIR_BITS | DWC_GCTL_SCALE_DOWN_BITS);

	temp |= DWC_GCTL_PRT_CAP_DEVICE << DWC_GCTL_PRT_CAP_DIR_SHIFT;

	dwc3_writel(temp, &global_regs->gctl);


	/* If forcing to a USB2 mode was requested */
	if (dev->core_params->usb2mode == 1) {
		/* Set speed to Full */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		temp &= ~(DWC_DCFG_DEVSPD_BITS << DWC_DCFG_DEVSPD_SHIFT);
		temp |= DWC_SPEED_FS_PHY_30MHZ_OR_60MHZ
						<< DWC_DCFG_DEVSPD_SHIFT;
		dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);

	} else if (dev->core_params->usb2mode == 2){
		/* Set speed to High */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		temp &= ~(DWC_DCFG_DEVSPD_BITS << DWC_DCFG_DEVSPD_SHIFT);
		temp |= DWC_SPEED_HS_PHY_30MHZ_OR_60MHZ
						<< DWC_DCFG_DEVSPD_SHIFT;
		dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);

	} else {
		/* Set highest supported speed in DCFG */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		temp &= ~(DWC_DCFG_DEVSPD_BITS << DWC_DCFG_DEVSPD_SHIFT);

		if ((dev->hwparams3 & DWC_HWP3_SSPHY_IFC_BITS) == 0) {
			temp |= DWC_SPEED_HS_PHY_30MHZ_OR_60MHZ
						<< DWC_DCFG_DEVSPD_SHIFT;
			dwc3_dev->capable = USB_SPEED_HIGH;
		}
		/* USB 3.1 Gen 2 */
		else if (((dev->hwparams3 & DWC_HWP3_SSPHY_IFC_BITS) == 2)
			&& (dev->core_params->usb3gen1 == 0)) {
			temp |= DWC_SPEED_SS_PHY_SSP
				<< DWC_DCFG_DEVSPD_SHIFT;
			dwc3_dev->capable = USB_SPEED_SUPER_PLUS;
		}
		else {
			temp |= DWC_SPEED_SS_PHY_125MHZ_OR_250MHZ
						<< DWC_DCFG_DEVSPD_SHIFT;
			dwc3_dev->capable = USB_SPEED_SUPER;
		}
		dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);
	}

	/* If LPM enable was requested */
	if (dev->core_params->lpmctl) {
		/* Set LPMCap bit */
		//temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		//temp |= DWC_DCFG_LPM_CAP_BIT;
		//dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);

		if (dev->core_params->lpmctl > 1) {
			/* Set AppL1Res bit */
			temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
			temp |= DWC_DCTL_APP_L1_RES_BIT;
			dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);
		}
	}

	/* If non-default NUMP was requested */
	if (dev->core_params->nump > 0 && dev->core_params->nump <= 16) {
		/* Set NUMP */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dcfg);
		temp &= ~DWC_DCFG_NUM_RCV_BUF_BITS;
		temp |= dev->core_params->nump << DWC_DCFG_NUM_RCV_BUF_SHIFT;
		dwc3_writel(temp, &dwc3_dev->dev_regs->dcfg);
	}

	if (!restore)
		/* Set device address to 0 */
		dwc3_set_address(dwc3_dev, 0);

	/* Enable hibernation if supported */
	if (dev->core_params->hibernate &&
	    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT) {
		/* Enable global hibernation bit */
		temp = dwc3_readl(&global_regs->gctl);
		temp |= DWC_GCTL_GBL_HIBER_EN_BIT;
		if (dev->core_params->clkgatingen)
			temp &= ~DWC_GCTL_DSBL_CLCK_GTNG_BIT;
		else
			temp |= DWC_GCTL_DSBL_CLCK_GTNG_BIT;
		dwc3_writel(temp, &global_regs->gctl);

		if (dev->core_params->lpmctl) {
			/* Set L1 hibernation values */
			temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
			temp &= ~DWC_DCTL_HIRD_THR_BITS;
			if (dev->hird_thresh)
				temp |= dev->hird_thresh << DWC_DCTL_HIRD_THR_SHIFT &
					DWC_DCTL_HIRD_THR_BITS;
			else
				temp |= 0x1c << DWC_DCTL_HIRD_THR_SHIFT;

			/* Enable L1 hibernation */
			temp |= DWC_DCTL_L1_HIBER_EN_BIT;
			dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);
		}

		/* Set GUSB2PHYCFG[6] (suspend 2.0 phy) */
		temp = dwc3_readl(&global_regs->gusb2phycfg[0]);
		if (dev->core_params->phyctl)
			temp |= DWC_USB2PHYCFG_SUS_PHY_BIT;
		//if (dev->core_params->lpmctl)
		//	temp |= DWC_USB2PHYCFG_ENBL_SLP_M_BIT;
		dwc3_writel(temp, &global_regs->gusb2phycfg[0]);

		/* Set GUSB3PIPECTL[17] (suspend SS phy) */
		temp = dwc3_readl(&global_regs->gusb3pipectl[0]);
		temp |= DWC_PIPECTL_SUS_PHY_BIT;
		dwc3_writel(temp, &global_regs->gusb3pipectl[0]);
	} else {
		if (dev->core_params->phyctl) {
			/* Enable Phy suspend */
			dwc3_ena_usb3_phy_suspend(dwc3_dev);
			dwc3_ena_usb2_phy_suspend(dwc3_dev);
		} else {
			/* Disable Phy suspend */
			dwc3_dis_usb3_phy_suspend(dwc3_dev);
			dwc3_dis_usb2_phy_suspend(dwc3_dev);
		}
	}

	/* NYET on short BULK OUT in HS. For cores >= 3.00a. No effect
	 * on earlier cores as this bit is reserved. */
	if (dev->core_params->nyet) {
		temp = dwc3_readl(&global_regs->guctl1);
		temp |= DWC_GUCTL1_NYET_BIT;
		dwc3_writel(temp, &global_regs->guctl1);
	}

	/* Separate L1/L2 events. For cores >= 3.00a. */
	if (dev->snpsid >= 0x5555300a) {
		temp = dwc3_readl(&global_regs->guctl1);
		temp |= DWC_GUCTL1_DECOUPLE_L1L2_BIT;
		dwc3_writel(temp, &global_regs->guctl1);
	}

	/* Reserve space on RX Fifo for ISOC OUT. For cores >= 3.00a. */
	if (dev->core_params->rx_fifo_isoc_out >= 0) {
		temp = dwc3_readl(&global_regs->grxthrcfg);
		temp &= ~DWC_RXTHRCTL_RX_FIFO_ISOC_OUT_BITS;
		temp |= (dev->core_params->rx_fifo_isoc_out << DWC_RXTHRCTL_RX_FIFO_ISOC_OUT_SHIFT);
		dwc3_writel(temp, &global_regs->grxthrcfg);
	}

	/* Interrupt moderation. For cores >= 3.00a. */
	if (dev->core_params->imod) {
		dwc3_writel(dev->core_params->imod, &dwc3_dev->dev_regs->imod[0]);
	} else {
		dwc3_writel(0, &dwc3_dev->dev_regs->imod[0]);
	}
	EXIT();
}

void dwc3_core_device_start(dwc3_core_t *dev,
			      int restore)
{
	ENTER();
	dwc3_device_t *dwc3_dev = &dev->dwc3_dev;
	u32 temp;

	/* Activate EP0 */
	dwc3_ep0_activate(dwc3_dev, restore);

	/* Enable EP0-OUT/IN in DALEPENA register */
	dwc3_writel(3, &dwc3_dev->dev_regs->dalepena);

	/* Start EP0 to receive SETUP packets */
	dwc3_device_ep0_out_start(dwc3_dev);

	dwc3_dev->eps_enabled = 0;

	/* Enable Global and Device interrupts */
	dwc3_enable_device_interrupts(dev);

	//dwc3_dump_global_registers(dev);
	//dwc3_dump_dev_registers(&dev->dwc3_dev);
	//dwc3_dump_ep_registers(&dev->dwc3_dev);

	/* Set Run/Stop bit, and Keep-Connect bit if hibernation enabled */
	temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	temp |= DWC_DCTL_RUN_STOP_BIT;
	if (dev->core_params->hibernate &&
	    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT)
		temp |= DWC_DCTL_KEEP_CONNECT_BIT;
	dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);

	/* Wait for core to run */
	do {
		udelay(1);
		temp = dwc3_readl(&dwc3_dev->dev_regs->dsts);
		if (!(temp & DWC_DSTS_DEV_CTRL_HLT_BIT))
			break;
		udelay(1);
	} while (1);

	EXIT();
}

/**
 * This routine deinitializes the DWC_usb3 controller registers.
 *
 * This routine is called by dwc3_device_remove() when the driver is unloaded,
 * so it normally does not need to be called separately,
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
void dwc3_core_device_remove(dwc3_core_t *dev)
{
	ENTER();
	dwc3_core_global_regs_t *global_regs =
						dev->core_global_regs;
	dwc3_device_t *dwc3_dev = &dev->dwc3_dev;
	u32 temp;

	if (dev->hibernate >= DWC_HIBER_SLEEPING)
		return;

	/* Clear the Run/Stop and Keep-Connect bits */
	temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
	temp &= ~(DWC_DCTL_RUN_STOP_BIT | DWC_DCTL_KEEP_CONNECT_BIT);
	dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);

	/* Disable device interrupts */
	dwc3_writel(0, &dwc3_dev->dev_regs->devten);

	/* Disable hibernation if supported */
	if (dev->core_params->hibernate &&
	    (dev->hwparams1 & DWC_HWP1_EN_PWROPT_BITS) ==
	    DWC_EN_PWROPT_HIBERNATION << DWC_HWP1_EN_PWROPT_SHIFT) {
		/* Clear GUSB3PIPECTL[17] (suspend SS phy) */
		temp = dwc3_readl(&global_regs->gusb3pipectl[0]);
		temp &= ~DWC_PIPECTL_SUS_PHY_BIT;
		dwc3_writel(temp, &global_regs->gusb3pipectl[0]);

		/* Clear GUSB2PHYCFG[6] (suspend 2.0 phy) */
		temp = dwc3_readl(&global_regs->gusb2phycfg[0]);
		temp &= ~DWC_USB2PHYCFG_SUS_PHY_BIT;
		temp &= ~DWC_USB2PHYCFG_ENBL_SLP_M_BIT;
		dwc3_writel(temp, &global_regs->gusb2phycfg[0]);

		/* Disable L1 hibernation */
		temp = dwc3_readl(&dwc3_dev->dev_regs->dctl);
		temp &= ~DWC_DCTL_L1_HIBER_EN_BIT;
		dwc3_writel(temp, &dwc3_dev->dev_regs->dctl);

		/* Disable global hibernation bit */
		temp = dwc3_readl(&global_regs->gctl);
		temp &= ~DWC_GCTL_GBL_HIBER_EN_BIT;
		temp |= DWC_GCTL_DSBL_CLCK_GTNG_BIT;
		dwc3_writel(temp, &global_regs->gctl);
	}
	EXIT();
}
