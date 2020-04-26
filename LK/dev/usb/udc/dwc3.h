#ifndef __DWC3_H
#define __DWC3_H

#include "dwc3-hw.h"
#include "dwc3-list.h"
#include "dwc3-defs.h"
#include "dwc3-usb.h"

struct dwc3_core;
struct usb2_phy;
struct usb3_phy;

typedef struct ss_endpoint_companion_descriptor ss_endpoint_companion_descriptor_t;

typedef udc_endpoint_t usb_endpoint_t;
typedef udc_request_t usb_request_t;

/**
 * These parameters may be specified when loading the module. They define how
 * the DWC_usb3 controller should be configured. The parameter values are passed
 * to the CIL initialization routine dwc3_core_init().
 */
typedef struct dwc3_core_params {
	int burst;
	int newcore;
	int phy;
	int wakeup;
	int pwrctl;
	int lpmctl;
	int phyctl;
	int usb2mode;
	int usb3gen1;
	int hibernate;
	int hiberdisc;
	int clkgatingen;
	int ssdisquirk;
	int nobos;
	int loop;
	int nump;
	int newcsr;
	int rxfsz;
	int txfsz[16];
	int txfsz_cnt;
	int baseline_besl;
	int deep_besl;
	int besl;
	int ebc;
	int nyet;
	int rx_fifo_isoc_out;
	int imod;
} dwc3_core_params_t;

/** Maximum number of Tx FIFOs. Depends on the RTL configuration. No way to
 * probe the value at runtime
 */
#define DWC_MAX_TX_FIFOS	16

/** Maximum number of physical EPs. Depends on the RTL configuration. No way to
 * probe the value at runtime
 */
#define DWC_MAX_PHYS_EP		32

/** Maximum number of data buffers per TRB. OS/application specific */
#define DWC_MAX_DATA_BUFS	13

/** Maximum number of EPs, defined by USB spec */
#define DWC_MAX_EPS		16

/** Maxpacket size for EP0, defined by USB3 spec */
#define DWC_MAX_EP0_SIZE	512

/** Maxpacket size for any EP, defined by USB3 spec */
#define DWC_MAX_PACKET_SIZE	1024

/**
 * States of EP0
 */
typedef enum ep0_state {
	EP0_IDLE,
	EP0_IN_DATA_PHASE,
	EP0_OUT_DATA_PHASE,
	EP0_IN_WAIT_GADGET,
	EP0_OUT_WAIT_GADGET,
	EP0_IN_WAIT_NRDY,
	EP0_OUT_WAIT_NRDY,
	EP0_IN_STATUS_PHASE,
	EP0_OUT_STATUS_PHASE,
	EP0_STALL,
} ep0state_e;

typedef enum dwc3_device_state {
	DWC_STATE_UNCONNECTED,	/* no host */
	DWC_STATE_DEFAULT,
	DWC_STATE_ADDRESSED,
	DWC_STATE_CONFIGURED,
} dwc3_device_state_e;

/**
 * The <code>dwc_req</code> structure represents the state of a single
 * transfer request when acting in device mode. It contains the data items
 * needed for a request to be started and completed.
 */
typedef struct dwc_req {
	dwc3_dma_desc_t *trb;	/**< TRB or TRB chain for this req */
	dwc_dma_t trbdma;		/**< DMA address of TRB or TRB chain */
	u32 length;			/**< total length of data bufs */
	u32 actual;			/**< actual amt of data transferred */
	u32 stream;			/**< stream # of this request */

	int flags;	/**< request flags - bits 8-31 are OS-specific */
#define DWC_PCD_REQ_ZERO	0x001
#define DWC_PCD_REQ_STARTED	0x002
#define DWC_PCD_REQ_MAP_DMA	0x100
#define DWC_PCD_REQ_IN		0x200

	int numbuf;				/**< number of data bufs */
	char *buf[DWC_MAX_DATA_BUFS];		/**< data buffers */
	dwc_dma_t bufdma[DWC_MAX_DATA_BUFS];	/**< DMA addrs of data bufs */
	u32 buflen[DWC_MAX_DATA_BUFS];		/**< length of data bufs */
} dwc_req_t;

