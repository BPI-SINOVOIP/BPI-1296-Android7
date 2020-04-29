/*
 * hdmirx_repeater.c - RTK hdmi rx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "hdmirx_video_dev.h"

#include "v4l2_hdmi_dev.h"
#include "hdmirx_reg.h"
#include "mipi_wrapper.h"

#include "rx_drv/hdmiInternal.h"

#if HDMI_REPEATER_SUPPORT
void __iomem *pll_hdmi2_vaddr,*disp_pll_vaddr;
void __iomem *pll_ldo1_vaddr,*pll_ldo2_vaddr,*pll_ldo3_vaddr,*pll_ldo4_vaddr,*pll_ldo5_vaddr;
int hdmi_repeater_mode=0;// 0:regular mode, 1:repeater mode

/*=================== extern Variable/Function ===================*/
extern MIPI_TOP_INFO mipi_top;
extern HDMI_INFO_T hdmi;

extern void Hdmi_SetHPD(char high);
/*======================================================*/

void Set_Hdmitx_source(unsigned char repeater)
{
	unsigned int reg_val;

	if(repeater)
	{
		reg_val = readl(pll_ldo3_vaddr)&0xFCFFFFFF;//Bit25:Bit24
		writel(reg_val|(1<<25), pll_ldo3_vaddr);//Set TX clock input from RX

		writel((1<<26)|(1<<25), disp_pll_vaddr);//Set TX data input from RX

		hdmi_rx_reg_write32(HDMIRPT, 0x1, HDMI_RX_PHY);//Enable HDMI repeater 1.4 mode
	}
	else
	{
		writel(readl(pll_ldo3_vaddr)&0xFCFFFFFF, pll_ldo3_vaddr);//Set TX clock input from TX
		writel((1<<26), disp_pll_vaddr);//Set TX data input from TX
		hdmi_rx_reg_write32(HDMIRPT, 0x0, HDMI_RX_PHY);//Disable HDMI repeater mode
	}
}

