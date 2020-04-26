#ifndef __NETWORKSET__
#define __NETWORKSET__
// for network setting - jason
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <pthread.h>
//#include <error.h>
#include <fcntl.h>
#include <sys/time.h>

/*
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <net/if.h>
*/
#include <OSAL/Threads.h>

#define UNSUPPORTED_DONGLE_ID -1
#define NETWORKSET_NAME_LEN	32
#define NETWORKSET_BUF_LEN	512

#define WIFI_FREQ_24G_VALUE  (2.4)
#define WIFI_FREQ_50G_VALUE  (5.0)

#define NETSET_ETH0_MANUAL_START_SERVER     0x00000001 // use manual start server -- this is first priority
#define NETSET_WLAN0_MANUAL_START_SERVER    0x00000002
#define NETSET_ETH0_THREAD_RUNNING          0x00000004 // Launch a thread to start device
#define NETSET_WLAN0_THREAD_RUNNING         0x00000008
#define NETSET_ETH0_SHUTDOWN                0x00000010 // Set to device shutdown
#define NETSET_WLAN0_SHUTDOWN               0x00000020
#define NETSET_RESTART_SERVER          0x00000040 // device to restart external server ex: smbd,btpd
#define NETSET_WLAN0_WIFI_AP_CONNECTING     0x00000080 // Start to connecting to WIFI Ap

#if AP_CONFIG_WITH(APCFG_USE_WIFI_WPS)
#define NETSET_WLAN0_SET_WPS_MODE           0x000000100
#endif
#define NETSET_WLAN0_GET_WPA_STATUS         0x000000200

#define NETSET_PPP0_MANUAL_START_SERVER     0x00001000 // use manual start server -- this is first priority
#define NETSET_PPP0_THREAD_RUNNING          0x00002000 // Launch a thread to start device
#define NETSET_PPP0_SHUTDOWN                0x00004000 // Set to device shutdown

#define NETSET_CONNECT_AP					0x00010000 // When DUT is connecting AP, ignore linkup.
#define NETSET_KEEPALIVE_THREAD_RUNNING     0x00020000 // Keepalive thread is running.
#define NETSET_WIFI_SOFTAP_PROCESSING		0x00040000 // When apply bridge and restart SoftAp to new setting is processing.


#if defined(ENABLE_SMART_DISPLAY_STAND_ALONE) && !defined(ENABLE_AIR_FUN) && defined(REALTEK_WIDI)
#define REALTEK_PURE_WIDI
#endif

typedef void * HANDLE;
typedef int (* GetIpCallBackFunction)(void *ptThis);

typedef enum net_mode_type
{
//#ifdef ENABLE_WIFI_SOFT_AP  
	NET_MODE_SOFTAP_ONLY 	= 0,
	NET_MODE_STA			= 1,
//#else  
	NET_MODE_PLAYER 		= 2, // pure wifi connection, no softAp
//#endif	//ENABLE_WIFI_SOFT_AP
//#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI)
	NET_MODE_P2P_ONLY		= 3,
//#ifdef WIFI_CONCURRENT	
	NET_MODE_P2P_CONCURRENT = 4,
//#endif //WIFI_CONCURRENT
//#endif //defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI)
//#ifdef REALTEK_WIDI
	NET_MODE_WIDI 			= 5,	
//#endif //REALTEK_WIDI	
} ENUM_NET_MODE ;

#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
typedef enum p2p_concurrent_state_type
{
	STATE_P2P_CONCURRENT = 0,
	STATE_P2P_BUSY, 
	STATE_NET_BUSY, 
} ENUM_P2P_CONCURRENT_STATE;
#endif 

typedef enum  type_wifi_model {
	WIFI_UNKNOWN = 0,
	WIFI_11G,
	WIFI_11N
}ENUM_WIFI_MODEL;

typedef enum type_wifi_freq{
	WIFI_FREQ_24G=0,
	WIFI_FREQ_50G=1
}ENUM_WIFI_FREQ;