/** DWC_usb3 request structure.
 * This structure is used to form a list of requests.
 */
typedef struct dwc3_device_req {
	/** DWC_usb3 request data */
	dwc_req_t dwc_req;

	DWC_CIRCLEQ_ENTRY(dwc3_device_req) entry;

	/** USB request */
	usb_request_t usb_req;
} dwc3_device_req_t;

/**
 * The <code>dwc_ep</code> structure represents the state of a single EP
 * when acting in device mode. It contains the data items needed for an EP
 * to be activated and transfer packets.
 */
typedef struct dwc_ep {
	/** Pointer to dwc3_device */
	struct dwc3_device *dwc3_dev;

	/** Pointer to OUT EP register */
	dwc3_dev_ep_regs_t __iomem *out_ep_reg;

	/** Pointer to IN EP register */
	dwc3_dev_ep_regs_t __iomem *in_ep_reg;

	/** Physical EP number */
	u8 phys;

	/** USB EP number */
	u8 num;

	/** EP type: 0 - Control, 1 - ISOC, 2 - BULK, 3 - INTR */
	u8 type;

	/** 'bInterval' value for Isoc EPs */
	u8 intvl;

	/** Max Packet bytes */
	u16 maxpacket;

	/** 'mult' value for SS Isoc EPs */
	u8 mult;

	/** Max burst size for SS EPs (0 - 15, actual burst is 1 - 16) */
	u8 maxburst;

	/** Number of streams for SS Bulk EPs (0 - 16, actual number is 2^n) */
	u8 num_streams;

	/** Tx FIFO # for IN EPs */
	u8 tx_fifo_num;

	/** Transfer Resource Index from the Start Transfer command */
	u8 tri_out;
	u8 tri_in;

	/** Status of the queue */
	unsigned int stopped		: 1;
	unsigned int disabling		: 1;
	unsigned int queue_sof		: 1;

	/** Send ZLP */
	unsigned int send_zlp		: 1;

	/** Stall clear flag */
	unsigned int stall_clear_flag	: 1;

	/** True if 3-stage control transfer */
	unsigned int three_stage	: 1;

	/** True if transfer has been started on EP */
	unsigned int xfer_started	: 1;

	/** EP direction 0 = OUT */
	unsigned int is_in		: 1;

	/** EP active */
	unsigned int active		: 1;

	/** True if TRB array has a link TRB at the end */
	unsigned int desc_link		: 1;

	/** DATA start PID for INTR and BULK EP */
	unsigned int data_pid_start	: 1;

	/** EP has been enabled for this configuration */
	unsigned int ena_once		: 1;

	/** EP was in stalled state when entering hibernation */
	unsigned int stalled_save	: 1;

	/** Saved parameters from the last DEPCFG for this EP. Used when
	 * resetting the sequence number
	 */
	u32 param0out;
	u32 param1out;
	u32 param0in;
	u32 param1in;

	/** EP state, saved across core hibernation */
	u32 save_state;

	/** Pointer to USB EP descriptor */
	const usb_endpoint_descriptor_t *usb_ep_desc;

	/** Array of DMA descriptors (TRBs) for this EP */
	dwc3_dma_desc_t *dma_desc;
	dwc_dma_t dma_desc_dma;
	int desc_size;
	int num_desc;

	/** Number of DMA descriptors available */
	int desc_avail;

	/** Index to next free DMA descriptor in array */
	int desc_idx;

	/** Index to DMA descriptor that was active before hibernation */
	int hiber_desc_idx;

	/** Condition variable for EPCMD_CMPL interrupt */
	u32 condition;

	/** Queue of dwc3_device_reqs */
	DWC_CIRCLEQ_HEAD(circleq, dwc3_device_req) queue;
} dwc_ep_t;

/** PCD EP structure.
 * This structure describes an EP, there is an array of EP pointers in the
 * PCD structure.
 */
typedef struct dwc3_device_ep {
	/** DWC_usb3 EP data */
	dwc_ep_t dwc_ep;

	/** USB EP */
	udc_endpoint_t usb_ep;
} dwc3_device_ep_t;

