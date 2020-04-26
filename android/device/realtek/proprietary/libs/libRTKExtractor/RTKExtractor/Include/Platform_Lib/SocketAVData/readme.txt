If a client application wants to play a media file but has difficulty to provide
the file path, a mechanism is proposed to transport file content to server side player.

Note :	1. some tables in the doc may not be up to date. Please refer to SocketAVData.h.
		2. you may find some FCC commands, for example GMEM/FMEM/GSSI/SSSI, in this doc.
		For more detail information, please read MediaPlayServer doc.txt.

1. Data connection protocol: 
>> url :
	url should be socket://[socket_file_name][property]
	But Netflix 3.1 is an exception, whose url is nrd3xx://[socket_file_name][property]
	
	socket_file_name - an absolute file path used to create unix socket connection
						for example, "/tmp/MediaData"
	property - give some hints of this data connection.
			Each property must be "?"-prefixed.
			Current defined properties are :			
			1) container : indicate the payload format of AccessUnit with type SOCKET_DATA_SS_PACKET.
							Current support container formats are es and ts.
							
							For example, "socket:///tmp/MediaData?container=ts"
							Then ts packets will conveyed by AccessUnit with type SOCKET_DATA_SS_PACKET.
							
							If url is "socket:///tmp/MediaData?container=es", it means what client will send
			                are elementary streams which conveyed by AccessUnit with type SOCKET_DATA_VIDEO/SOCKET_DATA_AUDIO/SOCKET_DATA_SPIC
			                and stream type is specified by AccessUnit type.
			                
			2) sharedMem : to avoid memcpy, client and server can choose a shared memory to transport data.
							Value of sharedMem is a physical address to memory header
							which is defined as follows.
							
							SOCKET_SHARED_MEM 
							{
								unsigned long   beginPhyAddr;
								unsigned long 	endPhyAddr;
								long 			size;
								unsigned long	readPtr;	//physical address
								unsigned long	writePtr;	//physical address
							}

						Note - in this way, a contigunous memory is required.
							To get this kind of memory, send "GMEM" in playback control connection
							and to release it, send "FMEM".
>> The basic syntactic element in data connection is called AccessUnit(AU), 
	of which various types are defined.

	AccessUnit 
	{
		SOCKET_DATA_HEADER	header;
		char[]				payload;
	}
	
