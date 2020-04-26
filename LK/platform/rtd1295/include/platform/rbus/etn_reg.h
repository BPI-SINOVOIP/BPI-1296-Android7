#ifndef _Kylin_ETN_REG_H_
   #define _Kylin_ETN_REG_H_

/* ===========   ETN RTL8168_registers =====================*/
#define REG_ETN	            0x98016000
#define MAC0						(REG_ETN+0x0000)		/* Ethernet hardware address. */
#define MAC4						(REG_ETN+0x0004)		/* Ethernet hardware address. */
#define MAR0						(REG_ETN+0x0008)		/* Multicast filter. */
#define MAR4						(REG_ETN+0x000C)		/* Multicast filter. */
#define CounterAddrLow			(REG_ETN+0x0010)		/* Dump Tally Counter Command Register */
#define CounterAddrHigh			(REG_ETN+0x0014)		/* Dump Tally Counter Command Register */
#define CustomLED					(REG_ETN+0x0018)		/*Customized LED*/
#define TxDescStartAddrLow		(REG_ETN+0x0020)		/*Tx normal Priority Descriptors Start address Low*/
#define TxDescStartAddrHigh	(REG_ETN+0x0024)		/*Tx normal Priority Descriptors Start address High*/
#define TxHDescStartAddrLow	(REG_ETN+0x0028)		/*Tx High Priority Descriptors Start address Low*/
#define TxHDescStartAddrHigh	(REG_ETN+0x002C)		/*Tx High Priority Descriptors Start address High*/
#define ChipCmd					(REG_ETN+0x0036)		/*Command Register */
#define TxPoll						(REG_ETN+0x0038)		/*Tx Priority Polling register */
#define IntrMask					(REG_ETN+0x003C)		/*Interrupt Mask Register */
#define IntrStatus				(REG_ETN+0x003E)		/*Interrupt Status Register*/
#define TxConfig					(REG_ETN+0x0040)		/*Tx Configuration Register */
#define RxConfig					(REG_ETN+0x0044)		/*Rx Configuration Register */
#define TCTR						(REG_ETN+0x0048)		/*Timer counter register 0 */
#define Config12					(REG_ETN+0x0052)		/*Configuration register 12 */
#define Config34					(REG_ETN+0x0054)		/*Configuration register 34 */
#define TDFNR_Config5			(REG_ETN+0x0056)		/*Fetch Tx Descriptor Number Configuration register 5 */
#define TimeIntr					(REG_ETN+0x0058)		/*Timer Interrupt Register 0 */
#define PHYAR						(REG_ETN+0x0060)		/*PHY Access Register*/
#define PHYstatus					(REG_ETN+0x006c)		/*PHY Status Register*/
#define PMCH_Config6				(REG_ETN+0x006E)		/*Configuration register 6 */
#define ERIDR						(REG_ETN+0x0070)		/*ERI Extended Registers Interface Data Register*/
#define ERIAR						(REG_ETN+0x0074)		/*ERI Extended Registers Interface Access Register*/
#define OCPR                  (REG_ETN+0x00B0)		/*OCP Register*/
#define OOB_CTRL					(REG_ETN+0x00D2)		/*OOB control*/
#define MACDBG						(REG_ETN+0x00D4)		/*MAC debug */
#define RxMaxSize					(REG_ETN+0x00DA)		/*RX Packet Maximum Size*/
#define EFUSEAR					(REG_ETN+0x00DC)		/*eFUSE Access Register*/
#define CPlusCmd					(REG_ETN+0x00E0)		/*C+ Command Register*/
#define RxDescAddrLow			(REG_ETN+0x00E4)		/*RX Descriptor Start Address Register Low*/
#define RxDescAddrHigh			(REG_ETN+0x00E8)		/*RX Descriptor Start Address Register High*/
#define MTPS						(REG_ETN+0x00EC)		/*Max Transmit Packet Size Register*/
#define SW_RST						(REG_ETN+0x00FE)		/*SW interrupt and reset or stop MCU*/

#define ETN_LED_PAD_Ctrl      0x98007304     // ISO spec, LED_RXTX=bit[31:28], LED_Link=bit[27:24]
#define ETN_LED_PAD_MUX       0x98007310     // ISO spec, LED_RXTX=bit[29:28], LED_Link=bit[27:26]

typedef struct _nic_frame_descriptor_ {
   unsigned int StsLen;
   unsigned int VLan;
   unsigned int DataPtrLow;
   unsigned int DataPtrHigh;
}NIC_FD_T;

typedef struct _nic_frame_ {
   unsigned char data[2048];
}NIC_FRAME;

enum RTL8168_register_content {
	/* InterruptStatusBits */
	SYSErr		= 0x8000,
	PCSTimeout	= 0x4000,
	SWInt		= 0x0100,
	TxDescUnavail	= 0x0080,
	RxFIFOOver	= 0x0040,
	LinkChg		= 0x0020,
	RxDescUnavail	= 0x0010,
	TxErr		= 0x0008,
	TxOK		= 0x0004,
	RxErr		= 0x0002,
	RxOK		= 0x0001,

