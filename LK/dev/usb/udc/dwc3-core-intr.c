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
 * This routine enables the Event Buffer interrupt.
 */
static void ena_eventbuf_intr(dwc3_core_t *dev, int bufno)
{
	u32 eventsiz;

	eventsiz =
			dwc3_readl(&dev->core_global_regs->geventbuf[bufno].geventsiz);
	eventsiz &= ~DWC_EVENTSIZ_INT_MSK_BIT;
	dwc3_writel(eventsiz, &dev->core_global_regs->geventbuf[bufno].geventsiz);
}

/**
 * This routine disables the Event Buffer interrupt.
 */
static void dis_eventbuf_intr(dwc3_core_t *dev, int bufno)
{
	u32 eventsiz;

	eventsiz =
			dwc3_readl(&dev->core_global_regs->geventbuf[bufno].geventsiz);
	eventsiz |= DWC_EVENTSIZ_INT_MSK_BIT;
	dwc3_writel(eventsiz, &dev->core_global_regs->geventbuf[bufno].geventsiz);
}

/**
 * This routine disables the Event Buffer interrupt and flushes any pending
 * events from the buffer.
 */
void dwc3_dis_flush_eventbuf_intr(dwc3_core_t *dev, int bufno)
{
	u32 cnt;

	if (dev->hibernate >= DWC_HIBER_SLEEPING)
		return;

	dis_eventbuf_intr(dev, bufno);
	cnt = dwc3_readl(&dev->core_global_regs->geventbuf[bufno].geventcnt);
	dwc3_writel(cnt, &dev->core_global_regs->geventbuf[bufno].geventcnt);
}

/**
 * This routine reads the current Event Buffer count.
 */
static int get_eventbuf_count(dwc3_core_t *dev, int bufno)
{
	u32 cnt;

	cnt = dwc3_readl(&dev->core_global_regs->geventbuf[bufno].geventcnt);
	return cnt & DWC_EVENTCNT_CNT_BITS;
}

/**
 * This routine clears the Event Handler Busy bit.
 */
static void clear_event_handler_busy(dwc3_core_t *dev, int bufno)
{
	dwc3_writel(0x80000000, &dev->core_global_regs->geventbuf[bufno].geventcnt);
}

/**
 * This routine writes the Event Buffer count.
 */
static void update_eventbuf_count(dwc3_core_t *dev, int bufno, int cnt)
{
	dwc3_writel(cnt, &dev->core_global_regs->geventbuf[bufno].geventcnt);
	wmb();
}

/**
 * This routine fetches the next event from the Event Buffer.
 */
static u32 get_eventbuf_event(dwc3_core_t *dev, int bufno, int size)
{
	u32 event;

	dwc3_inval_cache((uintptr_t)dev->event_buf[bufno], size * 4);
	event = *dev->event_ptr[bufno]++;
	if (dev->event_ptr[bufno] >= dev->event_buf[bufno] + size)
		dev->event_ptr[bufno] = dev->event_buf[bufno];
	return event;
}

/**
 * This routine initializes an Event Buffer.
 */
void dwc3_init_eventbuf(dwc3_core_t *dev, int bufno,
			    u32 *addr, int size, dwc_dma_t dma_addr)
{
	DBG("Event buf %d addr 0x%08lx phys 0x%08lx size %d\n",
		   bufno, (unsigned long)addr, (unsigned long)dma_addr, size);
	dwc3_writel(dma_addr & 0xffffffffU,
			&dev->core_global_regs->geventbuf[bufno].geventadr_lo);
#ifdef DWC_64_BIT_ARCH
	dwc3_writel(dma_addr >> 32U & 0xffffffffU,
			&dev->core_global_regs->geventbuf[bufno].geventadr_hi);
#else
	dwc3_writel(0, &dev->core_global_regs->geventbuf[bufno].geventadr_hi);
#endif
	dwc3_writel(size << 2, &dev->core_global_regs->geventbuf[bufno].geventsiz);
	dwc3_writel(0, &dev->core_global_regs->geventbuf[bufno].geventcnt);
}