2. AccessUnit	

	Syntax:
	SOCKET_DATA_HEADER 
	{
		SOCKET_DATA_TYPE type;		
		SOCKET_INFO_TYPE flag;
		long long        pts;		// -1 means no PTS
		long             payloadSize;
	}
	
	Semantics:
	type - indicate type of this AU. 
			Different type is corresponding to different structural payload.
			
			Table 1 - SOCKET_DATA_TYPE
			
				0 : SOCKET_DATA_NONE            //unknown data type
				1 : SOCKET_DATA_VIDEO           //video data
				2 : SOCKET_DATA_AUDIO           //audio data
				3 : SOCKET_DATA_SPIC            //sub-picture data
				4 : SOCKET_DATA_MEDIA_ATTRIBUTE //attributes about this media
				5 : SOCKET_DATA_INFO            //client private info
				6 : SOCKET_DATA_SS_PACKET       //standard specific packet
				
	flag - used to pass client private info and valid only when type is SOCKET_DATA_INFO.
			Not all private info come along with info data. 
			If necessary, write info data in the part of AU payload.
			
			Table 2 - SOCKET_INFO_TYPE
	
				0 : SOCKET_INFO_NONE  		//unknown info type
				1 : SOCKET_INFO_EOS   		// no info data
				2 : SOCKET_INFO_FLUSH 		// info data structure : NAVUSERDROP
				3 : SOCKET_INFO_IV			// initial vector : char[16]
				4 : SOCKET_INFO_KEYS		// char[2][16]
				5 : SOCKET_INFO_CLIENT_RESOLUTION	//long[2] : [0] - width, [1] - height
				6 : SOCKET_INFO_CLEAR_SCREEN	//no info data
				7 : SOCKET_INFO_PIC_INFO	// SOCKET_JPEG_PIC_INFO
				8 : SOCKET_INFO_DES_CONFIG	// info data : SOCKET_TS_DESC_CONFIG
			   11 : SOCKET_INFO_SPU_FRAME	//SOCKET_SPU_FRAME_INFO
	
			SOCKET_INFO_EOS - indicate client side has already completed streaming.
							  No info data is needed.
			SOCKET_INFO_FLUSH - For some reasons, client wants to flush data in the buffer.
								The mechanism is client sends a command "DROP" into playback connection first, 
								and then sends this kind of AU into data connection.
								Server side will drop data until encounter the AU which matchs "DROP" command.
			SOCKET_INFO_IV - if payload is encrypted, IV is required.
			SOCKET_INFO_KEYS - To decrypt payload, odd/even keys are required.
			
			These 3 info are used for S_MEDIA_TYPE_VIDEO_FJPEG:
			SOCKET_INFO_CLIENT_RESOLUTION - once client changes its resolution, server should be notified.
			SOCKET_INFO_CLEAR_SCREEN - clear screen to black
			SOCKET_INFO_PIC_INFO - information for each picture
			SOCKET_INFO_DES_CONFIG - ts descramble configuration about pes/tsp level descrambled and algorithm.
									Be sure send this info before SOCKET_INFO_IV and SOCKET_INFO_KEYS.
									If not, we will use tsp-level and AES_CBC by default.
			
	pts - a 64 bit timestamp of AU that is referenced, which is measured in units of a 90kHz clock. 
			This is valid when type is SOCKET_DATA_VIDEO/SOCKET_DATA_AUDIO/SOCKET_DATA_SPIC.
			-1 means no PTS.
			
	payloadSize - number of bytes immediately following this field.
	
	payload - this is type specific. For more detail, read paragraph 3.
	
3. payload
>>	SOCKET_DATA_MEDIA_ATTRIBUTE : refer to SOCKET_MEDIA_ATTRIBUTE
	This is expected to be the first AU.
	
	Syntax:
	SOCKET_MEDIA_ATTRIBUTE 
	{
		long              videoAttrSize;	
		SOCKET_MEDIA_TYPE videoType;
		long              pid;				// -1 means unknown
		
		long              audioAttrSize;	
		SOCKET_MEDIA_TYPE audioType;
		long              channelNum;		// -1 means unknown
		long              sampleRate;		// -1 means unknown
		long              bitsPerSample;	// -1 means unknown
		long              pid;				// -1 means unknown
		
		long              spicAttrSize;		
		SOCKET_MEDIA_TYPE spicType;
		long              pid;				// -1 means unknown
		unsigned int      spicLangCode;		// ISO-639 language code
		
		long			  contentAttrSize;
		long			  packetSize;		//packet size for ts container
		SOCKET_CONTAINER_TYPE containerType;
		SOCKET_APP_TYPE		appType;
	}
	
	Semantic:
	media attribute consists of video, audio, spic and content attribute in that order.
	If one set of them is unknown, its attrSize should be 0.
	
	When audio/spic stream is changed, this kind of AU should be sent again to 
	pass new audio/spic type. In this case, video attribute is unchanged. 
	Then how to indicate attribute is not change???
	Either pass the same attribute or just set attrSize = 0.
	
	
	videoAttrSize - number of bytes immediately following this field and 
					up to audioAttrSize. 0 means no video attribute.
					
	videoType - defined in paragraph 4.
	videoPid - For transport stream, it is used to demux video stream.
	
	audioAttrSize - number of bytes immediately following this field and 
					up to spicAttrSize. 0 means no audio attribute.

	audioType - defined in paragraph 4.			
	channelNum - number of channels. -1 means unknown.
	sampleRate - sampling rate. -1 means unknown.
	bitsPerSample -	bits per sample. -1 means unknown.
	audioPid - For transport stream, it is used to demux audio stream.
	
	spicAttrSize - number of bytes immediately following this field and 
					up to contentAttrSize.	0 means no spic attribute.
					
	spicType - defined in paragraph 4.
	spicPid - For transport stream, it is used to demux spic stream.
	
	contentAttrSize - number of bytes immediately following this field and up to
					the end of payload.	0 means no content attribute.
	packetSize - For transport stream, it means ts packet size.
	containerType - Server side supports change container type without reloading media.
					To indicate container type is not changed, just set the same
					container type or SOCKET_CONTAINER_UNCHANGED. Besides, for ts container and
					video/audio pid are unknown, set SOCKET_CONTAINER_TS_AUTO_PARSE then server
					side will parse PMT to find out video/audio pid.
					
					
	appType - Depend on appType, server side is able to do specific settings.
	bMonitorPAT - if it is set to false, once video/audio pid are found, player won't monitor 
				  PAT/PMT version change.
	