typedef enum _net_region {
    NET_REGION_NONE,
    NET_REGION_ARGENTINA,
    NET_REGION_AUSTRALIA,
    NET_REGION_AUSTRIA,
    NET_REGION_BELGIUM,
    NET_REGION_BRAZIL,
    NET_REGION_CHINA,
    NET_REGION_CZECH,
    NET_REGION_DENMARK,
    NET_REGION_FINLAND,
    NET_REGION_FRANCE,
    NET_REGION_GERMANY,
    NET_REGION_GREECE,
    NET_REGION_HUNGARY,
    NET_REGION_IRELAND,
    NET_REGION_ITALY,
    NET_REGION_JAPAN,
    NET_REGION_KOREA,
    NET_REGION_LUXEMBOURG,
    NET_REGION_NETHERLANDS,
    NET_REGION_NORWAY,
    NET_REGION_POLAND,
    NET_REGION_PORTUGAL,
    NET_REGION_RUSSIA,
    NET_REGION_SLOVAKIA,
    NET_REGION_SLOVENIA,
    NET_REGION_SPAIN_PENINSULA_Y_BALEARES,
    NET_REGION_SPAIN_CANARIAS,
    NET_REGION_SWEDEN,
    NET_REGION_SWITZERLAND,
    NET_REGION_TAIWAN,
    NET_REGION_TURKEY,
    NET_REGION_UNITED_KINGDOM,
    NET_REGION_USA,
    NET_REGION_NUMBER
} ENUM_NET_REGION;

typedef enum type_net_interface{
	ETH0,
	WLAN0,
	WLAN1,
	PPP0,
	BR0,
	NET_INTERFACE_NUM,
} ENUM_NET_INTERFACE;

typedef struct _IPV4 {
	unsigned char		ucIP[4];
} IPV4;

typedef struct _IPV4_2 {
	int ip[4];
} IPV4_2;

typedef enum wireless_security {
    WL_SECURITY_OPEN	= 0,
    WL_SECURITY_WEP	= 1,
    WL_SECURITY_WPA	= 2,
    WL_SECURITY_WEP_SHAREKEY = 3,
    WL_SECURITY_UNKNOWN	= 4,
} ENUM_WIRELESS_SECURITY;

typedef enum wireless_mode {
    WL_MODE_INFRASTRUCTURE	= 0,
    WL_MODE_P2P= 1,
#if AP_CONFIG_WITH(APCFG_USE_WIFI_WPS)
    WL_MODE_WPS=2,
    WL_MODE_WCN=3
#endif
} ENUM_WIRELESS_MODE;

#ifdef ENABLE_WIFI_SOFT_AP
typedef enum softAp_mode {
    MODE_SOFT_AP_OFF		= 0,
    MODE_SOFT_AP_ONLY		= 1,
#ifdef ENABLE_WIFI_BRIDGE_MODE    
    MODE_SOFT_AP_BRIDGE		= 2,
#endif    
} ENUM_SOFT_AP_MODE;
#endif //ENABLE_WIFI_SOFT_AP

#ifdef ENABLE_WIFI_BRIDGE_MODE
typedef enum _bridge_interface {
    SETUPCLASS_BRG_TO_WLAN = 0,
    SETUPCLASS_BRG_TO_ETH = 1,
    SETUPCLASS_BRG_DISABLE = 2,
} EMUN_BRIDGE_INTERFACE;
#endif //ENABLE_WIFI_BRIDGE_MODE


typedef enum {
	CB_DEVICE_PLUG_IN,
	CB_DEVICE_PLUG_OUT,
	CB_MANUAL_START_DEVICE,
	CB_POST_MANUAL_START_DEVICE,
}NETWORKSET_CB_API;

// network
typedef enum {
	IP_TYPE_IP,
	IP_TYPE_SUBMASK,
	IP_TYPE_GATEWAY,
	IP_TYPE_DNS,
} ENUM_NET_IP_TYPE;

#define JDBG_DBG 1

#ifdef JDBG_DBG
	#define JDBG(format, arg...) printf("%s: %d " format, __func__, __LINE__, ## arg)
	#if JDBG_DBG == 2
		#define JDBG2(format, arg...) printf("%s: %d " format, __func__, __LINE__, ## arg)
	#else
		#define JDBG2(format, arg...)
	#endif
#else
	#define JDBG(format, arg...)
	#define JDBG2(format, arg...)
#endif

class linklist
{
    private:

