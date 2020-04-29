#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>

#include <linux/interrupt.h>

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>

#include <linux/slab.h>

#include "../cec.h"
#include <soc/realtek/kernel-rpc.h>
#include "cec_rpc.h"
#include "cec_rtd1295.h"

#define CONVERT_FOR_AVCPU(x)        ((unsigned int)(x) | 0xA0000000)

//#define ORI_TX

//#ifdef ORI_TX
#define RTK_CEC_ANALOG					(0xE0)
#define CEC_ANALOG						RTK_CEC_ANALOG
#define CEC_ANALOG_REG_CEC_RPU_EN_MASK	(0x1<<3)

#define SYS_PLL_BUS1					0x164
#define SYS_PLL_BUS1_pllbus_n_shift		(25)
#define SYS_PLL_BUS1_pllbus_m_shift		(18)
#define SYS_PLL_BUS1_pllbus_n_mask		(0x06000000)
#define SYS_PLL_BUS1_pllbus_m_mask		(0x01FC0000)

#define SYS_PLL_BUS3					0x16c
#define SYS_PLL_BUS3_pllbus_o_shift		(3)
#define SYS_PLL_BUS3_pllbus_o_mask		(0x00000008)
//#else
#define CBUS_WRAPPER					(0)

#define CEC1_INT_SCPU_EN				(0x1<<17)
#define CEC1_INT_ACPU_EN				(0x1<<16)

#define CEC_PAD_EN						(0x01<<17)
#define CEC_PAD_EN_MODE					(0x01<<16)
#define CEC_HW_RETRY_EN					(0x01<<12)

#define CEC_RSEL(x)							(x & 0x1F)
#define CEC_RPU_EN							(1<<5)
#define IRQ_BY_CEC_RECEIVE_SPECIAL_CMD(x)	((x&0x1)<<8)
//#endif

static const struct of_device_id cec_ids[] __initconst = {
	{ .compatible = "Realtek,rtd1295-cec0", },
	{},
	{ .compatible = "Realtek,rtd1295-cec1", },
	{/*Sentinel*/},
};

static cec_device rtk_cec_controller[] =
{
	{	.id = 0,	.name = "cec0",	},
	{	.id = 1,	.name = "cec1",	},
};

static const int cec_divider_table[] =
{
	256000000, 255, 25,
	128000000, 200, 16,
};

extern struct ion_device *rtk_phoenix_ion_device;

struct ion_client* get_ion_client(const char *name)
{
	return ion_client_create(rtk_phoenix_ion_device, name);
}

static int clk_nb_func(struct notifier_block *nb,  unsigned long action, void *data)
{
	rtk_cec* p_this = (rtk_cec*) container_of(nb, rtk_cec, clk_nb);
	unsigned int i, reg, table_num;
	struct clk_notifier_data * d = data;

	if(action==2)
		return 0;

	reg = read_reg32(p_this->reg_base+ CEC_CR0);
	table_num = sizeof(cec_divider_table)/sizeof(int)/3;
	for(i=0; i<table_num; i++) {
		if(d->new_rate == cec_divider_table[i*3]) {
			reg &= ~0x00FFFF00;
			reg |= cec_divider_table[i*3+1] << 16 | cec_divider_table[i*3+2]<<8;
		}
	}
	write_reg32(p_this->reg_base+ CEC_CR0, reg);

	return 0;
}

/*------------------------------------------------------------------
 * Func : _read_cec_rx_fifo
 *
 * Desc : read rx fifo
 *
 * Parm : cmb : cec message buffer
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
int _read_cec_rx_fifo(void __iomem *reg_base, cm_buff* cmb)
{
	unsigned long rx_fifo_data[4];
	int len = read_reg32(reg_base + CEC_RXCR0) & 0x1F;
	int i;
	cec_rx_dbg("_read_cec_1_rx_fifo : rx data len = %d\n", len);

	if (cmb_tailroom(cmb) >= len)
	{
		while(len)
		{
			unsigned char rx_len = len;

			if (rx_len > 16)
				rx_len = 16;

			for (i=0; i<rx_len; i+=4)
				rx_fifo_data[i>>2] = __cpu_to_be32(read_reg32(reg_base + CEC_RXDR1 + i));

			cec_rx_dbg("cmb->tail=%p, rx_fifo_data=%p, rx_len=%d\n", cmb->tail, rx_fifo_data, rx_len);

			memcpy(cmb->tail, (unsigned char*)rx_fifo_data, rx_len);

			cmb_put(cmb, rx_len);

			write_reg32(reg_base + CEC_TXDR0, RX_SUB_CNT | FIFO_CNT(rx_len));  // fetch next rx data

			len -= rx_len;
		}

		return cmb->len;
	}
	else
	{
		cec_warning("no more space to read data\n");
		return -ENOMEM;
	}
}

/*------------------------------------------------------------------
 * Func : _write_cec_tx_fifo
 *
 * Desc : write data to tx fifo
 *
 * Parm : cmb
 *
 * Retn : write length
 *------------------------------------------------------------------*/