void Set_Hdmitx_PLL(void)
{
	unsigned int tmds,reg_val;

	if(!hdmi_repeater_mode)
		return;

	tmds = hdmi.b*27/256;
	if(TMDS_CPS_get_pll_div2_en(hdmi_rx_reg_read32(TMDS_CPS,HDMI_RX_MAC)))
		tmds = tmds*2;

	HDMIRX_INFO("Set TX PLL, TMDS(%u)",tmds);

	if (tmds > 500) {
		/* 594 MHz */
		writel(0x19400488, pll_ldo1_vaddr);
		writel(0x03FC00F7, pll_ldo2_vaddr);
		writel(0x03BDF400, pll_ldo3_vaddr);
		writel(0x0A004051, pll_ldo4_vaddr);
		writel(0x0007010E, pll_ldo5_vaddr);
		writel(0x00000208, pll_hdmi2_vaddr);

	} else if (tmds > 400) {
		/* 297MHz*1.5 */
		writel(0x19400488, pll_ldo1_vaddr);
		writel(0x035400F7, pll_ldo2_vaddr);
		writel(0x03ADDC00, pll_ldo3_vaddr);
		writel(0x0A004049, pll_ldo4_vaddr);
		writel(0x0000010E, pll_ldo5_vaddr);
		writel(0x00004808, pll_hdmi2_vaddr);

	} else if (tmds > 350) {
		/* 297MHz*1.25 */
		writel(0x19400488, pll_ldo1_vaddr);
		writel(0x035400F7, pll_ldo2_vaddr);
		writel(0x03A5DC00, pll_ldo3_vaddr);
		writel(0x0A004049, pll_ldo4_vaddr);
		writel(0x0000010E, pll_ldo5_vaddr);
		writel(0x00024E08, pll_hdmi2_vaddr);

	} else if (tmds > 250) {
		/* 297 MHz */
		writel(0x00190034, pll_ldo1_vaddr);
		writel(0x03FC68FF, pll_ldo2_vaddr);
		writel(0x03BE9800, pll_ldo3_vaddr);
		writel(0x0A00C6D3, pll_ldo4_vaddr);
		writel(0x00020526, pll_ldo5_vaddr);
		writel(0x00020210, pll_hdmi2_vaddr);

	} else if (tmds > 200) {
		/* 148.5 MHz*1.5*/
		writel(0x19001005, pll_ldo1_vaddr);
		writel(0x03FC00FF, pll_ldo2_vaddr);
		writel(0x03BFDC00, pll_ldo3_vaddr);
		writel(0x0A00C042, pll_ldo4_vaddr);
		writel(0x00000100, pll_ldo5_vaddr);
		writel(0x00024700, pll_hdmi2_vaddr);

	} else if (tmds > 170) {
		/* 148.5 MHz*1.25*/
		writel(0x19001005, pll_ldo1_vaddr);
		writel(0x03FC00FF, pll_ldo2_vaddr);
		writel(0x03BFDC00, pll_ldo3_vaddr);
		writel(0x0A00C000, pll_ldo4_vaddr);
		writel(0x00000100, pll_ldo5_vaddr);
		writel(0x00044280, pll_hdmi2_vaddr);

	} else if(tmds > 140) {
		/* 148.5 MHz */
		writel(0x19001005, pll_ldo1_vaddr);
		writel(0x03FC00FF, pll_ldo2_vaddr);
		writel(0x03BED800, pll_ldo3_vaddr);
		writel(0x0A00C000, pll_ldo4_vaddr);
		writel(0x00000100, pll_ldo5_vaddr);
		writel(0x00040221, pll_hdmi2_vaddr);

	} else if (tmds > 100) {
		/* 74.25MHz*1.5 */
		writel(0x0100000F, pll_ldo1_vaddr);
		writel(0x015400F7, pll_ldo2_vaddr);
		writel(0x03A49000, pll_ldo3_vaddr);
		writel(0x00024000, pll_ldo4_vaddr);
		writel(0x00000002, pll_ldo5_vaddr);
		writel(0x00044311, pll_hdmi2_vaddr);

	} else if (tmds > 80) {
		/* 74.25MHz*1.25 */
		writel(0x0000000F, pll_ldo1_vaddr);
		writel(0x015400F0, pll_ldo2_vaddr);
		writel(0x03249000, pll_ldo3_vaddr);
		writel(0x00024092, pll_ldo4_vaddr);
		writel(0x00000002, pll_ldo5_vaddr);
		writel(0x00064291, pll_hdmi2_vaddr);

	} else if (tmds > 60) {
		/* 74.25MHz */
		writel(0x0000000F, pll_ldo1_vaddr);
		writel(0x015400F0, pll_ldo2_vaddr);
		writel(0x03249000, pll_ldo3_vaddr);
		writel(0x0000C092, pll_ldo4_vaddr);
		writel(0x00000002, pll_ldo5_vaddr);
		writel(0x00060232, pll_hdmi2_vaddr);

	} else if (tmds > 35) {
		/* 27MHz*1.5 */
		writel(0x0000000F, pll_ldo1_vaddr);
		writel(0x015400F0, pll_ldo2_vaddr);
		writel(0x03249000, pll_ldo3_vaddr);
		writel(0x0007C092, pll_ldo4_vaddr);
		writel(0x00000002, pll_ldo5_vaddr);
		writel(0x00080333, pll_hdmi2_vaddr);

	} else if (tmds > 30) {
		/* 27MHz*1.25 */
		writel(0x0000000F, pll_ldo1_vaddr);
		writel(0x015400F0, pll_ldo2_vaddr);
		writel(0x03249000, pll_ldo3_vaddr);
		writel(0x0007C092, pll_ldo4_vaddr);
		writel(0x00000002, pll_ldo5_vaddr);
		writel(0x000842A3, pll_hdmi2_vaddr);

	} else {
		/* 27MHz */
		writel(0x00000c0f, pll_ldo1_vaddr);
		writel(0x035400f0, pll_ldo2_vaddr);
		writel(0x033C7000, pll_ldo3_vaddr);
		writel(0x0000c092, pll_ldo4_vaddr);
		writel(0x00000307, pll_ldo5_vaddr);
		writel(0x00080243, pll_hdmi2_vaddr);
	} /* end of if (tmds > 500)*/

	Set_Hdmitx_source(1);
}

int Get_Hdmi_repeater_mode(void)
{
	return hdmi_repeater_mode;
}

void Set_Hdmi_repeater_mode(int enable)
{

	if(hdmi_repeater_mode == enable)
		return;

	if(enable)
	{
		HDMIRX_INFO("Enable HDMI repeater mode");
		pll_hdmi2_vaddr = ioremap(0x98000194, 0x4);
		pll_ldo1_vaddr = ioremap(0x98000230, 0x4);
		pll_ldo2_vaddr = ioremap(0x98000234, 0x4);
		pll_ldo3_vaddr = ioremap(0x98000238, 0x4);
		pll_ldo4_vaddr = ioremap(0x9800023C, 0x4);
		pll_ldo5_vaddr = ioremap(0x98000240, 0x4);
		disp_pll_vaddr = ioremap(0x98000024, 0x4);

		if(mipi_top.hdmi_rx_init)
		{
			Hdmi_SetHPD(0);
			usleep_range(500000, 600000);
			Hdmi_SetHPD(1);
		}

	}
	else
	{
		HDMIRX_INFO("Disable HDMI repeater mode");
		Set_Hdmitx_source(0);

		iounmap(pll_hdmi2_vaddr);
		iounmap(pll_ldo1_vaddr);
		iounmap(pll_ldo2_vaddr);
		iounmap(pll_ldo3_vaddr);
		iounmap(pll_ldo4_vaddr);
		iounmap(pll_ldo5_vaddr);
		iounmap(disp_pll_vaddr);
	}

	hdmi_repeater_mode = enable;
}
#endif