        struct node
        {
            char *data; // mount point
            char *cExport; // cExport share point
            char *cUserID; // cExport share point
            char *cPassword; // cExport share point
            node *link;
        }*p;

    public:

        linklist();
        void append( char *dd , char *cExport,char *cUserID,char *cPassword);
        void add_as_first( char *dd , char *cExport,char *cUserID,char *cPassword);
        void addafter( int c, char *dd , char *cExport,char *cUserID,char *cPassword);
        void delAll();
        void del( char *dd );
        int DataInLinkList( char *dd , char *cExport,char *cUserID,char *cPassword);
//        char *getData(int c);
        int getData(int c,char **cMP,char **cExport,char **cUserID,char **cPassword);
        int count();
        ~linklist();
};

class NetPing{
public:
    NetPing();
    ~NetPing();
    int send_packet(IPV4 *dest);
    int send_packet(IPV4 *dest,int iPack_num);
    int recv_packet(void);
    int recv_packet(int Wait_msec);
private:
    pid_t pid;
    int sockfd;
    struct sockaddr_in dest_addr;
    int pack(int pack_no,char *data);
    unsigned short cal_chksum(unsigned short *addr,int len);
};

enum ENUM_WPA_STATUS
{
    WPA_STATUS_UNKNOWN  = -1,
    WPA_STATUS_DISCONNECTED  = 0,
    WPA_STATUS_INACTIVE  = 1,
    WPA_STATUS_SCANNING  = 2,
    WPA_STATUS_ASSOCIATING  = 3,
    WPA_STATUS_ASSOCIATED  = 4,
    WPA_STATUS_4WAY_HANDSHAKE  = 5,
    WPA_STATUS_GROUP_HANDSHAKE = 6,
    WPA_STATUS_COMPLETED  = 7,
    WPA_STATUS_NUM = 8,
};

enum ENUM_BRG_TO_ETH_CHECK_RESULT
{
	NETWORKSET_BRG2ETH_RESULT_OK,
	NETWORKSET_BRG2ETH_RESULT_FIX_IP,
	NETWORKSET_BRG2ETH_RESULT_ETH_UNPLUGGED,
	NETWORKSET_BRG2ETH_RESULT_ETH_ROUTER_DEAD
};

typedef struct _wpa_status
{
    ENUM_WPA_STATUS status;
    const char *name;
} WPA_STATUS;
const WPA_STATUS const_wpa_status[] ={
    { WPA_STATUS_DISCONNECTED,"DISCONNECTED"},
    { WPA_STATUS_INACTIVE,"INACTIVE"},
    { WPA_STATUS_SCANNING,"SCANNING"},
    { WPA_STATUS_ASSOCIATING,"ASSOCIATING"},
    { WPA_STATUS_ASSOCIATED,"ASSOCIATED"},
    { WPA_STATUS_4WAY_HANDSHAKE,"4WAY_HANDSHAKE"},
    { WPA_STATUS_GROUP_HANDSHAKE,"GROUP_HANDSHAKE"},
    { WPA_STATUS_COMPLETED,"COMPLETED"}
};
typedef struct _WiFiChannelTable
{
	int Ch;
	const char *GHz;
}WiFiChannelTable;
const WiFiChannelTable const_WiFiTable[]={
	{1,"2.412"},//0
	{2,"2.417"},
	{3,"2.422"},
	{4,"2.427"},
	{5,"2.432"},
	{6,"2.437"},
	{7,"2.442"},
	{8,"2.447"},
	{9,"2.452"},
	{10,"2.457"},
	{11,"2.462"},
	{12,"2.467"},
	{13,"2.472"},
	{14,"2.484"},
	{34, "5.17"},
	{36, "5.18"},
	{38, "5.19"},
	{40, "5.2"},
	{42, "5.21"},
	{44, "5.22"},
	{46, "5.23"},
	{48, "5.24"},
	{52, "5.26"},
	{56, "5.28"},
	{60, "5.3"},
	{64, "5.32"},
	{100, "5.5"},
	{104, "5.52"},
	{108, "5.54"},
	{112, "5.56"},
	{116, "5.58"},
	{120, "5.6"},
	{124, "5.62"},
	{128, "5.64"},
	{132, "5.66"},
	{136, "5.68"},
	{140, "5.7"},
	{149, "5.745"},
	{153, "5.765"},
	{157, "5.785"},
	{161, "5.805"},
	{165, "5.825"}//41
};
const int WiFiChannelMap = 41;
enum ENUM_WPS_MODE
{
    WPS_MODE_NONE  = 0,
    WPS_MODE_ENABLE  = 1,
    WPS_MODE_PBC  = 2,
    WPS_MODE_PIN  = 3
};