/**
 * This routine initializes the commmon interrupts.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
void dwc3_enable_common_interrupts(dwc3_core_t *dev)
{
	ENTER();
	/* Clear any pending interrupts */
	dwc3_dis_flush_eventbuf_intr(dev, 0);

	ena_eventbuf_intr(dev, 0);
	EXIT();
}

/**
 * This routine enables the Device mode interrupts.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
void dwc3_enable_device_interrupts(dwc3_core_t *dev)
{
	ENTER();
	u32 devten;

	/* Enable global interrupts */
	dwc3_enable_common_interrupts(dev);

	devten = DWC_DEVTEN_DISCONN_BIT | DWC_DEVTEN_CONNDONE_BIT |
		 DWC_DEVTEN_USBRESET_BIT | DWC_DEVTEN_HIBER_REQ_BIT |
		 DWC_DEVTEN_WKUP_BIT;

	if (dev->snpsid >= 0x5533230a)
		devten |= DWC_DEVTEN_U3_L2L1_SUSP_BIT;
	else
		devten |= DWC_DEVTEN_ULST_CHNG_BIT;

	if (dev->snpsid >= 0x5555300a) {
		devten |= DWC_DEVTEN_L1_SUSPEND_BIT;
		devten |= DWC_DEVTEN_L1_WAKEUP_BIT;
	}

	/* Enable device interrupts */
	dwc3_writel(devten, &dev->dwc3_dev.dev_regs->devten);

	DBG("%s() devten=%0x\n", __func__,
		   dwc3_readl(&dev->dwc3_dev.dev_regs->devten));
	EXIT();
}

/**
 * This routine handles all interrupt events. It is called by the
 * dwc3_irq() interrupt handler routine, and by the enter_hibernation()
 * routine after clearing the Run/Stop bit and waiting for the Halted bit to
 * be set.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 * return       1 if an interrupt event was seen, 0 if not.
 */
int dwc3_handle_event(dwc3_core_t *dev)
{
	dwc3_device_t *dwc3_dev = &dev->dwc3_dev;
	u32 event;
	int count, intr, physep, i;
	int ret = 0;
	static int msg_cnt;

	count = get_eventbuf_count(dev, 0);

	if (dev->core_params->imod) {
		u32 tmp = dwc3_readl(&dev->core_global_regs->geventbuf[0].geventcnt);

		/* If IMOD is enabled, check that the EHB bit is set.
		 * Don't handle until it is. We may reach here if we
		 * share and interrupt with another device in the
		 * system. */
		if (!(tmp & 0x80000000)) {
			goto out;
		}
	}

	if ((count & DWC_EVENTCNT_CNT_BITS) ==
					(0xffffffff & DWC_EVENTCNT_CNT_BITS) ||
	    count >= DWC_EVENT_BUF_SIZE * 4) {
		if (msg_cnt > 100) {
			msg_cnt = 100;
			WARN("Too many bad events!!\n");
		} else {
			msg_cnt++;
			WARN("Bad event count 0x%01x in dwc_usb3_irq() !!\n",
				count);
		}

		dis_eventbuf_intr(dev, 0);
		update_eventbuf_count(dev, 0, count);
		count = 0;
	}

	if (!count)
		goto out;
	else
		DBG("Interrupt count %d\n", count);

	ret = 1;

#if defined(CONFIG_IPMATE) || defined(COSIM) || defined(VIRTIO_MODEL)
	dis_eventbuf_intr(dev, 0);
#endif

	for (i = 0; i < count; i += 4) {
		DBG("Event addr 0x%08lx\n",
			   (unsigned long)dev->event_ptr[0]);
		event = get_eventbuf_event(dev, 0, DWC_EVENT_BUF_SIZE);
		update_eventbuf_count(dev, 0, 4);
		if (event == 0) {
			INF("## Null event! ##\n");
			/* Ignore null events */
			continue;
		}

		DBG("Interrupt event 0x%08x\n", event);
		if (event & DWC_EVENT_NON_EP_BIT) {
			DBG("Non-EP interrupt event\n");
			intr = event & DWC_EVENT_INTTYPE_BITS;

			if (intr ==
			    DWC_EVENT_DEV_INT << DWC_EVENT_INTTYPE_SHIFT) {
				DBG("## Device interrupt 0x%08x ##\n",
					   event);
				ret = dwc3_handle_dev_intr(dwc3_dev, event);
				if (ret) {
					ret = 2;
					goto out;
				}
				ret = 1;
			} else {
				DBG("## Core interrupt 0x%08x ##\n",
					   event);

				/* @todo Handle non-Device interrupts
				 * (OTG, CarKit, I2C)
				 */
			}
		} else {
			physep = event >> DWC_DEPEVT_EPNUM_SHIFT &
				DWC_DEPEVT_EPNUM_BITS >> DWC_DEPEVT_EPNUM_SHIFT;
			DBG("## Physical EP%d interrupt 0x%08x ##\n",
				   physep, event);
			DBG("[EP%d] %s\n", physep >> 1 & 0xf,
				   physep & 1 ? "IN" : "OUT");
			dwc3_handle_ep_intr(dwc3_dev, physep, event);
		}
	}

#if defined(CONFIG_IPMATE) || defined(COSIM) || defined(VIRTIO_MODEL)
	ena_eventbuf_intr(dev, 0);
#endif
	if (dev->core_params->imod) {
		clear_event_handler_busy(dev, 0);
	}

out:
	return ret;
}