int _write_cec_tx_fifo(void __iomem *reg_base, cm_buff* cmb)
{
	int remain = 0x1F - (unsigned char)(read_reg32(reg_base + CEC_TXCR0) & 0x1F);
	int i;
	int len = cmb->len;
	unsigned int tx_fifo_data[4];

	if (len > remain)
		len = remain;

	while(len)
	{
		unsigned char tx_len = len;

		if (tx_len >16)
			tx_len = 16;

		cec_tx_dbg("in _write_cec_1_tx_fifo : tx data len = %d\n", tx_len);

		memcpy(tx_fifo_data, cmb->data, tx_len);
		cmb_pull(cmb, tx_len);

		cec_tx_dbg("tx_fifo_data=%x\n",tx_fifo_data[0]);
		for (i=0; i<tx_len; i+=4)
			write_reg32(reg_base + CEC_TXDR1 + i, __cpu_to_be32(tx_fifo_data[i>>2]));

		write_reg32(reg_base + CEC_TXDR0, TX_ADD_CNT | FIFO_CNT(tx_len));  // add data to fifo
		len -= tx_len;
	}

	return len;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_rx_reset
 *
 * Desc : reset rx
 *
 * Parm : p_this : handle of rtk cec  
 *         
 * Retn : N/A
 *------------------------------------------------------------------*/
#ifdef ORI_TX
static void rtk_cec_rx_reset(rtk_cec* p_this)
{       
	write_reg32(p_this->reg_base + CEC_RXCR0, RX_RST);
	wmb();
	write_reg32(p_this->reg_base + CEC_RXCR0, 0);        
	wmb();
	p_this->rcv.state = IDEL;
}
static void rtk_cec_tx_reset(rtk_cec* p_this)
{
	write_reg32(p_this->reg_base + CEC_TXCR0, TX_RST);
	wmb();
	write_reg32(p_this->reg_base + CEC_TXCR0, 0);
	wmb();
}
#else
static void rtk_cec_rx_reset(rtk_cec* p_this)
{
	write_reg32(p_this->reg_base + CEC_RXCR0, 0x4040);
	write_reg32(p_this->reg_base + CEC_RXCR0, 0x0);
	p_this->rcv.state = IDEL;
}
static void rtk_cec_tx_reset(rtk_cec* p_this)
{
    write_reg32(p_this->reg_base + CEC_TXCR0, 0x4040);
    write_reg32(p_this->reg_base + CEC_TXCR0, 0x0);
    p_this->xmit.state = IDEL;
}
#endif

/*------------------------------------------------------------------
 * Func : _cmb_cec_tx_complete
 *
 * Desc : complete a tx cmb
 *
 * Parm : p_this : handle of rtk cec
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
void _cmb_cec_tx_complete(cm_buff* cmb)
{
	if (cmb->flags & NONBLOCK)
	{
		kfree_cmb(cmb);
	}
	else
	{
		if (cmb->status==WAIT_XMIT)
			cmb->status = XMIT_ABORT;
		if(cmb!=NULL)
			wake_up(&cmb->wq);
		else
			cec_info(KERN_ERR "[%s] cmb free, wake fail\n", __FUNCTION__);
	}
}

/*------------------------------------------------------------------
 * Func : rtk_cec_tx_work
 *
 * Desc : rtk cec tx function
 *
 * Parm : p_this : handle of rtk cec
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
void rtk_cec_tx_work(struct work_struct* work)
{
	rtk_cec* p_this = (rtk_cec*) container_of(work, rtk_cec, xmit.work.work);
	rtk_cec_xmit* p_xmit = &p_this->xmit;
	unsigned char dest;
	unsigned long flags;

	spin_lock_irqsave(&p_this->lock, flags);

	if (p_xmit->state == XMIT)
	{
		if (read_reg32(p_this->reg_base + CEC_TXCR0) & TX_EN)
		{
			// xmitting
			if (read_reg32(p_this->reg_base + CEC_TXCR0) & TX_INT)
			{
				if (p_xmit->cmb->len)
				{
					_write_cec_tx_fifo(p_this->reg_base, p_xmit->cmb);

					if (p_xmit->cmb->len==0)
						write_reg32(p_this->reg_base + CEC_TXCR0, read_reg32(p_this->reg_base + CEC_TXCR0) &~TX_CONTINUOUS);   // clear continue bits....
				}

				write_reg32(p_this->reg_base + CEC_TXCR0, read_reg32(p_this->reg_base + CEC_TXCR0));   // clear interrupt
			}

			if (time_after(jiffies, p_xmit->timeout))
			{
				write_reg32(p_this->reg_base + CEC_TXCR0, 0);

				p_xmit->cmb->status = XMIT_TIMEOUT;
				_cmb_cec_tx_complete(p_xmit->cmb);

				p_xmit->cmb   = NULL;
				p_xmit->state = IDEL;

				cec_warning("cec tx timeout\n");

				cancel_delayed_work(&p_xmit->work);
			}
		}
		else
		{
			cancel_delayed_work(&p_xmit->work);

			if ((read_reg32(p_this->reg_base + CEC_TXCR0) & TX_EOM)==0)
			{
				p_xmit->cmb->status = XMIT_FAIL;
				cec_warning("cec tx failed\n");
			}
			else
			{
				p_xmit->cmb->status = XMIT_OK;
				cec_tx_dbg("cec tx completed\n");
			}
#ifdef ORI_TX
			write_reg32(p_this->reg_base + CEC_TXCR0, 0);
#else
			write_reg32(p_this->reg_base + CEC_TXCR0, read_reg32(p_this->reg_base + CEC_TXCR0));   // clear interrupt
#endif
			_cmb_cec_tx_complete(p_xmit->cmb);
			p_xmit->cmb   = NULL;
			p_xmit->state = IDEL;
		}
	}

	if (p_xmit->state==IDEL)
	{
		if (p_xmit->enable)
		{
			p_xmit->cmb = cmb_dequeue(&p_this->tx_queue);

			if (p_xmit->cmb)
			{
				cec_tx_dbg("cec tx : cmb = %p, len=%d\n", p_xmit->cmb, p_xmit->cmb->len);
				dest = (p_xmit->cmb->data[0] & 0xf);
				cmb_pull(p_xmit->cmb, 1);
				p_xmit->timeout = jiffies + TX_TIMEOUT;

				rtk_cec_tx_reset(p_this);
				
				_write_cec_tx_fifo(p_this->reg_base, p_xmit->cmb);

//#ifdef ORI_TX
				if (p_xmit->cmb->len==0)
					write_reg32(p_this->reg_base + CEC_TXCR0, TX_ADDR_EN | TX_ADDR(p_this->standby_logical_addr) | DEST_ADDR(dest) | TX_EN | TX_INT_EN);										
				else
					write_reg32(p_this->reg_base + CEC_TXCR0, TX_ADDR(p_this->standby_logical_addr) | DEST_ADDR(dest) | TX_EN | TX_INT_EN | TX_CONTINUOUS);
//#else
//				if (p_xmit->cmb->len==0)
//					write_reg32(p_this->reg_base + CEC_TXCR0, TX_ADDR_EN | TX_ADDR(4) | DEST_ADDR(dest) | TX_EN | TX_INT_EN);					//???
//				else
//					write_reg32(p_this->reg_base + CEC_TXCR0, TX_ADDR(4) | DEST_ADDR(dest) | TX_EN | TX_INT_EN | TX_CONTINUOUS);				//???
//#endif
				p_xmit->state = XMIT;

				cec_tx_dbg("cec txx start\n");

				schedule_delayed_work(&p_xmit->work, TX_TIMEOUT+1);      // queue delayed work for timeout detection
			}
		}
	}

	spin_unlock_irqrestore(&p_this->lock, flags);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_tx_start
 *
 * Desc : start rx
 *
 * Parm : p_this : handle of rtk cec
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
static void rtk_cec_tx_start(rtk_cec* p_this)
{
	unsigned long flags;
	spin_lock_irqsave(&p_this->lock, flags);

	if (!p_this->xmit.enable)
	{
		cec_info("cec tx start\n");
		p_this->xmit.enable = 1;
		p_this->xmit.state  = IDEL;
		p_this->xmit.cmb    = NULL;

		INIT_DELAYED_WORK(&p_this->xmit.work, rtk_cec_tx_work);
		cmb_queue_head_init(&p_this->tx_queue);
	}

	spin_unlock_irqrestore(&p_this->lock, flags);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_tx_stop
 *
 * Desc : stop tx
 *
 * Parm : p_this : handle of rtk cec
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
static void rtk_cec_tx_stop(rtk_cec* p_this)
{
//	cm_buff* cmb;
	unsigned long flags;
	spin_lock_irqsave(&p_this->lock, flags);

	if (p_this->xmit.enable)
	{
		cec_info("cec tx stop\n");
		cancel_delayed_work(&p_this->xmit.work);
		rtk_cec_tx_reset(p_this);

		if (p_this->xmit.cmb)
			_cmb_cec_tx_complete(p_this->xmit.cmb);

//		while((cmb = cmb_dequeue(&p_this->tx_queue)))
//			_cmb_cec_tx_complete(cmb);

		p_this->xmit.enable = 0;
		p_this->xmit.state  = IDEL;
		p_this->xmit.cmb    = NULL;
	}

	spin_unlock_irqrestore(&p_this->lock, flags);
}	

extern int rtk_cec_xmit_message(rtk_cec* p_this, cm_buff* cmb, unsigned long flags);

/*------------------------------------------------------------------
 * Func : rtk_cec_1_standby_message_handler
 *
 * Desc : rtk cec message handler when cec works under standby mode
 *
 * Parm : p_this : handle of rtk cec
 *        init   : message initator
 *        dest   : message destination
 *        opcode : opcode of message
 *        param  : param of messsage
 *        len    : length of message parameter
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
void rtk_cec_standby_message_handler(
	rtk_cec*             p_this,
	unsigned char           init,
	unsigned char           dest,
	unsigned char           opcode,
	unsigned char*          param,
	unsigned char           len
	)
{
	cm_buff* cmb = NULL;

	unsigned char dev_type_map[15] =
	{
		CEC_DEVICE_TV,
		CEC_DEVICE_RECORDING_DEVICE,
		CEC_DEVICE_RECORDING_DEVICE,
		CEC_DEVICE_TUNER,
		CEC_DEVICE_PLAYBACK_DEVICE,
		CEC_DEVICE_AUDIO_SYSTEM,
		CEC_DEVICE_TUNER,
		CEC_DEVICE_TUNER,
		CEC_DEVICE_PLAYBACK_DEVICE,
		CEC_DEVICE_RECORDING_DEVICE,
		CEC_DEVICE_TUNER,
		CEC_DEVICE_PLAYBACK_DEVICE,
		CEC_DEVICE_RESERVED,
		CEC_DEVICE_RESERVED,
		CEC_DEVICE_TV
	};

    //cec_info("rtk_cec_1_standby_message_handler : init = %x, dest=%x, opcode=%x, len=%d\n", init, dest, opcode, len);

	switch(opcode)
	{
	case CEC_MSG_GIVE_DEVICE_POWER_STATUS:
	
		if (p_this->standby_config & STANBY_RESPONSE_GIVE_POWER_STATUS && len==0 && init!=0xF)
		{

#ifdef CEC_OPCODE_COMPARE_ENABLE
			if(PWR_STS_COMP_EN())
				break;
#endif
			if ((cmb=alloc_cmb(3)))
			{
				cmb->data[0] = (dest<<4) | init;
				cmb->data[1] = CEC_MSG_REPORT_POWER_STATUS;
				cmb->data[2] = CEC_POWER_STATUS_STANDBY;
				cmb_put(cmb,3);
			}
		}
		break;

	case CEC_MSG_GIVE_PHYSICAL_ADDRESS:

		if (p_this->standby_config & STANBY_WAKEUP_BY_SET_STREAM_PATH && len==0 && dest !=0xF)
		{

#ifdef CEC_OPCODE_COMPARE_ENABLE
			if(PHY_ADDR_COMP_EN())
				break;
#endif
			if ((cmb=alloc_cmb(5)))
			{
				cmb->data[0] = (dest<<4) | 0xF;
				cmb->data[1] = CEC_MSG_REPORT_PHYSICAL_ADDRESS;
				cmb->data[2] = (p_this->standby_physical_addr >> 8) & 0xFF;
				cmb->data[3] = (p_this->standby_physical_addr) & 0xFF;
				cmb->data[4] = dev_type_map[dest];
				cmb_put(cmb, 5);
			}
		}
		break;

	case CEC_MSG_STANDBY:
		break;

	default:
		// send feature abort
		if (init!=0xF && dest !=0xF)
		{
			if ((cmb=alloc_cmb(4)))
			{
				cmb->data[0] = (dest<<4) | init;
				cmb->data[1] = CEC_MSG_FEATURE_ABORT;
				cmb->data[2] = opcode;        // FEATURE ABORT
				cmb->data[3] = CEC_ABORT_NOT_IN_CORECT_MODE;
				cmb_put(cmb, 4);
			}
		}
	}

	if (cmb)
		rtk_cec_xmit_message(p_this, cmb, NONBLOCK);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_rx_work
 *
 * Desc : rtk cec rx function
 *
 * Parm : work_struct : handle of work structure
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
void rtk_cec_rx_work(struct work_struct * work)
{
	rtk_cec* p_this = (rtk_cec*) container_of(work, rtk_cec, rcv.work.work);
	rtk_cec_rcv* p_rcv = &p_this->rcv;
	unsigned long flags;
	unsigned long rx_event = read_reg32(p_this->reg_base + CEC_RXCR0);

	spin_lock_irqsave(&p_this->lock, flags);

	if (!p_rcv->enable)
	{
		if (p_rcv->state==RCV)
		{
			cec_rx_dbg("force stop\n");
			write_reg32(p_this->reg_base + CEC_RXCR0, 0);
			kfree_cmb(p_rcv->cmb);
			p_rcv->cmb   = NULL;
			p_rcv->state = IDEL;
		}
#ifdef ORI_TX
#else
		rtk_cec_rx_reset(p_this);				//simon : cec tx mark this ????
#endif
		goto end_proc;
	}

	if (p_rcv->state==RCV)
	{
#ifdef CEC_OPCODE_COMPARE_ENABLE
		if ((rx_event & RX_EN) && (!IS_OPCODE_COMP_EN()))
#else
		if (rx_event & RX_EN)
#endif
		{
			if (rx_event & RX_INT)
			{
				if (_read_cec_rx_fifo(p_this->reg_base, p_rcv->cmb)<0)
				{
					cec_rx_dbg("read rx fifo failed, return to rx\n");
					write_reg32(p_this->reg_base + CEC_RXCR0, 0);
					p_rcv->state = IDEL;
				}

				write_reg32(p_this->reg_base + CEC_RXCR0, RX_INT);
			}
		}
		else
		{
			cec_rx_dbg("rx_stop (0x%08x)\n", read_reg32(p_this->reg_base + CEC_RXCR0));

			if ((rx_event & RX_EOM) && _read_cec_rx_fifo(p_this->reg_base, p_rcv->cmb))
			{
				if (rx_event & BROADCAST_ADDR)
					*cmb_push(p_rcv->cmb, 1) = (((rx_event & INIT_ADDR_MASK)>>INIT_ADDR_SHIFT)<<4) | 0xF;
				else
					*cmb_push(p_rcv->cmb, 1) = (((rx_event & INIT_ADDR_MASK)>>INIT_ADDR_SHIFT)<<4) |
											((read_reg32(p_this->reg_base + CEC_CR0) & LOGICAL_ADDR_MASK) >> LOGICAL_ADDR_SHIFT);

				if (p_this->status.standby_mode)
				{
					rtk_cec_standby_message_handler(       // process messag
						p_this,
						p_rcv->cmb->data[0] >> 4,
						p_rcv->cmb->data[0] & 0xF,
						p_rcv->cmb->data[1],
						&p_rcv->cmb->data[2],
						p_rcv->cmb->len -2);
				}
				else
				{
					cmb_queue_tail(&p_this->rx_queue, p_rcv->cmb);
					p_rcv->cmb = NULL;

					wake_up_interruptible(&p_this->rcv.wq);
				}
			}

			p_rcv->state = IDEL;
		}
	}

	if (p_rcv->state==IDEL)
	{
		if (!p_rcv->enable)
			goto end_proc;

		if (!p_rcv->cmb)
		{
			if (cmb_queue_len(&p_this->rx_free_queue))
				p_rcv->cmb = cmb_dequeue(&p_this->rx_free_queue);
			else
			{
				cec_warning("[CEC] WARNING, rx over run\n");
				p_rcv->cmb = cmb_dequeue(&p_this->rx_queue);        // reclaim form rx fifo
			}
	
			if (p_rcv->cmb==NULL)
			{
				cec_warning("[CEC] FATAL, something wrong, no rx buffer left\n");
				goto end_proc;
			}
		}

		cmb_purge(p_rcv->cmb);
		cmb_reserve(p_rcv->cmb, 1);
#ifdef ORI_TX
		write_reg32(p_this->reg_base + CEC_RXCR0, RX_RST);
		wmb();
		write_reg32(p_this->reg_base + CEC_RXCR0, 0);
		wmb();
#else
		rtk_cec_rx_reset(p_this);
#endif

		write_reg32(p_this->reg_base + CEC_RXCR0, RX_EN | RX_INT_EN);
		cec_rx_dbg("rx_restart (0x%08x)\n", read_reg32(p_this->reg_base + CEC_RXCR0));
		p_rcv->state = RCV;
		cec_rx_dbg("[1.p_rcv->state=%d]\n",p_rcv->state);
	}

end_proc:

	spin_unlock_irqrestore(&p_this->lock, flags);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_rx_start
 *
 * Desc : start rx
 *
 * Parm : p_this : handle of rtk cec  
 *         
 * Retn : N/A
 *------------------------------------------------------------------*/
static void rtk_cec_rx_start(rtk_cec* p_this)
{   
	unsigned long flags;
	spin_lock_irqsave(&p_this->lock, flags);

	if (!p_this->rcv.enable)
	{
		cec_info("rx start");

		p_this->rcv.enable = 1;
		p_this->rcv.state  = IDEL;
		p_this->rcv.cmb    = NULL;

		INIT_DELAYED_WORK(&p_this->rcv.work, rtk_cec_rx_work);

		schedule_delayed_work(&p_this->rcv.work, 0);
	}

	spin_unlock_irqrestore(&p_this->lock, flags);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_rx_stop
 *
 * Desc : stop rx
 *
 * Parm : p_this : handle of rtk cec
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
static void rtk_cec_rx_stop(rtk_cec* p_this)
{   
    cm_buff* cmb;
    unsigned long flags;    
        
    spin_lock_irqsave(&p_this->lock, flags);    
    
    if (p_this->rcv.enable)
    {           
        cec_info("rx stop");        
        
        write_reg32(p_this->reg_base + CEC_RXCR0, RX_RST);
        wmb();
        write_reg32(p_this->reg_base + CEC_RXCR0, 0);        
        wmb();
                    
        if (p_this->rcv.cmb)
        {
            cmb_purge(p_this->rcv.cmb);
            cmb_reserve(p_this->rcv.cmb, 1);
            cmb_queue_tail(&p_this->rx_free_queue, p_this->rcv.cmb);
        }    
            
        while((cmb = cmb_dequeue(&p_this->rx_queue)))
        {
            cmb_purge(cmb);
            cmb_reserve(cmb, 1);
            cmb_queue_tail(&p_this->rx_free_queue, cmb);
        }
        
        p_this->rcv.enable = 0;
        p_this->rcv.state  = IDEL;
        p_this->rcv.cmb    = NULL;
        
        wake_up_interruptible(&p_this->rcv.wq);
    }    
    spin_unlock_irqrestore(&p_this->lock, flags);
}

/*------------------------------------------------------------------
 * Func : rtk_cec_isr
 *
 * Desc : isr of rtk cec
 *
 * Parm : p_this : handle of rtk cec 
 *        dev_id : handle of the rtk_cec 
 *         
 * Retn : IRQ_NONE, IRQ_HANDLED
 *------------------------------------------------------------------*/
static 
irqreturn_t rtk_cec_isr(
    int                     this_irq, 
    void*                   dev_id    
    )
{            
	rtk_cec* p_this;// = (rtk_cec*) dev_id;
	int i, dev_num;
	irqreturn_t ret = IRQ_NONE;

	dev_num = ARRAY_SIZE(rtk_cec_controller);
	for(i=0; i<dev_num; i++) {
		if(cec_device_pthis[i]!=0) {
			p_this = (rtk_cec *)cec_device_pthis[i];

			if (read_reg32(p_this->reg_base + CEC_TXCR0) & TX_INT) {
				rtk_cec_tx_work(&p_this->xmit.work.work);
				ret = IRQ_HANDLED;
			}

			if (read_reg32(p_this->reg_base + CEC_RXCR0) & RX_INT) {
				rtk_cec_rx_work(&p_this->rcv.work.work);
				ret = IRQ_HANDLED;
			}
		}
	}
	return ret;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_init
 *
 * Desc : init a rtk cec adapter
 *
 * Parm : N/A
 *
 * Retn : handle of cec module
 *------------------------------------------------------------------*/
int rtk_cec_init(rtk_cec* p_this)
{
	cm_buff*  cmb = NULL;
	int i;
//	int cec_pre_div = 0x21;
#ifdef ORI_TX
	static void __iomem *sys_pll_bus_base;
	int fout, pllbus_n, pllbus_m, pllbus_o;
	
	sys_pll_bus_base = ioremap(0x18000000, sizeof(unsigned int)+SYS_PLL_BUS3);
	pllbus_m = ((read_reg32(sys_pll_bus_base + SYS_PLL_BUS1) & SYS_PLL_BUS1_pllbus_m_mask) >> SYS_PLL_BUS1_pllbus_m_shift);
	pllbus_n = ((read_reg32(sys_pll_bus_base + SYS_PLL_BUS1) & SYS_PLL_BUS1_pllbus_n_mask) >> SYS_PLL_BUS1_pllbus_n_shift);
	pllbus_o = ((read_reg32(sys_pll_bus_base + SYS_PLL_BUS3) & SYS_PLL_BUS3_pllbus_o_mask) >> SYS_PLL_BUS3_pllbus_o_shift);
	fout = 27000000 * (pllbus_m + 2) / (pllbus_n + 1) / (pllbus_o + 1) / 2;
#endif
	if (!p_this->status.init)
	{              
		cec_info("Open rtk CEC");
#ifdef ORI_TX
		SETBITS(p_this->reg_base, RTK_CEC_ANALOG, CEC_ANALOG_REG_CEC_RPU_EN_MASK);
#endif
//		write_reg32(p_this->iso_base + RTK_CEC_SOFT_RESET, 0xFFFFFFFF); // 0xfe007088
//		write_reg32(p_this->iso_base + RTK_CEC_CLOCK_ENABLE, 0xFFFFFFFF); // 0xfe00708C
		rtk_cec_rx_reset(p_this);
		rtk_cec_tx_reset(p_this);
		
		write_reg32(p_this->reg_base+ CEC_TXTCR1, TX_DATA_LOW(0x1A) | TX_DATA_01(0x23) | TX_DATA_HIGH(0x22));
		write_reg32(p_this->reg_base+ CEC_TXTCR0, TX_START_LOW(0x93) | TX_START_HIGH(0x20));
		
		write_reg32(p_this->reg_base+ CEC_RXTCR0, RX_START_LOW(0x8C) | RX_START_PERIOD(0xC1) |
								RX_DATA_SAMPLE(0x2A) | RX_DATA_PERIOD(0x52));

		write_reg32(p_this->reg_base+ CEC_CR0 , CEC_MODE(1) | LOGICAL_ADDR(0x4) | TIMER_DIV(25) |
								PRE_DIV(255) | UNREG_ACK_EN);
		
		write_reg32(p_this->reg_base+ CEC_RTCR0,  CEC_PAD_EN | CEC_PAD_EN_MODE |RETRY_NO(2));

		write_reg32(p_this->reg_base+ CEC_RXCR0,  RX_EN | RX_INT_EN);
#ifdef CEC_OPCODE_COMPARE_ENABLE
		write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, 0); //disable opcode compare function
#endif	

		spin_lock_init(&p_this->lock);        
		p_this->xmit.state      = IDEL;    
		p_this->xmit.cmb        = NULL;    
		p_this->xmit.timeout    = 0;        

		p_this->rcv.state       = IDEL;  
		p_this->rcv.cmb         = NULL;  
		init_waitqueue_head(&p_this->rcv.wq);                    

		cmb_queue_head_init(&p_this->tx_queue);
		cmb_queue_head_init(&p_this->rx_queue);
		cmb_queue_head_init(&p_this->rx_free_queue);

		for (i=0; i< RX_RING_LENGTH; i++)
		{
			cmb = alloc_cmb(RX_BUFFER_SIZE);
			if (cmb)
				cmb_queue_tail(&p_this->rx_free_queue, cmb);            
		}

		if (request_irq(p_this->irq_num, rtk_cec_isr, IRQF_SHARED, "RTK CEC", p_this) < 0) 
		{
			cec_warning("open rtk cec failed, unable to request irq#%d", p_this->irq_num);              			
			return -EIO;
		}
		write_reg32( p_this->wrapper_reg + CBUS_WRAPPER, (read_reg32(p_this->wrapper_reg + CBUS_WRAPPER) | CEC1_INT_SCPU_EN | CEC1_INT_ACPU_EN ));         //CEC1_INT_ACPU_EN
		write_reg32( p_this->reg_base + GDI_POWER_SAVING_MODE , IRQ_BY_CEC_RECEIVE_SPECIAL_CMD(1) | CEC_RPU_EN | CEC_RSEL(0x17));
		p_this->status.init = 1;
	}
    return 0;
}
#define AUDIO_ION_FLAG              (ION_FLAG_NONCACHED |ION_FLAG_SCPUACC | ION_FLAG_ACPUACC)
//#ifdef ORI_TX
int rtk_cec_RPC_TOAGENT_PrivateInfo(rtk_cec* p_this, char *str, unsigned long para)
{
	struct AUDIO_RPC_PRIVATEINFO_PARAMETERS *rpc = NULL;
	struct ion_handle *handle = NULL;
	struct ion_client *client = p_this->rpc_ion_client;
	unsigned int RPC_ret;
	ion_phys_addr_t dat;
	size_t len;
	int ret=-1;
	unsigned long offset;

	cec_info("pass %s =%4lx to fw",str, para);
//	handle = ion_alloc(client, 4096, 1024, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, ION_FLAG_NONCACHED |
//																			ION_FLAG_SCPUACC |
//																			ION_FLAG_ACPUACC);

    handle = ion_alloc(client, 4096, 1024, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, AUDIO_ION_FLAG);
	if(IS_ERR(handle)) {
		cec_info("%s ion_alloc fail\n", __FUNCTION__);
		goto exit;
	}
	if(ion_phys(client, handle, &dat, &len) != 0) {
		printk(KERN_ERR "%s ion_phys fail\n", __FUNCTION__);
		goto exit;
	}
	rpc = ion_map_kernel(client, handle);

	offset = get_rpc_alignment_offset(sizeof(struct AUDIO_RPC_PRIVATEINFO_PARAMETERS));
    memset(rpc, 0, sizeof(struct AUDIO_RPC_PRIVATEINFO_PARAMETERS));

	rpc-> type = htonl(ENUM_PRIVATEINFO_AUDIO_SET_CEC_PARAMETERS);
	rpc-> privateInfo[0] = htonl(p_this->standby_config);
	rpc-> privateInfo[1] = htonl(p_this->standby_logical_addr);
	rpc-> privateInfo[2] = htonl(p_this->standby_physical_addr);
	rpc-> privateInfo[3] = htonl(p_this->standby_cec_version);
	rpc-> privateInfo[4] = htonl(p_this->standby_vendor_id);
	rpc-> privateInfo[5] = htonl(p_this->standby_rx_mask);
	rpc-> privateInfo[6] = htonl(p_this->standby_cec_wakeup_off);
	
	if (send_rpc_command(RPC_AUDIO, 
		ENUM_KERNEL_RPC_PRIVATEINFO,
		CONVERT_FOR_AVCPU(dat),
		CONVERT_FOR_AVCPU(dat + offset),
		&RPC_ret)) 
	{
		cec_info("[%s %d RPC fail]\n",__FUNCTION__, __LINE__);
		goto exit;
	}
	ret = 0;
exit:
    if(handle != NULL) {
        ion_unmap_kernel(client, handle);
        ion_free(client, handle);
    }
	return -1;
}
#if 0
int rtk_cec_RPC_TOAGENT_PrivateInfo(rtk_cec* p_this, char *str, unsigned long para) 
{	
	struct AUDIO_RPC_PRIVATEINFO_PARAMETERS *rpc = NULL;
	int ret = -1;
//	unsigned long RPC_ret;		//Simon : Fix warning
	unsigned int RPC_ret;
	dma_addr_t dat;	
	void *addr = 0;

	cec_info("pass %s =%4lx to fw",str, para);

	addr = dma_alloc_coherent(NULL, 4096, &dat, GFP_KERNEL);

	if(!addr)
	{
		cec_info("[%s %d fail]\n",__FUNCTION__, __LINE__);
		goto exit;
	}
	rpc = addr;

	rpc-> type = htonl(ENUM_PRIVATEINFO_AUDIO_SET_CEC_PARAMETERS);
	rpc-> privateInfo[0] = htonl(p_this->standby_config);
	rpc-> privateInfo[1] = htonl(p_this->standby_logical_addr);
	rpc-> privateInfo[2] = htonl(p_this->standby_physical_addr);
	rpc-> privateInfo[3] = htonl(p_this->standby_cec_version);
	rpc-> privateInfo[4] = htonl(p_this->standby_vendor_id);
	rpc-> privateInfo[5] = htonl(p_this->standby_rx_mask);

#ifdef AO_ON_SCPU
	printk(KERN_ALERT "#### hack @ %s %d\n", __FUNCTION__, __LINE__);
	rtk_ao_kernel_rpc(ENUM_KERNEL_RPC_PRIVATEINFO
		, (unsigned char *)rpc, sizeof(*rpc)
		, (unsigned char *)rpc + sizeof(*rpc), sizeof(long) * 17//sizeof(AUDIO_RPC_PRIVATEINFO_RETURNVAL)
		, (unsigned char *)&RPC_ret, sizeof(RPC_ret));
#else
	if (send_rpc_command(RPC_AUDIO, 
		ENUM_KERNEL_RPC_PRIVATEINFO,
		CONVERT_FOR_AVCPU(dat),
		CONVERT_FOR_AVCPU((dat + sizeof(struct AUDIO_RPC_PRIVATEINFO_PARAMETERS))),
		&RPC_ret)) 
	{
		cec_info("[%s %d RPC fail]\n",__FUNCTION__, __LINE__);
		goto exit;
	}
#endif

	ret = 0;
exit:
	if(addr)
		dma_free_coherent(NULL, 4096, addr, dat);
	return ret;
}
#endif
//#endif
/*------------------------------------------------------------------
 * Func : rtk_cec_set_mode
 *
 * Desc : enable ces module
 *
 * Parm : p_this   : handle of rtk cec adapter
 *        mode     : CEC_MODE_OFF : disable CEC module
 *                   CEC_MODE_ON : enable CEC module
 *                   CEC_MODE_OFF : put CEC module into standby mode
 *
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
int rtk_cec_set_mode(rtk_cec* p_this, unsigned char mode)
{
	switch(mode)
	{
	case CEC_MODE_OFF:        // disable
	
		cec_info("rtk cec mode : OFF\n");
		write_reg32(p_this->reg_base + CEC_CR0, (read_reg32(p_this->reg_base + CEC_CR0) & ~CTRL_MASK1) | CEC_MODE(0) | LOGICAL_ADDR(0xF));
		p_this->status.enable = 0;
		p_this->status.standby_mode = 0;
		rtk_cec_rx_stop(p_this);
		rtk_cec_tx_stop(p_this);
	
		break;

    case CEC_MODE_ON:
		cec_info("rtk cec mode : On\n");
		write_reg32(p_this->reg_base + CEC_CR0, (read_reg32(p_this->reg_base + CEC_CR0) & ~CTRL_MASK1) | CEC_MODE(1));
#ifdef ORI_TX
#else
		write_reg32(p_this->reg_base + CEC_RXCR0, RX_EN | RX_INT_EN);		//simon : cec tx mark this ????????
#endif
#ifdef CEC_OPCODE_COMPARE_ENABLE
		write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, 0);			//disable opcode compare function
#endif
		rtk_cec_rx_start(p_this);
		rtk_cec_tx_start(p_this);
		p_this->status.enable = 1;
		p_this->status.standby_mode = 0;		

        break;

    case CEC_MODE_STANDBY:

		cec_info("rtk cec mode : Standby (la=%x)\n", p_this->standby_logical_addr);
		write_reg32(p_this->reg_base + CEC_CR0, (read_reg32(p_this->reg_base + CEC_CR0) & ~CTRL_MASK1) |
							CEC_MODE(1) |
							LOGICAL_ADDR(p_this->standby_logical_addr));
		rtk_cec_rx_start(p_this);
		rtk_cec_tx_start(p_this);
		p_this->status.enable = 1;
		p_this->status.standby_mode = 1;

        break;
    }
    return 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_enable
 *
 * Desc : enable ces module
 *
 * Parm : p_this   : handle of rtk cec adapter
 *        on_off   : 0 : disable, others enable
 *
 * Retn : 0 for success, others failed
 *------------------------------------------------------------------*/
int rtk_cec_enable(rtk_cec* p_this, unsigned char on_off)
{
    if (on_off)
    {
        rtk_cec_set_mode(p_this, CEC_MODE_ON);
    }
    else
    {
        if (p_this->standby_config==0 || p_this->standby_logical_addr>=0xF || p_this->standby_physical_addr==0xFFFF)
            rtk_cec_set_mode(p_this, CEC_MODE_OFF);
        else
            rtk_cec_set_mode(p_this, CEC_MODE_STANDBY);        // standby mode
    }

    return 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_set_rx_mask
 *
 * Desc : set rx mask rtk ces
 *
 * Parm : p_this    : handle of rtk cec adapter
 *        rx_mask   : rx mask
 *
 * Retn : 0 : success, others fail
 *------------------------------------------------------------------*/
int rtk_cec_set_rx_mask(
    rtk_cec*             p_this,
    unsigned short          rx_mask
    )
{
	unsigned char log_addr = 0xf;
	int i;

	for (i=0; i<16; i++)
	{
		if (TEST_BIT(rx_mask, i))
		{
			log_addr = i;
			break;
		}
	}

	if (rx_mask & ~(BIT_MASK(15) | BIT_MASK(log_addr))) {
		cec_info("cec : multiple address specified (%04x), only address %x,f are valid\n", rx_mask, log_addr);
	}

	write_reg32(p_this->reg_base + CEC_CR0, (read_reg32(p_this->reg_base + CEC_CR0) & ~LOGICAL_ADDR_MASK) | LOGICAL_ADDR(log_addr));

	if (log_addr != 0xF)
	{
		p_this->standby_logical_addr = log_addr;         // save the latest vaild logical address
//#ifdef ORI_TX
		rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "logical address", p_this->standby_logical_addr); 
//#endif
	}
	return 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_set_physical_addr
 *
 * Desc : set physical address of rtk ces
 *
 * Parm : p_this    : handle of rtk cec adapter
 *        phy_addr  : physical address  
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
int rtk_cec_set_physical_addr(
    rtk_cec*             p_this,
    unsigned short          phy_addr    
    )
{
	cec_info("physcial address = %04x\n", phy_addr);    

	if (phy_addr!= 0xffff)
		p_this->standby_physical_addr = phy_addr;      // we always keep latest valid physical address during standby

//#ifdef ORI_TX
	rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "physical addr",(unsigned long)phy_addr);
//#endif
	return 0;            
}    

/*------------------------------------------------------------------
 * Func : rtk_cec_get_physical_addr
 *
 * Desc : get physical address of rtk ces
 *
 * Parm : p_this    : handle of rtk cec adapter
 *        phy_addr  : physical address
 *
 * Retn : 0 : success, others fail
 *------------------------------------------------------------------*/
int rtk_cec_get_physical_addr(
    rtk_cec*             p_this
    )
{
	return p_this->standby_physical_addr;
}
/*------------------------------------------------------------------
 * Func : rtk_cec_set_cec_version
 *
 * Desc : set cec version
 *
 * Parm : p_this    : handle of rtk cec adapter
 *        version   : cec version number
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
int rtk_cec_set_cec_version(rtk_cec* p_this, unsigned char version)
{      
	p_this->standby_cec_version = version;
//#ifdef ORI_TX
	rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "cec version", (unsigned long)version);
//#endif
	return 0; 
}

/*------------------------------------------------------------------
 * Func : rtk_cec_set_device_vendor_id
 *
 * Desc : set vendor id of rtk ces
 *
 * Parm : p_this    : handle of rtk cec adapter
 *        vendor_id  : device vendor id
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
int rtk_cec_set_device_vendor_id(rtk_cec* p_this, unsigned long vendor_id)
{      
	p_this->standby_vendor_id = vendor_id;
//#ifdef ORI_TX
	rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "vendor id", vendor_id);
//#endif
	return 0;       
}

/*------------------------------------------------------------------
 * Func : rtk_cec_set_stanby_mode
 *
 * Desc : set standy mode condition of rtk ces
 *
 * Parm : p_this  : handle of rtk cec adapter
 *        mode    : cec standby mode
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
int rtk_cec_set_stanby_mode(
    rtk_cec*             p_this,
    unsigned long           mode
    )
{         
	p_this->standby_config = mode;
//#ifdef ORI_TX
	rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "standy mode", mode);
//#endif
	return 0;            
}

/*------------------------------------------------------------------
 * Func : rtk_cec_set_retry_num
 *
 * Desc : set retry times of rtk cec
 *
 * Parm : p_this  : handle of rtk cec adapter
 *        mun    : retry times
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
int rtk_cec_set_retry_num(
    rtk_cec*         	p_this,
    unsigned long        	num
    )
{         
	write_reg32(p_this->reg_base + CEC_RTCR0, ((read_reg32(p_this->reg_base + CEC_RTCR0)&~RETRY_NO_MASK)|RETRY_NO(num)));  
	return 0;            
}

/*------------------------------------------------------------------
 * Func : rtk_cec_xmit_msg
 *
 * Desc : xmit message
 *
 * Parm : p_this   : handle of rtk cec
 *        cmb      : msg to xmit
 *
 * Retn : 0 : for success, others : fail
 *------------------------------------------------------------------*/
int rtk_cec_xmit_message(rtk_cec* p_this, cm_buff* cmb, unsigned long flags)
{
    int ret = 0;
	unsigned long flag;

    if (!p_this->xmit.enable)
        return -1;

    cmb->flags  = flags;
    cmb->status = WAIT_XMIT;

    cmb_queue_tail(&p_this->tx_queue, cmb);

    schedule_delayed_work(&p_this->xmit.work, 0);

    if ((cmb->flags & NONBLOCK)==0)
    {
        wait_event_timeout(cmb->wq, cmb->status!=WAIT_XMIT, TX_TIMEOUT + 50);

        switch(cmb->status)
        {
        case XMIT_OK:
            cec_tx_dbg("cec : xmit message success\n");
            break;

        case XMIT_ABORT:
            cec_warning("cec : xmit message abort\n");
            break;

        case XMIT_FAIL:
            cec_warning("cec1 : xmit message failed\n");
            break;

        default:
        case XMIT_TIMEOUT:
        case WAIT_XMIT:
            cec_warning("cec : xmit message timeout\n");
            break;
        }

		spin_lock_irqsave(&p_this->lock, flag);
        ret = (cmb->status==XMIT_OK) ? 0 : -1;
        kfree_cmb(cmb);
		spin_unlock_irqrestore(&p_this->lock, flag);
    }

    return ret;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_read_message
 *
 * Desc : read message
 *
 * Parm : p_this : handle of cec device
 *        flags  : flag of current read operation
 * 
 * Retn : cec message
 *------------------------------------------------------------------*/
static cm_buff* rtk_cec_read_message(rtk_cec* p_this, unsigned char flags)
{                
	while(!(flags & NONBLOCK) && p_this->status.enable && !cmb_queue_len(&p_this->rx_queue))
		wait_event_interruptible(p_this->rcv.wq, !p_this->status.enable || cmb_queue_len(&p_this->rx_queue));

	if (p_this->status.enable)
	{
		cm_buff* cmb = cmb_dequeue(&p_this->rx_queue);

		if (cmb)
		{   
			cec_rx_dbg("[cec0]got msg %p\n", cmb);
			cmb_queue_tail(&p_this->rx_free_queue, alloc_cmb(RX_BUFFER_SIZE));
			return cmb;
		}        
	}

	return NULL;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_uninit
 *
 * Desc : uninit a rtk cec adapter
 *
 * Parm : N/A
 *
 * Retn : N/A
 *------------------------------------------------------------------*/
void rtk_cec_uninit(rtk_cec* p_this)
{
    cec_info("rtk cec closed\n");

    free_irq(p_this->irq_num, p_this);
    write_reg32(p_this->reg_base + CEC_CR0, read_reg32(p_this->reg_base + CEC_CR0) & ~CEC_MODE_MASK);
    write_reg32(p_this->reg_base + CEC_RXCR0, 0);
    write_reg32(p_this->reg_base + CEC_TXCR0, 0);
    rtk_cec_enable(p_this, 0);
    p_this->status.init = 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_suspend
 *
 * Desc : suspend a rtk cec adapter
 *
 * Parm : p_this : handle of rtk cec adapter
 * 
 * Retn : 0
 *------------------------------------------------------------------*/
int rtk_cec_suspend(rtk_cec* p_this)
{
	if(!p_this->status.enable) //CEC is off 
	{
		printk("[CEC0]function is off when power on, doesn't enable!!\n");
		p_this->standby_cec_wakeup_off = 1;
		rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "wakeup off", p_this->standby_cec_wakeup_off);
		return 0;
	}

	p_this->standby_cec_wakeup_off = 0;
	rtk_cec_RPC_TOAGENT_PrivateInfo(p_this, "wakeup off", p_this->standby_cec_wakeup_off);

	cec_info("rtk cec suspended (la=%x, pa=%04x, standby_mode=%ld)",
			p_this->standby_logical_addr, 
			p_this->standby_physical_addr, 
			p_this->standby_config);
	
	rtk_cec_set_mode(p_this, CEC_MODE_OFF); 

	disable_irq(p_this->irq_num);

	if (p_this->standby_config && p_this->standby_logical_addr<0xF && p_this->standby_physical_addr!=0xFFFF)
	{
		p_this->status.enable = 1;
		p_this->status.standby_mode = 1;   

		write_reg32(p_this->reg_base + CEC_CR0, (read_reg32(p_this->reg_base + CEC_CR0) & ~(LOGICAL_ADDR(0xF))) | CEC_MODE(1) | LOGICAL_ADDR(p_this->standby_logical_addr));
		write_reg32(p_this->reg_base + CEC_RXCR0,  RX_RST);
		write_reg32(p_this->reg_base + CEC_RXCR0,  0);
		write_reg32(p_this->reg_base + CEC_TXCR0,  TX_RST);
		write_reg32(p_this->reg_base + CEC_TXCR0,  0);
#ifdef CEC_OPCODE_COMPARE_ENABLE
		write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, 0); //disable

		//-- compare 1
		if(p_this->standby_config&STANBY_RESPONSE_GIVE_POWER_STATUS) 
		{
			write_reg32(p_this->reg_base + GDI_CEC_COMPARE_OPCODE_01, CEC_MSG_GIVE_DEVICE_POWER_STATUS); 
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPCODE_01, CEC_MSG_REPORT_POWER_STATUS);  	
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPERAND_NUMBER_01, 1); 								
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_01, CEC_POWER_STATUS_STANDBY);		
			write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, (read_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE)|0x01)); 
		}
		//-- compare 2
		if(p_this->standby_config&STANBY_RESPONSE_GIVE_PHYSICAL_ADDR) 
		{
			write_reg32(p_this->reg_base + GDI_CEC_COMPARE_OPCODE_02, CEC_MSG_GIVE_PHYSICAL_ADDRESS); 
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPCODE_02, CEC_MSG_REPORT_PHYSICAL_ADDRESS);	
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPERAND_NUMBER_02, 2);									
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_04,(rtk_cec_standby_physical_addr >> 8) & 0xFF);	
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_05,(rtk_cec_standby_physical_addr) & 0xFF);				
			//write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, (read_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE)|0x02)); //invalid, disable temporarily
		}
		//-- compare 3	
		if(p_this->standby_config&STANBY_RESPONSE_GET_CEC_VERISON)
		{
			write_reg32(p_this->reg_base + GDI_CEC_COMPARE_OPCODE_03, CEC_MSG_GET_CEC_VERSION); 
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPCODE_03 , CEC_MSG_CEC_VERSION);				
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPERAND_NUMBER_03, 1);									
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_07, rtk_cec_standby_cec_version);	
			write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, (read_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE)|0x04)); 
		}
		//-- compare 4
		if(p_this->standby_config&STANBY_RESPONSE_GIVE_DEVICE_VENDOR_ID) 	
		{
			write_reg32(p_this->reg_base + GDI_CEC_COMPARE_OPCODE_04, CEC_MSG_GIVE_DEVICE_VENDOR_ID); 
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPCODE_04, CEC_MSG_DEVICE_VENDOR_ID);			
			write_reg32(p_this->reg_base + GDI_CEC_SEND_OPERAND_NUMBER_04, 3);									
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_10,(rtk_cec_standby_vendor_id >> 16) & 0xFF);	
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_11,(rtk_cec_standby_vendor_id >> 8) & 0xFF);				
			write_reg32(p_this->reg_base + GDI_CEC_OPERAND_12,(rtk_cec_standby_vendor_id) & 0xFF);			
			write_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE, (read_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE)|0x08)); 
		}
		printk("CEC_COMP_ENABLE = %08x\n", read_reg32(p_this->reg_base + GDI_CEC_OPCODE_ENABLE));