>> SOCKET_DATA_INFO
	1) SOCKET_INFO_FLUSH : 
	Syntax:
		INFO_DATA_FLUSH
		{
			long dataType;	
			long stamp;
		}
	
	Semantics:
	dataType - to indicate which type of data to drop and has the same definition as SOCKET_MEDIA_TYPE. 
	stamp - an incremental number. Server side will drop data until encounter this stamp.
	
	2) SOCKET_INFO_IV :
	Syntax :
		INFO_DATA_IV
		{
			unsigned char iv[16];
		}
	
	3) SOCKET_INFO_KEYS :
	Syntax:
		INFO_DATA_KEYS
		{
			unsigned char oddKey[16];
			unsigned char evenKey[16];
		}
	
>> SOCKET_DATA_VIDEO/SOCKET_DATA_AUDIO/SOCKET_DATA_SPIC		
	a byte sequence which contains the raw data of video/audio/spic.
	
>> SOCKET_DATA_SS_PACKET
	a byte sequence which contains packet layer data.

4. SOCKET_MEDIA_TYPE
	video type is in the region of 1 ~ 0x00000FFF 
	while audio type is in 0x00001000 ~0x00FFFFFF
	0x01000000 ~ are reserved for spic type. 
	
	Table 3 - SOCKET_MEDIA_TYPE
	
	S_MEDIA_TYPE_NONE			= 0,
	S_MEDIA_TYPE_VIDEO_VC1		= 1,
	S_MEDIA_TYPE_VIDEO_MPEG,
	S_MEDIA_TYPE_VIDEO_H264,
	S_MEDIA_TYPE_VIDEO_FJPEG,
	
	S_MEDIA_TYPE_AUDIO_MPEG		= 0x00001000,
	S_MEDIA_TYPE_AUDIO_HE_AAC,
	S_MEDIA_TYPE_AUDIO_AC3,
	S_MEDIA_TYPE_AUDIO_DDP,
	S_MEDIA_TYPE_AUDIO_LATM_AAC,
	
5. SOCKET_CONTAINER_TYPE

	Table 4 - 
	SOCKET_CONTAINER_UNCHANGED = -1,
	SOCKET_CONTAINER_TS = 0,
	SOCKET_CONTAINER_ES = 1,
	SOCKET_CONTAINER_TS_AUTO_PARSE = 2,

6. How to display spic stream?
	spic list is not ready until spicType in SOCKET_MEDIA_ATTRIBUTE is set
	and handled in server side. For this reason, make sure playback flow is at running state
	after client sending media attribute.
	
	Client side has to query spu stream info by "GSSI" to check if spu list is established. 
	Then, it can enable one of these spu streams by "SSSI".
	
	After spu stream is enabled, client is able to start to deliver spic data.
	To speicfy startTime and duration, SOCKET_INFO_SPU_FRAME is mandatory. Make sure
	SOCKET_INFO_SPU_FRAME is delivered before delivering each spic frame.