/** dwc3_device EP accessor functions */
#define dwc3_device_ep_to_dwc3_device(ep)	((ep)->dwc_ep.dwc3_device)
#define dwc3_device_ep_num(ep)	((ep)->dwc_ep.num)
#define dwc3_device_ep_type(ep)	((ep)->dwc_ep.type)
#define dwc3_device_ep_is_in(ep)	((ep)->dwc_ep.is_in)


/** DWC3 Device Structure.
 * This structure encapsulates the data for the dwc_usb3 PCD.
 */
typedef struct dwc3_device {
	struct dwc3_core *dwc3;

	/** USB3 link state */
	int link_state;

	/** State of the device */
	dwc3_device_state_e state;

	/** State of EP0 */
	ep0state_e ep0state;

	/** EP0 Status Request is pending */
	unsigned int ep0_status_pending		: 1;

	/** Indicates when SET CONFIGURATION Request is in process */
	unsigned int request_config		: 1;

	/** State of the Remote Wakeup Enable */
	unsigned int remote_wakeup_enable	: 1;

	/** State of the Latency Tolerance Messaging Enable */
	unsigned int ltm_enable			: 1;

	/** True if we should send an LPM notification after the status stage */
	unsigned int send_lpm			: 1;

	/** True if ready for remote wakeup request from user */
	unsigned int wkup_rdy			: 1;

	/** True if we have enabled some EPs */
	unsigned int eps_enabled		: 1;

	/** True if UTE has made some config changes */
	unsigned int ute_change			: 1;

	/** EP0 */
	dwc3_device_ep_t *ep0;

	/** Array of OUT EPs (not including EP0) */
	dwc3_device_ep_t *out_ep[DWC_MAX_EPS - 1];

	/** Array of IN EPs (not including EP0) */
	dwc3_device_ep_t *in_ep[DWC_MAX_EPS - 1];

	/** Pointer to device Global registers.
	 * Device Global Registers starting at offset 700h
	 */
	dwc3_device_regs_t *dev_regs;

	/** Device Logical OUT EP-Specific Registers 800h-9ECh */
	dwc3_dev_ep_regs_t *out_ep_regs;

	/** Device Logical IN EP-Specific Registers 810h-9FCh */
	dwc3_dev_ep_regs_t *in_ep_regs;

	/** EP0 state, saved across core hibernation */
	u32 ep0_out_save_state;
	u32 ep0_in_save_state;

	/** 'dummy' request, for EP0 only */
	dwc3_device_req_t *ep0_req;

	/** Thresholding enable flags and length variables */
	u16 rx_thr_en;
	u16 iso_tx_thr_en;
	u16 non_iso_tx_thr_en;
	u16 rx_thr_length;
	u16 tx_thr_length;

	/** Device configuration information */
	u8 capable; /**< USB_SPEED_HIGH or USB_SPEED_SUPER or USB_SPEED_SUPER_PLUS */
	u8 speed;	/**< Device Speed - 0:Unk 1:LS 2:FS 3:HS 4:Var 5:SS */
	u8 num_out_eps;	/**< Number # of Rx EP range: 0-15 except ep0 */
	u8 num_in_eps;	/**< Number # of Tx EP range: 0-15 except ep0 */

	/** The TEST mode to enter when test_mode_tasklet is executed */
	u8 test_mode;

	/** Count of pending Requests */
	unsigned request_pending;

	/** The spinlock for the PCD */
	spin_lock_t lock;

	/**
	 * Pointers to the DMA Descriptors for EP0 Control transfers
	 * (virtual and physical)
	 */

	/** Descriptor for SETUP packets */
	dwc3_dma_desc_t *ep0_setup_desc;
	dwc_dma_t ep0_setup_desc_dma;

	/** Descriptor for Data Out or Status Out phases */
	dwc3_dma_desc_t *ep0_out_desc;
	dwc_dma_t ep0_out_desc_dma;

	/** Descriptor for Data In or Status In phases */
	dwc3_dma_desc_t *ep0_in_desc;
	dwc_dma_t ep0_in_desc_dma;

	/** Data packet buffer used to return data for GET_STATUS and
	 *  GET_DESCRIPTOR(BOS) up to 512 bytes in length
	 */
	u8 *ep0_status_buf;
	dwc_dma_t ep0_status_buf_dma;
#define DWC_STATUS_BUF_SIZE 512

	/** SETUP packet buffer for EP0 */
	union dwc_setup_pkt {
		usb_device_request_t req;
		u32 d32[2];
		char d8[8];
	} *ep0_setup_pkt;
	dwc_dma_t ep0_setup_pkt_dma;

} dwc3_device_t;

