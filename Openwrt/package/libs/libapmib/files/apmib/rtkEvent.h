#ifndef RTK_EVENT_H_
#define RTK_EVENT_H_

typedef struct BASE_DATA_
{
	int wlan_idx;
	int vwlan_idx;
	int wan_idx;
	int lan_idx;
//	void* parm;
} BASE_DATA_T, *BASE_DATA_Tp;

typedef enum
{
	REINIT_EVENT_BEGIN=0,
	REINIT_EVENT_MIBCHANGE, 			//mib change
	REINIT_EVENT_APPLYCAHNGES, 			//apply change	
	REINIT_EVENT_WAN_POWER_ON = 3,		//wan power on 
	REINIT_EVENT_WAN_CONNECT =4, 		//wan connect
	REINIT_EVENT_WAN_DISCONNECT = 5,	//wan disconnect	
	REINIT_EVENT_FIREWALL	 = 6,		//firewall 		
	REINIT_EVENT_PPPOE_START = 7,		//pppoe start
	REINIT_EVENT_PPPOE_EXIT	 = 8,		//pppoe exit
	REINIT_EVENT_LAN_CONNECT = 9,		//lan connect
	REINIT_EVENT_LAN_DHCP_CONNECT = 10, //lan dhcp connect	
	REINIT_EVENT_PPTP_START = 11,		//pptp start
	REINIT_EVENT_PPTP_EXIT	 = 12,		//pptp exit	
	REINIT_EVENT_L2TP_START = 13,		//pppoe start
	REINIT_EVENT_L2TP_EXIT	 = 14,		//pppoe exit	
	REINIT_EVENT_END
} REINIT_EVENT_ID;
typedef struct EventHandleFuncItem_
{
	REINIT_EVENT_ID eventId;
	int (*event_handle)(char* data,int dataLen,char*errmsg);	
} EventHandleFuncItem_t, *EventHandleFuncItem_tp;


#endif