typedef struct _iface_info{
	const char *name;
	ENUM_NET_INTERFACE num;
	const char *dhcp_ok;
	const char *dhcp_pid;
	const char *linkstate_file;
	const char *dhcpd_pid;
	const char *dhcpd_conf;
} iface_info;

typedef struct _device_plugin_callback{
	ENUM_NET_INTERFACE dev;
	int test;
	unsigned char ip[4];
} NetworkSetCallBackParam;

class NetworkSet{
private:
	
	static int (*DevicePlugInHandle) (int, void *);
	static int (*DevicePlugOutHandle) (int, void *);
	static int (*ManualStartDeviceHandle) (int, void *); 
	static int (*PostManualStartDeviceHandle) (int, void *); 
	static int (*DestoryNetworkFlowHandle) (int, void *); 
	
public:
	void RegisterNetworksetCallBackAPI(int (*HandleAPI)(int, void*), NETWORKSET_CB_API APIType);
	int GetDeviceUp(ENUM_NET_INTERFACE enInterface);
	int get_best_channel(double freq, int dongle_id);
/*
return :    0 - up; not 0 - down
*/
// Adhoc
	int start_dhcpd(ENUM_NET_INTERFACE);
	int killProcessFromPidFile(const char *PidFile);
	int killWpaSupplicantProcess(void);
	int sendRestartServer(void);

    int StartDevice(ENUM_NET_INTERFACE enInterface);
	void stopPPPOE(void);
    int startPPPOE(void);
    int setPPPOEConfigure(void);
    int setPPPOEDNS(void);
	int set_net_mode(ENUM_NET_MODE net_mode_in);

	bool isPPPoeComplet(void);
	int getRetryPPPOE(void);
	void resetRetryPPPOE(void);
	void addRetryPPPOE(void);
    int getPPPOEStatus(void); // 0 : ok , > 0 : error , < 0 : internal error
    
    void setDeviceLinkState(ENUM_NET_INTERFACE enInterface,bool isUp) {m_isDeviceLinkUp[enInterface]=isUp;}
   	bool getDeviceLinkState(ENUM_NET_INTERFACE enInterface) { return m_isDeviceLinkUp[enInterface];}
    bool getIsDeviceLinkUp(ENUM_NET_INTERFACE enInterface);
    int manualStartDevice(ENUM_NET_INTERFACE enInterface);  // for main thread to start Network device
    int postManualStartDevice(ENUM_NET_INTERFACE enInterface);
	void cancelStartSoftAp();
    int wlan0Activate();
    int wlan0Deactivate();
    int setFixDNS(ENUM_NET_INTERFACE enInterface);
	//int initialSet(ENUM_NET_INTERFACE enInterface);
	int clearInitialSetting();
    int launchThreadToStartDevice(bool killPreThread=false);
    int launchThreadToStartDevice(bool killPreThread,ENUM_NET_INTERFACE enInterface); // to folk a thread to start Network device
    int StopDevice(ENUM_NET_INTERFACE enInterface,bool stopBackgroundProcess=false);
    int StopDhcp(ENUM_NET_INTERFACE enInterface);
    int stop_dhcp_client(ENUM_NET_INTERFACE iface);
	bool isNetworkConnected();
	bool isNetworkProcessing();
    bool isStartDeviceProcessing(ENUM_NET_INTERFACE enInterface);
	bool isConnectingWifiAp(void);
    bool isDeviceOnBoard(ENUM_NET_INTERFACE enInterface);	
	bool isSupportConcurrentMode();
	int getDongleSoftApDelayTime();
	