#endif //ifdef CEC_OPCODE_COMPARE_ENABLE
		
		write_reg32(p_this->reg_base + CEC_RXCR0,  RX_EN | RX_INT_EN | RX_INT);    // enable RX
	//	write_reg32(p_this->reg_base + CEC_TXCR0,  TX_EN | TX_INT_EN | TX_INT);    // enable TX
		write_reg32(p_this->reg_base + GDI_POWER_SAVING_MODE , IRQ_BY_CEC_RECEIVE_SPECIAL_CMD(1) | CEC_RPU_EN | CEC_RSEL(0x17));
		printk("cec 1 : enable CEC ACPU/SCPU interrupt\n");
		write_reg32(p_this->wrapper_reg + CBUS_WRAPPER, (read_reg32(p_this->wrapper_reg + CBUS_WRAPPER) & ~CEC1_INT_SCPU_EN));
		cec_info("cec standby enabled");
    }

    return 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_resume
 *
 * Desc : suspend a rtk cec adapter
 *
 * Parm : p_this : handle of rtk cec adapter
 * 
 * Retn : 0
 *------------------------------------------------------------------*/
int rtk_cec_resume(rtk_cec* p_this)
{
	cec_info("rtk cec resume");     
	if (read_reg32(p_this->reg_base + CEC_TXCR0) & TX_INT)
		write_reg32(p_this->reg_base + CEC_TXCR0, read_reg32(p_this->reg_base + CEC_TXCR0));// clear tx interrupt
#ifdef ORI_TX
	SETBITS(p_this->reg_base, RTK_CEC_ANALOG, CEC_ANALOG_REG_CEC_RPU_EN_MASK); //0xfe0075E0: enable pull up resistor.
#endif
	write_reg32(p_this->reg_base+ CEC_TXTCR1, TX_DATA_LOW(0x1A) | TX_DATA_01(0x23) | TX_DATA_HIGH(0x22));

	write_reg32(p_this->reg_base+ CEC_TXTCR0, TX_START_LOW(0x93) | TX_START_HIGH(0x20));

	write_reg32(p_this->reg_base+ CEC_RXTCR0, RX_START_LOW(0x8C) | RX_START_PERIOD(0xC1) |
							RX_DATA_SAMPLE(0x2A) | RX_DATA_PERIOD(0x52));

	write_reg32(p_this->reg_base+ CEC_CR0 , CEC_MODE(1) | LOGICAL_ADDR(0x4) | TIMER_DIV(25) |
								PRE_DIV(255) | UNREG_ACK_EN);
	write_reg32(p_this->reg_base+ CEC_RTCR0,  CEC_PAD_EN | CEC_PAD_EN_MODE |RETRY_NO(2));

	write_reg32(p_this->reg_base+ CEC_RXCR0,  RX_EN | RX_INT_EN);

	rtk_cec_rx_reset(p_this);
	rtk_cec_tx_reset(p_this);

	if (p_this->standby_config && p_this->standby_logical_addr<0xF && p_this->standby_physical_addr!=0xFFFF)
		rtk_cec_set_mode(p_this, CEC_MODE_ON);
	else
		rtk_cec_set_mode(p_this, CEC_MODE_OFF);

	if (p_this->status.enable)
		enable_irq(p_this->irq_num);

	write_reg32(p_this->wrapper_reg + CBUS_WRAPPER, (read_reg32(p_this->wrapper_reg + CBUS_WRAPPER) | CEC1_INT_SCPU_EN | CEC1_INT_ACPU_EN));
    return 0;
}

