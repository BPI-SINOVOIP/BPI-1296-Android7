#ifdef CONFIG_RTL_8196B
#ifdef CONFIG_RTL8196B_TLD
 unsigned char *fwVersion="v1.4_TD";
#else
 unsigned char *fwVersion="v1.4";
#endif
#elif defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD) || defined(CONFIG_RTL_8196E)
//unsigned char *fwVersion="v3.0";
unsigned char *fwVersion="v3.2.3";
#else
 unsigned char *fwVersion="v1.2f";
#endif
#define SDK_VERSION "Realtek SDK v3.2.3"