    ENUM_WIFI_MODEL getWifiDeviceModel();
//    int SetNetDHCP(void);
    int SetNetDHCP(ENUM_NET_INTERFACE enInterface);
    int connectToWiFiAp(bool use_tmp=false);
	bool setWlanIP();
	void setFixedSoftAPIp();
	void backupDNSFile();
	void restoreDNSFile();
    int purgeDNSCache();
	void refreshWpaState(void);
    int IsSetDHCPOk(void);
    int IsSetDHCPOk(ENUM_NET_INTERFACE enInterface);
    int SetNetDNSIP(IPV4 *ipDnsIP);
    int GetNetDNSIP(ENUM_NET_INTERFACE enInterface,IPV4 *ipDnsIP);

//    int SetNetFixIP(IPV4 *ipIP,IPV4 *ipSubNet,IPV4 *ipGateway,IPV4 *ipDnsIP) ;
    int SetNetFixIP(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP,IPV4 *ipSubNet,IPV4 *ipGateway) ;
    int GetNetFixIP(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP,IPV4 *ipSubNet,IPV4 *ipGateway);
//    int GetSysNetIP(IPV4 *ipIP);
	int getIPAddr(ENUM_NET_INTERFACE enInterface, ENUM_NET_IP_TYPE type, IPV4_2 *ip);
    int GetSysNetIP(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP);
    int GetSysNetMask(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP);
    int GetSysMacAddr(ENUM_NET_INTERFACE enInterface,sockaddr *sa);
    int GetSysNetGateway(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP);

    int SetMountPoint(char *cMP,char *cExportP,char *cUserID,char *cPassword, char *domain);
    int SetUnMountPoint(char *);
    int UnMountAllDisk();
    int UnMountUselessDisk();
//    int SetDefaultInterface(ENUM_NET_INTERFACE enInterface,char *essid);
// Scan and get Ssid
	void removeWifiApListCache();
	bool isWifiApListCacheValid();
    int StartNetSsidScan(ENUM_NET_INTERFACE enInterface);
    int IsNetSsidScanOk(void);
    HANDLE SelectNetSsidScanResult(int *num);
/*
wps : 
    0 - not wps mode
    1 - wps mode
    2 - wps push button mode
    3 - wps pin mode
*/
    int GetNetSsidScanResult(HANDLE handle,int num,char **ssid,int *security,int *level=NULL,char **bssid=NULL,ENUM_WPS_MODE *wps=NULL);
    int ReleaseNetSsidScanResult(HANDLE handle);
    int ToPing(IPV4 *ip);
    int ToPing(IPV4 *ip,int iSendNum,int iWait_msec);
    int MediaPath2FilePath(char *mediapath,char *filepath,int iMaxLen);
    int SetDefaultWiFiApName(int profile);
/*
return :
    0   : Yes
    1   : No
    -1  : Fail
*/
    int IsWPA_AP(ENUM_NET_INTERFACE enInterface,char *AP);
    int WiFiSignalStrength(ENUM_NET_INTERFACE enInterface,char *AP);
    int getActiveDeviceSignalInfo(ENUM_NET_INTERFACE enInterface,int *strength,int *noise=NULL,int *quality=NULL);
/*
return :
    0   : Last Ping Ok
    1   : Last Ping Timeout
    -1  : Device Fail
*/
    int TriggerPingGatewayAndGetLastPingResult(ENUM_NET_INTERFACE enInterface);
    int setActiveWepKey(ENUM_NET_INTERFACE enInterface,int key);
    int registerGetIpCallBack(GetIpCallBackFunction function,void *ptThis=NULL);
    int resetDriver(ENUM_NET_INTERFACE enInterface);
    ENUM_WPA_STATUS GetWL_WPAStatus (bool bCheck_Completed=true,bool bGetRealtime=false);
    char *getWL_WPAStatusDesc(ENUM_WPA_STATUS status);
#ifdef ENABLE_WIFI_BRIDGE_MODE	
	int addBridgeInterface();
	int delBridgeInterface();
	void prepareBridgeChange();
	int apply_bridge_target_change();	
	ENUM_BRG_TO_ETH_CHECK_RESULT check_brg_to_eth();	
#endif	
	ENUM_NET_INTERFACE getCurrentEthIface();
	ENUM_NET_INTERFACE getCurrentWlanIface();
	ENUM_NET_INTERFACE getCurrentSoftAPIface();
	ENUM_NET_INTERFACE getCurrentP2PIface();
	void setCurrentEthIface(ENUM_NET_INTERFACE iface);
	void setCurrentWlanIface(ENUM_NET_INTERFACE iface);
	void setCurrentSoftAPIface(ENUM_NET_INTERFACE iface);
	void setCurrentP2PIface(ENUM_NET_INTERFACE iface);