/**
 * This is the common interrupt handler routine.
 *
 * @param dev   Programming view of DWC_usb3 controller.
 * @param irq   IRQ number passed in by Linux kernel.
 * @return      1 if an interrupt event was seen, 0 if not.
 */
int dwc3_irq(dwc3_core_t *dev, int irq)
{
	int state, temp, ret = 0;

	u32 gsts;

	gsts = dwc3_readl(&dev->core_global_regs->gsts);

	if (gsts & DWC_GSTS_DEV_EVT_PENDING_BIT)
		DBG("%s: gsts=0x%08x\n", __func__, gsts);

	if (!dev->cmn_irq_installed)
		return 0;

	state = dev->hibernate;

	if (state != DWC_HIBER_SLEEPING && dev->snpsid >= 0x5533230a &&
							dev->hiber_wait_u0) {
		/* Handle waiting for U0 after requesting link state RECOVERY,
		 * because we don't have the link state change event enabled.
		 * We also do this in dwc_wait_pme_thread() in case an event
		 * doesn't come.
		 */
		temp = dwc3_device_get_link_state(&dev->dwc3_dev);
		dev->dwc3_dev.link_state = temp;
		DBG("intr WAIT_U0 state=%d\n", temp);

		if (temp == DWC_LINK_STATE_U0) {
			DBG("intr WAIT_U0 setting speed\n");
			dev->dwc3_dev.speed = dwc3_get_device_speed(&dev->dwc3_dev);
			if (dev->dwc3_dev.remote_wakeup_enable)
				dwc3_device_remote_wake(&dev->dwc3_dev, 0);
			dev->hiber_wait_u0 = 0;
		}
	}

	if (state >= DWC_HIBER_SLEEPING) {
		if (state == DWC_HIBER_WAIT_U0 ||
		    state == DWC_HIBER_SS_DIS_QUIRK) {
			dev->hibernate = DWC_HIBER_AWAKE;
		} else {
			if (dev->pme_ready) {
				//ret = dwc3_handle_pme_intr(dev);
				return ret;
			} else {
				if (state != DWC_HIBER_WAIT_LINK_UP)
					INF("Intr in hibernate but"
						       " pme_ready not set\n");
				return 1;
			}
		}
	}

	ret = dwc3_handle_event(dev);
	if (ret == 2)
		ret = 1;

	gsts = dwc3_readl(&dev->core_global_regs->gsts);
	if (ret)
		VDBG("%s DONE: gsts=0x%08x\n", __func__, gsts);

	return ret;
}