static int ops_probe(cec_device* dev)
{
	struct device_node __maybe_unused *cec_node;
	unsigned int module_en = 0;
	rtk_cec *m_cec;
	
	struct clk * clk;
	struct reset_control *rstc;

	m_cec = kzalloc(sizeof(rtk_cec), GFP_KERNEL);
	cec_node = of_find_matching_node(NULL, &cec_ids[dev->id*2]);
	if (!cec_node)
		cec_info(KERN_ERR "[%s] %s  %d no matching node", __FILE__,__FUNCTION__,__LINE__);

	cec_device_en[dev->id] = 0;
	cec_device_pthis[dev->id] = 0;
	of_property_read_u32(cec_node, "module-enable", &(module_en));

	if(!module_en)
		return -ENODEV;

	m_cec->rpc_ion_client = ion_client_create(rtk_phoenix_ion_device, "CECDriver");	
	cec_device_en[dev->id] = 1;

	m_cec->reg_base = of_iomap(cec_node , 0);
	m_cec->iso_base = of_iomap(cec_node , 1);
	m_cec->wrapper_reg = of_iomap(cec_node , 2);

	m_cec->clk_sys = of_clk_get_by_name(cec_node, "sys");
	m_cec->clk_nb.notifier_call = clk_nb_func;
	clk_notifier_register(m_cec->clk_sys, &m_cec->clk_nb);
	clk = of_clk_get_by_name(cec_node, "cbus_osc");
	clk_prepare_enable(clk);
	clk = of_clk_get_by_name(cec_node, "cbus_sys");
	clk_prepare_enable(clk);

	rstc = of_reset_control_get(cec_node, "cbus");
	reset_control_deassert(rstc);

	if(dev->id==0) {
		m_cec->clk = of_clk_get_by_name(cec_node, "cbustx_sys");
		m_cec->rstc = of_reset_control_get(cec_node, "cbustx");
	} else {
		m_cec->clk = of_clk_get_by_name(cec_node, "cbusrx_sys");
		m_cec->rstc = of_reset_control_get(cec_node, "cbusrx");
	}
	if(m_cec->rstc!=NULL)
		reset_control_deassert(m_cec->rstc);
	if(m_cec->clk!=NULL)
		clk_prepare_enable(m_cec->clk);

	m_cec->irq_num = irq_of_parse_and_map(cec_node, 0);
	if (!m_cec->irq_num) {
		cec_info(KERN_ERR "CEC Tx: fail to parse of irq.\n");
		return -ENXIO;
	}
	cec_info("[cecrx_irq_num = %d]\n", m_cec->irq_num);
	if (rtk_cec_init(m_cec)==0) {
		cec_set_drvdata(dev, m_cec);
		cec_device_pthis[dev->id] = (int *)m_cec;
	}

    return 0;
}