/** dwc3_device accessor functions */
#define dwc3_ep0_setup_desc(pcd)	({ \
	dwc3_inval_cache((uintptr_t)pcd->ep0_setup_desc, sizeof(dwc3_dma_desc_t)); \
	(pcd)->ep0_setup_desc; \
	})
#define dwc3_ep0_setup_desc_dma(pcd)	(pcd)->ep0_setup_desc_dma
#define dwc3_ep0_out_desc(pcd)		({ \
	dwc3_inval_cache((uintptr_t)pcd->ep0_out_desc, sizeof(dwc3_dma_desc_t)); \
	(pcd)->ep0_out_desc; \
	})
#define dwc3_ep0_out_desc_dma(pcd)		(pcd)->ep0_out_desc_dma
#define dwc3_ep0_in_desc(pcd)		({ \
	dwc3_inval_cache((uintptr_t)pcd->ep0_in_desc, sizeof(dwc3_dma_desc_t)); \
	(pcd)->ep0_in_desc; \
	})
#define dwc3_ep0_in_desc_dma(pcd)		(pcd)->ep0_in_desc_dma
#define dwc3_ep0_setup_pkt(pcd)		({ \
	dwc3_inval_cache((uintptr_t)pcd->ep0_setup_pkt, 8); \
	(pcd)->ep0_setup_pkt; \
	})
#define dwc3_ep0_setup_pkt_dma(pcd)		(pcd)->ep0_setup_pkt_dma