	/* RxStatusDesc */
	RxRWT = (1 << 22),
	RxRES = (1 << 21),
	RxRUNT = (1 << 20),
	RxCRC = (1 << 19),

	/* ChipCmdBits */
	StopReq  = (1 << 15),
	CmdReset = (1 << 12),
	CmdRxEnb = (1 << 11),
	CmdTxEnb = (1 << 10),
	RxPktftEN = (1 << 8),
//	RxBufEmpty = 0x01,	//??


	/* rx_mode_bits */
	RxEarly_128=0x00,
	RxEarly_256=0x01,
	RxEarly_512=0x02,
	RxEarly_1024=0x03,
	RxEarly_2048=0x04,

	AcceptFlow = 0x80,
	AcceptErr = 0x20,
	AcceptRunt = 0x10,
	AcceptBroadcast = 0x08,
	AcceptMulticast = 0x04,
	AcceptMyPhys = 0x02,
	AcceptAllPhys = 0x01,
	AcceptAllPackets =  0xbf,

	/* Transmit Priority Polling*/
	HPQ = 0x80,
	NPQ = 0x40,
//	FSWInt = 0x01,

	/* RxConfigBits */
//	Reserved2_shift = 13,
	RxCfgDMAShift = 8,
	rx_new_desc_format_en = (1 << 24),
	Rx_empty = (1 << 17),
	RxCfg_128_int_en = (1 << 15),
	RxCfg_single_fetch = (1 << 14),
	RxEarly_on = (1 << 11),
	RxCfg_9356SEL = (1 << 6),

   RX_DMA_BURST_unlimited = 7,
   RX_DMA_BURST_1024	     = 6,
   RX_DMA_BURST_512	     = 5,
   RX_DMA_BURST_256	     = 4,
   RX_DMA_BURST_128	     = 3,
   RX_DMA_BURST_64		  = 2,

	/* TxConfigBits */
	TxInterFrameGapShift = 24,
	TxCfgDMAShift = 8,	/* DMA burst value (0-7) is shift this many bits */
	TxMACLoopBack = (1 << 17),	/* MAC loopback */
	TxNoCRC	=	(1 << 16),
	dynamic_sharefifo_enable = (1 <<7),
   
   TX_DMA_BURST_unlimited	= 7,
   TX_DMA_BURST_1024	      = 6,
   TX_DMA_BURST_512	      = 5,
   TX_DMA_BURST_256	      = 4,
   TX_DMA_BURST_128	      = 3,
   TX_DMA_BURST_64		   = 2,
   TX_DMA_BURST_32		   = 1,
   TX_DMA_BURST_16		   = 0,
   
   InterFrameGap	         = 0x03,	/* 3 means InterFrameGap = the shortest one */

	/* Config1 register  */
	LEDS1		= (1 << 7),
	LEDS0		= (1 << 6),
	PMEnable	= (1 << 0),	/* Power Management Enable */

	/* Config3 register */
	MagicPacket	= (1 << 5),	/* Wake up when receives a Magic Packet */
	LinkUp		= (1 << 4),	/* This bit is reserved in RTL8168B.*/
					/* Wake up when the cable connection is re-established */
	ECRCEN		= (1 << 3),	/* This bit is reserved in RTL8168B*/
	Jumbo_En0	= (1 << 2),	/* This bit is reserved in RTL8168B*/
	RDY_TO_L23	= (1 << 1),	/* This bit is reserved in RTL8168B*/
	Beacon_en	= (1 << 0),	/* This bit is reserved in RTL8168B*/

	/* Config5 register */
	BWF		= (1 << 6),	/* Accept Broadcast wakeup frame */
	MWF		= (1 << 5),	/* Accept Multicast wakeup frame */
	UWF		= (1 << 4),	/* Accept Unicast wakeup frame */
	LanWake		= (1 << 1),	/* LanWake enable/disable */
//	PMEStatus	= (1 << 0),	/* PME status can be reset by PCI RST# */

	/* CPlusCmd */
	EnableBist	= (1 << 15),
	Macdbgo_oe	= (1 << 14),
	Normal_mode	= (1 << 13),
	Force_halfdup	= (1 << 12),
	Force_rxflow_en	= (1 << 11),
	Force_txflow_en	= (1 << 10),

	PktCntrDisable	= (1 << 7), //disable_imn
	RxVlan		= (1 << 6),
	RxChkSum	= (1 << 5),
	Macdbgo_sel	= 0x001C,
	INTT_0		= 0x0000,
	INTT_1		= 0x0001,

	/* rtl8168_PHYstatus */
	PowerSaveStatus = 0x80,
	TxFlowCtrl = 0x40,
	RxFlowCtrl = 0x20,
	_1000bpsF = 0x10,
	_100bps = 0x08,
	_10bps = 0x04,
	LinkStatus = 0x02,
	FullDup = 0x01,

	/* DBG_reg */
	Fix_Nak_1 = (1 << 4),
	Fix_Nak_2 = (1 << 3),
	DBGPIN_E2 = (1 << 0),

	/* DumpCounterCommand */
	CounterDump = 0x8,