static void ops_remove(cec_device* dev)
{
    rtk_cec_uninit((rtk_cec*) cec_get_drvdata(dev));
}

static int ops_enable(cec_device* dev, unsigned char on_off)
{
	rtk_cec* p_this = cec_get_drvdata(dev);
	cec_info("cec_1_ops_enable : %02x\n", on_off);
	if((on_off&0xf0) == 0)
	{
		if(on_off&0x0f) //on
		{
			p_this->standby_config			= (STANBY_RESPONSE_GIVE_POWER_STATUS |STANBY_RESPONSE_GET_CEC_VERISON);
			p_this->standby_logical_addr	= 0x0;
			p_this->standby_physical_addr	= 0x0000;
		}
		else{
			p_this->standby_config			= 0;
			p_this->standby_logical_addr	= 0xF;
			p_this->standby_physical_addr	= 0xFFFF;
		}
	}
    return rtk_cec_enable(p_this, (on_off&0x0f));
}

static int ops_set_rx_mask(cec_device* dev, unsigned short rx_mask)
{
    return rtk_cec_set_rx_mask((rtk_cec*) cec_get_drvdata(dev), rx_mask);
}

static int ops_set_physical_addr(cec_device* dev, unsigned short phy_addr)
{
	return rtk_cec_set_physical_addr((rtk_cec*) cec_get_drvdata(dev), phy_addr);
}

