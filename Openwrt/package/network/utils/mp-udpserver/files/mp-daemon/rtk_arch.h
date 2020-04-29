
#ifndef _RTK_ARCH_H_
#define _RTK_ARCH_H_

#ifndef MAX_PATH
#define MAX_PATH	256
#endif

//=========== web , auth ,iwcontrol & wscd use ===========
#define TOP_CONFIG_DIR		"/var"

//=========== web , flash  & wapi use ===========
#define WIFI_CONFIG_ROOT_DIR	"/var/rtl8192c"

//=========== web use ===========
#define WIFI_SCRIPT_DIR		"/root/script"

//=========== wapi and wscd use ===========
// the path of wifi daemon will use the path if $(WIFI_CONFIG_ROOT_DIR)/wifi_bin_dir don't exist. 
#define WIFI_BIN_DIR		"/root"




//=========== openssl daemon use ===========
//The parameter detide the path of openssl daemon and openssl.cnf 
#define OPENSSL_DIR			WIFI_BIN_DIR
//==========================================

#define WIFI_WPS_TMP_DIR	"/tmp"
#define WIFI_MP_TMP_FILE	"/tmp/MP.txt"

//=========== wapi daemon use ===========
#define ECDSATEST		WIFI_BIN_DIR"/ecdsatest"
#define OPENSSL			WIFI_BIN_DIR"/openssl"
#define WIFI_WAPI_TMP_DIR	"/var/tmp"
#define WIFI_WAPI_CERT_DIR	"/var/myca"
//=======================================

#define dump_cmd  {int iii; printf("%s cmd=",__FILE__);	for( iii=0 ; iii<argc ;iii++ ) 		printf("%s ",argv[iii]);	printf("\n");}
#define DAEMON_VERSION	"svn:( date:2015-07-08 )"

#endif // _RTK_ARCH_H_