	/* PHY access */
	PHYAR_Flag = 0x80000000,
	PHYAR_Write = 0x80000000,
	PHYAR_Read = 0x00000000,
	PHYAR_Reg_Mask = 0x1f,
	PHYAR_Reg_shift = 16,
	PHYAR_Data_Mask = 0xffff,
   PHYAR_Busy = (1 << 30),
   PHYAR_WR = (1 << 31),
   PHYAR_RD = (0 << 31),

	/* EPHY access */
	EPHYAR_Flag = 0x80000000,
	EPHYAR_Write = 0x80000000,
	EPHYAR_Read = 0x00000000,
	EPHYAR_Reg_Mask = 0x1f,
	EPHYAR_Reg_shift = 16,
	EPHYAR_Data_Mask = 0xffff,

	/* ERI access */
	ERIAR_Flag = 0x80000000,
	ERIAR_Write = 0x80000000,
	ERIAR_Read = 0x00000000,
	ERIAR_Addr_Align = 4, /* ERI access register address must be 4 byte alignment */
	ERIAR_ExGMAC = 0,
	ERIAR_MSIX = 1,
	ERIAR_Type_shift = 16,
	ERIAR_ByteEn = 0x0f,
	ERIAR_ByteEn_shift = 12,

	/* OCP GPHY access */
	OCPDR_Write = 0x80000000,
	OCPDR_Read = 0x00000000,
	OCPDR_Reg_Mask = 0xFF,
	OCPDR_Data_Mask = 0xFFFF,
	OCPDR_GPHY_Reg_shift = 16,
	OCPAR_Flag = 0x80000000,
	OCPAR_GPHY_Write = 0x8000F060,
	OCPAR_GPHY_Read = 0x0000F060,
	OCPR_Write = 0x80000000,
	OCPR_Read = 0x00000000,
	OCPR_Addr_Reg_shift = 16,
   OCPR_Addr_Mask = 0xFFFF,
	OCPR_Flag = 0x80000000,
	OCP_STD_PHY_BASE_PAGE = 0x0A40,

	/* MCU Command */
	Now_is_oob = (1 << 15),
	Txfifo_empty = (1 << 13),
	Rxfifo_empty = (1 << 12),

	/* GPIO */
	GPIO_en = (1 << 0),

};

enum _DescStatusBit {
	DescOwn		= (1 << 31), /* Descriptor is owned by NIC */
	RingEnd		= (1 << 30), /* End of descriptor ring */
	FirstFrag	= (1 << 29), /* First segment of a packet */
	LastFrag	= (1 << 28), /* Final segment of a packet */

	/* Tx private */
	/*------ offset 0 of tx descriptor ------*/
	LargeSend	= 	(1 << 27), /* TCP Large Send Offload (TSO) */
	GiantSendv4 = ( 1<< 26),	 /* TCP Giant Send Offload (TGO) for IPv4 packets */
	GiantSendv6 = (1 << 25),	 /* TCP Giant Send Offload (TGO) for IPv6 packets */
	MSSShift		= 18,        /* MSS value position */
	MSSMask			= 0x7ffU,    /* MSS value + LargeSend bit: 12 bits */
	TxVlanTag		= (1 << 17), /* Add VLAN tag */
   TX_DESC_DATA_LEN_MASK = 0x7ff,

//#define MSS_SHIFT	18		/* MSS value position */
//#define MSS_MASK	0x7FF		/* MSS value : 11 bits */

	TxUDPCS_C	= (1 << 31), /* Calculate UDP/IP checksum */
	TxTCPCS_C	= (1 << 30), /* Calculate TCP/IP checksum */
	TxIPCS_C	= (1 << 29), /* Calculate IP checksum */

	/* Rx private */
	/*------ offset 0 of rx descriptor ------*/
	PID1		= (1 << 18), /* Protocol ID bit 1/2 */
	PID0		= (1 << 17), /* Protocol ID bit 2/2 */

#define RxProtoUDP	(PID1)
#define RxProtoTCP	(PID0)
#define RxProtoIP	(PID1 | PID0)
#define RxProtoMask	RxProtoIP

	RxIPF		= (1 << 16), /* IP checksum failed */
	RxUDPF		= (1 << 15), /* UDP/IP checksum failed */
	RxTCPF		= (1 << 14), /* TCP/IP checksum failed */
	RxVlanTag	= (1 << 16), /* VLAN tag available */

	RxUDPT		= (1 << 18),
	RxTCPT		= (1 << 17),
	RxV6F		= (1 << 31),
	RxV4F		= (1 << 30),

};

typedef struct _Tally_Counter_{
   unsigned long long int  TxOk;
   unsigned long long int  RxOk;
   unsigned long long int  TxEr;
   unsigned int            RxEr;
   unsigned short int      MissPkt;
   unsigned short int      FAE;
   unsigned int            Tx1Col;
   unsigned int            TxMCol;
   unsigned long long int  RxOkPhy;
   unsigned long long int  RxOkBrd;
   unsigned int            RxOkMul;
   unsigned short int      TxAbt;
   unsigned short int      TxUndrn;
} type_Tally_Counter;

#endif   // _Kylin_ETN_REG_H_