typedef struct dwc3_core {
	/** Base address returned from ioremap() */
	u32 base;

	/** Count of threads inside Gadget API */
	int hiber_cnt;

	/** Hibernation state */
	int hibernate;

#define DWC_HIBER_AWAKE		0
#define DWC_HIBER_ENTER_NOSAVE	1
#define DWC_HIBER_ENTER_SAVE	2
#define DWC_HIBER_SLEEPING	3
#define DWC_HIBER_WAIT_LINK_UP	4
#define DWC_HIBER_WAIT_U0	5
#define DWC_HIBER_SS_DIS_QUIRK	6

	int pme_ready;

	struct dwc3_device dwc3_dev;

	struct usb2_phy *usb2phy;
	struct usb3_phy *usb3phy;

	/** Value from SNPSID register */
	u32 snpsid;

	/** Parameters that define how the core should be configured */
	dwc3_core_params_t *core_params;

	/** Core Global registers starting at offset 100h */
	dwc3_core_global_regs_t *core_global_regs;

#define DWC_EVENT_BUF_SIZE	256	// size in dwords
#define DWC_NUM_EVENT_BUFS	1
	/** Event Buffers for receiving interrupts. Up to 32 buffers are
	 * supported by the hardware, but we only use 1.
	 */
	u32 *event_ptr[DWC_NUM_EVENT_BUFS];
	u32 *event_buf[DWC_NUM_EVENT_BUFS];
	dwc_dma_t event_buf_dma[DWC_NUM_EVENT_BUFS];

	/** Total RAM for FIFOs (Bytes) */
	u16 total_fifo_size;

	/** Size of Rx FIFO (Bytes) */
	u16 rx_fifo_size;

	/** Hardware Configuration - stored here for convenience */
	u32 hwparams0;
	u32 hwparams1;
	u32 hwparams2;
	u32 hwparams3;
	u32 hwparams4;
	u32 hwparams5;
	u32 hwparams6;
	u32 hwparams7;
	u32 hwparams8;

	/** Register state, saved across core hibernation */
	u32 dcfg_save;
	u32 dctl_save;
	u32 gtxfifosiz0_save;
	u32 gtxfifosiz1_save;
	u32 gtxfifosiz2_save;
	u32 gtxfifosiz3_save;
	u32 grxfifosiz0_save;
	u32 guctl_save;
	u32 guctl1_save;

	/** Hooks for customizing device initialization. See
	 *  dwc3_core_device_init() in cil.c to see how these work.
	 */
	void (*soft_reset)(struct dwc3_core *dev, int softrst, int rstor);
	void (*phy_config)(struct dwc3_core *dev, int softrst, int rstor);
	void (*fifo_sizing)(struct dwc3_core *dev, int softrst, int rstor);
	void (*gctl_init)(struct dwc3_core *dev, int softrst, int rstor);
	void (*set_address)(struct dwc3_core *dev, int softrst, int rstor);
	void (*ep0_start)(struct dwc3_core *dev, int softrst, int rstor);

	/** Value to write into the DCTL HIRD_Thresh field on register
	 * initialization. If 0 then a default value of 0x1c will be used.
	 */
	u32 hird_thresh;

	/** Values to write into GSBUSCFG0 and GSBUSCFG1 on initialization or
	 * when exiting from hibernation. 'program_gsbuscfg' below must also be
	 * set to 1 to enable the writing of these values.
	 */
	u32 gsbuscfg0;
	u32 gsbuscfg1;

	/** True if common functionality has been initialized */
	unsigned int cmn_initialized		: 1;

	/** True if Gadget has been initialized */
	unsigned int gadget_initialized		: 1;

	/** True if PCD has been initialized */
	unsigned int pcd_initialized		: 1;

	/** True if common IRQ handler has been installed */
	unsigned int cmn_irq_installed		: 1;

	/** True if sysfs functions have been installed */
	unsigned int sysfs_initialized		: 1;

	/** True if waiting for connect before resuming from hibernation */
	unsigned int hiber_wait_connect		: 1;

	/** True if waiting for U0 state before sending remote wake */
	unsigned int hiber_wait_u0		: 1;

	/** True if GBUSCFG0/GBUSCFG1 should be written with the above
	 * values when exiting hibernation */
	unsigned int program_gsbuscfg		: 1;
} dwc3_core_t;

/** Wrapper routine for _handshake() */
#define handshake(_dev_, _ptr_, _mask_, _done_) \
	_handshake(_dev_, _ptr_, _mask_, _done_)
/** Takes a usb req pointer and returns the associated pcd req pointer */
#define dwc3_get_device_req(usbreq) 			\
	((dwc3_device_req_t *)((char *)(usbreq) - 	\
		offsetof(struct dwc3_device_req, usb_req)))

/** Takes a usb ep pointer and returns the associated pcd ep pointer */
#define dwc3_get_device_ep(usbep) 				\
	((dwc3_device_ep_t *)((char *)(usbep) - 	\
		offsetof(struct dwc3_device_ep, usb_ep)))

#define dwc3_is_hwo(desc)	({ \
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t)); \
	((desc)->control & DWC_DSCCTL_HWO_BIT); \
	})
#define dwc3_is_ioc(desc)	({ \
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t)); \
	((desc)->control & DWC_DSCCTL_IOC_BIT); \
	})
#define dwc3_get_xfercnt(desc) ({				\
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t)); \
	((desc)->status >> DWC_DSCSTS_XFRCNT_SHIFT & 		\
	 DWC_DSCSTS_XFRCNT_BITS >> DWC_DSCSTS_XFRCNT_SHIFT); \
	})
#define dwc3_get_xfersts(desc) ({				\
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t)); \
	((desc)->status >> DWC_DSCSTS_TRBRSP_SHIFT & 		\
	 DWC_DSCSTS_TRBRSP_BITS >> DWC_DSCSTS_TRBRSP_SHIFT); \
	})

#define dwc3_get_xfersofn(desc) ({			\
	dwc3_inval_cache((uintptr_t)desc, sizeof(dwc3_dma_desc_t)); \
	((desc)->control >> DWC_DSCCTL_STRMID_SOFN_SHIFT & 	\
	 DWC_DSCCTL_STRMID_SOFN_BITS >> DWC_DSCCTL_STRMID_SOFN_SHIFT); \
	})

