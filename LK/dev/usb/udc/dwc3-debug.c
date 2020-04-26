#include <dev/udc.h>

#include "udc-common.h"
#include "udc-core.h"
#include "udc-utils.h"
#include "dwc3.h"

#undef log_level
#define log_level 3

/**
 * This routine reads the core global registers and prints them
 *
 * @param dev   Programming view of DWC_usb3 controller.
 */
void dwc3_dump_global_registers(dwc3_core_t *dev)
{
	volatile u32 __iomem *addr;

	DBG("Core Global Registers\n");
	addr = &dev->core_global_regs->gsbuscfg0;
	DBG("GSBUSCFG0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gsbuscfg1;
	DBG("GSBUSCFG1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gtxthrcfg;
	DBG("GTXTHRCFG	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->grxthrcfg;
	DBG("GRXTHRCFG	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gctl;
	DBG("GCTL		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gevten;
	DBG("GEVTEN		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gsts;
	DBG("GSTS		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gsnpsid;
	DBG("GSNPSID	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->ggpio;
	DBG("GGPIO		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->guid;
	DBG("GUID		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->guctl;
	DBG("GUCTL		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));

	addr = &dev->core_global_regs->gdbgfifospace;
	DBG("GDBGFIFOSPACE	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gdbgltssm;
	DBG("GDBGLTSSM	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gusb2phycfg[0];
	DBG("GUSB2PHYCFG0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gusb2i2cctl[0];
	DBG("GUSB2I2CCTL0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gusb2phyacc[0];
	DBG("GUSB2PHYACC0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gusb3pipectl[0];
	DBG("GUSB3PIPECTL0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gtxfifosiz[0];
	DBG("GTXFIFOSIZ0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gtxfifosiz[1];
	DBG("GTXFIFOSIZ1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gtxfifosiz[2];
	DBG("GTXFIFOSIZ2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->gtxfifosiz[3];
	DBG("GTXFIFOSIZ3	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->grxfifosiz[0];
	DBG("GRXFIFOSIZ0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->grxfifosiz[1];
	DBG("GRXFIFOSIZ1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->grxfifosiz[2];
	DBG("GRXFIFOSIZ2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->geventbuf[0].geventadr_lo;
	DBG("GEVNTADRLO	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->geventbuf[0].geventadr_hi;
	DBG("GEVNTADRHI	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->geventbuf[0].geventsiz;
	DBG("GEVNTSIZ	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dev->core_global_regs->geventbuf[0].geventcnt;
	DBG("GEVNTCOUNT	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
}

/**
 * This routine reads the device registers and prints them
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_dump_dev_registers(dwc3_device_t *dwc3_dev)
{
	volatile u32 __iomem *addr;

	DBG("Device Global Registers\n");
	addr = &dwc3_dev->dev_regs->dcfg;
	DBG("DCFG		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dwc3_dev->dev_regs->dctl;
	DBG("DCTL		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dwc3_dev->dev_regs->devten;
	DBG("DEVTEN		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dwc3_dev->dev_regs->dsts;
	DBG("DSTS		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &dwc3_dev->dev_regs->dalepena;
	DBG("DALEPENA		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
}

/**
 * This routine reads the device registers and prints them
 *
 * @param dwc3_dev   Programming view of DWC_usb3 peripheral controller.
 */
void dwc3_dump_ep_registers(dwc3_device_t *dwc3_dev)
{
	volatile u32 __iomem *addr;
	dwc3_dev_ep_regs_t __iomem *ep_reg;

	DBG("Endpoint Registers\n");
	ep_reg = &dwc3_dev->out_ep_regs[0];
	addr = &ep_reg->depcmdpar2;
	DBG("EP0out PARA2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar1;
	DBG("EP0out PARA1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar0;
	DBG("EP0out PARA0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmd;
	DBG("EP0out CMD 	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	ep_reg = &dwc3_dev->in_ep_regs[0];
	addr = &ep_reg->depcmdpar2;
	DBG("EP0in PARA2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar1;
	DBG("EP0in PARA1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar0;
	DBG("EP0in PARA0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmd;
	DBG("EP0in CMD		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));

	ep_reg = &dwc3_dev->out_ep_regs[1];
	addr = &ep_reg->depcmdpar2;
	DBG("EP1out PARA2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar1;
	DBG("EP1out PARA1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar0;
	DBG("EP1out PARA0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmd;
	DBG("EP1out CMD 	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	ep_reg = &dwc3_dev->in_ep_regs[1];
	addr = &ep_reg->depcmdpar2;
	DBG("EP1in PARA2	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar1;
	DBG("EP1in PARA1	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmdpar0;
	DBG("EP1in PARA0	@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
	addr = &ep_reg->depcmd;
	DBG("EP1in CMD		@0x%08lx : 0x%08x\n",
		   (unsigned long)addr, readl(addr));
}