	void resetIface(ENUM_NET_INTERFACE iface);
	void setIfaceUp(ENUM_NET_INTERFACE iface);
	void setIfaceDown(ENUM_NET_INTERFACE iface);

	bool hasNetState(uint32_t state);
	void setNetState(uint32_t state);
	void resetNetState(uint32_t state);	
	uint32_t getNetState();

	void setWlan0Ready(bool set);
	void setWlan1Ready(bool set);
	bool isWirelessReady();

	int checkDongleId();
	int getDongleId();
	void resetDongleId();	

	void addDefaultRoute(ENUM_NET_INTERFACE iface, IPV4 *ipAddr);
	void delDefaultRoute(ENUM_NET_INTERFACE iface);
		
#if AP_CONFIG_WITH(APCFG_USE_WIFI_WPS)
    int getWPSPinNum(char **pin,bool generate=true);
    int unlockWPSMode(void);
    int lockWPSMode(void);
    int setWPSMode(ENUM_WPS_MODE mode);
	void resetWPSMode();
    ENUM_WPS_MODE getWPSMode(void){return m_wps_realtime_mode;}
    int saveWPSConf(int profile);
    int restoreWPSConf(int profile);
    int copyWPSConf(int fromId, int toId);
    int resendWPSCommand();
#endif	
#ifdef ENABLE_WIFI_SOFT_AP
	bool isSoftApReady();
	int stopSoftAP();
	int stopDHCPD_WLAN0();
	//void stopDhcpd();
	int stopDHCPD(ENUM_NET_INTERFACE);
    int stop_dhcp_daemon(ENUM_NET_INTERFACE iface);
    int startSoftAP(bool delayStart=true);
	void startDhcpd();
#endif //ENABLE_WIFI_SOFT_AP
	int GetInterfaceWorkingChannel(ENUM_NET_INTERFACE iface);
	int setWirelessDisconnect();
	int getWirelessCurrentAP(int, char*);
	bool isDongleSupportBridge();
	bool isDongleSupportSoftAP();
	bool isDongleSupportWIDI();
	bool isDongleSupportDualBand();
	int getSysIPAddress(ENUM_NET_INTERFACE enInterface, ENUM_NET_IP_TYPE type, IPV4_2 *ip);

	int update_iface_info();	

    int getDataFromWPAConf_needfree(char **ApName, char **Security);
    static NetworkSet *GetInstance(void)
    {
        osal_MutexLock(&p.m_instance_mutex);
        if(p.m_ptr) 
        {
            osal_MutexUnlock(&p.m_instance_mutex);
            return p.m_ptr;
        }
        else
        {
            p.m_ptr = new NetworkSet();
            osal_MutexUnlock(&p.m_instance_mutex);
            return(p.m_ptr);
        }
        osal_MutexUnlock(&p.m_instance_mutex);
    }
    static void DeleteInstance(void) {
        osal_MutexLock(&p.m_instance_mutex);
        if(p.m_ptr)
        {
            delete p.m_ptr;
            p.m_ptr=0;
        }
        osal_MutexUnlock(&p.m_instance_mutex);
    };

    friend class AutoPtr;


#ifndef WIN32
private:
#endif
    typedef struct _SYSNETIP
    {
        ENUM_NET_INTERFACE NetInterface;
        void *pInstance;
    } SYSNETIP;

	typedef struct _keepalive_thead_param {
		NetworkSet *p_this;
		int start_time;
	} keepalive_thread_param;

    NetworkSet();
    ~NetworkSet();
    class AutoPtr
    {
        public:
            NetworkSet *m_ptr;
            osal_mutex_t m_instance_mutex;            
            AutoPtr(){osal_MutexCreate(&m_instance_mutex); m_ptr=0;}
            ~AutoPtr(){if(m_ptr) delete m_ptr; m_ptr = NULL;osal_MutexDestroy(&m_instance_mutex);}
    };
    static AutoPtr p;