#define dwc3_get_eventsofn(event) 			\
	((event) >> DWC_DEPEVT_ISOC_UFRAME_NUM_SHIFT & 		\
	 DWC_DEPEVT_ISOC_UFRAME_NUM_BITS >> DWC_DEPEVT_ISOC_UFRAME_NUM_SHIFT)


/* dwc3-core */
int dwc3_device_get_link_state(dwc3_device_t *dwc3_dev);

int dwc3_get_frame(dwc3_device_t *dwc3_dev);

int dwc3_get_device_speed(dwc3_device_t *dwc3_dev);

void dwc3_device_remote_wake(dwc3_device_t *dwc3_dev, int function);

void dwc3_fill_desc(dwc3_dma_desc_t *desc, dwc_dma_t dma_addr,
			u32 dma_len, u32 stream, u32 type,
			u32 ctrlbits, int own);

int dwc3_enable_ep(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep);

int dwc3_disable_ep(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *ep);

int dwc3_dep_cstall(dwc3_device_t *dwc3_dev,
			dwc3_dev_ep_regs_t __iomem *ep_reg, int clr_pend);

int dwc3_dep_endxfer(dwc3_device_t *dwc3_dev,
			 dwc3_dev_ep_regs_t __iomem *ep_reg,
			 u32 tri, int flags, void *condition);

int dwc3_dep_startxfer(dwc3_device_t *dwc3_dev,
			   dwc3_dev_ep_regs_t __iomem *ep_reg,
			   dwc_dma_t dma_addr, u32 stream_or_uf);

int dwc3_dep_updatexfer(dwc3_device_t *dwc3_dev,
			    dwc3_dev_ep_regs_t __iomem *ep_reg,
			    u32 tri);

int dwc3_dep_startnewcfg(dwc3_device_t *dwc3_dev,
			     dwc3_dev_ep_regs_t __iomem *ep_reg,
			     u32 rsrcidx);

int dwc3_dep_cfg(dwc3_device_t *dwc3_dev,
		     dwc3_dev_ep_regs_t *ep_reg,
		     u32 depcfg0, u32 depcfg1, u32 depcfg2);

int dwc3_dep_xfercfg(dwc3_device_t *dwc3_dev,
			 dwc3_dev_ep_regs_t *ep_reg,
			 u32 depstrmcfg);

int dwc3_dep_sstall(dwc3_device_t *dwc3_dev,
			dwc3_dev_ep_regs_t __iomem *ep_reg);

void dwc3_enable_desc(dwc3_dma_desc_t *desc);
void dwc3_disable_desc(dwc3_dma_desc_t *desc);

void dwc3_ena_usb2_suspend(dwc3_device_t *dwc3_dev);
void dwc3_dis_usb2_suspend(dwc3_device_t *dwc3_dev);

int dwc3_u1_enabled(dwc3_device_t *dwc3_dev);
int dwc3_u2_enabled(dwc3_device_t *dwc3_dev);
void dwc3_disable_u1(dwc3_device_t *dwc3_dev);
void dwc3_disable_u2(dwc3_device_t *dwc3_dev);
void dwc3_accept_u1(dwc3_device_t *dwc3_dev);
void dwc3_accept_u2(dwc3_device_t *dwc3_dev);
void dwc3_enable_u1(dwc3_device_t *dwc3_dev);
void dwc3_enable_u2(dwc3_device_t *dwc3_dev);

void dwc3_clr_eps_enabled(dwc3_device_t *dwc3_dev);

void dwc3_set_address(dwc3_device_t *dwc3_dev, int addr);

int dwc3_core_init(dwc3_core_t *dwc3);

void dwc3_core_device_init(dwc3_core_t *dev, int soft_reset,
			      int restore);

void dwc3_core_device_start(dwc3_core_t *dev, int restore);

void dwc3_core_device_remove(dwc3_core_t *dev);

#define DWC_ENDXFER_FORCE 1
#define DWC_ENDXFER_NODELAY 2

/* dwc3-core-intr */
int dwc3_irq(dwc3_core_t *dev, int irq);