static int ops_get_physical_addr(cec_device* dev)
{
	return rtk_cec_get_physical_addr((rtk_cec*) cec_get_drvdata(dev));
}

static int ops_set_cev_version(cec_device* dev, unsigned char version)
{      
	return rtk_cec_set_cec_version((rtk_cec*) cec_get_drvdata(dev), version);       
}    

static int ops_set_device_vendor_id(cec_device* dev, unsigned long vendor_id)
{      
	return rtk_cec_set_device_vendor_id((rtk_cec*) cec_get_drvdata(dev), vendor_id);       
}    

static int ops_xmit(cec_device* dev, cm_buff* cmb, unsigned long flags)
{
	return rtk_cec_xmit_message((rtk_cec*) cec_get_drvdata(dev), cmb, flags);        
}

static cm_buff* ops_read(cec_device* dev, unsigned long flags)
{
	return rtk_cec_read_message((rtk_cec*) cec_get_drvdata(dev), flags);     
}

static int ops_set_stanby_mode(cec_device* dev, unsigned long flags)
{       
	return rtk_cec_set_stanby_mode((rtk_cec*) cec_get_drvdata(dev), flags);    
}

static int ops_set_retry_num(cec_device* dev, unsigned long num)
{       
	return rtk_cec_set_retry_num((rtk_cec*) cec_get_drvdata(dev), num);    
}