    static uint32_t m_state;
    bool m_wlan0Activate;
	osal_mutex_t m_getIp_mutex;
	osal_mutex_t m_wlanActivate_mutex;
	osal_mutex_t m_getWpaState_mutex;

    linklist *ll;
    linklist *ll_ssid;

    bool m_isDeviceLinkUp[NET_INTERFACE_NUM];
    int CheckMountPoint(char *cMP); // to get /proc/mounts about cMP in there
// Get NetSysIP
    pthread_t ptNetSysIP[NET_INTERFACE_NUM];
    bool bNetSysIP_ThreadStart[NET_INTERFACE_NUM];
    bool bNetSysIP_Result[NET_INTERFACE_NUM];
    unsigned char ucGetNetSysIPCount[NET_INTERFACE_NUM];
    IPV4 NetSysIP[NET_INTERFACE_NUM];
    SYSNETIP m_sysObj[NET_INTERFACE_NUM];
	//for-> int TriggerPingGatewayAndGetLastPingResult();
	bool bPingGateway_Result[NET_INTERFACE_NUM];
	bool bPingGateway_ThreadStart[NET_INTERFACE_NUM];
	pthread_t net_ping_gateway_thread_id[NET_INTERFACE_NUM];
	static void *net_ping_gateway_thread(void *param);
    static void *GetIP_thread(void *param);
    int BlockGetSysNetIP(ENUM_NET_INTERFACE enInterface,IPV4 *ipIP);
// check start up status
/*
return :    0 - up
         not 0 - down
*/
    int GetWLApStatus (ENUM_NET_INTERFACE enInterface);
// WPA function
    int genWL_DefWPAconf ();
	static int executeCmd(const char** cmdArray, int arraynum);
	
    GetIpCallBackFunction m_CallBackfunction;
    void *m_pCallBackVoid;
    ENUM_WPA_STATUS m_wpa_status;
	int m_pppoe_status;
	int m_retryPPPOE;
    time_t m_time2call_wpa_status;
#if AP_CONFIG_WITH(APCFG_USE_WIFI_WPS)
    ENUM_WPS_MODE m_wps_realtime_mode;
    char m_wps_realtime_pin[16];
    char *m_last_time_wps_cmd;
#endif
    ENUM_WIFI_FREQ getWiFiSuggestFreq(void);
	ENUM_WIFI_FREQ get_wifi_freq(ENUM_NET_INTERFACE);
	int ssid_psk_str_transform(char *old_str, char *new_str);
	void create_iface_info_list();
	bool kernel_support_bridge; 
	bool dongle_support_bridge;
	bool dongle_support_concurrent;
	bool init_flag;
	bool first_wifi_hotplug_flag;
	bool wlan0_ready;
	bool wlan1_ready;
	bool enable_dual_band;
	//int net_mode;
	int m_dongle_id;
#ifdef ENABLE_WIFI_BRIDGE_MODE	
	int previous_brg_target;
#endif//ENABLE_WIFI_BRIDGE_MODE
	char CCRMOD_ETH[NETWORKSET_NAME_LEN];
	char CCRMOD_LAN[NETWORKSET_NAME_LEN];
	char CCRMOD_WAN[NETWORKSET_NAME_LEN];
	char CCRMOD_P2P[NETWORKSET_NAME_LEN];
	ENUM_NET_INTERFACE ccrmod_eth_num;
	ENUM_NET_INTERFACE ccrmod_softAp_num;
	ENUM_NET_INTERFACE ccrmod_wan_num;
	ENUM_NET_INTERFACE ccrmod_p2p_num;

	iface_info iface_info_list[NET_INTERFACE_NUM];
	iface_info *current_wlan;
	iface_info *current_eth;
	iface_info *current_softAp;
	iface_info *current_p2p;
};

typedef struct _dongle_info{
	char name[NETWORKSET_NAME_LEN];
	char proc_file[NETWORKSET_BUF_LEN];
	ENUM_WIFI_MODEL wifi_model;
	ENUM_WIFI_FREQ wifi_freq;
	bool dual_band_support;
	bool brg_support;
	bool softAP_support;
	bool WIDI_support;
	int  softAP_delay_time;
} dongle_info;
#endif