void dwc3_init_eventbuf(dwc3_core_t *dev, int bufno,
			    u32 *addr, int size, dwc_dma_t dma_addr);

void dwc3_enable_device_interrupts(dwc3_core_t *dev);

/* dwc3-device */
dwc3_dma_desc_t *dwc3_device_trb_alloc(dwc3_device_ep_t *ep,
		int num_trbs, int trb_type, int iso_intvl, int link,
		dwc_dma_t *trbs_dma_ret);
void dwc3_device_trb_free(dwc3_device_ep_t *ep /*, int num_trbs, int link,
				  void *trbs, dwc_dma_t trbs_dma*/);
void dwc3_device_fill_trbs(dwc3_device_t *pcd, dwc3_device_ep_t *ep,
				   dwc3_device_req_t *req);

void dwc3_ep0_activate(dwc3_device_t *pcd, int restore);
void dwc3_device_do_setup(dwc3_device_t *pcd);
void dwc3_device_ep0_out_start(dwc3_device_t *pcd);
void dwc3_device_ep0_start_transfer(dwc3_device_t *pcd,
					    dwc3_device_req_t *req);
void dwc3_device_ep0_continue_transfer(dwc3_device_t *pcd,
					       dwc3_device_req_t *req);
void dwc3_device_ep0_data_stage(dwc3_device_t *pcd, int length);

int dwc3_device_ep_submit_req(dwc3_device_t *pcd,
				      dwc3_device_ep_t *ep,
				      dwc3_device_req_t *req, int req_flags);
void dwc3_device_ep_cancel_req(dwc3_device_t *pcd,
				       dwc3_device_ep_t *ep,
				       dwc3_device_req_t *req, u32 stream);
void dwc3_device_request_done(dwc3_device_t *pcd,
				      dwc3_device_ep_t *ep,
				      dwc3_device_req_t *req, int status);
void dwc3_device_ep_start_transfer(dwc3_device_t *pcd,
					   dwc3_device_ep_t *ep,
					   dwc3_device_req_t *req, u32 event);
void dwc3_device_ep_set_stall(dwc3_device_t *pcd,
				      dwc3_device_ep_t *ep);
void dwc3_device_ep_clear_stall(dwc3_device_t *pcd,
					dwc3_device_ep_t *ep);
void dwc3_device_ep_set_halt(dwc3_device_t *pcd, dwc3_device_ep_t *ep,
				     int value);

void dwc3_ep_activate(dwc3_device_t *pcd, dwc3_device_ep_t *ep,
				 int restore);
void dwc3_stop_all_xfers(dwc3_device_t *pcd);
void dwc3_complete_request(dwc3_device_t *pcd,
				      dwc3_device_ep_t *ep, u32 event);
dwc3_device_ep_t *dwc_usb3_get_out_ep(dwc3_device_t *pcd, u32 ep_num);
dwc3_device_ep_t *dwc_usb3_get_in_ep(dwc3_device_t *pcd, u32 ep_num);

int dwc3_device_ep_enable(dwc3_device_t *pcd, dwc3_device_ep_t *ep,
			const usb_endpoint_descriptor_t *ep_desc,
			const ss_endpoint_companion_descriptor_t *ep_comp);
int dwc3_device_ep_disable(dwc3_device_t *pcd, dwc3_device_ep_t *ep);

dwc3_device_ep_t *dwc3_get_out_ep(dwc3_device_t *dwc3_dev, u32 ep_num);

dwc3_device_ep_t *dwc3_get_in_ep(dwc3_device_t *dwc3_dev, u32 ep_num);

dwc3_device_ep_t *dwc3_device_get_ep_by_addr(dwc3_device_t *pcd,
						      u16 index);
int dwc3_device_get_frame_number(dwc3_device_t *pcd);
int dwc3_device_isoc_ep_hiber_restart(dwc3_device_t *pcd,
					      dwc3_device_ep_t *ep);
void dwc3_device_stop(dwc3_device_t *pcd);

int dwc3_device_init(struct dwc3_core *dev);
void dwc3_device_remove(struct dwc3_core *dev);