static int ops_suspend(cec_device* dev)
{       
	cec_info("[cec_0_ops_suspend]\n");
	return rtk_cec_suspend((rtk_cec*) cec_get_drvdata(dev));    
}

static int ops_resume(cec_device* dev)
{    
	return rtk_cec_resume((rtk_cec*) cec_get_drvdata(dev));
}

static cec_driver rtk_cec_driver = 
{    
	.name					= "cec", // "rtk_cec0"
	.probe					= ops_probe,
	.remove					= ops_remove,        
	.suspend				= ops_suspend,
	.resume					= ops_resume,
	.enable					= ops_enable,
	.xmit					= ops_xmit,
	.read					= ops_read,
	.set_rx_mask			= ops_set_rx_mask,    
	.set_physical_addr		= ops_set_physical_addr,
	.set_cec_version		= ops_set_cev_version,
	.set_device_vendor_id	= ops_set_device_vendor_id,
	.set_stanby_mode		= ops_set_stanby_mode,
	.set_retry_num			= ops_set_retry_num,
	.get_physical_addr              = ops_get_physical_addr,
};

/*------------------------------------------------------------------
 * Func : rtk_cec_module_init
 *
 * Desc : rtk cec module init function
 *
 * Parm : N/A
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
static int __init rtk_cec_module_init(void)
{
	int i, dev_num;
	
	if (register_cec_driver(&rtk_cec_driver)!=0)
		return -EFAULT;

	dev_num = ARRAY_SIZE(rtk_cec_controller);
	if(dev_num>0) {
		for(i=0; i<dev_num; i++)
			register_cec_device(&rtk_cec_controller[i]);
	}

	return 0;
}

/*------------------------------------------------------------------
 * Func : rtk_cec_module_exit
 *
 * Desc : rtk cec module exit function
 *
 * Parm : N/A
 *         
 * Retn : 0 : success, others fail  
 *------------------------------------------------------------------*/
static void __exit rtk_cec_module_exit(void)
{
	unregister_cec_driver(&rtk_cec_driver);
}

module_init(rtk_cec_module_init);
module_exit(rtk_cec_module_exit);
