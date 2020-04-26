package com.realtek.server;

public class JSONTAG {

	/* General Tags */
	static final String CMD_STATUS = "status";
	static final int	CMD_STATUS_CODE_SUCCESS = 0;
	static final int	CMD_STATUS_CODE_FAILURE = 1;

	/* Media Query */
	static final String RESPONSE_TYPE = "response_type";
	static final String RESPONSE_TYPE_MUSICLIST = "musiclist";
	static final String RESPONSE_TYPE_ALBUMLIST = "albumlist";
	static final String RESPONSE_TYPE_ARTISTLIST = "artistlist";
	static final String RESPONSE_TYPE_GENRELIST = "genrelist";
	static final String RESPONSE_TYPE_PHOTOLIST = "photolist";
	static final String RESPONSE_TYPE_VIDEOLIST = "videolist";
    static final String RESPONSE_TYPE_GETPROPERTY = "getproperty";
	
	static final String CMD_ERR_DESC = "error_desc";
    static final String CMD_FILE_PATH = "path";
	static final String TOTAL_ITEM = "total";
	static final String OFFSET = "offset";
	static final String NUM_OF_ITEM = "num_item";
	static final String ITEMS = "items";
	static final String ITEM_ID = "id";
	static final String ITEM_ARTIST = "artist";
	static final String ITEM_NUM_OF_SONGS = "num_of_songs";
	static final String ITEM_NUM_OF_ALBUMS = "num_of_albums";
	static final String ITEM_NAME = "name";
	static final String ITEM_DISPNAME = "disp_name";
	static final String ITEM_ALBUM = "album";
	static final String ITEM_DURATION = "duration";
	static final String ITEM_SIZE = "size";
	static final String ITEM_TITLE = "title";
	static final String ITEM_BUCKETID = "bucket_id";
	static final String ITEM_BUCKET_DISPNAME = "bucket_disp_name";	
	static final String ITEM_HEIGHT = "height";
	static final String ITEM_WIDTH = "width";
	static final String ITEM_DATE_TAKEN = "date_taken";

	/* Wifi operation */
	static final String WIFI_STATUS = "wifi_status";

	static final String WIFI_AP_LIST = "ap_list";
	static final String WIFI_AP_SSID = "ssid";
	static final String WIFI_AP_SECURITY = "security";
	static final String WIFI_AP_STRENGTH = "strength";
	
	static final String WIFI_INFO_CONNECTED = "connected";
	static final String WIFI_INFO_NETWORKID = "networkid";
	static final String WIFI_INFO_SSID = "ssid";
	static final String WIFI_INFO_SECURITY = "security";
	static final String WIFI_INFO_IP = "ip";
	static final String WIFI_INFO_SUBMASK = "submask";
	static final String WIFI_INFO_GATEWAY = "gateway";
}