/* dwc3-device-intr */
int dwc3_handle_dev_intr(dwc3_device_t *dwc3_dev, u32 event);
void dwc3_handle_ep_intr(dwc3_device_t *dwc3_dev, int physep, u32 event);
void dwc3_handle_connect_done_intr(dwc3_device_t *pcd);

/* dwc3-ep0 */
void dwc3_print_ep0_state(dwc3_device_t *dwc3_dev);
void dwc3_handle_ep0_xfer(dwc3_device_t *dwc3_dev, u32 event);

/* dwc3-gadget */
usb_request_t *dwc3_alloc_request(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep);
void dwc3_free_request(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req);
int dwc3_ep_queue(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req);
int dwc3_ep_dequeue(struct dwc3_core *dwc3, usb_endpoint_t *usb_ep, usb_request_t *usb_req);

dwc3_device_req_t *dwc3_gadget_get_request(dwc3_device_t *dwc3_dev, dwc3_device_ep_t *dev_ep);
void dwc3_gadget_do_setup(dwc3_device_t *dec3_dev);
int dwc3_gadget_setup(dwc3_device_t *dwc3_dev, usb_device_request_t *ctrl);
int dwc3_gadget_init(dwc3_core_t *dwc3);

int dwc3_gadget_connect(dwc3_device_t *pcd, int speed);
int dwc3_gadget_disconnect(dwc3_device_t *pcd);
int dwc3_gadget_suspend(dwc3_device_t *pcd);
int dwc3_gadget_resume(dwc3_device_t *pcd);
int dwc3_gadget_complete(dwc3_device_t *pcd, dwc3_device_ep_t *ep,
				    dwc3_device_req_t *pcd_req, int status);
void dw3_gadget_do_setup(dwc3_device_t *pcd);

void *dwc3_gadget_alloc_dma(dwc3_device_ep_t *ep, int size,
				       dwc_dma_t *mem_dma_ret);
void dwc3_gadget_free_dma(dwc3_device_ep_t *ep, int size, void *mem,
				     dwc_dma_t mem_dma);
dwc3_device_req_t *dwc_usb3_gadget_get_request(dwc3_device_t *pcd,
						       dwc3_device_ep_t *ep);
void dwc3_gadget_start_next_request(dwc3_device_t *pcd,
					       dwc3_device_ep_t *ep);
void dwc3_gadget_isoc_ep_start(dwc3_device_t *pcd,
					  dwc3_device_ep_t *ep, u32 event);
void dwc3_gadget_request_nuke(dwc3_device_t *pcd,
					 dwc3_device_ep_t *ep);
void dwc3_gadget_set_ep_not_started(dwc3_device_t *pcd,
					       dwc3_device_ep_t *ep);

/* dwc3-debug */

void dwc3_dump_global_registers(dwc3_core_t *dev);
void dwc3_dump_dev_registers(dwc3_device_t *dwc3_dev);
void dwc3_dump_ep_registers(dwc3_device_t *dwc3_dev);

/* dec3 gadget connect/disconnect*/
int usb_gadget_connect(dwc3_core_t *dwc3);
int usb_gadget_disconnect(dwc3_core_t *dwc3);


#include <arch/ops.h>
#include <stdlib.h>

#define CACHELINE_SIZE		64

/**
 * flushes the address passed till the length
 *
 * @param addr	pointer to memory region to be flushed
 * @param len	the length of the cache line to be flushed
 * @return none
 */
inline static void dwc3_flush_cache(uint32_t addr, u32 len)
{
	wmb();

	//DBG("%s addr %08lx len %d\n", __func__, (unsigned long)addr, len);
	arch_clean_invalidate_cache_range(addr, len);
}

/**
 * invalidates the address passed till the length
 *
 * @param add pointer to memory region to be invalidates
 * @param len the length of the cache line to be invalidated
 * @return none
 */
inline static void dwc3_inval_cache(uint32_t addr, u32 len)
{
	wmb();

	//DBG("%s addr %08lx len %d\n", __func__, (unsigned long)addr, len);
	arch_invalidate_cache_range(addr, len);
}

#endif // __DWC3_H
